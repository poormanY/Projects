// IoBoardDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "R7EOLvs2010.h"
#include "IoBoardDlg.h"
#include "afxdialogex.h"
#include "y_Color.h"
#include "y_Debugging.h"
#include "y_wm_user.h"

#include "y_LogManage.h"
#include "SettingDlg.h"

// IoBoardDlg 대화 상자입니다.
static CIoBoardMgt *m_IoBoard = NULL;		// IO Board 클래스


IMPLEMENT_DYNAMIC(IoBoardDlg, CDialogEx)

IoBoardDlg::IoBoardDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IoBoardDlg::IDD, pParent)
{

}

IoBoardDlg::~IoBoardDlg()
{
}

void IoBoardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OUT0, m_cbutton_out0);
	DDX_Control(pDX, IDC_OUT1, m_cbutton_out1);
	DDX_Control(pDX, IDC_OUT2, m_cbutton_out2);
	DDX_Control(pDX, IDC_OUT3, m_cbutton_out3);
	DDX_Control(pDX, IDC_OUT4, m_cbutton_out4);
	DDX_Control(pDX, IDC_OUT5, m_cbutton_out5);
	DDX_Control(pDX, IDC_OUT6, m_cbutton_out6);
	DDX_Control(pDX, IDC_OUT7, m_cbutton_out7);
	DDX_Control(pDX, IDC_OUT8, m_cbutton_out8);
	DDX_Control(pDX, IDC_OUT9, m_cbutton_out9);
	DDX_Control(pDX, IDC_OUT10,m_cbutton_out10);
	DDX_Control(pDX, IDC_OUT11,m_cbutton_out11);
	DDX_Control(pDX, IDC_OUT12,m_cbutton_out12);
	DDX_Control(pDX, IDC_OUT13,m_cbutton_out13);
	DDX_Control(pDX, IDC_OUT14,m_cbutton_out14);
	DDX_Control(pDX, IDC_OUT15,m_cbutton_out15);

	DDX_Control(pDX, IDC_IN0, m_cbutton_in0);
	DDX_Control(pDX, IDC_IN1, m_cbutton_in1);
	DDX_Control(pDX, IDC_IN2, m_cbutton_in2);
	DDX_Control(pDX, IDC_IN3, m_cbutton_in3);
	DDX_Control(pDX, IDC_IN4, m_cbutton_in4);
	DDX_Control(pDX, IDC_IN5, m_cbutton_in5);
	DDX_Control(pDX, IDC_IN6, m_cbutton_in6);
	DDX_Control(pDX, IDC_IN7, m_cbutton_in7);
	DDX_Control(pDX, IDC_IN8, m_cbutton_in8);
	DDX_Control(pDX, IDC_IN9, m_cbutton_in9);
	DDX_Control(pDX, IDC_IN10,m_cbutton_in10);
	DDX_Control(pDX, IDC_IN11,m_cbutton_in11);
	DDX_Control(pDX, IDC_IN12,m_cbutton_in12);
	DDX_Control(pDX, IDC_IN13,m_cbutton_in13);
	DDX_Control(pDX, IDC_IN14,m_cbutton_in14);
	DDX_Control(pDX, IDC_IN15,m_cbutton_in15);

	DDX_Control(pDX, IDC_BUTTON_IO_CONNECT, m_cbutton_io_connect);
	DDX_Control(pDX, IDOK, m_cbtmbtn_io_close);
	DDX_Control(pDX, IDC_EDIT_IO_PORT, m_edit_io_port);
	DDX_Control(pDX, IDC_EDIT_IO_BD_NAME, m_edit_io_bd_name);
}


