// BarcodeMgt.h: interface for the CBarcodeMgt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCREWMGT_H__CB41B617_8E88_4945_AA60_797D31EC6E68__INCLUDED_)
#define AFX_SCREWMGT_H__CB41B617_8E88_4945_AA60_797D31EC6E68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "jdefine.h"
#include "Uart4Pc.h"

class CScrewMgt
{
public:
	bool SetSendMsg(const char *szMeg, int nRepeatNum);
	int m_nScrew_State;
	CScrewMgt();
	virtual ~CScrewMgt();

    // Functions
    bool Open(const char *a_pszCom);
    void Close(void);

    bool  AddCount(void);
    bool  DelCount(void);
    void  DelAllCount(void);
    char *GetMsg(void);
    S16   GetCount(void);

    TOF  Thread_Begin(void);
    TOF  Thread_End(void);

    // Member Variables
    HANDLE m_hCom;

    bool m_tMsgComplete;
    S16  m_nCount;
    char m_szMsg[256];

    // Thread for Run
    TOF m_tThreadStop;
	CWinThread * m_pThread;

    friend UINT ThreadScrew(LPVOID pParam);
};

#endif // !defined(AFX_SCREWMGT_H__CB41B617_8E88_4945_AA60_797D31EC6E68__INCLUDED_)
