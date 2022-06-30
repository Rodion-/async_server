#include "command.h"

void Command :: get_data( char* data , int* length )
{
	memcpy( data , msg , msg_len );
	
	*length = msg_len;
}	
	
void Command :: cmd_decoder( char* data , int length , std::mutex& mtx )
{
	std::lock_guard lock(mtx);
	
	auto start = std::chrono::system_clock::now();
	
	int pos = 0; // индекс для заполнения команды
	int rpos = 0;
			
	int cmd_len = 0;
	
	flag = 0;
		
	for( int i = 0; i < length; i++ )
	{	
		if( ( data[ i - 1 ] != 'a' ) && ( data[ i ] != 'a' ) ) 
		{
			pos++;
			continue;
		}
		else
		{
			flag = 1;
			
			// copy header
			for( int k = 0; k < HEADER_LENGTH; k++ )
			{
				rdata[ rpos++ ] = data[ pos++ ];
			}
			// copy data								
			for( int k = 0; k <= CMD_BODY_MAX_SIZE; k++ )
			{
				rdata[ rpos++ ] = data[ pos++ ];
				
				if( data[ pos - 1 ] == 'c' && data[ pos ] == 'd' ) break;
				
				cmd_len++;
				
				if( ( k == CMD_BODY_MAX_SIZE - 1 ) && ( data[ pos - 1 ] != 'c' || data[ pos ] != 'd' ) )
				{
					flag = 0;
					break;
				}
			}
			
			if( flag == 0 ) break;
			
			//copy crc
			for( int k = 0; k < CRC_LENGTH; k++ )
			{
				rdata[ rpos++ ] = data[ pos++ ];
			}
				
			// check CRC
			//...	
			
				
			std::cout<<"cmd_decoder cmd_len : " << cmd_len <<std::endl;
			
			const char* ack = { "cmd : " };
				
			memcpy( msg , ack , 6 );
			memcpy( &msg[ 6 ] , &rdata[ HEADER_LENGTH ] , cmd_len );
			memcpy( &msg[ 6 + cmd_len ] , "\n" , 1 );
				
			msg_len = 6 + cmd_len + 1;
				
			break;
		}
	}
	
	if( flag == 0 )
	{
		const char* ack = { "invalid cmd : " };

		memcpy( msg , ack , 14 );
			
		memcpy( &msg[ 14 ] ,  "\n" , 1 );
			
		msg_len = 14 + 1;
	}
	static double time_left = 0;
	
	auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = end - start;	
        
        if( time_left < diff.count() ) time_left = diff.count();
        
        std::cout<<"time left : " << time_left << std::endl;	
}


TEST( Command , cmd_parse_test )
{
	//	ARRANGE	
	
	Command cmd;
	
	std::mutex mtx;
	char data_in[ 100 ] = { 0 };
	int length = 29;
	
	
	std::string s1 = "oitjogf3oiewoi";
	std::string s_cmd = "aa12345qwercd";
	std::string s2 = "i4320jr";
	
	std::string data = s1 + s_cmd + s2;
	
	length = data.size();
	
	char data_out[ 100 ] = { 0 };
	
	strncpy( data_in , data.c_str() , length );
	
	//	ACT
	
	cmd.cmd_decoder( data_in , length , mtx );
	
	cmd.get_data( data_out , &length );	
	
	
	//	ASSERT
    	
    	EXPECT_EQ( s_cmd.size() - HEADER_LENGTH - PATTERN_LENGTH ,  length - 7 );
    	
    	EXPECT_STREQ( data_out , "cmd : 12345qwer\n" );
}
