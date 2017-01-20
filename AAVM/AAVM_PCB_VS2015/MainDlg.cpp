
// MainDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MainApp.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include "wm_user.h"
#include "main_thread.h"
#include <ctime>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MainDlg *pMainDlg = NULL;

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// MainDlg ��ȭ ����



MainDlg::MainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(MainDlg::IDD, pParent)
    , m_EDIT_MMS_MOVE          (0)
    , m_EDIT_HP1_ARM           (0)
    , m_EDIT_HP3_SWING         (0)
    , m_EDIT_HP3_BOOM_UP       (0)
    , m_EDIT_HP3_BOOM_DN       (0)
    , m_EDIT_ESROS_WIPER       (0)
    , m_EDIT_ESROS_INTWIPER    (0)
    , m_EDIT_AAVM_VIEW_MODE    (_T(""))
    , m_EDIT_AAVM_WARNING_FRONT(_T(""))
    , m_EDIT_AAVM_WARNING_REAR (_T(""))
    , m_EDIT_AAVM_WARNING_LEFT (_T(""))
    , m_EDIT_AAVM_WARNING_RIGHT(_T(""))
    , m_EDIT_AAVM_IMOD_STATUS  (_T(""))
    , m_EDIT_AAVM_MODEL_STATUS (_T(""))
    , m_EDIT_AAVM_DTC          (_T(""))
    , m_EDIT_CAN_AAVO          (_T(""))
    , m_EDIT_CAMERA_STAT_FRONT (_T(""))
    , m_EDIT_CAMERA_STAT_REAR  (_T(""))
    , m_EDIT_CAMERA_STAT_LEFT  (_T(""))
    , m_EDIT_CAMERA_STAT_RIGHT (_T(""))
    , m_EDIT_CAMERA_GAIN_FRONT (_T(""))
    , m_EDIT_CAMERA_GAIN_REAR  (_T(""))
    , m_EDIT_CAMERA_GAIN_LEFT  (_T(""))
    , m_EDIT_CAMERA_GAIN_RIGHT (_T(""))
    , m_EDIT_EMERALD_STAT      (_T(""))
    , m_EDIT_BF609_STAT        (_T(""))
    , m_EDIT_BF609_FPS         (_T(""))
	, m_EDIT_Barcode		   (_T(""))
    , m_CHECK_MODE_NIGHT     (FALSE)
    , m_CHECK_MODE_MOVE      (FALSE)
    , m_CHECK_MODE_SWING     (FALSE)
    , m_CHECK_MODE_WORK      (FALSE)
    , m_CHECK_MODE_WIPER     (FALSE)
    , m_CHECK_MODE_INTWIPER  (FALSE)
    , m_CHECK_MODE_MANUAL_OFF(FALSE)
	,m_EDIT_TEST_NUM_OK (0)
	,m_EDIT_TEST_NUM_ALL (0)
	,m_EDIT_TEST_NUM_RATE (_T(""))
	,m_COMBO_PROGRAM_TYPE (_T(""))
	,m_EDIT_CAR_NAME (_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MainDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_PROCESS, m_LIST_PROCESS);
    DDX_Text(pDX, IDC_EDIT_MMS_MOVE          , m_EDIT_MMS_MOVE          );
    DDX_Text(pDX, IDC_EDIT_HP1_ARM           , m_EDIT_HP1_ARM           );
    DDX_Text(pDX, IDC_EDIT_HP3_SWING         , m_EDIT_HP3_SWING         );
    DDX_Text(pDX, IDC_EDIT_HP3_BOOM_UP       , m_EDIT_HP3_SWING         );
    DDX_Text(pDX, IDC_EDIT_HP3_BOOM_DN       , m_EDIT_HP3_SWING         );
    DDX_Text(pDX, IDC_EDIT_ESROS_WIPER       , m_EDIT_ESROS_WIPER       );
    DDX_Text(pDX, IDC_EDIT_ESROS_INTWIPER    , m_EDIT_ESROS_INTWIPER    );
    DDX_Text(pDX, IDC_EDIT_AAVM_VIEW_MODE    , m_EDIT_AAVM_VIEW_MODE    );
    DDX_Text(pDX, IDC_EDIT_AAVM_WARNING_FRONT, m_EDIT_AAVM_WARNING_FRONT);
    DDX_Text(pDX, IDC_EDIT_AAVM_WARNING_REAR , m_EDIT_AAVM_WARNING_REAR );
    DDX_Text(pDX, IDC_EDIT_AAVM_WARNING_LEFT , m_EDIT_AAVM_WARNING_LEFT );
    DDX_Text(pDX, IDC_EDIT_AAVM_WARNING_RIGHT, m_EDIT_AAVM_WARNING_RIGHT);
    DDX_Text(pDX, IDC_EDIT_AAVM_IMOD_STATUS  , m_EDIT_AAVM_IMOD_STATUS);
    DDX_Text(pDX, IDC_EDIT_AAVM_MODEL_STATUS , m_EDIT_AAVM_MODEL_STATUS);
    DDX_Text(pDX, IDC_EDIT_AAVM_DTC          , m_EDIT_AAVM_DTC);
    DDX_Text(pDX, IDC_EDIT_CAN_AAVO          , m_EDIT_CAN_AAVO          );
    DDX_Text(pDX, IDC_EDIT_CAMERA_STAT_FRONT , m_EDIT_CAMERA_STAT_FRONT );
    DDX_Text(pDX, IDC_EDIT_CAMERA_STAT_REAR  , m_EDIT_CAMERA_STAT_REAR  );
    DDX_Text(pDX, IDC_EDIT_CAMERA_STAT_LEFT  , m_EDIT_CAMERA_STAT_LEFT  );
    DDX_Text(pDX, IDC_EDIT_CAMERA_STAT_RIGHT , m_EDIT_CAMERA_STAT_RIGHT );
    DDX_Text(pDX, IDC_EDIT_CAMERA_GAIN_FRONT , m_EDIT_CAMERA_GAIN_FRONT );
    DDX_Text(pDX, IDC_EDIT_CAMERA_GAIN_REAR  , m_EDIT_CAMERA_GAIN_REAR  );
    DDX_Text(pDX, IDC_EDIT_CAMERA_GAIN_LEFT  , m_EDIT_CAMERA_GAIN_LEFT  );
    DDX_Text(pDX, IDC_EDIT_CAMERA_GAIN_RIGHT , m_EDIT_CAMERA_GAIN_RIGHT );
    DDX_Text(pDX, IDC_EDIT_EMERALD_STAT      , m_EDIT_EMERALD_STAT      );
    DDX_Text(pDX, IDC_EDIT_BF609_STAT        , m_EDIT_BF609_STAT        );
    DDX_Text(pDX, IDC_EDIT_BF609_FPS         , m_EDIT_BF609_FPS         );
	DDX_Text(pDX, IDC_EDIT_TEST_NUM_OK, m_EDIT_TEST_NUM_OK);
	DDX_Text(pDX, IDC_EDIT_TEST_NUM_ALL, m_EDIT_TEST_NUM_ALL);
	DDX_Text(pDX, IDC_EDIT_TEST_NUM_RATE, m_EDIT_TEST_NUM_RATE);
	DDX_Text(pDX, IDC_EDIT_Barcode           , m_EDIT_Barcode           );
	DDX_CBString(pDX, IDC_COMBO_PROGRAM_TYPE, m_COMBO_PROGRAM_TYPE);
	DDX_CBString(pDX, IDC_COMBO_CAR_TYPE, m_EDIT_CAR_NAME);
    DDX_Check(pDX, IDC_CHECK_MODE_NIGHT      , m_CHECK_MODE_NIGHT       );
    DDX_Check(pDX, IDC_CHECK_MODE_MOVE       , m_CHECK_MODE_MOVE        );
    DDX_Check(pDX, IDC_CHECK_MODE_SWING      , m_CHECK_MODE_SWING       );
    DDX_Check(pDX, IDC_CHECK_MODE_WORK       , m_CHECK_MODE_WORK        );
    DDX_Check(pDX, IDC_CHECK_MODE_WIPER      , m_CHECK_MODE_WIPER       );
    DDX_Check(pDX, IDC_CHECK_MODE_INTWIPER   , m_CHECK_MODE_INTWIPER    );
    DDX_Check(pDX, IDC_CHECK_MODE_MANUAL_OFF , m_CHECK_MODE_MANUAL_OFF  );
}

