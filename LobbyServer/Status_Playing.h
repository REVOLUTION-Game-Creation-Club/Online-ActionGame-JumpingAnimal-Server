// Status_Playing.h: interface for the CStatus_Playing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUS_PLAYING_H__BA4390AE_AA1E_4CDD_B684_0A9A450AEE7D__INCLUDED_)
#define AFX_STATUS_PLAYING_H__BA4390AE_AA1E_4CDD_B684_0A9A450AEE7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameStatus.h"
#include "Status_Over.h"
#include "GameLobby.h"

class CStatus_Playing  
	: public GameStatus
{
	C_physics* m_phy;
	CGamer* gamer[4];
	DWORD	m_CurrentTime;

	DWORD	m_MeteorTime;

	DWORD	m_GameStartTime;

public:
	CStatus_Playing();
	virtual ~CStatus_Playing();

	void ProcessPacket(Packet& packet);
	GameStatus* ChangeStatus(char* Message);
	
private:
	void phy_step();
	
	void onInitGameState(Packet& packet);
	void onRightStop(Packet& packet, DWORD targetgamer);
	void onLeftStop(Packet& packet, DWORD targetgamer);
	void onRightRun(Packet& packet, DWORD targetgamer);
	void onLeftRun(Packet& packet, DWORD targetgamer);
	void onRightJump(Packet& packet, DWORD targetgamer);
	void onLeftJump(Packet& packet, DWORD targetgamer);
	void onRightDash(Packet& packet, DWORD targetgamer);
	void onLeftDash(Packet& packet, DWORD targetgamer);
	void onRightJumpDash(Packet& packet, DWORD targetgamer);
	void onLeftJumpDash(Packet& packet, DWORD targetgamer);
	void onDie(Packet& packet, DWORD targetgamer);
	void onKill(Packet& packet, DWORD targetgamer);

	void SendMeteor(int RoomID);
	
};

#endif // !defined(AFX_STATUS_PLAYING_H__BA4390AE_AA1E_4CDD_B684_0A9A450AEE7D__INCLUDED_)
