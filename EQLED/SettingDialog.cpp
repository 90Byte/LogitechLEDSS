// SettingDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EQLED.h"
#include "SettingDialog.h"
#include "afxdialogex.h"

#include <bass.h>



// CSettingDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSettingDialog, CDialogEx)

CSettingDialog::CSettingDialog(CWnd* pParent /*=NULL*/, int selected)
	: CDialogEx(IDD_DIALOG_SETTING, pParent),
	m_iUpdatedDevice(selected)
{
}

CSettingDialog::~CSettingDialog()
{
}

void CSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DEV, m_cbDevices);
}


BEGIN_MESSAGE_MAP(CSettingDialog, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_DEV, &CSettingDialog::OnSelchangeComboDev)
//	ON_WM_CREATE()
//ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CSettingDialog �޽��� ó�����Դϴ�.


void CSettingDialog::OnSelchangeComboDev()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int sel = m_cbDevices.GetCurSel();
	this->m_iUpdatedDevice = sel - 1;
}

void CSettingDialog::GetDevicesList()
{

}

BOOL CSettingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_cbDevices.AddString(_T("Default Device"));

	BASS_DEVICEINFO buf;
	for (int i = 0; BASS_RecordGetDeviceInfo(i, &buf); ++i)
	{
		m_cbDevices.AddString(CString(buf.name));
	}

	m_cbDevices.SetCurSel(m_iUpdatedDevice + 1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
