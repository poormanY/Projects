#ifndef Y_DRAW_H
#define Y_DRAW_H

class CDraw
{
public:
	CDraw();
	~CDraw();

	//--------- 외부 환경 변수 ------------------------------------//

	//--------- 외부 사용 함수 ------------------------------------//
	void DrawBox (CDC* pDC, int left, int top, int right, int bottom, COLORREF color);
	void DrawConcaveBox (CDC* pDC, int left, int top, int right, int bottom);
	void DrawConvexBox (CDC* pDC, int left, int top, int right, int bottom);
	void DrawWhiteConvexBox (CDC* pDC, int left, int top, int right, int bottom);
	//void DrawCenterText (CDC* pDC, int x, int y, LPSTR str, COLORREF color);
	void DrawCenterText (CDC* pDC, int x, int y, CString str, COLORREF color);
	//void DrawText (CDC* pDC, int x, int y, LPSTR str, UINT Align, COLORREF color);
	void DrawText (CDC* pDC, int x, int y, CString str, UINT Align, COLORREF color);
	void DrawThickFrame (CDC* pDC, int left, int top, int right, int bottom);
	void Draw_LineFrame (CDC* pDC, int left, int top, int right, int bottom, int widthL, int widthR);
	void Draw_Line (CDC* pDC, int sx, int sy, int ex, int ey, COLORREF color);
	void Draw_ThickLine (CDC* pDC, int sx, int sy, int ex, int ey, int thickness, COLORREF color);
	void Draw_ThickLine (CPaintDC* dc, int sx, int sy, int ex, int ey, int thickness, COLORREF color);
	void Draw_ThickLine_Dot (CPaintDC* dc, int sx, int sy, int ex, int ey, int thickness, COLORREF color);
	void Draw_Rect (CDC* pDC, CRect R, COLORREF color);
	void Draw_ThickRect (CDC* pDC, int left, int top, int right, int bottom, int thickness, COLORREF color);
	void Draw_ThickRect (CDC* pDC, CRect R, int thickness, COLORREF color);
	void Draw_Circle_Solid (CDC* pDC, double cx, double cy, double R, COLORREF color);
	void Draw_Triangle (CDC* pDC, int x1, int y1, int x2, int y2, int x3, int y3, COLORREF color);

	void Draw_Rgn(CDC *pDC, CPoint *a_arPoint, int a_nPoint, COLORREF color);
protected:
	//--------- 내부 환경 변수 ------------------------------------//

	//--------- 내부 사용 함수 ------------------------------------//
	
};





#endif //  !Y_DRAW_H  //