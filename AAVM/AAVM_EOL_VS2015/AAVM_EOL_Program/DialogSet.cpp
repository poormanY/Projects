// DialogSet.cpp : implementation file
//

#include "stdafx.h"
#include "MainApp.h"
#include "DialogSet.h"
#include "wm_user.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern bool tSetupThreadStop;
extern UINT SetupThread(LPVOID pParam);

/////////////////////////////////////////////////////////////////////////////
// CDialogSet dialog


CDialogSet::CDialogSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSet)
	m_EDIT_COM_AMMETER = _T("COM2");
	m_EDIT_COM_EMERAD= _T("COM10");
	m_EDIT_COM_BF609 = _T("COM11");

	//m_COMBO_CAR_TYPE = _T("AAVMHX140L");
	m_COMBO_CAR_TYPE = _T("설정 필요"); //thlee
	m_COMBO_LOCATION = _T("없음");
	m_EDIT_COM_BARCODE1 = _T("COM6");
	m_EDIT_COM_BARCODE2 = _T("COM5");
	m_EDIT_COM_IOBOARD = _T("COM1");
	m_EDIT_COM_SCREW = _T("COM4");
	m_EDIT_ECU_ID = _T("AAVM CONTOLORER");
	m_EDIT_BF_VER_BOOT = _T("1200");
	m_EDIT_ECU_VER_HW = _T("1100");
	m_EDIT_ECU_VER_SW = _T("1200");
	m_EDIT_BF_VER_ARG = _T("1200");
	m_EDIT_FILENAME_LOG = _T("AAVM_test.log");
    m_EDIT_IO_IN_CONNECT = _T("0");
    m_EDIT_IO_OUT_BARCODE1 = _T("13");
	m_EDIT_IO_OUT_BARCODE2 = _T("14");
	m_EDIT_IO_OUT_DISCONNECT = _T("1");
	m_EDIT_IO_OUT_LED_G = _T("3");
	m_EDIT_IO_OUT_LED_R = _T("4");
	m_EDIT_IO_OUT_LED_Y = _T("2");
	m_EDIT_IO_OUT_POWER = _T("0");
    m_EDIT_TEST_BARCODE1 = _T("");
	m_EDIT_TEST_BARCODE2 = _T("");
	m_EDIT_TEST_IOBOARD = _T("");
	m_EDIT_TEST_SCREW = _T("");
	m_CHECK_IO_IN_CONNECT = FALSE;
	m_CHECK_IO_OUT_BARCODE1 = FALSE;
	m_CHECK_IO_OUT_BARCODE2 = FALSE;
	m_CHECK_IO_OUT_DISCONNECT = FALSE;
	m_CHECK_IO_OUT_LED_G = FALSE;
	m_CHECK_IO_OUT_LED_R = FALSE;
	m_CHECK_IO_OUT_LED_Y = FALSE;
	m_CHECK_IO_OUT_POWER = FALSE;
	m_COMBO_TIME_DELAY = _T("");
	m_EDIT_IO_IN_AMPERE_OVER_12 = _T("600");
	m_EDIT_IO_IN_AMPERE_UNDER_12 = _T("400");
	m_EDIT_IO_IN_AMPERE_OVER_24 = _T("300");
	m_EDIT_IO_IN_AMPERE_UNDER_24= _T("200");

	//}}AFX_DATA_INIT
}


void CDialogSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSet)
	DDX_CBString(pDX, IDC_COMBO_CAR_TYPE, m_COMBO_CAR_TYPE);
	DDX_CBString(pDX, IDC_COMBO_LOCATION, m_COMBO_LOCATION);
	DDX_Text(pDX, IDC_EDIT_COM_BARCODE_1, m_EDIT_COM_BARCODE1);
	DDX_Text(pDX, IDC_EDIT_COM_BARCODE_2, m_EDIT_COM_BARCODE2);
	DDX_Text(pDX, IDC_EDIT_COM_IOBOARD_1, m_EDIT_COM_IOBOARD);
	DDX_Text(pDX, IDC_EDIT_COM_SCREW, m_EDIT_COM_SCREW);
	DDX_Text(pDX, IDC_EDIT_ECU_ID, m_EDIT_ECU_ID);
	DDX_Text(pDX, IDC_EDIT_BF_BOOT_VER, m_EDIT_BF_VER_BOOT);
	DDX_Text(pDX, IDC_EDIT_ECU_VER_HW, m_EDIT_ECU_VER_HW);
	DDX_Text(pDX, IDC_EDIT_ECU_VER_SW, m_EDIT_ECU_VER_SW);
	DDX_Text(pDX, IDC_EDIT_BF_VER_ARG, m_EDIT_BF_VER_ARG);
	DDX_Text(pDX, IDC_EDIT_FILENAME_LOG_1, m_EDIT_FILENAME_LOG);
	DDX_Text(pDX, IDC_EDIT_IO_IN_CONNECT, m_EDIT_IO_IN_CONNECT);
	DDX_Text(pDX, IDC_EDIT_IO_OUT_BARCODE_1, m_EDIT_IO_OUT_BARCODE1);
	DDX_Text(pDX, IDC_EDIT_IO_OUT_BARCODE_2, m_EDIT_IO_OUT_BARCODE2);
	DDX_Text(pDX, IDC_EDIT_IO_OUT_DISCONECT, m_EDIT_IO_OUT_DISCONNECT);
	DDX_Text(pDX, IDC_EDIT_IO_OUT_LED_G, m_EDIT_IO_OUT_LED_G);
	DDX_Text(pDX, IDC_EDIT_IO_OUT_LED_R, m_EDIT_IO_OUT_LED_R);
	DDX_Text(pDX, IDC_EDIT_IO_OUT_LED_Y, m_EDIT_IO_OUT_LED_Y);
	DDX_Text(pDX, IDC_EDIT_IO_OUT_POWER, m_EDIT_IO_OUT_POWER);
	DDX_Text(pDX, IDC_EDIT_TEST_BARCODE1, m_EDIT_TEST_BARCODE1);
	DDX_Text(pDX, IDC_EDIT_TEST_BARCODE2, m_EDIT_TEST_BARCODE2);
	DDX_Text(pDX, IDC_EDIT_TEST_IOBOARD, m_EDIT_TEST_IOBOARD);
	DDX_Text(pDX, IDC_EDIT_TEST_SCREW, m_EDIT_TEST_SCREW);

	DDX_Check(pDX, IDC_CHECK_IO_IN_CONNECT, m_CHECK_IO_IN_CONNECT);
	DDX_Check(pDX, IDC_CHECK_IO_OUT_BARCODE1, m_CHECK_IO_OUT_BARCODE1);
	DDX_Check(pDX, IDC_CHECK_IO_OUT_BARCODE2, m_CHECK_IO_OUT_BARCODE2);
	DDX_Check(pDX, IDC_CHECK_IO_OUT_DISCONNECT, m_CHECK_IO_OUT_DISCONNECT);
	DDX_Check(pDX, IDC_CHECK_IO_OUT_LED_G, m_CHECK_IO_OUT_LED_G);
	DDX_Check(pDX, IDC_CHECK_IO_OUT_LED_R, m_CHECK_IO_OUT_LED_R);
	DDX_Check(pDX, IDC_CHECK_IO_OUT_LED_Y, m_CHECK_IO_OUT_LED_Y);
	DDX_Check(pDX, IDC_CHECK_IO_OUT_POWER, m_CHECK_IO_OUT_POWER);
	DDX_CBString(pDX, IDC_COMBO_TIME_DELAY, m_COMBO_TIME_DELAY);
	DDX_Text(pDX, IDC_EDIT_COM_AMMETER2, m_EDIT_COM_AMMETER);
	DDX_Text(pDX, IDC_EDIT_COM_Emerald, m_EDIT_COM_EMERAD);
	DDX_Text(pDX, IDC_EDIT_COM_BF609, m_EDIT_COM_BF609);

	DDX_Text(pDX, IDC_EDIT_IO_IN_AMPERE_OVER_12, m_EDIT_IO_IN_AMPERE_OVER_12);
	DDX_Text(pDX, IDC_EDIT_IO_IN_AMPERE_UNDER_12, m_EDIT_IO_IN_AMPERE_UNDER_12);
	DDX_Text(pDX, IDC_EDIT_IO_IN_AMPERE_OVER_24, m_EDIT_IO_IN_AMPERE_OVER_24);
	DDX_Text(pDX, IDC_EDIT_IO_IN_AMPERE_UNDER_24, m_EDIT_IO_IN_AMPERE_UNDER_24);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSet, CDialog)
	//{{AFX_MSG_MAP(CDialogSet)
	ON_BN_CLICKED(IDC_BTN_SET_TEST, OnBtnSetTest)
    ON_MESSAGE(_WM_THREAD_END   , OnThreadMsgEnd   )
    ON_MESSAGE(_WM_THREAD_UPDATE, OnThreadMsgUpdate)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_IO_OUT_BARCODE1, OnCheckIoOutBarcode1)
	ON_BN_CLICKED(IDC_CHECK_IO_OUT_BARCODE2, OnCheckIoOutBarcode2)
	ON_BN_CLICKED(IDC_CHECK_IO_OUT_DISCONNECT, OnCheckIoOutDisconnect)
	ON_BN_CLICKED(IDC_CHECK_IO_OUT_LED_G, OnCheckIoOutLedG)
	ON_BN_CLICKED(IDC_CHECK_IO_OUT_LED_R, OnCheckIoOutLedR)
	ON_BN_CLICKED(IDC_CHECK_IO_OUT_LED_Y, OnCheckIoOutLedY)
	ON_BN_CLICKED(IDC_CHECK_IO_OUT_POWER, OnCheckIoOutPower)
	ON_CBN_SELCHANGE(IDC_COMBO_CAR_TYPE, OnSelchangeComboCarType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSet message handlers

void CDialogSet::OnOK()
{
	// TODO: Add extra validation here
    UpdateData(TRUE);

	CDialog::OnOK();
}

BOOL CDialogSet::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
    m_CHECK_IO_OUT_DISCONNECT = true;

    m_pThreadTestSetup = NULL;

    TestRun(false);

    UpdateData(FALSE);

    OnSelchangeComboCarType();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogSet::CopyToData(CSetupData *a_pTgt)
{
	a_pTgt->m_EDIT_COM_BARCODE1       = this->m_EDIT_COM_BARCODE1      ;
	a_pTgt->m_EDIT_COM_BARCODE2       = this->m_EDIT_COM_BARCODE2      ;
	a_pTgt->m_EDIT_COM_IOBOARD        = this->m_EDIT_COM_IOBOARD       ;
	a_pTgt->m_EDIT_COM_SCREW          = this->m_EDIT_COM_SCREW         ;
	//psj Ammeter
	a_pTgt->m_EDIT_COM_AMMETER        = this->m_EDIT_COM_AMMETER       ;
	a_pTgt->m_EDIT_COM_EMERAD		  = this->m_EDIT_COM_EMERAD       ;
	a_pTgt->m_EDIT_COM_BF609		  = this->m_EDIT_COM_BF609       ;

	a_pTgt->m_EDIT_IO_IN_CONNECT      = this->m_EDIT_IO_IN_CONNECT     ;
    //psj can line test

	a_pTgt->m_EDIT_IO_OUT_BARCODE1    = this->m_EDIT_IO_OUT_BARCODE1   ;
	a_pTgt->m_EDIT_IO_OUT_BARCODE2    = this->m_EDIT_IO_OUT_BARCODE2   ;
	a_pTgt->m_EDIT_IO_OUT_DISCONNECT  = this->m_EDIT_IO_OUT_DISCONNECT ;
	a_pTgt->m_EDIT_IO_OUT_LED_G       = this->m_EDIT_IO_OUT_LED_G      ;
	a_pTgt->m_EDIT_IO_OUT_LED_R       = this->m_EDIT_IO_OUT_LED_R      ;
	a_pTgt->m_EDIT_IO_OUT_LED_Y       = this->m_EDIT_IO_OUT_LED_Y      ;
	a_pTgt->m_EDIT_IO_OUT_POWER       = this->m_EDIT_IO_OUT_POWER      ;
	a_pTgt->m_COMBO_CAR_TYPE          = this->m_COMBO_CAR_TYPE         ;
	a_pTgt->m_COMBO_LOCATION          = this->m_COMBO_LOCATION         ;
	a_pTgt->m_EDIT_ECU_ID             = this->m_EDIT_ECU_ID            ;
	a_pTgt->m_EDIT_BF_VER_BOOT        = this->m_EDIT_BF_VER_BOOT       ;
	a_pTgt->m_EDIT_BF_VER_ARG	      = this->m_EDIT_BF_VER_BOOT       ;
	a_pTgt->m_EDIT_ECU_VER_HW         = this->m_EDIT_ECU_VER_HW        ;
	a_pTgt->m_EDIT_ECU_VER_SW         = this->m_EDIT_ECU_VER_SW        ;
	//psj Ammeter
	a_pTgt->m_EDIT_IO_IN_AMPERE_OVER_12  = this->m_EDIT_IO_IN_AMPERE_OVER_12 ;
	a_pTgt->m_EDIT_IO_IN_AMPERE_UNDER_12 = this->m_EDIT_IO_IN_AMPERE_UNDER_12;
	a_pTgt->m_EDIT_IO_IN_AMPERE_OVER_24  = this->m_EDIT_IO_IN_AMPERE_OVER_24 ;
	a_pTgt->m_EDIT_IO_IN_AMPERE_UNDER_24 = this->m_EDIT_IO_IN_AMPERE_UNDER_24;
	a_pTgt->m_EDIT_FILENAME_LOG       = this->m_EDIT_FILENAME_LOG      ;
}

void CDialogSet::CopyFromData(CSetupData *a_pSrc)
{
	this->m_EDIT_COM_BARCODE1       = a_pSrc->m_EDIT_COM_BARCODE1      ;
	this->m_EDIT_COM_BARCODE2       = a_pSrc->m_EDIT_COM_BARCODE2      ;
	this->m_EDIT_COM_IOBOARD        = a_pSrc->m_EDIT_COM_IOBOARD       ;
	this->m_EDIT_COM_SCREW          = a_pSrc->m_EDIT_COM_SCREW         ;
	//psj Ammeter
	this->m_EDIT_COM_AMMETER        = a_pSrc->m_EDIT_COM_AMMETER       ;
	this->m_EDIT_COM_EMERAD         = a_pSrc->m_EDIT_COM_EMERAD       ;
	this->m_EDIT_COM_BF609          = a_pSrc->m_EDIT_COM_BF609       ;

	this->m_EDIT_IO_IN_CONNECT      = a_pSrc->m_EDIT_IO_IN_CONNECT     ;
    //psj can line test

	this->m_EDIT_IO_OUT_BARCODE1    = a_pSrc->m_EDIT_IO_OUT_BARCODE1   ;
	this->m_EDIT_IO_OUT_BARCODE2    = a_pSrc->m_EDIT_IO_OUT_BARCODE2   ;
	this->m_EDIT_IO_OUT_DISCONNECT  = a_pSrc->m_EDIT_IO_OUT_DISCONNECT ;
	this->m_EDIT_IO_OUT_LED_G       = a_pSrc->m_EDIT_IO_OUT_LED_G      ;
	this->m_EDIT_IO_OUT_LED_R       = a_pSrc->m_EDIT_IO_OUT_LED_R      ;
	this->m_EDIT_IO_OUT_LED_Y       = a_pSrc->m_EDIT_IO_OUT_LED_Y      ;
	this->m_EDIT_IO_OUT_POWER       = a_pSrc->m_EDIT_IO_OUT_POWER      ;
	this->m_COMBO_CAR_TYPE          = a_pSrc->m_COMBO_CAR_TYPE         ;
	this->m_COMBO_LOCATION          = a_pSrc->m_COMBO_LOCATION         ;
	this->m_EDIT_ECU_ID             = a_pSrc->m_EDIT_ECU_ID            ;
	this->m_EDIT_BF_VER_BOOT       = a_pSrc->m_EDIT_BF_VER_BOOT        ;
	this->m_EDIT_ECU_VER_HW         = a_pSrc->m_EDIT_ECU_VER_HW        ;
	this->m_EDIT_ECU_VER_SW         = a_pSrc->m_EDIT_ECU_VER_SW        ;
	this->m_EDIT_BF_VER_ARG         = a_pSrc->m_EDIT_BF_VER_ARG        ;
	//psj Ammeter
	this->m_EDIT_IO_IN_AMPERE_OVER_12  = a_pSrc->m_EDIT_IO_IN_AMPERE_OVER_12 ;
	this->m_EDIT_IO_IN_AMPERE_UNDER_12 = a_pSrc->m_EDIT_IO_IN_AMPERE_UNDER_12;
	this->m_EDIT_IO_IN_AMPERE_OVER_24  = a_pSrc->m_EDIT_IO_IN_AMPERE_OVER_24 ;
	this->m_EDIT_IO_IN_AMPERE_UNDER_24 = a_pSrc->m_EDIT_IO_IN_AMPERE_UNDER_24;
	this->m_EDIT_FILENAME_LOG       = a_pSrc->m_EDIT_FILENAME_LOG      ;
}

bool CDialogSet::SaveToData(LPCSTR a_pszFileName)
{
    FILE *pFile;

    if (a_pszFileName == NULL)
    {
        return false;
    }

    pFile = fopen(a_pszFileName, "wt+");

    if (pFile == NULL)
    {
        return false;
    }

	fprintf(pFile, "COM_BARCODE1       = %s\n", (LPCSTR)this->m_EDIT_COM_BARCODE1      );
	fprintf(pFile, "COM_BARCODE2       = %s\n", (LPCSTR)this->m_EDIT_COM_BARCODE2      );
	fprintf(pFile, "COM_IOBOARD        = %s\n", (LPCSTR)this->m_EDIT_COM_IOBOARD       );
	fprintf(pFile, "COM_SCREW          = %s\n", (LPCSTR)this->m_EDIT_COM_SCREW         );
	//psj Ammeter
	fprintf(pFile, "COM_AMMETER        = %s\n", (LPCSTR)this->m_EDIT_COM_AMMETER       );
	fprintf(pFile, "COM_EMERAD         = %s\n", (LPCSTR)this->m_EDIT_COM_EMERAD        );
	fprintf(pFile, "COM_BF609          = %s\n", (LPCSTR)this->m_EDIT_COM_BF609         );

	fprintf(pFile, "IO_IN_CONNECT      = %s\n", (LPCSTR)this->m_EDIT_IO_IN_CONNECT     );
    //psj can line test

	fprintf(pFile, "IO_OUT_BARCODE1    = %s\n", (LPCSTR)this->m_EDIT_IO_OUT_BARCODE1   );
	fprintf(pFile, "IO_OUT_BARCODE2    = %s\n", (LPCSTR)this->m_EDIT_IO_OUT_BARCODE2   );
	fprintf(pFile, "IO_OUT_DISCONNECT  = %s\n", (LPCSTR)this->m_EDIT_IO_OUT_DISCONNECT );
	fprintf(pFile, "IO_OUT_LED_G       = %s\n", (LPCSTR)this->m_EDIT_IO_OUT_LED_G      );
	fprintf(pFile, "IO_OUT_LED_R       = %s\n", (LPCSTR)this->m_EDIT_IO_OUT_LED_R      );
	fprintf(pFile, "IO_OUT_LED_Y       = %s\n", (LPCSTR)this->m_EDIT_IO_OUT_LED_Y      );
	fprintf(pFile, "IO_OUT_POWER       = %s\n", (LPCSTR)this->m_EDIT_IO_OUT_POWER      );
	fprintf(pFile, "CAR_TYPE           = %s\n", (LPCSTR)this->m_COMBO_CAR_TYPE         );
	fprintf(pFile, "LOCATION           = %s\n", (LPCSTR)this->m_COMBO_LOCATION         );
	fprintf(pFile, "ECU_ID             = %s\n", (LPCSTR)this->m_EDIT_ECU_ID            );
	fprintf(pFile, "ECU_VER_BOOT       = %s\n", (LPCSTR)this->m_EDIT_BF_VER_BOOT       );
	fprintf(pFile, "ECU_VER_ARG        = %s\n", (LPCSTR)this->m_EDIT_BF_VER_ARG       );
	fprintf(pFile, "ECU_VER_HW         = %s\n", (LPCSTR)this->m_EDIT_ECU_VER_HW        );
	fprintf(pFile, "ECU_VER_SW         = %s\n", (LPCSTR)this->m_EDIT_ECU_VER_SW        );
	//psj Ammeter
	fprintf(pFile, "IO_IN_AMPERE_OVER  = %s\n", (LPCSTR)this->m_EDIT_IO_IN_AMPERE_OVER_12 );
	fprintf(pFile, "IO_IN_AMPERE_UNDER = %s\n", (LPCSTR)this->m_EDIT_IO_IN_AMPERE_UNDER_12 );
	fprintf(pFile, "IO_IN_AMPERE_OVER  = %s\n", (LPCSTR)this->m_EDIT_IO_IN_AMPERE_OVER_24  );
	fprintf(pFile, "IO_IN_AMPERE_UNDER = %s\n", (LPCSTR)this->m_EDIT_IO_IN_AMPERE_UNDER_24 );
	fprintf(pFile, "FILENAME_LOG       = %s\n", (LPCSTR)this->m_EDIT_FILENAME_LOG      );

	fclose(pFile);

	return true;
}

bool CDialogSet::LoadFromData(LPCSTR a_pszFileName)
{
    FILE *pFile;
    char  szData[256];

    if (a_pszFileName == NULL)
    {
        return false;
    }

    pFile = fopen(a_pszFileName, "rt");

    if (pFile == NULL)
    {
        return false;
    }

	fscanf(pFile, "COM_BARCODE1       = %s\n", szData); m_EDIT_COM_BARCODE1       = szData;
	fscanf(pFile, "COM_BARCODE2       = %s\n", szData); m_EDIT_COM_BARCODE2       = szData;
	fscanf(pFile, "COM_IOBOARD        = %s\n", szData); m_EDIT_COM_IOBOARD        = szData;
	fscanf(pFile, "COM_SCREW          = %s\n", szData); m_EDIT_COM_SCREW          = szData;
	//psj Ammeter
	fscanf(pFile, "COM_AMMETER        = %s\n", szData); m_EDIT_COM_AMMETER        = szData;
	fscanf(pFile, "COM_EMERAD         = %s\n", szData); m_EDIT_COM_EMERAD         = szData;
	fscanf(pFile, "COM_BF609          = %s\n", szData); m_EDIT_COM_BF609          = szData;

	fscanf(pFile, "IO_IN_CONNECT      = %s\n", szData); m_EDIT_IO_IN_CONNECT      = szData;
    //psj can line test

	fscanf(pFile, "IO_OUT_BARCODE1    = %s\n", szData); m_EDIT_IO_OUT_BARCODE1    = szData;
	fscanf(pFile, "IO_OUT_BARCODE2    = %s\n", szData); m_EDIT_IO_OUT_BARCODE2    = szData;
	fscanf(pFile, "IO_OUT_DISCONNECT  = %s\n", szData); m_EDIT_IO_OUT_DISCONNECT  = szData;
	fscanf(pFile, "IO_OUT_LED_G       = %s\n", szData); m_EDIT_IO_OUT_LED_G       = szData;
	fscanf(pFile, "IO_OUT_LED_R       = %s\n", szData); m_EDIT_IO_OUT_LED_R       = szData;
	fscanf(pFile, "IO_OUT_LED_Y       = %s\n", szData); m_EDIT_IO_OUT_LED_Y       = szData;
	fscanf(pFile, "IO_OUT_POWER       = %s\n", szData); m_EDIT_IO_OUT_POWER       = szData;
	fscanf(pFile, "CAR_TYPE           = %s\n", szData); m_COMBO_CAR_TYPE          = szData;
	fscanf(pFile, "LOCATION           = %s\n", szData); m_COMBO_LOCATION          = szData;
	fscanf(pFile, "ECU_ID             = %s\n", szData); m_EDIT_ECU_ID             = szData;
	fscanf(pFile, "ECU_VER_BOOT       = %s\n", szData); m_EDIT_BF_VER_BOOT        = szData;
	fscanf(pFile, "ECU_VER_ARG        = %s\n", szData); m_EDIT_BF_VER_ARG         = szData;
	fscanf(pFile, "ECU_VER_HW         = %s\n", szData); m_EDIT_ECU_VER_HW         = szData;
	fscanf(pFile, "ECU_VER_SW         = %s\n", szData); m_EDIT_ECU_VER_SW         = szData;
    //psj Ammeter
	fscanf(pFile, "IO_IN_AMPERE_OVER  = %s\n", szData); m_EDIT_IO_IN_AMPERE_OVER_12  = szData;
	fscanf(pFile, "IO_IN_AMPERE_UNDER = %s\n", szData); m_EDIT_IO_IN_AMPERE_UNDER_12 = szData;
	fscanf(pFile, "IO_IN_AMPERE_OVER  = %s\n", szData); m_EDIT_IO_IN_AMPERE_OVER_24  = szData;
	fscanf(pFile, "IO_IN_AMPERE_UNDER = %s\n", szData); m_EDIT_IO_IN_AMPERE_UNDER_24 = szData;
	fscanf(pFile, "FILENAME_LOG       = %s\n", szData); m_EDIT_FILENAME_LOG       = szData;

	fclose(pFile);

    return true;
}


void CDialogSet::OnBtnSetTest()
{
    UpdateData(TRUE);
    TestRun( (m_tTestOn == true)? false: true );
}

void CDialogSet::TestRun(bool a_tOn)
{
    if (a_tOn == true)
    {
        m_tTestOn = true;

        GetDlgItem(IDC_BTN_SET_TEST)->SetWindowText("테스트 중지");

        m_EDIT_TEST_BARCODE1 = "";
	    m_EDIT_TEST_BARCODE2 = "";
	    m_EDIT_TEST_IOBOARD  = "";
	    m_EDIT_TEST_SCREW    = "";

    	GetDlgItem(IDC_EDIT_COM_BARCODE_1     )->EnableWindow(FALSE);
    	GetDlgItem(IDC_EDIT_COM_BARCODE_2     )->EnableWindow(FALSE);
    	GetDlgItem(IDC_EDIT_COM_IOBOARD_1      )->EnableWindow(FALSE);
    	GetDlgItem(IDC_EDIT_COM_SCREW        )->EnableWindow(FALSE);
		//psj Ammeter
		GetDlgItem(IDC_EDIT_COM_AMMETER2      )->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM_Emerald      )->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM_BF609      )->EnableWindow(FALSE);
    	GetDlgItem(IDC_EDIT_IO_IN_CONNECT    )->EnableWindow(FALSE);
        //psj can line test
    	GetDlgItem(IDC_EDIT_IO_OUT_BARCODE_1  )->EnableWindow(FALSE);
    	GetDlgItem(IDC_EDIT_IO_OUT_BARCODE_2  )->EnableWindow(FALSE);
    	GetDlgItem(IDC_EDIT_IO_OUT_DISCONECT )->EnableWindow(FALSE);
    	GetDlgItem(IDC_EDIT_IO_OUT_LED_G     )->EnableWindow(FALSE);
    	GetDlgItem(IDC_EDIT_IO_OUT_LED_R     )->EnableWindow(FALSE);
    	GetDlgItem(IDC_EDIT_IO_OUT_LED_Y     )->EnableWindow(FALSE);
    	GetDlgItem(IDC_EDIT_IO_OUT_POWER     )->EnableWindow(FALSE);
    	GetDlgItem(IDC_COMBO_CAR_TYPE        )->EnableWindow(FALSE);
    	GetDlgItem(IDC_COMBO_LOCATION        )->EnableWindow(FALSE);
    	GetDlgItem(IDC_EDIT_ECU_ID           )->EnableWindow(FALSE);
    	GetDlgItem(IDC_EDIT_BF_BOOT_VER     )->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BF_VER_ARG     )->EnableWindow(FALSE);
    	GetDlgItem(IDC_EDIT_ECU_VER_HW       )->EnableWindow(FALSE);
    	GetDlgItem(IDC_EDIT_ECU_VER_SW       )->EnableWindow(FALSE);
    	GetDlgItem(IDC_EDIT_FILENAME_LOG_1     )->EnableWindow(FALSE);

        tSetupThreadStop = false;
        m_pThreadTestSetup = AfxBeginThread(SetupThread, this);

    }
    else
    {
        if (m_pThreadTestSetup != NULL)
        {
            tSetupThreadStop = true;
            WaitForSingleObject(m_pThreadTestSetup->m_hThread, 1000);
            tSetupThreadStop = NULL;
        }

        m_tTestOn = false;
        GetDlgItem(IDC_BTN_SET_TEST)->SetWindowText("테스트 시작");

    	GetDlgItem(IDC_EDIT_COM_BARCODE_1     )->EnableWindow(TRUE);
    	GetDlgItem(IDC_EDIT_COM_BARCODE_2     )->EnableWindow(TRUE);
    	GetDlgItem(IDC_EDIT_COM_IOBOARD_1      )->EnableWindow(TRUE);
    	GetDlgItem(IDC_EDIT_COM_SCREW        )->EnableWindow(TRUE);
		//psj Ammeter
		GetDlgItem(IDC_EDIT_COM_AMMETER2     )->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COM_Emerald      )->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COM_BF609      )->EnableWindow(TRUE);
    	GetDlgItem(IDC_EDIT_IO_IN_CONNECT    )->EnableWindow(TRUE);
        //psj can line test
    	GetDlgItem(IDC_EDIT_IO_OUT_BARCODE_1  )->EnableWindow(TRUE);
    	GetDlgItem(IDC_EDIT_IO_OUT_BARCODE_2  )->EnableWindow(TRUE);
    	GetDlgItem(IDC_EDIT_IO_OUT_DISCONECT )->EnableWindow(TRUE);
    	GetDlgItem(IDC_EDIT_IO_OUT_LED_G     )->EnableWindow(TRUE);
    	GetDlgItem(IDC_EDIT_IO_OUT_LED_R     )->EnableWindow(TRUE);
    	GetDlgItem(IDC_EDIT_IO_OUT_LED_Y     )->EnableWindow(TRUE);
    	GetDlgItem(IDC_EDIT_IO_OUT_POWER     )->EnableWindow(TRUE);
    	GetDlgItem(IDC_COMBO_CAR_TYPE        )->EnableWindow(TRUE);
    	GetDlgItem(IDC_COMBO_LOCATION        )->EnableWindow(TRUE);
    	GetDlgItem(IDC_EDIT_ECU_ID           )->EnableWindow(TRUE);
    	GetDlgItem(IDC_EDIT_BF_BOOT_VER     )->EnableWindow(TRUE);
    	GetDlgItem(IDC_EDIT_ECU_VER_HW       )->EnableWindow(TRUE);
    	GetDlgItem(IDC_EDIT_ECU_VER_SW       )->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_BF_VER_ARG       )->EnableWindow(TRUE);
    	GetDlgItem(IDC_EDIT_FILENAME_LOG_1     )->EnableWindow(TRUE);
    }
}