BEGIN_MESSAGE_MAP(IoBoardDlg, CDialogEx)
	ON_WM_PAINT()
	ON_MESSAGE(_WM_IO_THREAD_UPDATE_IN, (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM)) OnIoThreadMsgUpdate_IN)
	ON_MESSAGE(_WM_IO_THREAD_UPDATE_OUT, (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM)) OnIoThreadMsgUpdate_OUT)
	ON_MESSAGE(_WM_IO_THREAD_UPDATE_END, (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM)) OnIoThreadMsgUpdate_END)

	ON_BN_CLICKED(IDC_OUT0, &IoBoardDlg::OnBnClickedOut0)
	ON_BN_CLICKED(IDC_OUT1, &IoBoardDlg::OnBnClickedOut1)
	ON_BN_CLICKED(IDC_OUT2, &IoBoardDlg::OnBnClickedOut2)
	ON_BN_CLICKED(IDC_OUT3, &IoBoardDlg::OnBnClickedOut3)
	ON_BN_CLICKED(IDC_OUT4, &IoBoardDlg::OnBnClickedOut4)
	ON_BN_CLICKED(IDC_OUT5, &IoBoardDlg::OnBnClickedOut5)
	ON_BN_CLICKED(IDC_OUT6, &IoBoardDlg::OnBnClickedOut6)
	ON_BN_CLICKED(IDC_OUT7, &IoBoardDlg::OnBnClickedOut7)
	ON_BN_CLICKED(IDC_OUT8, &IoBoardDlg::OnBnClickedOut8)
	ON_BN_CLICKED(IDC_OUT9, &IoBoardDlg::OnBnClickedOut9)
	ON_BN_CLICKED(IDC_OUT10, &IoBoardDlg::OnBnClickedOut10)
	ON_BN_CLICKED(IDC_OUT11, &IoBoardDlg::OnBnClickedOut11)
	ON_BN_CLICKED(IDC_OUT12, &IoBoardDlg::OnBnClickedOut12)
	ON_BN_CLICKED(IDC_OUT13, &IoBoardDlg::OnBnClickedOut13)
	ON_BN_CLICKED(IDC_OUT14, &IoBoardDlg::OnBnClickedOut14)
	ON_BN_CLICKED(IDC_OUT15, &IoBoardDlg::OnBnClickedOut15)

	ON_BN_CLICKED(IDC_IN0, &IoBoardDlg::OnBnClickedIn0)
	ON_BN_CLICKED(IDC_IN1, &IoBoardDlg::OnBnClickedIn1)
	ON_BN_CLICKED(IDC_IN2, &IoBoardDlg::OnBnClickedIn2)
	ON_BN_CLICKED(IDC_IN3, &IoBoardDlg::OnBnClickedIn3)
	ON_BN_CLICKED(IDC_IN4, &IoBoardDlg::OnBnClickedIn4)
	ON_BN_CLICKED(IDC_IN5, &IoBoardDlg::OnBnClickedIn5)
	ON_BN_CLICKED(IDC_IN6, &IoBoardDlg::OnBnClickedIn6)
	ON_BN_CLICKED(IDC_IN7, &IoBoardDlg::OnBnClickedIn7)
	ON_BN_CLICKED(IDC_IN8, &IoBoardDlg::OnBnClickedIn8)
	ON_BN_CLICKED(IDC_IN9, &IoBoardDlg::OnBnClickedIn9)
	ON_BN_CLICKED(IDC_IN10, &IoBoardDlg::OnBnClickedIn10)
	ON_BN_CLICKED(IDC_IN11, &IoBoardDlg::OnBnClickedIn11)
	ON_BN_CLICKED(IDC_IN12, &IoBoardDlg::OnBnClickedIn12)
	ON_BN_CLICKED(IDC_IN13, &IoBoardDlg::OnBnClickedIn13)
	ON_BN_CLICKED(IDC_IN14, &IoBoardDlg::OnBnClickedIn14)
	ON_BN_CLICKED(IDC_IN15, &IoBoardDlg::OnBnClickedIn15)

	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_IO_CONNECT, &IoBoardDlg::OnBnClickedButtonIoConnect)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// IoBoardDlg 메시지 처리기입니다.


