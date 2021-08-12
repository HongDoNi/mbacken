/**
 * Socket class which wraps low level C socket functions for client/server socket programming.
 * Provide timeout functionality for functions which are usually blocking calls.
 * 
 * Client sockets must: Create -> Connect
 * Server sockets must: Create -> Bind -> Listen -> loop(Accept)
 */
 
#include "Socket.h"
using namespace b2be::utils;
 
Socket::Socket() :  m_sock ( -1 )
{
	memset ( &m_addr, 0, sizeof ( m_addr ) );
		
	ResetError();
}

Socket::~Socket()
{
	if ( IsValid() )
		close ( m_sock );
}

/**
 * 
 * Creates a socket which can then be used to listen as a server or to connect to a remote server as a client.
 * <br/>
 * Example:
 * <code>
 * 		mySocket->Create();
 * </code>
 *
 * Upon failure to create a socket, IsError() will return true and an appropriate error message will be set for GetErrorMessage()
 *
 */
void Socket::Create()
{
	m_sock = socket ( AF_INET, SOCK_STREAM, 0 );

	if ( ! IsValid() )
	{
		is_error = true;
		error_message = "Unable to create socket";
		return;
	}

	int on = 1;
	if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof ( on ) ) == -1 )
	{
		is_error = true;
		error_message = "Unable to set socket options (reuse)";
		return;
	}
	if ( setsockopt ( m_sock, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof ( on ) ) == -1 )
	{
		is_error = true;
		error_message = "Unable to set socket options (keepalive)";
		return;
	}
	struct linger ling = {0, 0};
	if ( setsockopt(m_sock, SOL_SOCKET, SO_LINGER, &ling, sizeof ling) == -1 )
	{
		is_error = true;
		error_message = "Unable to set socket options (linger)";
		return;
	}
	if ( setsockopt(m_sock, IPPROTO_TCP, TCP_NODELAY, &on, sizeof ( on )) == -1 )
	{
		is_error = true;
		error_message = "Unable to set socket options (nodelay)";
		return;
	}
	
	
}
	
/**
 * 
 * Binds a socket to a specific port number. To only be used when setting up a server socket.
 * A client socket is automatically allocated a random port by the system kernel when making outbound connections.
 * <br/>
 * Example:
 * <code>
 * 		mySocket->Bind(11223);
 * </code>
 *
 * Upon failure to bind the port to the socket, IsError() will return true and an appropriate error message will be set for GetErrorMessage()
 *
 * @param port The port you want to bind to the socket.
 *
 */
void Socket::Bind ( const int port )
{
	if ( ! IsValid() )
	{
		is_error = true;
		error_message = "No socket has been created";
		return;
	}
    
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = INADDR_ANY;
	m_addr.sin_port = htons ( port );

	int bind_return = bind ( m_sock, ( struct sockaddr * ) &m_addr, sizeof ( m_addr ) );

	if ( bind_return == -1 )
    {
		is_error = true;
		error_message = "Unable to bind port to socket";
		return;
    }
}

/**
 * 
 * Prepares the socket to listen for incoming connections.
 * A default number of maximum connections will be set.
 * <br/>
 * Example:
 * <code>
 * 		mySocket->Listen();
 * </code>
 *
 * Upon failure to listen on the socket, IsError() will return true and an appropriate error message will be set for GetErrorMessage()
 *
 */
void Socket::Listen() 
{
	Listen(MAXCONNECTIONS);
}

/**
 * 
 * Prepares the socket to listen for incoming connections.
 * <br/>
 * Example:
 * <code>
 * 		mySocket->Listen();
 * </code>
 *
 * Upon failure to listen on the socket, IsError() will return true and an appropriate error message will be set for GetErrorMessage()
 *
 * @param maxconnections The maximum number of concurrent inbound connections to allow.
 *
 */
void Socket::Listen(int maxconnections) 
{
	if ( ! IsValid() )
    {
		is_error = true;
		error_message = "No socket has been created";
		return;
	}

	int listen_return = listen ( m_sock, maxconnections );

	if ( listen_return == -1 )
    {
		is_error = true;
		error_message = "Unable to listen on socket";
		return;
    }
}

