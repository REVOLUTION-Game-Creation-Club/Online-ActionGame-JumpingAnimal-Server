// GM.h: interface for the CGM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GM_H__97D29D3E_7769_4CE8_AEDA_6D210A77C3E4__INCLUDED_)
#define AFX_GM_H__97D29D3E_7769_4CE8_AEDA_6D210A77C3E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameManager.h"

class CGM
{
private:
	static CGameManager GM;
public:
	CGM();
	~CGM();

	static CGameManager* GetGameManager()
	{
		return &GM;
	}
};

#endif // !defined(AFX_GM_H__97D29D3E_7769_4CE8_AEDA_6D210A77C3E4__INCLUDED_)
