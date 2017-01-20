// CExpressImgDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "R7EOLvs2010.h"
#include "y_ExpressImgDlg.h"
#include "afxdialogex.h"
#include "y_wm_user.h"
#include "y_Color.h"

// CExpressImgDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CExpressImgDlg, CDialogEx)

CExpressImgDlg::CExpressImgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExpressImgDlg::IDD, pParent)
{

}

CExpressImgDlg::~CExpressImgDlg()
{

}

void CExpressImgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CExpressImgDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CExpressImgDlg 메시지 처리기입니다.

BOOL CExpressImgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//타이머 시작
    SetTimer(TIMER_EXPRESS_IMG, 100, 0);
	
	m_nCurrPos = 0;
	m_tImageShowCompleted = 0;
	m_nImgView = CALIBRATION;
	m_nPreImgView = CALIBRATION;

	m_nVanish = 90;						// Vanish 값
	m_nLaneWidth = 12;					// 차선 넓이
	m_nLaneMovement = 30;				// 차선 이동량
	m_nLaneLColor = YELLOW;				// 왼쪽 차선 색상
	m_nLaneRColor = WHITE;				// 오른쪽 차선 색상

	CreateExpressImg();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CExpressImgDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CClientDC dc(this);
	int nSequence = 25;	// 최대 25번 이동

	switch(nIDEvent)
	{
	// 타이머를 이용하여 변수의 값이 변하는 순간에 전체 이미지를 표시한다.
	case TIMER_EXPRESS_IMG :

		if ( m_nPreImgView != m_nImgView )
		{
			m_nCurrPos = 0;
			m_tImageShowCompleted = 0;
			m_nPreImgView = m_nImgView;
		}
		else
		{
			if (m_nCurrPos > nSequence)
			{
				m_tImageShowCompleted = 1;
			}
			else
			{
				DrawALL(&dc, m_nImgView);
			}
		}		
		break;
	default :

		break;
	}


	CDialogEx::OnTimer(nIDEvent);
}


BOOL CExpressImgDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CDialogEx::OnEraseBkgnd(pDC);
}


void CExpressImgDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	DrawALL(&dc,  m_nImgView);
}

