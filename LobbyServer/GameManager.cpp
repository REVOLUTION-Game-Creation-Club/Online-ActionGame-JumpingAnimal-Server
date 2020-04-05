// GameManager.cpp: implementation of the CGameManager class.
//
//////////////////////////////////////////////////////////////////////

#include "GameManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameManager::CGameManager()
{
	int i;
	for(i = 0 ; i < MAX_PLAYING_GAME ; i++)
		m_emptyGame.push_back(i);
	InitializeCriticalSection(&m_cs);
}

CGameManager::~CGameManager()
{
	DeleteCriticalSection(&m_cs);
}

void CGameManager::ProcessPacket(Packet& packet)
{
	DWORD GameID;
	packet >> GameID;
	
	if(GameID == INVAILID_GAME_ID)
		return;
	
	m_Game[GameID].ProcessPacket(packet);
}

unsigned long CGameManager::CreateGame(Packet& UserInfo)
{
	Lock();

	if(m_emptyGame.empty())
	{
		Unlock();
		return INVAILID_GAME_ID;
	}

	int GameID = m_emptyGame.back();
	m_emptyGame.pop_back();

	Unlock();

	m_Game[GameID].StartGame(UserInfo);

	return GameID;
}

void CGameManager::DeleteGame(unsigned long GameID)
{
	if(GameID == INVAILID_GAME_ID)
		return;
	
	m_Game[GameID].GameOver();

	Lock();

	m_emptyGame.push_back(GameID);

	Unlock();
}

void CGameManager::Lock()
{
	EnterCriticalSection(&m_cs);
}

void CGameManager::Unlock()
{
	LeaveCriticalSection(&m_cs);
}
