// BarCodeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "R7EOLvs2010.h"
#include "BarCodeDlg.h"
#include "afxdialogex.h"

#include "SettingDlg.h"
#include "y_LogManage.h"
#include "y_BarcodeMgt.h"
#include "y_Debugging.h"
#include "y_Color.h"
#include "y_IoBoardMgt.h"

#define TIMER_BARCODE1 8001
#define TIMER_BARCODE2 8002
static CBarcodeMgt* pBarCode1 = NULL;		// Barcode1 클래스
static CBarcodeMgt* pBarCode2 = NULL;		// Barcode2 클래스
static CIoBoardMgt *pBarIoBoard = NULL;		// IO Board 클래스

static signed short preBarcodeCount1 = 0;
static signed short preBarcodeCount2 = 0;

// BarCodeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(BarCodeDlg, CDialogEx)

BarCodeDlg::BarCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(BarCodeDlg::IDD, pParent)
{

}

BarCodeDlg::~BarCodeDlg()
{
}

void BarCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_BAR1_OPEN, m_btn_bar1_open);
	DDX_Control(pDX, IDC_BUTTON_BAR2_OPEN, m_btn_bar2_open);
	DDX_Control(pDX, IDOK, m_btn_close);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_btn_save);
	DDX_Control(pDX, IDC_BUTTON_LOAD, m_btn_load);
	DDX_Control(pDX, IDC_LIST_BAR1, m_list_bar1);
	DDX_Control(pDX, IDC_LIST_BAR2, m_list_bar2);
	DDX_Control(pDX, IDC_EDIT_BAR1_PORT, m_edit_bar1_port);
	DDX_Control(pDX, IDC_EDIT_BAR2_PORT, m_edit_bar2_port);
	DDX_Control(pDX, IDC_EDIT_BAR1_CONNECT, m_edit_bar1_connect);
	DDX_Control(pDX, IDC_EDIT_BAR2_CONNECT, m_edit_bar2_connect);
	DDX_Control(pDX, IDC_BUTTON_BAR1_CLR, m_btn_bar1_clr);
	DDX_Control(pDX, IDC_BUTTON_BAR2_CLR, m_btn_bar2_clr);
}


BEGIN_MESSAGE_MAP(BarCodeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_BAR1_OPEN, &BarCodeDlg::OnBnClickedButtonBar1Open)
	ON_BN_CLICKED(IDC_BUTTON_BAR2_OPEN, &BarCodeDlg::OnBnClickedButtonBar2Open)
	ON_BN_CLICKED(IDOK, &BarCodeDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_BAR1_CLR, &BarCodeDlg::OnBnClickedButtonBar1Clr)
	ON_BN_CLICKED(IDC_BUTTON_BAR2_CLR, &BarCodeDlg::OnBnClickedButtonBar2Clr)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// BarCodeDlg 메시지 처리기입니다.