void CExpressImgDlg::DrawALL(CDC *pDC, int PartNum)
{
	int CenterX, CenterY;
	int ScreenWidth = rtWholeScreenSize.right - rtWholeScreenSize.left;
	int ScreenHeight = rtWholeScreenSize.bottom - rtWholeScreenSize.top;

	/////////////////////////////////////////////////////////////////
	//화면 깜박임 방지
	//화면 DC와 호환성있는 메모리 DC를 만듦
	CDC BufferDC;
	BufferDC.CreateCompatibleDC(pDC);

	//화면 DC와 호환성있는 메모리 비트맵을 만듦
	CBitmap bmpBuffer;
	bmpBuffer.CreateCompatibleBitmap(pDC, ScreenWidth, ScreenHeight);

	//메모리 DC에 메모리 비트맵을 선택
	CBitmap *pOldBitmap = (CBitmap*)BufferDC.SelectObject(&bmpBuffer);
	/////////////////////////////////////////////////////////////////

	GetDisplaySpace(rtSubScreenSize);
	
    CenterX	= (rtSubScreenSize.right + rtSubScreenSize.left) / 2;
	CenterY = (rtSubScreenSize.bottom + rtSubScreenSize.top) / 2;

	switch ( PartNum )
	{
	//보정판 이미지
	case CALIBRATION :
		// 가로 선
        theDraw.Draw_Line(&BufferDC, rtSubScreenSize.left, CenterY  , rtSubScreenSize.right, CenterY  , WHITE);

		// 세로 선
        theDraw.Draw_Line(&BufferDC, CenterX  , rtSubScreenSize.top, CenterX  , rtSubScreenSize.bottom, WHITE);

        pDC->BitBlt(rtWholeScreenSize.left, rtWholeScreenSize.top, ScreenWidth, ScreenHeight, &BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.top, SRCCOPY);
        BufferDC.SelectObject(pOldBitmap);
		break;
	case LDW_DETECT :
		DrawLine_Sub (&BufferDC, 0);	//이동량
		m_nCurrPos++;
		break;
	case LDW_WARN_R :
		DrawLine_Sub (&BufferDC, m_nLaneMovement * (-1));	//이동량
		m_nCurrPos++;
		break;
	case LDW_WARN_L :
		DrawLine_Sub (&BufferDC, m_nLaneMovement);	//이동량
		m_nCurrPos++;
		break;
	case FCW_CAR_DETECT :
		DrawLine_Sub (&BufferDC, 0);	//이동량
		Vehicle_Sub (&BufferDC, 0, 0, IDB_BITMAP_REALCAR01);	//이동량, 초기크기
		m_nCurrPos++;
		break;
	case FCW_CAR_WARN :
		DrawLine_Sub (&BufferDC, 0);	//이동량
		Vehicle_Sub (&BufferDC, 4, 0, IDB_BITMAP_REALCAR01);	//이동량, 초기크기
		m_nCurrPos++;
		break;
	case FCDA_CAR_DETECT :
		DrawLine_Sub (&BufferDC, 0);	//이동량
		Vehicle_Sub (&BufferDC, 0, 35, IDB_BITMAP_REALCAR01BRAKE);	//이동량, 초기크기
		m_nCurrPos++;
		break;
	case FCDA_CAR_WARN :
		DrawLine_Sub (&BufferDC, 0);	//이동량
		Vehicle_Sub (&BufferDC, -3, 35, IDB_BITMAP_REALCAR01);	//이동량, 초기크기
		m_nCurrPos++;
		break;
	case LDW_DETECT_NIGHT :
		DrawLineNight_Sub (&BufferDC, 0);	//이동량
		m_nCurrPos++;
		break;
	case LDW_WARN_L_NIGHT :
		DrawLineNight_Sub (&BufferDC, m_nLaneMovement * (-1));	//이동량
		m_nCurrPos++;
		break;
	case LDW_WARN_R_NIGHT :
		DrawLineNight_Sub (&BufferDC, m_nLaneMovement);	//이동량
		m_nCurrPos++;
		break;
	case FCW_CAR_DETECT_NIGHT :
		DrawLineNight_Sub (&BufferDC, 0);	//이동량
		Vehicle_Sub (&BufferDC, 0, 0, IDB_BITMAP_REALCAR01BRAKE);	//이동량, 초기크기
		m_nCurrPos++;
		break;
	case FCW_CAR_WARN_NIGHT :
		DrawLineNight_Sub (&BufferDC, 0);	//이동량
		Vehicle_Sub (&BufferDC, 4, 0, IDB_BITMAP_REALCAR01BRAKE);	//이동량, 초기크기
		m_nCurrPos++;
		break;
	case FCDA_CAR_DETECT_NIGHT :
		DrawLineNight_Sub (&BufferDC, 0);	//이동량
		Vehicle_Sub (&BufferDC, 0, 35, IDB_BITMAP_REALCAR01BRAKE);	//이동량, 초기크기
		m_nCurrPos++;
		break;
	case FCDA_CAR_WARN_NIGHT :
		DrawLineNight_Sub (&BufferDC, 0);	//이동량
		Vehicle_Sub (&BufferDC, -3, 35, IDB_BITMAP_REALCAR01BRAKE);	//이동량, 초기크기
		m_nCurrPos++;
		break;
	case FCW_CAR_DETECT2 :
		DrawLine_Sub (&BufferDC, 0);	//이동량
		Vehicle_Sub (&BufferDC, 0, 0, IDB_BITMAP_REALCAR02);	//이동량, 초기크기
		m_nCurrPos++;
		break;
	case FCW_CAR_WARN2 :
		DrawLine_Sub (&BufferDC, 0);	//이동량
		Vehicle_Sub (&BufferDC, 4, 0, IDB_BITMAP_REALCAR02);	//이동량, 초기크기
		m_nCurrPos++;
		break;
	case FCDA_CAR_DETECT2 :
		DrawLine_Sub (&BufferDC, 0);	//이동량
		Vehicle_Sub (&BufferDC, 0, 35, IDB_BITMAP_REALCAR02);	//이동량, 초기크기
		m_nCurrPos++;
		break;
	case FCDA_CAR_WARN2 :
		DrawLine_Sub (&BufferDC, 0);	//이동량
		Vehicle_Sub (&BufferDC, -3, 35, IDB_BITMAP_REALCAR02);	//이동량, 초기크기
		m_nCurrPos++;
		break;
	case FCW_CAR_DETECT3 :
		DrawLine_Sub (&BufferDC, 0);	//이동량
		Vehicle_Sub (&BufferDC, 0, 0, IDB_BITMAP_REALCAR03);	//이동량, 초기크기
		m_nCurrPos++;
		break;
	case FCW_CAR_WARN3 :
		DrawLine_Sub (&BufferDC, 0);	//이동량
		Vehicle_Sub (&BufferDC, 4, 0, IDB_BITMAP_REALCAR03);	//이동량, 초기크기
		m_nCurrPos++;
		break;
	case FCDA_CAR_DETECT3 :
		DrawLine_Sub (&BufferDC, 0);	//이동량
		Vehicle_Sub (&BufferDC, 0, 35, IDB_BITMAP_REALCAR03);	//이동량, 초기크기
		m_nCurrPos++;
		break;
	case FCDA_CAR_WARN3 :
		DrawLine_Sub (&BufferDC, 0);	//이동량
		Vehicle_Sub (&BufferDC, -3, 35, IDB_BITMAP_REALCAR03);	//이동량, 초기크기
		m_nCurrPos++;
		break;
	default :

		break;
	}


	//메모리 비트맵에 그려진 내용을 화면으로 전송
	pDC->BitBlt(rtWholeScreenSize.left, rtWholeScreenSize.top, ScreenWidth, ScreenHeight, &BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.top, SRCCOPY);

	//DC 복원
	BufferDC.SelectObject(pOldBitmap);
}

