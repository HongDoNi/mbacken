/**
 * Socket class which wraps low level C socket functions for client/server socket programming.
 * Provide timeout functionality for functions which are usually blocking calls.
 * 
 */
 
#ifndef Socket_class
#define Socket_class

#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

namespace b2be
{
	namespace utils
	{
		const int MAXHOSTNAME = 200;
		const int MAXCONNECTIONS = 5;
		const int MAXRECV = 4096;
		const int CONNECTIONTIMEOUT = 15; //seconds
		const int RECVTIMEOUT = 1000; //ms

		class Socket
		{
			public:
				Socket();
				virtual ~Socket();
				
				// Server initialization
				void Create();
				void Bind ( const int port );
				void Listen() ;
				void Listen(int maxconnections) ;
				void Accept ( Socket* ) ;

				// Client initialization
				std::string LookupIP( const std::string &host );
				
				void Connect ( const std::string &host, const int port );
				void Connect ( const std::string &host, const int port, const int timeoutsec );

				// Data Transimission
				void Send ( const std::string &s, unsigned int &sentbytes ) ;
				void Receive ( std::string&, unsigned int &receivedbytes ) ;
				void Receive ( std::string&, unsigned int &receivedbytes, const int maxbufsize ) ;
				void Receive ( std::string&, unsigned int &receivedbytes, const int maxbufsize, const int timeoutms ) ;

				bool IsError() const;
				std::string GetErrorMessage() const;
				void ResetError();

			private:
				bool IsValid() const { return m_sock != -1; }
				void SetNonBlocking ( const bool );
				void SetConnectionErrorString(const std::string &host, int port, const std::string &err);

				int m_sock;
				sockaddr_in m_addr;
		  
				bool is_error;
				std::string error_message;
		};
	}
}

#endif