/**
 * 
 * Accepts an incoming socket connection and prepares the Socket instance passed to it for use.
 * This is a blocking call which will wait until it receives a connection.
 * <br/>
 * Example:
 * <code>
 * 		mySocket->Accept(newSocket);
 * </code>
 *
 * Upon failure to accept the incoming connection, IsError() will return true and an appropriate error message will be set for GetErrorMessage()
 *
 * @param new_socket A pre-created Socket object which will be initialised for use by this function.
 *
 */
void Socket::Accept ( Socket *new_socket ) 
{
	int addr_length = sizeof ( m_addr );
	new_socket->m_sock = accept ( m_sock, ( sockaddr * ) &m_addr, ( socklen_t * ) &addr_length );

	if ( new_socket->m_sock <= 0 )
	{
		is_error = true;
		error_message = "Unable to accept incoming socket connection";
		return;
	}
}

/**
 * 
 * Send data out on the socket connection.
 * <br/>
 * Example:
 * <code>
 * 		mySocket->Send(myData, sent);
 * </code>
 *
 * Upon failure of the socket, IsError() will return true and an appropriate error message will be set for GetErrorMessage().
 * This function will indicate an error if the socket is closed from the other end, however this error may not be immediate and may allow one send to appear
 * successful before indicating an error.
 *
 * @param s The string buffer to be sent.
 * @param sentbytes An int which will be populated with the number of bytes we sent. This will always be the size of the string unless there is an error.
 *
 */
void Socket::Send ( const std::string &s, unsigned int &sentbytes ) 
{
	sentbytes = 0;        // how many bytes we've sent
    int bytesleft = s.size(); // how many we have left to send
    int n = 0;

	errno = 0;
    while(sentbytes < s.size()) 
	{
        n = send(m_sock, s.c_str()+sentbytes, bytesleft, MSG_NOSIGNAL);
        if (n == -1)
			break;
        sentbytes += n;
        bytesleft -= n;
    }    

	if (n==-1)
    {
		is_error = true;
		error_message = "Socket has been disconnected. Error: ";
		error_message.append(strerror(errno));
		return;
	}
}

/**
 * 
 * Receive incoming data on the socket connection. 
 * To keep your application repsonsive, the timeout should be kept small and looping for more data should be managed in your application.
 * A default timeout value and buffer size are used.
 * <br/>
 * Example:
 * <code>
 * 		mySocket->Receive(buffer, received, );
 * </code>
 *
 * Upon failure of the socket, IsError() will return true and an appropriate error message will be set for GetErrorMessage().
 * This function is not able to indicate if the socket has been closed at the other end. It will return 0 bytes received which 
 * could indicate the socket is closed or that the other end of the socket just hasn't sent anything yet.
 *
 * @param s The string buffer to be populated with the data we receive.
 * @param receivedbytes An int which will be populated with the number of bytes we receive.
 *
 */
void Socket::Receive( std::string& s , unsigned int& receivedbytes ) 
{
	Receive(s,receivedbytes,MAXRECV,RECVTIMEOUT);
}

/**
 * 
 * Receive incoming data on the socket connection. 
 * To keep your application repsonsive, the timeout should be kept small and looping for more data should be managed in your application.
 * A default timeout value is used.
 * <br/>
 * Example:
 * <code>
 * 		mySocket->Receive(buffer, received, bufsize);
 * </code>
 *
 * Upon failure of the socket, IsError() will return true and an appropriate error message will be set for GetErrorMessage().
 * This function is not able to indicate if the socket has been closed at the other end. It will return 0 bytes received which 
 * could indicate the socket is closed or that the other end of the socket just hasn't sent anything yet.
 *
 * @param s The string buffer to be populated with the data we receive.
 * @param receivedbytes An int which will be populated with the number of bytes we receive.
 * @param maxbufsize The maximum amount of data to place into the buffer.
 *
 */
void Socket::Receive( std::string& s , unsigned int& receivedbytes, const int maxbufsize ) 
{
	Receive(s,receivedbytes,maxbufsize,RECVTIMEOUT);
}

