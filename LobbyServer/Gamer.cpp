// Gamer.cpp: implementation of the CGamer class.
//
//////////////////////////////////////////////////////////////////////

#include "Gamer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGamer::CGamer(char* _name, float _mas, char* _shafe)
: C_obj(_name, _mas, _shafe)
{
	m_kill = 0;
	m_dead = 0;
	
	state = new RStop;
}

CGamer::~CGamer()
{
	delete state;
}

void CGamer::ChangeState(UserState* in_state)
{
	delete state;
	state = in_state;
}

int CGamer::GetKillPoint()
{
	return m_kill;
}

int CGamer::GetDeadPoint()
{
	return m_dead;
}

void CGamer::KillOther()
{
	m_kill++;
}

void CGamer::Dead()
{
	m_dead++;
}

void CGamer::crash_obj_fix_down(C_obj* temp )
{
	if(state->StopState())
	{
		if(state->IsRight())
		{
			delete state;
			state = new RStop();
		}
		else
		{
			delete state;
			state = new LStop();
		}
	}
	C_obj::crash_obj_fix_down( temp );
}

void CGamer::step_for_movable()
{
	UserState* temp = state->ChangeUserState(0, *this);
	if(temp != state)
	{
		delete state;
		state = temp;
	}
}
