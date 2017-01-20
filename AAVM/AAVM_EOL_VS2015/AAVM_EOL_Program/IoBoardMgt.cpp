// IoBoardMgt.cpp: implementation of the CIoBoardMgt class.
//
//////////////////////////////////////////////////////////////////////
//  KM 6050 Test Result: KM 6050은 7개의 Digital Inputs, 8개의 Digital Output 포트로 구성되어 있다.
//      Send:?ID
//      Rcv :
//      1.ID 01                 <- 장치 번호는 1번이다.
//      2.Model KM 6050         <- 모델은 KM 6050이다.
//      3.wR/W 0010,1010,2010   <- 워드 단위로 읽기/쓰기가 가능한 메모리는 0x00번지 16개, 0x10번지 16개, 0x20번지 16개
//      4.wR// 8010,9010,A010   <- 워드 단위로 읽기만      가능한 메모리는 0x80번지 16개, 0x90번지 16개, 0xA0번지 16개
//      5.bR/W 0010             <- 비트 단위로 읽기/쓰기가 가능한 메모리는 0x00번지 16개
//      6.bR// 8010             <- 워드 단위로 읽기만      가능한 메모리는 0x80번지 16개
//
//      Send:$01P8007           <- 01번 장치의 0x80번지에서 7개의 데이터를 읽어라.
//      Rcv :!01P8007,0111111   <- Lo(DI0),Hi(DI1),Hi(DI2),Hi(DI3),Hi(DI4),Hi(DI5),Hi(DI6)
//      Send:$01A0008,1-------  <- 01번 장치의 0x00번지에서 8개의 데이터를 설정한다. ('1'=Hi, '0'=Lo, '-'=No Change)
//      Rcv :!01A0008,1-------  <- 그대로 응답만 한다.
//      Send:$01P0008           <- 01번 장치의 0x00번지에서 8개의 데이터를 읽어라.
//      Rcv :!01P0008,10000000  <- Hi(DO0),Lo(DO1),Lo(DO2),Lo(DO3),Lo(DO4),Lo(DO5),Lo(DO6),Lo(DO7)
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainApp.h"
#include "IoBoardMgt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIoBoardMgt::CIoBoardMgt()
{
    m_hCom = INVALID_HANDLE_VALUE;

    memset(m_szMsg, 0, sizeof(m_szMsg));

    m_wMsgCnt = 0;
}

CIoBoardMgt::~CIoBoardMgt()
{
    Close();
}

bool CIoBoardMgt::Open(const char *a_pszCom)
{
    Close();

    m_hCom = UartSetup(a_pszCom, 9600);

   if (m_hCom == INVALID_HANDLE_VALUE)
   {
      return false;
   }

   return true;
}

