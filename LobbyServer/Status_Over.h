// Status_Over.h: interface for the CStatus_Over class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUS_OVER_H__1148AA89_125D_4D02_B9FE_03D3EF8585A8__INCLUDED_)
#define AFX_STATUS_OVER_H__1148AA89_125D_4D02_B9FE_03D3EF8585A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Status_Playing.h"

class CStatus_Over  
	: public GameStatus
{
public:
	CStatus_Over();
	virtual ~CStatus_Over();

	void ProcessPacket(Packet& packet);
	GameStatus* ChangeStatus(char* Message);
};

#endif // !defined(AFX_STATUS_OVER_H__1148AA89_125D_4D02_B9FE_03D3EF8585A8__INCLUDED_)
