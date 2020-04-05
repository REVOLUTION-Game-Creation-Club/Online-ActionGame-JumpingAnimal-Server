// GameManager.h: interface for the CGameManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEMANAGER_H__42A49551_7768_4B5D_8B64_1144E52E924C__INCLUDED_)
#define AFX_GAMEMANAGER_H__42A49551_7768_4B5D_8B64_1144E52E924C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Game.h"
#include <list>

using namespace std;

#define MAX_PLAYING_GAME 10
#define INVAILID_GAME_ID MAX_PLAYING_GAME + 1

class CGame;

class CGameManager  
{
	CRITICAL_SECTION m_cs;
	CGame m_Game[MAX_PLAYING_GAME];
	list<int> m_emptyGame;
public:
	CGameManager();
	virtual ~CGameManager();

	void ProcessPacket(Packet& packet);
	
	unsigned long CreateGame(Packet& UserInfo);
	void DeleteGame(unsigned long GameID);

private:
	void Lock();
	void Unlock();

};

#endif // !defined(AFX_GAMEMANAGER_H__42A49551_7768_4B5D_8B64_1144E52E924C__INCLUDED_)