BOOL BarCodeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// Port 초기화
	CString strBar1Com;
	CString strBar2Com;
	CLogManage	*pEolSetting;
	pEolSetting = new CLogManage;
	pEolSetting->SetFilePath(SETTING_FILE_PATH);
	strBar1Com = pEolSetting->ReadSetting(COM_BAR1);
	strBar2Com = pEolSetting->ReadSetting(COM_BAR2);
	delete pEolSetting;
    pEolSetting = NULL;
	// Port Number
	m_edit_bar1_port.SetText(BLACK, GREEN, strBar1Com, UPDATE_ON);
	m_edit_bar2_port.SetText(BLACK, GREEN, strBar2Com, UPDATE_ON);
	// Port Connect
	m_edit_bar1_connect.SetText(BLACK, GREEN, _T("Not Connected"), UPDATE_ON);
	m_edit_bar2_connect.SetText(BLACK, GREEN, _T("Not Connected"), UPDATE_ON);

	// Button Skin
	// LoadBitmap(가만히 있을때(U), 눌렸을 때(D), 포커스가 갔을때(F), 비활성화(X));
    m_btn_bar1_open.LoadBitmaps(IDB_BITMAP_BAR1_OPEN, IDB_BITMAP_BLACKBTN, IDB_BITMAP_BAR1_OPEN, IDB_BITMAP_BAR1_OPEN);
    m_btn_bar1_open.SizeToContent(); // 이미지에 맞게 버튼 사이즈를 조정
	m_btn_bar2_open.LoadBitmaps(IDB_BITMAP_BAR2_OPEN, IDB_BITMAP_BLACKBTN, IDB_BITMAP_BAR2_OPEN, IDB_BITMAP_BAR2_OPEN);
    m_btn_bar2_open.SizeToContent();
	m_btn_close.LoadBitmaps(IDB_BITMAP_BAR_CLOSE, IDB_BITMAP_BLACKBTN, IDB_BITMAP_BAR_CLOSE, IDB_BITMAP_BAR_CLOSE);
    m_btn_close.SizeToContent();
	m_btn_save.LoadBitmaps(IDB_BITMAP_BAR_SAVE, IDB_BITMAP_BLACKBTN, IDB_BITMAP_BAR_SAVE, IDB_BITMAP_BAR_SAVE);
    m_btn_save.SizeToContent();
	m_btn_load.LoadBitmaps(IDB_BITMAP_BAR_LOAD, IDB_BITMAP_BLACKBTN, IDB_BITMAP_BAR_LOAD, IDB_BITMAP_BAR_LOAD);
    m_btn_load.SizeToContent();
	m_btn_bar1_clr.LoadBitmaps(IDB_BITMAP_BAR1_CLR, IDB_BITMAP_BLACKBTN, IDB_BITMAP_BAR1_CLR, IDB_BITMAP_BAR1_CLR);
    m_btn_bar1_clr.SizeToContent();
	m_btn_bar2_clr.LoadBitmaps(IDB_BITMAP_BAR2_CLR, IDB_BITMAP_BLACKBTN, IDB_BITMAP_BAR2_CLR, IDB_BITMAP_BAR2_CLR);
    m_btn_bar2_clr.SizeToContent();

	// 리스트 초기화
	m_list_bar1.DeleteAllItems();
	m_list_bar2.DeleteAllItems();
	// 리스트 스타일 설정
	//LVS_EX_FULLROWSELECT  : 아이템을 선택할 때 한 줄 전체를 반전시킨다.
	//LVS_EX_GRIDLINES      : 각 아이템에 경계선을 그려준다.
	//LVS_EX_CHECKBOXES     : 각 아이템에 Check Box를 표시해 준다.
	//LVS_EX_HEADERDRAGDROP : 컬럼 헤더를 드래그 함으로써 컬럼의 순서를 바꿀 수 있게 해준다.
	m_list_bar1.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_list_bar2.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	// 리스트 타이틀 삽입
	m_list_bar1.InsertColumn(0, _T("번호"), LVCFMT_CENTER, 50, -1);
	m_list_bar1.InsertColumn(1, _T("바코드 번호"), LVCFMT_CENTER, 215, -1);
	m_list_bar2.InsertColumn(0, _T("번호"), LVCFMT_CENTER, 50, -1);
	m_list_bar2.InsertColumn(1, _T("바코드 번호"), LVCFMT_CENTER, 215, -1);
	
	// 리스트 컨트롤 배경을 투명하게 한다.
	m_list_bar1.SetBkColor(TRANSPARENT);
	m_list_bar1.SetTextBkColor(TRANSPARENT);
    m_list_bar1.SetTextColor(GREEN);
	
	m_list_bar2.SetBkColor(TRANSPARENT);
	m_list_bar2.SetTextBkColor(TRANSPARENT);
	m_list_bar2.SetTextColor(GREEN);

	// IO Board Open for switching on scanner
	OpenIoBoard();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void BarCodeDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
}