LRESULT CDialogSet::OnThreadMsgEnd(WPARAM a_wParam, LPARAM a_lParam)
{
    OnThreadMsgUpdate(0,0);

    tSetupThreadStop = false;
    m_pThreadTestSetup = NULL;

    TestRun(false);

	return 0;
}

LRESULT CDialogSet::OnThreadMsgUpdate(WPARAM a_wParam, LPARAM a_lParam)
{
    UpdateData(FALSE);
	return 0;
}

void CDialogSet::OnDestroy()
{
    UpdateData(TRUE);

	CDialog::OnDestroy();

	// TODO: Add your message handler code here
    if (m_pThreadTestSetup != NULL)
    {
        tSetupThreadStop = true;
        WaitForSingleObject(m_pThreadTestSetup->m_hThread, 1000);
        tSetupThreadStop = NULL;
    }
}

void CDialogSet::OnCheckIoOutBarcode1()
{
    UpdateData(TRUE);
}

void CDialogSet::OnCheckIoOutBarcode2()
{
    UpdateData(TRUE);
}

void CDialogSet::OnCheckIoOutDisconnect()
{
    UpdateData(TRUE);
}

void CDialogSet::OnCheckIoOutLdwsBtn()
{
    UpdateData(TRUE);
}

void CDialogSet::OnCheckIoOutLedG()
{
    UpdateData(TRUE);
}

