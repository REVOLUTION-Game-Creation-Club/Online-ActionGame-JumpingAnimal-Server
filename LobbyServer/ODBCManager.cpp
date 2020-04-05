#include "GameLobby.h"
#include "odbcmanager.h"
#include <process.h>

CODBCManager* CODBCManager::selfInstance = 0;
SQLHENV CODBCManager::hEnv = 0;	
SQLHDBC CODBCManager::hDbc = 0;	
SQLHSTMT CODBCManager::hStmt = 0;
HANDLE CODBCManager::hRankThread = 0;


CODBCManager::CODBCManager(void)
{
	hEnv = SQL_NULL_HANDLE;
	hStmt = SQL_NULL_HANDLE;
    hDbc = SQL_NULL_HANDLE;
	
	//dbUpdateTime = 30000;
	check1stTime = 30000;
}

CODBCManager::~CODBCManager(void)
{
}

CODBCManager* CODBCManager::getInstance()
{
	if( selfInstance == 0 )
		selfInstance = new CODBCManager();

	return selfInstance;
}

void CODBCManager::releaseInstance()
{
	DeleteCriticalSection( &g_csBestUser );

    DBDisConnect();

	if ( selfInstance != 0 )
	{
		delete selfInstance;
		selfInstance = 0;
	}
}

bool CODBCManager::initialize()
{
	InitializeCriticalSection( &g_csBestUser );

	if( !DBConnect() )
		return false;	
	return true;
}