void BarCodeDlg::DrawDoubleBuffering(void)
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
	MemDC.PatBlt(0, 0, rect.Width(), rect.Height(), WHITENESS);		// 패턴 출력
																	// PATCOPY(복사), BLACKNESS(검정), WHITENESS(흰색), DSTINVERT(색상반전)

	///////////////////////////////////////////////
	// 메모리 DC에 비트맵 그리기
	///////////////////////////////////////////////
	CBitmap bmpZebra;
	bmpZebra.LoadBitmap(IDB_BITMAP_ZEBRACODE3);	// 비트맵 로드
	MemDC.SelectObject(&bmpZebra);

	BITMAP bmpZebraSize;						// 비트맵 오브젝트
    bmpZebra.GetObject(sizeof(BITMAP), &bmpZebraSize);	// 비트맵 사이즈 얻기 (bmpZebraSize.bmWidth, bmpZebraSize.bmHeight)

	// StretchBlt (
	//	HDC hdcDest  : 이미지를 출력할 위치의 핸들
	//  nXOriginDest : 이미지를 출력할 x 좌표
	//  nYOriginDest : 이미지를 출력할 y 좌표
	//  nWidthDest   : 출력할 이미지의 너비
	//  nHeightDest  : 출력할 이미지의 높이
	//                (hdcSrc로부터 받은 이미지를 이 너비와 높이에 맞게 이미지 크기를 변경합니다)
	//  hDcSrc       : 이미지의 핸들
	//  nXOriginSrc  : 가져올 이미지의 시작지점인 x 좌표
	//  nOriginSrc   : 가져올 이미지의 시작지점인 y 좌표 
	//                (지정한 위치로부터 nWidthSrc, nHeightSrc만큼 이미지를 잘라옵니다)
	//  nWidthSrc    : 원본 이미지로부터 해당 크기만큼 잘라낼 이미지의 너비
	//  nHeightSrc   : 원본 이미지로부터 해당 크기만큼 잘라낼 이미지의 높이
	//                (이 크기만큼 원본 이미지에서 잘라내어 nWidthDest, nHeightDest의 크기에 맞게 이미지 크기를 변경합니다)
	//  dwRop        : 이미지의 출력 방법
	pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, bmpZebraSize.bmWidth, bmpZebraSize.bmHeight, SRCCOPY);

	// 사용된 메모리 및 비트맵 삭제
	MemDC.SelectObject(pOldBitmap);
    MemDC.DeleteDC();
}


void BarCodeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString strBarCode;
	CString strListPos;
	int nListPos;
	static int nBar1ScanCnt = 0;
	static int nBar2ScanCnt = 0;
	
	if ( nIDEvent == TIMER_BARCODE1 )
	{
		// IO Board -> Scan on
		Bar1ScanON();

		if (pBarCode1->GetCount() > preBarcodeCount1)
		{
			// 읽은 바코드 카운트가 증가했을 때
			preBarcodeCount1 = pBarCode1->GetCount();

			// 새로운 바코드 값
			strBarCode = "";
			strBarCode = (LPCSTR)pBarCode1->GetDataAt(preBarcodeCount1-1);

			// 리스트 아이템 갯수
			nListPos = m_list_bar1.GetItemCount();
			strListPos.Format( _T("%d"), (nListPos+1) );

			// 리스트 번호값
			m_list_bar1.InsertItem(nListPos, strListPos);
			// PCB 바코드 값
			m_list_bar1.SetItem(nListPos, 1, LVIF_TEXT, strBarCode, 0, 0, 0, NULL );

			// ListBox 컨트롤 자동 스크롤
			m_list_bar1.SendMessage(WM_VSCROLL, SB_BOTTOM);

			// IO Board -> Scan off
			// 바코드를 읽은 후에는 OFF -> ON 리셋 필요
			Bar1ScanOFF();
			nBar1ScanCnt = 0;
		}
		else
		{
			nBar1ScanCnt++;	// per100ms

			// 30초간 바코드를 읽지 못하면 OFF -> ON 리셋 필요
			if ( nBar1ScanCnt > 30 )
			{
				Bar1ScanOFF();
				nBar1ScanCnt = 0;
			}
		}
	}
	else if ( nIDEvent == TIMER_BARCODE2 )
	{
		// IO Board -> Scan on
		Bar2ScanON();

		if (pBarCode2->GetCount() > preBarcodeCount2)
		{
			// 읽은 바코드 카운트가 증가했을 때
			preBarcodeCount2 = pBarCode2->GetCount();

			// 새로운 바코드 값
			strBarCode = "";
			strBarCode = (LPCSTR)pBarCode2->GetDataAt(preBarcodeCount2-1);

			// 리스트 아이템 갯수
			nListPos = m_list_bar2.GetItemCount();
			strListPos.Format( _T("%d"), (nListPos+1) );

			// 리스트 번호값
			m_list_bar2.InsertItem(nListPos, strListPos);
			// PCB 바코드 값
			m_list_bar2.SetItem(nListPos, 1, LVIF_TEXT, strBarCode, 0, 0, 0, NULL );
								
			// ListBox 컨트롤 자동 스크롤
			m_list_bar2.SendMessage(WM_VSCROLL, SB_BOTTOM);

			// IO Board -> Scan off
			// 바코드를 읽은 후에는 OFF -> ON 리셋 필요
			Bar2ScanOFF();
			nBar2ScanCnt = 0;
		}
		else
		{
			nBar2ScanCnt++;	// per100ms

			// 30초간 바코드를 읽지 못하면 OFF -> ON 리셋 필요
			if ( nBar2ScanCnt > 30 )
			{
				Bar2ScanOFF();
				nBar2ScanCnt = 0;
			}
		}
	}
	
	CDialogEx::OnTimer(nIDEvent);
}


