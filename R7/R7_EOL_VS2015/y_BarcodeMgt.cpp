// BarcodeMgt.cpp: implementation of the CBarcodeMgt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "y_BarcodeMgt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBarcodeMgt::CBarcodeMgt()
{
    m_hCom = INVALID_HANDLE_VALUE;

    m_tThreadStop = false;
    m_pThread = NULL;

    m_nBuffIdx = 0;
    memset(m_szBarCode, 0, sizeof(m_szBarCode));

    m_nData = 0;
    memset(m_arszData, 0, sizeof(m_arszData));
}

CBarcodeMgt::~CBarcodeMgt()
{
    Close();
}

bool CBarcodeMgt::Open(const char *a_pszCom)
{
    Close();

    m_hCom = UartSetup(a_pszCom, 9600);

   if (m_hCom == INVALID_HANDLE_VALUE)
   {
      return false;
   }
   
   return true;
}

void CBarcodeMgt::Close(void)
{
    Thread_End();

    if (m_hCom != INVALID_HANDLE_VALUE)
    {
        PurgeComm( m_hCom, PURGE_TXABORT | PURGE_RXABORT |
                           PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
        CloseHandle( m_hCom );

        m_hCom = INVALID_HANDLE_VALUE;
    }
}


bool  CBarcodeMgt::AddData(const char *a_pszBarCode)
{
    if (a_pszBarCode == NULL)
    {
        return false;
    }

    if (m_nData >= (_BARCODE_DATA_MAX-2))
    {
        return false;
    }

    strcpy(m_arszData[m_nData], a_pszBarCode);
    m_nData++;

    return true;
}

bool  CBarcodeMgt::DelData(void)
{
    S16 i;

    if (m_nData == 0)
    {
        return false;
    }
    else
    {
        for (i = 1; i <= m_nData; i++)
        {
            strcpy(m_arszData[i-1], m_arszData[i]);
        }
        m_nData--;
    }

    return true;
}

bool  CBarcodeMgt::DelDataAt(S16 a_nAt)
{
    S16 i;
    
    if (m_nData <= a_nAt)
    {
        return false;
    }
    else
    {
        for (i = a_nAt; i < m_nData; i++)
        {
            strcpy(m_arszData[i], m_arszData[i+1]);
        }
        m_nData--;
    }
    
    return true;
}

void CBarcodeMgt::DelAllData(void)
{
    S16 i;

    for (i = 0; i < m_nData; i++)
    {
        m_arszData[i][0] = 0;
    }
    
    m_nData = 0;
}

char *CBarcodeMgt::GetDataAt(S16 a_nAt)
{
    return m_arszData[a_nAt];
}

S16   CBarcodeMgt::GetCount(void)
{
    return m_nData;
}

S16   CBarcodeMgt::SearchData(const char *a_pszBarCode)
{
    S16 i;

    for (i = 0; i < m_nData; i++)
    {
        if (strcmp(m_arszData[i], a_pszBarCode) == 0)
        {
            return i;
        }
    }

    return -1;
}

TOF  CBarcodeMgt::Thread_Begin(void)
{
    Thread_End();

    if (m_hCom == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    // New Start Thread
    m_tThreadStop = false;
    m_pThread = AfxBeginThread(ThreadBarCode, this);

    if (m_pThread == NULL)
    {
        Thread_End();
        return false;
    }

    return true;
}

TOF  CBarcodeMgt::Thread_End(void)
{
    m_tThreadStop = true;
    if (m_pThread != NULL)
    {
        ::WaitForSingleObject(m_pThread->m_hThread, 1000);
        m_pThread = NULL;
    }

    m_tThreadStop = false;
    return true;
}

UINT ThreadBarCode(LPVOID pParam)
{
    int nRcvLen;
    int nRcvIdx;
    char szBuff[_BARCODE_BUFF_MAX];
    CBarcodeMgt *pBarCode = (CBarcodeMgt *)pParam;

    nRcvIdx = 0;
    while ( (pBarCode->m_tThreadStop == false) && (pBarCode->m_hCom != INVALID_HANDLE_VALUE) )
    {
        Sleep(50);

        nRcvLen = UartRxCount(pBarCode->m_hCom);
        if (nRcvLen != 0)
        {
            UartRead(pBarCode->m_hCom, &szBuff[nRcvIdx], nRcvLen);
            nRcvIdx += nRcvLen;
        }
        else
        {
            if (nRcvIdx > 0)
            {
                szBuff[nRcvIdx] = 0;
                nRcvIdx = 0;

                pBarCode->AddData(szBuff);
            }
        }
    }

    return 0;
}
