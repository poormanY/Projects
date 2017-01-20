#include "StdAfx.h"
#include "pcan_auto_rx.h"
#include "GetTickCountEx.h"

CPcanAutoRx::CPcanAutoRx(U32 a_wId, U32 a_wId_Mask, void (*a_pfCallBack)(CAN_MSG* a_pCanMsg))
{
    m_CanMsg.uCnt      = 0;
    m_CanMsg.wTimeMsec = 0;

    m_pfCallBack = a_pfCallBack;

    m_wId_Match = a_wId;
    m_wId_Mask  = a_wId_Mask;
}

void CPcanAutoRx::CallBack(void)
{
    m_CanMsg.wTimeMsec = GetTickCountEx();
    m_CanMsg.uCnt++;
    m_pfCallBack(&m_CanMsg);
}