void BarCodeDlg::Barcode1Close()
{
	if ( pBarCode1 != NULL )
	{
		KillTimer(TIMER_BARCODE1);

		pBarCode1->Close();
		pBarCode1->DelAllData();
		delete pBarCode1;
		pBarCode1 = NULL;

		// 바코드 갯수 초기화
		preBarcodeCount1 = 0;

		// Port Connect
		m_edit_bar1_connect.SetText(BLACK, GREEN, _T("Not Connected"), UPDATE_ON);

		// 스캐너 초기화
		Bar1ScanOFF();

		dp("Barcode1 Close\n");
	}
}
void BarCodeDlg::Barcode2Close()
{
	if ( pBarCode2 != NULL )
	{
		KillTimer(TIMER_BARCODE2);

		pBarCode2->Close();
		pBarCode2->DelAllData();
		delete pBarCode2;
		pBarCode2 = NULL;

		// 바코드 갯수 초기화
		preBarcodeCount2 = 0;

		// Port Connect
		m_edit_bar2_connect.SetText(BLACK, GREEN, _T("Not Connected"), UPDATE_ON);

		// 스캐너 초기화
		Bar2ScanOFF();

		dp("Barcode2 Close\n");
	}
}

void BarCodeDlg::OnBnClickedButtonBar1Open()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 포트 불러오기
	CString strBar1Com;
	CLogManage	*pEolSetting;
	pEolSetting = new CLogManage;
	pEolSetting->SetFilePath(SETTING_FILE_PATH);
	strBar1Com = pEolSetting->ReadSetting(COM_BAR1);
	delete pEolSetting;
    pEolSetting = NULL;

	m_edit_bar1_port.SetText(BLACK, GREEN, strBar1Com, UPDATE_ON);
	
	if ( pBarCode1 == NULL )
	{
		// 바코드 초기화
		pBarCode1 = new CBarcodeMgt;

		if (pBarCode1->Open((CStringA)strBar1Com) == true)
		{
			dp("Barcode1 : %s Open Success\n", (CStringA)strBar1Com);

			// 바코드 Thread 시작
			pBarCode1->Thread_Begin();
			// 바코드 타이머 설정
			SetTimer(TIMER_BARCODE1, 100, NULL);

			// Port Connect
			m_edit_bar1_connect.SetText(GREEN, BLACK, _T("Connected"), UPDATE_ON);
		}
		else
		{
			dp("Barcode1 : %s Open Fail\n", (CStringA)strBar1Com);

			Barcode1Close();
		}
	}
	else
	{
		Barcode1Close();
	}
}