BOOL IoBoardDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	
	// 변수 초기화
	m_nBtnConnect = 0; // 연결 상태 (1:Connect, 0:Not Connect)
	uInPort  = 0xFFFF;	// IO Inport Data
	uOutPort = 0x0000;	// IO Outport Data
	m_bThreadStatus_Run = 0;

	// 타이틀 글자
	// 글자 폰트 설정
	CFont titleFont;
	titleFont.CreateFont( 14, // nHeight 
						   0, // nWidth 
						   0,  // nEscapement 
						   0,  // nOrientation 
						   1,  // nWeight 
						   0,  // bItalic 
						   0,  // bUnderline 
						   0,  // cStrikeOut 
						   0,  // nCharSet 
						   OUT_DEFAULT_PRECIS,          // nOutPrecision 
						   0,                           // nClipPrecision 
						   DEFAULT_QUALITY,             // nQuality
						   DEFAULT_PITCH | FF_DONTCARE,  // nPitchAndFamily 
						   _T("굴림") ); // lpszFacename 
	GetDlgItem( IDC_STATIC_IO_BD_TITLE )->SetFont( &titleFont );
	titleFont.Detach();

	// 버튼 스킨 입히기
	// LoadBitmap(가만히 있을때(U), 눌렸을 때(D), 포커스가 갔을때(F), 비활성화(X));
	// (ex.  m_btn.LoadBitmaps(IDB_BTN_U, IDB_BTN_D, IDB_BTN_F, IDB_BTN_X) )
	m_cbtmbtn_io_close.LoadBitmaps(IDB_BITMAP_CLOSE_F, IDB_BITMAP_CLOSE, IDB_BITMAP_CLOSE_F, IDB_BITMAP_CLOSE_X);
	m_cbtmbtn_io_close.SizeToContent();	

	m_cbutton_io_connect.LoadBitmaps(IDB_BITMAP_START_F, IDB_BITMAP_START, IDB_BITMAP_START_F, IDB_BITMAP_START_X);
	m_cbutton_io_connect.SizeToContent();

	// 에디트 박스 설정 값 초기화
	m_edit_io_port.SetTextString(_T("Not Connected"));
	m_edit_io_port.SetBgColor(BLACK);
	m_edit_io_port.SetTextColor(WHITE);

	m_edit_io_bd_name.SetTextString(_T(""));
	m_edit_io_bd_name.SetTextFormat(DT_CENTER | DT_VCENTER);	// 좌우/상하 정렬
	m_edit_io_bd_name.SetBgColor(BLACK);
	m_edit_io_bd_name.SetTextColor(WHITE);

	// 버튼 설정 값 초기화
	m_cbutton_out0.SetTextString(_T("OUT 0"));	// 내용
	CButtonClickedOn(&m_cbutton_out0, 0);		// chk = 1, 버튼 눌림
	m_cbutton_out1.SetTextString(_T("OUT 1"));
	CButtonClickedOn(&m_cbutton_out1, 0);
	m_cbutton_out2.SetTextString(_T("OUT 2"));
	CButtonClickedOn(&m_cbutton_out2, 0);
	m_cbutton_out3.SetTextString(_T("OUT 3"));
	CButtonClickedOn(&m_cbutton_out3, 0);
	m_cbutton_out4.SetTextString(_T("OUT 4"));
	CButtonClickedOn(&m_cbutton_out4, 0);
	m_cbutton_out5.SetTextString(_T("OUT 5"));
	CButtonClickedOn(&m_cbutton_out5, 0);
	m_cbutton_out6.SetTextString(_T("OUT 6"));
	CButtonClickedOn(&m_cbutton_out6, 0);
	m_cbutton_out7.SetTextString(_T("OUT 7"));
	CButtonClickedOn(&m_cbutton_out7, 0);
	m_cbutton_out8.SetTextString(_T("OUT 8"));
	CButtonClickedOn(&m_cbutton_out8, 0);
	m_cbutton_out9.SetTextString(_T("OUT 9"));
	CButtonClickedOn(&m_cbutton_out9, 0);
	m_cbutton_out10.SetTextString(_T("OUT 10"));
	CButtonClickedOn(&m_cbutton_out10, 0);
	m_cbutton_out11.SetTextString(_T("OUT 11"));
	CButtonClickedOn(&m_cbutton_out11, 0);
	m_cbutton_out12.SetTextString(_T("OUT 12"));
	CButtonClickedOn(&m_cbutton_out12, 0);
	m_cbutton_out13.SetTextString(_T("OUT 13"));
	CButtonClickedOn(&m_cbutton_out13, 0);
	m_cbutton_out14.SetTextString(_T("OUT 14"));
	CButtonClickedOn(&m_cbutton_out14, 0);
	m_cbutton_out15.SetTextString(_T("OUT 15"));
	CButtonClickedOn(&m_cbutton_out15, 0);

	m_cbutton_in0.SetTextString(_T("IN 0"));	// 내용
	CButtonClickedOn(&m_cbutton_in0, 0);		// chk = 1, 버튼 눌림
	m_cbutton_in1.SetTextString(_T("IN 1"));
	CButtonClickedOn(&m_cbutton_in1, 0);
	m_cbutton_in2.SetTextString(_T("IN 2"));
	CButtonClickedOn(&m_cbutton_in2, 0);
	m_cbutton_in3.SetTextString(_T("IN 3"));
	CButtonClickedOn(&m_cbutton_in3, 0);
	m_cbutton_in4.SetTextString(_T("IN 4"));
	CButtonClickedOn(&m_cbutton_in4, 0);
	m_cbutton_in5.SetTextString(_T("IN 5"));
	CButtonClickedOn(&m_cbutton_in5, 0);
	m_cbutton_in6.SetTextString(_T("IN 6"));
	CButtonClickedOn(&m_cbutton_in6, 0);
	m_cbutton_in7.SetTextString(_T("IN 7"));
	CButtonClickedOn(&m_cbutton_in7, 0);
	m_cbutton_in8.SetTextString(_T("IN 8"));
	CButtonClickedOn(&m_cbutton_in8, 0);
	m_cbutton_in9.SetTextString(_T("IN 9"));
	CButtonClickedOn(&m_cbutton_in9, 0);
	m_cbutton_in10.SetTextString(_T("IN 10"));
	CButtonClickedOn(&m_cbutton_in10, 0);
	m_cbutton_in11.SetTextString(_T("IN 11"));
	CButtonClickedOn(&m_cbutton_in11, 0);
	m_cbutton_in12.SetTextString(_T("IN 12"));
	CButtonClickedOn(&m_cbutton_in12, 0);
	m_cbutton_in13.SetTextString(_T("IN 13"));
	CButtonClickedOn(&m_cbutton_in13, 0);
	m_cbutton_in14.SetTextString(_T("IN 14"));
	CButtonClickedOn(&m_cbutton_in14, 0);
	m_cbutton_in15.SetTextString(_T("IN 15"));
	CButtonClickedOn(&m_cbutton_in15, 0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void IoBoardDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

}

