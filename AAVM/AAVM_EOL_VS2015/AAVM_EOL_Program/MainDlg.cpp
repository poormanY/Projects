// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainApp.h"
#include "MainDlg.h"
#include "ExpressImg.h"
#include "wm_user.h"
#include "Color.h"
#include "FrontMoniter.h"
#include "RearMoniter.h"
#include "LeftMoniter.h"
#include "RightMoniter.h"
#include "atlimage.h"
//#include "CanProtocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define     CreateImg_TEST 0

MainDlg     *pMainDlg  = NULL;
//CCanProtocol*pCanprt   = NULL;
CDialogSet  *pSetupDlg = NULL;
CExpressImg *pImgDlg   = NULL;

CFrontImg	*pFImgDlg  = NULL;
CRearImg	*pReImgDlg = NULL;
CLeftImg	*pLImgDlg  = NULL;
CRightImg	*pRImgDlg  = NULL;


int m_nImgView = 0;
int tImageShowCompleted = 0;	//mook - �����̵� �ִϸ��̼��� �Ϸ�Ǿ����� üũ
int nPowerPinUsed = 0;

unsigned int ui_FStartNumber;
unsigned int ui_FStopNumber;

unsigned int ui_ReStartNumber;
unsigned int ui_ReStopNumber;

unsigned int ui_LStartNumber;
unsigned int ui_LStopNumber;

unsigned int ui_RStartNumber;
unsigned int ui_RStopNumber;


bool FDrawStop=false;
bool ReDrawStop=false;
bool LDrawStop=false;
bool RDrawStop=false;
bool tFImageThreadStop;
bool tReImageThreadStop;
bool tLImageThreadStop;
bool tRImageThreadStop;

extern bool tMainThreadStop;
extern bool tMainThreadManulError;
extern UINT MainThread(LPVOID pParam);
extern bool tBoxBarcodeState;

static CString StrTesterLine[] = {"�Ϲ���", "�����˻�1(PCB ASSY)","�����˻�2(ASSY)"};
int m_nProgramType;
static CString StrTesterName[] = {"������", "�����", "�۹̼�", "������", "�ڻ��", "���⼱", "������", "�ڼ�", "����ȣ"};

// BOX �ִ� ���� ����
// HG : 1���� 12�� ������ ��� ��ǰ�� 1���� 18��
#define _BOX_HG_UNIT_CNT        0x0000000C
#define _BOX_DEFAULT_UNIT_CNT   0x00000012

/////////////////////////////////////////////////////////////////////////////
// MainDlg dialog

