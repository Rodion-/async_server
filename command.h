#ifndef	COMMAND_H
#define	COMMAND_H

// command struct
// header aa
// body   [ DATA ] 
// DATA 5-10
// crc char 2
// pattern ccdd
// aa data1-10 CRC ccdd

#include <iostream>
#include <mutex>
#include <cstring>

#include <gtest/gtest.h>

class Command
{
	#define HEADER_LENGTH	2
	#define PATTERN_LENGTH 2
	#define CMD_BODY_MAX_SIZE 10
	#define CRC_LENGTH 2

	enum DATA_BUF
	{
		MAX_SIZE = 100
	};
	
	char rdata[ MAX_SIZE ];	
	char msg[ MAX_SIZE ];
	
	int msg_len = 0;
	
	int flag = 0;	
	
	public:
	
	Command(){}
	~Command(){}
	Command( const Command& ) = delete;
	Command( const Command&& ) = delete;
	Command& operator= ( Command& ) = delete;
	Command& operator= ( Command&& ) = delete;
	
	void get_data( char* data , int* length );
	
	void cmd_decoder( char* data , int length_ , std::mutex& mtx );
};

#endif	//	COMMAND_H
