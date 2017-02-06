
// GifMakerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GifMaker.h"
#include "GifMakerDlg.h"
#include "afxdialogex.h"
#include "PictureEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////
// define
// Transparant color
#define RGB_TRANSPARANT			RGB(123, 132, 231)
// Eye Soft color
#define SOFT_GREEN              RGB(163,204,163)
#define SOFT_LTGREEN            RGB(181,214,146)
#define SOFT_BLUE               RGB(214,230,245)

#define BLACK					RGB(0, 0, 0)
#define WHITE					RGB(255, 255, 255)
#define GREEN					RGB(0, 255, 0)
#define RED						RGB(255, 0, 0)
#define BLUE					RGB(0, 0, 255)

// Monitor Type
#define TYPE_CUSTOMIZING		_T("Customizing")

// Command
#define F5_IMAGE_START			_T("F5 REC")
#define F5_IMAGE_WAIT			_T("F5 Waiting")
#define F5_IMAGE_STOP			_T("F5 Stop")

// File Path
#define FILE_PATH_DEFAULT		_T("C:\\Users\\YSW\\Pictures\\Capture")

///////////////////////////////////////////////////////////////////
// global variable
HHOOK g_hook = NULL;

///////////////////////////////////////////////////////////////////
// static variable
enum _AreaFlag
{
	AreaActiveOff = 0,
	AreaActiveOn,
	AreaSelecting,
	AreaSelectComplete
};

enum _TimerID
{
	TID_REC
};

