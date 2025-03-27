
// Kol2016.h : main header file for the Kol2016 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKol2016App:
// See Kol2016.cpp for the implementation of this class
//

class CKol2016App : public CWinApp
{
public:
	CKol2016App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKol2016App theApp;
