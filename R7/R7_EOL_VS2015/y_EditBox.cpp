// Edit Box Tool Function

#include "stdafx.h"
#include "y_EditBox.h"
#include "y_wm_user.h"

//--- 클래스 생성자
CEditBox::CEditBox()
{
	//--> 초기상태
	m_strMsg = "";
	m_ColorBg  = RGB(255,255,255);
	m_ColorTxt = RGB(  0,  0,  0);
	m_nFontFormat = DT_CENTER;
	m_tFontInstalled = FALSE;
}

CEditBox::~CEditBox()
{
	
}

BEGIN_MESSAGE_MAP(CEditBox, CStatic)
	//{{AFX_MSG_MAP(CTextStatic)
	ON_WM_PAINT()
    ON_MESSAGE(_WM_THREAD_UPDATE, (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM)) OnThreadMsgUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CEditBox::OnPaint()
{
	CPaintDC dc(this); // device context for paintin

	CRect oRect;
	GetClientRect(&oRect);

	/////// Fill Background Rectangle ///////
	// Use NULL_PEN
	dc.SelectStockObject(NULL_PEN);

	// Use SolidBrush
	CBrush oBrush, *pBrushOld;
	oBrush.CreateSolidBrush(m_ColorBg);
	pBrushOld = dc.SelectObject(&oBrush);

	// Draw Rectangle
	dc.Rectangle(&oRect);

	// End of Fill Background Rectangle //////
	dc.SelectObject(pBrushOld);

	// Font
	if (m_tFontInstalled == FALSE)
	{
		GetFont()->GetLogFont(&m_LogFont);
		m_Font.CreateFontIndirect(&m_LogFont);
		m_tFontInstalled = TRUE;
	}

	CFont *pFontOld;
	pFontOld = dc.SelectObject(&m_Font);

	dc.SetTextColor(m_ColorTxt);

	dc.SetBkMode(TRANSPARENT);

	oRect.left   += 2;
	oRect.right  -= 2;
	oRect.top    += 2;
	oRect.bottom -= 2;

	if (m_nFontFormat & DT_VCENTER)
	{
		int iTextHeight = GetMsgLines() * abs(m_LogFont.lfHeight);
		oRect.top    += (oRect.Height() - iTextHeight) / 2;
	}

	dc.DrawText(m_strMsg, &oRect, m_nFontFormat);

	dc.SelectObject(pFontOld);
   
	// Do not call CStatic::OnPaint() for painting messages
}
LRESULT CEditBox::OnThreadMsgUpdate(WPARAM a_wParam, LPARAM a_lParam)
{
	this->Invalidate(TRUE);	// 화면 갱신
    UpdateData(FALSE);

	return 0;
}

void CEditBox::SetTextFormat(UINT a_uFormat)
{
	m_nFontFormat = a_uFormat;
}
void CEditBox::SetLogFont(LOGFONT *a_pLogFont)
{
   memcpy(&m_LogFont, a_pLogFont, sizeof(LOGFONT));

   if (m_tFontInstalled != FALSE)
   {
      m_Font.DeleteObject();
   }
   m_Font.CreateFontIndirect(&m_LogFont);
   m_tFontInstalled = TRUE;
}
void CEditBox::SetTextColor(COLORREF a_Color)
{
	m_ColorTxt = a_Color;
}
void CEditBox::SetBgColor(COLORREF a_Color)
{
	m_ColorBg = a_Color;
}
void CEditBox::SetTextString(CString a_String)
{
	m_strMsg = a_String;
}
void CEditBox::SetText(COLORREF a_BgColor, COLORREF a_TextColor, CString a_StrText, BOOL a_UpdateMsg)
{
	m_ColorBg  = a_BgColor;
	m_ColorTxt = a_TextColor;
	m_strMsg   = a_StrText;

	if ( a_UpdateMsg == UPDATE_ON )
	{
		PostMessageW(_WM_THREAD_UPDATE);
	}
}

int CEditBox::GetMsgLines()
{
   int nMsgLines = 0;
   const wchar_t *pszMsg = (LPCTSTR)m_strMsg;

   if (*pszMsg != 0) nMsgLines++;

   while (*pszMsg != 0)
   {
      if (*pszMsg++ == '\n') nMsgLines++;
   }

   return nMsgLines;
}