MainDlg::MainDlg(CWnd* pParent /*=NULL*/)
    : CDialog(MainDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(MainDlg)
	m_EDIT_TESTER_LINE = _T("");
	m_EDIT_TESTER_NAME = _T("");
	m_EDIT_TESTER_TIME = _T("");
	m_EDIT_CAR_NAME = _T("AAVM ������");
	m_nComboTesterLine = -1;
	m_nComboTesterName = -1;
	m_EDIT_PIN_USED = 0;
	m_COMBO_PROGRAM_TYPE = _T("");
	m_COMBO_VOLTAGE_TYPE =_T("24V");
	m_EDIT_BOX_UNIT_MAX = 0;
	m_EDIT_BOX_UNIT_CNT = 0;
	m_STATIC_LINE_NAME = _T("");
	m_EDIT_TEST_NUM_OK =0;
	m_EDIT_TEST_NUM_ALL =0;

	m_EDIT_MMS_MOVE			= 0;
    m_EDIT_HP1_ARM			= 0;
    m_EDIT_HP3_SWING		= 0;
    m_EDIT_HP3_BOOM_UP		= 0;
    m_EDIT_HP3_BOOM_DN		= 0;
    m_EDIT_ESROS_WIPER		= 0;
	m_EDIT_ESROS_INTWIPER	= 0;
	m_EDIT_AAVM_VIEW_MODE    =_T("");
	m_EDIT_AAVM_WARNING_FRONT=_T("");
	m_EDIT_AAVM_WARNING_REAR =_T("");
	m_EDIT_AAVM_WARNING_LEFT =_T("");
	m_EDIT_AAVM_WARNING_RIGHT=_T("");
	m_EDIT_AAVM_IMOD_STATUS	 =_T("");
    m_EDIT_AAVM_MODEL_STATUS =_T("");
    m_EDIT_AAVM_DTC          =_T("");
	m_EDIT_CAN_AAVO          =_T("");
	m_EDIT_CAMERA_STAT_FRONT =_T("");
	m_EDIT_CAMERA_STAT_REAR  =_T("");
	m_EDIT_CAMERA_STAT_LEFT  =_T("");
	m_EDIT_CAMERA_STAT_RIGHT =_T("");
	m_EDIT_CAMERA_GAIN_FRONT =_T("");
	m_EDIT_CAMERA_GAIN_REAR  =_T("");
	m_EDIT_CAMERA_GAIN_LEFT  =_T("");
	m_EDIT_CAMERA_GAIN_RIGHT =_T("");
	m_EDIT_EMERALD_STAT      =_T("");
	m_EDIT_BF609_STAT        =_T("");
	m_EDIT_BF609_FPS         =_T("");
	m_CHECK_MODE_NIGHT		=FALSE;
	m_CHECK_MODE_MOVE		=FALSE;
	m_CHECK_MODE_SWING		=FALSE;
	m_CHECK_MODE_WORK		=FALSE;
	m_CHECK_MODE_WIPER		=FALSE;
    m_CHECK_MODE_INTWIPER	=FALSE;
    m_CHECK_MODE_MANUAL_OFF	=FALSE;
	//}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MainDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(MainDlg)
	DDX_Control(pDX, IDC_SPIN_BOX_UNIT_CNT, m_SPIN_BOX_UNIT_CNT);
	DDX_Control(pDX, IDC_COMBO_TESTER_NAME, m_CtrlComboTesterName);
	DDX_Control(pDX, IDC_COMBO_TESTER_LINE, m_CtrlComboTesterLine);
	DDX_Control(pDX, IDC_TXT_GUIDE, m_TXT_GUIDE);
    DDX_Control(pDX, IDC_TXT_LABEL, m_TXT_LABEL);
    DDX_Control(pDX, IDC_LIST_PROCESS, m_LIST_PROCESS);
    DDX_Text(pDX, IDC_EDIT_TEST_NUM_OK, m_EDIT_TEST_NUM_OK);
    DDX_Text(pDX, IDC_EDIT_TEST_NUM_ALL, m_EDIT_TEST_NUM_ALL);
    DDX_Text(pDX, IDC_EDIT_TEST_NUM_RATE, m_EDIT_TEST_NUM_RATE);
	DDX_Text(pDX, IDC_EDIT_MONITOR, m_EDIT_MONITOR);
	DDX_Text(pDX, IDC_EDIT_TESTER_LINE, m_EDIT_TESTER_LINE);
    DDX_Text(pDX, IDC_EDIT_TESTER_NAME, m_EDIT_TESTER_NAME);
	DDX_Text(pDX, IDC_EDIT_TESTER_TIME, m_EDIT_TESTER_TIME);
	DDX_Text(pDX, IDC_EDIT_CARNAME, m_EDIT_CAR_NAME);
	DDX_CBIndex(pDX, IDC_COMBO_TESTER_LINE, m_nComboTesterLine);
	DDX_CBIndex(pDX, IDC_COMBO_TESTER_NAME, m_nComboTesterName);
	DDX_Text(pDX, IDC_EDIT_PIN_USED, m_EDIT_PIN_USED);
	DDX_CBString(pDX, IDC_COMBO_PROGRAM_TYPE, m_COMBO_PROGRAM_TYPE);
	DDX_CBString(pDX, IDC_COMBO_VOLTAGE,      m_COMBO_VOLTAGE_TYPE);

	DDX_Text(pDX, IDC_EDIT_BOX_UNIT_MAX, m_EDIT_BOX_UNIT_MAX);
	DDX_Text(pDX, IDC_EDIT_BOX_UNIT_CNT, m_EDIT_BOX_UNIT_CNT);
	DDX_Text(pDX, IDC_STATIC_LINE_NAME, m_STATIC_LINE_NAME);


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
	DDX_Check(pDX, IDC_CHECK_MODE_NIGHT, m_CHECK_MODE_NIGHT);
	DDX_Check(pDX, IDC_CHECK_MODE_MOVE , m_CHECK_MODE_MOVE );
	DDX_Check(pDX, IDC_CHECK_MODE_SWING, m_CHECK_MODE_SWING);
	DDX_Check(pDX, IDC_CHECK_MODE_WORK , m_CHECK_MODE_WORK );
	DDX_Check(pDX, IDC_CHECK_MODE_WIPER      , m_CHECK_MODE_WIPER       );
    DDX_Check(pDX, IDC_CHECK_MODE_INTWIPER   , m_CHECK_MODE_INTWIPER    );
    DDX_Check(pDX, IDC_CHECK_MODE_MANUAL_OFF , m_CHECK_MODE_MANUAL_OFF  );
	DDX_Text(pDX, IDC_EDIT_BARCODE_PCB,   m_EDIT_BARCODE_PCB);
	DDX_Text(pDX, IDC_EDIT_BARCODE_CASE,  m_EDIT_BARCODE_CASE);
	DDX_Text(pDX, IDC_EDIT_BARCODE_CASE2, m_EDIT_BARCODE_CASE2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(MainDlg, CDialog)
    //{{AFX_MSG_MAP(MainDlg)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()

    ON_WM_DESTROY()
    ON_MESSAGE(_WM_THREAD_END   , OnThreadMsgEnd   )
    ON_MESSAGE(_WM_THREAD_UPDATE, OnThreadMsgUpdate)
    ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	ON_EN_CHANGE(IDC_EDIT_MONITOR, OnChangeEditMonitor)
	ON_BN_CLICKED(IDC_BTN_MONITOR, OnBtnMonitor)
	ON_BN_CLICKED(IDC_BTN_MAUAL_ERROR, OnBtnMaualError)
	ON_CBN_SELCHANGE(IDC_COMBO_TESTER_LINE, OnSelchangeComboTesterLine)
	ON_CBN_SELCHANGE(IDC_COMBO_TESTER_NAME, OnSelchangeComboTesterName)
	ON_CBN_SELCHANGE(IDC_COMBO_PROGRAM_TYPE, OnSelchangeComboProgramType)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &MainDlg::OnBnClickedBtnStart)

	ON_BN_CLICKED(IDC_BUTTON1, &MainDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &MainDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MainDlg message handlers

BOOL MainDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    // TODO: Add extra initialization here
    CString strMsg;

#if _SCREW_SERIAL
	m_EDIT_TESTER_LINE = _T("PLK AAVM");
	m_STATIC_LINE_NAME = _T("���θ� : PLK AAVM");
#else
    m_EDIT_TESTER_LINE = _T("PLK AAVM 1ȣ��");
    m_STATIC_LINE_NAME = _T("���θ� : PLK AAVM 1ȣ��");
#endif

    GetDlgItem(IDC_BTN_MAUAL_ERROR)->EnableWindow(FALSE);
    pMainDlg = this;
    pSetupDlg = new CDialogSet(this);

    // Setup
    m_pFileNameSetup  = m_ExeFileName.m_szDrive;
    m_pFileNameSetup += m_ExeFileName.m_szDir;
    m_pFileNameSetup += m_ExeFileName.m_szFname;
    m_pFileNameSetup += ".ini";
#if 1
    if (pSetupDlg->LoadFromData(m_pFileNameSetup) == false)
    {
        if (pSetupDlg->SaveToData(m_pFileNameSetup) == false)
        {
            AfxMessageBox("Setup File Open Error!");
        }
    }
#endif
    // System Data Load
    m_pFileNameSystem  = m_ExeFileName.m_szDrive;
    m_pFileNameSystem += m_ExeFileName.m_szDir;
    m_pFileNameSystem += "AAVM_Checker.dat";
	m_pFileNameSystem = "D:\\AAVMLOG\AAVM_Checker.dat";	//2012.02.22

    FILE *pFile;

    pFile = fopen(m_pFileNameSystem, "rt");

    if (pFile == NULL)
    {
        pFile = fopen(m_pFileNameSystem, "w+t");
        if (pFile == NULL)
        {
            strMsg.Format("�ý��� ������ ������ ���� �����ϴ�.\n%s", (LPCTSTR)m_pFileNameSystem);
        }
        else
        {
            // ZIG_01 :
			fprintf(pFile, "ZIG_01 PIN USED = %d\n", m_EDIT_PIN_USED);
			fprintf(pFile, "ZIG_02 PIN USED = %d\n", m_EDIT_PIN_USED);
			fprintf(pFile, "ZIG_03 PIN USED = %d\n", m_EDIT_PIN_USED);
			fprintf(pFile, "ZIG_04 PIN USED = %d\n", m_EDIT_PIN_USED);
			fprintf(pFile, "ZIG_05 PIN USED = %d\n", m_EDIT_PIN_USED);
            fclose(pFile);
        }
    }
    else
    {
        // ZIG_01 :
		fscanf(pFile, "ZIG_01 PIN USED = %d\n", &n_Zig_01_Pin_Used);
		fscanf(pFile, "ZIG_02 PIN USED = %d\n", &n_Zig_02_Pin_Used);
		fscanf(pFile, "ZIG_03 PIN USED = %d\n", &n_Zig_03_Pin_Used);
		fscanf(pFile, "ZIG_04 PIN USED = %d\n", &n_Zig_04_Pin_Used);
		fscanf(pFile, "ZIG_05 PIN USED = %d\n", &n_Zig_05_Pin_Used);

		//if ((pSetupDlg->m_COMBO_CAR_TYPE == "29TON"))
		m_EDIT_PIN_USED = n_Zig_01_Pin_Used;
		//else m_EDIT_PIN_USED = n_Zig_03_Pin_Used;
        fclose(pFile);
    }

    // Set Guide
    LOGFONT oLogFont;
    GetFont()->GetLogFont(&oLogFont);

//    oLogFont.lfHeight = -60;
    oLogFont.lfHeight = -50;
    m_TXT_GUIDE.SetLogFont(&oLogFont);
    m_TXT_GUIDE.SetBgColor(RGB(255,255,255));
    m_TXT_GUIDE.SetTextColor(RGB(0,0,0));
    m_TXT_GUIDE.SetTextFormat(DT_CENTER|DT_VCENTER);

    m_TXT_GUIDE.m_strMsg = "PLK Technologies ";
    m_TXT_GUIDE.m_strMsg+= "AAVM";


    //Set Label - mook
    m_TXT_LABEL.SetLogFont(&oLogFont);
    m_TXT_LABEL.SetBgColor(RGB(255,255,255));
    m_TXT_LABEL.SetTextColor(RGB(0,0,0));
    m_TXT_LABEL.SetTextFormat(DT_CENTER|DT_VCENTER);

    m_TXT_LABEL.SetBgColor (WHITE);
    m_TXT_LABEL.m_strMsg = "��� ����\nȮ���ϼ���";

    if (pSetupDlg->m_COMBO_LOCATION != "����")
    {
        m_TXT_GUIDE.m_strMsg += " ";
        m_TXT_GUIDE.m_strMsg += pSetupDlg->m_COMBO_LOCATION;
        m_TXT_GUIDE.m_strMsg += "��";
    }
    m_TXT_GUIDE.m_strMsg += "\n��� �����˻�";

    m_EDIT_CAR_NAME = pSetupDlg->m_COMBO_CAR_TYPE;
    if (pSetupDlg->m_COMBO_LOCATION != "����")
    {
        m_EDIT_CAR_NAME += " ";
        m_EDIT_CAR_NAME += pSetupDlg->m_COMBO_LOCATION;
        m_EDIT_CAR_NAME += "��";
    }

    m_pThreadTestMain = NULL;

    m_LIST_PROCESS.InsertColumn(0, "�˻��׸�", LVCFMT_CENTER, 180);
    m_LIST_PROCESS.InsertColumn(1, "��ǥ��"  , LVCFMT_CENTER, 230);
    m_LIST_PROCESS.InsertColumn(2, "������"  , LVCFMT_CENTER, 230);
    m_LIST_PROCESS.InsertColumn(3, "����"    , LVCFMT_CENTER,  85);
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

    //mook
    for (int k=0; k<2; k++)		m_CtrlComboTesterLine.InsertString(k, StrTesterLine[k]);
    m_CtrlComboTesterLine.SetCurSel (0);
    for (int k=0; k<9; k++)		m_CtrlComboTesterName.InsertString(k, StrTesterName[k]);
    m_CtrlComboTesterName.SetCurSel (0);

    // box barcode max setting
    UDACCEL accels[]={{0, 1}};
    m_SPIN_BOX_UNIT_CNT.SetRange(0, 18);
	m_SPIN_BOX_UNIT_CNT.SetAccel(1, accels);

    UpdateData(FALSE);

	OnSelchangeComboProgramType();	//2012.02.22

//    dprintf(("WINVER = 0x%08X\n", _winver));

    return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void MainDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);

    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR MainDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void MainDlg::OnDestroy()
{
    CDialog::OnDestroy();

    // TODO: Add your message handler code here
	// System Data Save
    FILE *pFile;
    CString strMsg;

    pFile = fopen(m_pFileNameSystem, "w+t");

    if (pFile == NULL)
    {
        strMsg.Format("�ý��� ������ ������ ���� �����ϴ�.\n%s", (LPCTSTR)m_pFileNameSystem);
    }
    else
    {
		// ZIG_01 : VI ����, ZIG_02 : KH ����, ZIG_03 : BH ����, ZIG_04 : RP ����, ZIG_05 : QZ ����
		//if ( (pSetupDlg->m_COMBO_CAR_TYPE == "29TON") )
			n_Zig_01_Pin_Used = m_EDIT_PIN_USED;
		//else n_Zig_03_Pin_Used = m_EDIT_PIN_USED;

		fprintf(pFile, "ZIG_01 PIN USED = %d\n", n_Zig_01_Pin_Used);
		fprintf(pFile, "ZIG_02 PIN USED = %d\n", n_Zig_02_Pin_Used);
		fprintf(pFile, "ZIG_03 PIN USED = %d\n", n_Zig_03_Pin_Used);
		fprintf(pFile, "ZIG_04 PIN USED = %d\n", n_Zig_04_Pin_Used);
		fprintf(pFile, "ZIG_05 PIN USED = %d\n", n_Zig_05_Pin_Used);

        fclose(pFile);
    }

    if (m_pThreadTestMain != NULL)
    {
        tMainThreadStop = true;
        WaitForSingleObject(m_pThreadTestMain->m_hThread, 1000);
        m_pThreadTestMain = NULL;
    }
#if 0
	if(m_pThreadFrontImage !=NULL)
	{
		tFImageThreadStop= true;
//		WaitForSingleObject(m_pThreadFrontImage->m_hThread, 1000);
		m_pThreadTestMain = NULL;
	}
	if(m_pThreadRearImage !=NULL)
	{
		tReImageThreadStop= true;
//		WaitForSingleObject(m_pThreadRearImage->m_hThread, 1000);
		m_pThreadTestMain = NULL;
	}
	if(m_pThreadLeftImage !=NULL)
	{
		tLImageThreadStop= true;
//		WaitForSingleObject(m_pThreadLeftImage->m_hThread, 1000);
		m_pThreadTestMain = NULL;
	}
	if(m_pThreadRightImage !=NULL)
	{
		tRImageThreadStop= true;
//		WaitForSingleObject(m_pThreadRightImage->m_hThread, 1000);
		m_pThreadTestMain = NULL;
	}
#endif


    if (pImgDlg != NULL)
    {
        delete pImgDlg;
        pImgDlg = NULL;
    }

    if (pSetupDlg != NULL)
    {
        delete pSetupDlg;
        pSetupDlg = NULL;
    }
}

LRESULT MainDlg::OnThreadMsgEnd(WPARAM a_wParam, LPARAM a_lParam)
{
    OnThreadMsgUpdate(0,0);

    tMainThreadStop  = false;

	tFImageThreadStop = false;
	tReImageThreadStop = false;
	tLImageThreadStop = false;
	tRImageThreadStop = false;

	m_pThreadTestMain   = NULL;
	m_pThreadFrontImage	= NULL;
	m_pThreadRearImage	= NULL;
	m_pThreadLeftImage	= NULL;
	m_pThreadRightImage	= NULL;



    if (pImgDlg != NULL)
    {
        delete pImgDlg;
        pImgDlg = NULL;
    }

    GetDlgItem(IDC_BTN_SET        )->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_TEST       )->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_TEST       )->SetWindowText("����");
    GetDlgItem(IDC_BTN_MAUAL_ERROR)->EnableWindow(FALSE);
    GetDlgItem(IDOK               )->EnableWindow(TRUE);

    m_TXT_GUIDE.SetBgColor(RGB(255, 255, 255));

	return 0;
}

