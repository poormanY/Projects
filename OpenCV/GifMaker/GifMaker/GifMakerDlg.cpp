
// GifMakerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GifMaker.h"
#include "GifMakerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static CRect* MonitorsRect = NULL;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGifMakerDlg dialog



CGifMakerDlg::CGifMakerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GIFMAKER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGifMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MONITOR_TYPE, m_combo_monitor_type);
	DDX_Control(pDX, IDC_PIC_SCREEN, m_pic_screen);
	DDX_Control(pDX, IDC_EDIT_LEFT, m_edit_left);
	DDX_Control(pDX, IDC_EDIT_TOP, m_edit_top);
	DDX_Control(pDX, IDC_EDIT_RIGHT, m_edit_right);
	DDX_Control(pDX, IDC_EDIT_BOTTOM, m_edit_bottom);
}

BEGIN_MESSAGE_MAP(CGifMakerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CAPTURE, &CGifMakerDlg::OnBnClickedButtonCapture)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CGifMakerDlg::OnBnClickedButtonClear)
	ON_CBN_SELCHANGE(IDC_COMBO_MONITOR_TYPE, &CGifMakerDlg::OnCbnSelchangeComboMonitorType)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CGifMakerDlg::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CGifMakerDlg message handlers

BOOL CGifMakerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	// get monitors info
	DISPLAY_DEVICE dd;
	DEVMODE dm;
	int monitor_num = ::GetSystemMetrics(SM_CMONITORS);	// get connected the number of monitors
	int deviceIdx, monitorIdx;

	MonitorsRect = new CRect[monitor_num];
	for (deviceIdx = 0, monitorIdx = 0; monitorIdx < monitor_num; deviceIdx++)
	{
		memset(&dd, 0, sizeof(DISPLAY_DEVICE));
		dd.cb = sizeof(DISPLAY_DEVICE);
		// get display information
		EnumDisplayDevices(NULL, deviceIdx, &dd, 0);
		memset(&dm, 0, sizeof(DEVMODE));
		// get display setting value
		if (EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm))
		{
			MonitorsRect[monitorIdx].left = dm.dmPosition.x;
			MonitorsRect[monitorIdx].top = dm.dmPosition.y;
			MonitorsRect[monitorIdx].right = dm.dmPosition.x + dm.dmPelsWidth;
			MonitorsRect[monitorIdx].bottom = dm.dmPosition.y + dm.dmPelsHeight;
			monitorIdx++;
		}
	}

	// Monitor Type  Combo Box
	int comboIdx;
	CString strMonitorType;
	for (comboIdx = 1; comboIdx <= monitor_num; comboIdx++)
	{
		strMonitorType.Format(_T("Monitor %d"), comboIdx);
		m_combo_monitor_type.AddString(strMonitorType);
	}
	m_combo_monitor_type.SetCurSel(0);
	
	// set cpature area - default
	OnCbnSelchangeComboMonitorType();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGifMakerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGifMakerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGifMakerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CGifMakerDlg::SaveCDCtoBMP(CDC* pDC, CRect BitmapSize, int BitCount, const char *filename)
{
	CDC memDC;
	CBitmap Bitmap, *pOldBmp;

	HANDLE handle;
	HANDLE hDIB;
	HPALETTE hPal = NULL;

	BITMAP bm;
	BITMAPINFOHEADER bi;
	LPBITMAPINFOHEADER lpbi;

	DWORD dwLen;

	/*----- CDC의 내용을 Bitmap으로 전송 ----*/
	memDC.CreateCompatibleDC(pDC);
	Bitmap.CreateCompatibleBitmap(pDC, BitmapSize.Width(), BitmapSize.Height());
	pOldBmp = (CBitmap*)memDC.SelectObject(&Bitmap);
	memDC.BitBlt(0, 0, BitmapSize.Width(), BitmapSize.Height(), pDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBmp);

	/*------------------------- 비트멥 헤더를 기록함 -------------------------*/
	if (hPal == NULL)
		hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);

	GetObject(HBITMAP(Bitmap), sizeof(BITMAP), &bm);

	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bm.bmWidth;
	bi.biHeight = bm.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = BitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = bm.bmWidth * bm.bmHeight * 3;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	int nColors = (1 << bi.biBitCount);
	if (nColors > 256)
		nColors = 0;

	dwLen = bi.biSize + nColors * sizeof(RGBQUAD);

	hPal = SelectPalette(pDC->GetSafeHdc(), hPal, FALSE);

	RealizePalette(pDC->GetSafeHdc());

	hDIB = GlobalAlloc(GMEM_FIXED, dwLen);

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	*lpbi = bi;

	GetDIBits(pDC->GetSafeHdc(), HBITMAP(Bitmap), 0, (DWORD)bi.biHeight,
		(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;

	if (bi.biSizeImage == 0)
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) * bi.biHeight;

	dwLen += bi.biSizeImage;

	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
		hDIB = handle;

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	GetDIBits(pDC->GetSafeHdc(), HBITMAP(Bitmap), 0, (DWORD)bi.biHeight,
		(LPBYTE)lpbi + (bi.biSize + nColors * sizeof(RGBQUAD)),
		(LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	BITMAPFILEHEADER hdr;

	hdr.bfType = ((WORD)('M' << 8) | 'B');
	hdr.bfSize = (DWORD)(GlobalSize(hDIB) + sizeof(hdr));
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;
	hdr.bfOffBits = (DWORD)(sizeof(hdr) + lpbi->biSize + nColors * sizeof(RGBQUAD));

	char* pBmpBuf;
	DWORD FileSize;
	FileSize = (DWORD)(sizeof(hdr) + GlobalSize(hDIB));
	pBmpBuf = new char[FileSize];

	memcpy(pBmpBuf, &hdr, sizeof(hdr));
	memcpy(pBmpBuf + sizeof(hdr), lpbi, GlobalSize(hDIB));

	/*--------------------- 실제 파일에 기록함 --------------------------*/
	FILE *pFile = NULL;

	fopen_s(&pFile, filename, "wb");

	bool bResult = false;
	if (pFile != NULL)
	{
		fwrite(pBmpBuf, FileSize, 1, pFile);
		fclose(pFile);
		bResult = true;
	}

	delete[] pBmpBuf;

	if (hDIB)
		GlobalFree(hDIB);

	SelectPalette(pDC->GetSafeHdc(), hPal, FALSE);

	return bResult;
}

void CGifMakerDlg::GetCaptureArea(CRect *pCaptureRect)
{
	CString strRectL, strRectT, strRectR, strRectB;

	m_edit_left.GetWindowTextW(strRectL);
	m_edit_top.GetWindowTextW(strRectT);
	m_edit_right.GetWindowTextW(strRectR);
	m_edit_bottom.GetWindowTextW(strRectB);

	pCaptureRect->left = _ttoi(strRectL);
	pCaptureRect->top = _ttoi(strRectT);
	pCaptureRect->right = _ttoi(strRectR);
	pCaptureRect->bottom = _ttoi(strRectB);
}

void CGifMakerDlg::GetRectSize(CRect ret, CSize *pSize)
{
	pSize->cx = ret.right - ret.left;
	pSize->cy = ret.bottom - ret.top;
}

void CGifMakerDlg::OnBnClickedButtonCapture()
{
	// TODO: Add your control notification handler code here
	CDC *pDc = m_pic_screen.GetWindowDC();	// picture control (draw main)
	CClientDC ScreenDC(GetDesktopWindow());	// screen dc
	CDC memDC;								// memory dc
	memDC.CreateCompatibleDC(&ScreenDC);	// memory dc compatible with screen dc
	
	CRect captureRect, pictureRect;
	CSize captureSize, pictureSize;
	GetCaptureArea(&captureRect);							// get capture area
	GetRectSize(captureRect, &captureSize);					// get capture size
	m_pic_screen.GetWindowRect(&pictureRect);				// get picture control position
	GetRectSize(pictureRect, &pictureSize);					// get picture control size

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&ScreenDC, captureSize.cx, captureSize.cy);	// create bitmap compatible with screen dc
	CBitmap *pOldBitmap = memDC.SelectObject(&bitmap);		// bitmap point
	// copy screenDC (x,y)~(x+cx,y+cy) to memDC (0,0)~(cx,cy)
	memDC.BitBlt(0, 0, captureSize.cx, captureSize.cy, &ScreenDC, captureRect.left, captureRect.top, SRCCOPY);
	// copy memDC (0,0)~(cx,cy) to pDc (0,0)~(pictureCx, pictureCy)
	pDc->StretchBlt(0, 0, pictureSize.cx, pictureSize.cy, &memDC, 0, 0, captureSize.cx, captureSize.cy, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
}

void CGifMakerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	static SIZE size = { cx,cy };	//변경전 사이즈 저장
	SIZE changeSize;				//변경된 사이즈
	changeSize.cx = cx - size.cx;
	changeSize.cy = cy - size.cy;

	RECT rect;
	POINT pt;
	SIZE sz;
	if (m_pic_screen.GetSafeHwnd())
	{
		m_pic_screen.GetWindowRect(&rect); // x,y 모니터 절대 위치
		ScreenToClient(&rect);             // x,y 다이얼로그 박스내 상대 위치

		pt.x = rect.left;
		pt.y = rect.top;
		sz.cx = (rect.right - rect.left);
		sz.cy = (rect.bottom - rect.top);

		// x,y 위치 고정, x,y 사이즈 변경
		m_pic_screen.SetWindowPos(NULL, 0, 0, (sz.cx + (changeSize.cx)), (sz.cy + changeSize.cy), SWP_NOMOVE);
		OnBnClickedButtonClear();
	}
	
	//변경 되는 사이즈를 알기위해 이전 사이즈 저장
	size.cx = cx;
	size.cy = cy;
}

void CGifMakerDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnGetMinMaxInfo(lpMMI);

	lpMMI->ptMinTrackSize = CPoint(350, 300);

}

