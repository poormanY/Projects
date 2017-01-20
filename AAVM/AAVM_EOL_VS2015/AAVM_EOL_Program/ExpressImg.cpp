#include "stdafx.h"
#include "MainApp.h"
#include "ExpressImg.h"
#include "Color.h"
#include "Draw.h"
#include "DialogSet.h"

extern CDialogSet  *pSetupDlg;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExpressImg dialog

//�̹����� �׸��� ���� Ŭ���� ȣ��
Draw theDraw;

extern int m_nImgView;
extern int tImageShowCompleted;

CExpressImg::CExpressImg(CWnd* pParent /*=NULL*/)
	: CDialog(CExpressImg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExpressImg)
		// NOTE: the ClassWizard will add member initialization here
	m_pbmBack = NULL;
	//}}AFX_DATA_INIT
}


void CExpressImg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExpressImg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExpressImg, CDialog)
	//{{AFX_MSG_MAP(CExpressImg)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExpressImg message handlers

BOOL CExpressImg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	//Ÿ�̸� ����
    SetTimer(0, 100, 0);

    nCurrPos = 0;
	tImageShowCompleted = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// �ʱ� ȭ�鿡 �̹��� ǥ��.
void CExpressImg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	DrawLine(&dc, m_nImgView);

	// Do not call CDialog::OnPaint() for painting messages
}

// ���̾�α� ����� ����̹��� �޸� ����.
BOOL CExpressImg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pbmBack)
	{
		DeleteObject(m_pbmBack);
	}

	return CDialog::DestroyWindow();
}

//����̹����� ����
BOOL CExpressImg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	if (m_pbmBack)
	{
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		CBitmap* poldbitmap = memDC.SelectObject(m_pbmBack);

		// Get ImageSize
		BITMAP bm;
		m_pbmBack->GetObject(sizeof(bm),(LPSTR)&bm);

		int temp;
		CRect desRect,clientRect;
		GetClientRect(clientRect);
		desRect=clientRect;

		if (desRect.Width()*bm.bmHeight>= desRect.Height()*bm.bmWidth)
		{
			temp=desRect.Width()-bm.bmWidth*desRect.Height()/bm.bmHeight;
			desRect.left+=temp/2;
			desRect.right-=temp/2;
		}
		else
		{
			temp=desRect.Height()-bm.bmHeight*desRect.Width()/bm.bmWidth;
			desRect.top+=temp/2;
			desRect.bottom-=temp/2;
		}

		COLORREF colBack = memDC.GetPixel(0,0);
		pDC->FillSolidRect(clientRect, colBack);

		pDC->StretchBlt(desRect.left, desRect.top, desRect.Width(), desRect.Height(),
			&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

		memDC.SelectObject(poldbitmap);

		return TRUE;
	}
	else
	{
		return CDialog::OnEraseBkgnd(pDC);
	}
}