void BarCodeDlg::OnBnClickedButtonBar2Open()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 포트 불러오기
	CString strBar2Com;
	CLogManage	*pEolSetting;
	pEolSetting = new CLogManage;
	pEolSetting->SetFilePath(SETTING_FILE_PATH);
	strBar2Com = pEolSetting->ReadSetting(COM_BAR2);
	delete pEolSetting;
    pEolSetting = NULL;

	m_edit_bar2_port.SetText(BLACK, GREEN, strBar2Com, UPDATE_ON);

	if ( pBarCode2 == NULL )
	{
		// 바코드 초기화
		pBarCode2 = new CBarcodeMgt;

		if (pBarCode2->Open((CStringA)strBar2Com) == true)
		{
			dp("Barcode2 : %s Open Success\n", (CStringA)strBar2Com);

			// 바코드 Thread 시작
			pBarCode2->Thread_Begin();
			// 바코드 타이머 설정
			SetTimer(TIMER_BARCODE2, 100, NULL);

			// Port Connect
			m_edit_bar2_connect.SetText(GREEN, BLACK, _T("Connected"), UPDATE_ON);
		}
		else
		{
			dp("Barcode2 : %s Open Fail\n", (CStringA)strBar2Com);

			Barcode2Close();
		}
	}
	else
	{
		Barcode2Close();
	}
}

void BarCodeDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();

	// 바코드 종료
	Barcode1Close();
	Barcode2Close();
	// IO 종료
	CloseIoBoard();
}

void BarCodeDlg::OnBnClickedButtonBar1Clr()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 모든 아이템 지우기
	m_list_bar1.DeleteAllItems();
	
	dp("Clear Barcode1\n");
}


void BarCodeDlg::OnBnClickedButtonBar2Clr()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 모든 아이템 지우기
	m_list_bar2.DeleteAllItems();
	
	dp("Clear Barcode2\n");
}

HBRUSH BarCodeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	switch(nCtlColor)
    {
		// 에디트 박스
		case CTLCOLOR_EDIT:
		{
			// 텍스트 배경을 투명하게 한다.
			pDC->SetBkMode(TRANSPARENT);
			// 텍스트의 색상을 변경한다.
			pDC->SetTextColor(GREEN);
			// 리스트 컨트롤의 배경을 투명하게 한다.
			pDC->SelectStockObject(NULL_BRUSH);
			return NULL;
		}
		// 읽기 전용 컨트롤
		case CTLCOLOR_STATIC:
        {
			// 텍스트 배경을 투명하게 한다.
			pDC->SetBkMode(TRANSPARENT);
			// 텍스트의 색상을 변경한다.
			pDC->SetTextColor(GREEN);
			// 리스트 컨트롤의 배경을 투명하게 한다.
			pDC->SelectStockObject(NULL_BRUSH);
			return NULL;
		}
    }
	
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


// SerialDlg 메시지 처리기입니다.
BOOL BarCodeDlg::OpenIoBoard()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	pBarIoBoard = new CIoBoardMgt;
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
	
    if (pBarIoBoard == NULL)
    {
        return false;
    }

	if (pBarIoBoard->Open(pbIoBdCom) == false)
    {
        dp("[%s] IO Board Open Fail!\n",pbIoBdCom);

		CloseIoBoard();
        return false;
    }
		

	strIoBdProduct = _T("KM ");
    if (pBarIoBoard->TransferMsg("$01GA001\r") == false)
    {
        dp("[%s] IO Board communication error!!\n", pbIoBdCom);

		CloseIoBoard();
        return false;
    }
	else
	{
		strIoBdProduct += (pBarIoBoard->GetMsg()+9);
	}

	strIoBdProduct += _T(" Ver ");
    if (strcmp(pBarIoBoard->GetMsg()+9, "6050") != 0)
    {
        dp("[%s] IO Board model %s error!\n",pbIoBdCom, pBarIoBoard->GetMsg()+9);

		CloseIoBoard();
        return false;
    }
	else
	{
		strIoBdProduct += (pBarIoBoard->GetMsg()+9);
	}
		
	delete pbIoBdCom;
	pbIoBdCom= NULL;

	// 스캐너 초기화
	Bar1ScanOFF();
	Bar2ScanOFF();

	return true;
}

BOOL BarCodeDlg::CloseIoBoard()
{
	if (pBarIoBoard == NULL)
    {
        return false;
    }

	// 스캐너 초기화
	Bar1ScanOFF();
	Bar2ScanOFF();

	delete pBarIoBoard;
    pBarIoBoard = NULL;
	dp("IO Board Close\n");

	return true;
}