LRESULT MainDlg::OnThreadMsgUpdate(WPARAM a_wParam, LPARAM a_lParam)
{
    CTime oTime;
    CRect oRect;
    CSize oSize;
    int nScrollNeed;

    oTime = CTime::GetCurrentTime();
    m_EDIT_TESTER_TIME.Format("%4d-%02d-%02d %02d:%02d:%02d",
        oTime.GetYear(), oTime.GetMonth(), oTime.GetDay(),
        oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());

    nScrollNeed = m_LIST_PROCESS.GetItemCount() - (m_LIST_PROCESS.GetScrollPos(SB_VERT) + m_LIST_PROCESS.GetCountPerPage());
    if (nScrollNeed > 0)
    {
        m_LIST_PROCESS.GetSubItemRect(0, 0, LVIR_BOUNDS, oRect);
        nScrollNeed *= (oRect.bottom - oRect.top);
        oSize.cx = 0; oSize.cy = nScrollNeed;
        m_LIST_PROCESS.Scroll(oSize);
    }

    m_TXT_GUIDE.PostMessage(_WM_THREAD_UPDATE);
    UpdateData(FALSE);

	return 0;
}

void MainDlg::OnBtnSet()
{
    CSetupData *pPrevDlg = new CSetupData;

    pSetupDlg->CopyToData(pPrevDlg);

    if (pSetupDlg->DoModal() == IDOK)
    {
        m_TXT_GUIDE.m_strMsg = "���� �Ϸ�";

        m_TXT_LABEL.SetBgColor (WHITE);
		m_TXT_LABEL.m_strMsg = "��� ����\nȮ���ϼ���";

        // Save Setting Data
        if (pSetupDlg->SaveToData(m_pFileNameSetup) == false)
        {
            AfxMessageBox("Setup File Save Error!");
        }
    }
    else
    {
        m_TXT_GUIDE.m_strMsg = "���� ���";
       // Restore Setting Data
        pSetupDlg->CopyFromData(pPrevDlg);
    }

    m_TXT_GUIDE.PostMessage(_WM_THREAD_UPDATE);
    m_TXT_LABEL.PostMessage(_WM_THREAD_UPDATE); //mook

    m_EDIT_CAR_NAME = pSetupDlg->m_COMBO_CAR_TYPE;
    if (pSetupDlg->m_COMBO_LOCATION != "����")
    {
        m_EDIT_CAR_NAME += " ";
        m_EDIT_CAR_NAME += pSetupDlg->m_COMBO_LOCATION;
        m_EDIT_CAR_NAME += "��";
    }

    UpdateData(FALSE);

    delete pPrevDlg;
    pPrevDlg = NULL;
}


