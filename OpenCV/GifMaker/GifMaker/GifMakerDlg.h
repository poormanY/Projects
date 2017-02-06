
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
	CRect m_recRec;
	CString m_strRecFilePath;

	void SaveRecordingImage_Start();
	void SaveRecordingImage_Execute();
	void SaveRecordingImage_End();

	bool SaveCDCtoBMP(CDC* pDC, CRect BitmapSize, int BitCount, const char *filename);
	
	void CalibrateCaputreArea();
	void SaveCaptureArea(CRect area, CString strFilePath);
	void SetCaptureArea(CRect area);
	void SetAutoCaptureArea(void);
	void GetCaptureArea(CRect *area);
	void GetRectSize(CRect ret, CSize *pSize);
		
	void SetCaptureSize(void);
	void SetCaptureAreaTransparant();
	
	// Capture
	int m_nCaptureNum;
	int m_nRecNum;
	int m_nFb, m_nFbMax;
	void SaveCaptureImage();
	void SaveRecImage();

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
	BOOL SetHook();
	BOOL UnHook();
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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
	CEdit m_edit_size;
	afx_msg void OnEnChangeEditLeft();
	afx_msg void OnEnChangeEditRight();
	afx_msg void OnEnChangeEditTop();
	afx_msg void OnEnChangeEditBottom();
	afx_msg void OnMove(int x, int y);
	CEdit m_edit_file_path;
	afx_msg void OnBnClickedButtonFileName();
	afx_msg void OnBnClickedButtonGif();
};
