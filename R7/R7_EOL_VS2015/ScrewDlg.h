#pragma once


// ScrewDlg 대화 상자입니다.

class ScrewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ScrewDlg)

public:
	ScrewDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~ScrewDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SCREW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
