// Status_Playing.cpp: implementation of the CStatus_Playing class.
//
//////////////////////////////////////////////////////////////////////

#include "Status_Playing.h"
#include <fstream>
#include <string>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatus_Playing::CStatus_Playing()
{
	m_CurrentTime = 0;
	m_phy = NULL;
	int i;
	for(i = 0 ; i < 4 ; i++)
		gamer[i] = NULL;
	m_GameStartTime = m_MeteorTime = 0;
}

CStatus_Playing::~CStatus_Playing()
{
	if(!m_phy)
		delete m_phy;
	int i;
	for( i = 0 ; i < 4 ; i++)
		if(!gamer[i])
			delete gamer[i];
}

void CStatus_Playing::ProcessPacket(Packet& packet)
{	
	CGamer* TargetGamer;
	DWORD UserID, PKtype;				  
	packet >> UserID >> PKtype;
	if(UserID > 3)
		return;
	switch(PKtype)
	{
	case INIT_GAMESTATE:	onInitGameState(packet);		break;
	case RIGHT_STOP:		onRightStop(packet, UserID);	break;
	case LEFT_STOP:			onLeftStop(packet, UserID);		break;
	case RIGHT_RUN:			onRightRun(packet, UserID);		break;
	case LEFT_RUN:			onLeftRun(packet, UserID);		break;
	case RIGHT_JUMP:		onRightJump(packet, UserID);	break;
	case LEFT_JUMP:			onLeftJump(packet, UserID);		break;
	case RIGHT_DASH:		onRightDash(packet, UserID);	break;
	case LEFT_DASH:			onLeftDash(packet, UserID);		break;
	case RIGHT_JUMP_DASH:	onRightJumpDash(packet, UserID);	break;
	case LEFT_JUMP_DASH:	onLeftJumpDash(packet, UserID);	break;
	case DIE:				onDie(packet, UserID);			break;
	case KILL:				onKill(packet, UserID);			break;
	}
}

void CStatus_Playing::phy_step()
{
	if(m_CurrentTime == 0)
		m_CurrentTime = GetTickCount();

	DWORD	Time = GetTickCount();
	int i;
	if(Time - m_CurrentTime >= 30)
	{
		for( i = (Time - m_CurrentTime)/30 ; i > 0 ; i-- )
		{
			m_phy->step();
		}
		m_CurrentTime = Time - ((Time - m_CurrentTime)%30);
	}
}

void CStatus_Playing::onInitGameState(Packet& packet)
{
	int MapNo, RoomID;
	Packet SendPacket(REQ_GAME);
	SendPacket << (DWORD)INIT_GAMESTATE;
	packet >> MapNo;
	SendPacket << MapNo;
	
	m_phy = new C_physics(0.0f, 10.0f);
	
	//Initialize Map
	char MapFileName[64];
	sprintf(MapFileName, "./Config\\m%d.txt", MapNo);
	ifstream MapFile(MapFileName);
	
	DWORD TileNo, pos_x, pos_y;
	C_obj* TileObj;
	char TileName[20];
	while(!MapFile.eof())
	{
		MapFile >> TileNo >> pos_x >> pos_y;
		sprintf(TileName, "Tile%d", TileNo);
		//임시로 const 날림
		TileObj = new C_obj((char*)TileName, 1.0f, "square");
		TileObj->setedge(pos_x*25, pos_y*25, 25, 25);
		m_phy->fixed_obj_include(TileObj);
	}
	MapFile.close();

	int i, UserCount;
	packet >> UserCount;
	SendPacket << UserCount;
	char User_Name[20];
	for(i = 0 ; i < UserCount ; i++)
	{
		packet >> User_Name;
		gamer[i] = new CGamer(User_Name, 1.0f, "square");
		gamer[i]->setedge(100*(i+1), 100, 50, 50);
		m_phy->movable_obj_include(gamer[i]);
		SendPacket << User_Name << 100*(i+1) << 100;
	}

	packet.clear();
	packet.id(SendPacket.id());
	packet.writeData(&(SendPacket.getPacketBuffer()[2+2]), SendPacket.getDataFieldSize());

	m_GameStartTime = m_MeteorTime = GetTickCount();

}

