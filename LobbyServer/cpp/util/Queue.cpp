#include "Queue.h"

Queue::Queue()
{
}

Queue::~Queue()
{
	while( queue.size() > 0 )
		queue.popHead();
}

void Queue::push( Object* obj )
{
	queue.pushTail( obj );
}

Object* Queue::pop()
{
	if( queue.size() == 0 )
		return 0;

	return queue.popHead();
}

int Queue::size()
{
	return queue.size();
}