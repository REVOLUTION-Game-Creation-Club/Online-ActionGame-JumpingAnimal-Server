#ifndef  __Sync_Queue_H
#define  __Sync_Queue_H

#include "List.h"

class Queue : public Object
{
public:
			Queue();
	virtual	~Queue();

			void		push( Object* obj );
			Object*		pop();
			int			size();

private:
			List		queue;
};

#endif