BOOL CODBCManager::DBConnect(void)
{
	/************************************************************************/
	/* SQL �ڵ��� �Ҵ��Ѵ�.
	/************************************************************************/
	if( SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS )
		return FALSE;
	if( SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS)
		return FALSE;

	if( SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS)
		return FALSE;
	
	/************************************************************************/
	/* SQLDriverConnect�Լ��� �̿��ؼ� DB�� �����Ѵ�(FILEDSN���)
	/************************************************************************/
	SQLCHAR InCon[256];
	SQLCHAR OutCon[1024];
	SQLSMALLINT cbOutCon;
	CHAR Dir[MAX_PATH];
	//strcpy( (char*)InCon, "DRIVER={SQL Server}; SERVER=155.230.25.123; DATABASE=RevGame; UID=revgameuser; PWD=wwwrevgameuser;" );
	GetCurrentDirectory(MAX_PATH, Dir);

	wsprintf( (char*)InCon, "FILEDSN=%s\\RevGame_Data.MDF.dsn", Dir);
	
	sRet = SQLDriverConnect(hDbc,(SQLHWND)NULL,InCon, sizeof(InCon), OutCon, sizeof(OutCon)
		, &cbOutCon, SQL_DRIVER_NOPROMPT);

	if( (sRet != SQL_SUCCESS) && (sRet != SQL_SUCCESS_WITH_INFO) )
		return FALSE;

	/************************************************************************/
	/* DB���ῡ ���������� ������ �����ϱ� ���� STMT �ڵ��� �Ҵ��Ѵ�
	/************************************************************************/
	if( SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
		return FALSE;

	/************************************************************************/
	/* DB ����� ���ÿ� Thread ����
	/************************************************************************/
	//hUpdateThread = (HANDLE)_beginthreadex( NULL, 0, UpdateThreadFunc, (void *)0, 0, (unsigned int *)&updateThreadID);
	//CloseHandle(hUpdateThread);

	if( !selfInstance )
		selfInstance = this;

	hRankThread = (HANDLE)_beginthreadex( NULL, 0, RankThreadFunc, (void *)0, 0, (unsigned int *)&rankThreadID);	

	return TRUE;
}

/************************************************************************/
/* 1�� ������ �����ϱ� ���� ������
/************************************************************************/
unsigned int WINAPI CODBCManager::RankThreadFunc( LPVOID temp )
{
	while(1)
	{
		if( timeGetTime() - selfInstance->check1stCurrentTime > (DWORD) selfInstance->check1stTime )
		{
			EnterCriticalSection( &g_csBestUser );

			SQLCHAR strUserID[21] = {0,};
			long int nScore = 0;
			long int nKillSum = 0;
			SQLINTEGER lstrUserID, lnScore, lnKillSum;

			SQLBindCol(selfInstance->hStmt, 1, SQL_C_CHAR, strUserID, sizeof(strUserID), &lstrUserID);			
			SQLBindCol(selfInstance->hStmt, 2, SQL_C_ULONG, &nScore, 0, &lnScore);
			SQLBindCol(selfInstance->hStmt, 3, SQL_C_ULONG, &nKillSum, 0, &lnKillSum);

			/************************************************************************/
			/*	���� 1�� ã��
			/************************************************************************/
			wsprintf( selfInstance->strCommand,
	"Select Top 1 strUserID, nScore, nkillSum from Score where strContentName = 'beinghuman' ORDER BY nScore DESC");
				//"Select Top %d strUserID, nScore from Score where strContentName = '%s' ORDER BY nScore DESC"
				//, GetContentName(dwContentKey));

			if( SQL_ERROR == SQLExecDirect(selfInstance->hStmt, (SQLCHAR *)selfInstance->strCommand, SQL_NTS) )
			{
				SQLCloseCursor(selfInstance->hStmt);				
				LeaveCriticalSection( &g_csBestUser );
				return FALSE;
			}

			while( SQLFetch(selfInstance->hStmt) != SQL_NO_DATA )
			{
				GameLobby::getInstance()->getScoreBestUser().account = (char *)strUserID;
				GameLobby::getInstance()->getScoreBestUser().score = nScore;
				GameLobby::getInstance()->getScoreBestUser().killSum = nKillSum;
			}
			SQLCloseCursor(selfInstance->hStmt);
			/************************************************************************/			

			/************************************************************************/
			/*	ų�� 1�� ã��
			/************************************************************************/
			wsprintf( selfInstance->strCommand,
	"Select Top 1 strUserID, nScore, nkillSum from Score where strContentName = 'beinghuman' ORDER BY nkillSum DESC");

			if( SQL_ERROR == SQLExecDirect(selfInstance->hStmt, (SQLCHAR *)selfInstance->strCommand, SQL_NTS) )
			{
				SQLCloseCursor(selfInstance->hStmt);				
				LeaveCriticalSection( &g_csBestUser );
				return FALSE;
			}

			while( SQLFetch(selfInstance->hStmt) != SQL_NO_DATA )
			{
				GameLobby::getInstance()->getKillBestUser().account = (char *)strUserID;
				GameLobby::getInstance()->getKillBestUser().score = nScore;
				GameLobby::getInstance()->getKillBestUser().killSum = nKillSum;
			}
			SQLCloseCursor(selfInstance->hStmt);
			/************************************************************************/

			long int nScoreRank = 0;
			long int nKillRank = 0;
			SQLINTEGER lnScoreRank, lnKillRank;

			/************************************************************************/
			/*	���� 1�� ������ ScoreRank 1��
			/************************************************************************/
			GameLobby::getInstance()->getScoreBestUser().scoreRank = 1;
			/************************************************************************/

			/************************************************************************/
			/*	���� 1�� ������ Kill Rank ���
			/************************************************************************/
			SQLBindCol( selfInstance->hStmt, 1, SQL_C_ULONG, &nKillRank, 0, &lnKillRank );

			wsprintf( selfInstance->strCommand, 
				"Select count(*) from Score where strContentName = 'beinghuman' and nkillsum > %ld ", 
				GameLobby::getInstance()->getScoreBestUser().killSum );

			if( SQL_ERROR == SQLExecDirect(selfInstance->hStmt, (SQLCHAR *)selfInstance->strCommand, SQL_NTS) )
			{
				SQLCloseCursor(selfInstance->hStmt);
				LeaveCriticalSection( &g_csBestUser );
				return FALSE;
			}

			while( SQLFetch(selfInstance->hStmt) != SQL_NO_DATA )
				GameLobby::getInstance()->getScoreBestUser().killRank = nKillRank + 1;

			SQLCloseCursor(selfInstance->hStmt);
			/************************************************************************/

			/************************************************************************/
			/*	ų�� 1�� ������ KillRank 1��
			/************************************************************************/
			GameLobby::getInstance()->getKillBestUser().killRank = 1;
			/************************************************************************/

			/************************************************************************/
			/*	ų�� 1�� ������ Score Rank ���
			/************************************************************************/
			SQLBindCol( selfInstance->hStmt, 1, SQL_C_ULONG, &nScoreRank, 0, &lnScoreRank );

			wsprintf( selfInstance->strCommand, 
				"Select count(*) from Score where strContentName = 'beinghuman' and nScore > %ld ", 
				GameLobby::getInstance()->getKillBestUser().score );

			if( SQL_ERROR == SQLExecDirect(selfInstance->hStmt, (SQLCHAR *)selfInstance->strCommand, SQL_NTS) )
			{
				SQLCloseCursor(selfInstance->hStmt);
				LeaveCriticalSection( &g_csBestUser );
				return FALSE;
			}

			while( SQLFetch(selfInstance->hStmt) != SQL_NO_DATA )
				GameLobby::getInstance()->getKillBestUser().scoreRank =  nScoreRank + 1;

			SQLCloseCursor(selfInstance->hStmt);
			/************************************************************************/

			selfInstance->check1stCurrentTime = timeGetTime();

			LeaveCriticalSection( &g_csBestUser );
		}

		Sleep(10);
	}

	return TRUE;
}

void CODBCManager::DBDisConnect(void)
{
	/************************************************************************/
	/* �ڵ��� �����ϰ� DB�� ������ ���´�
	/************************************************************************/
	if( hStmt ) SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	if( hDbc ) SQLDisconnect(hDbc);
	if( hDbc ) SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	if( hEnv ) SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
	
	TerminateThread(hRankThread, 0);
}

/************************************************************************/
/* 1��üũThread�� ������Ʈ�� �ð��� ����
/************************************************************************/
void CODBCManager::set1stupdatetime( DWORD dwupdatetime )
{
	check1stTime = dwupdatetime;
}

BOOL CODBCManager::getUserInfo( User &_target )
{
	long int nkillRank = 0;
	long int nScoreRank = 0;
	long int nScore = 0;
	long int nKillSum = 0;
	SQLINTEGER lnScoreRank, lnkillRank, lnScore, lnKillSum;

	/************************************************************************/
	/*	������ ������ ������ �´�
	/************************************************************************/
	SQLBindCol(hStmt, 1, SQL_C_ULONG, &nScore, 0, &lnScore);
	SQLBindCol(hStmt, 2, SQL_C_ULONG, &nKillSum, 0, &lnKillSum);

	char p_strCommand[512];

	wsprintf(p_strCommand,
		"select nscore, nkillsum from score where strcontentname='beinghuman' and struserid = '%s'"
		, _target.getAccount().getBuffer() );	
	
	if( SQL_ERROR == SQLExecDirect(hStmt, (SQLCHAR *)p_strCommand, SQL_NTS) )
	{
		SQLCloseCursor(hStmt);
		return FALSE;
	}

	int isNew = true;

	while( SQLFetch(hStmt) != SQL_NO_DATA )
	{
		_target.setUserScore( nScore );
		_target.setUserKillSum( nKillSum );
		isNew = false;
	}

	SQLCloseCursor(hStmt);

	/************************************************************************/
	/* ���࿡ ����DATA�� ��� �� ������� �ǴܵǸ�
	/************************************************************************/
	if( isNew )
	{
		/************************************************************************/
		/* DB�� ������ ���
		/************************************************************************/
		wsprintf( p_strCommand, "insert score values('%s', 'beinghuman', 0, 0, GetDate(), 'a')", _target.getAccount().getBuffer() );

		if( SQL_ERROR == SQLExecDirect(hStmt, (SQLCHAR *)p_strCommand, SQL_NTS) )
		{			
			return FALSE;
		}
	}
	/************************************************************************/

	/************************************************************************/
	/*	Score Rank ���
	/************************************************************************/
	SQLBindCol(hStmt, 1, SQL_C_ULONG, &nScoreRank, 0, &lnScoreRank);

	wsprintf( p_strCommand, 
		"Select count(*) from Score where strContentName = 'beinghuman' and nScore > %ld ", _target.getUserScore() );

	if( SQL_ERROR == SQLExecDirect(hStmt, (SQLCHAR *)p_strCommand, SQL_NTS) )
	{
		SQLCloseCursor(hStmt);
		return FALSE;
	}

	while( SQLFetch(hStmt) != SQL_NO_DATA )
		_target.setUserScoreRank( nScoreRank + 1 );

	SQLCloseCursor(hStmt);
	/************************************************************************/

    /************************************************************************/
    /*	Kill Rank ���
    /************************************************************************/
	SQLBindCol( hStmt, 1, SQL_C_ULONG, &nkillRank, 0, &lnkillRank );

	wsprintf( p_strCommand, 
		"Select count(*) from Score where strContentName = 'beinghuman' and nkillsum > %ld ", _target.getUserKillSum() );

	if( SQL_ERROR == SQLExecDirect(hStmt, (SQLCHAR *)p_strCommand, SQL_NTS) )
	{
		SQLCloseCursor(hStmt);
		return FALSE;
	}

	while( SQLFetch(hStmt) != SQL_NO_DATA )
		_target.setUserKillRank( nkillRank + 1 );

	SQLCloseCursor(hStmt);
	/************************************************************************/
	return TRUE;
}

BOOL CODBCManager::UpdateUserInfo( User &_target )
{
	char p_strCommand[512];

	wsprintf(p_strCommand, "update score set nscore = %u, nkillsum = %u where struserid = '%s' and strcontentname = 'beinghuman'", _target.getUserScore() ,_target.getUserKillSum, _target.getAccount().getBuffer() );

	if( SQL_ERROR == SQLExecDirect(hStmt, (SQLCHAR *) p_strCommand, SQL_NTS) )
		return FALSE;
	
	return TRUE;
}