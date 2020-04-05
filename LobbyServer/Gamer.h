// Gamer.h: interface for the CGamer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMER_H__B7C82104_029D_4591_82D0_7F78CF9C26DA__INCLUDED_)
#define AFX_GAMER_H__B7C82104_029D_4591_82D0_7F78CF9C26DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserState.h"

const float GAME_GRAVITY	=	1.0f;

class CGamer  : public C_obj
{
	int m_kill;
	int m_dead;
	UserState* state;

public:
	CGamer(char* _name, float _mas, char* _shafe);
	virtual ~CGamer();

	void KillOther();
	void Dead();

	int GetKillPoint();
	int GetDeadPoint();

	void ChangeState(UserState* in_state);
	void crash_obj_fix_down(C_obj* temp );
	void step_for_movable();

};

#endif // !defined(AFX_GAMER_H__B7C82104_029D_4591_82D0_7F78CF9C26DA__INCLUDED_)