static CRect* MonitorsRect = NULL;
static BOOL isDlgInit = FALSE;
static const CPoint DlgMinSize = CPoint(300, 100);



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
	DDX_Control(pDX, IDC_EDIT_REC_FPS, m_edit_rec_fps);
	DDX_Control(pDX, IDC_EDIT_REC_TIME, m_edit_rec_time);
	DDX_Control(pDX, IDC_BUTTON_REC, m_btn_rec);
	DDX_Control(pDX, IDC_EDIT_SIZE, m_edit_size);
	DDX_Control(pDX, IDC_EDIT_FILE_PATH, m_edit_file_path);
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
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_AREA_DETAIL, &CGifMakerDlg::OnBnClickedButtonAreaDetail)
	ON_BN_CLICKED(IDC_BUTTON_AREA_SETTING, &CGifMakerDlg::OnBnClickedButtonAreaSetting)
	ON_BN_CLICKED(IDC_BUTTON_REC, &CGifMakerDlg::OnBnClickedButtonRec)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EDIT_LEFT, &CGifMakerDlg::OnEnChangeEditLeft)
	ON_EN_CHANGE(IDC_EDIT_RIGHT, &CGifMakerDlg::OnEnChangeEditRight)
	ON_EN_CHANGE(IDC_EDIT_TOP, &CGifMakerDlg::OnEnChangeEditTop)
	ON_EN_CHANGE(IDC_EDIT_BOTTOM, &CGifMakerDlg::OnEnChangeEditBottom)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON_FILE_NAME, &CGifMakerDlg::OnBnClickedButtonFileName)
	ON_BN_CLICKED(IDC_BUTTON_GIF, &CGifMakerDlg::OnBnClickedButtonGif)
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

	// add GWL_EXSTYLE for tranparant image
	LONG lResult = SetWindowLong(this->GetSafeHwnd()
		, GWL_EXSTYLE
		, GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(RGB_TRANSPARANT, 255, LWA_COLORKEY | LWA_ALPHA);

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

	// set file path
	m_edit_file_path.SetWindowTextW(FILE_PATH_DEFAULT);
	m_nCaptureNum = 1;
	m_nRecNum = 1;

	// Monitor Type  Combo Box
	int comboIdx;
	CString strMonitorType;
	for (comboIdx = 1; comboIdx <= monitor_num; comboIdx++)
	{
		strMonitorType.Format(_T("Monitor %d"), comboIdx);
		m_combo_monitor_type.AddString(strMonitorType);
	}
	m_combo_monitor_type.AddString(TYPE_CUSTOMIZING);
	m_combo_monitor_type.SetCurSel(0);
	// set capture area - default
	OnCbnSelchangeComboMonitorType();
	// set capture size
	SetCaptureSize();
	// set capture area - transparant
	SetCaptureAreaTransparant();

	// set recording info - FPS, Times
	m_edit_rec_fps.SetWindowTextW(_T("2"));
	m_edit_rec_time.SetWindowTextW(_T("5"));

	// F5 REC BUTTON
	m_btn_rec.SetWindowTextW(F5_IMAGE_START);

	// Set Hook
	SetHook();

	// Set Dialog init variable
	isDlgInit = TRUE;

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

void CGifMakerDlg::SetCaptureSize()
{
	CRect area;
	CSize size;
	CString strSizeX, strSizeY, strSize;

	//CalibrateCaputreArea();			// calibrate capture area
	GetCaptureArea(&area);				// get capture area
	GetRectSize(area, &size);			// get capture size
	
	strSizeX.Format(_T("%d"), size.cx);
	strSizeY.Format(_T("%d"), size.cy);
	strSize = strSizeX + _T(" X ") + strSizeY;

	m_edit_size.SetWindowTextW(strSize);
}

void CGifMakerDlg::SetAutoCaptureArea()
{
	CStatic *staticSize = (CStatic *)GetDlgItem(IDC_PIC_SCREEN);
	CRect rect;
	staticSize->GetClientRect(rect);	// get picture control rect
	staticSize->ClientToScreen(rect);	// convert coordinate from window client to monitor screen
	SetCaptureArea(rect);
}

void CGifMakerDlg::SetCaptureArea(CRect area)
{
	CString strRectL, strRectT, strRectR, strRectB;

	strRectL.Format(_T("%d"), area.left);
	strRectT.Format(_T("%d"), area.top);
	strRectR.Format(_T("%d"), area.right);
	strRectB.Format(_T("%d"), area.bottom);
	// set cpature area - default
	m_edit_left.SetWindowTextW(strRectL);
	m_edit_top.SetWindowTextW(strRectT);
	m_edit_right.SetWindowTextW(strRectR);
	m_edit_bottom.SetWindowTextW(strRectB);
}

void CGifMakerDlg::GetCaptureArea(CRect *area)
{
	CString strRectL, strRectT, strRectR, strRectB;

	m_edit_left.GetWindowTextW(strRectL);
	m_edit_top.GetWindowTextW(strRectT);
	m_edit_right.GetWindowTextW(strRectR);
	m_edit_bottom.GetWindowTextW(strRectB);

	area->left = _ttoi(strRectL);
	area->top = _ttoi(strRectT);
	area->right = _ttoi(strRectR);
	area->bottom = _ttoi(strRectB);
}

void CGifMakerDlg::CalibrateCaputreArea()
{
	CRect area;
	GetCaptureArea(&area);				// get capture area
	long areaTemp;
	if (area.left > area.right)			// change left <-> right
	{
		areaTemp = area.left;
		area.left = area.right;
		area.right = areaTemp;
	}
	if (area.top > area.bottom)			// change top <-> bottom
	{
		areaTemp = area.top;
		area.top = area.bottom;
		area.bottom = areaTemp;
	}
	SetCaptureArea(area);				// set capture rect
}

void CGifMakerDlg::GetRectSize(CRect ret, CSize *pSize)
{
	pSize->cx = ret.right - ret.left;
	pSize->cy = ret.bottom - ret.top;
}

void CGifMakerDlg::OnBnClickedButtonCapture()
{
	// TODO: Add your control notification handler code here
	SetLayeredWindowAttributes(0, 0, LWA_ALPHA);	// make tool transparant on

	CClientDC ScreenDC(GetDesktopWindow());	// screen dc
	CDC *pDc = m_pic_screen.GetWindowDC();	// picture control (draw main)
	CDC memDC;								// memory dc
	memDC.CreateCompatibleDC(&ScreenDC);	// memory dc compatible with screen dc
	
	CRect captureRect, pictureRect;
	CSize captureSize, pictureSize;
	CalibrateCaputreArea();						// calibrate capture area
	GetCaptureArea(&captureRect);				// get capture area
	GetRectSize(captureRect, &captureSize);		// get capture size
	m_pic_screen.GetWindowRect(&pictureRect);	// get picture control position
	GetRectSize(pictureRect, &pictureSize);		// get picture control size

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&ScreenDC, captureSize.cx, captureSize.cy);	// create bitmap compatible with screen dc
	CBitmap *pOldBitmap = memDC.SelectObject(&bitmap);		// bitmap point
	// copy screenDC (x,y)~(x+cx,y+cy) to memDC (0,0)~(cx,cy)
	memDC.BitBlt(0, 0, captureSize.cx, captureSize.cy, &ScreenDC, captureRect.left, captureRect.top, SRCCOPY);
	// copy memDC (0,0)~(cx,cy) to pDc (0,0)~(pictureCx, pictureCy)
	pDc->StretchBlt(0, 0, pictureSize.cx, pictureSize.cy, &memDC, 0, 0, captureSize.cx, captureSize.cy, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
	
	SetLayeredWindowAttributes(0, 255, LWA_ALPHA);	// make tool transparant off
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

		// set capture area L,T,R,B
		if (isDlgInit == TRUE)
		{
			CString strType;
			m_combo_monitor_type.GetWindowTextW(strType);
			if (strType.Compare(TYPE_CUSTOMIZING) == 0)
			{
				// set capture area L,T,R,B
				SetAutoCaptureArea();
			}
		}
	}
	
	//변경 되는 사이즈를 알기위해 이전 사이즈 저장
	size.cx = cx;
	size.cy = cy;
}

void CGifMakerDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnGetMinMaxInfo(lpMMI);

	lpMMI->ptMinTrackSize = DlgMinSize;

}

void CGifMakerDlg::SetCaptureAreaTransparant()
{
	CDC *pDc = m_pic_screen.GetWindowDC();	// picture control
	CRect rect;
	GetClientRect(rect);
	pDc->FillSolidRect(rect, RGB_TRANSPARANT);
	SetLayeredWindowAttributes(RGB_TRANSPARANT, 0, LWA_COLORKEY);	// transparant
}

void CGifMakerDlg::OnBnClickedButtonClear()
{
	// TODO: Add your control notification handler code here
	CDC *pDc = m_pic_screen.GetWindowDC();	// picture control
	CRect rect;
	GetClientRect(rect);
	pDc->FillSolidRect(rect, RGB_TRANSPARANT);
	SetLayeredWindowAttributes(RGB_TRANSPARANT, 0, LWA_COLORKEY);	// transparant
}

void CGifMakerDlg::OnCbnSelchangeComboMonitorType()
{
	// TODO: Add your control notification handler code here
	// get main monitor info
	CRect mainRect;
	CString strType;
	int monitor_type = m_combo_monitor_type.GetCurSel();	// get monitor type
	m_combo_monitor_type.GetWindowTextW(strType);
	if (strType.Compare(TYPE_CUSTOMIZING) == 0)
	{
		SetAutoCaptureArea();
	}
	else
	{
		mainRect.left = MonitorsRect[monitor_type].left;
		mainRect.top = MonitorsRect[monitor_type].top;
		mainRect.right = MonitorsRect[monitor_type].right;
		mainRect.bottom = MonitorsRect[monitor_type].bottom;

		SetCaptureArea(mainRect);
	}
}

void CGifMakerDlg::SaveCaptureImage()
{
	///////////////////////////////////////////////////////////////////
	// file path
	CString strFp, strFpNum;
	CString strSaveFilePath, strFileExtension;

	m_edit_file_path.GetWindowTextW(strFp);
	strFpNum.Format(_T("%03d"), m_nCaptureNum);
	strFp += _T("C");
	strFp += strFpNum;
	m_nCaptureNum++;
	// confirm fiel extension
	strFileExtension = strFp.Right(4);
	if (strFileExtension.Compare(_T(".bmp")) != 0)
	{
		strSaveFilePath = strFp + _T(".bmp");
	}
	else
	{
		strSaveFilePath = strFp;
	}

	///////////////////////////////////////////////////////////////////
	// capture area
	CRect captureRect;
	CalibrateCaputreArea();						// calibrate capture area
	GetCaptureArea(&captureRect);				// get capture area

	///////////////////////////////////////////////////////////////////
	// save image
	SaveCaptureArea(captureRect, strSaveFilePath);
}

