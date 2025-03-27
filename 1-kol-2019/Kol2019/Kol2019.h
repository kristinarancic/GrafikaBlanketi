
// Kol2019.h : main header file for the Kol2019 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKol2019App:
// See Kol2019.cpp for the implementation of this class
//

class CKol2019App : public CWinApp
{
public:
	CKol2019App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKol2019App theApp;