void IoBoardDlg::CButtonClickedOn(CEditBox *Btn, int On)
{
	if ( On == 1 )
	{
		Btn->SetTextColor(BLACK);
		Btn->SetBgColor(GREEN);
	}
	else
	{
		Btn->SetTextColor(WHITE);
		Btn->SetBgColor(RED);
	}
	Btn->SetTextFormat(DT_CENTER | DT_VCENTER);	// 좌우/상하 정렬
	//Btn->PostMessageW(_WM_THREAD_UPDATE);
}



void IoBoardDlg::OnBnClickedOut0()
{
BOOL chk = IsDlgButtonChecked(IDC_OUT0); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_out0, chk);
m_cbutton_out0.PostMessageW(_WM_THREAD_UPDATE);
}
void IoBoardDlg::OnBnClickedOut1()
{
BOOL chk = IsDlgButtonChecked(IDC_OUT1); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_out1, chk);
m_cbutton_out1.PostMessageW(_WM_THREAD_UPDATE);
}
void IoBoardDlg::OnBnClickedOut2()
{
BOOL chk = IsDlgButtonChecked(IDC_OUT2); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_out2, chk);
m_cbutton_out2.PostMessageW(_WM_THREAD_UPDATE);
}
void IoBoardDlg::OnBnClickedOut3()
{
BOOL chk = IsDlgButtonChecked(IDC_OUT3); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_out3, chk);
m_cbutton_out3.PostMessageW(_WM_THREAD_UPDATE);
}
void IoBoardDlg::OnBnClickedOut4()
{
BOOL chk = IsDlgButtonChecked(IDC_OUT4); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_out4, chk);
m_cbutton_out4.PostMessageW(_WM_THREAD_UPDATE);
}
void IoBoardDlg::OnBnClickedOut5()
{
BOOL chk = IsDlgButtonChecked(IDC_OUT5); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_out5, chk);
m_cbutton_out5.PostMessageW(_WM_THREAD_UPDATE);
}
void IoBoardDlg::OnBnClickedOut6()
{
BOOL chk = IsDlgButtonChecked(IDC_OUT6); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_out6, chk);
m_cbutton_out6.PostMessageW(_WM_THREAD_UPDATE);
}
void IoBoardDlg::OnBnClickedOut7()
{
BOOL chk = IsDlgButtonChecked(IDC_OUT7); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_out7, chk);
m_cbutton_out7.PostMessageW(_WM_THREAD_UPDATE);
}
void IoBoardDlg::OnBnClickedOut8()
{
BOOL chk = IsDlgButtonChecked(IDC_OUT8); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_out8, chk);
m_cbutton_out8.PostMessageW(_WM_THREAD_UPDATE);
}
void IoBoardDlg::OnBnClickedOut9()
{
BOOL chk = IsDlgButtonChecked(IDC_OUT9); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_out9, chk);
m_cbutton_out9.PostMessageW(_WM_THREAD_UPDATE);
}
void IoBoardDlg::OnBnClickedOut10()
{
BOOL chk = IsDlgButtonChecked(IDC_OUT10); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_out10, chk);
m_cbutton_out10.PostMessageW(_WM_THREAD_UPDATE);
}
void IoBoardDlg::OnBnClickedOut11()
{
BOOL chk = IsDlgButtonChecked(IDC_OUT11); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_out11, chk);
m_cbutton_out11.PostMessageW(_WM_THREAD_UPDATE);
}
void IoBoardDlg::OnBnClickedOut12()
{
BOOL chk = IsDlgButtonChecked(IDC_OUT12); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_out12, chk);
m_cbutton_out12.PostMessageW(_WM_THREAD_UPDATE);
}
void IoBoardDlg::OnBnClickedOut13()
{
BOOL chk = IsDlgButtonChecked(IDC_OUT13); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_out13, chk);
m_cbutton_out13.PostMessageW(_WM_THREAD_UPDATE);
}
void IoBoardDlg::OnBnClickedOut14()
{
BOOL chk = IsDlgButtonChecked(IDC_OUT14); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_out14, chk);
m_cbutton_out14.PostMessageW(_WM_THREAD_UPDATE);
}
void IoBoardDlg::OnBnClickedOut15()
{
BOOL chk = IsDlgButtonChecked(IDC_OUT15); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_out15, chk);
m_cbutton_out15.PostMessageW(_WM_THREAD_UPDATE);
}