void CIoBoardMgt::Close(void)
{
    if (m_hCom != INVALID_HANDLE_VALUE)
    {
        PurgeComm( m_hCom, PURGE_TXABORT | PURGE_RXABORT |
                           PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
        CloseHandle( m_hCom );

        m_hCom = INVALID_HANDLE_VALUE;
    }
}

char *CIoBoardMgt::GetMsg(void)
{
    return m_szMsg;
}

U32  CIoBoardMgt::GetMsgCnt(void)
{
    return m_wMsgCnt;
}

bool CIoBoardMgt::TransferMsg(const char *a_pszTx)
{
    int nLen;
    U32 wRxWaitTime;
    int iRxCount;

    // Check Error

    if ( (m_hCom == INVALID_HANDLE_VALUE) || (a_pszTx == NULL) )
    {
        return false;
    }

   // Clear Dummy Rx Buffer
   iRxCount = UartRxCount(m_hCom);
   while (iRxCount > 0)
   {
      UartRead(m_hCom, m_szMsg, iRxCount);
      Sleep(100);
      iRxCount = UartRxCount(m_hCom);
   }

   // Transmit Start
    nLen = strlen(a_pszTx);
    UartWrite(m_hCom, a_pszTx, nLen);

   // Wait Rx Start
    wRxWaitTime = GetTickCount() + 500;

    while ( UartRxCount(m_hCom) == 0 )
    {
        Sleep(50);
        if ( GetTickCount() >  wRxWaitTime )
        {
            return false;
        }
    }

    // Receive Rx
    nLen = UartRxCount(m_hCom);
    UartRead(m_hCom, m_szMsg, nLen);

    while (true)
    {
        Sleep(50);

        iRxCount = UartRxCount(m_hCom);
        if (iRxCount == 0)
        {
            m_szMsg[nLen] = 0;
            while (nLen > 0)
            {
                if ( (m_szMsg[nLen-1] == '\r') || (m_szMsg[nLen-1] == '\n') )
                {
                    nLen--;
                    m_szMsg[nLen] = 0;
                }
                else
                {
                    break;
                }
            }

            m_wMsgCnt++;
            break;
        }
        else
        {
            UartRead(m_hCom, &m_szMsg[nLen], iRxCount);
            nLen += iRxCount;
        }
    }

    return true;
}

U16  CIoBoardMgt::GetInPort(void)
{
//      Send:$01P8007           <- 01번 장치의 0x80번지에서 7개의 데이터를 읽어라.
//      Rcv :!01P8007,0111111   <- Lo(DI0),Hi(DI1),Hi(DI2),Hi(DI3),Hi(DI4),Hi(DI5),Hi(DI6)
    char*pszRx;
    U16 uRtnData, uBitData, i;

    //if (TransferMsg("$01P8007\r") == false)
	if (TransferMsg("$01P8010\r") == false)//KBR 10개의 데이터 read
    {
        return 0xFFFF;  // error return
    }
    
    pszRx = GetMsg();
    pszRx = strchr(pszRx, ',');
    if (pszRx == NULL)
    {
        return 0xFFFF;
    }

    pszRx++;
    uRtnData = 0;
    uBitData = 0x0001;

    for (i = 0; i < 15; i++)
    {
        if (pszRx[i] == '1')
        {
            uRtnData |= uBitData;
        }
        uBitData <<= 1;
    }

    return uRtnData;
}

U16  CIoBoardMgt::GetOutPort(void)
{
//      Send:$01P0008           <- 01번 장치의 0x00번지에서 8개의 데이터를 읽어라.
//      Rcv :!01P0008,10000000  <- Hi(DO0),Lo(DO1),Lo(DO2),Lo(DO3),Lo(DO4),Lo(DO5),Lo(DO6),Lo(DO7)
    char*pszRx;
    U16 uRtnData, uBitData, i;

    //if (TransferMsg("$01P0008\r") == false)
    if (TransferMsg("$01P0010\r") == false)  //KBR 10개의 데이터 write
	{
        return 0xFFFF;  // error return
    }
    
    pszRx = GetMsg();
    pszRx = strchr(pszRx, ',');
    if (pszRx == NULL)
    {
        return 0xFFFF;
    }

    pszRx++;
    uRtnData = 0;
    uBitData = 0x0001;

    for (i = 0; i < 16; i++)
    {
        if (pszRx[i] == '1')
        {
            uRtnData |= uBitData;
        }
        uBitData <<= 1;
    }

    return uRtnData;
}

U16  CIoBoardMgt::PutOutPort(U16 a_bData)
{
//      Send:$01A0008,1-------  <- 01번 장치의 0x00번지에서 8개의 데이터를 설정한다. ('1'=Hi, '0'=Lo, '-'=No Change)
//      Rcv :!01A0008,1-------  <- 그대로 응답만 한다.
    //             012345678901234567
    //char szTx[] = "$01A0008,01234567\r";
	char szTx[] = "$01A0010,01234567--------\r";//KBR 초기화 16비트
    U16 i;

    for (i = 0; i < 16; i++)
    {
        szTx[9+i] = (a_bData & 0x01)? '1': '0';
        a_bData >>= 1;
    }

    if (TransferMsg(szTx) == false)
    {
        return 0xFFFF;  // error return
    }

    return GetOutPort();
}

U16  CIoBoardMgt::PutOutBit(U16 a_bBitPos, bool a_tHi)
{
    //             012345678901234567
    //char szTx[] = "$01A0008,--------\r";
	char szTx[] = "$01A0010,----------------\r";//KBR  초기화 16비트
    if (a_bBitPos >= 16)
    {
        return 0xFFFF;
    }

    szTx[9+a_bBitPos] = (a_tHi == false)? '0': '1';

    if (TransferMsg(szTx) == false)
    {
        return 0xFFFF;  // error return
    }

    return GetOutPort();
}
