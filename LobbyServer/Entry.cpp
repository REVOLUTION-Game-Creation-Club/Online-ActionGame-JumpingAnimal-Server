#include "Entry.h"

CRITICAL_SECTION g_csBestUser;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	if( Entry::getInstance()->initialize() == false )
		return 0;

	Entry::getInstance()->join();

	return 0;
}

#include "cpp/lang/ThreadManager.h"

#include "Acceptor.h"
#include "CompletionHandler.h"
#include "SessionManager.h"
#include "GameLobby.h"


Entry*		Entry::selfInstance = 0;

Entry::Entry()
{
}

Entry::~Entry()
{
	/************************************************************************/
	/* DB ³¡³»±â
	/************************************************************************/
//	CODBCManager::releaseInstance();

	SessionManager::releaseInstance();
	CompletionHandler::releaseInstance();
	GameLobby::releaseInstance();

	ThreadManager::releaseInstance();
}

Entry* Entry::getInstance()
{
	if( selfInstance == 0 )
		selfInstance = new Entry();

	return selfInstance;
}

void Entry::releaseInstance()
{
	if( selfInstance == 0 )
		return;

	delete selfInstance;
	selfInstance = 0;
}

bool Entry::initialize()
{
	if( createSocket() == false )
		return false;

	if( CompletionHandler::getInstance()->initialize() == false )
		return false;
	if( acceptor.initialize( "211.61.163.211", 9630 ) == false )
		return false;

	/************************************************************************/
	/* ¾¾¿¤ DBÃÊ±âÈ­
	/************************************************************************/
//	if( CODBCManager::getInstance()->initialize() == false )
//		return false;

	return true;
}

bool Entry::createSocket()
{
 	WSADATA wsaData;

	if( ::WSAStartup( 0x202, &wsaData ) != 0 )
		return false;

	if( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 )
	{
		::WSACleanup();

		return false;
	}

	return true;
}

void Entry::join()
{
	ThreadManager::getInstance()->join();
}
