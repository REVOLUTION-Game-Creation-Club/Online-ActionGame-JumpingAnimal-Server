/************************************************************************/
/*	ODBCManager By DaeWook
	���� : (�߲�;;;)
	��ŷ�ý���.plx ����
* ��� *
	����̰� ������ ������ �������� ����������� �ٸ� ������� ���⿡�� �����ϱ� ������� ����	
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
	/* DB���� ���� & �����ϴ� �Լ� RETURN ���� ���� ����                    */
	/************************************************************************/
	BOOL DBConnect(void);

	/************************************************************************/
	/* DB���� ����                                                          */
	/************************************************************************/
	static void DBDisConnect(void);

	/************************************************************************/
	/* SQL ���� & ����� ����                                               */
	/************************************************************************/
	//BOOL DBExecuteSQL(char*); ��� �ߴ�
	//BOOL DBTop10SQL( DWORD );
	//BOOL DBUserInfo( char*, DWORD &, DWORD & , DWORD);
	//BOOL DBUserInfoWithCmpScore( char*, DWORD &, DWORD &, DWORD );	������ ������� ����

	/************************************************************************/
	/* ���� �ؼ���
	/************************************************************************/
	//void QueryAnalizer(char*);

	/************************************************************************/
	/* ThreadFunction
	/************************************************************************/
	//static DWORD WINAPI UpdateThreadFunc( LPVOID temp );
	static unsigned int WINAPI RankThreadFunc( LPVOID temp );

	/************************************************************************/
	/* update�ð�����
	/************************************************************************/
//	void setdbupdatetime(DWORD);
	void set1stupdatetime(DWORD);

	/************************************************************************/
	/* ���������� ä����
	/************************************************************************/
	BOOL getUserInfo( User &_target );

	/************************************************************************/
	/*	���� ������ ������Ʈ
	/************************************************************************/
	BOOL UpdateUserInfo( User &_target );

private:
	static SQLHENV hEnv;		// ȯ�� �ڵ�
	static SQLHDBC hDbc;		// ���� �ڵ�
	static SQLHSTMT hStmt;		// ��� �ڵ�
	
	SQLRETURN sRet;		// DB Connect�� ��ȯ��

	static CODBCManager* selfInstance;	// this ������

public:
	//DWORD updateThreadID;
	//HANDLE hUpdateThread;
	DWORD rankThreadID;
	static HANDLE hRankThread;
	//DWORD CurrentTime;
	DWORD check1stCurrentTime;
	//DWORD dbUpdateTime;	// UpdateList�� ������Ʈ�ϴ� �ð�
	DWORD check1stTime;		// 1�� ������ �����ϴ� �ð�
	char strCommand[256];	// Query�� ���� char �迭
	static CODBCManager* getInstance();
	static void releaseInstance();
	bool initialize();
};