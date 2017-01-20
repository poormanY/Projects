// Draw.cpp: implementation of the Draw class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
//#include <afxwin.h>		// MFC core and standard components
//#include <afxext.h>		// MFC extensions
//#include <afxdisp.h>		// MFC Automation classes
//#include <afxdtctl.h>	// MFC support for Internet Explorer 4 Common Controls

#include "Draw.h"
#include "color.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Draw::Draw()
{

}

Draw::~Draw()
{

}

void Draw::DrawBox (CDC* pDC, int left, int top, int right, int bottom, COLORREF color)
{
	HBRUSH	hBrush  = CreateSolidBrush(color);
	HBRUSH	hBkBrush = (HBRUSH)SelectObject(pDC->GetSafeHdc(), hBrush);
	SelectObject(pDC->GetSafeHdc(), GetStockObject( NULL_PEN ) ); 
	
	pDC->Rectangle(left, top, right, bottom);
	
	SelectObject(pDC->GetSafeHdc(), hBkBrush);
    DeleteObject(hBrush);
}

void Draw::DrawBox (CDC* pDC, CRect R, COLORREF color)
{
	HBRUSH	hBrush  = CreateSolidBrush(color);
	HBRUSH	hBkBrush = (HBRUSH)SelectObject(pDC->GetSafeHdc(), hBrush);
	SelectObject(pDC->GetSafeHdc(), GetStockObject( NULL_PEN ) ); 
	
	pDC->Rectangle(R.left, R.top, R.right, R.bottom);
	
	SelectObject(pDC->GetSafeHdc(), hBkBrush);
    DeleteObject(hBrush);
}

void Draw::DrawConcaveBox (CDC* pDC, int left, int top, int right, int bottom)
{
	CPen	*lOldPen;
	CPen	lSolidPenGray (PS_SOLID, 1, BLACK);
	CPen	lSolidPenWhite (PS_SOLID, 1, GRAY);
	
	lOldPen = pDC->SelectObject(&lSolidPenGray);
	pDC->MoveTo(left, bottom);
	pDC->LineTo(left, top);
	pDC->LineTo(right, top);
	pDC->SelectObject(lOldPen);

	lOldPen = pDC->SelectObject(&lSolidPenWhite);
	pDC->MoveTo(left+1, bottom);
	pDC->LineTo(right, bottom);
	pDC->LineTo(right, top+1);
	pDC->SelectObject(lOldPen);
}

void Draw::DrawConvexBox (CDC* pDC, int left, int top, int right, int bottom)
{
	DrawBox (pDC, left, top, right, bottom, LTGRAY);

//	Draw_Line (pDC, left-3, bottom+3, left-3, top-3, BLACK);
//	Draw_Line (pDC, left-3, top-3, right+3, top-3, BLACK);
//	Draw_Line (pDC, right+3, top-3, right+3, bottom+3, BLACK);
//	Draw_Line (pDC, right+3, bottom+3, left-3, bottom+3, BLACK);
	
	Draw_Line (pDC, right+2, top-2, right+2, bottom+2, BLACK);
	Draw_Line (pDC, right+2, bottom+2, left-2, bottom+2, BLACK);

	Draw_Line (pDC, left-2, bottom+1, left-2, top-2, WHITE);
	Draw_Line (pDC, left-2, top-2, right+1, top-2, WHITE);
	
	Draw_Line (pDC, right+1, top-1, right+1, bottom+1, GRAY);
	Draw_Line (pDC, right+1, bottom+1, left-1, bottom+1, GRAY);
	
	Draw_Line (pDC, left-1, bottom, left-1, top-1, LIGHTGRAY);
	Draw_Line (pDC, left-1, top-1, right, top-1, LIGHTGRAY);
}

void Draw::DrawWhiteConvexBox (CDC* pDC, int left, int top, int right, int bottom)
{
	DrawBox (pDC, left, top, right, bottom, RGB(236, 233, 216));
	Draw_Line (pDC, left, top, right-1, top, WHITE);
	Draw_Line (pDC, left, top, left, bottom-1, WHITE);
	Draw_Line (pDC, left+1, top+1, right-2, top+1, WHITE);
	Draw_Line (pDC, left+1, top+1, left+1, bottom-2, WHITE);

	Draw_Line (pDC, right, top, right, bottom, RGB(113, 111, 100));
	Draw_Line (pDC, left, bottom, right, bottom, RGB(113, 111, 100));

	Draw_Line (pDC, right-1, top+1, right-1, bottom-1, RGB(172, 168, 153));
	Draw_Line (pDC, left+1, bottom-1, right-1, bottom-1, RGB(113, 111, 100));
}