BOOL BarCodeDlg::Bar1ScanON()
{
	if (pBarIoBoard == NULL)
    {
        return false;
    }

	pBarIoBoard->PutOutBit(_BIT_OUT_BARCODE1, true);

	return true;
}
BOOL BarCodeDlg::Bar1ScanOFF()
{
	if (pBarIoBoard == NULL)
    {
        return false;
    }

	pBarIoBoard->PutOutBit(_BIT_OUT_BARCODE1, false);

	return true;
}
BOOL BarCodeDlg::Bar2ScanON()
{
	if (pBarIoBoard == NULL)
    {
        return false;
    }

	pBarIoBoard->PutOutBit(_BIT_OUT_BARCODE2, true);

	return true;
}
BOOL BarCodeDlg::Bar2ScanOFF()
{
	if (pBarIoBoard == NULL)
    {
        return false;
    }

	pBarIoBoard->PutOutBit(_BIT_OUT_BARCODE2, false);

	return true;
}

BOOL BarCodeDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//CDC *pDC = GetDC();		// 실제 DC 얻기
	CRect rect;
	GetClientRect(rect);	// 현재 창 사이즈 얻기

	CDC MemDC;				// 메모리 DC
	CBitmap *pOldBitmap;	// 메모리 DC의 비트맵
	CBitmap bmp;			// 현재 DC 설정의 비트맵

	MemDC.CreateCompatibleDC(pDC);									// 메모리 DC와 현재 DC의 설정 일치
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());	// 비트맵과 현재 DC의 설정 일치 
	pOldBitmap = (CBitmap *)MemDC.SelectObject(&bmp);				// 메모리 DC의 비트맵 선택
	MemDC.PatBlt(0, 0, rect.Width(), rect.Height(), WHITENESS);		// 패턴 출력
																	// PATCOPY(복사), BLACKNESS(검정), WHITENESS(흰색), DSTINVERT(색상반전)

	///////////////////////////////////////////////
	// 메모리 DC에 비트맵 그리기
	///////////////////////////////////////////////
	CBitmap bmpZebra;
	bmpZebra.LoadBitmap(IDB_BITMAP_ZEBRACODE3);	// 비트맵 로드
	MemDC.SelectObject(&bmpZebra);

	BITMAP bmpZebraSize;						// 비트맵 오브젝트
    bmpZebra.GetObject(sizeof(BITMAP), &bmpZebraSize);	// 비트맵 사이즈 얻기 (bmpZebraSize.bmWidth, bmpZebraSize.bmHeight)

	// StretchBlt (
	//	HDC hdcDest  : 이미지를 출력할 위치의 핸들
	//  nXOriginDest : 이미지를 출력할 x 좌표
	//  nYOriginDest : 이미지를 출력할 y 좌표
	//  nWidthDest   : 출력할 이미지의 너비
	//  nHeightDest  : 출력할 이미지의 높이
	//                (hdcSrc로부터 받은 이미지를 이 너비와 높이에 맞게 이미지 크기를 변경합니다)
	//  hDcSrc       : 이미지의 핸들
	//  nXOriginSrc  : 가져올 이미지의 시작지점인 x 좌표
	//  nOriginSrc   : 가져올 이미지의 시작지점인 y 좌표 
	//                (지정한 위치로부터 nWidthSrc, nHeightSrc만큼 이미지를 잘라옵니다)
	//  nWidthSrc    : 원본 이미지로부터 해당 크기만큼 잘라낼 이미지의 너비
	//  nHeightSrc   : 원본 이미지로부터 해당 크기만큼 잘라낼 이미지의 높이
	//                (이 크기만큼 원본 이미지에서 잘라내어 nWidthDest, nHeightDest의 크기에 맞게 이미지 크기를 변경합니다)
	//  dwRop        : 이미지의 출력 방법
	pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, bmpZebraSize.bmWidth, bmpZebraSize.bmHeight, SRCCOPY);

	// 사용된 메모리 및 비트맵 삭제
	MemDC.SelectObject(pOldBitmap);
    MemDC.DeleteDC();
	return true;

	return CDialogEx::OnEraseBkgnd(pDC);
}
