// BarcodeMgt.h: interface for the CBarcodeMgt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOBOARDMGT_H__CB41B617_8E88_4945_AA60_797D31EC6E68__INCLUDED_)
#define AFX_IOBOARDMGT_H__CB41B617_8E88_4945_AA60_797D31EC6E68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "jdefine.h"
#include "Uart4Pc.h"

class CIoBoardMgt
{
public:
	CIoBoardMgt();
	virtual ~CIoBoardMgt();

    // Functions
    bool Open(const char *a_pszCom);
    void Close(void);
    char *GetMsg(void);
    U32  GetMsgCnt(void);
    bool TransferMsg(const char *a_pszTx);

    // Port Control
    U16  GetInPort(void);
    U16  GetOutPort(void);
    U16  PutOutPort(U16 a_bData);
    U16  PutOutBit(U16 a_bBitPos, bool a_tHi);

    // Member Variables
    HANDLE m_hCom;

    U32  m_wMsgCnt;
    char m_szMsg[1024];
};

#endif // !defined(AFX_IOBOARDMGT_H__CB41B617_8E88_4945_AA60_797D31EC6E68__INCLUDED_)
