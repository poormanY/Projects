#pragma once
#include "afxwin.h"
#include "y_EditBox.h"
#include "y_LogManage.h"

#define TESTER_MAX 14

// SettingDlg 대화 상자입니다.
#define SETTING_FILE_PATH	_T("EOL_SETTING.ini")
const CString COM_IO			= _T("COM_IOBOARD");
const CString COM_AM			= _T("COM_AMMETER");
const CString COM_SCREW			= _T("COM_SCREW");
const CString COM_BAR1			= _T("COM_BARCODE1");
const CString COM_BAR2			= _T("COM_BARCODE2");
const CString LOG_FILE_PATH		= _T("LOG_FILE_PATH");
const CString DRAW_VANISHY		= _T("DRAW_VANISHY");
const CString DRAW_WIDTH		= _T("DRAW_LANE_WIDTH");
const CString DRAW_MOVEMENT		= _T("DRAW_LANE_MOVEMNET");
const CString DRAW_LANE_L_COLOR = _T("DRAW_LANE_L_COLOR");
const CString DRAW_LANE_R_COLOR = _T("DRAW_LANE_R_COLOR");
const CString PROC_PRODUCT_TYPE = _T("PROC_PRODUCT_TYPE");
const CString TOTAL_TEST_NUM    = _T("TOTAL_TEST_NUM");
const CString PROC_MCU_VER		= _T("PROC_MCU_VER");
const CString PROC_SW_VER		= _T("PROC_SW_VER");
const CString PROC_SERIAL_NUM	= _T("PROC_SERIAL_NUM");
const CString PROC_LDW_SPD		= _T("PROC_LDW_SPD");
const CString PROC_FCW_SPD		= _T("PROC_FCW_SPD");
const CString TESTER_LINE		= _T("TESTER_LINE");
const CString TESTER_NUM		= _T("TESTER_NUMBER");
const CString TESTER_1			= _T("TESTER_1");
const CString TESTER_2			= _T("TESTER_2");
const CString TESTER_3			= _T("TESTER_3");
const CString TESTER_4			= _T("TESTER_4");
const CString TESTER_5			= _T("TESTER_5");
const CString TESTER_6			= _T("TESTER_6");
const CString TESTER_7			= _T("TESTER_7");
const CString TESTER_8			= _T("TESTER_8");
const CString TESTER_9			= _T("TESTER_9");
const CString TESTER_10			= _T("TESTER_10");
const CString TESTER_11			= _T("TESTER_11");
const CString TESTER_12			= _T("TESTER_12");
const CString TESTER_13			= _T("TESTER_13");
const CString TESTER_14			= _T("TESTER_14");
// 테스트 항목
const CString STEP_CHECK_MCU_VER	= _T("STEP_CHECK_MCU_VER");
const CString STEP_CHECK_SW_VER		= _T("STEP_CHECK_SW_VER");
const CString STEP_CHECK_SERIAL_NUM	= _T("STEP_CHECK_SERIAL_NUM");
const CString STEP_CHECK_IO_BOARD	= _T("STEP_CHECK_IO_BOARD");
const CString STEP_CHECK_SCREW		= _T("STEP_CHECK_SCREW");
const CString STEP_CHECK_BARCODE1	= _T("STEP_CHECK_BARCODE1");
const CString STEP_CHECK_BARCODE2	= _T("STEP_CHECK_BARCODE2");
const CString STEP_CHECK_ACCLERATION = _T("STEP_CHECK_ACCLERATION");
const CString STEP_CHECK_SPEED_PULSE = _T("STEP_CHECK_SPEED_PULSE");
const CString STEP_CHECK_DTC_CLEAR	= _T("STEP_CHECK_DTC_CLEAR");

const CString STEP_LDW_SPEED	= _T("STEP_LDW_SPEED");
const CString STEP_FCW_SPEED	= _T("STEP_FCW_SPEED");
const CString STEP_LDW_DETECT	= _T("STEP_LDW_DETECT");
const CString STEP_LDW_WARN_L	= _T("STEP_LDW_WARN_L");
const CString STEP_LDW_WARN_R	= _T("STEP_LDW_WARN_R");
const CString STEP_FCW_WARN		= _T("STEP_FCW_WARN");
const CString STEP_FCW_WARN2	= _T("STEP_FCW_WARN2");
const CString STEP_FCW_WARN3	= _T("STEP_FCW_WARN3");
const CString STEP_LDW_WARN_L_WINK_L	= _T("STEP_LDW_WARN_L_WINK_L");
const CString STEP_LDW_WARN_L_WINK_R	= _T("STEP_LDW_WARN_L_WINK_R");
const CString STEP_LDW_WARN_L_WINK_LR	= _T("STEP_LDW_WARN_L_WINK_LR");
const CString STEP_LDW_WARN_R_WINK_L	= _T("STEP_LDW_WARN_R_WINK_L");
const CString STEP_LDW_WARN_R_WINK_R	= _T("STEP_LDW_WARN_R_WINK_R");
const CString STEP_LDW_WARN_R_WINK_LR	= _T("STEP_LDW_WARN_R_WINK_LR");
const CString STEP_FCDA_WARN			= _T("STEP_FCDA_WARN");
const CString STEP_ADAS_ALARM_DELAY		= _T("STEP_ALARM_DELAY");
// 언어 선택
const CString SET_LANGUAGE		= _T("LANGUAGE");

class SettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SettingDlg)

public:
	SettingDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~SettingDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	void DrawDoubleBuffering(void);						// 더블퍼버링 그리기

	DECLARE_MESSAGE_MAP()
private:
	CLogManage		EolSetLog;

public:
	virtual BOOL OnInitDialog();
	CEditBox m_edit_set_file_path;
	// Port Setting
	CEditBox m_static_g_comport;
	CEditBox m_edit_com_io;
	CEditBox m_edit_com_am;
	CEditBox m_edit_com_screw;
	CEditBox m_edit_com_bar1;
	CEditBox m_edit_com_bar2;
	// Tester Setting	
	CEditBox m_edit_product_line;
	CEditBox m_static_g_tester;
	CEditBox m_edit_tester_num;
	CEditBox m_edit_tester1;
	CEditBox m_edit_tester2;
	CEditBox m_edit_tester3;
	CEditBox m_edit_tester4;
	CEditBox m_edit_tester5;
	CEditBox m_edit_tester6;
	CEditBox m_edit_tester7;
	CEditBox m_edit_tester8;
	CEditBox m_edit_tester9;
	CEditBox m_edit_tester10;
	CEditBox m_edit_tester11;
	CEditBox m_edit_tester12;
	CEditBox m_edit_tester13;
	CEditBox m_edit_tester14;
	// Draw Setting
	CEditBox m_static_g_draw;
	CEditBox m_edit_draw_vanishy;
	CEditBox m_edit_draw_width;
	CEditBox m_edit_draw_move;
	CEditBox m_edit_lane_l_color;
	CEditBox m_edit_lane_r_color;
	// TEST Process Setting
	CEditBox m_static_g_test_process;
	CEditBox m_edit_proc_product_type;
	CEditBox m_edit_proc_mcu_ver;
	CEditBox m_edit_proc_sw_ver;
	CEditBox m_edit_proc_serial_num;
	CEditBox m_edit_proc_ldw_spd;
	CEditBox m_edit_proc_fcw_spd;
	// TEST ADAS Setting
	CEditBox m_static_g_test_adas;
	// Test Step Setting	
	BOOL m_check_ldw_speed;
	BOOL m_check_fcw_speed;
	BOOL m_check_ldw_detect;
	BOOL m_check_ldw_warn_l;
	BOOL m_check_ldw_warn_r;
	BOOL m_check_fcw_warn;	
	BOOL m_check_fcw_warn2;
	BOOL m_check_fcw_warn3;
	BOOL m_check_ldw_warnL_winkL;
	BOOL m_check_ldw_warnL_winkR;
	BOOL m_check_ldw_warnL_winkLR;
	BOOL m_check_ldw_warnR_winkL;
	BOOL m_check_ldw_warnR_winkR;
	BOOL m_check_ldw_warnR_winkLR;
	BOOL m_check_fcda_warn;
	BOOL m_check_mcu_ver;
	BOOL m_check_sw_ver;
	BOOL m_check_serial_num;
	BOOL m_check_io_board;
	BOOL m_check_screw;
	BOOL m_check_bar1;
	BOOL m_check_bar2;
	BOOL m_check_accleration;
	BOOL m_check_speed_pulse;
	BOOL m_check_dtc_clear;
	
	CEditBox m_edit_adas_alarm_delay;
	
	// Log File Path	
	CStatic m_static_g_log_path;
	CEdit m_edit_set_log_path;

	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnEnChangeEditSetFilePath();
	afx_msg void OnEnChangeEditComIo();
	afx_msg void OnEnChangeEditComAm();
	afx_msg void OnEnChangeEditComScrew();
	afx_msg void OnEnChangeEditComBar1();
	afx_msg void OnEnChangeEditComBar2();
	afx_msg void OnEnChangeEditDrawVanishy();
	afx_msg void OnEnChangeEditDrawWidth();
	afx_msg void OnEnChangeEditDrawMove();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnEnChangeEditProcProductType();
	afx_msg void OnEnChangeEditProcMcuVer();
	afx_msg void OnEnChangeEditProcSwVer();
	afx_msg void OnEnChangeEditProcSerialNum();
	afx_msg void OnEnChangeEditProcLdwSpd();
	afx_msg void OnEnChangeEditProcFcwSpd();
	afx_msg void OnEnChangeEditTesterNum();
	afx_msg void OnEnChangeEditTester1();
	afx_msg void OnEnChangeEditTester2();
	afx_msg void OnEnChangeEditTester3();
	afx_msg void OnEnChangeEditTester4();
	afx_msg void OnEnChangeEditTester5();
	afx_msg void OnEnChangeEditTester6();
	afx_msg void OnEnChangeEditTester7();
	afx_msg void OnEnChangeEditTester8();
	afx_msg void OnEnChangeEditTester9();
	afx_msg void OnEnChangeEditTester10();
	afx_msg void OnEnChangeEditTester11();
	afx_msg void OnEnChangeEditTester12();
	afx_msg void OnEnChangeEditTester13();
	afx_msg void OnEnChangeEditTester14();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditProductLine();
	afx_msg void OnEnChangeEditLaneLColor();
	afx_msg void OnEnChangeEditLaneRColor();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnChangeEditAdasAlarmDelay();
	afx_msg void OnBnClickedRadio(UINT msg);
	int m_radio_language;
};
