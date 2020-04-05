#ifndef  __LOBBY_H_
#define  __LOBBY_H_

#include "cpp/lang/String.h"
#include "cpp/util/List.h"
#include "cpp/net/Packet.h"
#include "Protocol.h"

class User;
class Room : public Object
{
public:
	virtual	bool		enter( User* user ) = 0;
	virtual	void		leave( User* user ) = 0;
			int			getRoomNo(){ return roomNo; }
			void		setRoomNo(int _roomNo){ roomNo = _roomNo; }
			int			getRoomIndex(){ return roomIndex; }
			void		setRoomIndex(int _roomIndex){ roomIndex = _roomIndex; }
			String		getTitle(){return title; }
			void		setTitle(String _title){ title = _title; }
			void		getUserCount( int* cur, int* max );
			int			getUserCount() { return userCount;}
			void		setUserCount(int _userCount) { userCount = _userCount;	}
			bool		addUser( User* user );
			void		deleteUser( User* user );
			void		broadcast( Packet& packet, User* except = 0, bool ttcp = false );
			List*		getUserList();
protected:
			Room( int _roomIndex, int _roomNo, String& _t, int _lc );
	virtual	~Room();
			int			roomIndex;	// 순서
			int			roomNo;		// 고유 번호
			String		title;
			int			userCount;
			int			limitCount;
			List		listUser;
};

#endif