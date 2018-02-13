#pragma once
#include "afxwin.h"


// CSettingDialog ��ȭ �����Դϴ�.

class CSettingDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDialog)

public:
	CSettingDialog(CWnd* pParent = NULL, int selected=-1);   // ǥ�� �������Դϴ�.
	virtual ~CSettingDialog();

	int m_iUpdatedDevice = 0;
// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSelchangeComboDev();

private:
	void GetDevicesList();
public:
	CComboBox m_cbDevices;
	virtual BOOL OnInitDialog();
};
