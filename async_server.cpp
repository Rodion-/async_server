#include "async_server.h"

void connection :: set_bridge( server* s )
{
	bridge_to_server = new bridge( s , this ); 
}

void connection :: read ( void )
{
	auto self( shared_from_this() );

	socket_.async_read_some(	boost::asio::buffer( data , MAX_SIZE ),
				 	[ this , self ]( boost::system::error_code ec, std::size_t length )
        	    		 	{
        	    		 		if ( !ec )
          					{             						
          						cmd.cmd_decoder( data , length , mtx );         						
          						
          						char msg[ 100 ] = { "Your id : " };
          						std::string s = std::to_string(id);
							char const *pchar = s.c_str();  
          						
          						memcpy( &msg[ 11 ] , pchar , s.size() );
          						memcpy( &msg[ 11 + s.size() ] , "\n" , 1 );
          						
          						int msg_len = 0;
          						cmd.get_data( &msg[ 11 + s.size() + 1 ] , &msg_len );
          						
          						write_ack( msg , msg_len + 13 );						
          					}
          					else
          					{
	          					if( ec.value() == 2 )
        	  					{
        	  						std::cout<<"client id : " << id << " disconnect "<<std::endl;
        	  						bridge_to_server->erase( id );
        	  					}
        					}
        				}
        			);
}

void connection :: write_ack ( char* data_ , int len )
{
	auto self(shared_from_this());

	memcpy( data , data_ , len );
		
      	std::cout<<std::endl;
         		
	boost::asio::async_write(	socket_,
					boost::asio::buffer(data, len ),
      					[ this , self ]( boost::system::error_code ec , std::size_t /*length*/ )
      					{
      						std::cout<<"async_write2 ec : "<<ec.value()<<std::endl;
         	
      						if (!ec)
      						{	
      							std::cout<<std::endl;
      							read();
						}
      	 				}
      	 			);
}

void connection :: write_echo ( int len )
{
	auto self( shared_from_this() );

      	std::cout<<std::endl;
          		
	boost::asio::async_write(	socket_,
					boost::asio::buffer(data, len ),
      					[ this , self ]( boost::system::error_code ec , std::size_t /*length*/ )
      					{
      	  					if (!ec)
      	  					{	
      	  						std::cout<<std::endl;
							read();
						}
      					}
      				);
}


//-------------------  

server :: server( boost::asio::io_context& io_context, short port ) : acceptor_( io_context , tcp::endpoint( tcp::v4(), port ) )
{
	do_accept();
}

void server :: do_accept()
{
	acceptor_.async_accept(	[ this ]( boost::system::error_code ec , tcp::socket socket ) 
        				{
          					if ( !ec )
          					{                         						      						
          						client_pool.insert( std::pair<int, std::shared_ptr< connection  > > ( counter ,  std::make_shared< connection >(std::move(socket) , counter ) ) );
          						
          						client_pool.at( counter )->set_bridge( this );
          						          						
          						client_pool.at( counter )->start();
          						          						
          						counter++;
						}
						do_accept();
					}
				);	
}

void server :: erase ( int id )
{   			
	client_pool.erase( id );
 			
	std::cout<<"erase id : " << id << " client_pool.size : " << client_pool.size() << std::endl;   			
}

//---------------------TESTS--------------------------------

serverTests :: serverTests( boost::asio::io_context& io_context, short port )
{
	do_accept();
}

void serverTests :: do_accept()
{
	client_pool.insert( std::pair<int, std::shared_ptr< connectionTests  > > ( counter ,  std::make_shared< connectionTests >( counter ) ) );
        						
	client_pool.at( counter )->set_bridge( this );
          						          						
	client_pool.at( counter )->start();
          						          						
	counter++;	
}

void serverTests :: erase ( int id )
{   			
	client_pool.erase( id );   			
}

void connectionTests :: set_bridge( serverTests* s )
{
	bridge_to_server = new bridgeTests( s , this );
}


TEST( serverTests , connect )
{
	//	ARRANGE
	
	boost::asio::io_context io_context;
	
	int port = 1234;
	
	//	ACT
	
	serverTests server(io_context, port );
	
	for( int i = 0; i < 100; i++ )
	{
		server.connect();
	}
	
	//	ASSERT
    	
    	EXPECT_EQ( server.client_pool_size() , 101 );
}

TEST( serverTests , erase )
{
	//	ARRANGE
	
	boost::asio::io_context io_context;
	
	int port = 1234;
	
	//	ACT
	
	serverTests server(io_context, port );
	
	server.erase( 0 );
	
	//	ASSERT
    	
    	EXPECT_EQ( server.client_pool_size() , 0 );
}
