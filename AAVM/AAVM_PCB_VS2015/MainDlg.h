
// MainDlg.h : 헤더 파일
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console" )

#ifdef _DEBUG
#define dprintf(a) printf a
#else
#define dprintf(a)
#endif

// MainDlg 대화 상자
class MainDlg : public CDialogEx
{
// 생성입니다.
public:
	MainDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
    afx_msg LRESULT OnWmThreadEnd(WPARAM a_wParam, LPARAM a_lParam);
    afx_msg LRESULT OnWmThreadUpdate(WPARAM a_wParam, LPARAM a_lParam);
public:
    CListCtrl m_LIST_PROCESS;
    int     m_EDIT_MMS_MOVE          ;
    int     m_EDIT_HP1_ARM           ;
    int     m_EDIT_HP3_SWING         ;
    int     m_EDIT_HP3_BOOM_UP       ;
    int     m_EDIT_HP3_BOOM_DN       ;
    int     m_EDIT_ESROS_WIPER       ;
    int     m_EDIT_ESROS_INTWIPER    ;
	int     m_EDIT_TEST_NUM_OK;
	int     m_EDIT_TEST_NUM_ALL;
	CString m_EDIT_TEST_NUM_RATE;
	CString	m_COMBO_PROGRAM_TYPE;
	CString m_EDIT_CAR_NAME;
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
	CString	m_EDIT_Barcode			 ;
    afx_msg void OnEnChangeEditControl();
    afx_msg void OnBnClickedBtnTest();
};
