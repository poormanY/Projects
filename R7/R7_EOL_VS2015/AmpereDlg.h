#pragma once
#include "afxwin.h"

// AmpereDlg dialog

class AmpereDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AmpereDlg)

public:
	AmpereDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~AmpereDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_AMPERE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void OpenAmpere();
	void CloseAmpere();
	int DoAmpere();

	afx_msg void OnBnClickedButtonConnect();
	CEdit m_edit_comport;
	CEdit m_edit_min;
	CEdit m_edit_max;
	CEdit m_edit_value;
	CEdit m_edit_result;
	virtual BOOL OnInitDialog();
	CButton m_button_connect;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
