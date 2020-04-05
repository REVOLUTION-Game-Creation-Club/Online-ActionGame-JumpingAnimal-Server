#ifndef  __Sync_ServerSocket_H
#define  __Sync_ServerSocket_H

#include <WinSock2.h>

#include "../lang/Object.h"
#include "../lang/Exception.h"


class SocketErrorException : public Exception
{
};

class ServerSocket : public Object
{
public:
			ServerSocket();
	virtual	~ServerSocket();

			bool		initialize( String ipAddr, int portNo, int backlogCount = SOMAXCONN );
			bool		initialize( int portNo, int backlogCount = SOMAXCONN );
			void		close();

			SOCKET		getListenSocket(){ return socketListen; }
			SOCKET		acceptConnection( struct timeval timeout, struct sockaddr FAR* addr = 0 ) throw ( SocketErrorException* );

protected:
			SOCKET		socketListen;
};

#endif