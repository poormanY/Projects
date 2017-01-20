
// MainDlg.cpp : 구현 파일
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

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// MainDlg 대화 상자



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


// MainDlg 메시지 처리기

BOOL MainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

    pMainDlg = this;

    m_LIST_PROCESS.InsertColumn(0, "검사항목", LVCFMT_CENTER, 200);
    m_LIST_PROCESS.InsertColumn(1, "목표값"  , LVCFMT_CENTER, 220);
    m_LIST_PROCESS.InsertColumn(2, "측정값"  , LVCFMT_CENTER, 220);
    m_LIST_PROCESS.InsertColumn(3, "판정"    , LVCFMT_CENTER,  80);
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

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void MainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
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
    
        UpdateData(FALSE);  // 컨트롤러에 설정한 값을 윈도우 화면으로 출력
    }
    else
    {
        UpdateData(TRUE);   // 윈도우 화면에 표시된 값을 컨트롤러 변수에 적용
    }

    UpdateData(FALSE);
    return 0;
}

LRESULT MainDlg::OnWmThreadEnd(WPARAM a_wParam, LPARAM a_lParam)
{
    OnWmThreadUpdate(0,0);

    Main_Thread_End();

    GetDlgItem(IDC_BTN_TEST       )->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_TEST       )->SetWindowText("시작");
    GetDlgItem(IDOK               )->EnableWindow(TRUE);
	UpdateData(FALSE);
    return 0;
}



void MainDlg::OnEnChangeEditControl()
{
    UpdateData(TRUE);   // 윈도우 화면에 표시된 값을 컨트롤러 변수에 적용
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
			AfxMessageBox("패키지, 생산공정, 바코드를 확인하여 주시기 바랍니다.");
			
			return;
		}
		
	}*/
	GetDlgItem(IDC_BTN_TEST)->GetWindowText(strMsg);
    if (strMsg == "시작")
    {
		UpdateData(TRUE);   // 윈도우 화면에 표시된 값을 컨트롤러 변수에 적용
		if (m_COMBO_PROGRAM_TYPE != "카메라" &&
			(m_EDIT_CAR_NAME == "" || m_COMBO_PROGRAM_TYPE == "" || 
			!(m_EDIT_Barcode.GetAt(0) >= 'A' && m_EDIT_Barcode.GetAt(0) <= 'I') ||
			!((m_EDIT_Barcode.GetAt(1) >= '1' && m_EDIT_Barcode.GetAt(1) <= '9') || (m_EDIT_Barcode.GetAt(1) >= 'A' && m_EDIT_Barcode.GetAt(1) <= 'C')) ||
			!(m_EDIT_Barcode.GetAt(2) >= '0' && m_EDIT_Barcode.GetAt(2) <= '3') ||
			!(m_EDIT_Barcode.GetAt(3) >= '0' && m_EDIT_Barcode.GetAt(3) <= '9') ||
			m_EDIT_Barcode.GetLength() != 8))
		{
			AfxMessageBox("패키지, 생산공정, 바코드를 확인하여 주시기 바랍니다.");
			GetDlgItem(IDC_BTN_TEST)->SetWindowText("재시작");
			GetDlgItem(IDC_EDIT_Barcode)->SetFocus();
			return;
		}
        
        if (Main_Thread_Begin() == false)
        {
            AfxMessageBox("Main Thread Initializing Error!");
            return;
        }
        GetDlgItem(IDOK)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_TEST)->SetWindowText("중지");
		GetDlgItem(IDC_BTN_TEST)->EnableWindow(FALSE);
    }
	else if (strMsg == "재시작")
    {
		UpdateData(TRUE);   // 윈도우 화면에 표시된 값을 컨트롤러 변수에 적용
		if (m_COMBO_PROGRAM_TYPE != "카메라" &&
			(m_EDIT_CAR_NAME == "" || m_COMBO_PROGRAM_TYPE == "" || 
			!(m_EDIT_Barcode.GetAt(0) >= 'A' && m_EDIT_Barcode.GetAt(0) <= 'I') ||
			!((m_EDIT_Barcode.GetAt(1) >= '1' && m_EDIT_Barcode.GetAt(1) <= '9') || (m_EDIT_Barcode.GetAt(1) >= 'A' && m_EDIT_Barcode.GetAt(1) <= 'C')) ||
			!(m_EDIT_Barcode.GetAt(2) >= '0' && m_EDIT_Barcode.GetAt(2) <= '3') ||
			!(m_EDIT_Barcode.GetAt(3) >= '0' && m_EDIT_Barcode.GetAt(3) <= '9') ||
			m_EDIT_Barcode.GetLength() != 8))
		{
			AfxMessageBox("패키지, 생산공정, 바코드를 확인하여 주시기 바랍니다.");
			GetDlgItem(IDC_BTN_TEST)->SetWindowText("재시작");
			GetDlgItem(IDC_EDIT_Barcode)->SetFocus();
			return;
		}
        
        if (Main_Thread_Begin() == false)
        {
            AfxMessageBox("Main Thread Initializing Error!");
            return;
        }
        GetDlgItem(IDOK)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_TEST)->SetWindowText("중지");
		GetDlgItem(IDC_BTN_TEST)->EnableWindow(FALSE);
    }
    else
    {
        UpdateData(FALSE);  // 컨트롤러에 설정한 값을 윈도우 화면으로 출력
		nListPos =m_LIST_PROCESS.GetItemCount();
		m_LIST_PROCESS.InsertItem(nListPos, "검사  종료");
		m_LIST_PROCESS.SetItemText(nListPos, 1, "검사 중지");
		m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
        Main_Thread_End();
        GetDlgItem(IDOK)->EnableWindow(TRUE);
        GetDlgItem(IDC_BTN_TEST)->SetWindowText("시작");
		m_LIST_PROCESS.DeleteAllItems();
		PostMessage(_WM_THREAD_UPDATE);
    }
	UpdateData(FALSE);
}

