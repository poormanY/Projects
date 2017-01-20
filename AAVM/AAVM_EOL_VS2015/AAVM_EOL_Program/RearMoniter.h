
class CRearImg : public CDialog
{
	// Construction
public:

	void DrawImage(CDC *pDC);

	int RearDraw(CString Name, unsigned int StartNumber, unsigned int StopNumber);
	CRearImg(CWnd* pParent = NULL);   // standard constructor

	afx_msg void OnPaint();

	// Dialog Data
	//{{AFX_DATA(CExpressImg)
	enum { IDD = IDD_Rear_Image };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExpressImg)
public:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExpressImg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