BEGIN_MESSAGE_MAP(MainDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_MESSAGE(_WM_THREAD_UPDATE, &MainDlg::OnWmThreadUpdate)
    ON_MESSAGE(_WM_THREAD_END, &MainDlg::OnWmThreadEnd)
    ON_EN_CHANGE(IDC_EDIT_MMS_MOVE, &MainDlg::OnEnChangeEditControl)
    ON_EN_CHANGE(IDC_EDIT_HP1_ARM, &MainDlg::OnEnChangeEditControl)
    ON_EN_CHANGE(IDC_EDIT_HP3_SWING, &MainDlg::OnEnChangeEditControl)
    ON_EN_CHANGE(IDC_EDIT_HP3_BOOM_UP, &MainDlg::OnEnChangeEditControl)
    ON_EN_CHANGE(IDC_EDIT_HP3_BOOM_DN, &MainDlg::OnEnChangeEditControl)
    ON_BN_CLICKED(IDC_BTN_TEST, &MainDlg::OnBnClickedBtnTest)
END_MESSAGE_MAP()


// MainDlg �޽��� ó����

BOOL MainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

    pMainDlg = this;

    m_LIST_PROCESS.InsertColumn(0, "�˻��׸�", LVCFMT_CENTER, 200);
    m_LIST_PROCESS.InsertColumn(1, "��ǥ��"  , LVCFMT_CENTER, 220);
    m_LIST_PROCESS.InsertColumn(2, "������"  , LVCFMT_CENTER, 220);
    m_LIST_PROCESS.InsertColumn(3, "����"    , LVCFMT_CENTER,  80);
    m_LIST_PROCESS.SetExtendedStyle(m_LIST_PROCESS.GetExtendedStyle()|LVS_EX_GRIDLINES);

    int nScrollNeed;
    nScrollNeed = m_LIST_PROCESS.GetItemCount() - (m_LIST_PROCESS.GetScrollPos(SB_VERT) + m_LIST_PROCESS.GetCountPerPage());
    if (nScrollNeed > 0)
    {
        CRect oRect;
        m_LIST_PROCESS.GetSubItemRect(0, 0, LVIR_BOUNDS, oRect);
        int nHeight = oRect.bottom - oRect.top;
        nHeight *= nScrollNeed;
        CSize oSize(0, nHeight);
        m_LIST_PROCESS.Scroll(oSize);
    }
	//GetDlgItem(IDC_EDIT_Barcode)->SetFocus();
    UpdateData(FALSE);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void MainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void MainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR MainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT MainDlg::OnWmThreadUpdate(WPARAM a_wParam, LPARAM a_lParam)
{
    //CTime oTime;
    CRect oRect;
    CSize oSize;
    int nScrollNeed;

    if (a_wParam == 0)
    {
        nScrollNeed = m_LIST_PROCESS.GetItemCount() - (m_LIST_PROCESS.GetScrollPos(SB_VERT) + m_LIST_PROCESS.GetCountPerPage());
        if (nScrollNeed > 0)
        {
            m_LIST_PROCESS.GetSubItemRect(0, 0, LVIR_BOUNDS, oRect);
            nScrollNeed *= (oRect.bottom - oRect.top);
            oSize.cx = 0; oSize.cy = nScrollNeed;
            m_LIST_PROCESS.Scroll(oSize);
        }
    
        UpdateData(FALSE);  // ��Ʈ�ѷ��� ������ ���� ������ ȭ������ ���
    }
    else
    {
        UpdateData(TRUE);   // ������ ȭ�鿡 ǥ�õ� ���� ��Ʈ�ѷ� ������ ����
    }

    UpdateData(FALSE);
    return 0;
}

LRESULT MainDlg::OnWmThreadEnd(WPARAM a_wParam, LPARAM a_lParam)
{
    OnWmThreadUpdate(0,0);

    Main_Thread_End();

    GetDlgItem(IDC_BTN_TEST       )->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_TEST       )->SetWindowText("����");
    GetDlgItem(IDOK               )->EnableWindow(TRUE);
	UpdateData(FALSE);
    return 0;
}



