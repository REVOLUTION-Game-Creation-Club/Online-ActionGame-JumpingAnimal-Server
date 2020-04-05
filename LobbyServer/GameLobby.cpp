#include "cpp/lang/Synchronized.h"
#include "Room.h"
#include "GameLobby.h"
#include "GameRoom.h"
#include "User.h"
#include "Protocol.h"
#include "ODBCManager.h"

GameLobby*		GameLobby::selfInstance = 0;

GameLobby::GameLobby()
	: Room( 0, 0, String( _T( "Game Lobby" ) ), 10 )
{
}

GameLobby::~GameLobby()
{
}

GameLobby* GameLobby::getInstance()
{
	if( selfInstance == 0 )
		selfInstance = new GameLobby();

	return selfInstance;
}

void GameLobby::releaseInstance()
{

	if( selfInstance == 0 )
		return;

	delete selfInstance;
	selfInstance = 0;
}

bool GameLobby::enter( User* user )
{
	if( addUser( user ) == false )
		return false;	
	return true;
}

void GameLobby::leave( User* user )
{
	deleteUser( user );
	Packet sendPacket( ACK_LEAVE_LOBBY );
	sendPacket << user->getAccount();
	broadcast( sendPacket );
}

void GameLobby::getRoomCount( int* cur, int* max )
{
	*cur = listRoom.size();
	*max = limitRoomCount;
}

bool GameLobby::addRoom( int _roomIndex, int _roomNo, String _title , User* _user)
{
	if( listRoom.size() >= limitCount )
		return false;
	
	Synchronized sync( &listRoom );

	GameRoom* gameRoom = new GameRoom( _roomIndex, _roomNo, _title , 4);		// 게임 방을 생성한다.
	listRoom.pushTail( gameRoom );

	return true;
}

void GameLobby::deleteRoom( int _roomNo )
{
	int ssIndex = 0;
	List::Position pos, posPrev;

	Synchronized sync( &listRoom );
	pos = listRoom.headPosition();
	while( listRoom.isEnd( pos ) == false )
	{
		posPrev = pos;

		GameRoom* ss = ( GameRoom* )listRoom.getItem( pos++ );

		if( _roomNo == ss->getRoomNo() )
		{
			ssIndex = ss->getRoomIndex();
			delete ss;

			listRoom.removeItem( posPrev );
			break;
		}
	}

	pos = listRoom.headPosition();
	while( listRoom.isEnd( pos ) == false )
	{
		posPrev = pos;

		GameRoom* ss = ( GameRoom* )listRoom.getItem( pos++ );	
		if(ssIndex < ss->getRoomIndex())
		{
			int index = ss->getRoomIndex();
			--index;
			ss->setRoomIndex(index);
		}
	}
}

void GameLobby::deleteAllRoom()
{
	Synchronized sync( &listRoom );
	listRoom.clear();
}

GameRoom* GameLobby::getRoom(int _roomNo)
{
	List::Position pos, posPrev;

	Synchronized sync( &listRoom );
	pos = listRoom.headPosition();
	while( listRoom.isEnd( pos ) == false )
	{
		posPrev = pos;

		GameRoom* ss = ( GameRoom* )listRoom.getItem( pos++ );

		if( _roomNo == ss->getRoomNo() )
		{
			return ss;
		}
	}

	return NULL;
}

GameRoom* GameLobby::getRoomForIndex(int _index)
{
	List::Position pos, posPrev;
	int count = 0;
	GameRoom* ss = NULL;
	Synchronized sync( &listRoom );
	pos = listRoom.headPosition();

	while( listRoom.isEnd( pos ) == false )
	{
		posPrev = pos;

		GameRoom* ss = ( GameRoom* )listRoom.getItem( pos++ );

		if( _index == ss->getRoomIndex() )
		{
			return ss;
		}
	}

	return ss;
}
// MakeRoom 할때 인덱스를 얻기 위해서 사용..
int GameLobby::getRoomIndex()
{
	return listRoom.size();
}

int	GameLobby::getRoomNo()
{
	int roomNo = 0;
	List ::Position pos, posPrev;

	Synchronized sync( &listRoom );

	while(1)
	{
		int count = 0;

		pos = listRoom.headPosition();
		while( listRoom.isEnd( pos ) == false )
		{
			posPrev = pos;

			GameRoom* ss = ( GameRoom* )listRoom.getItem( pos++ );

			if(roomNo != ss->getRoomNo())
				count++;
		}

		if(count == listRoom.size())
			return roomNo;
		roomNo++;
	}
}

void GameLobby::sendRoomList(User* user)
{
	List::Position pos;
	Packet sendPacket( ACK_LOBBY_ROOM_LIST );

	Synchronized sync( &listRoom );

	sendPacket << listRoom.size();

	pos = listRoom.headPosition();
	while( listRoom.isEnd( pos ) == false )
	{
		GameRoom* find = ( GameRoom* )listRoom.getItem( pos++ );

		sendPacket << find->getRoomIndex();
		sendPacket << find->getRoomNo();
		sendPacket << find->getTitle();
		sendPacket << find->getUserCount();
	}

	user->sendMessage( sendPacket );
}

void GameLobby::sendLobbyUserList( User* user)
{
	List::Position pos;
	Packet sendPacket( ACK_LOBBY_USER_LIST );

	Synchronized sync( &listUser );

	sendPacket << listUser.size();

	pos = listUser.headPosition();
	while( listUser.isEnd( pos ) == false )
	{
		User* find = ( User* )listUser.getItem( pos++ );
		sendPacket << find->getAccount();
		sendPacket << find->getLocation();
	}

	broadcast( sendPacket );
}

void GameLobby::getDBUserInfo(User* user, String account )
{
	List::Position pos;

	Synchronized sync( &listUser);
	pos = listUser.headPosition();
	while( listUser.isEnd( pos ) == false )
	{
		User* find = ( User* )listUser.getItem( pos++ );

		if( find->getAccount() == account )
		{
			CODBCManager::getInstance()->getUserInfo( *find );
			CODBCManager::getInstance()->UpdateUserInfo( *find );

			Packet sendPacket( ACK_USER_INFO );

			sendPacket << find->getUserScore();
			sendPacket << find->getUserScoreRank();
			sendPacket << find->getUserKillSum();
			sendPacket << find->getUserKillRank();

			user->sendMessage( sendPacket );

			break;
		}
	}
}