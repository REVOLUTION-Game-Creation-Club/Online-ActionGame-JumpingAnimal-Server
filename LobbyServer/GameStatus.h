// GameStatus.h: interface for the GameStatus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMESTATUS_H__36A3E9DB_D792_445E_865B_696D5EEF7E73__INCLUDED_)
#define AFX_GAMESTATUS_H__36A3E9DB_D792_445E_865B_696D5EEF7E73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#include "cpp/net/Packet.h"
#include "Gamer.h"

class GameStatus  
{
public:
	GameStatus();
	virtual ~GameStatus();

	virtual void ProcessPacket(Packet& packet) = 0;
	virtual GameStatus* ChangeStatus(char* Message) = 0;

	bool operator==(GameStatus* input);
};

#endif // !defined(AFX_GAMESTATUS_H__36A3E9DB_D792_445E_865B_696D5EEF7E73__INCLUDED_)