void CExpressImg::DrawLine(CDC *pDC, int PartNum)
{
	CPoint arPoint[4];
	int CenterX, CenterY;
	int ScreenWidth = rtWholeScreenSize.right - rtWholeScreenSize.left;
	int ScreenHeight = rtWholeScreenSize.bottom - rtWholeScreenSize.top;

	/////////////////////////////////////////////////////////////////
	//ȭ�� ������ ����
	//ȭ�� DC�� ȣȯ���ִ� �޸� DC�� ����
	CDC BufferDC;
	BufferDC.CreateCompatibleDC(pDC);

	//ȭ�� DC�� ȣȯ���ִ� �޸� ��Ʈ���� ����
	CBitmap bmpBuffer;
	bmpBuffer.CreateCompatibleBitmap(pDC, ScreenWidth, ScreenHeight);

	//�޸� DC�� �޸� ��Ʈ���� ����
	CBitmap *pOldBitmap = (CBitmap*)BufferDC.SelectObject(&bmpBuffer);
	/////////////////////////////////////////////////////////////////


	GetDisplaySpace(rtSubScreenSize);

    CenterX	= (rtSubScreenSize.right + rtSubScreenSize.left) / 2 - 14;
	CenterY = (rtSubScreenSize.bottom + rtSubScreenSize.top) / 2 + 15;

	switch( PartNum )
	{
	//������ �̹���
	case 0 :
		// ���� ��
        theDraw.Draw_Line(&BufferDC, rtSubScreenSize.left, CenterY  , rtSubScreenSize.right, CenterY  , RGB(255,255,255));

		// ���� ��
        theDraw.Draw_Line(&BufferDC, CenterX  , rtSubScreenSize.top, CenterX  , rtSubScreenSize.bottom, RGB(255,255,255));

        pDC->BitBlt(rtWholeScreenSize.left, rtWholeScreenSize.top, ScreenWidth, ScreenHeight, &BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.top, SRCCOPY);
        BufferDC.SelectObject(pOldBitmap);
		break;

	//���� �̹���
	case 1 :
		DrawLine_Sub (&BufferDC, 0);	//0:�̵���

		break;

	// ���� ���� ��Ż �̹���
	case 2 :
        DrawLine_Sub (&BufferDC, -18);	//�̵���

		nCurrPos++;
		break;

	//���� ���� ��Ż �̹���
	case 3 :
        DrawLine_Sub (&BufferDC, +18);	//�̵���

        nCurrPos++;
		break;

	//���� ����
	case 4 :
        RearLght_Sub (&BufferDC, false, true);
		break;

	//���� ����
	case 5 :
        RearLght_Sub (&BufferDC, true, true);
		break;

    case 6 :
        DrawLine_AS_Calibration (&BufferDC);
        break;

	//���� ����
	case 7 :
        RearLght_Sub (&BufferDC, false, false);
		break;


	default :
		break;
	}

	//�޸� ��Ʈ�ʿ� �׷��� ������ ȭ������ ����
	pDC->BitBlt(rtWholeScreenSize.left, rtWholeScreenSize.top, ScreenWidth, ScreenHeight, &BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.top, SRCCOPY);

	//DC ����
	BufferDC.SelectObject(pOldBitmap);
}

// ���������� �ػ󵵸� Ȯ��
// 320 * 240�� ������ ���÷��̸� �ϱ� ���Ͽ� �����¿��� ��ǥ���� ����
void CExpressImg::GetDisplaySpace(CRect &rt)
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

// Ÿ�̸Ӹ� �̿��Ͽ� ������ ���� ���ϴ� ������ ��ü �̹����� ǥ���Ѵ�.
void CExpressImg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);

    int nSequence = 25;

    nSequence = 25;

	if ( nPreImgView != m_nImgView )
	{
        nCurrPos = 0;
        tImageShowCompleted = 0;
        nPreImgView = m_nImgView;
    }
    else
    {
        if (nCurrPos>nSequence)
        {
            tImageShowCompleted = 1;
        }
        else
        {
            DrawLine(&dc, m_nImgView);
        }
	}

    CDialog::OnTimer(nIDEvent);
}

