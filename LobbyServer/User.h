#ifndef  __User_H_
#define  __User_H_

#include "Session.h"
#include "GameLobby.h"

class User : public Session
{
	friend class SessionManager;
public:
			int				getIndex(){return index;}
			void			setIndex(int _index){ index = _index;}

			String			getAccount(){ return myAccount; }

			void			setUserScore(DWORD score){ score = score; }
			void			setUserKillSum(DWORD killSum){	killSum = killSum; }
			void			setUserScoreRank(DWORD scoreRank){ scoreRank = scoreRank; }
			void			setUserKillRank(DWORD killRank){ killRank = killRank; }
			DWORD			getUserScore(){ return score;	}
			DWORD			getUserKillSum(){ return killSum;	}
			DWORD			getUserScoreRank(){ return scoreRank; }
			DWORD			getUserKillRank(){ return killRank; }

			int				getSlotNo(){ return mySlotNo; }
			void			setSlotNo( int _mySlotNo ){ mySlotNo = _mySlotNo; }

			int				getRoomNo(){ return myRoomNo; }
			void			setRoomNo(int _myRoomNo){ myRoomNo = _myRoomNo; }

			bool			getReady() {return ready; }
			void			setReady( bool _ready ){ ready = _ready; }

			int				getCharacter() {return character; }
			void			setCharacter( int _character ){ character = _character; }

			int				getTeam() { return team; }
			void			setTeam( int _team ){ team = _team; }

			int				getLocation() { return location; }
			void			setLocation( int _location ){ location = _location; }
protected:
			User( SOCKET socketValue, struct sockaddr FAR* addr = 0 );
	virtual	~User();

	virtual void			onCreate();
			void			onDestroy();

	virtual	void			packetParsing( Packet& packet );

private:
			void			onReqLogin(Packet& packet);
			void			onReqPing(Packet& packet);
			void			onReqDisconnect(Packet& packet);

			void			onReqEnterLobby(Packet& packet);
			void			onReqLobbyUserList(Packet& packet);
			void			onReqLobbyRoomList(Packet& packet);
			void			onReqUserInfo(Packet& packet);
			void			onReqLobbyChat(Packet& packet);
			void			onReqRoomChat(Packet& packet);
			void			onReqGameChat(Packet& packet);
			void			onReqMakeRoom(Packet& packet );
			void			onReqEnterRoom(Packet& packet);
			void			onReqLeaveRoom(Packet& packet);
			void			onReqDeleteRoom(Packet& packet);
			void			onReqLeaveLobby(Packet& packet);
			void			onReqResetRoom(Packet& packet);
			void			onReqGameReady(Packet& packet );
			void			onReqGameStart(Packet& packet );
			void			onReqGameQuit(Packet& packet );
			void			onReqRoomUserList(Packet& packet );
			void			onReqGameMode(Packet& packet );
			void			onReqCharacterSelect(Packet& packet );
			void			onReqTeamSelect(Packet& packet );
			void			onReqMapChange(Packet& packet );
			void			onReqKillSum(Packet& packet );
			void			onReqTeamKill(Packet& packet );
			void			onReqKick(Packet& packet );
			void			onReqChangeMajor(Packet& packet );
	
			void			onReqGame(Packet& packet);

			int				index;
			String			myAccount;
			int				myRoomIndex;	
			int				myRoomNo;
			int				mySlotNo;
			int				location;

			bool			ready;
			int				character;
			int				team;

			DWORD			killSum;			// ´©ÀûÅ³
			DWORD			killRank;			// ´©ÀûÅ³ ·©Å©
			DWORD			score;				// ´©Àû½ÂÁ¡
			DWORD			scoreRank;			// ´©Àû½ÂÁ¡ ·©Å©
};

#endif