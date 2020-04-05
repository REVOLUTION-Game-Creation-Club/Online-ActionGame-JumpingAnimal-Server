#ifndef  __GameLobby_H
#define  __GameLobby_H

#include <list>

#include "Room.h"
#include "GameRoom.h"

#include "User.h"

class BestUser{
public:
	String	account;
	DWORD	score;
	DWORD	scoreRank;
	DWORD	killSum;
	DWORD	killRank;
};

class GameLobby : public Room
{
public:
	static	GameLobby*	getInstance();
	static	void		releaseInstance();

	virtual	bool		enter( User* user );
	virtual	void		leave( User* user );
			void		sendLobbyUserList( User* user);

			void		getRoomCount( int* cur, int* max );
			bool		addRoom(int _roomIndex, int _roomNo, String _title , User* _user );
			void		deleteRoom( int _roomNo );
			GameRoom*	getRoom(int _roomNo);
			GameRoom*	getRoomForIndex(int _index);
			int			getRoomIndex();
			int			getRoomNo();
			void		sendRoomList(User* user);

			void		getDBUserInfo(User* user, String account);
			int			getRoomListCount(){return listRoom.size();}

			BestUser&	getScoreBestUser(){return bestUser[0];}
			BestUser&	getKillBestUser(){return bestUser[1];}

			void		deleteAllRoom();			
			List		listRoom;
private:
			GameLobby();
	virtual	~GameLobby();

	static	GameLobby*	selfInstance;
			int			limitRoomCount;

			BestUser	bestUser[2];	// 랭킹 1위 정보 구조체
};

#endif