
// avm_eol_sample.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// MainApp:
// �� Ŭ������ ������ ���ؼ��� avm_eol_sample.cpp�� �����Ͻʽÿ�.
//

class MainApp : public CWinApp
{
public:
	MainApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern MainApp theApp;