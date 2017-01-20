#if !defined(AFX_EXPRESSIMG_H__B39A0463_D462_487E_91CD_5BA06AD413AF__INCLUDED_)
#define AFX_EXPRESSIMG_H__B39A0463_D462_487E_91CD_5BA06AD413AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExpressImg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExpressImg dialog

class CExpressImg : public CDialog
{
// Construction
public:
	int nPreImgView;
	int nCurrPos;
	void GetDisplaySpace(CRect &rt);
	CRect rtSubScreenSize;
	CRect rtWholeScreenSize;	//mook
	void DrawLine(CDC *pDC, int PartNum);
	CBitmap* m_pbmBack;
	CExpressImg(CWnd* pParent = NULL);   // standard constructor

	afx_msg void OnPaint();
	void CExpressImg::DrawLine_Sub (CDC *BufferDC, int GapMovement);
    void CExpressImg::RearLght_Sub (CDC *BufferDC, int bLightOn, int bDark);
    void CExpressImg::DrawLine_AS_Calibration (CDC *BufferDC);

// Dialog Data
	//{{AFX_DATA(CExpressImg)
	enum { IDD = IDD_EXPRESS_IMG_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExpressImg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExpressImg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPRESSIMG_H__B39A0463_D462_487E_91CD_5BA06AD413AF__INCLUDED_)
