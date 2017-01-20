#ifndef _PCAN_AUTO_TX_H_
#define _PCAN_AUTO_TX_H_

#include "CanProtocol.h"

class CPcanAutoTx
{
public:
    CPcanAutoTx(U32 a_wID, U32 a_wIntervalMsec, void (*a_pfCallBack)(CAN_MSG* a_pCanMsg));
    void CallBack(void);

    CAN_MSG  m_CanMsg;
    void   (*m_pfCallBack)(CAN_MSG* a_pCanMsg);

    U32 m_wIntervalMsec;
    TOF m_tPause;
};

#endif // _PCAN_AUTO_TX_H_