/*
void CExpressImg::DrawLine_Sub (CDC *BufferDC, int GapMovement)
{
	CPoint arPoint[4];
	double scale = (double)(rtSubScreenSize.right - rtSubScreenSize.left)/320.0;
//    int VanishY = rtSubScreenSize.top + (int)(scale*50.0);	//BH=44, KH=90
//    int VanishY = rtSubScreenSize.top + (int)(scale*33.0);	//BH=44, KH=90
    int VanishY = rtSubScreenSize.top + (int)(scale*41.0);	//BH=50, KH=90
//	int LineWidth = (int)(scale*30.0);
    int LineWidth = (int)(scale*30.0);
	int CenterX	= (rtSubScreenSize.right + rtSubScreenSize.left) / 2 - 14;
    int CenterY = (rtSubScreenSize.bottom + rtSubScreenSize.top) / 2 + 15;
	int ScreenWidth = rtWholeScreenSize.right - rtWholeScreenSize.left;
	int ScreenHeight = rtWholeScreenSize.bottom - rtWholeScreenSize.top;

    // int offset = 240;   //pixel
    int offset = (int)(scale * 90);   // ������
    CenterX += (int)((160-160)*scale);

	//���� ����
	arPoint[0].x = CenterX;
	arPoint[0].y = VanishY;
	arPoint[1].x = rtWholeScreenSize.left-offset + GapMovement*(nCurrPos-1);
	arPoint[1].y = rtWholeScreenSize.bottom;
	arPoint[2].x = rtWholeScreenSize.left-(offset+LineWidth) + GapMovement*(nCurrPos-1);
	arPoint[2].y = rtWholeScreenSize.bottom;
	arPoint[3] = arPoint[0];
	theDraw.Draw_Rgn(BufferDC, arPoint, 4, BLACK);

	arPoint[0].x = CenterX;
	arPoint[0].y = VanishY;
	arPoint[1].x = rtWholeScreenSize.left-offset + GapMovement*nCurrPos;
	arPoint[1].y = rtWholeScreenSize.bottom;
	arPoint[2].x = rtWholeScreenSize.left-(offset+LineWidth) + GapMovement*nCurrPos;
	arPoint[2].y = rtWholeScreenSize.bottom;
	arPoint[3] = arPoint[0];
	theDraw.Draw_Rgn(BufferDC, arPoint, 4, YELLOW);

	//���� ����
	arPoint[0].x = CenterX;
	arPoint[0].y = VanishY;
	arPoint[1].x = rtWholeScreenSize.right+offset + GapMovement*(nCurrPos-1);
	arPoint[1].y = rtWholeScreenSize.bottom;
	arPoint[2].x = rtWholeScreenSize.right+(offset+LineWidth) + GapMovement*(nCurrPos-1);
	arPoint[2].y = rtWholeScreenSize.bottom;
	arPoint[3] = arPoint[0];
	theDraw.Draw_Rgn(BufferDC, arPoint, 4, BLACK);

	arPoint[0].x = CenterX;
	arPoint[0].y = VanishY;
	arPoint[1].x = rtWholeScreenSize.right+offset + GapMovement*nCurrPos;
	arPoint[1].y = rtWholeScreenSize.bottom;
	arPoint[2].x = rtWholeScreenSize.right+(offset+LineWidth) + GapMovement*nCurrPos;
	arPoint[2].y = rtWholeScreenSize.bottom;
	arPoint[3] = arPoint[0];
	theDraw.Draw_Rgn(BufferDC, arPoint, 4, WHITE);

//	theDraw.Draw_ThickRect(BufferDC, rtSubScreenSize, 2, WHITE);

    theDraw.DrawBox (BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.top, rtWholeScreenSize.right, VanishY, SKY);
}
*/