bool MainDlg::CreateExpressImg()
{
    if (pImgDlg != NULL)
    {
        return true;
	}
    if (::GetSystemMetrics(SM_CMONITORS) < 5) // ����� ���� ���ϱ�
    {
        AfxMessageBox("����� 5�� �̻� �ʿ�.");
        return false;
    }
    // ��޸��� ����.
    pImgDlg = new CExpressImg();
    pImgDlg->Create(IDD_EXPRESS_IMG_DIALOG, this);

    // ���� ����� ����� ���̾�α� ���
    HMONITOR    hMonitorThis, hMonitorTarget;
    MONITORINFO oMonitorThis, oMonitorTarget;
    POINT oPoint;

    // ���� �������� ����� ��ġ ���ϱ�
    hMonitorThis = ::MonitorFromWindow( this->GetSafeHwnd(), MONITOR_DEFAULTTONEAREST );

    oMonitorThis.cbSize = sizeof(MONITORINFO);
    ::GetMonitorInfo(hMonitorThis, &oMonitorThis);

    // Ÿ�� �������� ����� ��ġ ���ϱ�
    if (oMonitorThis.rcMonitor.left != 0)
    {
        // ���� ������� ���� ����Ϳ� ��ġ�� �ִ�.
        // ���� Ÿ�� ������� �� �����(0, 0)�� �����Ѵ�.
        oPoint.x = 0; oPoint.y = 0;
        hMonitorTarget = ::MonitorFromPoint(oPoint, MONITOR_DEFAULTTONEAREST);
        oMonitorTarget.cbSize = sizeof(MONITORINFO);
        ::GetMonitorInfo(hMonitorTarget, &oMonitorTarget);
    }
    else
    {
        // ���� ������� �� ����Ϳ� ��ġ�� �ִ�.
        // ���� ����Ͱ� ���ʿ� �ִ��� �����ʿ� �ִ��� Ȯ���ؾ� �Ѵ�.
        oPoint.x = -1; oPoint.y = 0;
        hMonitorTarget = ::MonitorFromPoint(oPoint, MONITOR_DEFAULTTONULL);
        if (hMonitorTarget <= 0)
        {
            oPoint.x = oMonitorThis.rcMonitor.right+1; oPoint.y = 0;
            hMonitorTarget = ::MonitorFromPoint(oPoint, MONITOR_DEFAULTTONULL);
        }
        oMonitorTarget.cbSize = sizeof(MONITORINFO);
        ::GetMonitorInfo(hMonitorTarget, &oMonitorTarget);
    }

    // pImgDlg->MoveWindow(-1920, 0, 1920, 1080);
    pImgDlg->MoveWindow(oMonitorTarget.rcMonitor.left,
                     oMonitorTarget.rcMonitor.top,
                     oMonitorTarget.rcMonitor.right  - oMonitorTarget.rcMonitor.left,
                     oMonitorTarget.rcMonitor.bottom - oMonitorTarget.rcMonitor.top );

    // pImgDlg->MoveWindow(rtMainScreen.right+1, rtSubScreen.top, (rtSubScreen.right-rtMainScreen.right), rtSubScreen.bottom);

    // ���� ����� ����� ���̾�αװ� ���̵��� ����
    pImgDlg->ShowWindow(SW_SHOW);

    return true;
}