void Draw::DrawCenterText (CDC* pDC, int x, int y, LPSTR str, COLORREF color)
{
	pDC->SetTextColor (color);
	pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextAlign (TA_CENTER);
	pDC->TextOut(x, y, str);
}

void Draw::DrawCenterText (CDC* pDC, int x, int y, CString str, COLORREF color)
{
	pDC->SetTextColor (color);
	pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextAlign (TA_CENTER);
	pDC->TextOut(x, y, str);
}

void Draw::DrawText (CDC* pDC, int x, int y, LPSTR str, UINT Align, COLORREF color)
{
	pDC->SetTextColor (color);
	pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextAlign (Align);
	pDC->TextOut(x, y, str);
}
void Draw::DrawText2 (CDC* pDC, int x, int y, CString str, UINT Align, COLORREF color)
{
	pDC->SetTextColor (color);
	pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextAlign (Align);
	pDC->TextOut(x, y, str);
}

void Draw::DrawThickFrame (CDC* pDC, int left, int top, int right, int bottom)
{
	CPen	*lOldPen;
	CPen	lSolidPenBlack (PS_SOLID, 1, BLACK);
	CPen	lSolidPenGray (PS_SOLID, 1, GRAY);
	CPen	lSolidPenLtGray (PS_SOLID, 1, LTGRAY);
	CPen	lSolidPenWhite (PS_SOLID, 1, WHITE);
	
	lOldPen = pDC->SelectObject(&lSolidPenBlack);
	pDC->MoveTo(left-3, bottom+3);
	pDC->LineTo(right+3, bottom+3);
	pDC->LineTo(right+3, top-3);
	pDC->SelectObject(lOldPen);

	lOldPen = pDC->SelectObject(&lSolidPenGray);
	pDC->MoveTo(left-2, bottom+2);
	pDC->LineTo(right+2, bottom+2);
	pDC->LineTo(right+2, top-2);
	pDC->SelectObject(lOldPen);

	lOldPen = pDC->SelectObject(&lSolidPenLtGray);
	pDC->MoveTo(left-2, bottom+1);
	pDC->LineTo(left-2, top-2);
	pDC->LineTo(right+1, top-2);
	pDC->LineTo(right+1, bottom+1);
	pDC->LineTo(left-2, bottom+1);
	pDC->SelectObject(lOldPen);
	
	lOldPen = pDC->SelectObject(&lSolidPenWhite);
	pDC->MoveTo(left-1, bottom);
	pDC->LineTo(right, bottom);
	pDC->LineTo(right, top-1);
	pDC->SelectObject(lOldPen);


	lOldPen = pDC->SelectObject(&lSolidPenWhite);
	pDC->MoveTo(left-3, bottom+2);
	pDC->LineTo(left-3, top-3);
	pDC->LineTo(right+2, top-3);
	pDC->SelectObject(lOldPen);

	lOldPen = pDC->SelectObject(&lSolidPenGray);
	pDC->MoveTo(left-1, bottom-1);
	pDC->LineTo(left-1, top-1);
	pDC->LineTo(right, top-1);
	pDC->SelectObject(lOldPen);
}

void Draw::Draw_LineFrame (CDC* pDC, int left, int top, int right, int bottom, int widthL, int widthR)
{
	Draw_Line (pDC, left+widthL, top, left, top, GRAY);
	Draw_Line (pDC, left, top, left, bottom-1, GRAY);
	Draw_Line (pDC, left, bottom-1, right-1, bottom-1, GRAY);
	Draw_Line (pDC, right-1, bottom-1, right-1, top, GRAY);
	Draw_Line (pDC, right-1, top, right-1-widthR, top, GRAY);

	Draw_Line (pDC, left+widthL, top+1, left+1, top+1, WHITE);
	Draw_Line (pDC, left+1, top+1, left+1, bottom, WHITE);
	Draw_Line (pDC, left+1, bottom, right, bottom, WHITE);
	Draw_Line (pDC, right, bottom, right, top+1, WHITE);
	Draw_Line (pDC, right, top+1, right-widthR, top+1, WHITE);
}

void Draw::Draw_Line (CDC* pDC, int sx, int sy, int ex, int ey, COLORREF color)
{
	CPen	*lOldPen;
	CPen	lSolidPenColor (PS_SOLID, 1, color);
	
	lOldPen = pDC->SelectObject(&lSolidPenColor);
	
	pDC->MoveTo(sx, sy);
	pDC->LineTo(ex, ey);
	
	pDC->SelectObject(lOldPen);
}

