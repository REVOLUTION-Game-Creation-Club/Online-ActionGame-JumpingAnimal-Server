#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

//////////////////////////////////////////////////////////////////////////
//	REQ : 클라이언트가 서버에게 요청
//	ACK : 서버가 클라이언트에게 응답
//	NTF	: 서버가 클라이언트에게 통지
//////////////////////////////////////////////////////////////////////////
enum PACKET_TYPE{
	// 로비 패킷
	REQ_LOGIN =  100,	// 로그인
	ACK_LOGIN,
	REQ_PING,			// 핑 체크
	ACK_PING,
	NTF_CONNECT,		// 서버로 부터 접속이 됐음
	REQ_DISCONNECT,		// 서버와 접속 종료
	ACK_DISCONNECT,

	REQ_ENTER_LOBBY,	// 로비 입장
	ACK_ENTER_LOBBY,	
	REQ_LEAVE_LOBBY,	// 로비 퇴장
	ACK_LEAVE_LOBBY,
	REQ_LOBBY_USER_LIST,// 유저 리스트 
	ACK_LOBBY_USER_LIST,
	REQ_LOBBY_ROOM_LIST,// 방 리스트
	ACK_LOBBY_ROOM_LIST,
	REQ_USER_INFO,		// 유저 정보
	ACK_USER_INFO,
	REQ_USER_LOCATION,	// 유저 위치 정보
	ACK_USER_LOCATION,
	REQ_LOBBY_CHAT,		// 로비 채팅 메세지
	ACK_LOBBY_CHAT,
	REQ_MAKE_ROOM,		// 방 만들기
	ACK_MAKE_ROOM,
	REQ_DELETE_ROOM,
	ACK_DELETE_ROOM,	
	REQ_ENTER_ROOM,		// 방 입장
	ACK_ENTER_ROOM,
	REQ_RESET_ROOM,		// 방 리셋
	ACK_RESET_ROOM,

	// 게임 패킷
	REQ_WAIT,			// 게임 대기 상태 ( 게임에서 아직 돌아 오지 않은 상태 )
	ACK_WAIT,
	REQ_RESUME,			// 개임 시작 가능 상태 ( 게임에서 돌아온 상태 )
	ACK_RESUME,
	REQ_ROOM_CHAT,		// 방 채팅 메세지
	ACK_ROOM_CHAT,
	REQ_GAME_CHAT,		// 게임 채팅 메세지
	ACK_GAME_CHAT,
	REQ_ROOM_USER_LIST,	// 방의 유저 정보
	ACK_ROOM_USER_LIST,
	REQ_GAME_MODE_CHANGE,// 게임 방식 변경
	ACK_GAME_MODE_CHANGE,
	REQ_CHARACTER_SELECT,// 캐릭터 선택
	ACK_CHARACTER_SELECT,
	REQ_GAME_READY,			// 게임 준비 
	ACK_GAME_READY,
	REQ_GAME_START,		// 게임 시작 
	ACK_GAME_START,
	REQ_GAME_QUIT,		// 종료
	ACK_GAME_QUIT,
	REQ_KILL_SUM,		// 킬수
	ACK_KILL_SUM,		
	REQ_TEAM_KILL,		// 팀킬
	ACK_TEAM_KILL,
	REQ_KICK,			// 강퇴
	ACK_KICK,
	REQ_LEAVE_ROOM,		// 방 퇴장
	ACK_LEAVE_ROOM,
	REQ_TEAM_SELECT,
	ACK_TEAM_SELECT,
	REQ_MAP_CHANGE,
	ACK_MAP_CHANGE,
	REQ_CHANGE_MAJOR,	// 방장 권한 
	ACK_CHANGE_MAJOR,

	REQ_GAME,
};

enum GAME_MOVING_PACKET_TYPE{
	INIT_GAMESTATE = 149,
	
	RIGHT_STOP = 150,
	RIGHT_RUN,
	RIGHT_JUMP,
	RIGHT_DASH,
	RIGHT_JUMP_DASH,
	
	LEFT_STOP,
	LEFT_RUN,
	LEFT_JUMP,
	LEFT_DASH,
	LEFT_JUMP_DASH,
	DIE,

	METEOR_GOOD,
	METEOR_BAD,

	TIME_OVER,
	KILL,
};

#endif