void CStatus_Playing::onDie(Packet& packet, DWORD targetgamer)
{
	DWORD Data1, Data2, RoomID;
	float v_x, v_y;
	Packet sendPacket(REQ_GAME);
	sendPacket << targetgamer << DIE;

	phy_step();

	gamer[targetgamer]->ChangeState( new Die );

	packet >> RoomID >> Data1 >> Data2;
	gamer[targetgamer]->setxy(Data1, Data2);
	sendPacket << Data1 << Data2;

	packet >> v_x >> v_y;
	gamer[targetgamer]->impulsex(v_x);
	gamer[targetgamer]->impulsey(v_y);
	sendPacket << v_x << v_y;

	GameLobby::getInstance()->getRoom(RoomID)->broadcast(sendPacket);

	SendMeteor(RoomID);
}

void CStatus_Playing::SendMeteor(int RoomID)
{
	if(m_GameStartTime == 0)
		return;
	if(GetTickCount() - m_MeteorTime >= 10000)
	{
		m_MeteorTime = GetTickCount();

		Packet MeteorPacket(REQ_GAME);
		if(rand()%2 == 0)
			MeteorPacket << METEOR_GOOD;
		else
			MeteorPacket << METEOR_BAD;

		int s_x, s_y = -70, e_x, e_y;

		switch(rand()%6)
		{
		case 0:
			s_x = 50 + 10;
			e_x = 50 + 10;
			e_y = 225 - 70 -1;
			break;
		case 1:
			s_x = 25*13 + 10;
			e_x = 25*13 + 10;
			e_y = 25*16 - 70 - 1;
			break;
		case 2:
			s_x = 25*18 + 10;
			e_x = 25*18 + 10;
			e_y = 25*11 - 70 - 1;
			break;
		case 3:
			s_x = 25*25 + 10;
			e_x = 25*25 + 10;
			e_y = 25*8 - 70 - 1;
			break;
		case 4:
			s_x = 25*28 + 10;
			e_x = 25*28 + 10;
			e_y = 25*14 - 70 - 1;
			break;
		case 5:
			s_x = 25*5 + 10;
			e_x = 25*5 + 10;
			e_y = 25*20 - 70 - 1;
		}

		MeteorPacket << s_x << s_y << e_x << e_y;

		GameLobby::getInstance()->getRoom(RoomID)->broadcast(MeteorPacket);
	}

	if(GetTickCount() - m_GameStartTime > 180000)
	{
		Packet OverPacket(REQ_GAME);
		OverPacket << TIME_OVER;
		GameLobby::getInstance()->getRoom(RoomID)->broadcast(OverPacket);
	}
}

void CStatus_Playing::onRightStop(Packet& packet, DWORD targetgamer)
{
	DWORD Data1, Data2, RoomID;
	float v_x, v_y;
	Packet sendPacket(REQ_GAME);
	sendPacket << targetgamer << (DWORD)RIGHT_STOP;

	phy_step();

	gamer[targetgamer]->ChangeState( new RStop );

	packet >> RoomID >> Data1 >> Data2;
	gamer[targetgamer]->setxy(Data1, Data2);
	sendPacket << Data1 << Data2;

	packet >> v_x >> v_y;
	gamer[targetgamer]->impulsex(v_x);
	gamer[targetgamer]->impulsey(v_y);
	sendPacket << v_x << v_y;

	GameLobby::getInstance()->getRoom(RoomID)->broadcast(sendPacket);

	SendMeteor(RoomID);
}

void CStatus_Playing::onKill(Packet& packet, DWORD UserID)
{
	DWORD RoomID;
	packet >> RoomID;
	Packet sendPacket(REQ_GAME);
	sendPacket << (DWORD)KILL << UserID;

	GameLobby::getInstance()->getRoom( RoomID )->broadcast( sendPacket );
}