void MainDlg::OnChangeEditMonitor()
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    tImageShowCompleted = 0;
    m_nImgView	= m_EDIT_MONITOR;
}

void MainDlg::OnBtnMonitor()
{
	// TODO: Add your control notification handler code here
    if (pImgDlg == NULL)
    {
	    CreateExpressImg();
    }
    else
    {
        delete pImgDlg;
        pImgDlg = NULL;
    }
}

void MainDlg::OnBtnMaualError()
{
    tMainThreadManulError = true;
}

void MainDlg::OnSelchangeComboTesterLine()
{
	// TODO: Add your control notification handler code here
    m_nComboTesterLine = m_CtrlComboTesterLine.GetCurSel();

#if _SCREW_SERIAL
    m_CtrlComboTesterLine.GetLBText(m_nComboTesterLine, "PLK AAVM 3ȣ��");
    m_EDIT_TESTER_LINE = "PLK AAVM 3ȣ��";
#else
    m_CtrlComboTesterLine.GetLBText(m_nComboTesterLine, "PLK AAVM 1ȣ��");
    m_EDIT_TESTER_LINE = "PLK AAVM 1ȣ��";
#endif

//    m_CtrlComboTesterLine.GetLBText(m_nComboTesterLine, StrTesterLine[m_nComboTesterLine]);
//    m_EDIT_TESTER_LINE = StrTesterLine[m_nComboTesterLine];

    UpdateData(FALSE);
}