void CGifMakerDlg::OnBnClickedButtonClear()
{
	// TODO: Add your control notification handler code here
	CDC *pDc = m_pic_screen.GetWindowDC();	// picture control
	CRect rect;
	GetClientRect(rect);
	pDc->FillSolidRect(rect, RGB(255, 255, 255));
}

void CGifMakerDlg::OnCbnSelchangeComboMonitorType()
{
	// TODO: Add your control notification handler code here
	// get main monitor info
	CRect mainRect;
	CString strRectL, strRectT, strRectR, strRectB;
	int monitor_type = m_combo_monitor_type.GetCurSel();	// get monitor type
		
	mainRect.left = MonitorsRect[monitor_type].left;
	mainRect.top = MonitorsRect[monitor_type].top;
	mainRect.right = MonitorsRect[monitor_type].right;
	mainRect.bottom = MonitorsRect[monitor_type].bottom;
	
	strRectL.Format(_T("%d"), mainRect.left);
	strRectT.Format(_T("%d"), mainRect.top);
	strRectR.Format(_T("%d"), mainRect.right);
	strRectB.Format(_T("%d"), mainRect.bottom);
	// set cpature area - default
	m_edit_left.SetWindowTextW(strRectL);
	m_edit_top.SetWindowTextW(strRectT);
	m_edit_right.SetWindowTextW(strRectR);
	m_edit_bottom.SetWindowTextW(strRectB);
}