void CExpressImg::DrawLine_Sub (CDC *BufferDC, int GapMovement)
{
    CPoint arPoint[5];
    double scale = (double)(rtSubScreenSize.right - rtSubScreenSize.left)/320.0;
//    int VanishY = rtSubScreenSize.top + (int)(scale*43.0);	//BH=50, KH=90
    int VanishY = rtSubScreenSize.top + (int)(scale*50.0);	//BH=50, KH=90 - 2011.09.29
    int LineWidth = (int)(scale*12.0);
//    int CenterX	= (rtSubScreenSize.right + rtSubScreenSize.left) / 2 - 14;
    int CenterX	= (rtSubScreenSize.right + rtSubScreenSize.left) / 2 - 10;  //2011.09.29
    int offset = (int)(scale * 100);   // ������
    int offset2 = (int)(scale * 60);
//    CenterX += (int)((160-160)*scale);

	if (pSetupDlg->m_COMBO_CAR_TYPE == "QZ")
    {
        offset = (int)(scale * -90);
        offset2 = (int)(scale * -90);
        CenterX	= (rtSubScreenSize.right + rtSubScreenSize.left) / 2;
    }
	else if (pSetupDlg->m_COMBO_CAR_TYPE == "GZ")
	{
		offset = (int)(scale * 30);
        offset2 = (int)(scale * -30);
        CenterX	= (rtSubScreenSize.right + rtSubScreenSize.left) / 2;
	}
    //khj QZ line
    else if (pSetupDlg->m_COMBO_CAR_TYPE == "EU")
    {
       // offset = (int)(scale * 0);
        //offset2 = (int)(scale * 0);
        //CenterX	= (rtSubScreenSize.right + rtSubScreenSize.left) / 2;
		VanishY = rtSubScreenSize.top + (int)(scale*50.0);	//BH=50, KH=90 - 2011.09.29
        offset = (int)(scale * (-45));  //2012.04.16
        offset2 = (int)(scale * (-45));
        CenterX	= (rtSubScreenSize.right + rtSubScreenSize.left) / 2;
	}
    else if (pSetupDlg->m_COMBO_CAR_TYPE == "QT")
    {
        offset = (int)(scale * -90);
        offset2 = (int)(scale * -90);
        CenterX	= (rtSubScreenSize.right + rtSubScreenSize.left) / 2;
    }
    theDraw.DrawBox (BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.top, rtWholeScreenSize.right, rtWholeScreenSize.bottom, RGB(96,96,96));

    //���� ����
    arPoint[0].x = CenterX+5;
    arPoint[0].y = VanishY;
    arPoint[1].x = rtWholeScreenSize.left-offset2 + GapMovement*nCurrPos;
    arPoint[1].y = rtWholeScreenSize.bottom;
    arPoint[2].x = rtWholeScreenSize.left-(offset2+LineWidth) + GapMovement*nCurrPos;
    arPoint[2].y = rtWholeScreenSize.bottom;
    arPoint[3].x = CenterX-5;
    arPoint[3].y = VanishY;
    arPoint[4] = arPoint[0];
    theDraw.Draw_Rgn(BufferDC, arPoint, 4, YELLOW);
//    theDraw.Draw_ThickLine(BufferDC, CenterX, VanishY, arPoint[1].x, rtWholeScreenSize.bottom, 10, YELLOW);

    //���� ����
    arPoint[0].x = CenterX-5;
    arPoint[0].y = VanishY;
    arPoint[1].x = rtWholeScreenSize.right+offset + GapMovement*nCurrPos;
    arPoint[1].y = rtWholeScreenSize.bottom;
    arPoint[2].x = rtWholeScreenSize.right+(offset+LineWidth) + GapMovement*nCurrPos;
    arPoint[2].y = rtWholeScreenSize.bottom;
    arPoint[3].x = CenterX+5;
    arPoint[3].y = VanishY;
    arPoint[4] = arPoint[0];
    theDraw.Draw_Rgn(BufferDC, arPoint, 4, WHITE);
//    theDraw.Draw_ThickLine(BufferDC, CenterX, VanishY, arPoint[1].x, rtWholeScreenSize.bottom, 10, WHITE);

    theDraw.DrawBox (BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.top, rtWholeScreenSize.right, VanishY, SKY);
}


void CExpressImg::RearLght_Sub (CDC *BufferDC, int bLightOn, int bDark)
{
    CPoint arPoint[5];
    double scale = (double)(rtSubScreenSize.right - rtSubScreenSize.left)/320.0;
    int VanishY = rtSubScreenSize.top + (int)(scale*43.0);	//BH=50, KH=90
    int LineWidth = (int)(scale*12.0);
    int CenterX	= (rtSubScreenSize.right + rtSubScreenSize.left) / 2 - 14;
    int CenterY = (rtSubScreenSize.bottom + rtSubScreenSize.top) / 2 + 15;

    if ((pSetupDlg->m_COMBO_CAR_TYPE == "KH") || (pSetupDlg->m_COMBO_CAR_TYPE == "VG_FL") || (pSetupDlg->m_COMBO_CAR_TYPE == "HG FL") || (pSetupDlg->m_COMBO_CAR_TYPE == "RP"))
    {
        VanishY = rtSubScreenSize.top + (int)(scale*100.0);	//BH=50, KH=90
    }

	if (bDark==1)
		theDraw.DrawBox (BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.top, rtWholeScreenSize.right, rtWholeScreenSize.bottom, BLACK);
	else
		theDraw.DrawBox (BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.top, rtWholeScreenSize.right, rtWholeScreenSize.bottom, WHITE);

//    theDraw.DrawBox (BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.top, rtWholeScreenSize.right, rtWholeScreenSize.bottom, RGB(16,16,16));
//    theDraw.DrawBox (BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.top, rtWholeScreenSize.right, rtWholeScreenSize.bottom, RGB(32,32,32));
//    theDraw.DrawBox (BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.top, rtWholeScreenSize.right, rtWholeScreenSize.bottom, RGB(96,96,96));
//    theDraw.DrawBox (BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.top, rtWholeScreenSize.right, rtWholeScreenSize.bottom, RGB(128,128,128));

    if (bLightOn==1)
    {
        //���� �Ĺ̵�
//        theDraw.Draw_Circle_Solid(BufferDC, CenterX - 20, VanishY+170, 20, WHITE);
        theDraw.Draw_Circle_Solid(BufferDC, CenterX - 40, VanishY+270, 25, WHITE);

        //���� �Ĺ̵�
//        theDraw.Draw_Circle_Solid(BufferDC, CenterX + 20, VanishY+170, 20, WHITE);
        theDraw.Draw_Circle_Solid(BufferDC, CenterX + 30, VanishY+270, 25, WHITE);
    }
    else
    {
//        theDraw.DrawBox (BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.bottom-150, rtWholeScreenSize.right, rtWholeScreenSize.bottom, WHITE);

//        theDraw.DrawBox (BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.bottom-500, rtWholeScreenSize.right, rtWholeScreenSize.bottom, WHITE);
    }
}