void MainDlg::OnSelchangeComboTesterName()
{
	// TODO: Add your control notification handler code here
    m_nComboTesterName = m_CtrlComboTesterName.GetCurSel();
    m_CtrlComboTesterName.GetLBText(m_nComboTesterName, StrTesterName[m_nComboTesterName]);
    m_EDIT_TESTER_NAME = StrTesterName[m_nComboTesterName];

    UpdateData(FALSE);
}

void MainDlg::OnSelchangeComboProgramType()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}



void MainDlg::OnBnClickedBtnStart()
{
	UpdateData(TRUE);

	if (m_pThreadTestMain == NULL)
    {
/*        if (m_EDIT_TESTER_LINE == "")
        {
            AfxMessageBox("���θ��� �����Ǿ� ���� �ʽ��ϴ�.");
            return;
        }
*/
        if (m_EDIT_TESTER_NAME == "")
        {
            AfxMessageBox("�˻��ڰ� �����Ǿ� ���� �ʽ��ϴ�.");
            return;
        }

		//(pSetupDlg->m_COMBO_CAR_TYPE == "HG") ? (uUnitMax = _BOX_HG_UNIT_CNT) : (uUnitMax = _BOX_DEFAULT_UNIT_CNT);



        m_EDIT_BOX_UNIT_MAX = _BOX_DEFAULT_UNIT_CNT;


		// box barcode max setting
        m_SPIN_BOX_UNIT_CNT.SetRange(0, m_EDIT_BOX_UNIT_MAX);

		// box barcode �Է�
		tBoxBarcodeState = true;

		OnBnClickedButton1();

		tMainThreadStop    = false;
		tFImageThreadStop  = false;
		tReImageThreadStop = false;
		tLImageThreadStop  = false;
		tRImageThreadStop  = false;
        m_pThreadTestMain  = AfxBeginThread(MainThread, this);

        GetDlgItem(IDC_BTN_SET        )->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_TEST       )->SetWindowText("����");
        GetDlgItem(IDC_BTN_MAUAL_ERROR)->EnableWindow(FALSE);

    }
    else
    {
        if (pImgDlg != NULL)
        {
            delete pImgDlg;
            pImgDlg = NULL;
        }

        GetDlgItem(IDC_BTN_TEST       )->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_SET        )->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_MAUAL_ERROR)->EnableWindow(FALSE);
        GetDlgItem(IDOK               )->EnableWindow(FALSE);

        tMainThreadStop    = true;
		tFImageThreadStop  = true;
		tReImageThreadStop = true;
		tLImageThreadStop  = true;
		tRImageThreadStop  = true;

    }

    UpdateData(FALSE);
}



