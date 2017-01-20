#include "stdafx.h"
#include "MainApp.h"
#include "Color.h"
#include "Draw.h"
#include "DialogSet.h"
#include "RightMoniter.h"
#include "atlimage.h"
#include "MainDlg.h"
extern CDialogSet  *pSetupDlg;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString strRImagePath;

CRightImg::CRightImg(CWnd* pParent /*=NULL*/)
	: CDialog(CRightImg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExpressImg)
		// NOTE: the ClassWizard will add member initialization here

	//}}AFX_DATA_INIT
}


void CRightImg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExpressImg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRightImg, CDialog)
	//{{AFX_MSG_MAP(CExpressImg)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExpressImg message handlers

BOOL CRightImg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 초기 화면에 이미지 표시.
void CRightImg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	DrawImage(&dc);

	// Do not call CDialog::OnPaint() for painting messages
}


void CRightImg::DrawImage(CDC *pDC)
{

	CImage Image;
	CClientDC dc(this);
	//	CPaintDC dc(this);
	HRESULT hresult = Image.Load(strRImagePath);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Image.TransparentBlt(dc.m_hDC, 0, 0, 1280, 720, RGB(255, 0, 0) );
	//Image.BitBlt(dc.m_hDC, 500, 300, SRCCOPY);
}

// 서브모니터의 해상도를 확인
// 320 * 240의 비율로 디스플레이를 하기 위하여 상하좌우의 좌표값을 저장

int CRightImg::RightDraw(CString Name, unsigned int StartNumber, unsigned int StopNumber)
{
	CImage Image;
	CClientDC dc(this);


	CString Path;
	int strLen = Name.GetLength();
	unsigned int Number_INDEX;
	unsigned int Number_FILE;
	int k=1;
	char ch[100];

	if((StartNumber==0)&&(StopNumber==0)) return 0;

	Number_INDEX=strLen-5;
	strcpy_s(ch,(LPSTR)(LPCSTR)Name);
#if 0
	strcpy_s(ch,(LPSTR)(LPCSTR)Name);

	Number_FILE=0;
	for(int i=0;i<5; i++)
	{
		if((ch[Number_INDEX-i]<48)&&(ch[Number_INDEX-i]>57))
		{
			AfxMessageBox("파일번호가 올바르지 않습니다. 파일명의 끝 5자리가 파일번호");
			return 0;
		}
		if(!i) Number_FILE +=((ch[Number_INDEX]-48));
		else
		{
			k*=10;
			Number_FILE +=((ch[Number_INDEX-i]-48)*(k));

		}
	}

	if(Number_FILE<9200)
	{

		//Path.Format(_T("%s"),ch);
		Image.Destroy();
		HRESULT hresult = Image.Load(strRImagePath);
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		Image.TransparentBlt(dc.m_hDC, 0, 0, 1280, 720, RGB(255, 0, 0) );

		Number_FILE++;
		ch[Number_INDEX-3]=(Number_FILE/1000)+48;
		ch[Number_INDEX-2]=((Number_FILE%1000)/100)+48;
		ch[Number_INDEX-1]=((Number_FILE%100)/10)+48;
		ch[Number_INDEX]=(Number_FILE%10)+48;
		Path=(LPSTR)ch;
		strRImagePath=Path;
	}
	else
	{
		Image.Destroy();
		//return 0;
	}
#else

	Number_FILE=StartNumber;

	if(Number_FILE<StopNumber)
	{

		//Path.Format(_T("%s"),ch);
		Image.Destroy();
		HRESULT hresult = Image.Load(strRImagePath);
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		Image.TransparentBlt(dc.m_hDC, 0, 0, 1280, 720, RGB(255, 0, 0) );

		Number_FILE++;
		ui_RStartNumber++;
		ch[Number_INDEX-3]=(Number_FILE/1000)+48;
		ch[Number_INDEX-2]=((Number_FILE%1000)/100)+48;
		ch[Number_INDEX-1]=((Number_FILE%100)/10)+48;
		ch[Number_INDEX]=(Number_FILE%10)+48;
		Path=(LPSTR)ch;
		strRImagePath=Path;
	}
	else
	{
		Image.Destroy();
		return 0;
	}


#endif

	return 1;
	//delete[] ch;
	//	CPaintDC dc(this);

	//Image.BitBlt(dc.m_hDC, 500, 300, SRCCOPY);
}