// 이미지 세팅 값이 이전과 같으면 0, 다르면 1을 리턴
BOOL CExpressImgDlg::ImgSet(_ImgView ImgNum)
{
	m_nImgView = ImgNum;

	if ( m_nImgView == m_nPreImgView )	return 0;
	else								return 1;
}

BOOL CExpressImgDlg::ImgSet_Vanish(int vanish)
{
	// vanish 값
	if (vanish < 0) return 0;
	//if ( (vanish < 0) || (vanish > rtWholeScreenSize.bottom) ) return 0;

	m_nVanish = vanish;
	return 1;
}
BOOL CExpressImgDlg::ImgSet_LandWidth(int laneWidth)
{
	// 차선 넓이
	if ( (laneWidth < 0) || ( laneWidth > (rtSubScreenSize.right - rtSubScreenSize.left)/2) ) return 0;

	m_nLaneWidth = laneWidth;
	return 1;
}
BOOL CExpressImgDlg::ImgSet_LaneMovemnet(int laneMove)
{
	// 차선 이동값
	if ( laneMove < 0 )		return 0;

	m_nLaneMovement = laneMove;
	return 1;
}
BOOL CExpressImgDlg::ImgSet_LaneLColor(COLORREF color)
{
	m_nLaneLColor = color;

	return 1;
}
BOOL CExpressImgDlg::ImgSet_LaneRColor(COLORREF color)
{
	m_nLaneRColor = color;

	return 1;
}