void CStatus_Playing::onLeftStop(Packet& packet, DWORD targetgamer)
{
	DWORD Data1, Data2, RoomID;
	float v_x, v_y;
	Packet sendPacket(REQ_GAME);
	sendPacket << targetgamer << LEFT_STOP;

	phy_step();
	
	gamer[targetgamer]->ChangeState( new LStop );

	packet >> RoomID >> Data1 >> Data2;
	gamer[targetgamer]->setxy(Data1, Data2);
	sendPacket << Data1 << Data2;

	packet >> v_x >> v_y;
	gamer[targetgamer]->impulsex(v_x);
	gamer[targetgamer]->impulsey(v_y);
	sendPacket << v_x << v_y;

	GameLobby::getInstance()->getRoom(RoomID)->broadcast(sendPacket);

	SendMeteor(RoomID);
}

void CStatus_Playing::onRightRun(Packet& packet, DWORD targetgamer)
{
	DWORD Data1, Data2, RoomID;
	float v_x, v_y;
	Packet sendPacket(REQ_GAME);
	sendPacket << targetgamer << RIGHT_RUN;

	phy_step();

	gamer[targetgamer]->ChangeState( new RRun );

	packet >> RoomID >> Data1 >> Data2;
	gamer[targetgamer]->setxy(Data1, Data2);
	sendPacket << Data1 << Data2;

	packet >> v_x >> v_y;
	gamer[targetgamer]->impulsex(v_x);
	gamer[targetgamer]->impulsey(v_y);
	sendPacket << v_x << v_y;

	GameLobby::getInstance()->getRoom(RoomID)->broadcast(sendPacket);

	SendMeteor(RoomID);
}

void CStatus_Playing::onLeftRun(Packet& packet, DWORD targetgamer)
{
	DWORD Data1, Data2, RoomID;
	float v_x, v_y;
	Packet sendPacket(REQ_GAME);
	sendPacket << targetgamer << LEFT_RUN;

	phy_step();

	gamer[targetgamer]->ChangeState( new LRun );

	packet >> RoomID >> Data1 >> Data2;
	gamer[targetgamer]->setxy(Data1, Data2);
	sendPacket << Data1 << Data2;

	packet >> v_x >> v_y;
	gamer[targetgamer]->impulsex(v_x);
	gamer[targetgamer]->impulsey(v_y);
	sendPacket << v_x << v_y;

	GameLobby::getInstance()->getRoom(RoomID)->broadcast(sendPacket);

	SendMeteor(RoomID);
}

void CStatus_Playing::onRightJump(Packet& packet, DWORD targetgamer)
{
	DWORD Data1, Data2, RoomID;
	float v_x, v_y;
	Packet sendPacket(REQ_GAME);
	sendPacket << targetgamer << RIGHT_JUMP;

	phy_step();

	gamer[targetgamer]->ChangeState( new RJump );

	packet >> RoomID >> Data1 >> Data2;
	gamer[targetgamer]->setxy(Data1, Data2);
	sendPacket << Data1 << Data2;

	packet >> v_x >> v_y;
	gamer[targetgamer]->impulsex(v_x);
	gamer[targetgamer]->impulsey(v_y);
	sendPacket << v_x << v_y;

	GameLobby::getInstance()->getRoom(RoomID)->broadcast(sendPacket);

	SendMeteor(RoomID);
}

void CStatus_Playing::onLeftJump(Packet& packet, DWORD targetgamer)
{
	DWORD Data1, Data2, RoomID;
	float v_x, v_y;
	Packet sendPacket(REQ_GAME);
	sendPacket << targetgamer << LEFT_JUMP;

	phy_step();

	gamer[targetgamer]->ChangeState( new LJump );

	packet >> RoomID >> Data1 >> Data2;
	gamer[targetgamer]->setxy(Data1, Data2);
	sendPacket << Data1 << Data2;

	packet >> v_x >> v_y;
	gamer[targetgamer]->impulsex(v_x);
	gamer[targetgamer]->impulsey(v_y);
	sendPacket << v_x << v_y;

	GameLobby::getInstance()->getRoom(RoomID)->broadcast(sendPacket);

	SendMeteor(RoomID);
}

