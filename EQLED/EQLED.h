
// EQLED.h : EQLED ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CEQLEDApp:
// �� Ŭ������ ������ ���ؼ��� EQLED.cpp�� �����Ͻʽÿ�.
//

class CEQLEDApp : public CWinApp
{
public:
	CEQLEDApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.

public:
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CEQLEDApp theApp;