void CGifMakerDlg::SaveRecImage()
{
	///////////////////////////////////////////////////////////////////
	// file path
	CString strFp, strFpNum;
	CString strSaveFilePath, strFileExtension;

	m_edit_file_path.GetWindowTextW(strFp);
	strFpNum.Format(_T("%03d"), m_nRecNum);
	strFp += _T("R");
	strFp += strFpNum;
	m_nRecNum++;
	m_nFb++;
	// confirm fiel extension
	strFileExtension = strFp.Right(4);
	if (strFileExtension.Compare(_T(".bmp")) != 0)
	{
		strSaveFilePath = strFp + _T(".bmp");
	}
	else
	{
		strSaveFilePath = strFp;
	}

	///////////////////////////////////////////////////////////////////
	// capture area
	CRect captureRect;
	CalibrateCaputreArea();						// calibrate capture area
	GetCaptureArea(&captureRect);				// get capture area

	///////////////////////////////////////////////////////////////////
	// save image
	SaveCaptureArea(captureRect, strSaveFilePath);
}

void CGifMakerDlg::OnBnClickedButtonSave()
{
	// TODO: Add your control notification handler code here
	SetLayeredWindowAttributes(0, 0, LWA_ALPHA);	// make tool transparant on
	
	SaveCaptureImage();

	SetLayeredWindowAttributes(RGB_TRANSPARANT, 255, LWA_COLORKEY | LWA_ALPHA);	// make tool transparant off
}


LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	LPKBDLLHOOKSTRUCT pkbhs = (LPKBDLLHOOKSTRUCT)lParam;
	if (nCode == HC_ACTION)
	{
		if (pkbhs->vkCode == VK_F3)
		{
			DWORD dwFlags = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) ? 0 : KEYEVENTF_KEYUP;
			if (dwFlags == 0)
			{
				CGifMakerDlg* pMainDlg = (CGifMakerDlg*)::AfxGetMainWnd();	// get main dialog pointer
				pMainDlg->OnBnClickedButtonFileName();
				return TRUE;
			}
		}
		else if (pkbhs->vkCode == VK_F4)
		{
			DWORD dwFlags = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) ? 0 : KEYEVENTF_KEYUP;
			if (dwFlags == 0)
			{
				CGifMakerDlg* pMainDlg = (CGifMakerDlg*)::AfxGetMainWnd();	// get main dialog pointer
				pMainDlg->OnBnClickedButtonSave();
				return TRUE;
			}
		}
		else if (pkbhs->vkCode == VK_F5)
		{
			DWORD dwFlags = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) ? 0 : KEYEVENTF_KEYUP;
			if (dwFlags == 0)
			{
				CGifMakerDlg* pMainDlg = (CGifMakerDlg*)::AfxGetMainWnd();	// get main dialog pointer
				pMainDlg->OnBnClickedButtonRec();
				return TRUE;
			}
		}
		else if (pkbhs->vkCode == VK_F6)
		{
			DWORD dwFlags = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) ? 0 : KEYEVENTF_KEYUP;
			if (dwFlags == 0)
			{
				CGifMakerDlg* pMainDlg = (CGifMakerDlg*)::AfxGetMainWnd();	// get main dialog pointer
				pMainDlg->OnBnClickedButtonGif();
				return TRUE;
			}
		}
	}

	return CallNextHookEx(g_hook, nCode, wParam, lParam);
}

BOOL CGifMakerDlg::SetHook()
{
	if (g_hook != NULL)
		UnHook();
	g_hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, (HINSTANCE)theApp.m_hInstance, NULL);

	return TRUE;
}

BOOL CGifMakerDlg::UnHook()
{
	if (g_hook != NULL)
	{
		UnhookWindowsHookEx(g_hook);
	}
	return TRUE;
}