void IoBoardDlg::OnBnClickedIn0()
{
BOOL chk = IsDlgButtonChecked(IDC_IN0); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_in0, chk);
}
void IoBoardDlg::OnBnClickedIn1()
{
BOOL chk = IsDlgButtonChecked(IDC_IN1); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_in1, chk);
}
void IoBoardDlg::OnBnClickedIn2()
{
BOOL chk = IsDlgButtonChecked(IDC_IN2); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_in2, chk);
}
void IoBoardDlg::OnBnClickedIn3()
{
BOOL chk = IsDlgButtonChecked(IDC_IN3); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_in3, chk);
}
void IoBoardDlg::OnBnClickedIn4()
{
BOOL chk = IsDlgButtonChecked(IDC_IN4); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_in4, chk);
}
void IoBoardDlg::OnBnClickedIn5()
{
BOOL chk = IsDlgButtonChecked(IDC_IN5); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_in5, chk);
}
void IoBoardDlg::OnBnClickedIn6()
{
BOOL chk = IsDlgButtonChecked(IDC_IN6); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_in6, chk);
}
void IoBoardDlg::OnBnClickedIn7()
{
BOOL chk = IsDlgButtonChecked(IDC_IN7); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_in7, chk);
}
void IoBoardDlg::OnBnClickedIn8()
{
BOOL chk = IsDlgButtonChecked(IDC_IN8); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_in8, chk);
}
void IoBoardDlg::OnBnClickedIn9()
{
BOOL chk = IsDlgButtonChecked(IDC_IN9); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_in9, chk);
}
void IoBoardDlg::OnBnClickedIn10()
{
BOOL chk = IsDlgButtonChecked(IDC_IN10); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_in10, chk);
}
void IoBoardDlg::OnBnClickedIn11()
{
BOOL chk = IsDlgButtonChecked(IDC_IN11); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_in11, chk);
}
void IoBoardDlg::OnBnClickedIn12()
{
BOOL chk = IsDlgButtonChecked(IDC_IN12); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_in12, chk);
}
void IoBoardDlg::OnBnClickedIn13()
{
BOOL chk = IsDlgButtonChecked(IDC_IN13); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_in13, chk);
}
void IoBoardDlg::OnBnClickedIn14()
{
BOOL chk = IsDlgButtonChecked(IDC_IN14); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_in14, chk);
}
void IoBoardDlg::OnBnClickedIn15()
{
BOOL chk = IsDlgButtonChecked(IDC_IN15); // 체크 되어있는지 알아보기
CButtonClickedOn(&m_cbutton_in15, chk);
}

HBRUSH IoBoardDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	switch(nCtlColor)
    {
        // 읽기 전용 컨트롤
        case CTLCOLOR_STATIC:
        {
			pDC->SetTextColor(GREEN);
			pDC->SetBkColor(BLACK);
        }
        break;
    }

	return hbr;
}

void IoBoardDlg::DrawDoubleBuffering(void)
{
	CDC *pDC = GetDC();		// 실제 DC 얻기
	CRect rect;
	GetClientRect(rect);	// 현재 창 사이즈 얻기

	CDC MemDC;				// 메모리 DC
	CBitmap *pOldBitmap;	// 메모리 DC의 비트맵
	CBitmap bmp;			// 현재 DC 설정의 비트맵

	MemDC.CreateCompatibleDC(pDC);									// 메모리 DC와 현재 DC의 설정 일치
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());	// 비트맵과 현재 DC의 설정 일치 
	pOldBitmap = (CBitmap *)MemDC.SelectObject(&bmp);				// 메모리 DC의 비트맵 선택
	MemDC.PatBlt(0, 0, rect.Width(), rect.Height(), BLACKNESS);		// 패턴 출력
																	// PATCOPY(복사), BLACKNESS(검정), WHITENESS(흰색), DSTINVERT(색상반전)

	// 메모리 DC에 그리기
	m_DrawIOBD->Draw_Line(&MemDC, 0, 70, rect.Width(), 70, RED);
    
	// 메모리 DC를 현재 DC에 복사
	pDC->BitBlt(0, 0, rect.Width(),rect.Height(), &MemDC, 0,0,SRCCOPY);

	// 사용된 메모리 및 비트맵 삭제
	MemDC.SelectObject(pOldBitmap);
    MemDC.DeleteDC();
}

