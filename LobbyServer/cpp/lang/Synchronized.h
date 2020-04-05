#ifndef  __Sync_Synchronized_H
#define  __Sync_Synchronized_H

class Object;

class Synchronized
{
public:
			Synchronized( Object* obj );
	virtual	~Synchronized();

private:
			Object*		monitorObject;
};

#endif
