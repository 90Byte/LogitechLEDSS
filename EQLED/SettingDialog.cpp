// SettingDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EQLED.h"
#include "SettingDialog.h"
#include "afxdialogex.h"

#include <bass.h>



// CSettingDialog 대화 상자입니다.

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


// CSettingDialog 메시지 처리기입니다.


void CSettingDialog::OnSelchangeComboDev()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int sel = m_cbDevices.GetCurSel();
	this->m_iUpdatedDevice = sel - 1;
}

void CSettingDialog::GetDevicesList()
{

}

BOOL CSettingDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_cbDevices.AddString(_T("Default Device"));

	BASS_DEVICEINFO buf;
	for (int i = 0; BASS_RecordGetDeviceInfo(i, &buf); ++i)
	{
		m_cbDevices.AddString(CString(buf.name));
	}

	m_cbDevices.SetCurSel(m_iUpdatedDevice + 1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
