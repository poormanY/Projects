
// GifMakerDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CGifMakerDlg dialog
class CGifMakerDlg : public CDialogEx
{
// Construction
public:
	CGifMakerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GIFMAKER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	bool SaveCDCtoBMP(CDC* pDC, CRect BitmapSize, int BitCount, const char *filename);

	void GetCaptureArea(CRect *pCaptureRect);
	void GetRectSize(CRect ret, CSize *pSize);

	CComboBox m_combo_monitor_type;
	afx_msg void OnBnClickedButtonCapture();
	CStatic m_pic_screen;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnBnClickedButtonClear();
	CEdit m_edit_left;
	CEdit m_edit_top;
	CEdit m_edit_right;
	CEdit m_edit_bottom;
	afx_msg void OnCbnSelchangeComboMonitorType();
	afx_msg void OnBnClickedButtonSave();
};