void CExpressImg::DrawLine_AS_Calibration (CDC *BufferDC)
{
    CPoint arPoint[5];
    int CenterX	= (rtSubScreenSize.right + rtSubScreenSize.left) / 2;
    int CenterY = (rtSubScreenSize.bottom + rtSubScreenSize.top) / 2;

    if ((pSetupDlg->m_COMBO_CAR_TYPE == "KH") || (pSetupDlg->m_COMBO_CAR_TYPE == "VG_FL") || (pSetupDlg->m_COMBO_CAR_TYPE == "HG FL"))
    {
//        CenterY = (rtSubScreenSize.top + rtSubScreenSize.bottom)/2 + 385;
        CenterY = (rtSubScreenSize.top + rtSubScreenSize.bottom)/2 + 435;
    }
    else if (pSetupDlg->m_COMBO_CAR_TYPE == "VG")
    {
        CenterX = (rtSubScreenSize.left + rtSubScreenSize.right)/2 + 185 - 20*4 - 16 + 2 + 16;
        CenterY = (rtSubScreenSize.top + rtSubScreenSize.bottom)/2 + 160;   // + 20*4 + 8 + 4;
    }
	else if (pSetupDlg->m_COMBO_CAR_TYPE == "HG")
	{
		CenterX = ((rtSubScreenSize.left + rtSubScreenSize.right)/320) * 131;
        CenterY = ((rtSubScreenSize.top + rtSubScreenSize.bottom)/240) * 182;
	}

    theDraw.DrawBox (BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.top, rtWholeScreenSize.right, rtWholeScreenSize.bottom, LTGRAY);
    if (pSetupDlg->m_COMBO_CAR_TYPE == "KH")
        theDraw.DrawBox (BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.top, rtWholeScreenSize.right, (rtWholeScreenSize.bottom + rtWholeScreenSize.top) / 2, BLACK);
    else if (pSetupDlg->m_COMBO_CAR_TYPE == "VG")
        theDraw.DrawBox (BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.top, rtWholeScreenSize.right, (rtWholeScreenSize.bottom + rtWholeScreenSize.top) / 2-100, BLACK);
	else if (pSetupDlg->m_COMBO_CAR_TYPE == "HG")
        theDraw.DrawBox (BufferDC, rtWholeScreenSize.left, rtWholeScreenSize.top, rtWholeScreenSize.right, (rtWholeScreenSize.bottom + rtWholeScreenSize.top) / 2-100, BLACK);

    if (pSetupDlg->m_COMBO_CAR_TYPE == "KH")
    {
        theDraw.Draw_ThickLine (BufferDC, CenterX-80, CenterY, CenterX+80, CenterY, 8, BLACK);
        theDraw.Draw_ThickLine (BufferDC, CenterX, CenterY-80, CenterX, CenterY+80, 8, BLACK);
    }
    else
    {
        theDraw.Draw_ThickLine (BufferDC, CenterX-70, CenterY, CenterX+70, CenterY, 5, BLACK);
        theDraw.Draw_ThickLine (BufferDC, CenterX, CenterY-70, CenterX, CenterY+70, 5, BLACK);
    }
}