void Draw::Draw_ThickLine (CDC* pDC, int sx, int sy, int ex, int ey, int thickness, COLORREF color)
{
	CPen	*lOldPen;
	CPen	lSolidPenColor (PS_SOLID, thickness, color);
	
	lOldPen = pDC->SelectObject(&lSolidPenColor);
	
	pDC->MoveTo(sx, sy);
	pDC->LineTo(ex, ey);
	
	pDC->SelectObject(lOldPen);
}

void Draw::Draw_ThickLine (CPaintDC* dc, int sx, int sy, int ex, int ey, int thickness, COLORREF color)
{
	CPen	*lOldPen;
	CPen	lSolidPenColor (PS_SOLID, thickness, color);
	
	lOldPen = dc->SelectObject(&lSolidPenColor);
	
	dc->MoveTo(sx, sy);
	dc->LineTo(ex, ey);
	
	dc->SelectObject(lOldPen);
}

void Draw::Draw_ThickLine_Dot (CPaintDC* dc, int sx, int sy, int ex, int ey, int thickness, COLORREF color)
{
	CPen	*lOldPen;
	CPen	lSolidPenColor (PS_DOT, thickness, color);
	
	lOldPen = dc->SelectObject(&lSolidPenColor);
	
	dc->MoveTo(sx, sy);
	dc->LineTo(ex, ey);
	
	dc->SelectObject(lOldPen);
}

void Draw::Draw_ThickRect (CDC* pDC, int left, int top, int right, int bottom, int thickness, COLORREF color)
{
	CPen	*lOldPen;
	CPen	lSolidPenColor (PS_SOLID, thickness, color);
	
	lOldPen = pDC->SelectObject(&lSolidPenColor);
	
	pDC->MoveTo(left, top);
	pDC->LineTo(right, top);
	pDC->LineTo(right, bottom);
	pDC->LineTo(left, bottom);
	pDC->LineTo(left, top);
	
	pDC->SelectObject(lOldPen);
}

void Draw::Draw_ThickRect (CDC* pDC, CRect R, int thickness, COLORREF color)
{
	CPen	*lOldPen;
	CPen	lSolidPenColor (PS_SOLID, thickness, color);
	
	lOldPen = pDC->SelectObject(&lSolidPenColor);
	
	pDC->MoveTo(R.left, R.top);
	pDC->LineTo(R.right, R.top);
	pDC->LineTo(R.right, R.bottom);
	pDC->LineTo(R.left, R.bottom);
	pDC->LineTo(R.left, R.top);
	
	pDC->SelectObject(lOldPen);
}

void Draw::Draw_Triangle (CPaintDC* dc, int x1, int y1, int x2, int y2, int x3, int y3, COLORREF color)
{
	HBRUSH	hBrush  = CreateSolidBrush(color);
	HBRUSH	hBkBrush = (HBRUSH)SelectObject(dc->GetSafeHdc(), hBrush);
	SelectObject (dc->GetSafeHdc(), GetStockObject( BLACK_PEN ) ); 
	
	POINT pt[3];
	pt[0].x = x1;
	pt[0].y = y1;
	pt[1].x = x2;
	pt[1].y = y2;
	pt[2].x = x3;
	pt[2].y = y3;
	dc->Polygon (pt, 3);
	
	SelectObject (dc->GetSafeHdc(), hBkBrush);
    DeleteObject (hBrush);
}

void Draw::Draw_Circle_Solid (CDC* pDC, double cx, double cy, double R, COLORREF color)
{
	HBRUSH	hBrush  = CreateSolidBrush(color);
	HBRUSH	hBkBrush = (HBRUSH)SelectObject(pDC->GetSafeHdc(), hBrush);
	SelectObject (pDC->GetSafeHdc(), GetStockObject( NULL_PEN ) ); 
	
	Ellipse (pDC->GetSafeHdc(), (int)(cx + R), (int)(cy + R), (int)(cx - R), (int)(cy - R));
	
	SelectObject (pDC->GetSafeHdc(), hBkBrush);
    DeleteObject (hBrush);
}

//±èÁø¿ì
void Draw::Draw_Rgn(CDC *pDC, CPoint *a_arPoint, int a_nPoint, COLORREF color)
{
	CRgn oRgn;
	CBrush oBrush;

	oRgn.CreatePolygonRgn(a_arPoint, a_nPoint, WINDING);

	oBrush.CreateSolidBrush(color);

	HBRUSH	hBkBrush = (HBRUSH)SelectObject(pDC->GetSafeHdc(), oBrush.m_hObject);
	SelectObject(pDC->GetSafeHdc(), GetStockObject( NULL_PEN ) ); 
	
	pDC->FillRgn(&oRgn, &oBrush);

	SelectObject(pDC->GetSafeHdc(), hBkBrush);
    oBrush.DeleteObject();
}
