#ifndef Y_EDIT_BOX_H
#define Y_EDIT_BOX_H

#include "y_wm_user.h"

enum
{
	UPDATE_OFF = 0,
	UPDATE_ON,
};

class CEditBox : public CStatic
{
public:

	CEditBox();
	~CEditBox();

	//--------- 외부 환경 변수 ------------------------------------//
	BOOL m_tFontInstalled;  // 폰트 설정 여부
	UINT m_nFontFormat;		// 폰트 포맷
							//--> 좌우 정렬 : DT_LEFT, DT_CENTER , DT_RIGHT
							//--> 상하 정렬 : DT_TOP , DT_VCENTER, DT_BOTTOM
	LOGFONT  m_LogFont;		// 폰트 설정
	CFont    m_Font;		// 폰트 크기 및 디자인
	COLORREF m_ColorTxt;	// 폰트 색
	COLORREF m_ColorBg;		// 배경 색
	CString  m_strMsg;		// 내용
	//--------- 외부 사용 함수 ------------------------------------//
	void SetTextFormat(UINT a_uFormat);
	void SetTextColor(COLORREF a_Color);
	void SetBgColor(COLORREF a_Color);
	void SetLogFont(LOGFONT *a_pLogFont);
	void SetTextString(CString a_String);

	void SetText(COLORREF a_BgColor, COLORREF a_TextColor, CString a_StrText, BOOL a_UpdateMsg);
	
protected:
	//--------- 내부 환경 변수 ------------------------------------//

	//--------- 내부 사용 함수 ------------------------------------//
	int GetMsgLines();

	//{{AFX_MSG(CTextStatic)
	afx_msg void OnPaint();
    afx_msg LRESULT OnThreadMsgUpdate(WPARAM a_wParam, LPARAM a_lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};





#endif //  !Y_EDIT_BOX_H  //