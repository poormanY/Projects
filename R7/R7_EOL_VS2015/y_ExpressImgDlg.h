#pragma once

#include "y_Draw.h"

// CExpressImgDlg 대화 상자입니다.

class CExpressImgDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CExpressImgDlg)

public:
	CExpressImgDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CExpressImgDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EXPRESS_IMG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	CRect rtSubScreenSize, rtWholeScreenSize;	// 화면 크기
	CDraw theDraw;								// 이미지를 그리기 위한 클래스 호출
	CBitmap* m_pbmBack;							// 배경이미지 메모리
	int m_nImgView;								// 이미지 선택 번호
	int m_nPreImgView;							// 이전 이미지 번호
	int m_tImageShowCompleted;					// 차선이동 애니메이션이 완료되었는지 체크
	int m_nCurrPos;								// 이미지 이동 현재 상태
	////////////////////
	// ADAS Draw Data
	int m_nVanish;							// Vanish 값
	int m_nLaneWidth;						// 차선 넓이
	int m_nLaneMovement;					// 차선 이동량
	COLORREF m_nLaneLColor;					// 좌측차선 색상
	COLORREF m_nLaneRColor;					// 우측차선 색상

	BOOL CreateExpressImg();				// 모니터 갯수를 구한 후 서브 모니터 사이즈에 다이얼로그 표시
	void GetDisplaySpace(CRect &rt);		// 서브 모니터의 해상도를 확인
	void DrawALL(CDC *pDC, int PartNum);	// 전체 그림을 그림
	void DrawLine_Sub (CDC *BufferDC, int GapMovement); // ADAS 차선
	void DrawLineNight_Sub (CDC *BufferDC, int GapMovement);
	void Vehicle_Sub (CDC *BufferDC, int GapMovement, int nInitSize, int CAR_BITMAP);	// ADAS 차

	COLORREF RGBtoDark(COLORREF rgb);
public:
	enum _ImgView
	{
		CALIBRATION = 0,
		LDW_DETECT = 1,
		LDW_WARN_L = 2,
		LDW_WARN_R = 3,
		FCW_CAR_DETECT = 4,
		FCW_CAR_WARN,
		FCDA_CAR_DETECT,
		FCDA_CAR_WARN,
		LDW_DETECT_NIGHT = 101,
		LDW_WARN_L_NIGHT,
		LDW_WARN_R_NIGHT,
		FCW_CAR_DETECT_NIGHT,
		FCW_CAR_WARN_NIGHT,
		FCDA_CAR_DETECT_NIGHT,
		FCDA_CAR_WARN_NIGHT,
		FCW_CAR_DETECT2 = 1004,
		FCW_CAR_WARN2,
		FCDA_CAR_DETECT2,
		FCDA_CAR_WARN2,
		FCW_CAR_DETECT3 = 2004,
		FCW_CAR_WARN3,
		FCDA_CAR_DETECT3,
		FCDA_CAR_WARN3,
	};
	BOOL ImgSet(_ImgView ImgNum);			// 이미지 세팅 값이 이전과 같으면 0, 다르면 1을 리턴
	BOOL ImgSet_Vanish(int vanish);			// vanish 값
	BOOL ImgSet_LandWidth(int laneWidth);	// 차선 넓이
	BOOL ImgSet_LaneMovemnet(int laneMove);	// 차선 이동값
	BOOL ImgSet_LaneLColor(COLORREF color);	// 좌측차선 색상
	BOOL ImgSet_LaneRColor(COLORREF color);	// 우측차선 색상

	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void PostNcDestroy();
};