// 서브모니터의 해상도를 확인
// 320 * 240의 비율로 디스플레이를 하기 위하여 상하좌우의 좌표값을 저장
void CExpressImgDlg::GetDisplaySpace(CRect &rt)
{
    int nH, nW, nRate;
    CRect oRect;

    ::GetClientRect(this->m_hWnd, &rtWholeScreenSize);

    oRect = rtWholeScreenSize;

    nW = oRect.right  / 320;
    nH = oRect.bottom / 240;

    nRate = (nH < nW)? nH: nW;
    oRect.right  = 320 * nRate;
    oRect.bottom = 240 * nRate;

    nW = (rtWholeScreenSize.right  - oRect.right ) / 2;
    nH = (rtWholeScreenSize.bottom - oRect.bottom) / 2;

    oRect.left	 += nW;
	oRect.right	 += nW;

    // oRect.top    += (nH - 8*nRate);
    // oRect.bottom += (nH - 8*nRate);

    rt = oRect;
}

// 다이얼로그 종료시 배경이미지 메모리 삭제.
BOOL CExpressImgDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// 타이머 제거
	KillTimer(TIMER_EXPRESS_IMG);

	if(m_pbmBack)
	{
		DeleteObject(m_pbmBack);
	}

	return CDialogEx::DestroyWindow();
}



BOOL CExpressImgDlg::CreateExpressImg()
{
    if (::GetSystemMetrics(SM_CMONITORS) < 2) // 모니터 갯수 구하기
    {
		// 모니터가 하나의 경우
		int nMainLeft   = ::GetSystemMetrics(SM_XVIRTUALSCREEN); // 제일 왼쪽 모니터 X 좌표
		int nMainTop    = ::GetSystemMetrics(SM_YVIRTUALSCREEN); // 제일 위쪽 모니터 Y 좌표
		int nMainWidth  = ::GetSystemMetrics(SM_CXSCREEN);		 // 현재 모니터 전체 넓이
		int nMainHeight = ::GetSystemMetrics(SM_CYSCREEN);		 // 현재 모니터 전체 높이

		MoveWindow(nMainLeft, nMainTop, nMainWidth, nMainHeight);
		
		return false;
    }

    // 서브 모니터 사이즈에 다이얼로그 출력
    HMONITOR    hMonitorThis, hMonitorTarget;
    MONITORINFO oMonitorThis, oMonitorTarget;
    POINT oPoint;

    // 현재 윈도우의 모니터 위치 구하기
    hMonitorThis = ::MonitorFromWindow( this->GetSafeHwnd(), MONITOR_DEFAULTTONEAREST );

    oMonitorThis.cbSize = sizeof(MONITORINFO);
    ::GetMonitorInfo(hMonitorThis, &oMonitorThis);

    // 타겟 윈도우의 모니터 위치 구하기
    if (oMonitorThis.rcMonitor.left != 0)
    {
        // 현재 윈도우는 보조 모니터에 위치해 있다.
        // 따라서 타겟 윈도우는 주 모니터(0, 0)로 설정한다.
        oPoint.x = 0; oPoint.y = 0;
        hMonitorTarget = ::MonitorFromPoint(oPoint, MONITOR_DEFAULTTONEAREST);
        oMonitorTarget.cbSize = sizeof(MONITORINFO);
        ::GetMonitorInfo(hMonitorTarget, &oMonitorTarget);
    }
    else
    {
        // 현재 윈도우는 주 모니터에 위치해 있다.
        // 보조 모니터가 왼쪽에 있는지 오른쪽에 있는지 확인해야 한다.
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

    MoveWindow(oMonitorTarget.rcMonitor.left,
                     oMonitorTarget.rcMonitor.top,
                     oMonitorTarget.rcMonitor.right  - oMonitorTarget.rcMonitor.left,
                     oMonitorTarget.rcMonitor.bottom - oMonitorTarget.rcMonitor.top );

    return true;
}

// PostNcDestroy()는 대화상자가 완전히 소멸된 후에 호출되기 때문에 안전하게 삭제할 수 있다.
void CExpressImgDlg::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;

	//CDialogEx::PostNcDestroy();
}