// SerialDlg 메시지 처리기입니다.
BOOL IoBoardDlg::OpenIoBoard()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_IoBoard = new CIoBoardMgt;
	CString strIoBdProduct;

	// 포트 불러오기
	CString strIoBdCom;
	CLogManage	*pEolSetting;
	pEolSetting = new CLogManage;
	pEolSetting->SetFilePath(SETTING_FILE_PATH);
	strIoBdCom = pEolSetting->ReadSetting(COM_IO);
	delete pEolSetting;
    pEolSetting = NULL;

	char *pbIoBdCom = new char[strIoBdCom.GetLength()+1];
	strcpy(pbIoBdCom, CT2A(strIoBdCom));
	pbIoBdCom[strIoBdCom.GetLength()] = '\0';
	
	
    if (m_IoBoard == NULL)
    {
        return false;
    }

	if (m_IoBoard->Open(pbIoBdCom) == false)
    {
        dp("[%s] Open Fail!\n",pbIoBdCom);
        return false;
    }
		

	strIoBdProduct = _T("KM ");
    if (m_IoBoard->TransferMsg("$01GA001\r") == false)
    {
        dp("[%s] IO Board communication error!!\n", pbIoBdCom);
        return false;
    }
	else
	{
		strIoBdProduct += (m_IoBoard->GetMsg()+9);
	}

	strIoBdProduct += _T(" Ver ");
    if (strcmp(m_IoBoard->GetMsg()+9, "6050") != 0)
    {
        dp("[%s] IO Board model %s error!\n",pbIoBdCom, m_IoBoard->GetMsg()+9);
        return false;
    }
	else
	{
		strIoBdProduct += (m_IoBoard->GetMsg()+9);
	}

	// IO Board 모델명, 제품군, ver 표시
	m_edit_io_bd_name.SetTextString(strIoBdProduct);
	m_edit_io_bd_name.PostMessageW(_WM_THREAD_UPDATE);


	
	delete pbIoBdCom;
	pbIoBdCom= NULL;

	return true;
}

BOOL IoBoardDlg::CloseIoBoard()
{
	if (m_IoBoard == NULL)
    {
        return false;
    }

	delete m_IoBoard;
    m_IoBoard = NULL;
	dp("IO Board Close\n");

	return true;
}


void IoBoardDlg::OnBnClickedButtonIoConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if ( m_nBtnConnect == 0 )
	{
		OnBnClickedButtonIoConnect_Start();

	}
	else
	{
		OnBnClickedButtonIoConnect_End();
	}

}

void IoBoardDlg::OnBnClickedButtonIoConnect_Start()
{
	GetDlgItem(IDOK)->EnableWindow(FALSE);	// Close 비활성화
	GetDlgItem(IDC_BUTTON_IO_CONNECT)->EnableWindow(FALSE); // Connect 버튼 비활성화
	m_cbutton_io_connect.LoadBitmaps(IDB_BITMAP_STOP_F, IDB_BITMAP_STOP, IDB_BITMAP_STOP_F, IDB_BITMAP_STOP_X);
	m_cbutton_io_connect.SizeToContent();

	m_hThread_IoBoard = NULL;
	// 포트 콤보박스 초기화
	if ( OpenIoBoard() == TRUE )
	{
		dp("IO Board Open Success!\n");

		m_nBtnConnect = 1;

		// IO Board 연결 상태
		m_edit_io_port.SetTextString(_T("Connected"));
		m_edit_io_port.PostMessageW(_WM_THREAD_UPDATE);

		//--> 포트 감시 쓰레드 생성
		DWORD dwThreadID;
		m_hThread_IoBoard = NULL;
		m_hThread_IoBoard = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)Thread_IoBoard_Running, this, 0, &dwThreadID);
	}
	else
	{
		dp("IO Board Open Fail!\n");

		OnBnClickedButtonIoConnect_End();
	}

	UpdateData(FALSE);

	GetDlgItem(IDC_BUTTON_IO_CONNECT)->EnableWindow(TRUE); // Connect 버튼 활성화
}

