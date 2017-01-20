// TextStatic.cpp : implementation file
//

#include "stdafx.h"
#include "MainApp.h"
#include "TextStatic.h"
#include "wm_user.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextStatic

CTextStatic::CTextStatic()
{
   m_strMsg = "";
   m_ColorBg  = RGB(255,255,255);
   m_ColorTxt = RGB(  0,  0,  0);
   m_nFontFormat = DT_CENTER;
   m_tFontInstalled = FALSE;
}

CTextStatic::~CTextStatic()
{
}


BEGIN_MESSAGE_MAP(CTextStatic, CStatic)
	//{{AFX_MSG_MAP(CTextStatic)
	ON_WM_PAINT()
    ON_MESSAGE(_WM_THREAD_UPDATE, OnThreadMsgUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextStatic message handlers

void CTextStatic::OnPaint() 
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

LRESULT CTextStatic::OnThreadMsgUpdate(WPARAM a_wParam, LPARAM a_lParam)
{
    this->Invalidate(TRUE);
    UpdateData(FALSE);
	
	return 0;
}

void CTextStatic::SetLogFont(LOGFONT *a_pLogFont)
{
   memcpy(&m_LogFont, a_pLogFont, sizeof(LOGFONT));

   if (m_tFontInstalled != FALSE)
   {
      m_Font.DeleteObject();
   }
   m_Font.CreateFontIndirect(&m_LogFont);
   m_tFontInstalled = TRUE;
}

void CTextStatic::SetBgColor(COLORREF a_Color)
{
   m_ColorBg = a_Color;
}

void CTextStatic::SetTextColor(COLORREF a_Color)
{
   m_ColorTxt = a_Color;
}

void CTextStatic::SetTextFormat(UINT a_uFormat)
{
   m_nFontFormat = a_uFormat;
}

int CTextStatic::GetMsgLines()
{
   int nResult = 0;
   const char *pszMsg = (LPCTSTR)m_strMsg;

   if (*pszMsg != 0) nResult++;

   while (*pszMsg != 0)
   {
      if (*pszMsg++ == '\n') nResult++;
   }

   return nResult;
}
