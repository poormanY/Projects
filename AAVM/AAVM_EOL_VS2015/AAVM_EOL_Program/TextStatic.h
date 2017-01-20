#if !defined(AFX_TEXTSTATIC_H__B7B1C5D9_9DB0_4A77_A08C_6F0AEA9FE86F__INCLUDED_)
#define AFX_TEXTSTATIC_H__B7B1C5D9_9DB0_4A77_A08C_6F0AEA9FE86F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextStatic window

class CTextStatic : public CStatic
{
// Construction
public:
	CTextStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetTextFormat(UINT a_uFormat);
	void SetTextColor(COLORREF a_Color);
	void SetBgColor(COLORREF a_Color);
	void SetLogFont(LOGFONT *a_pLogFont);
	CString m_strMsg;
	virtual ~CTextStatic();

	// Generated message map functions
protected:
	BOOL m_tFontInstalled;
	LOGFONT m_LogFont;
	int GetMsgLines();
	UINT m_nFontFormat;
	CFont m_Font;
	COLORREF m_ColorTxt;
   COLORREF m_ColorBg;

	//{{AFX_MSG(CTextStatic)
	afx_msg void OnPaint();
    afx_msg LRESULT OnThreadMsgUpdate(WPARAM a_wParam, LPARAM a_lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTSTATIC_H__B7B1C5D9_9DB0_4A77_A08C_6F0AEA9FE86F__INCLUDED_)
