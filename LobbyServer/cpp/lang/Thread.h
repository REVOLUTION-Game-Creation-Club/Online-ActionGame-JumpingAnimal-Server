#ifndef  __Sync_Thread_H
#define  __Sync_Thread_H

#include "Object.h"


class Thread : public Object
{
public:
			Thread();
	virtual ~Thread();

	virtual	void		run(){}

			void		begin();

protected:
			HANDLE		handleThread;

private:
	static	DWORD	WINAPI	handleRunner( LPVOID parameter );

			bool		isStarted;
			DWORD		thredID;
};

#endif
