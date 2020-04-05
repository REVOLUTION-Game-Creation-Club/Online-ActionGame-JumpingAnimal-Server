// Status_Over.cpp: implementation of the CStatus_Over class.
//
//////////////////////////////////////////////////////////////////////

#include "Status_Over.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatus_Over::CStatus_Over()
{

}

CStatus_Over::~CStatus_Over()
{

}

void CStatus_Over::ProcessPacket(Packet& packet)
{
}

GameStatus* CStatus_Over::ChangeStatus(char* Message)
{
	if(!strcmp(Message, "Start"))
		return new CStatus_Playing;
	return this;
}