void IoBoardDlg::OnBnClickedButtonIoConnect_End()
{
	m_nBtnConnect = 0;

	GetDlgItem(IDC_BUTTON_IO_CONNECT)->EnableWindow(FALSE); // Connect 버튼 비활성화
	m_cbutton_io_connect.LoadBitmaps(IDB_BITMAP_START_F, IDB_BITMAP_START, IDB_BITMAP_START_F, IDB_BITMAP_START_X);
	m_cbutton_io_connect.SizeToContent();

	// IO Board 모델명, 제품군, ver 표시
	m_edit_io_bd_name.SetTextString(_T(""));
	m_edit_io_bd_name.PostMessageW(_WM_THREAD_UPDATE);
	// IO Board 연결 상태
	m_edit_io_port.SetTextString(_T("Not Connected"));
	m_edit_io_port.PostMessageW(_WM_THREAD_UPDATE);

	UpdateData(FALSE);

	// Thread 가 끝날 때까지 기다린다.
	while ( m_bThreadStatus_Run == 1 )
	{
		dp("m_bThreadStatus_Run : %d\n", m_bThreadStatus_Run);
		Sleep(200);
	}

	CloseIoBoard();
	GetDlgItem(IDOK)->EnableWindow(TRUE);	// Close 활성화
	GetDlgItem(IDC_BUTTON_IO_CONNECT)->EnableWindow(TRUE); // Connect 버튼 활성화
}



UINT IoBoardDlg::Thread_IoBoard_Running(IoBoardDlg* pDlg)
{
	U16 uOldInPort = 0xFFFF;
	U16 uOldOutPort = 0x0000;

	dp("- IO Board Thread Open -\n");

	pDlg->m_bThreadStatus_Run = 1;
	////////////////////////////////////////////
	// 입력 포트
	pDlg->uInPort = m_IoBoard->GetInPort();
	uOldInPort = pDlg->uInPort;
	pDlg->PostMessageW(_WM_IO_THREAD_UPDATE_IN);
	dp("uInPort <--- %4X\n", pDlg->uInPort);
	
	////////////////////////////////////////////
	// 출력 포트
	pDlg->PostMessageW(_WM_IO_THREAD_UPDATE_OUT);
	uOldOutPort = pDlg->uOutPort;
	m_IoBoard->PutOutPort(pDlg->uOutPort);
	dp("uOutPort ---> %4X\n", pDlg->uOutPort);
	
	while(pDlg->m_nBtnConnect == 1)
	{
		////////////////////////////////////////////
		// 입력 포트
		pDlg->uInPort = m_IoBoard->GetInPort();
		dp("uInPort : %4X\n", pDlg->uInPort);
		// 입력 포트에 변화가 있을 경우에만, 다이얼로그를 갱신
		if ( uOldInPort != pDlg->uInPort )
		{
			uOldInPort = pDlg->uInPort;
			pDlg->PostMessageW(_WM_IO_THREAD_UPDATE_IN);
			dp("uInPort <--- %4X\n", pDlg->uInPort);
		}
		
		////////////////////////////////////////////
		// 출력 포트
		pDlg->PostMessageW(_WM_IO_THREAD_UPDATE_OUT);
		dp("uOutPort : %4X\n", pDlg->uOutPort);
		// 출력 포트에 변화가 있을 경우에만, IO Board에 데이터 송신
		if ( uOldOutPort != pDlg->uOutPort )
		{
			uOldOutPort = pDlg->uOutPort;
			m_IoBoard->PutOutPort(pDlg->uOutPort);
			dp("uOutPort ---> %4X\n", pDlg->uOutPort);
		}

		Sleep(200);
	}
	pDlg->m_bThreadStatus_Run = 0;
	pDlg->PostMessageW(_WM_IO_THREAD_UPDATE_END);
	dp("- IO Board Thread Close -\n");

	return true;
}

