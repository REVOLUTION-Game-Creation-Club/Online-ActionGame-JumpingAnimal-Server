#include "cpp/util/List.h"

#include "Room.h"
#include "User.h"
#include "Protocol.h"

Room::Room(int _roomIndex, int _roomNo, String& _t, int _lc)
{
	roomIndex = _roomIndex;
	roomNo = _roomNo;
	title = _t;
	userCount = 0;
	limitCount = _lc;
}

Room::~Room()
{
}

void Room::getUserCount( int* cur, int* max )
{
	*cur = listUser.size();
	*max = limitCount;
}

bool Room::addUser( User* user )
{
	if( listUser.size() >= limitCount )
		return false;
	Synchronized sync( &listUser );

	listUser.pushTail( user );

	return true;
}

void Room::deleteUser( User* user )
{
	List::Position pos, posPrev;
	Synchronized sync( &listUser );

	pos = listUser.headPosition();
	while( listUser.isEnd( pos ) == false )
	{
		posPrev = pos;

		User* ss = ( User* )listUser.getItem( pos++ );

		if( user == ss )
		{
			listUser.removeItem( posPrev );
			break;
		}
	}
}

void Room::broadcast( Packet& packet, User* except, bool ttcp )
{
	List::Position pos;

	Synchronized sync( &listUser );
	pos = listUser.headPosition();
	while( listUser.isEnd( pos ) == false )
	{
		User* ss = ( User* )listUser.getItem( pos++ );

		if( ss == except )
			continue;

		ss->sendMessage( packet, ttcp );
	}
}