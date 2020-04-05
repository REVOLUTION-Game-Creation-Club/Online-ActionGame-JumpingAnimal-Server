#ifndef __Sync_InetAddress_H
#define __Sync_InetAddress_H

#include "../lang/String.h"


class InetAddress : public Object
{
public:
			InetAddress();
	virtual	~InetAddress();

	static	String		getHostAddress();
	static	String		getHostName();
	static	String		getHostByName( String& name );
};

#endif