void CExpressImgDlg::DrawLine_Sub (CDC *BufferDC, int GapMovement)
{
    CPoint arPoint[5];
	
	// 320 * 240의 비율로 디스플레이를 하기 위하여 상하좌우의 좌표값을 저장
    double scaleX = (double)(rtSubScreenSize.right  - rtSubScreenSize.left)   / 320.0;
	double scaleY = (double)(rtSubScreenSize.bottom - rtSubScreenSize.top)    / 240.0;

	int VanishY   = rtSubScreenSize.top + (int)(scaleY * m_nVanish) + 240;
	int LineWidth = (int)(scaleX * m_nLaneWidth);
	int CenterX   = (rtSubScreenSize.right + rtSubScreenSize.left) / 2;
	
    int offset = (int)(scaleX * 5);
    int offset2 = (int)(scaleX * 5);

    // 차도 (지평선 아래)
    theDraw.DrawBox (BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.top, rtWholeScreenSize.right, rtWholeScreenSize.bottom, GREY_ROAD);

    //좌측 차선
    arPoint[0].x = CenterX+5; 
    arPoint[0].y = VanishY;

    arPoint[1].x = rtWholeScreenSize.left-offset2 + GapMovement*m_nCurrPos; 
    arPoint[1].y = rtWholeScreenSize.bottom;

    arPoint[2].x = rtWholeScreenSize.left-(offset2+LineWidth) + GapMovement*m_nCurrPos; 
    arPoint[2].y = rtWholeScreenSize.bottom;

    arPoint[3].x = CenterX-5; 
    arPoint[3].y = VanishY;

    arPoint[4] = arPoint[0];
    theDraw.Draw_Rgn(BufferDC, arPoint, 4, m_nLaneLColor);
    
    //우측 차선
    arPoint[0].x = CenterX-5; 
    arPoint[0].y = VanishY;

    arPoint[1].x = rtWholeScreenSize.right+offset + GapMovement*m_nCurrPos; 
    arPoint[1].y = rtWholeScreenSize.bottom;
    
	arPoint[2].x = rtWholeScreenSize.right+(offset+LineWidth) + GapMovement*m_nCurrPos; 
    arPoint[2].y = rtWholeScreenSize.bottom;
    
	arPoint[3].x = CenterX+5; 
    arPoint[3].y = VanishY;
    
	arPoint[4] = arPoint[0];
    theDraw.Draw_Rgn(BufferDC, arPoint, 4, m_nLaneRColor);
    
	// 하늘 (지평선 위)
    theDraw.DrawBox (BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.top, rtWholeScreenSize.right, VanishY, SKY);
}

COLORREF CExpressImgDlg::RGBtoDark(COLORREF rgb)
{
	COLORREF darkrgb;
	DWORD red, green, blue;
	
	red		= rgb & 0xFF0000;
	green	= rgb & 0x00FF00;
	blue	= rgb & 0x0000FF;

	darkrgb = (red>>1 & 0xFF0000) | (green>>1 & 0x00FF00) | (blue>>1 & 0x0000FF);
	
	return darkrgb;
}



