#ifndef  __GAME_ROOM_H_
#define  __GAME_ROOM_H_

#include "Room.h"

class GameRoom : public Room
{
private:
	bool		playingGame;		// 게임 중
	bool		isWait;				// 기다리는 중
	int			gameMode;			// 게임모드
	int			mapKind;			// 맵 종류
	
	User*		master;

public:
				GameRoom(int _roomIndex, int _roomNo, String _title, int _lc);
	virtual		~GameRoom();

	virtual	bool		enter( User* user );
	virtual	void		leave( User* user );
			void		sendRoomUserList( User* user);

			bool		startGame();
			bool		getPlayingGame(){ return playingGame; }
			void		setPlayingGame(bool _playingGame){ playingGame = _playingGame; }
			bool		isWaited(){ return isWait; }
			int			getMapKind(){return mapKind; }
			void		setMapKind(int _mapKind){ mapKind = _mapKind; }
			int			getGameMode(){return gameMode; }
			void		setGameMode(int _gameMode){ gameMode = _gameMode; }
			int			getEmptySlotNo();

			bool		slot[4];
};

#endif