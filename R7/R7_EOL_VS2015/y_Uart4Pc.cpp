// Uart.c: Kim JinWoo. 20004-06-01

/* ------------------------- Note -----------------------------------
   Menu > Project > Settings > Select this <File> & C/C++ Tab > Category = Precompiled Haders > Not Using Precompiled Header

   ------------------------------------------------------------------ */

#include "stdafx.h"
#include "y_Uart4Pc.h"

DWORD dwUartError;
DWORD dwUartBaudRate;

HANDLE UartSetup(const char *a_pszCom, DWORD a_lBaudRate)
{
   HANDLE hCom;
   //char szComName[32];
	CString szComName;

	//strcpy(szComName, "\\\\.\\");
	//strcpy(szComName+strlen(szComName), a_pszCom);
	szComName.Format(_T("\\\\.\\"));
	szComName += a_pszCom;
	// open COMM device
   hCom = CreateFile(
            szComName,                     // pointer to name of the file
            GENERIC_READ | GENERIC_WRITE, // access (read-write) mode
            0,                            // share mode
            NULL,                         // pointer to security attributes
            OPEN_EXISTING,                // how to create
            FILE_ATTRIBUTE_NORMAL,        // file attributes
            NULL                          // handle to file with attributes to copy
           );

   if (hCom == INVALID_HANDLE_VALUE)
   {
      dwUartError = GetLastError();
      return hCom;
   }

   // setup device buffers
   SetupComm( hCom,
      0x10000,       // size of input  buffer = 64KBytes
      0x10000        // size of output buffer = 64KBytes
     );

   // purge any information in the buffer
   PurgeComm( hCom, PURGE_TXABORT | PURGE_RXABORT |
                    PURGE_TXCLEAR | PURGE_RXCLEAR ) ;

   // Setup Time Out
   COMMTIMEOUTS CommTimeOuts;

   CommTimeOuts.ReadIntervalTimeout = MAXDWORD;

   CommTimeOuts.ReadTotalTimeoutMultiplier = 0 ;
   CommTimeOuts.ReadTotalTimeoutConstant   = 0 ;

   CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
   CommTimeOuts.WriteTotalTimeoutConstant   = 0;

   SetCommTimeouts( hCom, &CommTimeOuts ) ;

   // Setup Connection
   DCB dcb ;

   dcb.DCBlength          = sizeof(DCB) ;   // sizeof(DCB)

   GetCommState( hCom, &dcb ) ;

   dcb.BaudRate           = a_lBaudRate ;    // current baud rate:
                                             //   CBR_2400 ,CBR_4800 ,CBR_9600 ,CBR_14400,
                                             //   CBR_19200,CBR_38400,CBR_57600,CBR_115200
   dcb.fBinary            = true  ;          // binary mode, no EOF check
   dcb.fParity            = false ;          // enable parity checking
   dcb.ByteSize           = 8     ;          // number of bits/byte, 4-8
   dcb.Parity             = 0     ;          // 0-4=no,odd,even,mark,space
   dcb.StopBits           = 0     ;          // 0,1,2 = 1, 1.5, 2

   // other members setting
   dcb.fOutxCtsFlow       = false ;          // CTS output flow control
   dcb.fOutxDsrFlow       = false ;          // DSR output flow control
   dcb.fDtrControl        = DTR_CONTROL_DISABLE ;  // DTR flow control type
   dcb.fDsrSensitivity    = false ;          // DSR sensitivity
   dcb.fTXContinueOnXoff  = false ;          // XOFF continues Tx
   dcb.fOutX              = false ;          // XON/XOFF out flow control
   dcb.fInX               = false ;          // XON/XOFF in flow control
   dcb.fErrorChar         = false ;          // enable error replacement
   dcb.fNull              = false ;          // enable null stripping
   dcb.fRtsControl        = RTS_CONTROL_DISABLE ;  // RTS flow control
   dcb.fAbortOnError      = false ;          // abort reads/writes on error

   // don't change
// dcb.fDummy2            = 0     ;          // reserved
// dcb.wReserved          = 0     ;          // not currently used
// dcb.XonLim             = 2048  ;          // transmit XON threshold
// dcb.XoffLim            = 512   ;          // transmit XOFF threshold
// dcb.XonChar            = 17    ;          // Tx and Rx XON character
// dcb.XoffChar           = 19    ;          // Tx and Rx XOFF character
// dcb.ErrorChar          = 0     ;          // error replacement character
// dcb.EofChar            = 0     ;          // end of input character
// dcb.EvtChar            = 0     ;          // received event character
// dcb.wReserved1         = 0     ;          // reserved; do not use

   if (SetCommState( hCom, &dcb ) == false)
   {
      dwUartError = GetLastError();
      CloseHandle( hCom );
      return (HANDLE) -1;
   }

   dwUartBaudRate = a_lBaudRate;
   return ( hCom ) ;
}

int UartWrite(HANDLE a_hCom, const void *a_pData, int a_nLen)
{
   BOOL  tResult;
   DWORD dwLen;

   if (a_hCom <= 0) return 0;
   
   tResult = WriteFile( 
               a_hCom,     // handle to file to write to
               a_pData,    // pointer to data to write to file
               a_nLen,     // number of bytes to write. Limited 65,535 bytes
               &dwLen,     // pointer to number of bytes written
               NULL        // pointer to structure for overlapped I/O
              );

   return (int)dwLen;
}

int UartRxCount(HANDLE a_hCom)
{
   DWORD   dwComError;
   COMSTAT ComStat;
   
   ClearCommError(a_hCom, &dwComError, &ComStat);
   return ComStat.cbInQue;
}

int UartRead(HANDLE a_hCom, void *a_pData, int a_nLen)
{
   DWORD dwLen;
   int   nLen;

   if (a_hCom <= 0) return 0;

   nLen = UartRxCount(a_hCom);
   if (nLen < a_nLen) a_nLen = nLen;

   ReadFile(a_hCom, a_pData, a_nLen, &dwLen, NULL);

   return (int)dwLen;
}

DWORD UartCalcTransTime(DWORD a_dwUartBaudRate, DWORD a_dwLen)   // Miliseconds
{
   // 1000: milisecond, 12: bits / byte
   if (a_dwUartBaudRate <= 0) return 0;
   return ((a_dwLen * 1000L * 12L) / a_dwUartBaudRate);
}

void DelayMs(DWORD a_lMiliSec)
{
   DWORD lStart;
   
   lStart = GetTickCount();
   while (GetTickCount() < (lStart + a_lMiliSec));
}