void CStatus_Playing::onRightDash(Packet& packet, DWORD targetgamer)
{
	DWORD Data1, Data2, RoomID;
	float v_x, v_y;
	Packet sendPacket(REQ_GAME);
	sendPacket << targetgamer << RIGHT_DASH;

	phy_step();

	gamer[targetgamer]->ChangeState( new RDash );

	packet >> RoomID >> Data1 >> Data2;
	gamer[targetgamer]->setxy(Data1, Data2);
	sendPacket << Data1 << Data2;

	packet >> v_x >> v_y;
	gamer[targetgamer]->impulsex(v_x);
	gamer[targetgamer]->impulsey(v_y);
	sendPacket << v_x << v_y;

	GameLobby::getInstance()->getRoom(RoomID)->broadcast(sendPacket);

	SendMeteor(RoomID);
}

void CStatus_Playing::onLeftDash(Packet& packet, DWORD targetgamer)
{
	DWORD Data1, Data2, RoomID;
	float v_x, v_y;
	Packet sendPacket(REQ_GAME);
	sendPacket << targetgamer << LEFT_DASH;

	phy_step();

	gamer[targetgamer]->ChangeState( new LDash );

	packet >> RoomID >> Data1 >> Data2;
	gamer[targetgamer]->setxy(Data1, Data2);
	sendPacket << Data1 << Data2;

	packet >> v_x >> v_y;
	gamer[targetgamer]->impulsex(v_x);
	gamer[targetgamer]->impulsey(v_y);
	sendPacket << v_x << v_y;

	GameLobby::getInstance()->getRoom(RoomID)->broadcast(sendPacket);

	SendMeteor(RoomID);
}

void CStatus_Playing::onRightJumpDash(Packet& packet, DWORD targetgamer)
{
	DWORD Data1, Data2, RoomID;
	float v_x, v_y;
	Packet sendPacket(REQ_GAME);
	sendPacket << targetgamer << RIGHT_JUMP_DASH;

	phy_step();

	gamer[targetgamer]->ChangeState( new RJDash );

	packet >> RoomID >> Data1 >> Data2;
	gamer[targetgamer]->setxy(Data1, Data2);
	sendPacket << Data1 << Data2;

	packet >> v_x >> v_y;
	gamer[targetgamer]->impulsex(v_x);
	gamer[targetgamer]->impulsey(v_y);
	sendPacket << v_x << v_y;

	GameLobby::getInstance()->getRoom(RoomID)->broadcast(sendPacket);

	SendMeteor(RoomID);
}

void CStatus_Playing::onLeftJumpDash(Packet& packet, DWORD targetgamer)
{
	DWORD Data1, Data2, RoomID;
	float v_x, v_y;
	Packet sendPacket(REQ_GAME);
	sendPacket << targetgamer << LEFT_JUMP_DASH;

	phy_step();

	gamer[targetgamer]->ChangeState( new LJDash );

	packet >> RoomID >> Data1 >> Data2;
	gamer[targetgamer]->setxy(Data1, Data2);
	sendPacket << Data1 << Data2;

	packet >> v_x >> v_y;
	gamer[targetgamer]->impulsex(v_x);
	gamer[targetgamer]->impulsey(v_y);
	sendPacket << v_x << v_y;

	GameLobby::getInstance()->getRoom(RoomID)->broadcast(sendPacket);

	SendMeteor(RoomID);
}

GameStatus* CStatus_Playing::ChangeStatus(char* Message)
{
	if(!strcmp(Message, "GameOver"))
		return new CStatus_Over;
	return this;
}
