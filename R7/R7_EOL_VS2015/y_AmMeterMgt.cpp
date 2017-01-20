// AmMeterMgt.cpp: implementation of the CAmMeterMgt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "y_AmMeterMgt.h"

#define _AMMETER_NONE       0
#define _AMMETER_READ       1
#define _AMMETER_WRITE      2

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAmMeterMgt::CAmMeterMgt()
{
	m_hCom = INVALID_HANDLE_VALUE;

	m_nAmMeter_State = _AMMETER_NONE;
    m_tThreadStop = false;
    m_pThread = NULL;

    memset(m_szMsg, 0, sizeof(m_szMsg));
    m_tMsgComplete = false;

	m_Query.id            = 1;
	//m_Query.func          = 1;
	m_Query.func          = 4;
	m_Query.start_addr_hi = 0;
	m_Query.start_addr_lo = 0;
	m_Query.size_hi       = 0;
	m_Query.size_lo       = 4;
	//m_Query.size_lo       = 3;
	m_Query.CRC16         = 0xC9F1;
	//m_Query.CRC16         = 0x0b7c;
	
	memset(&m_OBJ, 0, sizeof(stOBJ));
}

CAmMeterMgt::~CAmMeterMgt()
{
	Close();
}

bool CAmMeterMgt::Open(const char *a_pszCom)
{
	Close();

    m_hCom = UartSetup(a_pszCom, 9600);

   if (m_hCom == INVALID_HANDLE_VALUE)
   {
      return false;
   }

   return true;
}

void CAmMeterMgt::Close()
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

char *CAmMeterMgt::GetMsg()
{
	return m_szMsg;
}

TOF CAmMeterMgt::Thread_Begin(void)
{
    Thread_End();

    if (m_hCom == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    // New Start Thread
    m_tThreadStop = false;
    m_pThread = AfxBeginThread(ThreadAmMeter, this);

    if (m_pThread == NULL)
    {
        Thread_End();
        return false;
    }

    return true;
}

TOF CAmMeterMgt::Thread_End(void)
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

UINT ThreadAmMeter(LPVOID pParam)
{
    int nRcvLen;
    int nRcvIdx;
    char szBuff[15];
    CAmMeterMgt *pAmMeter = (CAmMeterMgt *)pParam;

    nRcvIdx = 0;
    while ( (pAmMeter->m_tThreadStop == false) && (pAmMeter->m_hCom != INVALID_HANDLE_VALUE) )
    {
        Sleep(50);

		switch (pAmMeter->m_nAmMeter_State)
		{
		case _AMMETER_NONE:
			break;
		case _AMMETER_READ:
			nRcvLen = UartRxCount(pAmMeter->m_hCom);

			if (nRcvLen != 0)
			{
				UartRead(pAmMeter->m_hCom, &szBuff[nRcvIdx], nRcvLen);
				nRcvIdx += nRcvLen;
			}
			else
			{
				if (nRcvIdx > 0)
				{
					if (pAmMeter->m_tMsgComplete == true)
					{
						pAmMeter->m_szMsg[0] = 0;
						pAmMeter->m_tMsgComplete = false;
					}
					else
					{
						if (szBuff[1] != 4)
						{
							pAmMeter->m_szMsg[0] = 0;
							printf("szBuff[1] != 4\r\n");
						}
						else
						{
							szBuff[nRcvIdx] = 0;
							nRcvIdx = 0;
							if (strstr(szBuff, "OK") != NULL)
							{
								pAmMeter->m_tMsgComplete = true;
							}
							memcpy(pAmMeter->m_szMsg, szBuff, sizeof(szBuff));

							printf("szBuff[1] == 4\r\n");

							pAmMeter->m_nAmMeter_State = _AMMETER_NONE;
						}
					}
				}
			}
			break;
		case _AMMETER_WRITE:
			pAmMeter->m_szMsg[0] = 0;
			UartWrite(pAmMeter->m_hCom, &pAmMeter->m_Query, sizeof(stQuery));
			pAmMeter->m_nAmMeter_State = _AMMETER_READ;
			break;
		}
    }

    return 0;
}
