// Uart4Pc.h: Kim JinWoo. 20004-06-01

/* ------------------------- Note -----------------------------------
   Menu > Project > Settings > Select this <File> & C/C++ Tab > Category = Precompiled Haders > Not Using Precompiled Header

   ------------------------------------------------------------------ */

#ifndef Y_UART4PC_H
#define Y_UART4PC_H

extern DWORD dwUartError;
extern DWORD dwUartBaudRate;

HANDLE UartSetup(const char *a_pszCom, DWORD a_lBaudRate);

int UartWrite(HANDLE a_hCom, const void *a_pData, int a_nLen);
int UartRxCount(HANDLE a_hCom);
int UartRead(HANDLE a_hCom, void *a_pData, int a_nLen);

DWORD UartCalcTransTime(DWORD a_dwUartBaudRate, DWORD a_dwLen);   // Miliseconds
void DelayMs(DWORD a_lMiliSec);

#endif //  !Y_UART4PC_H  //
