
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once
#include "ChildView.h"
#include "LEDThread.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CChildView    m_wndView;

// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	LRESULT OnTrayNotification(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

public:
	void setTray();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CLEDThread* th;
	static UINT THREADF(LPVOID);
	afx_msg void OnSetting();
};


