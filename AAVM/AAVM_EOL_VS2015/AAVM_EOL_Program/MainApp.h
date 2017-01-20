// KH_Checker.h : main header file for the KH_CHECKER application
//

#if !defined(AFX_KH_CHECKER_H__A7327B7C_D3C5_48AD_8897_681918605587__INCLUDED_)
#define AFX_KH_CHECKER_H__A7327B7C_D3C5_48AD_8897_681918605587__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// MainApp:
// See KH_Checker.cpp for the implementation of this class
//
extern CString strImagePath;
class MainApp : public CWinApp
{
public:
	MainApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MainApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(MainApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KH_CHECKER_H__A7327B7C_D3C5_48AD_8897_681918605587__INCLUDED_)
