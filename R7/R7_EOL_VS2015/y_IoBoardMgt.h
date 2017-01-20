// BarcodeMgt.h: interface for the CBarcodeMgt class.
//
//////////////////////////////////////////////////////////////////////

#ifndef Y_IOBOARDMGT_H
#define Y_IOBOARDMGT_H

#include "y_JDEFINE.h"

////////////////////////////////////
// Input Pin
#define _PIN_IN_CONNECT		0x0001
#define _PIN_IN_1			0x0002
#define _PIN_IN_2			0x0004
#define _PIN_IN_3			0x0008
#define _PIN_IN_4			0x0010
#define _PIN_IN_5			0x0020
#define _PIN_IN_6			0x0040
#define _PIN_IN_7			0x0080
#define _PIN_IN_8			0x0100
#define _PIN_IN_9			0x0200
#define _PIN_IN_10			0x0400
#define _PIN_IN_11			0x0800
#define _PIN_IN_12			0x1000
#define _PIN_IN_13			0x2000
#define _PIN_IN_14			0x4000
#define _PIN_IN_15			0x8000
////////////////////////////////////
// Output Pin
#define _BIT_OUT_POWER			0
#define _BIT_OUT_SILINDER_EN	1
#define _BIT_OUT_LED_Y_POWER	2
#define _BIT_OUT_LED_G_OK		3
#define _BIT_OUT_LED_R_NG		4
#define _BIT_OUT_NC_5			5
#define _BIT_OUT_CAN_SHORT		6
#define _BIT_OUT_BH_RESET		7	//_BIT_OUT_TURN_L to _BIT_OUT_BH_RESET [IA]
#define _BIT_OUT_LKAS_BTN		8	//_BIT_OUT_TURN_R to _BIT_OUT_LCAS_BTN [IA]
#define _BIT_OUT_NC_9			9
#define _BIT_OUT_WIPPER			10
#define _BIT_OUT_TURN_R			11
#define _BIT_OUT_TURN_L			12
#define _BIT_OUT_BARCODE1		13
#define _BIT_OUT_BARCODE2		14
#define _BIT_OUT_LDWS_SW		15

class CIoBoardMgt
{
public:
	CIoBoardMgt();
	virtual ~CIoBoardMgt();

    // Functions
    bool Open(const char *a_pszCom);
    void Close(void);
    char *GetMsg(void);
    U32  GetMsgCnt(void);
    bool TransferMsg(const char *a_pszTx);

    // Port Control
    U16  GetInPort(void);
    U16  GetOutPort(void);
    U16  PutOutPort(U16 a_bData);
    U16  PutOutBit(U16 a_bBitPos, bool a_tHi);

    // Member Variables
    HANDLE m_hCom;

    U32  m_wMsgCnt;
    char m_szMsg[1024];
};

#endif //  !Y_IOBOARDMGT_H  //
