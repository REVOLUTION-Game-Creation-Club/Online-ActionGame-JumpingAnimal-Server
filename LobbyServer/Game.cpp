// Game.cpp: implementation of the CGame class.
//
//////////////////////////////////////////////////////////////////////

#include "Game.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGame::CGame()
{
	m_GameStatus = new CStatus_Over;
	InitializeCriticalSection(&m_cs);
}

CGame::~CGame()
{
	delete m_GameStatus;
	DeleteCriticalSection(&m_cs);
}

void CGame::ProcessPacket(Packet& packet)
{
	Lock();
	m_GameStatus->ProcessPacket(packet);
	Unlock();
}

void CGame::StartGame(Packet& UserInfo)
{
	Lock();

	SwitchStatus(m_GameStatus->ChangeStatus("Start"));
	
	//Initialize Map and Gamer...
	m_GameStatus->ProcessPacket(UserInfo);

	Unlock();
}

void CGame::GameOver()
{
	Lock();

	SwitchStatus(m_GameStatus->ChangeStatus("GameOver"));

	Unlock();
}

void CGame::Lock()
{
	EnterCriticalSection(&m_cs);
}

void CGame::Unlock()
{
	LeaveCriticalSection(&m_cs);
}

void CGame::SwitchStatus(GameStatus* status)
{
	if(m_GameStatus == status)
		return;
	delete m_GameStatus;
	m_GameStatus = status;
}