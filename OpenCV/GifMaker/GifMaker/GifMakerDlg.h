
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
	int AreaFlag;
	int m_nRecFb;
	int m_nFbMax;
	CRect m_recRec;
	CString m_strRecFilePath;

	void SaveRecordingImage_Start();
	void SaveRecordingImage_Execute();
	void SaveRecordingImage_End();

	bool SaveCDCtoBMP(CDC* pDC, CRect BitmapSize, int BitCount, const char *filename);
	
	void CalibrateCaputreArea();
	void SaveCaptureArea(CRect area, CString strFilePath);
	void SetCaptureArea(CRect area);
	void GetCaptureArea(CRect *area);
	void GetRectSize(CRect ret, CSize *pSize);

	/////////////////////////////////////////////////////
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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonAreaDetail();
	afx_msg void OnBnClickedButtonAreaSetting();
	CEdit m_edit_rec_fps;
	CEdit m_edit_rec_time;
	afx_msg void OnBnClickedButtonRec();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CButton m_btn_rec;
};
