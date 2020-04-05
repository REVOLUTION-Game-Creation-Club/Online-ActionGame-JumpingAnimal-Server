#include "cpp/lang/Synchronized.h"

#include "GameRoom.h"
#include "User.h"
#include "Protocol.h"


GameRoom::GameRoom(int _roomIndex, int _roomNo, String _title ,int _lc)
	: Room(_roomIndex,  _roomNo, String( _title ), _lc )
{
	for(int i = 0; i<4; i++)
		slot[i] = false;
	mapKind = 0;
	gameMode = 0;
}

GameRoom::~GameRoom()
{	
}

int GameRoom::getEmptySlotNo()
{
	for(int i = 0; i<4; i++)
	{
		if( slot[i] == false )
			return i;
	}

	return -1;
}
bool GameRoom::enter( User* user )
{
	bool error = false;
	int slotNo;
	Packet sendPacket(ACK_ENTER_ROOM);


	slotNo = getEmptySlotNo();
	user->setSlotNo(slotNo);

	slot[slotNo] = true;

	if( addUser( user ) == false )
		return false;
	++userCount;
	if( slotNo == -1)
		return false;		

	sendPacket << error << (bool) true << getRoomIndex() << user->getRoomNo() << getTitle() << slotNo << user->getAccount() << userCount;
	user->sendMessage(sendPacket);
	sendPacket.clear();
	sendPacket.id(ACK_ENTER_ROOM) << error << (bool)false << getRoomIndex() << user->getRoomNo() << getTitle() << slotNo << user->getAccount() << userCount;
	broadcast( sendPacket , 0, true);
	return true;
}

void GameRoom::leave( User* user )
{
	deleteUser( user );
}
void GameRoom::sendRoomUserList( User* user)
{
	Packet sendPacket( ACK_ROOM_USER_LIST );
	List::Position pos;

	Synchronized sync( &listUser );

	sendPacket << userCount;

	pos = listUser.headPosition();
	while( listUser.isEnd( pos ) == false )
	{
		User* find = ( User* )listUser.getItem( pos++ );
		sendPacket << find->getSlotNo();
		sendPacket << find->getTeam();
		sendPacket << find->getCharacter();
		sendPacket << find->getAccount();
	}

	user->sendMessage( sendPacket );
}

bool GameRoom::startGame()
{
	return true;
}