BOOL CGifMakerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if (pMsg->wParam == VK_RETURN)			//enter
	{
		return TRUE;
	}
	else if (pMsg->wParam == VK_ESCAPE)		//esc
	{
		//return TRUE;
	}
	else if (pMsg->wParam == VK_F1)			//F1
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CGifMakerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (AreaFlag == AreaActiveOn)
	{
		CRect areaRect;
		CPoint mousePos;
		::GetCursorPos(&mousePos);	//get mouse position on monitor
		GetCaptureArea(&areaRect);	//get area rect

		areaRect.left = mousePos.x;
		areaRect.top = mousePos.y;
		
		SetCaptureArea(areaRect);
	}
	else if (AreaFlag == AreaSelecting)
	{
		CRect areaRect;
		CPoint mousePos;
		::GetCursorPos(&mousePos);	//get mouse position on monitor
		GetCaptureArea(&areaRect);	//get area rect

		areaRect.right = mousePos.x;
		areaRect.bottom = mousePos.y;

		SetCaptureArea(areaRect);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

// mouse left button - down
void CGifMakerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (AreaFlag == AreaActiveOn)
	{
		CRect areaRect;
		CPoint mousePos;
		::GetCursorPos(&mousePos);	//get mouse position on monitor
		GetCaptureArea(&areaRect);	//get area rect

		areaRect.left = mousePos.x;
		areaRect.top = mousePos.y;

		SetCaptureArea(areaRect);
		AreaFlag = AreaSelecting;
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

//  mouse left button - up
void CGifMakerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (AreaFlag == AreaSelecting)
	{
		CRect areaRect;
		CPoint mousePos;
		::GetCursorPos(&mousePos);	//get mouse position on monitor
		GetCaptureArea(&areaRect);	//get area rect

		areaRect.right = mousePos.x;
		areaRect.bottom = mousePos.y;

		SetCaptureArea(areaRect);
		ReleaseCapture();
		AreaFlag = AreaActiveOff;
	}


	CDialogEx::OnLButtonUp(nFlags, point);
}


HBRUSH CGifMakerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_PIC_SCREEN)
	{
		OnBnClickedButtonClear();
	}

	switch (nCtlColor)
	{
		case CTLCOLOR_STATIC:
		{
			pDC->SetTextColor(BLACK);
			pDC->SetBkColor(SOFT_BLUE);
		}
		break;
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CGifMakerDlg::OnBnClickedButtonAreaDetail()
{
	// TODO: Add your control notification handler code here
	if (AreaFlag == AreaActiveOff)
	{
		OnBnClickedButtonClear();
		AreaFlag = AreaActiveOn;
		SetCapture();
	}
}


void CGifMakerDlg::OnBnClickedButtonAreaSetting()
{
	// TODO: Add your control notification handler code here
	// set capture area L,T,R,B
	SetAutoCaptureArea();

	OnBnClickedButtonCapture();
}


void CGifMakerDlg::SaveCaptureArea(CRect area, CString strFilePath)
{
	CClientDC ScreenDC(GetDesktopWindow());	// screen dc
	CDC memDC;								// memory dc
	memDC.CreateCompatibleDC(&ScreenDC);	// memory dc compatible with screen dc

	CSize captureSize;
	GetRectSize(area, &captureSize);	// get capture size

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&ScreenDC, captureSize.cx, captureSize.cy);	// create bitmap compatible with screen dc
	CBitmap *pOldBitmap = memDC.SelectObject(&bitmap);		// bitmap point
	// copy screenDC (x,y)~(x+cx,y+cy) to memDC (0,0)~(cx,cy)
	memDC.BitBlt(0, 0, captureSize.cx, captureSize.cy, &ScreenDC, area.left, area.top, SRCCOPY);

	// convert CString to char*
	char *fp = new char[strFilePath.GetLength() + 1];
	strcpy_s(fp, strFilePath.GetLength() + 1, CT2A(strFilePath));

	// save 24bit bmp file
	SaveCDCtoBMP(&memDC, area, 24, fp);

	delete[] fp;
	fp = NULL;

	memDC.SelectObject(pOldBitmap);
}

void CGifMakerDlg::OnBnClickedButtonRec()
{
	// TODO: Add your control notification handler code here
	CString strRecButton;
	m_btn_rec.GetWindowTextW(strRecButton);
	if (strRecButton.Compare(F5_IMAGE_START) == 0)
	{
		SaveRecordingImage_Start();
	}
	else if (strRecButton.Compare(F5_IMAGE_WAIT) == 0)
	{
		
	}
	else
	{
		SaveRecordingImage_End();
	}
}

