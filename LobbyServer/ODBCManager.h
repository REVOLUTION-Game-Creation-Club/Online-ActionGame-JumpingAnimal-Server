/************************************************************************/
/*	ODBCManager By DaeWook
	사용법 : (뜨끔;;;)
	랭킹시스템.plx 참조
* 경고 *
	대욱이가 공부한 내용을 바탕으로 만들었음으로 다른 사람들이 보기에는 이해하기 힘들수도 있음	
/************************************************************************/
#pragma once

#include <windows.h>
#include <sql.h>
#include <sqlExt.h>
#include <MMSystem.h>
#include "User.h"

#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "odbc32.lib" )

extern CRITICAL_SECTION g_csBestUser;

class CODBCManager
{
public:
	CODBCManager(void);
	~CODBCManager(void);

	/************************************************************************/
	/* DB연결 설정 & 연결하는 함수 RETURN 연결 성공 유무                    */
	/************************************************************************/
	BOOL DBConnect(void);

	/************************************************************************/
	/* DB연결 해제                                                          */
	/************************************************************************/
	static void DBDisConnect(void);

	/************************************************************************/
	/* SQL 실행 & 결과값 저장                                               */
	/************************************************************************/
	//BOOL DBExecuteSQL(char*); 사용 중단
	//BOOL DBTop10SQL( DWORD );
	//BOOL DBUserInfo( char*, DWORD &, DWORD & , DWORD);
	//BOOL DBUserInfoWithCmpScore( char*, DWORD &, DWORD &, DWORD );	지금은 사용하지 않음

	/************************************************************************/
	/* 쿼리 해석기
	/************************************************************************/
	//void QueryAnalizer(char*);

	/************************************************************************/
	/* ThreadFunction
	/************************************************************************/
	//static DWORD WINAPI UpdateThreadFunc( LPVOID temp );
	static unsigned int WINAPI RankThreadFunc( LPVOID temp );

	/************************************************************************/
	/* update시간조정
	/************************************************************************/
//	void setdbupdatetime(DWORD);
	void set1stupdatetime(DWORD);

	/************************************************************************/
	/* 유저정보를 채워줌
	/************************************************************************/
	BOOL getUserInfo( User &_target );

	/************************************************************************/
	/*	유저 정보를 업데이트
	/************************************************************************/
	BOOL UpdateUserInfo( User &_target );

private:
	static SQLHENV hEnv;		// 환경 핸들
	static SQLHDBC hDbc;		// 연결 핸들
	static SQLHSTMT hStmt;		// 명령 핸들
	
	SQLRETURN sRet;		// DB Connect시 반환값

	static CODBCManager* selfInstance;	// this 포인터

public:
	//DWORD updateThreadID;
	//HANDLE hUpdateThread;
	DWORD rankThreadID;
	static HANDLE hRankThread;
	//DWORD CurrentTime;
	DWORD check1stCurrentTime;
	//DWORD dbUpdateTime;	// UpdateList를 업데이트하는 시간
	DWORD check1stTime;		// 1위 유저를 갱신하는 시간
	char strCommand[256];	// Query를 담을 char 배열
	static CODBCManager* getInstance();
	static void releaseInstance();
	bool initialize();
};