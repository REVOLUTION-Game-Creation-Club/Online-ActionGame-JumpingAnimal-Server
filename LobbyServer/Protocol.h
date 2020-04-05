#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

//////////////////////////////////////////////////////////////////////////
//	REQ : Ŭ���̾�Ʈ�� �������� ��û
//	ACK : ������ Ŭ���̾�Ʈ���� ����
//	NTF	: ������ Ŭ���̾�Ʈ���� ����
//////////////////////////////////////////////////////////////////////////
enum PACKET_TYPE{
	// �κ� ��Ŷ
	REQ_LOGIN =  100,	// �α���
	ACK_LOGIN,
	REQ_PING,			// �� üũ
	ACK_PING,
	NTF_CONNECT,		// ������ ���� ������ ����
	REQ_DISCONNECT,		// ������ ���� ����
	ACK_DISCONNECT,

	REQ_ENTER_LOBBY,	// �κ� ����
	ACK_ENTER_LOBBY,	
	REQ_LEAVE_LOBBY,	// �κ� ����
	ACK_LEAVE_LOBBY,
	REQ_LOBBY_USER_LIST,// ���� ����Ʈ 
	ACK_LOBBY_USER_LIST,
	REQ_LOBBY_ROOM_LIST,// �� ����Ʈ
	ACK_LOBBY_ROOM_LIST,
	REQ_USER_INFO,		// ���� ����
	ACK_USER_INFO,
	REQ_USER_LOCATION,	// ���� ��ġ ����
	ACK_USER_LOCATION,
	REQ_LOBBY_CHAT,		// �κ� ä�� �޼���
	ACK_LOBBY_CHAT,
	REQ_MAKE_ROOM,		// �� �����
	ACK_MAKE_ROOM,
	REQ_DELETE_ROOM,
	ACK_DELETE_ROOM,	
	REQ_ENTER_ROOM,		// �� ����
	ACK_ENTER_ROOM,
	REQ_RESET_ROOM,		// �� ����
	ACK_RESET_ROOM,

	// ���� ��Ŷ
	REQ_WAIT,			// ���� ��� ���� ( ���ӿ��� ���� ���� ���� ���� ���� )
	ACK_WAIT,
	REQ_RESUME,			// ���� ���� ���� ���� ( ���ӿ��� ���ƿ� ���� )
	ACK_RESUME,
	REQ_ROOM_CHAT,		// �� ä�� �޼���
	ACK_ROOM_CHAT,
	REQ_GAME_CHAT,		// ���� ä�� �޼���
	ACK_GAME_CHAT,
	REQ_ROOM_USER_LIST,	// ���� ���� ����
	ACK_ROOM_USER_LIST,
	REQ_GAME_MODE_CHANGE,// ���� ��� ����
	ACK_GAME_MODE_CHANGE,
	REQ_CHARACTER_SELECT,// ĳ���� ����
	ACK_CHARACTER_SELECT,
	REQ_GAME_READY,			// ���� �غ� 
	ACK_GAME_READY,
	REQ_GAME_START,		// ���� ���� 
	ACK_GAME_START,
	REQ_GAME_QUIT,		// ����
	ACK_GAME_QUIT,
	REQ_KILL_SUM,		// ų��
	ACK_KILL_SUM,		
	REQ_TEAM_KILL,		// ��ų
	ACK_TEAM_KILL,
	REQ_KICK,			// ����
	ACK_KICK,
	REQ_LEAVE_ROOM,		// �� ����
	ACK_LEAVE_ROOM,
	REQ_TEAM_SELECT,
	ACK_TEAM_SELECT,
	REQ_MAP_CHANGE,
	ACK_MAP_CHANGE,
	REQ_CHANGE_MAJOR,	// ���� ���� 
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