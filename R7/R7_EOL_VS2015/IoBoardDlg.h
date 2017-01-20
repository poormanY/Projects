#pragma once
#include "afxwin.h"
#include "afxext.h"

#include "y_EditBox.h"
#include "y_Draw.h"
#include "y_Color.h"
#include "y_IoBoardMgt.h"
#include "y_SerialComm.h"

// IoBoardDlg 대화 상자입니다.

class IoBoardDlg : public CDialogEx
{
	DECLARE_DYNAMIC(IoBoardDlg)

public:
	IoBoardDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~IoBoardDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_IO_BOARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	void IoBoardDlg::CButtonClickedOn(CEditBox *Btn, int On);	// 버튼 눌림 상태에 따른 변화 (On = 1, 눌림)
	void IoBoardDlg::DrawDoubleBuffering(void);					// 더블버퍼링 작업 함수
	BOOL IoBoardDlg::OpenIoBoard();								// IO Board Open
	BOOL IoBoardDlg::CloseIoBoard();							// IO Board Close

	CBitmap  m_Bitmap;	// 더블 버퍼링을 위한 비트맵

	HANDLE m_hThread_IoBoard;								// Thread Handler
	static UINT Thread_IoBoard_Running(IoBoardDlg* pDlg);	// Thread함수
	BOOL m_bThreadStatus_Run;								// Thread함수 실행 상태 (0:정지, 1:실행중)

	CDraw *m_DrawIOBD;			// Draw 클래스
	CSerialComm *m_SerialCom;	// Serial Comm 클래스
	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg LRESULT OnIoThreadMsgUpdate_IN(WPARAM a_wParam, LPARAM a_lParam);
	afx_msg LRESULT OnIoThreadMsgUpdate_OUT(WPARAM a_wParam, LPARAM a_lParam);
	afx_msg LRESULT OnIoThreadMsgUpdate_END(WPARAM a_wParam, LPARAM a_lParam);

	int m_nBtnConnect;			 // Connect 상태
	unsigned short int uInPort;	 // IO Board 입력 포트 값 (bd -> pc)
	unsigned short int uOutPort; // IO Board 출력 포트 값 (pc -> bd)

	unsigned short int GetOutPort(void); // Out Port 출력할 16비트 값 구하기

	CEditBox m_cbutton_out0;
	CEditBox m_cbutton_out1;
	CEditBox m_cbutton_out2;
	CEditBox m_cbutton_out3;
	CEditBox m_cbutton_out4;
	CEditBox m_cbutton_out5;
	CEditBox m_cbutton_out6;
	CEditBox m_cbutton_out7;
	CEditBox m_cbutton_out8;
	CEditBox m_cbutton_out9;
	CEditBox m_cbutton_out10;
	CEditBox m_cbutton_out11;
	CEditBox m_cbutton_out12;
	CEditBox m_cbutton_out13;
	CEditBox m_cbutton_out14;
	CEditBox m_cbutton_out15;

	CEditBox m_cbutton_in0;
	CEditBox m_cbutton_in1;
	CEditBox m_cbutton_in2;
	CEditBox m_cbutton_in3;
	CEditBox m_cbutton_in4;
	CEditBox m_cbutton_in5;
	CEditBox m_cbutton_in6;
	CEditBox m_cbutton_in7;
	CEditBox m_cbutton_in8;
	CEditBox m_cbutton_in9;
	CEditBox m_cbutton_in10;
	CEditBox m_cbutton_in11;
	CEditBox m_cbutton_in12;
	CEditBox m_cbutton_in13;
	CEditBox m_cbutton_in14;
	CEditBox m_cbutton_in15;

	afx_msg void OnBnClickedOut0();
	afx_msg void OnBnClickedOut1();
	afx_msg void OnBnClickedOut2();
	afx_msg void OnBnClickedOut3();
	afx_msg void OnBnClickedOut4();
	afx_msg void OnBnClickedOut5();
	afx_msg void OnBnClickedOut6();
	afx_msg void OnBnClickedOut7();
	afx_msg void OnBnClickedOut8();
	afx_msg void OnBnClickedOut9();
	afx_msg void OnBnClickedOut10();
	afx_msg void OnBnClickedOut11();
	afx_msg void OnBnClickedOut12();
	afx_msg void OnBnClickedOut13();
	afx_msg void OnBnClickedOut14();
	afx_msg void OnBnClickedOut15();

	afx_msg void OnBnClickedIn0();
	afx_msg void OnBnClickedIn1();
	afx_msg void OnBnClickedIn2();
	afx_msg void OnBnClickedIn3();
	afx_msg void OnBnClickedIn4();
	afx_msg void OnBnClickedIn5();
	afx_msg void OnBnClickedIn6();
	afx_msg void OnBnClickedIn7();
	afx_msg void OnBnClickedIn8();
	afx_msg void OnBnClickedIn9();
	afx_msg void OnBnClickedIn10();
	afx_msg void OnBnClickedIn11();
	afx_msg void OnBnClickedIn12();
	afx_msg void OnBnClickedIn13();
	afx_msg void OnBnClickedIn14();
	afx_msg void OnBnClickedIn15();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonIoConnect();
	void OnBnClickedButtonIoConnect_Start();
	void OnBnClickedButtonIoConnect_End();
	CBitmapButton m_cbutton_io_connect;
	CBitmapButton m_cbtmbtn_io_close;
	CEditBox m_edit_io_port;
	CEditBox m_edit_io_bd_name;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
