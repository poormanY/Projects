// AmMeterMgt.h: interface for the CAmMeterMgt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AMMETERMGT_H__AB2D5E66_F409_4171_ABA4_854D723A910C__INCLUDED_)
#define AFX_AMMETERMGT_H__AB2D5E66_F409_4171_ABA4_854D723A910C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "jdefine.h"
#include "Uart4Pc.h"

typedef struct _stQuery_ {
	u08 id;
	u08 func;
	u08 start_addr_hi;
	u08 start_addr_lo;
	u08 size_hi;
	u08 size_lo;
	u16 CRC16;
} stQuery;

typedef struct _stRegister_ {
	u08 value_hi;
	u08 value_lo;
	u08 dot_hi;
	u08 dot_lo;
	u08 hipeak_hi;
	u08 hipeak_lo;
	u08 lopeak_hi;
	u08 lopeak_lo;
	u16 CRC16;
} stRegister;

typedef struct _stOBJ_ {
	BYTE	stat;		// 통신상태
	BYTE	coil;		// coil stat
	BYTE	mode;		// standard(0) or scale(other)
	BYTE	dot;		// 점위치(0~3)
	short	value;		// 측정값
	short	hipeak;
	short	lopeak;
} stOBJ;

class CAmMeterMgt  
{
public:
	CAmMeterMgt();
	virtual ~CAmMeterMgt();

	// Functions
	TOF Thread_Begin(void);
	TOF Thread_End(void);
	char *GetMsg(void);
	void Close(void);
	bool Open(const char *a_pszCom);
	
	// Member Variables
    HANDLE m_hCom;

	int m_nAmMeter_State;
    bool m_tMsgComplete;
    char m_szMsg[256];

	stQuery    m_Query;
	stRegister m_Register;
	stOBJ      m_OBJ;

    // Thread for Run
    TOF m_tThreadStop;
	CWinThread * m_pThread;

    friend UINT ThreadAmMeter(LPVOID pParam);
};

#endif // !defined(AFX_AMMETERMGT_H__AB2D5E66_F409_4171_ABA4_854D723A910C__INCLUDED_)
