#ifndef  __Sync_Exception_H
#define  __Sync_Exception_H

#include "String.h"

class Exception
{
public:
			Exception();
			Exception( String msg );
	virtual ~Exception();

			String			message;
};

#endif
