// BarcodeMgt.h: interface for the CBarcodeMgt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BARCODEMGT_H__CB41B617_8E88_4945_AA60_797D31EC6E68__INCLUDED_)
#define AFX_BARCODEMGT_H__CB41B617_8E88_4945_AA60_797D31EC6E68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "jdefine.h"
#include "Uart4Pc.h"

#define _BARCODE_DATA_MAX   101 // Available Data = _BARCODE_DATA_MAX - 1
#define _BARCODE_BUFF_MAX   128

class CBarcodeMgt  
{
public:
	CBarcodeMgt();
	virtual ~CBarcodeMgt();

    // Functions
    bool Open(const char *a_pszCom);
    void Close(void);

    bool  AddData(const char *a_pszBarCode);
    bool  DelData(void);
    bool  DelDataAt(S16 a_nAt);
    void  DelAllData(void);
    char *GetDataAt(S16 a_nAt);
    S16   GetCount(void);
    S16   SearchData(const char *a_pszBarCode);

    TOF  Thread_Begin(void);
    TOF  Thread_End(void);

    // Member Variables
    HANDLE m_hCom;
    
    S16  m_nBuffIdx;
    char m_szBarCode[_BARCODE_BUFF_MAX];

    S16  m_nData;
    char m_arszData[_BARCODE_DATA_MAX][_BARCODE_BUFF_MAX];

    // Thread for Run
    TOF m_tThreadStop;
	CWinThread * m_pThread;

    friend UINT ThreadBarCode(LPVOID pParam);
};

#endif // !defined(AFX_BARCODEMGT_H__CB41B617_8E88_4945_AA60_797D31EC6E68__INCLUDED_)
