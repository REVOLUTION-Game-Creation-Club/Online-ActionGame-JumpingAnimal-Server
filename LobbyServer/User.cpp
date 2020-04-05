#include "User.h"
#include "GameLobby.h"
#include "GameRoom.h"
#include "Protocol.h"
#include ".\user.h"
#include "GM.h"

User::User( SOCKET socketValue, struct sockaddr FAR* addr )
	: Session( socketValue, addr )
{
	index			=	0;
	myRoomNo		=	0;
	mySlotNo			=	0;
	location		=	0;

	ready			=	false;
	character		=	0;
	team			=	0;

	killSum			=	0;			// ����ų
	killRank		=	0;			// ����ų ��ũ
	score			=	0;			// ��������
	scoreRank		=	0;			// �������� ��ũ
}

User::~User()
{
}

void User::onCreate()
{
	Session::onCreate();

	sendMessage( NTF_CONNECT );
}

void User::onDestroy()
{
	GameLobby::getInstance()->leave( this );
	Session::onDestroy();
}

void User::packetParsing( Packet& packet )
{
	try
	{
		switch( packet.id() )
		{
			case	REQ_LOGIN:				onReqLogin( packet );			break;	// �α���
			case	REQ_PING:				onReqPing( packet );			break;	// �� üũ
			case	REQ_ENTER_LOBBY:		onReqEnterLobby( packet );		break;	// �κ� ����
			case	REQ_LOBBY_USER_LIST:	onReqLobbyUserList( packet );	break;	// ���� ����Ʈ 
			case	REQ_LOBBY_ROOM_LIST:	onReqLobbyRoomList( packet );	break;	// �� ����Ʈ
			case	REQ_USER_INFO:			onReqUserInfo( packet );		break;	// ���� ����
			case	REQ_LOBBY_CHAT:			onReqLobbyChat( packet );		break;	// ä�ø޼���
			case	REQ_ROOM_CHAT:			onReqRoomChat( packet );		break;	// ä�ø޼���			
			case	REQ_MAKE_ROOM:			onReqMakeRoom( packet );		break;	// �� �����
			case	REQ_ENTER_ROOM:			onReqEnterRoom( packet );		break;	// �� ����
			case	REQ_DELETE_ROOM:		onReqDeleteRoom( packet );		break;	// �� ����
			case	REQ_LEAVE_ROOM:			onReqLeaveRoom( packet	);		break;	// �� ����
			case	REQ_RESET_ROOM:			onReqResetRoom( packet );		break;	// �� ��ü ����
			case	REQ_LEAVE_LOBBY:		onReqLeaveLobby( packet );		break;	// ���� ����
			case	REQ_WAIT:												break;	// ���� ��� ���� ( ���ӿ��� ���� ���� ���� ���� ���� )
			case	REQ_RESUME:												break;	// ���� ���� ���� ���� ( ���ӿ��� ���ƿ� ���� )
			case	REQ_GAME_READY:			onReqGameReady( packet );			break;	// ���� �غ� 
			case	REQ_GAME_START:			onReqGameStart( packet );			break;	// ���� ����
			case	REQ_GAME_QUIT:			onReqGameQuit( packet );			break;	// ���� ����
			case	REQ_ROOM_USER_LIST:		onReqRoomUserList( packet );	break;	// ���� ���� ����
			case	REQ_GAME_MODE_CHANGE:	onReqGameMode( packet );		break;	// ���� ��� ����
			case	REQ_CHARACTER_SELECT:	onReqCharacterSelect( packet );	break;	// ĳ���� ����
			case	REQ_TEAM_SELECT:		onReqTeamSelect( packet );		break;	// �� ����
			case	REQ_MAP_CHANGE:			onReqMapChange( packet );		break;	// �� ����
			case	REQ_KILL_SUM:			onReqKillSum( packet );			break;	// ų��
			case	REQ_TEAM_KILL:			onReqTeamKill( packet );		break;	// ��ų
			case	REQ_KICK:				onReqKick( packet );			break;	// ����
			case	REQ_CHANGE_MAJOR:		onReqChangeMajor( packet );		break;	// ���� ���� 
			
			case	REQ_GAME:				onReqGame( packet );			break;	//���� ���� ó��
			case	REQ_GAME_CHAT:			onReqGameChat( packet );		break;	// ����ä��
		}
	}
	catch( InvalidPacketException* ex )
	{
		delete ex;
	}
//	catch( ... )
	{
//		DebugAssert( false );
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////  Game server - Game client  ///////////////////////////////////////////

void User::onReqLogin(Packet& packet)
{

}
void User::onReqPing(Packet& packet)
{

}
void User::onReqDisconnect(Packet& packet)
{

}
void User::onReqMakeRoom(Packet& packet)
{
	Packet sendPacket( ACK_MAKE_ROOM );
	String title;
	String account;
	int	roomIndex, roomNo;
	bool error;
	packet >> title;
	roomIndex = GameLobby::getInstance()->getRoomIndex();
	roomNo = GameLobby::getInstance()->getRoomNo();				//  ���� ��ȣ�� �޾ƿ´�.		
	error = GameLobby::getInstance()->addRoom(roomIndex, roomNo, title, this );		// �κ� ���� �߰�
	sendPacket <<(bool)false << roomIndex << roomNo << title<< (int)0;
	GameLobby::getInstance()->broadcast(sendPacket, 0, true );

	sendPacket.clear();
	sendPacket.id(ACK_MAKE_ROOM);
	sendPacket <<(bool)true << roomIndex << roomNo << title<< (int)0;
	sendMessage(sendPacket);
}

void User::onReqEnterLobby(Packet& packet)
{
	packet >> myAccount;
	
	if(	GameLobby::getInstance()->enter( this ) == false )
		throw ( new SessionAbortException() );

	Packet sendPacket( ACK_ENTER_LOBBY );
	GameLobby::getInstance()->broadcast( sendPacket );

	location = 2;
}

void User::onReqLobbyUserList(Packet& packet)
{
	GameLobby::getInstance()->sendLobbyUserList( this );
}

void User::onReqRoomUserList(Packet& packet )
{
	GameLobby::getInstance()->getRoom(myRoomNo)->sendRoomUserList( this );
}

void User::onReqLobbyRoomList(Packet& packet)
{
	GameLobby::getInstance()->sendRoomList( this );
}

void User::onReqUserInfo(Packet& packet)
{
	String account;
	packet >> account;
	GameLobby::getInstance()->getDBUserInfo( this, account);
}

void User::onReqLobbyChat(Packet& packet)
{
	Packet sendPacket( ACK_LOBBY_CHAT );
	String message;
	packet >> message;
	sendPacket << myAccount << message;
	GameLobby::getInstance()->broadcast( sendPacket, 0, true );
}

void User::onReqRoomChat(Packet& packet)
{
	Packet sendPacket( ACK_ROOM_CHAT );
	String message;

	packet >> message;
	sendPacket << myAccount << message;
	GameLobby::getInstance()->getRoom(myRoomNo)->broadcast( sendPacket, 0, true);
}

void User::onReqGameChat(Packet& packet)
{
	Packet sendPacket( ACK_GAME_CHAT);
	String account,message;
	packet >> account >> message;
	sendPacket << account << message;
	GameLobby::getInstance()->getRoom(myRoomNo)->broadcast( sendPacket, 0, true);
}

void User::onReqEnterRoom(Packet& packet)
{
	Packet sendPacket(ACK_ENTER_ROOM);
	packet >> myRoomIndex >> myRoomNo;
	if(	GameLobby::getInstance()->getRoom(myRoomNo)->getPlayingGame() == true)
	{
		sendPacket << (bool)true;
		sendMessage(sendPacket);
	}
	else
	{
        GameLobby::getInstance()->getRoom(myRoomNo)->enter(this);
		location = 3;
	}
}

void User::onReqLeaveRoom(Packet& packet)
{
	int roomIndex, roomNo, slotNo;
	int userCount;
	packet >> roomIndex >>roomNo >> slotNo;

	// �� ���� ���� �Ѹ� ���δ�.
	userCount = GameLobby::getInstance()->getRoom(roomNo)->getUserCount();
	--userCount;
	GameLobby::getInstance()->getRoom(roomNo)->setUserCount(userCount);

	GameLobby::getInstance()->getRoom(roomNo)->slot[slotNo] = false;
	GameLobby::getInstance()->getRoom(roomNo)->leave(this);

	Packet sendPacket( ACK_LEAVE_ROOM );
	sendPacket <<(bool)false<< roomIndex << roomNo << slotNo << userCount;
	GameLobby::getInstance()->broadcast( sendPacket );

	sendPacket.clear();
	sendPacket.id( ACK_LEAVE_ROOM);
	sendPacket <<(bool)true<< roomIndex << roomNo << slotNo << userCount;
	sendMessage(sendPacket);

	location = 2;
}

void User::onReqDeleteRoom(Packet& packet)
{
	int roomIndex, roomNo;
	packet >> roomIndex >> roomNo;
	GameLobby::getInstance()->deleteRoom(roomNo);

	Packet sendPacket( ACK_DELETE_ROOM );
	sendPacket << roomIndex << roomNo;
	GameLobby::getInstance()->broadcast( sendPacket );
}

void User::onReqResetRoom(Packet& packet)
{
	GameLobby::getInstance()->deleteAllRoom();
	Packet sendPacket( ACK_RESET_ROOM );
	GameLobby::getInstance()->broadcast( sendPacket );

}

void User::onReqLeaveLobby(Packet& packet)
{
	GameLobby::getInstance()->leave(this);
}

void User::onReqGameReady(Packet& packet )
{
	int slotNo;
	bool b;
	packet >> slotNo >> b;
	ready = !b;
	Packet sendPacket( ACK_GAME_READY );
	sendPacket << slotNo;
	sendPacket << ready;
	GameLobby::getInstance()->getRoom(myRoomNo)->broadcast( sendPacket, 0, true);
}

void User::onReqGameStart(Packet& packet )
{
	Packet sendPacket( ACK_GAME_START );
	GameLobby::getInstance()->getRoom(myRoomNo)->setPlayingGame(true);
	GameLobby::getInstance()->getRoom(myRoomNo)->broadcast( sendPacket, 0, true);

	//CGM::GetGameManager()->CreateGame(packet);
	location = 4;
	//����
	Packet GamePacket(REQ_GAME);
	GamePacket << 0 << INIT_GAMESTATE << GameLobby::getInstance()->getRoom(myRoomNo)->getMapKind() << GameLobby::getInstance()->getRoom(myRoomNo)->getUserCount();
	int i;
	char buff[10];
	for( i = 0 ; i < GameLobby::getInstance()->getRoom(myRoomNo)->getUserCount() ; i++)
	{
		sprintf(buff, "%dP", i+1);
		GamePacket << buff;
	}
	i = CGM::GetGameManager()->CreateGame(GamePacket);
    GamePacket << i;
	GameLobby::getInstance()->getRoom(myRoomNo)->broadcast( GamePacket, 0, true);

}
void User::onReqGameQuit(Packet& packet )
{
	DWORD GameID;
	packet >> GameID;
	GameLobby::getInstance()->getRoom(myRoomNo)->setPlayingGame(false);
	Packet sendPacket( ACK_GAME_QUIT );
	CGM::GetGameManager()->DeleteGame(GameID);
	GameLobby::getInstance()->getRoom(myRoomNo)->broadcast( sendPacket, 0, true);
	location = 3;
}

void User::onReqGameMode(Packet& packet )
{
	int slotNo;
	int gameMode;
	packet >> slotNo >> gameMode;
	if( gameMode < 2)
		++gameMode;
	else
		gameMode = 0;
	Packet sendPacket( ACK_GAME_MODE_CHANGE );
	sendPacket << slotNo << gameMode;
	GameLobby::getInstance()->getRoom(myRoomNo)->setGameMode(gameMode);
	GameLobby::getInstance()->getRoom(myRoomNo)->broadcast( sendPacket, 0, true);
}

void User::onReqCharacterSelect(Packet& packet )
{
	int slotNo;
	packet >> slotNo >> character ;
	Packet sendPacket( ACK_CHARACTER_SELECT );
	sendPacket << slotNo << character;
	GameLobby::getInstance()->getRoom(myRoomNo)->broadcast( sendPacket, 0, true);
}
void User::onReqTeamSelect(Packet& packet )
{
	int slotNo;
	packet >> slotNo >> team;
	Packet sendPacket( ACK_TEAM_SELECT );
	sendPacket << slotNo << team;
	GameLobby::getInstance()->getRoom(myRoomNo)->broadcast( sendPacket, 0, true);
}
void User::onReqMapChange(Packet& packet )
{
	int slotNo, mapKind;
	packet >> slotNo >> mapKind;
	Packet sendPacket( ACK_MAP_CHANGE );
	if( mapKind == 0)
		mapKind = 1;
	else
		mapKind = 0;
	sendPacket << slotNo << mapKind;
	GameLobby::getInstance()->getRoom(myRoomNo)->setMapKind(mapKind);
	GameLobby::getInstance()->getRoom(myRoomNo)->broadcast( sendPacket, 0, true);
}
void User::onReqKillSum(Packet& packet )
{
}
void User::onReqTeamKill(Packet& packet )
{
}
void User::onReqKick(Packet& packet )
{
}
void User::onReqChangeMajor(Packet& packet )
{
}

void User::onReqGame(Packet& packet)
{
	CGM::GetGameManager()->ProcessPacket(packet);
}
