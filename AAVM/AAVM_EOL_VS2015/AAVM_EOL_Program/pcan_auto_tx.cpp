#include "StdAfx.h"
#include "pcan_auto_tx.h"
#include "GetTickCountEx.h"

CPcanAutoTx::CPcanAutoTx(U32 a_wId, U32 a_wIntervalMsec, void (*a_pfCallBack)(CAN_MSG* a_pCanMsg))
{
    m_CanMsg.wId       = a_wId;
    m_CanMsg.bLen      = 8; // default CAN LEN = 8
    m_CanMsg.uCnt      = 0;
    m_CanMsg.wTimeMsec = 0;

    m_pfCallBack = a_pfCallBack;

    m_wIntervalMsec = a_wIntervalMsec;
    m_tPause = false;
}

void CPcanAutoTx::CallBack(void)
{
    m_CanMsg.wTimeMsec = GetTickCountEx();
    m_pfCallBack(&m_CanMsg);
    m_CanMsg.uCnt++;;
}
