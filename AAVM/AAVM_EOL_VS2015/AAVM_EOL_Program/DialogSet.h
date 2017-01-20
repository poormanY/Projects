#if !defined(AFX_DIALOGSET_H__E329AC2F_6634_4C38_A1AF_F7B643738E1C__INCLUDED_)
#define AFX_DIALOGSET_H__E329AC2F_6634_4C38_A1AF_F7B643738E1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogSet.h : header file
//
typedef struct
{
	CString m_EDIT_COM_BARCODE1     ;
	CString m_EDIT_COM_BARCODE2     ;
	CString m_EDIT_COM_IOBOARD      ;
	CString m_EDIT_COM_SCREW        ;
	//psj Ammeter
	CString m_EDIT_COM_AMMETER      ;
	CString	m_EDIT_COM_EMERAD;
	CString	m_EDIT_COM_BF609;
	CString m_EDIT_IO_IN_12V_OVER   ;
	CString m_EDIT_IO_IN_24V_OVER   ;
	CString m_EDIT_IO_IN_12V_UNDER  ;
	CString m_EDIT_IO_IN_24V_UNDER  ;
	CString m_EDIT_IO_IN_CONNECT    ;
	//psj can line test
	CString m_EDIT_IO_IN_CANLINE    ;
	CString m_EDIT_IO_OUT_BARCODE1  ;
	CString m_EDIT_IO_OUT_BARCODE2  ;
	CString m_EDIT_IO_OUT_DISCONNECT;
	CString m_EDIT_IO_OUT_LED_G     ;
	CString m_EDIT_IO_OUT_LED_R     ;
	CString m_EDIT_IO_OUT_LED_Y     ;
	CString m_EDIT_IO_OUT_POWER     ;
	CString m_COMBO_CAR_TYPE        ;
	CString m_COMBO_LOCATION        ;
	CString m_EDIT_ECU_ID           ;
	CString m_EDIT_BF_VER_BOOT      ;
	CString m_EDIT_ECU_VER_HW       ;
	CString m_EDIT_ECU_VER_SW       ;
	CString m_EDIT_BF_VER_ARG       ;
	//psj Ammeter
	CString m_EDIT_IO_IN_AMPERE_OVER_12 ;
	CString m_EDIT_IO_IN_AMPERE_UNDER_12;
	CString m_EDIT_IO_IN_AMPERE_OVER_24 ;
	CString m_EDIT_IO_IN_AMPERE_UNDER_24;
	CString m_EDIT_FILENAME_LOG     ;
} CSetupData;

/////////////////////////////////////////////////////////////////////////////
// CDialogSet dialog

class CDialogSet : public CDialog
{
// Construction
public:
	CWinThread * m_pThreadTestSetup;
	void TestRun(bool a_tOn);
	bool m_tTestOn;
    void CopyToData(CSetupData *a_pTgt);
    void CopyFromData(CSetupData *a_pSrc);
    bool SaveToData(LPCSTR a_pszFileName);
    bool LoadFromData(LPCSTR a_pszFileName);
	CDialogSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogSet)
	enum { IDD = IDD_DIALOG_SET };
	CString	m_COMBO_CAR_TYPE;
	CString	m_COMBO_LOCATION;
	CString	m_EDIT_COM_BARCODE1;
	CString	m_EDIT_COM_BARCODE2;
	CString	m_EDIT_COM_IOBOARD;
	CString	m_EDIT_COM_SCREW;
	CString	m_EDIT_ECU_ID;
	CString	m_EDIT_BF_VER_BOOT;
	CString	m_EDIT_BF_VER_ARG;
	CString	m_EDIT_ECU_VER_HW;
	CString	m_EDIT_ECU_VER_SW;
	CString	m_EDIT_FILENAME_LOG;
	CString	m_EDIT_IO_IN_CONNECT;
	CString	m_EDIT_IO_OUT_BARCODE1;
	CString	m_EDIT_IO_OUT_BARCODE2;
	CString	m_EDIT_IO_OUT_DISCONNECT;
	CString	m_EDIT_IO_OUT_LED_G;
	CString	m_EDIT_IO_OUT_LED_R;
	CString	m_EDIT_IO_OUT_LED_Y;
	CString	m_EDIT_IO_OUT_POWER;
	CString	m_EDIT_TEST_BARCODE1;
	CString	m_EDIT_TEST_BARCODE2;
	CString	m_EDIT_TEST_IOBOARD;
	CString	m_EDIT_TEST_SCREW;
	BOOL	m_CHECK_IO_IN_CONNECT;
	BOOL	m_CHECK_IO_OUT_BARCODE1;
	BOOL	m_CHECK_IO_OUT_BARCODE2;
	BOOL	m_CHECK_IO_OUT_DISCONNECT;
	BOOL	m_CHECK_IO_OUT_LED_G;
	BOOL	m_CHECK_IO_OUT_LED_R;
	BOOL	m_CHECK_IO_OUT_LED_Y;
	BOOL	m_CHECK_IO_OUT_POWER;
	CString	m_COMBO_TIME_DELAY;
	CString	m_EDIT_COM_AMMETER;
	CString	m_EDIT_COM_EMERAD;
	CString	m_EDIT_COM_BF609;
	CString	m_EDIT_IO_IN_AMPERE_OVER_12;
	CString	m_EDIT_IO_IN_AMPERE_UNDER_12;
	CString	m_EDIT_IO_IN_AMPERE_OVER_24;
	CString	m_EDIT_IO_IN_AMPERE_UNDER_24;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogSet)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSetTest();
    afx_msg LRESULT OnThreadMsgEnd(WPARAM a_wParam, LPARAM a_lParam);
    afx_msg LRESULT OnThreadMsgUpdate(WPARAM a_wParam, LPARAM a_lParam);
	afx_msg void OnDestroy();
	afx_msg void OnCheckIoOutBarcode1();
	afx_msg void OnCheckIoOutBarcode2();
	afx_msg void OnCheckIoOutDisconnect();
	afx_msg void OnCheckIoOutLdwsBtn();
	afx_msg void OnCheckIoOutLedG();
	afx_msg void OnCheckIoOutLedR();
	afx_msg void OnCheckIoOutLedY();
	afx_msg void OnCheckIoOutPower();
	afx_msg void OnBtnFilenameLog();
	afx_msg void OnSelchangeComboCarType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSET_H__E329AC2F_6634_4C38_A1AF_F7B643738E1C__INCLUDED_)
