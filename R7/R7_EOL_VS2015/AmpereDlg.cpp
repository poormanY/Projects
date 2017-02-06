// AmpereDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AmpereDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "y_AmMeterMgt.h"
#include "y_LogManage.h"
#include "SettingDlg.h"

// AmpereDlg dialog

IMPLEMENT_DYNAMIC(AmpereDlg, CDialogEx)

AmpereDlg::AmpereDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_AMPERE, pParent)
{

}

AmpereDlg::~AmpereDlg()
{
}

void AmpereDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_COMPORT, m_edit_comport);
	DDX_Control(pDX, IDC_EDIT_MIN, m_edit_min);
	DDX_Control(pDX, IDC_EDIT_MAX, m_edit_max);
	DDX_Control(pDX, IDC_EDIT_VALUE, m_edit_value);
	DDX_Control(pDX, IDC_EDIT_RESULT, m_edit_result);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_button_connect);
}


BEGIN_MESSAGE_MAP(AmpereDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &AmpereDlg::OnBnClickedButtonConnect)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// AmpereDlg message handlers

#define AMPERE_CONNECT	_T("CONNECT")
#define AMPERE_STOP		_T("STOP")
#define AMPERE_WAIT		_T("WAITING")

#define TIMER_AMPERE	2000

enum _ERROR_STATUS
{
	ERR_NONE = 0,
	ERR_CONNECT,
	ERR_NOREPLY
};

static CAmMeterMgt* pAmMeter = NULL;

void AmpereDlg::OnBnClickedButtonConnect()
{
	// TODO: Add your control notification handler code here
	CString strButton;
	m_button_connect.GetWindowTextW(strButton);

	if (strButton.Compare(AMPERE_CONNECT) == 0)
	{
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		m_button_connect.SetWindowTextW(AMPERE_WAIT);

		OpenAmpere();

		m_button_connect.SetWindowTextW(AMPERE_STOP);
	}
	else if (strButton.Compare(AMPERE_STOP) == 0)
	{
		m_button_connect.SetWindowTextW(AMPERE_WAIT);

		CloseAmpere();

		m_button_connect.SetWindowTextW(AMPERE_CONNECT);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	}

}

void AmpereDlg::OpenAmpere()
{
	pAmMeter = new CAmMeterMgt;

	if (pAmMeter != NULL)
	{
		CString strComport;
		m_edit_comport.GetWindowTextW(strComport);

		if (pAmMeter->Open((CStringA)strComport) == true)
		{
			pAmMeter->Thread_Begin();
			SetTimer(TIMER_AMPERE, 500, 0);
			printf("AmMeter Open Success!\n");
		}
		else
		{
			m_edit_result.SetWindowTextW(_T("Open FAIL"));
			printf("AmMeter Open Fail!\n");
		}
	}
}

void AmpereDlg::CloseAmpere()
{
	KillTimer(TIMER_AMPERE);

	if (pAmMeter != NULL)
	{
		pAmMeter->Close();
		delete pAmMeter;
		pAmMeter = NULL;
	}
}

int AmpereDlg::DoAmpere()
{
	if (pAmMeter == NULL)
	{
		return -1;
	}

	CString strAmpare, strAmpare_Min, strAmpare_Max;
	int nAmpare, nAmpare_Min, nAmpare_Max;
	int nCount, nError = ERR_NONE;
	double digit = 1.0;
	char szTgtMsg[128];
	char *pszRecMsg = "";
	stRegister m_Register;

	// load ampare min,max
	m_edit_min.GetWindowTextW(strAmpare_Min);
	m_edit_max.GetWindowTextW(strAmpare_Max);
	nAmpare_Min = _ttoi(strAmpare_Min);
	nAmpare_Max = _ttoi(strAmpare_Max);
	// memeset TgtMsg
	memset(szTgtMsg, 0x00, sizeof(szTgtMsg));

	pAmMeter->m_nAmMeter_State = _AMMETER_WRITE;
	
	DWORD nStartTime = GetTickCount();
	while (strlen(pAmMeter->GetMsg()) >= 1)
	{
		pszRecMsg = pAmMeter->GetMsg();
		memcpy(&m_Register, &pszRecMsg[3], sizeof(stRegister));
		
		if ((GetTickCount() - nStartTime) > 3000)
		{
			nError = ERR_NOREPLY;
			break;
		}
	}
	
	nAmpare = m_Register.value_hi << 4;
	nAmpare |= m_Register.value_lo;
	for (nCount = 0; nCount<m_Register.dot_lo; nCount++) digit /= 10.0;
	strAmpare.Format(_T("%.3f mA"), (float(nAmpare * digit)));
	m_edit_value.SetWindowTextW(strAmpare);

	if (nError == ERR_NOREPLY)
	{
		m_edit_result.SetWindowTextW(_T("No Reply"));
	}
	else if ((nAmpare > nAmpare_Min) && (nAmpare < nAmpare_Max))
	{
		m_edit_result.SetWindowTextW(_T("PASS"));
	}
	else
	{
		m_edit_result.SetWindowTextW(_T("FAIL"));
	}

	return 1;
}

BOOL AmpereDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	// Load Ampere Comport
	CLogManage SetLog;
	CString strComport;
	SetLog.SetFilePath(SETTING_FILE_PATH);
	strComport = SetLog.ReadSetting(COM_AM);
	m_edit_comport.SetWindowTextW(strComport);

	// Set Button caption
	m_button_connect.SetWindowTextW(AMPERE_CONNECT);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void AmpereDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case TIMER_AMPERE:
		DoAmpere();
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}