/**
 * 
 * Receive incoming data on the socket connection. 
 * The timeout is how long to wait for initial data to become available - looping for more data should be managed in your application.
 * <br/>
 * Example:
 * <code>
 * 		mySocket->Receive(buffer, received, bufsize, 5);
 * </code>
 *
 * Upon failure of the socket, IsError() will return true and an appropriate error message will be set for GetErrorMessage().
 * This function is not able to indicate if the socket has been closed at the other end. It will return 0 bytes received which 
 * could indicate the socket is closed or that the other end of the socket just hasn't sent anything yet.
 *
 * The function will just append to the string provided.
 *
 * @param s The string buffer to be populated with the data we receive.
 * @param receivedbytes An int which will be populated with the number of bytes we receive.
 * @param maxbufsize The maximum amount of data to place into the buffer.
 * @param timeoutms The number of milliseconds before the receive will timeout and return.
 *
 */
void Socket::Receive( std::string& s , unsigned int& receivedbytes, const int maxbufsize, const int timeoutms) 
{
    receivedbytes = 0;        // how many bytes we've received
    	
	struct timeval tv;
    fd_set readfds;

    tv.tv_sec = 0;
    tv.tv_usec = timeoutms * 1000; //timeoutms converted to usec

    FD_ZERO(&readfds);
    FD_SET(m_sock, &readfds);
	
	// don't care about writefds and exceptfds:
	select(m_sock+1, &readfds, NULL, NULL, &tv);		
	
	if (FD_ISSET(m_sock, &readfds))
	{		
		char *buf = new char[maxbufsize+1];
		memset ( buf, 0, maxbufsize + 1 );
		
		errno = 0;
		int rb = recv(m_sock, buf, maxbufsize, 0);
		
		if (rb == -1) 
		{ 
			is_error = true;
			error_message = "Socket has been disconnected. Error: ";
			error_message.append(strerror(errno));
			delete [] buf;
			return; 
		}
		
		receivedbytes = rb;
		
		if (receivedbytes != 0)
		{
			s.append(buf);
		}	
		delete [] buf;		
	}
	else
	{
		//timeout - just return without erroring
		return;
	}
} 

/**
 * 
 * Looks up an IP address for the host name provided.
 * <br/>
 * Example:
 * <code>
 * 		string ip = mySocket->LookupIP("www.b2be.com");
 * </code>
 *
 * Upon failure to lookup the IP, IsError() will return true and an appropriate error message will be set for GetErrorMessage()
 *
 * @param host The host name to lookup.
 * @return An IP address for the specified host, or empty if not found.
 *
 */
std::string Socket::LookupIP( const std::string &host )
{
	addrinfo *m_servinfo;
	addrinfo hints;
	addrinfo *p;
	
	int status;	
	
	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_INET;     // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

	if ((status = getaddrinfo(host.c_str(), NULL, &hints, &m_servinfo)) != 0) 
	{
		is_error = true;
		error_message = "getaddrinfo error: ";
		error_message.append(gai_strerror(status));
		return "";
	}
	
	//there are potentially multiple results but the first one should usually do
    for(p = m_servinfo;p != NULL; p = p->ai_next) 
	{
        void *addr;

        struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
        addr = &(ipv4->sin_addr);

        // convert the IP to a string: ip
		char ip[20];
        inet_ntop(p->ai_family, addr, ip, sizeof(ip));
		
		freeaddrinfo(m_servinfo); // free the linked list
		
		return ip;
    }
	
	is_error = true;
	error_message = "Unable to find IP for host.";
	return "";
}

/**
 * 
 * Connects the socket to a server running on the specified host and port. If the host is a domain name the IP address will be looked up.
 * A default connection timeout will be used.
 * <br/>
 * Example:
 * <code>
 * 		mySocket->Connect("www.b2be.com", 80);
 * </code>
 *
 * Upon failure to connect, IsError() will return true and an appropriate error message will be set for GetErrorMessage()
 *
 * @param host The host name or IP address to connect to.
 * @param port The port to connect to.
 *
 */
void Socket::Connect ( const std::string &host, const int port )
{
	Connect(host,port,CONNECTIONTIMEOUT);
}

/**
 * 
 * Connects the socket to a server running on the specified host and port. If the host is a domain name the IP address will be looked up.
 * <br/>
 * Example:
 * <code>
 * 		mySocket->Connect("www.b2be.com", 80, 5);
 * </code>
 *
 * Upon failure to connect, IsError() will return true and an appropriate error message will be set for GetErrorMessage()
 *
 * @param host The host name or IP address to connect to.
 * @param port The port to connect to.
 * @param timeoutsec The number of seconds before the connection attempt will timeout.
 *
 */
