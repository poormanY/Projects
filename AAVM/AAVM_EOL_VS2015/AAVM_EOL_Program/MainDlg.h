// MainDlg.h : header file
//

#if !defined(AFX_MAINDLG_H__AA385BC0_DA0F_4AFB_A087_CC7502A688F0__INCLUDED_)
#define AFX_MAINDLG_H__AA385BC0_DA0F_4AFB_A087_CC7502A688F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// #include "PCANBasicClass.h"
#include "ExeFileName.h"
#include "DialogSet.h"
//#include "pcan_inf.h"
#include "TextStatic.h"
#include "afxwin.h"

////////////////////////////////////////////////////////////////////////////
// Create Console Window for printf()
#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console" )
#define dprintf(a) printf a
#else
#define dprintf(a)
#endif

// screw control    생산라인 1호기 : 0, 생산라인 3호기 : 1.
#define _SCREW_SERIAL   1
//#define _SCREW_SERIAL   1






/////////////////////////////////////////////////////////////////////////////
// MainDlg dialog

class MainDlg : public CDialog
{
// Construction
public:
	int n_Zig_05_Pin_Used;
	int n_Zig_04_Pin_Used;
	int n_Zig_03_Pin_Used;
	int n_Zig_02_Pin_Used;
	int n_Zig_01_Pin_Used;
	bool CreateExpressImg(void);
	bool CreateImage(void);
	CWinThread * m_pThreadFrontImage;
	CWinThread * m_pThreadRearImage;
	CWinThread * m_pThreadLeftImage;
	CWinThread * m_pThreadRightImage;
	CWinThread * m_pThreadTestMain;
	MainDlg(CWnd* pParent = NULL);	// standard constructor
    ExeFileName m_ExeFileName;
    CString m_pFileNameSetup;
    CString m_pFileNameSystem;

// Dialog Data
	//{{AFX_DATA(MainDlg)#ifdef _DEBUG
	enum { IDD = IDD_DIALOG_MAIN };
	CSpinButtonCtrl	m_SPIN_BOX_UNIT_CNT;
	CComboBox	m_CtrlComboTesterName;
	CComboBox	m_CtrlComboTesterLine;
	CTextStatic	m_TXT_GUIDE;
    CTextStatic	m_TXT_LABEL;
	CListCtrl	m_LIST_PROCESS;
	int     m_EDIT_TEST_NUM_OK;
	int     m_EDIT_TEST_NUM_ALL;
	CString m_EDIT_TEST_NUM_RATE;
	int		m_EDIT_MONITOR;
	CString	m_EDIT_USER_MSG;
	CString	m_EDIT_TESTER_LINE;
	CString	m_EDIT_TESTER_NAME;
	CString	m_EDIT_TESTER_TIME;
	CString	m_EDIT_CAR_NAME;
	int		m_nComboTesterLine;
	int		m_nComboTesterName;
	int		m_EDIT_PIN_USED;
	CString	m_COMBO_PROGRAM_TYPE;
	CString m_COMBO_VOLTAGE_TYPE;
	int		m_EDIT_BOX_UNIT_MAX;
	int		m_EDIT_BOX_UNIT_CNT;
	CString	m_STATIC_LINE_NAME;

	int		m_EDIT_MMS_MOVE			;
    int		m_EDIT_HP1_ARM			;
    int		m_EDIT_HP3_SWING		;
    int		m_EDIT_HP3_BOOM_UP		;
    int		m_EDIT_HP3_BOOM_DN		;
    int		m_EDIT_ESROS_WIPER		;
	int		m_EDIT_ESROS_INTWIPER	;
	CString m_EDIT_AAVM_VIEW_MODE    ;
    CString m_EDIT_AAVM_WARNING_FRONT;
    CString m_EDIT_AAVM_WARNING_REAR ;
    CString m_EDIT_AAVM_WARNING_LEFT ;
    CString m_EDIT_AAVM_WARNING_RIGHT;
    CString m_EDIT_AAVM_IMOD_STATUS  ;
    CString m_EDIT_AAVM_MODEL_STATUS ;
    CString m_EDIT_AAVM_DTC          ;
    CString m_EDIT_CAN_AAVO          ;
	BOOL    m_CHECK_MODE_NIGHT       ;
    BOOL    m_CHECK_MODE_MOVE        ;
    BOOL    m_CHECK_MODE_SWING       ;
    BOOL    m_CHECK_MODE_WORK        ;
    BOOL    m_CHECK_MODE_WIPER       ;
    BOOL    m_CHECK_MODE_INTWIPER    ;
    BOOL    m_CHECK_MODE_MANUAL_OFF  ;
	CString m_EDIT_CAMERA_STAT_FRONT ;
	CString m_EDIT_CAMERA_STAT_REAR  ;
	CString m_EDIT_CAMERA_STAT_LEFT  ;
	CString m_EDIT_CAMERA_STAT_RIGHT ;
	CString m_EDIT_CAMERA_GAIN_FRONT ;
	CString m_EDIT_CAMERA_GAIN_REAR  ;
	CString m_EDIT_CAMERA_GAIN_LEFT  ;
	CString m_EDIT_CAMERA_GAIN_RIGHT ;
	CString m_EDIT_EMERALD_STAT      ;
	CString m_EDIT_BF609_STAT        ;
	CString m_EDIT_BF609_FPS         ;
      
	int     b_ModelMessage;
	int     m_ModelNumber;

	CString m_EDIT_BARCODE_PCB       ;
	CString m_EDIT_BARCODE_CASE      ;
	CString m_EDIT_BARCODE_CASE2     ;

	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(MainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
    afx_msg LRESULT OnThreadMsgEnd(WPARAM a_wParam, LPARAM a_lParam);
    afx_msg LRESULT OnThreadMsgUpdate(WPARAM a_wParam, LPARAM a_lParam);
	afx_msg void OnBtnSet();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnChangeEditMonitor();
	afx_msg void OnBtnMonitor();
	afx_msg void OnBtnMaualError();
	afx_msg void OnSelchangeComboTesterLine();
	afx_msg void OnSelchangeComboTesterName();
	afx_msg void OnSelchangeComboProgramType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:


	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
	CComboBox m_ctrl_program_type;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

static UINT FrontThread(LPVOID pParam);
static UINT RearThread(LPVOID pParam);
static UINT LeftThread(LPVOID pParam);
static UINT RightThread(LPVOID pParam);

extern CString strFImagePath;
extern CString strReImagePath;
extern CString strLImagePath;
extern CString strRImagePath;

extern bool FDrawStop;
extern bool ReDrawStop;
extern bool LDrawStop;
extern bool RDrawStop;

extern unsigned int ui_FStartNumber;
extern unsigned int ui_FStopNumber;

extern unsigned int ui_ReStartNumber;
extern unsigned int ui_ReStopNumber;

extern unsigned int ui_LStartNumber;
extern unsigned int ui_LStopNumber;

extern unsigned int ui_RStartNumber;
extern unsigned int ui_RStopNumber;


extern bool DrawStop;



#endif // !defined(AFX_MAINDLG_H__AA385BC0_DA0F_4AFB_A087_CC7502A688F0__INCLUDED_)