LRESULT IoBoardDlg::OnIoThreadMsgUpdate_IN(WPARAM a_wParam, LPARAM a_lParam)
{
	int nInputBit[16];
	int CHK_BIT = 0x0001;
	int nIdxBit;

	UpdateData(TRUE);

	for ( nIdxBit = 0; nIdxBit < 16; nIdxBit++ )
	{
		nInputBit[nIdxBit] = (uInPort & CHK_BIT)? 1 : 0;
		CHK_BIT <<= 1;
	}
	CButtonClickedOn( &m_cbutton_in0, nInputBit[0] );
	m_cbutton_in0.PostMessageW(_WM_THREAD_UPDATE);
	CButtonClickedOn( &m_cbutton_in1, nInputBit[1] );
	m_cbutton_in1.PostMessageW(_WM_THREAD_UPDATE);
	CButtonClickedOn( &m_cbutton_in2, nInputBit[2] );
	m_cbutton_in2.PostMessageW(_WM_THREAD_UPDATE);
	CButtonClickedOn( &m_cbutton_in3, nInputBit[3] );
	m_cbutton_in3.PostMessageW(_WM_THREAD_UPDATE);
	CButtonClickedOn( &m_cbutton_in4, nInputBit[4] );
	m_cbutton_in4.PostMessageW(_WM_THREAD_UPDATE);
	CButtonClickedOn( &m_cbutton_in5, nInputBit[5] );
	m_cbutton_in5.PostMessageW(_WM_THREAD_UPDATE);
	CButtonClickedOn( &m_cbutton_in6, nInputBit[6] );
	m_cbutton_in6.PostMessageW(_WM_THREAD_UPDATE);
	CButtonClickedOn( &m_cbutton_in7, nInputBit[7] );
	m_cbutton_in7.PostMessageW(_WM_THREAD_UPDATE);
	CButtonClickedOn( &m_cbutton_in8, nInputBit[8] );
	m_cbutton_in8.PostMessageW(_WM_THREAD_UPDATE);
	CButtonClickedOn( &m_cbutton_in9, nInputBit[9] );
	m_cbutton_in9.PostMessageW(_WM_THREAD_UPDATE);
	CButtonClickedOn( &m_cbutton_in10, nInputBit[10] );
	m_cbutton_in10.PostMessageW(_WM_THREAD_UPDATE);
	CButtonClickedOn( &m_cbutton_in11, nInputBit[11] );
	m_cbutton_in11.PostMessageW(_WM_THREAD_UPDATE);
	CButtonClickedOn( &m_cbutton_in12, nInputBit[12] );
	m_cbutton_in12.PostMessageW(_WM_THREAD_UPDATE);
	CButtonClickedOn( &m_cbutton_in13, nInputBit[13] );
	m_cbutton_in13.PostMessageW(_WM_THREAD_UPDATE);
	CButtonClickedOn( &m_cbutton_in14, nInputBit[14] );
	m_cbutton_in14.PostMessageW(_WM_THREAD_UPDATE);
	CButtonClickedOn( &m_cbutton_in15, nInputBit[15] );
	m_cbutton_in15.PostMessageW(_WM_THREAD_UPDATE);

    UpdateData(FALSE);

	return 0;
}

LRESULT IoBoardDlg::OnIoThreadMsgUpdate_OUT(WPARAM a_wParam, LPARAM a_lParam)
{
	unsigned short int uOut = 0x0000;
	unsigned short int OUT_BIT = 0x0001;

	UpdateData(TRUE);

	// 체크 되어있는지 알아보기
	if(IsDlgButtonChecked(IDC_OUT0))	uOut |= OUT_BIT;	OUT_BIT<<=1;
	if(IsDlgButtonChecked(IDC_OUT1))	uOut |= OUT_BIT;	OUT_BIT<<=1;
	if(IsDlgButtonChecked(IDC_OUT2))	uOut |= OUT_BIT;	OUT_BIT<<=1;
	if(IsDlgButtonChecked(IDC_OUT3))	uOut |= OUT_BIT;	OUT_BIT<<=1;
	if(IsDlgButtonChecked(IDC_OUT4))	uOut |= OUT_BIT;	OUT_BIT<<=1;
	if(IsDlgButtonChecked(IDC_OUT5))	uOut |= OUT_BIT;	OUT_BIT<<=1;
	if(IsDlgButtonChecked(IDC_OUT6))	uOut |= OUT_BIT;	OUT_BIT<<=1;
	if(IsDlgButtonChecked(IDC_OUT7))	uOut |= OUT_BIT;	OUT_BIT<<=1;
	if(IsDlgButtonChecked(IDC_OUT8))	uOut |= OUT_BIT;	OUT_BIT<<=1;
	if(IsDlgButtonChecked(IDC_OUT9))	uOut |= OUT_BIT;	OUT_BIT<<=1;
	if(IsDlgButtonChecked(IDC_OUT10))	uOut |= OUT_BIT;	OUT_BIT<<=1;
	if(IsDlgButtonChecked(IDC_OUT11))	uOut |= OUT_BIT;	OUT_BIT<<=1;
	if(IsDlgButtonChecked(IDC_OUT12))	uOut |= OUT_BIT;	OUT_BIT<<=1;
	if(IsDlgButtonChecked(IDC_OUT13))	uOut |= OUT_BIT;	OUT_BIT<<=1;
	if(IsDlgButtonChecked(IDC_OUT14))	uOut |= OUT_BIT;	OUT_BIT<<=1;
	if(IsDlgButtonChecked(IDC_OUT15))	uOut |= OUT_BIT;	OUT_BIT<<=1;

	uOutPort = uOut;
	
	UpdateData(FALSE);

	return 0;
}

LRESULT IoBoardDlg::OnIoThreadMsgUpdate_END(WPARAM a_wParam, LPARAM a_lParam)
{
	UpdateData(TRUE);

	m_bThreadStatus_Run = 0;
	dp("[MSG END] m_bThreadStatus_Run : %d\n",m_bThreadStatus_Run);

	UpdateData(FALSE);

	return 0;
}

BOOL IoBoardDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
    GetClientRect(rect);
    pDC->FillSolidRect( rect, BLACK );
 
    return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}
