#pragma once
#include "afxwin.h"


// CSettingDialog 대화 상자입니다.

class CSettingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDialog)

public:
	CSettingDialog(CWnd* pParent = NULL, int selected=-1);   // 표준 생성자입니다.
	virtual ~CSettingDialog();

	int m_iUpdatedDevice = 0;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSelchangeComboDev();

private:
	void GetDevicesList();
public:
	CComboBox m_cbDevices;
	virtual BOOL OnInitDialog();
};
