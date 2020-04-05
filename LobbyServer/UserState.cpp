// UserState.cpp: implementation of the UserState class.
//
//////////////////////////////////////////////////////////////////////

#include "UserState.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UserState::UserState()
{

}

UserState::~UserState()
{

}

UserState* UserState::ChangeUserState(char msg, C_obj& ob)
{
	return this;
}

UserState* RRun::ChangeUserState(char msg, C_obj& ob)
{
	ob.impulsex(RIGHT_RUN_VAL);
	return this;
}

UserState* LRun::ChangeUserState(char msg, C_obj& ob)
{
	ob.impulsex(LEFT_RUN_VAL);
	return this;
}

UserState* RDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if(Time < 0)
		return new RStop;

	ob.impulsex(RIGHT_DASH_VAL);
	return this;
}

UserState* LDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if(Time < 0)
		return new LStop;

	ob.impulsex(LEFT_DASH_VAL);
	return this;
}

UserState* RJDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if(Time < 0)
	{
		ob.impulsex(RIGHT_RUN_VAL);
		return new RJump;
	}

	return this;
}

UserState* RJump::ChangeUserState(char msg, C_obj& ob)
{
	ob.impulsex(RIGHT_RUN_VAL);
	return this;
}

UserState* LJump::ChangeUserState(char msg, C_obj& ob)
{
	ob.impulsex(LEFT_RUN_VAL);
	return this;
}

UserState* LJDash::ChangeUserState(char msg, C_obj& ob)
{
	Time--;
	if(Time < 0)
	{
		ob.impulsex(LEFT_RUN_VAL);
		return new LJump;
	}

	return this;
}