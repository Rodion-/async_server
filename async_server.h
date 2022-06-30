#ifndef	ASYNC_SERVER
#define	ASYNC_SERVER

#include "command.h"

#include <iostream>

#include <ctime>
#include <chrono>

#include <boost/asio.hpp>

#include <thread>
#include <mutex>

#include <map>

#include <gtest/gtest.h>

using boost::asio::ip::tcp;

class bridge;
class server;

class connection : public std::enable_shared_from_this< connection >
{	
	enum DATA_BUFF
	{
		MAX_SIZE = 1000
	};
	
	int id;
	
	char data[ MAX_SIZE ];
	
	tcp::socket socket_;
	
	bridge* bridge_to_server;

	Command cmd;

	std::mutex mtx;
	
	public:
	
	connection(tcp::socket socket , int id_ ) : socket_(std::move(socket)) , id( id_ ) {}
	~connection(){}
	
	connection( const connection& ) = delete;
	connection( const connection&& ) = delete;
	connection& operator= ( connection& ) = delete;
	connection& operator= ( connection&& ) = delete;
	
	void set_bridge( server* s );
	
	void start( void ) { read(); }
	
	void read ( void );
	
	void write_echo ( int len );
		
	void write_ack ( char* data_ , int len );
};

class server
{
	tcp::acceptor acceptor_;
	
	int counter = 0;
	
	std::map< int , std::shared_ptr < connection  > > client_pool;
		
	public:
		server( boost::asio::io_context& io_context , short port );
		~server(){}
		server( const server& ) = delete;
		server( const server&& ) = delete;
		server& operator= ( const server& ) = delete;
		server& operator= ( const server&& ) = delete;
   		
   		void erase ( int id );

	private:
		void do_accept();				
};

class bridge
{	
	server* s;
	connection* c;
	
	public:
		
	bridge(server* s_ , connection* c_ ) : s( s_ ) , c( c_ ){}
	~bridge(){}
	bridge( const bridge& ) = delete;
	bridge( const bridge&& ) = delete;
	bridge& operator= ( const bridge& ) = delete;
	bridge& operator= ( const bridge&& ) = delete;
		
	void erase ( int id ) { s->erase( id ); }
};
//---------------
class bridgeTests;
class serverTests;

class connectionTests : public std::enable_shared_from_this< connectionTests >
{	
	enum DATA_BUFF
	{
		MAX_SIZE = 1000
	};
	
	int id;
	
	char data[ MAX_SIZE ];
	
	//tcp::socket socket_;
	
	bridgeTests* bridge_to_server;

	Command cmd;

	std::mutex mtx;
	
	public:
	
	connectionTests (int id_ ) : id( id_ ) {}
	~connectionTests (){}
	
	connectionTests ( const connectionTests& ) = delete;
	connectionTests ( const connectionTests&& ) = delete;
	connectionTests& operator= ( connectionTests& ) = delete;
	connectionTests& operator= ( connectionTests&& ) = delete;
	
	void set_bridge ( serverTests* s );
	
	void start( void )  { read(); }
	
	void read ( void ) {}
	
	void write_echo ( int len ) {}
		
	void write_ack ( char* data_ , int len ) {}
};
class serverTests
{
	int counter = 0;
					   
	std::map< int , std::shared_ptr < connectionTests  > > client_pool;
		
	public:
		serverTests ( boost::asio::io_context& io_context , short port );
		~serverTests (){}
		serverTests ( const serverTests& ) = delete;
		serverTests  ( const serverTests&& ) = delete;
		serverTests& operator= ( const serverTests& ) = delete;
		serverTests& operator= ( const serverTests&& ) = delete;
   		
   		void erase ( int id );
   		
   		int client_pool_size ( void ) { return client_pool.size(); }
   		
   		void connect ( void ) { do_accept(); }

	private:
		void do_accept();				
};

class bridgeTests
{	
	serverTests* s;
	connectionTests* c;
	
	public:
		
	bridgeTests(serverTests* s_ , connectionTests* c_ ) : s( s_ ) , c( c_ ){}
	~bridgeTests(){}
	bridgeTests( const bridgeTests& ) = delete;
	bridgeTests( const bridgeTests&& ) = delete;
	bridgeTests& operator= ( const bridgeTests& ) = delete;
	bridgeTests& operator= ( const bridgeTests&& ) = delete;		
	
	void erase ( int id ) { s->erase( id ); }
};

#endif	//ASYNC_SERVER
