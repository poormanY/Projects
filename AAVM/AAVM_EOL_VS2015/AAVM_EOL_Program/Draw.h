// Draw.h: interface for the Draw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAW_H__FD3E3B00_D834_11D4_BD28_00001CDB85EC__INCLUDED_)
#define AFX_DRAW_H__FD3E3B00_D834_11D4_BD28_00001CDB85EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define HIMETRIC_INCH        2540

#if 0
private:
	// 이미지 객체의 포인터를 저장할 변수
	LPPICTURE mp_image_object;
	// 읽어들인 이미지의 폭과 높이에 대한 크기 정보
	long m_width_size, m_height_size;
#endif
class Draw  
{
public:
	Draw();
	virtual ~Draw();

	void DrawBox (CDC* pDC, int left, int top, int right, int bottom, COLORREF color);
	void Draw::DrawBox (CDC* pDC, CRect R, COLORREF color);
	void DrawConcaveBox (CDC* pDC, int left, int top, int right, int bottom);
	void DrawConvexBox (CDC* pDC, int left, int top, int right, int bottom);
	void DrawWhiteConvexBox (CDC* pDC, int left, int top, int right, int bottom);
	void DrawCenterText (CDC* pDC, int x, int y, LPSTR str, COLORREF color);
	void DrawCenterText (CDC* pDC, int x, int y, CString str, COLORREF color);
	void DrawText (CDC* pDC, int x, int y, LPSTR str, UINT Align, COLORREF color);
	void DrawText2 (CDC* pDC, int x, int y, CString str, UINT Align, COLORREF color);
	void DrawThickFrame (CDC* pDC, int left, int top, int right, int bottom);
	void Draw_LineFrame (CDC* pDC, int left, int top, int right, int bottom, int widthL, int widthR);
	void Draw_Line (CDC* pDC, int sx, int sy, int ex, int ey, COLORREF color);
	void Draw_ThickLine (CDC* pDC, int sx, int sy, int ex, int ey, int thickness, COLORREF color);
	void Draw_ThickLine (CPaintDC* dc, int sx, int sy, int ex, int ey, int thickness, COLORREF color);
	void Draw_ThickLine_Dot (CPaintDC* dc, int sx, int sy, int ex, int ey, int thickness, COLORREF color);
	void Draw_Rect (CDC* pDC, CRect R, COLORREF color);
	void Draw_ThickRect (CDC* pDC, int left, int top, int right, int bottom, int thickness, COLORREF color);
	void Draw::Draw_ThickRect (CDC* pDC, CRect R, int thickness, COLORREF color);
	void Draw_Circle_Solid (CDC* pDC, double cx, double cy, double R, COLORREF color);
	void Draw_Triangle (CPaintDC* dc, int x1, int y1, int x2, int y2, int x3, int y3, COLORREF color);

	void Draw::Draw_Rgn(CDC *pDC, CPoint *a_arPoint, int a_nPoint, COLORREF color);

};

#endif // !defined(AFX_DRAW_H__FD3E3B00_D834_11D4_BD28_00001CDB85EC__INCLUDED_)
