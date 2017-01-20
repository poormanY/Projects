#ifndef _PCAN_AUTO_RX_H_
#define _PCAN_AUTO_RX_H_

#include "CanProtocol.h"

class CPcanAutoRx
{
public:
    CPcanAutoRx(U32 a_wId, U32 a_wId_Mask, void (*a_pfCallBack)(CAN_MSG* a_pCanMsg));
    void CallBack(void);

    CAN_MSG  m_CanMsg;
    void   (*m_pfCallBack)(CAN_MSG* a_pCanMsg);

    U32 m_wId_Mask;   // bit '1' = don't care, bit '0'= match
    U32 m_wId_Match;
};

#endif // _CAN_INMSG_H_
