#include "async_server.h"

boost::asio::io_context io_context;

void server_run ( void )
{
	io_context.run();
}

int main ( int argc , char** argv )
{
	
	#if TEST_RUN 
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
	#endif
		
	try
	{
		std::cout<<"async server"<<std::endl;

		if (argc < 2)
   		{
      			std::cerr << "Entre port\n";
      		}
      		else
      		{
      		}
      		
    		server server(io_context, std::atoi(argv[1]));
    		    		
    		std::thread t1( server_run );
    		std::thread t2( server_run );
    		std::thread t3( server_run );
    		std::thread t4( server_run );
    		
    		t1.join();
    		t2.join();
    		t3.join();
    		t4.join();   		
	
	}
	catch( int e )
	{
		 std::cout<<"error "<<e<<std::endl;
	}
		
	return 0;
}