void CExpressImgDlg::DrawLineNight_Sub (CDC *BufferDC, int GapMovement)
{
    CPoint arPoint[5];
	
	// 320 * 240의 비율로 디스플레이를 하기 위하여 상하좌우의 좌표값을 저장
    double scaleX = (double)(rtSubScreenSize.right  - rtSubScreenSize.left)   / 320.0;
	double scaleY = (double)(rtSubScreenSize.bottom - rtSubScreenSize.top)    / 240.0;

	int VanishY   = rtSubScreenSize.top + (int)(scaleY * m_nVanish) + 240;
	int LineWidth = (int)(scaleX * m_nLaneWidth);
	int CenterX   = (rtSubScreenSize.right + rtSubScreenSize.left) / 2;
	
    int offset = (int)(scaleX * 5);
    int offset2 = (int)(scaleX * 5);

    // 차도 (지평선 아래)
    theDraw.DrawBox (BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.top, rtWholeScreenSize.right, rtWholeScreenSize.bottom, RGBtoDark(GREY_ROAD));

    //좌측 차선
    arPoint[0].x = CenterX+5; 
    arPoint[0].y = VanishY;

    arPoint[1].x = rtWholeScreenSize.left-offset2 + GapMovement*m_nCurrPos; 
    arPoint[1].y = rtWholeScreenSize.bottom;

    arPoint[2].x = rtWholeScreenSize.left-(offset2+LineWidth) + GapMovement*m_nCurrPos; 
    arPoint[2].y = rtWholeScreenSize.bottom;

    arPoint[3].x = CenterX-5; 
    arPoint[3].y = VanishY;

    arPoint[4] = arPoint[0];
    theDraw.Draw_Rgn(BufferDC, arPoint, 4, RGBtoDark(m_nLaneLColor));
    
    //우측 차선
    arPoint[0].x = CenterX-5; 
    arPoint[0].y = VanishY;

    arPoint[1].x = rtWholeScreenSize.right+offset + GapMovement*m_nCurrPos; 
    arPoint[1].y = rtWholeScreenSize.bottom;
    
	arPoint[2].x = rtWholeScreenSize.right+(offset+LineWidth) + GapMovement*m_nCurrPos; 
    arPoint[2].y = rtWholeScreenSize.bottom;
    
	arPoint[3].x = CenterX+5; 
    arPoint[3].y = VanishY;
    
	arPoint[4] = arPoint[0];
    theDraw.Draw_Rgn(BufferDC, arPoint, 4, RGBtoDark(m_nLaneRColor));
    
	// 하늘 (지평선 위)
    theDraw.DrawBox (BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.top, rtWholeScreenSize.right, VanishY, BLACK);
}

void CExpressImgDlg::Vehicle_Sub (CDC *BufferDC, int GapMovement, int nInitSize, int CAR_BITMAP)
{
	CDC MemDC;
	CBitmap bmp;

	double scale = (double)(rtSubScreenSize.right - rtSubScreenSize.left)/320.0;
	double scaleH = (double)(rtSubScreenSize.bottom - rtSubScreenSize.top)/320.0;
	int CenterX	= (rtSubScreenSize.right + rtSubScreenSize.left) / 2;
	int CenterY = rtSubScreenSize.top + (int)(scale*m_nVanish) + 240;

	MemDC.CreateCompatibleDC(BufferDC);
	bmp.LoadBitmap(CAR_BITMAP);
	MemDC.SelectObject(&bmp);
	if ( (CAR_BITMAP == IDB_BITMAP_REALCAR01) || (CAR_BITMAP == IDB_BITMAP_REALCAR01BRAKE) )
	{
		BufferDC->StretchBlt(CenterX-((89 + 89 * ((nInitSize + (GapMovement/2)*m_nCurrPos)/10.0))/2), CenterY, 100 + 100 * ((nInitSize + (GapMovement/2)*m_nCurrPos)/10.0), 89 + 89 * ((nInitSize + (GapMovement/2)*m_nCurrPos)/10.0)/1.0, &MemDC, 0, 0, 200, 200, SRCCOPY);	
	}
	else if ( CAR_BITMAP == IDB_BITMAP_REALCAR02 )
	{
		//CenterY -= 50;
		BufferDC->StretchBlt(CenterX-((89 + 89 * ((nInitSize + (GapMovement/2)*m_nCurrPos)/10.0))/2), CenterY, 100 + 100 * ((nInitSize + (GapMovement/2)*m_nCurrPos)/10.0), 89 + 89 * ((nInitSize + (GapMovement/2)*m_nCurrPos)/10.0), &MemDC, 0, 0, 200, 250, SRCCOPY);	
	}
	else if ( CAR_BITMAP == IDB_BITMAP_REALCAR03 )
	{
		BufferDC->StretchBlt(CenterX-((89 + 89 * ((nInitSize + (GapMovement/2)*m_nCurrPos)/10.0))/2), CenterY, 100 + 100 * ((nInitSize + (GapMovement/2)*m_nCurrPos)/10.0), 89 + 89 * ((nInitSize + (GapMovement/2)*m_nCurrPos)/10.0), &MemDC, 0, 0, 200, 200, SRCCOPY);	
	}
		
	bmp.DeleteObject();
	MemDC.DeleteDC();
}