void MainDlg::OnEnChangeEditControl()
{
    UpdateData(TRUE);   // ������ ȭ�鿡 ǥ�õ� ���� ��Ʈ�ѷ� ������ ����
}


void MainDlg::OnBnClickedBtnTest()
{
    CString strMsg;
	int nListPos;
	//clock_t begin = clock();
	//clock_t end;
	//double elapsed_secs;
	//UpdateData(TRUE);
	/*
	while (TRUE)
	{
		Sleep(10000);
		UpdateData(TRUE);
		if (m_EDIT_CAR_NAME != "" && m_COMBO_PROGRAM_TYPE != "" && m_EDIT_Barcode != "")
		{
			break;
		}
		end = clock();
		elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

		if (elapsed_secs > 60)
		{
			AfxMessageBox("��Ű��, �������, ���ڵ带 Ȯ���Ͽ� �ֽñ� �ٶ��ϴ�.");
			
			return;
		}
		
	}*/
	GetDlgItem(IDC_BTN_TEST)->GetWindowText(strMsg);
    if (strMsg == "����")
    {
		UpdateData(TRUE);   // ������ ȭ�鿡 ǥ�õ� ���� ��Ʈ�ѷ� ������ ����
		if (m_COMBO_PROGRAM_TYPE != "ī�޶�" &&
			(m_EDIT_CAR_NAME == "" || m_COMBO_PROGRAM_TYPE == "" || 
			!(m_EDIT_Barcode.GetAt(0) >= 'A' && m_EDIT_Barcode.GetAt(0) <= 'I') ||
			!((m_EDIT_Barcode.GetAt(1) >= '1' && m_EDIT_Barcode.GetAt(1) <= '9') || (m_EDIT_Barcode.GetAt(1) >= 'A' && m_EDIT_Barcode.GetAt(1) <= 'C')) ||
			!(m_EDIT_Barcode.GetAt(2) >= '0' && m_EDIT_Barcode.GetAt(2) <= '3') ||
			!(m_EDIT_Barcode.GetAt(3) >= '0' && m_EDIT_Barcode.GetAt(3) <= '9') ||
			m_EDIT_Barcode.GetLength() != 8))
		{
			AfxMessageBox("��Ű��, �������, ���ڵ带 Ȯ���Ͽ� �ֽñ� �ٶ��ϴ�.");
			GetDlgItem(IDC_BTN_TEST)->SetWindowText("�����");
			GetDlgItem(IDC_EDIT_Barcode)->SetFocus();
			return;
		}
        
        if (Main_Thread_Begin() == false)
        {
            AfxMessageBox("Main Thread Initializing Error!");
            return;
        }
        GetDlgItem(IDOK)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_TEST)->SetWindowText("����");
		GetDlgItem(IDC_BTN_TEST)->EnableWindow(FALSE);
    }
	else if (strMsg == "�����")
    {
		UpdateData(TRUE);   // ������ ȭ�鿡 ǥ�õ� ���� ��Ʈ�ѷ� ������ ����
		if (m_COMBO_PROGRAM_TYPE != "ī�޶�" &&
			(m_EDIT_CAR_NAME == "" || m_COMBO_PROGRAM_TYPE == "" || 
			!(m_EDIT_Barcode.GetAt(0) >= 'A' && m_EDIT_Barcode.GetAt(0) <= 'I') ||
			!((m_EDIT_Barcode.GetAt(1) >= '1' && m_EDIT_Barcode.GetAt(1) <= '9') || (m_EDIT_Barcode.GetAt(1) >= 'A' && m_EDIT_Barcode.GetAt(1) <= 'C')) ||
			!(m_EDIT_Barcode.GetAt(2) >= '0' && m_EDIT_Barcode.GetAt(2) <= '3') ||
			!(m_EDIT_Barcode.GetAt(3) >= '0' && m_EDIT_Barcode.GetAt(3) <= '9') ||
			m_EDIT_Barcode.GetLength() != 8))
		{
			AfxMessageBox("��Ű��, �������, ���ڵ带 Ȯ���Ͽ� �ֽñ� �ٶ��ϴ�.");
			GetDlgItem(IDC_BTN_TEST)->SetWindowText("�����");
			GetDlgItem(IDC_EDIT_Barcode)->SetFocus();
			return;
		}
        
        if (Main_Thread_Begin() == false)
        {
            AfxMessageBox("Main Thread Initializing Error!");
            return;
        }
        GetDlgItem(IDOK)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_TEST)->SetWindowText("����");
		GetDlgItem(IDC_BTN_TEST)->EnableWindow(FALSE);
    }
    else
    {
        UpdateData(FALSE);  // ��Ʈ�ѷ��� ������ ���� ������ ȭ������ ���
		nListPos =m_LIST_PROCESS.GetItemCount();
		m_LIST_PROCESS.InsertItem(nListPos, "�˻�  ����");
		m_LIST_PROCESS.SetItemText(nListPos, 1, "�˻� ����");
		m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        Main_Thread_End();
        GetDlgItem(IDOK)->EnableWindow(TRUE);
        GetDlgItem(IDC_BTN_TEST)->SetWindowText("����");
		m_LIST_PROCESS.DeleteAllItems();
		PostMessage(_WM_THREAD_UPDATE);
    }
	UpdateData(FALSE);
}