void CDialogSet::OnCheckIoOutLedR()
{
    UpdateData(TRUE);
}

void CDialogSet::OnCheckIoOutLedY()
{
    UpdateData(TRUE);
}

void CDialogSet::OnCheckIoOutPower()
{
    UpdateData(TRUE);
}

void CDialogSet::OnSelchangeComboCarType()
{
    int     nLoc;
    CString strLoc;

    strLoc = m_COMBO_LOCATION;

    UpdateData(TRUE);

    ((CComboBox*)GetDlgItem(IDC_COMBO_LOCATION))->ResetContent();

    if ((m_COMBO_CAR_TYPE == "29TON"))
    {
        ((CComboBox*)GetDlgItem(IDC_COMBO_LOCATION))->AddString("통합");

        nLoc = ((CComboBox*)GetDlgItem(IDC_COMBO_LOCATION))->FindString(0, strLoc);
        if (nLoc < 0)
        {
            ((CComboBox*)GetDlgItem(IDC_COMBO_LOCATION))->SetCurSel(0);
            m_COMBO_LOCATION = "통합";
        }
        else
        {
            ((CComboBox*)GetDlgItem(IDC_COMBO_LOCATION))->SetCurSel(nLoc);
            m_COMBO_LOCATION = strLoc;
        }
    }
	else
    {
        ((CComboBox*)GetDlgItem(IDC_COMBO_LOCATION))->AddString("통합");

        nLoc = ((CComboBox*)GetDlgItem(IDC_COMBO_LOCATION))->FindString(0, strLoc);
        if (nLoc < 0)
        {
            ((CComboBox*)GetDlgItem(IDC_COMBO_LOCATION))->SetCurSel(0);
            m_COMBO_LOCATION = "통합";
        }
        else
        {
            ((CComboBox*)GetDlgItem(IDC_COMBO_LOCATION))->SetCurSel(nLoc);
            m_COMBO_LOCATION = strLoc;
        }
    }



    UpdateData(TRUE);
}


