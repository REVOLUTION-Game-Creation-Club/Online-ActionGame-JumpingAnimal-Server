// Game.h: interface for the CGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAME_H__920BCA2A_E57B_43B3_8F51_DF5748D4FDB9__INCLUDED_)
#define AFX_GAME_H__920BCA2A_E57B_43B3_8F51_DF5748D4FDB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Status_Playing.h"

class CGame  
{
	CRITICAL_SECTION m_cs;
	GameStatus* m_GameStatus;

public:
	CGame();
	virtual ~CGame();

	void ProcessPacket(Packet& packet);

	void StartGame(Packet& UserInfo);
	void GameOver();
	
private:
	void Lock();
	void Unlock();

	void SwitchStatus(GameStatus* status);
};

#endif // !defined(AFX_GAME_H__920BCA2A_E57B_43B3_8F51_DF5748D4FDB9__INCLUDED_)
