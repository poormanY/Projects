// ScrewMgt.cpp: implementation of the CScrewMgt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainApp.h"
#include "DialogSet.h"
#include "ScrewCounterMgt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// screw control    생산라인 1호기 : 0, 생산라인 3호기 : 1.
#define _SCREW_SERIAL   1


#define _SCREW_NONE         0
#define _SCREW_RST          1
#define _SCREW_READ         2

extern CDialogSet  *pSetupDlg;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScrewMgt::CScrewMgt()
{
    m_hCom = INVALID_HANDLE_VALUE;

    m_tThreadStop = false;
    m_pThread = NULL;

    memset(m_szMsg, 0, sizeof(m_szMsg));
    m_tMsgComplete = false;

    m_nCount = 0;
}

CScrewMgt::~CScrewMgt()
{
    Close();
}

bool CScrewMgt::Open(const char *a_pszCom)
{
    Close();

    m_hCom = UartSetup(a_pszCom, 9600);

   if (m_hCom == INVALID_HANDLE_VALUE)
   {
      return false;
   }

   return true;
}

void CScrewMgt::Close(void)
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


bool  CScrewMgt::AddCount(void)
{
    m_nCount++;

    return true;
}

bool  CScrewMgt::DelCount(void)
{
    if (m_nCount > 0)
    {
        m_nCount--;
        return true;
    }
    else
    {
        return false;
    }
}

void CScrewMgt::DelAllCount(void)
{
    m_nCount = 0;
}

char *CScrewMgt::GetMsg(void)
{
    return m_szMsg;
}

S16   CScrewMgt::GetCount(void)
{
    return m_nCount;
}

TOF  CScrewMgt::Thread_Begin(void)
{
    Thread_End();

    if (m_hCom == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    // New Start Thread
    m_tThreadStop = false;
    m_pThread = AfxBeginThread(ThreadScrew, this);

    if (m_pThread == NULL)
    {
        Thread_End();
        return false;
    }

    return true;
}

TOF  CScrewMgt::Thread_End(void)
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

bool CScrewMgt::SetSendMsg(const char *szMeg, int nRepeatNum)
{
	char szSndBuff[256];
	int  nCount;

	memset(szSndBuff, 0x00, sizeof(szSndBuff));

	strcpy_s(szSndBuff, szMeg);
	
	for (nCount=0; nCount<nRepeatNum; nCount++)
	{
		UartWrite(m_hCom, &szSndBuff, strlen(szSndBuff));
		Sleep(250);
	}

	return true;
}

UINT ThreadScrew(LPVOID pParam)
{
    int nRcvLen;
    int nRcvIdx;
    char szBuff[256];
    CScrewMgt *pScrew = (CScrewMgt *)pParam;

    nRcvIdx = 0;
    while ( (pScrew->m_tThreadStop == false) && (pScrew->m_hCom != INVALID_HANDLE_VALUE) )
    {
        Sleep(50);
#if _SCREW_SERIAL
		if (pScrew->m_nScrew_State==_SCREW_NONE)
		{
			//printf("_SCREW_NONE\r\n");
			nRcvLen = UartRxCount(pScrew->m_hCom);
			if (nRcvLen != 0)
			{
				UartRead(pScrew->m_hCom, &szBuff[nRcvIdx], nRcvLen);
				if (strstr(szBuff, "*OK") != NULL)
				{
					pScrew->SetSendMsg("*OK", 2);
					memcpy(pScrew->m_szMsg, szBuff, sizeof(szBuff));
					pScrew->m_tMsgComplete = true;
				}
				nRcvIdx = 0;
			}
		}
		else if (pScrew->m_nScrew_State==_SCREW_RST)
		{
			//printf("_SCREW_RST\r\n");
			pScrew->SetSendMsg("RESET", 1);
			pScrew->m_nScrew_State = _SCREW_NONE;
		}
		else if (pScrew->m_nScrew_State==_SCREW_READ)
		{
			//printf("_SCREW_READ\r\n");
			nRcvLen = UartRxCount(pScrew->m_hCom);
			if (nRcvLen != 0)
			{
				UartRead(pScrew->m_hCom, &szBuff[nRcvIdx], nRcvLen);
				nRcvIdx += nRcvLen;
			}
			else
			{
				if (nRcvIdx > 0)
				{
					if (pScrew->m_tMsgComplete == true)
					{
						pScrew->m_szMsg[0] = 0;
						pScrew->m_tMsgComplete = false;
						
						if (strstr(szBuff, "*OK") != NULL)
						{
							pScrew->m_nScrew_State = _SCREW_NONE;
						}
					}
					else
					{
						szBuff[nRcvIdx] = 0;
						nRcvIdx = 0;
						//printf("pScrew->m_tMsgComplete != true\r\n");
						if (strstr(szBuff, "*RJCK") != NULL)
						{
							pScrew->SetSendMsg("*01", 2);
							//printf("pScrew->SetSendMsg01\r\n");
						}
						else if (strstr(szBuff, "*TYPE:AAVM        ") != NULL)
						{
							pScrew->SetSendMsg("*TYPE:AAVM        ", 2);
						}
						else if (strstr(szBuff, "*TYPE:ALL       ") != NULL)
						{
							pScrew->SetSendMsg("*TYPE:ALL       ", 2);
						}
						else if (strstr(szBuff, "*OK") != NULL)
						{
							pScrew->SetSendMsg("*OK", 2);
							memcpy(pScrew->m_szMsg, szBuff, sizeof(szBuff));
						}
						else
						{
							pScrew->SetSendMsg("*TYPE:AAVM        ", 2);
						}


						pScrew->m_tMsgComplete = true;
					}
				}
			}
		}
#else
		nRcvLen = UartRxCount(pScrew->m_hCom);
        if (nRcvLen != 0)
        {
            UartRead(pScrew->m_hCom, &szBuff[nRcvIdx], nRcvLen);
            nRcvIdx += nRcvLen;
        }
        else
        {
            if (nRcvIdx > 0)
            {
                if (pScrew->m_tMsgComplete == true)
                {
                    pScrew->m_szMsg[0] = 0;
                    pScrew->m_tMsgComplete = false;
                }
                else
                {
					szBuff[nRcvIdx] = 0;
                    nRcvIdx = 0;
                    if (strstr(szBuff, "OK") != NULL)
                    {
                        pScrew->AddCount();
                        pScrew->m_tMsgComplete = true;
                    }
                    strcpy_s(pScrew->m_szMsg, szBuff);

                }
            }
        }
#endif
    }

    return 0;
}