bool MainDlg::CreateImage()
{

#if CreateImg_TEST
	pFImgDlg = new CFrontImg();
	pFImgDlg->Create(IDD_Front_Image, this);
	pFImgDlg->MoveWindow(1920, 0, 1280, 1024);
	pFImgDlg->ShowWindow(SW_SHOW);
	// ���� ����� ����� ���̾�αװ� ���̵��� ����

#else
	if ((pFImgDlg != NULL)||(pReImgDlg != NULL)||(pLImgDlg != NULL)||(pRImgDlg != NULL))
	{
		return true;
	}
	if (::GetSystemMetrics(SM_CMONITORS) < 5) // ����� ���� ���ϱ�
	{
		AfxMessageBox("����� 5�� �̻� �ʿ�.");
		return false;
	}

    //Front
	// ��޸��� ����.
	pFImgDlg = new CFrontImg();
	pFImgDlg->Create(IDD_Front_Image, this);
	pFImgDlg->MoveWindow(1600, 0, 1280, 720);
	// ���� ����� ����� ���̾�αװ� ���̵��� ����
	pFImgDlg->ShowWindow(SW_SHOW);

    //Rear
	// ��޸��� ����.
	pReImgDlg = new CRearImg();
	pReImgDlg->Create(IDD_Rear_Image, this);
	pReImgDlg->MoveWindow(2880, 0, 1280, 720);//890);
	// ���� ����� ����� ���̾�αװ� ���̵��� ����
	pReImgDlg->ShowWindow(SW_SHOW);

	//Left
	// ��޸��� ����.
	pLImgDlg = new CLeftImg();
	pLImgDlg->Create(IDD_Left_Image, this);
	pLImgDlg->MoveWindow(4160, 0, 1280, 720);
	// ���� ����� ����� ���̾�αװ� ���̵��� ����
	pLImgDlg->ShowWindow(SW_SHOW);

	//Right
	// ��޸��� ����.
	pRImgDlg = new CRightImg();
	pRImgDlg->Create(IDD_Right_Image, this);
	pRImgDlg->MoveWindow(5440, 0, 1280, 720);
	// ���� ����� ����� ���̾�αװ� ���̵��� ����
	pRImgDlg->ShowWindow(SW_SHOW);
#endif
	return true;
}