void Socket::Connect ( const std::string &host, const int port, const int timeoutsec )
{
	if ( ! IsValid() ) 
	{
		is_error = true;
		error_message = "No socket has been created";
		return;
	}

	std::string ip = LookupIP(host);
	std::string hostip = host;
	if (ip != host)
		hostip = host + " (" + ip + ")";
	
	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons ( port );

	errno = 0;
	int status = inet_pton ( AF_INET, ip.c_str(), &m_addr.sin_addr );

	if ( errno == EAFNOSUPPORT ) //address family not support by protocol
	{
		is_error = true;
		error_message = "Host address not supported by protocol";
		return;
	}
	
	struct timeval tv;
    fd_set writefds;

    tv.tv_sec = timeoutsec;
    tv.tv_usec = 0; 

    FD_ZERO(&writefds);
    FD_SET(m_sock, &writefds);
		
	//set non blocking scoket
	SetNonBlocking(true);
	connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

	// don't care about readfds and exceptfds:
	errno = 0;
	status = select(m_sock+1, NULL, &writefds, NULL, &tv);
	
	if (status == 0)
	{
		//timeout
		is_error = true;
		SetConnectionErrorString(hostip,port,"Connection Timed Out");
		return;
	}
	else if (status == -1)
	{
		//error
		is_error = true;
		SetConnectionErrorString(hostip,port,strerror(errno));
		return;
	}
	else
	{
		if (FD_ISSET(m_sock, &writefds) )
		{
			//get a possible error message from the socket
			//must be done this way for non blocking sockets
			int err = 0;
			socklen_t len = sizeof(int);			
			getsockopt(m_sock, SOL_SOCKET, SO_ERROR, &err, &len);
			
			if (err != 0)
			{
				is_error = true;
				SetConnectionErrorString(hostip,port,strerror(err));
				return;
			}

			//unset non blocking
			SetNonBlocking(false);
			return;		
		}
		else
		{
			is_error = true;
			SetConnectionErrorString(hostip,port,"select() error");
			return;		
		}
	}
}

/**
 * 
 * Builds a standard connection error string to be used in the error message.
 * <br/>
 * Example:
 * <code>
 * 		mySocket->SetConnectionErrorString(host, port, errmsg);
 * </code>
 *
 * @param host The host name and/or IP address we are trying to connect to.
 * @param port The port we are trying to connect to.
 * @param err Optional additional error information.
 *
 */
void Socket::SetConnectionErrorString(const std::string &host, int port, const std::string &err)
{
	char pt[10];	
	snprintf(pt,9,"%d",port);
	
	error_message = "Unable to connect to host (";
	error_message.append(host);
	error_message.append(") on specified port (");
	error_message.append(pt);
	error_message.append(")");
	if (err != "")
	{
		error_message.append(". Error: ");
		error_message.append(err);
	}
}

/**
 * 
 * Sets the non-blocking status of this Socket.
 * <br/>
 * Example:
 * <code>
 * 		mySocket->SetNonBlocking(true);
 * </code>
 *
 * @param b A boolean to indicate whether to enable or disable the non-blocking status of this socket.
 *
 */
void Socket::SetNonBlocking ( const bool b )
{
	int opts;

	opts = fcntl ( m_sock, F_GETFL );

	if ( opts < 0 )
    {
		return;
    }

	if ( b )
		opts = ( opts | O_NONBLOCK );
	else
		opts = ( opts & ~O_NONBLOCK );

	fcntl ( m_sock, F_SETFL,opts );
}

/**
 * 
 * Indicates if there is currently an error for this Socket
 * <br/>
 * Example:
 * <code>
 * 		bool socketErrored = mySocket->IsError();
 * </code>
 *
 * @return A boolean indicating if there is currently and error.
 *
 */
bool Socket::IsError() const
{
	return is_error;
}

/**
 * 
 * Returns the current error message for this Socket
 * <br/>
 * Example:
 * <code>
 * 		string myError = mySocket->GetErrorMessage();
 * </code>
 *
 * @return The current error message.
 *
 */
std::string Socket::GetErrorMessage() const
{
	return error_message;
}

/**
 * 
 * Resets the current error status for this Socket
 * <br/>
 * Example:
 * <code>
 * 		mySocket->ResetError();
 * </code>
 *
 */
void Socket::ResetError()
{
	is_error = false;
	error_message = "";
}
