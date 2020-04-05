// UserState.h: interface for the UserState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERSTATE_H__7C4C3D13_3FF7_4615_BDED_D94F386E38B5__INCLUDED_)
#define AFX_USERSTATE_H__7C4C3D13_3FF7_4615_BDED_D94F386E38B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "C_physics.h"
#include "protocol.h"

const int DASH_READY_TIME	=	7;
const int DASH_DELAY_TIME	=	30;
const int DELAY_INFINITE	=	0x7fffffff;
const int DASH_TIME			=	7;
const int SJUMP_READY_TIME	=	3;

const float RIGHT_RUN_VAL	=	5.0f;
const float LEFT_RUN_VAL	=	-5.0f;
const float	RIGHT_DASH_VAL	=	15.0f;
const float LEFT_DASH_VAL	=	-15.0f;
const float JUMP_VAL		=	15.0f;

class UserState  
{
public:
	UserState();
	virtual ~UserState();

	virtual UserState* ChangeUserState(char msg, C_obj& ob);
	virtual bool StopState() {	return true;	}
	virtual bool IsRight()	{	return	true;	}

};

class Die
	:	public UserState
{
public:
	bool StopState()	{	return false;	}
};
class RStop
: public UserState{
public:
	bool StopState() {	return false;	}
};

class LStop
: public UserState{
public:
	bool StopState() {	return false;	}
	virtual bool IsRight()	{	return	false;	}
};

class RRun
: public UserState{
public:
	UserState* ChangeUserState(char msg, C_obj& ob);
	bool StopState() {	return false;	}
};

class LRun
:public UserState{
public:
	UserState* ChangeUserState(char msg, C_obj& ob);
	bool StopState() { return false; }
	virtual bool IsRight()	{	return	false;	}
};

class RDash
:public UserState{
	int Time;
public:
	RDash() { Time = DASH_TIME; }

	UserState* ChangeUserState(char msg, C_obj& ob);
	bool StopState() { return false; }
};

class LDash
: public UserState{
	int Time;
public:
	LDash() { Time = DASH_TIME; }

	UserState* ChangeUserState(char msg, C_obj& ob);
	bool StopState() { return false; }
	virtual bool IsRight()	{	return	false;	}
};

class RJump
: public UserState{
public:
	UserState* ChangeUserState(char msg, C_obj& ob);
};

class LJump
:public UserState{
public:
	UserState* ChangeUserState(char msg, C_obj& ob);
	virtual bool IsRight()	{	return	false;	}
};

class RJDash
:public UserState{
	int Time;
public:
	RJDash() { Time = DASH_TIME; }

	UserState* ChangeUserState(char msg, C_obj& ob);
};

class LJDash
: public UserState{
	int Time;
public:
	LJDash() { Time = DASH_TIME; }
	
	UserState* ChangeUserState(char msg, C_obj& ob);
	virtual bool IsRight()	{	return	false;	}
};

#endif // !defined(AFX_USERSTATE_H__7C4C3D13_3FF7_4615_BDED_D94F386E38B5__INCLUDED_)