void MainDlg::OnBnClickedButton1()
{
	bool error;
#if 0
	CFileDialog	fileDlgF( TRUE, "jpg", "*.jpg", OFN_HIDEREADONLY, "����|*.jpg" );
	if ( fileDlgF.DoModal() != IDOK ) return;
	strFImagePath=fileDlgF.GetPathName();
	CFileDialog	fileDlgRe( TRUE, "jpg", "*.jpg", OFN_HIDEREADONLY, "����|*.jpg" );
	if ( fileDlgRe.DoModal() != IDOK ) return;
	strReImagePath=fileDlgRe.GetPathName();
	CFileDialog	fileDlgL( TRUE, "jpg", "*.jpg", OFN_HIDEREADONLY, "����|*.jpg" );
	if ( fileDlgL.DoModal() != IDOK ) return;
	strLImagePath=fileDlgL.GetPathName();
	CFileDialog	fileDlgR( TRUE, "jpg", "*.jpg", OFN_HIDEREADONLY, "����|*.jpg" );
	if ( fileDlgR.DoModal() != IDOK ) return;
	strRImagePath=fileDlgR.GetPathName();
#endif

#if 0
	strFImagePath  =_T("D:\\AAVM\\FrontImage\\FImage00001.jpg");
	strReImagePath =_T("D:\\AAVM\\RearImage\\ReImage00001.jpg");
	strLImagePath  =_T("D:\\AAVM\\LeftImage\\LImage00001.jpg");
	strRImagePath  =_T("D:\\AAVM\\RightImage\\RImage00001.jpg");
#elif 1
	strFImagePath  =_T("D:\\AAVM\\TestImage\\00001.jpg");
	strReImagePath =_T("D:\\AAVM\\RearImage\\ReImage00001.jpg");
	strLImagePath  =_T("D:\\AAVM\\LeftImage\\LImage00001.jpg");
	strRImagePath  =_T("D:\\AAVM\\RightImage\\RImage00001.jpg");
#endif

	error=CreateImage();

	ui_FStartNumber=1;
	ui_FStopNumber=1;

	ui_ReStartNumber=1;
	ui_ReStopNumber=1;

	ui_LStartNumber=1;
	ui_LStopNumber=1;

	ui_RStartNumber=1;
	ui_RStopNumber=1;
}

void MainDlg::OnBnClickedButton2()
{

}
#if 1
UINT FrontThread(LPVOID pParam)
{
    int ret;

	ret=1;
	while(1)
	{
		if(FDrawStop==false)
		{
			ret=pFImgDlg->FrontDraw(strFImagePath,ui_FStartNumber,ui_FStopNumber);
			//Sleep(70);
		}
		if(ret!=1)
		{
			FDrawStop =true;
			ui_FStartNumber=0;
			ui_FStopNumber=0;
			ret=1;
		}
	}

	return 0;
}

#endif

#if 1
UINT RearThread(LPVOID pParam)
{
	int ret;
	ret=1;
	while(1)
	{
		if(ReDrawStop==false)
		{
			ret=pReImgDlg->RearDraw(strReImagePath,ui_ReStartNumber,ui_ReStopNumber);
			//Sleep(90);
		}

		if(ret!=1)
		{
			ReDrawStop =true;
			ui_ReStartNumber=0;
			ui_ReStopNumber=0;
			ret=1;
		}
	}
	return 0;
}

#endif

#if 1
UINT LeftThread(LPVOID pParam)
{
	int ret;
	ret=1;
	while(1)
	{
		if(LDrawStop==false)
		{
			ret=pLImgDlg->LeftDraw(strLImagePath,ui_LStartNumber,ui_LStopNumber);
			//Sleep(90);
		}
		if(ret!=1)
		{
			LDrawStop =true;
			ui_LStartNumber=0;
			ui_LStopNumber=0;
			ret=1;
		}

	}

	return 0;
}

#endif

#if 1
UINT RightThread(LPVOID pParam)
{
	int ret;
	ret=1;
	while(1)
	{
		if(RDrawStop==false)
		{
			ret=pRImgDlg->RightDraw(strRImagePath,ui_RStartNumber,ui_RStopNumber);
			//Sleep(90);
		}
		if(ret!=1)
		{
			RDrawStop =true;
			ui_RStartNumber=0;
			ui_RStopNumber=0;
			ret=1;
		}

	}

	return 0;
}

#endif