void CGifMakerDlg::OnBnClickedButtonSave()
{
	// TODO: Add your control notification handler code here
	CClientDC ScreenDC(GetDesktopWindow());	// screen dc
	CDC memDC;								// memory dc
	memDC.CreateCompatibleDC(&ScreenDC);	// memory dc compatible with screen dc

	CRect captureRect;
	CSize captureSize;
	GetCaptureArea(&captureRect);							// get capture area
	GetRectSize(captureRect, &captureSize);					// get capture size

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&ScreenDC, captureSize.cx, captureSize.cy);	// create bitmap compatible with screen dc
	CBitmap *pOldBitmap = memDC.SelectObject(&bitmap);		// bitmap point
	// copy screenDC (x,y)~(x+cx,y+cy) to memDC (0,0)~(cx,cy)
	memDC.BitBlt(0, 0, captureSize.cx, captureSize.cy, &ScreenDC, captureRect.left, captureRect.top, SRCCOPY);

	CString szFilter = _T("Image (*.BMP, *.PNG, *.JPG) | *.BMP;*.PNG;*.JPG | All Files(*.*)|*.*||");
	
	// FALSE -> save dialog / TRUE -> load dialog
	CFileDialog fsave_dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, szFilter);
	// select ok button
	if (fsave_dlg.DoModal() == IDOK)
	{
		CString strFp = fsave_dlg.GetPathName();	// file path
		CString strEx = strFp.Right(4);		// file extension
		// if compare result is equal, return 0
		if (strEx.Compare(_T(".bmp")) && strEx.Compare(_T(".BMP")))
		{
			strFp += _T(".bmp");	// add bitmap extension
		}

		char *fp = new char[strFp.GetLength()+1];
		strcpy_s(fp, strFp.GetLength()+1, CT2A(strFp));
		
		// save 24bit bmp file
		SaveCDCtoBMP(&memDC, captureRect, 24, fp);
		
		delete[] fp;
		fp = NULL;
	}
	
	memDC.SelectObject(pOldBitmap);
}