void CGifMakerDlg::SaveRecordingImage_Start()
{
	m_btn_rec.SetWindowTextW(F5_IMAGE_WAIT);

	SetLayeredWindowAttributes(0, 0, LWA_ALPHA);	// make tool transparant on
	///////////////////////////////////////////////////////////////////
	// capture area
	CalibrateCaputreArea();					// calibrate capture area
	GetCaptureArea(&m_recRec);				// get capture area

	///////////////////////////////////////////////////////////////////
	// frame buffer
	double fps;			// frame buffer per second
	double rec_time;		// recording time
	CString strFps, strTime;
	m_edit_rec_fps.GetWindowTextW(strFps);
	m_edit_rec_time.GetWindowTextW(strTime);
	fps = _ttof(strFps);
	rec_time = _ttof(strTime);

	double fb_timer;		// second per 1 frame buffer
	fb_timer = 1000.0 / fps;
	m_nFbMax = fps * rec_time;
	m_nFb = 1;
	///////////////////////////////////////////////////////////////////
	// start timer - save recording image
	SetTimer(TID_REC, (UINT)fb_timer, NULL);

	m_btn_rec.SetWindowTextW(F5_IMAGE_STOP);
}

void CGifMakerDlg::SaveRecordingImage_Execute()
{
	SaveRecImage();

	if (m_nFb > m_nFbMax)
	{
		SaveRecordingImage_End();
	}
}

void CGifMakerDlg::SaveRecordingImage_End()
{
	KillTimer(TID_REC);
	m_btn_rec.SetWindowTextW(F5_IMAGE_START);
	SetLayeredWindowAttributes(RGB_TRANSPARANT, 255, LWA_COLORKEY | LWA_ALPHA);	// make tool transparant off
}

void CGifMakerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case TID_REC:

		SaveRecordingImage_Execute();

		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

BOOL CGifMakerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetClientRect(bgRect);
	pDC->FillSolidRect(bgRect, SOFT_BLUE);
	return true;

	//return CDialogEx::OnEraseBkgnd(pDC);
}


void CGifMakerDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnClose();

	UnHook();
}


void CGifMakerDlg::OnEnChangeEditLeft()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	static CString strLeftPre = _T("0");
	CString strLeftNow;
	m_edit_left.GetWindowTextW(strLeftNow);
	if (strLeftNow.Compare(strLeftPre) != 0)
	{
		SetCaptureSize();
		strLeftPre = strLeftNow;
	}
}


void CGifMakerDlg::OnEnChangeEditRight()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	static CString strRightPre = _T("0");
	CString strRightNow;
	m_edit_right.GetWindowTextW(strRightNow);
	if (strRightNow.Compare(strRightPre) != 0)
	{
		SetCaptureSize();
		strRightPre = strRightNow;
	}
}


void CGifMakerDlg::OnEnChangeEditTop()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	static CString strTopPre = _T("0");
	CString strTopNow;
	m_edit_top.GetWindowTextW(strTopNow);
	if (strTopNow.Compare(strTopPre) != 0)
	{
		SetCaptureSize();
		strTopPre = strTopNow;
	}
}


void CGifMakerDlg::OnEnChangeEditBottom()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	static CString strBottomPre = _T("0");
	CString strBottomNow;
	m_edit_bottom.GetWindowTextW(strBottomNow);
	if (strBottomNow.Compare(strBottomPre) != 0)
	{
		SetCaptureSize();
		strBottomPre = strBottomNow;
	}
}


void CGifMakerDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	// TODO: Add your message handler code here
	if(isDlgInit == TRUE)
	{
		CString strType;
		m_combo_monitor_type.GetWindowTextW(strType);
		if (strType.Compare(TYPE_CUSTOMIZING) == 0)
		{
			// set capture area L,T,R,B
			SetAutoCaptureArea();
		}
	}
}

void CGifMakerDlg::OnBnClickedButtonFileName()
{
	// TODO: Add your control notification handler code here
	// capture number init
	m_nCaptureNum = 1;
	m_nRecNum = 1;

	///////////////////////////////////////////////////////////////////
	// file path
	CString szFilter = _T("Image (*.BMP, *.PNG, *.JPG) | *.BMP;*.PNG;*.JPG | All Files(*.*)|*.*||");
	CString strFp;
	// FALSE -> save dialog / TRUE -> load dialog
	CFileDialog fsave_dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, szFilter);
	// select ok button
	if (fsave_dlg.DoModal() == IDOK)
	{
		strFp = fsave_dlg.GetPathName();	// file path
		
		m_edit_file_path.SetWindowTextW(strFp);
	}
}


void CGifMakerDlg::OnBnClickedButtonGif()
{
	// TODO: Add your control notification handler code here
	ShellExecute(this->m_hWnd, TEXT("open"), TEXT("IEXPLORE.EXE"), TEXT(" http://ezgif.com/maker"), NULL, SW_SHOW);

}