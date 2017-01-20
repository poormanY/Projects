#include "stdafx.h"
#include "MainApp.h"
#include "MainDlg.h"
#include "DialogSet.h"
#include "ExpressImg.h"
#include "pcan_inf.h"
#include "wm_user.h"
#include "barcodemgt.h"
#include "ScrewCounterMgt.h"
#include "IoBoardMgt.h"
#include "AmMeterMgt.h"
#include "can_msg.h"
#include "FrontMoniter.h"
#include "RearMoniter.h"
#include "LeftMoniter.h"
#include "RightMoniter.h"
#include "sys_ctimer.h"
#include "GetTickCountEx.h"

#include <mmsystem.h>

#define Sigle_PC_TEST // Define = EOL Undefine = simplicity zig
#define WARRNING_EN

#define _PIN_IN_CONNECT     0x01
#define _PIN_IN_12V_OVER    0x02
#define _PIN_IN_12V_UNDER   0x04
#define _PIN_IN_24V_OVER    0x08
#define _PIN_IN_24V_UNDER   0x10
//psj can line test
#define _PIN_IN_CANLINE     0x20

#define _PIN_OUT_POWER      0x0001
#define _PIN_OUT_DISCONNECT 0x0002
#define _PIN_OUT_LED_Y      0x0004
#define _PIN_OUT_LED_G      0x0008
#define _PIN_OUT_LED_R      0x0010
#define _PIN_OUT_LDWS_BTN   0x8000
#define _PIN_OUT_CAN_SHORT  0x0040
#define _PIN_OUT_SET_VOLT   0x0080
#define _PIN_OUT_TURN_L     0x1000
#define _PIN_OUT_TURN_R     0x0800
#define _PIN_OUT_WIPPER     0x0400
#define _PIN_OUT_BARCODE1   0x2000
#define _PIN_OUT_BARCODE2   0x4000
//#define _PIN_OUT_BARCODE3   0x8380


#define _BIT_OUT_POWER      0
#define _BIT_OUT_DISCONNECT 1
#define _BIT_OUT_LED_Y      2
#define _BIT_OUT_LED_G      3
#define _BIT_OUT_LED_R      4
#define _BIT_OUT_LDWS_BTN   15
#define _BIT_OUT_CAN_SHORT  6
#define _BIT_OUT_SET_VOLT   7
#define _BIT_OUT_TURN_L     12
#define _BIT_OUT_TURN_R     11
#define _BIT_OUT_WIPPER     10
#define _BIT_OUT_BARCODE1   13
#define _BIT_OUT_BARCODE2   14

#define _AMMETER_NONE       0
#define _AMMETER_READ       1
#define _AMMETER_WRITE      2

#define _SCREW_NONE         0
#define _SCREW_RST          1
#define _SCREW_READ         2

#define _ADC_VOL_12         1989

DWORD m_sleep_time;

extern MainDlg     *pMainDlg;
extern CDialogSet  *pSetupDlg;
extern CExpressImg *pImgDlg;
//extern CCanProtocol*pCanprt;

extern CFrontImg   *pFImgDlg;
extern CRearImg	   *pReImgDlg;
extern CLeftImg	   *pLImgDlg;
extern CRightImg   *pRImgDlg;


extern int m_nImgView;
extern int tImageShowCompleted;
extern int  nPCAN_WriteState;

int			TempMax;
int			TempMin;
CString		TempData;

//static CPcan      * pCan      = NULL;
static CBarcodeMgt* pBarCode1 = NULL;
static CBarcodeMgt* pBarCode2 = NULL;
static CScrewMgt  * pScrew    = NULL;
static CIoBoardMgt* pIoBoard  = NULL;
//psj Ammeter
static CAmMeterMgt* pAmMeter  = NULL;

bool StopThread = false; //thlee

typedef enum
{
    _MODE_MASTER_GOOD,
    _MODE_MASTER_BAD,
    _MODE_MAIN,
	_MODE_MAIN_2TH
} TEST_MODE;

#define _MODE_START_DEFAULT _MODE_MASTER_GOOD
//#define _MODE_START_DEFAULT _MODE_MAIN
///////////////////////////////////////////////////////////
// STEP Define에 대한 값 설정 규칙
//  1. 각 단계는 작업 순서대로 비트를 설정한다.
//  2. 00-08 비트는 검사 사전작업에 할단된다.
//  3. 09-23 비트는 검사 작업에 할당된다.
//  4. 24-31 비트는 검사 사후작업에 할당된다.
#define _STEP_INIT              0x00000000
#define _STEP_BARCODE1_BOX      0x00000001
#define _STEP_BARCODE1_PCB      0x00000002
#define _STEP_SCREW             0x00000004
#define _STEP_BARCODE1_CASE     0x00000008
#define _STEP_CONNECT           0x00000040
#define _STEP_TEST_START        0x00000100
#define _STEP_TEST_CAL_ON       0x00000200
#define _STEP_TEST_MAIN         0x00000400
#define _STEP_TEST_CAL_OFF      0x00400000
#define _STEP_TEST_END          0x00800000
#define _STEP_BARCODE2_CASE     0x01000000
#define _STEP_SAVE_LOG          0x04000000
#define _STEP_END               0x80000000
#define WHITE    	RGB(255,255,255) //thlee

///////////////////////////////////////////////////////////
// BOX 최대 유닛 개수
// HG : 1상자 12개 나머지 모든 제품은 1상자 18개
#define _BOX_HG_UNIT_CNT        0x0000000C
#define _BOX_DEFAULT_UNIT_CNT   0x00000012

typedef enum
{
    _ERR_OK,            // 정상
    _ERR_FAIL,          // 테스트 실패
    _ERR_TIMEOVER,      // 검사시간 초과
    _ERR_DISCONNECT,    // 장치 연결 끊김
    _ERR_POWER,         // 전원 이상
	_ERR_VOLTMETER,     // 전원 측정 이상
	_ERR_AMPERE,        // 전류 이상
    _ERR_MANUAL,        // 작업자에 의한 불량 판정(불량처리)
    _ERR_STOP,          // 검사 중지 -> 테스트 중지
    _ERR_SYSTEM,        // 검사 장비 에러 -> 테스트 중지
	//psj can line test
	_ERR_CANLINE,        // CAN 라인 에러
	_ERR_CANSHORT       //kbr CANSHORT
} TEST_ERR;

typedef enum
{
    _LOCAL_KOR,         // 내수
    _LOCAL_CHA,         // 테스트 실패
    _LOCAL_EUR,         // 검사시간 초과
    _LOCAL_MES         // 장치 연결 끊김
} LOCAL_STATE;

typedef enum
{
	_TURN_OFF,          // 턴신호 OFF
    _TURN_LEFT_ON,      // 좌측턴신호 ON
    _TURN_RIGHT_ON      // 우측턴신호 ON
} TEST_TURN;

bool        tMainThreadStop       = false;
bool        tMainThreadTestError  = false;
bool        tMainThreadManulError = false;
TEST_ERR    iMainThreadTestError  = _ERR_OK;
U32         wMainThreadTestStep   = _STEP_INIT;
TEST_MODE   iMainThreadTestMode   = _MODE_MASTER_GOOD;
LOCAL_STATE iLocalState           = _LOCAL_KOR;
bool        tMainThreadHapticMode = false;
bool		tBoxBarcodeState;
CPcan* pCan;
unsigned int Cal_REC;


static const char *GetTestErrString(TEST_ERR a_iTestErr)
{
    char *pszMsg;

    switch (a_iTestErr)
    {
    case _ERR_OK:
        pszMsg = "검사 완료";
        break;
    case _ERR_FAIL:
        pszMsg = "검사 실패";
        break;
    case _ERR_TIMEOVER:
        pszMsg = "시간 초과";
        break;
    case _ERR_DISCONNECT:
        pszMsg = "장치 연결 끊김";
        break;
    case _ERR_POWER:         // 전원 이상
        pszMsg = "전원 이상";
        break;
	case _ERR_VOLTMETER:     // 전원 측정 이상
        pszMsg = "전원 측정 이상";
        break;
	case _ERR_AMPERE:        // 전류 이상
		pszMsg = "전류 이상";
		break;
    case _ERR_MANUAL:        // 작업자에 의한 불량 판정
        pszMsg = "불량 처리";
        break;
    case _ERR_STOP:          // 검사 중지
        pszMsg = "검사 중지";
        break;
    case _ERR_SYSTEM:        // 검사 장비 에러
        pszMsg = "검사 장비 에러";
        break;
		//psj can line test
    case _ERR_CANLINE:       // CAN 라인 에러
        pszMsg = "CAN 라인 에러";
        break;
    //kbr CANSHORT
#if 0
	case _ERR_CANSHORT:
		pszMsg = "CAN SHORT 에러";
        break;
#endif
    default:     // 특수3
        pszMsg = "미지정 에러";
        break;
    }

    return pszMsg;
}

unsigned char HexToByte(char hex)
{
	switch(hex)
	{
		case '0' : return 0;
		case '1' : return 1;
		case '2' : return 2;
		case '3' : return 3;
		case '4' : return 4;
		case '5' : return 5;
		case '6' : return 6;
		case '7' : return 7;
		case '8' : return 8;
		case '9' : return 9;
		case 'A' : case 'a' : return 10;
		case 'B' : case 'b' : return 11;
		case 'C' : case 'c' : return 12;
		case 'D' : case 'd' : return 13;
		case 'E' : case 'e' : return 14;
		case 'F' : case 'f' : return 15;
	}

	return 0;
}

static TEST_ERR CheckLoopCondition(U32 a_wTimeLimit, TOF a_tConnected = true)
{
    U16 uInPort;

    Sleep(50);

    // 전원 테스트
    if (a_tConnected == true)
    {
        uInPort = pIoBoard->GetInPort();

        if ((uInPort & _PIN_IN_CONNECT) != 0)
        {
            return _ERR_DISCONNECT;
        }
    }

    if (tMainThreadManulError == true)
    {
        return _ERR_MANUAL;
    }

    if (tMainThreadStop == true)
    {
        return _ERR_STOP;
    }

    if (GetTickCount() > a_wTimeLimit)
    {
        return _ERR_TIMEOVER;
    }
	//kbr can short test
#if 0
	if(pIoBoard->GetOutPort() & 0x40)
	{
	   return _ERR_CANSHORT;
	}
#endif
    return _ERR_OK;
}
static bool OpenCan(void)
{
	///////////////////////////////////////////
    // 초기화 상태가 맞는지 확인한다.
    ///////////////////////////////////////////
    if ( (pCan != NULL) )
    {
        dprintf(("<== Can't Open PCAN. CPcan ran!\n"));
        return false;
    }

	///////////////////////////////////////////
    // 1ms Timer를 설치한다. (CPcan에서 사용됨)
    ///////////////////////////////////////////
    if (GetTickCountEx_Init() == false)
    {
        dprintf(("<== CTimer Init Error!\n"));
        return false;
    }
    ///////////////////////////////////////////
    // 10ms Timer를 설치한다. (j1939에서 사용됨)
    ///////////////////////////////////////////
    if (CTimerInit() == false)
    {
        dprintf(("<== CTimer Init Error!\n"));
        return false;
    }
	///////////////////////////////////////////
    // CPCan 개체를 생성한다.
    ///////////////////////////////////////////
    pCan = new CPcan;
    if (pCan == NULL)
    {
        return false;
    }
    dprintf(("<== Create CPcan Complete!\n"));

    ///////////////////////////////////////////
    // PCANBasic.dll를 로드한다.
    ///////////////////////////////////////////
    if (pCan->Open() == false)
    {
        return false;
    }
    dprintf(("<== Open Complete!\n"));

    ///////////////////////////////////////////
    // 500kbps, Standard(11bits) or Extended(29bits) mode 로 연결한다.
    ///////////////////////////////////////////
    if (pCan->Connect(500, true) == false)
    {
        return false;
    }
    dprintf(("<== Connect Complete!\n"));

    ///////////////////////////////////////////
    // 자동으로 지정된 주기마다 CAN 메시지를 송신하는 개체를 등록한다.
    ///////////////////////////////////////////

	CPcanAutoTx *pAutoTx;
    pAutoTx = new CPcanAutoTx((6<<_BITP_J1939_ID_PRIO)+_CAN_ID_MMS, 200, MakeMsg_MMS);
    pCan->AutoTx_Add(pAutoTx);

    pAutoTx = new CPcanAutoTx((6<<_BITP_J1939_ID_PRIO)+_CAN_ID_HP1, 200, MakeMsg_HP1);
    pCan->AutoTx_Add(pAutoTx);

    pAutoTx = new CPcanAutoTx((6<<_BITP_J1939_ID_PRIO)+_CAN_ID_HP3, 200, MakeMsg_HP3);
    pCan->AutoTx_Add(pAutoTx);

    pAutoTx = new CPcanAutoTx((6<<_BITP_J1939_ID_PRIO)+_CAN_ID_ESROS, 1000, MakeMsg_ESROS);
    pCan->AutoTx_Add(pAutoTx);

    pAutoTx = new CPcanAutoTx((6<<_BITP_J1939_ID_PRIO)+((_CAN_PGN_REQ+_CAN_ADDR_AAVM)<<8)+_CAN_ADDR_EOL, 1000, MakeMsg_REQ_AAVM_Auto);
    pCan->AutoTx_Add(pAutoTx);

    pAutoTx = new CPcanAutoTx((6<<_BITP_J1939_ID_PRIO)+((_CAN_PGN_REQ+_CAN_ADDR_AAVM)<<8)+_CAN_ADDR_EOL2, 1000, MakeMsg_REQ_AAVST_Auto);
    pCan->AutoTx_Add(pAutoTx);


	///////////////////////////////////////////
    // 지정된 ID로 수신된 CAN 메시지를 자동으로 처리하는 개체를 등록한다.
    ///////////////////////////////////////////
    CPcanAutoRx *pAutoRx;
    pAutoRx = new CPcanAutoRx((_CAN_PGN_AAVO<<8)+_CAN_ADDR_AAVM, 0x1F000000, UseMsg_AAVO);
    pCan->AutoRx_Add(pAutoRx);

    pAutoRx = new CPcanAutoRx((_CAN_PGN_AAVM<<8)+_CAN_ADDR_AAVM, 0x1F000000, UseMsg_AAVM);
    pCan->AutoRx_Add(pAutoRx);

    pAutoRx = new CPcanAutoRx((_CAN_PGN_AAVST<<8)+_CAN_ADDR_AAVM, 0x1F000000, UseMsg_AAVST);
    pCan->AutoRx_Add(pAutoRx);

    pAutoRx = new CPcanAutoRx((_CAN_PGN_CID<<8)+_CAN_ADDR_AAVM, 0x1F000000, UseMsg_CID);
    pCan->AutoRx_Add(pAutoRx);

    pAutoRx = new CPcanAutoRx((_CAN_PGN_VER1<<8)+_CAN_ADDR_AAVM, 0x1F000000, UseMsg_VER1);
    pCan->AutoRx_Add(pAutoRx);

    pAutoRx = new CPcanAutoRx((_CAN_PGN_VER2<<8)+_CAN_ADDR_AAVM, 0x1F000000, UseMsg_VER2);
    pCan->AutoRx_Add(pAutoRx);

    pAutoRx = new CPcanAutoRx((_CAN_PGN_MLST<<8)+_CAN_ADDR_AAVM, 0x1F000000, UseMsg_MLST);
    pCan->AutoRx_Add(pAutoRx);

    pAutoRx = new CPcanAutoRx(((_CAN_PGN_TPCM+0xFF)<<8)+_CAN_ADDR_AAVM, 0x1F000000, UseMsg_TPCM_BAM);
    pCan->AutoRx_Add(pAutoRx);

    pAutoRx = new CPcanAutoRx(((_CAN_PGN_TPDT+0xFF)<<8)+_CAN_ADDR_AAVM, 0x1F000000, UseMsg_TPDT_BAM);
    pCan->AutoRx_Add(pAutoRx);

    ///////////////////////////////////////////
	// 자동 송신/수신 기능을 수행하는 PCan의 쓰레드를 시작한다.
    ///////////////////////////////////////////
    pCan->Thread_Begin();
    dprintf(("<== PCAN Thread Begin.\n"));

	Sleep(100);

    return true;
}

void  ReOpencan(void)
{

	pCan->Reset();
	pCan->Disconnect();
	pCan->Close();

	OpenCan();
}

static bool OpenIoBoard(void)
{
    pIoBoard = new CIoBoardMgt;
    if (pIoBoard == NULL)
    {
        return false;
    }

    if (pIoBoard->Open((LPCSTR)pSetupDlg->m_EDIT_COM_IOBOARD) == false)
    {
        dprintf(("IO Board Open Fail!\n"));
        return false;
    }

    if (pIoBoard->TransferMsg("$01GA001\r") == false)
    {
        dprintf(("IO Board communication error!!\n"));
        return false;
    }

    if (strcmp(pIoBoard->GetMsg()+9, "6050") != 0)
    {
        dprintf(("IO Board model %s error!\n", pIoBoard->GetMsg()+9));
        return false;
    }

    return true;
}

static bool Step_ReadBarcode1_BOX(char *a_pszBarcodeBOX)
{
    TEST_ERR iError;
    TOF tOn = false;
    U32 wTimeMsec;

    wMainThreadTestStep |= _STEP_BARCODE1_BOX;

    pMainDlg->m_TXT_GUIDE.m_strMsg = "BOX 바코드 등록 대기 중";

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

    if (pBarCode1->GetCount() < 1)
    {
        pIoBoard->PutOutBit(_BIT_OUT_BARCODE1, true);
        tOn = true;

        wTimeMsec = GetTickCount();
        while (tMainThreadStop == false)
        {
            if (tOn == true)
            {
                if (GetTickCount() > (wTimeMsec + 1000*10))
                {
                    pIoBoard->PutOutBit(_BIT_OUT_BARCODE1, false);
                    tOn = false;
                    wTimeMsec = GetTickCount();
                }
            }
            else
            {
                if (GetTickCount() > (wTimeMsec + 100))
                {
                    pIoBoard->PutOutBit(_BIT_OUT_BARCODE1, true);
                    tOn = true;
                    wTimeMsec = GetTickCount();
                }
            }

            if (pBarCode1->GetCount() >= 1)
            {
				if (pBarCode1->GetDataAt(0) == a_pszBarcodeBOX)
				{
					iError = _ERR_FAIL;
				}
				else
				{
					iError = _ERR_OK;
				}
                break;
            }

            iError = CheckLoopCondition((U32)-1, false);
            if (iError != _ERR_OK)
            {
                break;
            }
        }

        pIoBoard->PutOutBit(_BIT_OUT_BARCODE1, false);
        Sleep(500);
    }
    else
    {
        iError = _ERR_OK;
    }

    if (iError == _ERR_OK)
    {
        pMainDlg->m_TXT_GUIDE.m_strMsg = "BOX 바코드 등록 완료";
	}
    else
    {
        pMainDlg->m_TXT_GUIDE.m_strMsg = GetTestErrString(iError);
    }

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

    return (iError == 0)? true: false;
}

static bool Step_ReadBarcode1_PCB(void)
{
    TEST_ERR iError;
    TOF tOn = false;
    U32 wTimeMsec;
	S16 barcodecnt;

    wMainThreadTestStep |= _STEP_BARCODE1_PCB;

    pMainDlg->m_TXT_GUIDE.m_strMsg = "PCB 바코드 등록 대기 중";

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

    //psj 바코드 작업 수정 진행 필요.
    //if (pBarCode1->GetCount() < 2)
    if (pBarCode1->GetCount() < 1)
    {
        pIoBoard->PutOutBit(_BIT_OUT_BARCODE1, true);
        tOn = true;
	}

    wTimeMsec = GetTickCount();
    while (tMainThreadStop == false)
	{
		if (tOn == true)
		{
			if (GetTickCount() > (wTimeMsec + 1000*10))
			{
				pIoBoard->PutOutBit(_BIT_OUT_BARCODE1, false);
				tOn = false;
				wTimeMsec = GetTickCount();
			}
		}
		else
		{
			if (GetTickCount() > (wTimeMsec + 100))
			{
				pIoBoard->PutOutBit(_BIT_OUT_BARCODE1, true);
				tOn = true;
				wTimeMsec = GetTickCount();
			}
		}
		//psj 바코드 작업 수정 진행 필요.
		//if (pBarCode1->GetCount() >= 2)
		barcodecnt = pBarCode1->GetCount();
		if (barcodecnt >= 1)
		{
			pMainDlg->m_EDIT_BARCODE_PCB = (LPCSTR)pBarCode1->GetDataAt(0);
			iError = _ERR_OK;
			break;
		}

		iError = CheckLoopCondition((U32)-1, false);
		if (iError != _ERR_OK)
		{
			break;
		}
	}
	pIoBoard->PutOutBit(_BIT_OUT_BARCODE1, false);
	Sleep(500);

/*}
    else
    {
        iError = _ERR_OK;
    }
*/
    if (iError == _ERR_OK)
    {
        pMainDlg->m_TXT_GUIDE.m_strMsg = "PCB 바코드 등록 완료";
    }
    else
    {
        pMainDlg->m_TXT_GUIDE.m_strMsg = GetTestErrString(iError);
    }

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

    return (iError == 0)? true: false;
}
static bool Step_ReadBarcode1_Case(void)
{
    TEST_ERR iError;
    TOF tOn = false;
    U32 wTimeMsec;

    wMainThreadTestStep |= _STEP_BARCODE1_CASE;

    pMainDlg->m_TXT_GUIDE.m_strMsg = "케이스 바코드 등록 대기 중";

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

    //psj 바코드 작업 수정 진행 필요.
    //if (pBarCode1->GetCount() < 3)
    if (pBarCode1->GetCount() < 2)
    {
        pIoBoard->PutOutBit(_BIT_OUT_BARCODE1, true);
        tOn = true;
    }

    wTimeMsec = GetTickCount();
    while (tMainThreadStop == false)
    {
        if (tOn == true)
        {
            if (GetTickCount() > (wTimeMsec + 1000*10))
            {
                pIoBoard->PutOutBit(_BIT_OUT_BARCODE1, false);
                tOn = false;
                wTimeMsec = GetTickCount();
            }
        }
        else
        {
            if (GetTickCount() > (wTimeMsec + 100))
            {
                pIoBoard->PutOutBit(_BIT_OUT_BARCODE1, true);
                tOn = true;
                wTimeMsec = GetTickCount();
            }
        }

        //psj 바코드 작업 수정 진행 필요.
        //if (pBarCode1->GetCount() >= 3)
        if (pBarCode1->GetCount() >= 2)
        {
        	pMainDlg->m_EDIT_BARCODE_CASE= (LPCSTR)pBarCode1->GetDataAt(1);
            iError = _ERR_OK;
            break;
        }

        iError = CheckLoopCondition((U32)-1, false);
        if (iError != _ERR_OK)
        {
            break;
        }
    }

    pIoBoard->PutOutBit(_BIT_OUT_BARCODE1, false);

    if (iError == _ERR_OK)
    {
        pMainDlg->m_TXT_GUIDE.m_strMsg = "케이스 바코드 등록 완료";
    }
    else
    {
        pMainDlg->m_TXT_GUIDE.m_strMsg = GetTestErrString(iError);
    }

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

    return (iError == 0)? true: false;
}
static bool Step_ReadBarcode1_Case2(void)
{
    TEST_ERR iError;
    TOF tOn = false;
    U32 wTimeMsec;

    wMainThreadTestStep |= _STEP_BARCODE1_CASE;

    pMainDlg->m_TXT_GUIDE.m_strMsg = "케이스 바코드 등록 대기 중";

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

    //psj 바코드 작업 수정 진행 필요.
    //if (pBarCode1->GetCount() < 3)
    if (pBarCode1->GetCount() < 1)
    {
        pIoBoard->PutOutBit(_BIT_OUT_BARCODE1, true);
        tOn = true;
    }

    wTimeMsec = GetTickCount();
    while (tMainThreadStop == false)
    {
        if (tOn == true)
        {
            if (GetTickCount() > (wTimeMsec + 1000*10))
            {
                pIoBoard->PutOutBit(_BIT_OUT_BARCODE1, false);
                tOn = false;
                wTimeMsec = GetTickCount();
            }
        }
        else
        {
            if (GetTickCount() > (wTimeMsec + 100))
            {
                pIoBoard->PutOutBit(_BIT_OUT_BARCODE1, true);
                tOn = true;
                wTimeMsec = GetTickCount();
            }
        }

        //psj 바코드 작업 수정 진행 필요.
        //if (pBarCode1->GetCount() >= 3)
        if (pBarCode1->GetCount() >= 1)
        {
        	pMainDlg->m_EDIT_BARCODE_CASE= (LPCSTR)pBarCode1->GetDataAt(0);
            iError = _ERR_OK;
            break;
        }

        iError = CheckLoopCondition((U32)-1, false);
        if (iError != _ERR_OK)
        {
            break;
        }
    }

    pIoBoard->PutOutBit(_BIT_OUT_BARCODE1, false);

    if (iError == _ERR_OK)
    {
        pMainDlg->m_TXT_GUIDE.m_strMsg = "케이스 바코드 등록 완료";
    }
    else
    {
        pMainDlg->m_TXT_GUIDE.m_strMsg = GetTestErrString(iError);
    }

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

    return (iError == 0)? true: false;
}

static bool Step_Screw_Work(void)
{
    TEST_ERR iError;
	CString strRetData;

    wMainThreadTestStep |= _STEP_SCREW;

    pMainDlg->m_TXT_GUIDE.m_strMsg = "스크류 작업 대기 중";

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

#if _SCREW_SERIAL
	pScrew->DelAllCount();
	pScrew->m_szMsg[0] = 0;
	pScrew->m_nScrew_State = _SCREW_READ;
#endif

    while (tMainThreadStop == false)
    {
#if _SCREW_SERIAL
        if (strlen(pScrew->GetMsg()) >= 1)
        {
			strRetData.Format("%s", pScrew->GetMsg());
			if (strRetData.Find("*OK") != -1)
			{
				iError = _ERR_OK;
			}
			else
			{
				iError = _ERR_FAIL;
			}
			break;
		}
#else
		if (pScrew->GetCount() >= 1)
		{
			iError = _ERR_OK;
			break;
		}
#endif

        iError = CheckLoopCondition((U32)-1, false);
        if (iError != _ERR_OK)
        {
            break;
        }
    }

    if (iError == _ERR_OK)
    {
        pMainDlg->m_TXT_GUIDE.m_strMsg = "스크류 작업 완료 ";
    }
    else
    {
        pMainDlg->m_TXT_GUIDE.m_strMsg = GetTestErrString(iError);
    }

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

    return (iError == 0)? true: false;
}

static bool Step_Connect(void)
{
    int nListPos;
    TEST_ERR iError = _ERR_OK;
    char *pszTgtMsg = "-";

    wMainThreadTestStep |= _STEP_CONNECT;
#ifdef Sigle_PC_TEST
    if ((pIoBoard->GetInPort() & _PIN_IN_CONNECT) == 0)
    {
        pIoBoard->PutOutBit(_BIT_OUT_DISCONNECT, false);
        Sleep(1000);
        pIoBoard->PutOutBit(_BIT_OUT_DISCONNECT, true);
        Sleep(500);
    }
#endif
    if (iMainThreadTestMode == _MODE_MASTER_GOOD)
    {
        pMainDlg->m_TXT_GUIDE.m_strMsg.Format("양품 마스터 테스트 %d\n", pMainDlg->m_EDIT_TEST_NUM_ALL+1);
    }
    else if (iMainThreadTestMode == _MODE_MASTER_BAD)
    {
        pMainDlg->m_TXT_GUIDE.m_strMsg.Format("불량 마스터 테스트 %d\n", pMainDlg->m_EDIT_TEST_NUM_ALL+1);
    }
    else if (iMainThreadTestMode == _MODE_MAIN)
    {
        pMainDlg->m_TXT_GUIDE.m_strMsg = "";
    }
    else
    {
        pMainDlg->m_TXT_GUIDE.m_strMsg = "미지정 테스트\n";
    }

    pMainDlg->m_TXT_GUIDE.m_strMsg += "시작 버튼을 동시에 누르세요.";

    nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
    pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "장치 연결");
    pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgtMsg);

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

#ifdef Sigle_PC_TEST
    while (tMainThreadStop == false)
    {
        if ((pIoBoard->GetInPort() & _PIN_IN_CONNECT) == 0)
        {
            iError = _ERR_OK;
            break;
        }

        iError = CheckLoopCondition((U32)-1, false);
        if (iError != _ERR_OK)
        {
            break;
        }
    }
#endif
    if (iError == _ERR_OK)
    {
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "연결됨");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "PASS");

        pMainDlg->m_TXT_GUIDE.m_strMsg = "장치가 연결되었습니다.";

        pMainDlg->m_EDIT_PIN_USED++;
#ifdef Sigle_PC_TEST
        pScrew->DelCount();
#endif
    }
    else
    {
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, GetTestErrString(iError));
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "ERROR");

        pMainDlg->m_TXT_GUIDE.m_strMsg = GetTestErrString(iError);
    }

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

    return (iError == 0)? true: false;
}

static bool Step_Test_Ampare(void)
{
    TEST_ERR iError;
	int nCount;
    int nListPos;
	int nAmpare;
	int nAmpare_Min;
	int nAmpare_Max;
    char szTgtMsg[128];
	char *pszRecMsg = "";
	double digit=1.0;
	CString    strData;
	stRegister m_Register;

    wMainThreadTestStep |= _STEP_TEST_START;

	memset(szTgtMsg, 0x00, sizeof(szTgtMsg));

	if (pMainDlg->m_COMBO_VOLTAGE_TYPE=="24V")
	{
		nAmpare_Min = atoi(pSetupDlg->m_EDIT_IO_IN_AMPERE_UNDER_24);
		nAmpare_Max = atoi(pSetupDlg->m_EDIT_IO_IN_AMPERE_OVER_24);
	}
	else if (pMainDlg->m_COMBO_VOLTAGE_TYPE=="12V")
	{
		nAmpare_Min = atoi(pSetupDlg->m_EDIT_IO_IN_AMPERE_UNDER_12);
		nAmpare_Max = atoi(pSetupDlg->m_EDIT_IO_IN_AMPERE_OVER_12);
	}
	else
	{
		nAmpare_Min = atoi(pSetupDlg->m_EDIT_IO_IN_AMPERE_UNDER_24);
		nAmpare_Max = atoi(pSetupDlg->m_EDIT_IO_IN_AMPERE_OVER_24);
	}



    sprintf_s(szTgtMsg, "범위 : 0.%03d~0.%03d mA", nAmpare_Min, nAmpare_Max);



    pMainDlg->m_TXT_GUIDE.m_strMsg = "전류 확인";

    nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
    pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "전류 확인");
    pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, szTgtMsg);

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

	pAmMeter->m_nAmMeter_State = _AMMETER_WRITE;

    while (tMainThreadStop == false)
    {
        if (strlen(pAmMeter->GetMsg()) >= 1)
        {
			pszRecMsg = pAmMeter->GetMsg();
			memcpy(&m_Register, &pszRecMsg[3], sizeof(stRegister));
			iError = _ERR_OK;
            break;
        }

        iError = CheckLoopCondition((U32)-1, false);
        if (iError != _ERR_OK)
        {
            break;
        }
    }

    if (iError == _ERR_OK)
    {
		nAmpare = 0;
		nAmpare =  m_Register.value_hi << 8;
		nAmpare |=  m_Register.value_lo;
		dprintf(("%d\n",nAmpare));
		for(nCount=0;nCount<m_Register.dot_lo;nCount++) digit /= 10.0;

        strData.Format("%.3f mA", (float)(nAmpare*digit));


        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, strData);

		if ((nAmpare>nAmpare_Min) && (nAmpare<nAmpare_Max))
		{
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "PASS");
			pMainDlg->m_TXT_GUIDE.m_strMsg = "전류값 측정 완료";
		}
		else
		{
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "ERROR");
			pMainDlg->m_TXT_GUIDE.m_strMsg = "전류값 측정 에러";
			iError = _ERR_AMPERE;
		}
    }
    else
    if (iError == _ERR_TIMEOVER)
    {
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "전류값 측정 에러");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "ERROR");

        pMainDlg->m_TXT_GUIDE.m_strMsg = "전류값 측정 에러";
    }
    else
    {
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, GetTestErrString(iError));
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "ERROR");

        pMainDlg->m_TXT_GUIDE.m_strMsg = GetTestErrString(iError);
    }

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

    return (iError == 0)? true: false;
}

static bool Step_Test_Start(void)
{
    TEST_ERR iError = _ERR_OK;
    int nListPos;
    char *pszTgtMsg = "-";

    wMainThreadTestStep |= _STEP_TEST_START;

    pMainDlg->m_TXT_GUIDE.m_strMsg = "전원 및 CAN 연결";

    nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
    pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "전원 및 CAN 연결");
    pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgtMsg);

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

    // 전원 공급
#ifdef Sigle_PC_TEST
    pIoBoard->PutOutBit(_BIT_OUT_POWER, true);
    Sleep(500);

	if (pMainDlg->m_COMBO_VOLTAGE_TYPE=="24V")
	{
		pIoBoard->PutOutBit(_BIT_OUT_SET_VOLT, true);
	}
    else if (pMainDlg->m_COMBO_VOLTAGE_TYPE=="12V")
	{
		pIoBoard->PutOutBit(_BIT_OUT_SET_VOLT, false);
	}
	else
	{
		pIoBoard->PutOutBit(_BIT_OUT_SET_VOLT, true);
	}
#endif

#ifdef Sigle_PC_TEST
        //psj can line test
        //if ((pIoBoard->GetInPort() & _PIN_IN_CANLINE) != 0)
        if (((pIoBoard->GetInPort() & _PIN_IN_CANLINE) == 0))
        {
            iError = _ERR_CANLINE;
        }
		else
		{
			iError = _ERR_OK;
		}
#endif


    if (iError == _ERR_OK)
    {
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "연결 완료");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "PASS");

        pMainDlg->m_TXT_GUIDE.m_strMsg = "전원 및 CAN 연결 완료";
    }
	//kbr can short test
#if 0
    else if(iError == _ERR_CANSHORT)
	{
		pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "LDWS CAN SHORT");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "ERROR");

        pMainDlg->m_TXT_GUIDE.m_strMsg = "CAN SHORT 에러";
	}
#endif
    else if (iError == _ERR_TIMEOVER)
    {
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "LDWS CAN 메시지 없음");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "ERROR");

        pMainDlg->m_TXT_GUIDE.m_strMsg = "CAN 연결 에러";
    }

    else
    {
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, GetTestErrString(iError));
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "ERROR");

        pMainDlg->m_TXT_GUIDE.m_strMsg = GetTestErrString(iError);
    }

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

    return (iError == 0)? true: false;
}

static bool Step_Test_End(void)
{
    int nListPos;
    TEST_ERR iError = _ERR_OK;

    char *pszTgtMsg = "-";

    wMainThreadTestStep |= _STEP_TEST_END;

    pMainDlg->m_TXT_GUIDE.m_strMsg = "전원 및 CAN 해제";

    nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
    pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "전원 및 CAN 해제");
    pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgtMsg);

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

    // CAN Thread Stop
    //pCan->Thread_End();
#ifdef Sigle_PC_TEST
	U32 wTimeLimit;
    // 전원 해제
    pIoBoard->PutOutBit(_BIT_OUT_POWER, false);
    Sleep(500);

    // CAN Tread End
    //pCan->Thread_End();

    // 2초간 장치가 해제되지 않으면 에러
    if ((pIoBoard->GetInPort() & _PIN_IN_CONNECT) == 0)
    {
        pIoBoard->PutOutBit(_BIT_OUT_DISCONNECT, false);

        wTimeLimit = GetTickCount() + 2000;
        while (true)
        {
            if ((pIoBoard->GetInPort() & _PIN_IN_CONNECT) != 0)
            {
                iError = _ERR_OK;
                break;
            }

            iError = CheckLoopCondition(wTimeLimit, false);
            if (iError == _ERR_TIMEOVER)
            {
                break;
            }
        }
    }

    pIoBoard->PutOutBit(_BIT_OUT_DISCONNECT, true);
#endif
    if (iError == _ERR_OK)
    {
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "해제 완료");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "PASS");

        pMainDlg->m_TXT_GUIDE.m_strMsg = "전원 및 CAN 해제 완료";
    }
    else
    if (iError == _ERR_TIMEOVER)
    {
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "장치 해제 에러");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "ERROR");

        pMainDlg->m_TXT_GUIDE.m_strMsg = "자치해제 에러";
    }
    else
    {
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, GetTestErrString(iError));
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "ERROR");

        pMainDlg->m_TXT_GUIDE.m_strMsg = GetTestErrString(iError);
    }

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

    return (iError == 0)? true: false;
}

static bool Step_ReadBarcode2_Case(char *a_pszBarcodeCase, char *a_pszBarcodePCB)
{
    int  nListPos;
    TEST_ERR iError;
    S16  iSearchedBarcode;
    char *pszTgtMsg = "-";
    TOF  tOn;
    U32  wTimeMsec;

    wMainThreadTestStep |= _STEP_BARCODE2_CASE;

    pMainDlg->m_TXT_GUIDE.m_strMsg = "케이스 바코드 확인 대기 중";

    nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
    pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "케이스 바코드 확인");
    pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgtMsg);

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

    pBarCode2->DelAllData();

    a_pszBarcodeCase[0] = 0;
    a_pszBarcodePCB [0] = 0;

    pIoBoard->PutOutBit(_BIT_OUT_BARCODE2, true);
    tOn = true;
    wTimeMsec = GetTickCount();

    while (tMainThreadStop == false)
    {
        if (tOn == true)
        {
            if (GetTickCount() > (wTimeMsec + 1000*10))
            {
                pIoBoard->PutOutBit(_BIT_OUT_BARCODE2, false);
                tOn = false;
                wTimeMsec = GetTickCount();
            }
        }
        else
        {
            if (GetTickCount() > (wTimeMsec + 100))
            {
                pIoBoard->PutOutBit(_BIT_OUT_BARCODE2, true);
                tOn = true;
                wTimeMsec = GetTickCount();
            }
        }

        if (pBarCode2->GetCount() >= 1)
        {
            iError = _ERR_OK;
            break;
        }

        iError = CheckLoopCondition((U32)-1, false);
        if ( (iError != _ERR_OK) && (iError != _ERR_MANUAL) )
        {
            break;
        }
    }

    pIoBoard->PutOutBit(_BIT_OUT_BARCODE2, false);

    if (iError == _ERR_OK)
    {
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pBarCode2->GetDataAt(0));

        iSearchedBarcode = pBarCode1->SearchData(pBarCode2->GetDataAt(0));
        pMainDlg->m_EDIT_BARCODE_CASE2= (LPCSTR)pBarCode2->GetDataAt(0);
        if (iSearchedBarcode == -1)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "ERROR");
            pMainDlg->m_TXT_GUIDE.m_strMsg = "미등록된 바코드입니다.\n";

            pBarCode1->DelAllData();
            iError = _ERR_FAIL;
        }
        else
        if ((iSearchedBarcode % 2) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "ERROR");
            pMainDlg->m_TXT_GUIDE.m_strMsg = "케이스 바코드가 아닙니다:\n";
            pBarCode1->DelAllData();
            iError = _ERR_FAIL;
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "PASS");
            pMainDlg->m_TXT_GUIDE.m_strMsg = "바코드 확인 완료";

            strcpy(a_pszBarcodePCB , pBarCode1->GetDataAt(iSearchedBarcode-1));
			strcpy(a_pszBarcodeCase, pBarCode1->GetDataAt(iSearchedBarcode-0));

            pBarCode1->DelDataAt(iSearchedBarcode-1);
            pBarCode1->DelDataAt(iSearchedBarcode-1);
            pBarCode1->DelDataAt(iSearchedBarcode-1);
            iError = _ERR_OK;
        }
    }

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

    return (iError == 0)? true: false;
}

static bool Step_ReadBarcode2_Case2(char *a_pszBarcodeCase)
{
    int  nListPos;
    TEST_ERR iError;
    S16  iSearchedBarcode;
    char *pszTgtMsg = "-";
    TOF  tOn;
    U32  wTimeMsec;

    wMainThreadTestStep |= _STEP_BARCODE2_CASE;

    if (pSetupDlg->m_COMBO_CAR_TYPE == "HG")
        pMainDlg->m_TXT_GUIDE.m_strMsg = "케이스 바코드 확인 대기 중";
    else
        pMainDlg->m_TXT_GUIDE.m_strMsg = "케이스 바코드 확인 대기 중\n카메라 캡이 씌워졌는지 확인하세요.";

    nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
    pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "케이스 바코드 확인");
    pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgtMsg);

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

    pBarCode2->DelAllData();

    a_pszBarcodeCase[0] = 0;

    pIoBoard->PutOutBit(_BIT_OUT_BARCODE2, true);
    tOn = true;
    wTimeMsec = GetTickCount();

    while (tMainThreadStop == false)
    {
        if (tOn == true)
        {
            if (GetTickCount() > (wTimeMsec + 1000*10))
            {
                pIoBoard->PutOutBit(_BIT_OUT_BARCODE2, false);
                tOn = false;
                wTimeMsec = GetTickCount();
            }
        }
        else
        {
            if (GetTickCount() > (wTimeMsec + 100))
            {
                pIoBoard->PutOutBit(_BIT_OUT_BARCODE2, true);
                tOn = true;
                wTimeMsec = GetTickCount();
            }
        }

        if (pBarCode2->GetCount() >= 1)
        {
            iError = _ERR_OK;
            break;
        }

        iError = CheckLoopCondition((U32)-1, false);
        if ( (iError != _ERR_OK) && (iError != _ERR_MANUAL) )
        {
            break;
        }
    }

    pIoBoard->PutOutBit(_BIT_OUT_BARCODE2, false);

    if (iError == _ERR_OK)
    {
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pBarCode2->GetDataAt(0));

        iSearchedBarcode = pBarCode1->SearchData(pBarCode2->GetDataAt(0));
		pMainDlg->m_EDIT_BARCODE_CASE2= (LPCSTR)pBarCode2->GetDataAt(0);

        if (iSearchedBarcode == -1)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "ERROR");
            pMainDlg->m_TXT_GUIDE.m_strMsg = "미등록된 바코드입니다.\n";

            pBarCode1->DelAllData();
            iError = _ERR_FAIL;
        }
        else
        if (iSearchedBarcode != 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "ERROR");
            pMainDlg->m_TXT_GUIDE.m_strMsg = "케이스 바코드가 아닙니다:\n";
            pBarCode1->DelAllData();
            iError = _ERR_FAIL;
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "PASS");
            pMainDlg->m_TXT_GUIDE.m_strMsg = "바코드 확인 완료";

		   strcpy(a_pszBarcodeCase, pBarCode1->GetDataAt(iSearchedBarcode-0));

           pBarCode1->DelDataAt(iSearchedBarcode);
           pBarCode1->DelDataAt(iSearchedBarcode);
           pBarCode1->DelDataAt(iSearchedBarcode);
            iError = _ERR_OK;
        }
    }

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

    return (iError == 0)? true: false;
}

static bool Step_SaveLogFile(const char *a_pszBarcodeCase, const char *a_pszBarcodePCB)
{
    FILE   *pFileLog, *pFileSCType;
    int     nListPos, nListMax;
    CTime   oTime;
    char    szMsg[256];
	//psj 바코드 작업 수정 진행 필요.
	CString strFilePath;

    wMainThreadTestStep |= _STEP_SAVE_LOG;



	strFilePath = pSetupDlg->m_EDIT_FILENAME_LOG;
	strFilePath.Replace("AAVM_LOG", "AAVM_제출용");

	pFileLog    = fopen(pSetupDlg->m_EDIT_FILENAME_LOG, "a+t");
		//psj 바코드 작업 수정 진행 필요.
	pFileSCType = fopen(strFilePath, "a+t");

    if (pFileLog != NULL)
    {
        dprintf(("Log File <%s> Open Success: Test.\n", (LPCTSTR)pSetupDlg->m_EDIT_FILENAME_LOG));
    }
    else
    {
        dprintf(("Log File <%s> Open Fail: Test!\n", (LPCTSTR)pSetupDlg->m_EDIT_FILENAME_LOG));
        return false;
    }

    oTime = CTime::GetCurrentTime();

    fseek(pFileLog, SEEK_END, 0);

    if (iMainThreadTestMode == _MODE_MASTER_GOOD)
    {
        fprintf(pFileLog, "\n양품 마스터 테스트 모드, %d\n", pMainDlg->m_EDIT_TEST_NUM_ALL+1);
    }
    else if (iMainThreadTestMode == _MODE_MASTER_BAD)
    {
        fprintf(pFileLog, "\n불량 마스터 테스트 모드, %d\n", pMainDlg->m_EDIT_TEST_NUM_ALL+1);
    }
    else if(iMainThreadTestMode == _MODE_MAIN_2TH)
    {
		fprintf(pFileLog, "\n케이스 바코드, %s\n", a_pszBarcodeCase);
		//psj 바코드 작업 수정 진행 필요.
		fprintf(pFileSCType, "%s\t%s\n", pSetupDlg->m_COMBO_CAR_TYPE, a_pszBarcodeCase);
    }
    else
    {
        fprintf(pFileLog, "\n케이스 바코드, %s\n", a_pszBarcodeCase);
        fprintf(pFileLog, "PCB 바코드, %s\n", a_pszBarcodePCB);

		//psj 바코드 작업 수정 진행 필요.
		fprintf(pFileSCType, "%s\t%s\n", pSetupDlg->m_COMBO_CAR_TYPE, a_pszBarcodeCase);
    }

    fprintf(pFileLog, "테스트 판정, %s\n", (tMainThreadTestError == false)? "양품": "불량");
    fprintf(pFileLog, "테스트 라인: %s\n", pMainDlg->m_EDIT_TESTER_LINE);
    fprintf(pFileLog, "검사자, %s\n", pMainDlg->m_EDIT_TESTER_NAME);
    fprintf(pFileLog, "테스트 시간, %04d-%02d-%02d, %02d:%02d:%02d\n",
        oTime.GetYear(), oTime.GetMonth(), oTime.GetDay(),
        oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());

    nListMax = pMainDlg->m_LIST_PROCESS.GetItemCount();
    for (nListPos = 0; nListPos < nListMax; nListPos++)
    {
        pMainDlg->m_LIST_PROCESS.GetItemText(nListPos, 0, szMsg, sizeof(szMsg));
        fprintf(pFileLog, "%s, ", szMsg);

        pMainDlg->m_LIST_PROCESS.GetItemText(nListPos, 1, szMsg, sizeof(szMsg));
        fprintf(pFileLog, "%s, ", szMsg);

        pMainDlg->m_LIST_PROCESS.GetItemText(nListPos, 2, szMsg, sizeof(szMsg));
        fprintf(pFileLog, "%s, ", szMsg);

        pMainDlg->m_LIST_PROCESS.GetItemText(nListPos, 3, szMsg, sizeof(szMsg));
        fprintf(pFileLog, "%s\n", szMsg);
    }

    fclose(pFileLog);
	//psj 바코드 작업 수정 진행 필요.
	fclose(pFileSCType);

    return true;
}

static bool Step_SaveFile(const char *a_pszBarcodeCase, const char *a_pszBarcodePCB, const char *a_pszBarcodeBOX)
{
}

//kbr cpk 추가
static bool Step_SaveCPKFile(const char *a_pszBarcodeCase, const char *a_pszBarcodePCB, const char *a_pszBarcodeBOX)
{
	FILE   *pFileLog, *pFileSCType;
    int     nListPos, nListMax;
    CTime   oTime;
    char    szMsg[256];
	//psj 바코드 작업 수정 진행 필요.
	CString strFilePath;

    wMainThreadTestStep |= _STEP_SAVE_LOG;

	//psj 바코드 작업 수정 진행 필요.
	strFilePath = pSetupDlg->m_EDIT_FILENAME_LOG;
	strFilePath.Replace("LDWS_LOG", "LDWS_제출용");

    pFileLog    = fopen(pSetupDlg->m_EDIT_FILENAME_LOG, "a+t");
	//psj 바코드 작업 수정 진행 필요.
	pFileSCType = fopen(strFilePath, "a+t");

    if (pFileLog != NULL)
    {
        dprintf(("CPK File <%s> Open Success: Test.\n", (LPCTSTR)pSetupDlg->m_EDIT_FILENAME_LOG));
    }
    else
    {
        dprintf(("CPK File <%s> Open Fail: Test!\n", (LPCTSTR)pSetupDlg->m_EDIT_FILENAME_LOG));
        return false;
    }

    oTime = CTime::GetCurrentTime();

    fseek(pFileLog, SEEK_END, 0);

    if (iMainThreadTestMode == _MODE_MASTER_GOOD)
    {
        fprintf(pFileLog, "\n양품 마스터 테스트 모드, %d\n", pMainDlg->m_EDIT_TEST_NUM_ALL+1);
    }
    else if (iMainThreadTestMode == _MODE_MASTER_BAD)
    {
        fprintf(pFileLog, "\n불량 마스터 테스트 모드, %d\n", pMainDlg->m_EDIT_TEST_NUM_ALL+1);
    }
    else
    {
        fprintf(pFileLog, "\n케이스 바코드, %s\n", a_pszBarcodeCase);
        fprintf(pFileLog, "PCB 바코드, %s\n", a_pszBarcodePCB);
        //psj 바코드 작업 수정 진행 필요.
		fprintf(pFileLog, "BOX바코드, %s\n", a_pszBarcodeBOX);

		//psj 바코드 작업 수정 진행 필요.
		fprintf(pFileSCType, "%s\t%s\t%s\n", pSetupDlg->m_COMBO_CAR_TYPE, a_pszBarcodeBOX, a_pszBarcodeCase);
    }

    fprintf(pFileLog, "테스트 판정, %s\n", (tMainThreadTestError == false)? "양품": "불량");
    fprintf(pFileLog, "테스트 라인: %s\n", pMainDlg->m_EDIT_TESTER_LINE);
    fprintf(pFileLog, "검사자, %s\n", pMainDlg->m_EDIT_TESTER_NAME);
    fprintf(pFileLog, "테스트 시간, %04d-%02d-%02d, %02d:%02d:%02d\n",
        oTime.GetYear(), oTime.GetMonth(), oTime.GetDay(),
        oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());

	nListMax = pMainDlg->m_LIST_PROCESS.GetItemCount();
    for (nListPos = 0; nListPos < nListMax; nListPos++)
    {
		//KBR 13.07.03 CPK PASS or ERROR
        pMainDlg->m_LIST_PROCESS.GetItemText(nListPos, 3, szMsg, sizeof(szMsg));
        fprintf(pFileLog, "%s\n", szMsg);
    }


    fclose(pFileLog);
	//psj 바코드 작업 수정 진행 필요.
	fclose(pFileSCType);

    return true;
}
static bool Step_Test_AAVM_ALL(void)
{
	int nListPos;
	CAN_MSG oCanMsg;
	U32 wCAN_Prev_GetCnt;
    CString stMsg;
    const CHR *pszSrc, *pszTgt;
	unsigned int CAN_TimeOutcnt;
	bool tError = false;
	
	do
	{

		//2013. 06. 04 KBR 전류 확인 추가
		if (tMainThreadStop != false)
		{
			tError = true;
			break;
		}
		//ReOpencan();

#ifdef Sigle_PC_TEST
		Sleep(5500);
		if (Step_Test_Ampare() == false)
		{
			tError = true;
			break;
		}
#endif
		MakeMsg_AAVCS(&oCanMsg, 0, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
		CAN_TimeOutcnt = 0;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

		pMainDlg->m_LIST_PROCESS.DeleteAllItems();
		pMainDlg->PostMessage(_WM_THREAD_UPDATE);
		/////////////////////////////////////
        //CID TEST///////////////////////////
        /////////////////////////////////////
		nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "AAVM CID 제품 코드");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "85");

        MakeMsg_REQ_CID(&oCanMsg);
        wCAN_Prev_GetCnt = wCAN_CID_GetCnt;
        pCan->Write(&oCanMsg);
		CAN_TimeOutcnt = 0;
        while ( wCAN_Prev_GetCnt == wCAN_CID_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }
        stMsg.Format("%d", bCAN_CID_CC);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (bCAN_CID_CC == 85)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }
		/////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "AAVM CID 공급자 코드");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "22");

        stMsg.Format("%d", bCAN_CID_MC);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (bCAN_CID_MC == 22)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "AAVM CID 제조일자");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "2016-08-22");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, szCAN_CID_MDay);
        if (strcmp(szCAN_CID_MDay, "2016-08-22") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "AAVM CID 버전");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "2.8.0");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, szCAN_CID_Ver);
        if (strcmp(szCAN_CID_Ver, "2.8.0") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "AAVM CID 모델이름");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "AAVMHW60");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, szCAN_CID_ModelName);
        if (strcmp(szCAN_CID_ModelName, "AAVMHW60") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        //Version Test 1/////////////////////
        /////////////////////////////////////
		tCAN_VER1_Complete = false;
        MakeMsg_REQ_VER1(&oCanMsg);
        pCan->Write(&oCanMsg);
		CAN_TimeOutcnt = 0;
        while ( tCAN_VER1_Complete == false)
        {
            Sleep(100);
            pCan->Write(&oCanMsg);
			if (CAN_TimeOutcnt++ > 100)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }
        
        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "AAVM H/W 버전");
        pszSrc = szCAN_Ver_HW;
        pszTgt = "1.5";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "BF609 F/W 버전");
        pszSrc = szCAN_Ver_BF;
        pszTgt = "2.8";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "에머랄드 F/W 버전");
        pszSrc = szCAN_Ver_EL_Main;
        pszTgt = "1.12";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "에머랄드 합성 버전");
        pszSrc = szCAN_Ver_EL_Sub;
        pszTgt = "없음";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }
		/////////////////////////////////////
        //Version Test 2/////////////////////
        /////////////////////////////////////
        tCAN_VER2_Complete = false;
        MakeMsg_REQ_VER2(&oCanMsg);
        pCan->Write(&oCanMsg);
		CAN_TimeOutcnt = 0;
        while ( tCAN_VER2_Complete == false)
        {
            Sleep(100);
            pCan->Write(&oCanMsg);
			if (CAN_TimeOutcnt++ > 100)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "카메라 버전 - 전방");
        pszSrc = szCAN_Ver_Camera_Front;
        pszTgt = "3.2";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "카메라 버전 - 후방");
        pszSrc = szCAN_Ver_Camera_Rear;
        pszTgt = "3.2";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "카메라 버전 - 좌측");
        pszSrc = szCAN_Ver_Camera_Left;
        pszTgt = "3.2";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "카메라 버전 - 우측");
        pszSrc = szCAN_Ver_Camera_Right;
        pszTgt = "3.2";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }
		/////////////////////////////////////
        //MLST Test//////////////////////////
        /////////////////////////////////////
		int Car_Types[5] = {0, 0, 0, 0, 0};
		if (pMainDlg->m_EDIT_CAR_NAME == "21k8-46000 통합형")
		{
			Car_Types[0] = 2;
			Car_Types[1] = 5;
			Car_Types[2] = 7;
			Car_Types[3] = 10;
		}
		else if (pMainDlg->m_EDIT_CAR_NAME == "21k8-46100 통합형")
		{
			Car_Types[0] = 4;
			Car_Types[1] = 3;
			Car_Types[2] = 6;
			Car_Types[3] = 8;
			Car_Types[4] = 9;
		}
		else if (pMainDlg->m_EDIT_CAR_NAME == "21k8-43000 통합형")
		{
			Car_Types[0] = 13;
			Car_Types[1] = 14;
			Car_Types[2] = 15;
			Car_Types[3] = 16;
		}
		else if (pMainDlg->m_EDIT_CAR_NAME == "21k8-46200 통합형")
		{
			Car_Types[0] = 17;
			Car_Types[1] = 18;
			Car_Types[2] = 19;
		}
		else
		{
		}


        MakeMsg_REQ_MLST(&oCanMsg);
        wCAN_Prev_GetCnt = wCAN_MLST_GetCnt;
        pCan->Write(&oCanMsg);
		CAN_TimeOutcnt = 0;
        while ( wCAN_MLST_GetCnt < (wCAN_Prev_GetCnt+4) )
        {
            Sleep(1000);
            pCan->Write(&oCanMsg);
			if (CAN_TimeOutcnt++ > 10 + 4)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        /////////////////////////////////////
		for (int ii = 0; ii < 5; ii++)
		{
			nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
			pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "내장된 합성 모델");
			if (Car_Types[ii] == 0)
			{
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "예정된 합성 모델 없음");
				continue;
			}
			else
			{
				pszTgt = arpszModelName[Car_Types[ii]];
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
				for (int jj = 0; jj<5; jj++)
				{
					pszSrc = arszCAN_MLST[jj];
					if (strcmp(pszSrc, pszTgt) == 0)
					{
						pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
						pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
						break;
					}
				}
				if (strcmp(pszSrc, pszTgt) != 0)
				{
					pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "저장된 합성 모델 없음");
					pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
					tError = true;
					break;
				}
			}
		}
		if(tError == true)
		{
			break;
		}
		/*
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "내장된 합성 모델 1");
        pszSrc = arszCAN_MLST[0];
        pszTgt = arpszModelName[13];
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "내장된 합성 모델 2");
        pszSrc = arszCAN_MLST[1];
        pszTgt = arpszModelName[14];
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "내장된 합성 모델 3");
        pszSrc = arszCAN_MLST[2];
        pszTgt = arpszModelName[15];
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "내장된 합성 모델 4");
        pszSrc = arszCAN_MLST[3];
        pszTgt = arpszModelName[16];
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "내장된 합성 모델 5");
        pszSrc = arszCAN_MLST[4];
        pszTgt = arpszModelName[19];
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }


        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "내장된 합성 모델 6");
        pszSrc = arszCAN_MLST[5];
        pszTgt = "없음";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }
		*/

		/////////////////////////////////////
        //AAVST Test/////////////////////////
        /////////////////////////////////////
        wCAN_Prev_GetCnt = wCAN_AAVST_GetCnt;
		CAN_TimeOutcnt = 0;
        while ( wCAN_AAVST_GetCnt <= (wCAN_Prev_GetCnt+4) )
        {
            Sleep(100);
			if (CAN_TimeOutcnt++ > 100 + 40)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "BF609 영상 입력 상태");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "정상");

        if ((bCAN_AAVST_BF609_Stat & _BF_ERR_CAMERA) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "정상");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "에러");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "BF609 vs 에메랄드 통신 상태");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "정상");

        if ((bCAN_AAVST_BF609_Stat & _BF_ERR_EMERALD_ST) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "정상");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "에러");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "BF609 CAN ESROS 통신 상태");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "정상");

        if ((bCAN_AAVST_BF609_Stat & _BF_ERR_CAN_ESROS) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "정상");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "에러");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "BF609 CAN HP2 통신 상태");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "정상");

        if ((bCAN_AAVST_BF609_Stat & _BF_ERR_CAN_HP2) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "정상");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "에러");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "BF609 CAN HP3 통신 상태");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "정상");

        if ((bCAN_AAVST_BF609_Stat & _BF_ERR_CAN_HP3) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "정상");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "에러");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "BF609 처리 속도");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "5 이상");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_BF609_FPS);
        if (bCAN_AAVST_BF609_FPS >= 50)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }


        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "에메랄드 상태");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "미보정");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_EMERALD_STAT);
        if (strcmp(pMainDlg->m_EDIT_EMERALD_STAT, "미보정") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        
        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "카메라 상태 - 전방");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "정상");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_CAMERA_STAT_FRONT);
        if (strcmp(pMainDlg->m_EDIT_CAMERA_STAT_FRONT, "정상") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "카메라 상태 - 후방");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "정상");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_CAMERA_STAT_REAR);
        if (strcmp(pMainDlg->m_EDIT_CAMERA_STAT_REAR, "정상") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "카메라 상태 - 좌측");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "정상");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_CAMERA_STAT_LEFT);
        if (strcmp(pMainDlg->m_EDIT_CAMERA_STAT_LEFT, "정상") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "카메라 상태 - 우측");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "정상");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_CAMERA_STAT_RIGHT);
        if (strcmp(pMainDlg->m_EDIT_CAMERA_STAT_RIGHT, "정상") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }
		/////////////////////////////////////
        /////////////////////////////////////
        /////////////////////////////////////

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "카메라 밝기값 측정 - 전방");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "전방>45");

        stMsg.Format("전방(%d)",bCAN_AAVST_CameraGain_Front);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);

        if ( (bCAN_AAVST_CameraGain_Front > 45) )
		{
  			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
   			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }


        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "카메라 밝기값 측정 - 후방");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "후방>45");

        stMsg.Format("후방(%d)",bCAN_AAVST_CameraGain_Rear);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);

        if ((bCAN_AAVST_CameraGain_Rear  > 45))
		{
  			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
   			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "카메라 밝기값 측정 - 좌측");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "좌측>45");

        stMsg.Format("좌측(%d)", bCAN_AAVST_CameraGain_Left);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if ((bCAN_AAVST_CameraGain_Left  > 45))
		{
  			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
   			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "카메라 밝기값 측정 - 우측");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "우측>45");

        stMsg.Format("우측(%d)",bCAN_AAVST_CameraGain_Right);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if ((bCAN_AAVST_CameraGain_Right > 45))
		{
  			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
   			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true && tError;
        }

		/////////////////////////////////////
        //AAVO Test//////////////////////////
        /////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 0, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
		CAN_TimeOutcnt = 0;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "출력 화면 뷰 모드 - 옴니(전)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "옴니(전)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "옴니(전)") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 1, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
		CAN_TimeOutcnt = 0;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "출력 화면 뷰 모드 - 옴니(후)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "옴니(후)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "옴니(후)") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 8, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
		CAN_TimeOutcnt = 0;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "출력 화면 뷰 모드 - 옴니(좌)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "옴니(좌)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "옴니(좌)") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 9, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
		CAN_TimeOutcnt = 0;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "출력 화면 뷰 모드 - 옴니(우)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "옴니(우)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "옴니(우)") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 2, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
		CAN_TimeOutcnt = 0;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "출력 화면 뷰 모드 - 버드뷰");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "버드뷰");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "버드뷰") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 3, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
		CAN_TimeOutcnt = 0;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "출력 화면 뷰 모드 - 4채널");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "4채널");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "4채널") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 4, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
		CAN_TimeOutcnt = 0;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "출력 화면 뷰 모드 - 전방");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "전방");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "전방") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 5, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
		CAN_TimeOutcnt = 0;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "출력 화면 뷰 모드 - 후방");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "후방");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "후방") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 6, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
		CAN_TimeOutcnt = 0;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "출력 화면 뷰 모드 - 좌측");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "좌측");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "좌측") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 7, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
		CAN_TimeOutcnt = 0;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "출력 화면 뷰 모드 - 우측");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "우측");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "우측") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////// Return Default View Mode //////
        MakeMsg_AAVCS(&oCanMsg, 0, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
		CAN_TimeOutcnt = 0;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "출력 화면 뷰 모드 - 옴니(전)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "옴니(전)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "옴니(전)") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }
		/////////////////////////////////////
        //AAVCS Test/////////////////////////
        /////////////////////////////////////
        pMainDlg->m_EDIT_ESROS_WIPER = 0;
        pMainDlg->m_EDIT_ESROS_INTWIPER = 0;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        MakeMsg_AAVCS(&oCanMsg, 0, 2);
        pCan->Write(&oCanMsg);
		CAN_TimeOutcnt = 0;

        // 인식모드
        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "인식모드 - 사용자 스위치 Off");
        pszTgt = "사용자 Off(On)";
        stMsg.Format("사용자 Off(%s)", 
            (pMainDlg->m_CHECK_MODE_MANUAL_OFF == FALSE)? "Off": "On");
        pszSrc = stMsg;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        // IMOD 상태
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
		CAN_TimeOutcnt = 0;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "IMOD 상태 - 사용자 스위치 Off");
        pszTgt = "수동 Off";
        pszSrc = pMainDlg->m_EDIT_AAVM_IMOD_STATUS;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 0, 1);
        pCan->Write(&oCanMsg);
		CAN_TimeOutcnt = 0;

        // 인식모드
        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "인식모드 - 사용자 스위치 On");
        pszTgt = "사용자 Off(Off)";
        stMsg.Format("사용자 Off(%s)", 
            (pMainDlg->m_CHECK_MODE_MANUAL_OFF == FALSE)? "Off": "On");
        pszSrc = stMsg;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        // IMOD 상태
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
		CAN_TimeOutcnt = 0;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "IMOD 상태 - 사용자 스위치 On");
        pszTgt = "정상작동";
        pszSrc = pMainDlg->m_EDIT_AAVM_IMOD_STATUS;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

#if 1
        /////////////////////////////////////
        pMainDlg->m_EDIT_ESROS_WIPER = 1;
        pMainDlg->m_EDIT_ESROS_INTWIPER = 0;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        // 인식모드
		CAN_TimeOutcnt = 0;
        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "인식모드 - Wiper On");
        pszTgt = "Wiper(On)";
        stMsg.Format("Wiper(%s)", 
            (pMainDlg->m_CHECK_MODE_WIPER == FALSE)? "Off": "On");
        pszSrc = stMsg;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        // IMOD 상태
		CAN_TimeOutcnt = 0;
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+7) >= wCAN_AAVO_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000 + 700)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "IMOD 상태 - Wiper On");
        pszTgt = "자동 Off";
        pszSrc = pMainDlg->m_EDIT_AAVM_IMOD_STATUS;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        pMainDlg->m_EDIT_ESROS_WIPER = 0;
        pMainDlg->m_EDIT_ESROS_INTWIPER = 0;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        // 인식모드
		CAN_TimeOutcnt = 0;
        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "인식모드 - Wiper Off");
        pszTgt = "Wiper(Off)";
        stMsg.Format("Wiper(%s)", 
            (pMainDlg->m_CHECK_MODE_WIPER == FALSE)? "Off": "On");
        pszSrc = stMsg;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        // IMOD 상태
		CAN_TimeOutcnt = 0;
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+7) >= wCAN_AAVO_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000 + 700)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "IMOD 상태 - Wiper Off");
        pszTgt = "정상작동";
        pszSrc = pMainDlg->m_EDIT_AAVM_IMOD_STATUS;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        pMainDlg->m_EDIT_ESROS_WIPER = 0;
        pMainDlg->m_EDIT_ESROS_INTWIPER = 1;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        // 인식모드
		CAN_TimeOutcnt = 0;
        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "인식모드 - Int.Wiper On");
        pszTgt = "Int.Wiper(On)";
        stMsg.Format("Int.Wiper(%s)", 
            (pMainDlg->m_CHECK_MODE_INTWIPER == FALSE)? "Off": "On");
        pszSrc = stMsg;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        // IMOD 상태
		CAN_TimeOutcnt = 0;
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+7) >= wCAN_AAVO_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000 + 700)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "IMOD 상태 - Int.Wiper On");
        pszTgt = "자동 Off";
        pszSrc = pMainDlg->m_EDIT_AAVM_IMOD_STATUS;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        pMainDlg->m_EDIT_ESROS_WIPER = 0;
        pMainDlg->m_EDIT_ESROS_INTWIPER = 0;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        // 인식모드
		CAN_TimeOutcnt = 0;
        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "인식모드 - Int.Wiper Off");
        pszTgt = "Int.Wiper(Off)";
        stMsg.Format("Int.Wiper(%s)", 
            (pMainDlg->m_CHECK_MODE_INTWIPER == FALSE)? "Off": "On");
        pszSrc = stMsg;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        // IMOD 상태
		CAN_TimeOutcnt = 0;
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+11) >= wCAN_AAVO_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000 + 1100)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "IMOD 상태 - Int.Wiper Off");
        pszTgt = "정상작동";
        pszSrc = pMainDlg->m_EDIT_AAVM_IMOD_STATUS;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        pMainDlg->m_EDIT_MMS_MOVE    = 1;
        pMainDlg->m_EDIT_HP1_ARM     = 80;
        pMainDlg->m_EDIT_HP3_SWING   = 80;
        pMainDlg->m_EDIT_HP3_BOOM_UP = 80;
        pMainDlg->m_EDIT_HP3_BOOM_DN = 80;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
		CAN_TimeOutcnt = 0;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "인식모드 - 주행중");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "주행(On), 회전(Off), 작업(Off)");

        stMsg.Format("주행(%s), 회전(%s), 작업(%s)", 
            (pMainDlg->m_CHECK_MODE_MOVE  == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_SWING == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_WORK  == FALSE)? "Off": "On");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "주행(On), 회전(Off), 작업(Off)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        pMainDlg->m_EDIT_MMS_MOVE    = 0;
        pMainDlg->m_EDIT_HP1_ARM     = 80;
        pMainDlg->m_EDIT_HP3_SWING   = 81;
        pMainDlg->m_EDIT_HP3_BOOM_UP = 80;
        pMainDlg->m_EDIT_HP3_BOOM_DN = 80;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

		CAN_TimeOutcnt = 0;
        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "인식모드 - 회전중");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "주행(Off), 회전(On), 작업(Off)");

        stMsg.Format("주행(%s), 회전(%s), 작업(%s)", 
            (pMainDlg->m_CHECK_MODE_MOVE  == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_SWING == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_WORK  == FALSE)? "Off": "On");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "주행(Off), 회전(On), 작업(Off)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        pMainDlg->m_EDIT_MMS_MOVE    = 0;
        pMainDlg->m_EDIT_HP1_ARM     = 81;
        pMainDlg->m_EDIT_HP3_SWING   = 80;
        pMainDlg->m_EDIT_HP3_BOOM_UP = 80;
        pMainDlg->m_EDIT_HP3_BOOM_DN = 80;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

		CAN_TimeOutcnt = 0;
        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+7) >= wCAN_AAVM_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000 + 700)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "인식모드 - 작업중");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "주행(Off), 회전(Off), 작업(On)");

        stMsg.Format("주행(%s), 회전(%s), 작업(%s)", 
            (pMainDlg->m_CHECK_MODE_MOVE  == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_SWING == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_WORK  == FALSE)? "Off": "On");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "주행(Off), 회전(Off), 작업(On)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        pMainDlg->m_EDIT_MMS_MOVE    = 0;
        pMainDlg->m_EDIT_HP1_ARM     = 80;
        pMainDlg->m_EDIT_HP3_SWING   = 80;
        pMainDlg->m_EDIT_HP3_BOOM_UP = 81;
        pMainDlg->m_EDIT_HP3_BOOM_DN = 80;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

		CAN_TimeOutcnt = 0;
        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "인식모드 - 작업중");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "주행(Off), 회전(Off), 작업(On)");

        stMsg.Format("주행(%s), 회전(%s), 작업(%s)", 
            (pMainDlg->m_CHECK_MODE_MOVE  == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_SWING == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_WORK  == FALSE)? "Off": "On");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "주행(Off), 회전(Off), 작업(On)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        pMainDlg->m_EDIT_MMS_MOVE    = 0;
        pMainDlg->m_EDIT_HP1_ARM     = 80;
        pMainDlg->m_EDIT_HP3_SWING   = 80;
        pMainDlg->m_EDIT_HP3_BOOM_UP = 80;
        pMainDlg->m_EDIT_HP3_BOOM_DN = 81;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
		CAN_TimeOutcnt = 0;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "인식모드 - 작업중");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "주행(Off), 회전(Off), 작업(On)");

        stMsg.Format("주행(%s), 회전(%s), 작업(%s)", 
            (pMainDlg->m_CHECK_MODE_MOVE  == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_SWING == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_WORK  == FALSE)? "Off": "On");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "주행(Off), 회전(Off), 작업(On)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
        }

        /////////////////////////////////////
        pMainDlg->m_EDIT_MMS_MOVE    = 0;
        pMainDlg->m_EDIT_HP1_ARM     = 0;
        pMainDlg->m_EDIT_HP3_SWING   = 0;
        pMainDlg->m_EDIT_HP3_BOOM_UP = 0;
        pMainDlg->m_EDIT_HP3_BOOM_DN = 0;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
		CAN_TimeOutcnt = 0;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }
////////////////////////////////////////////
// THLEE
// 전방 카메라만 테스트 진행
// 인식률 산정 안함. 인식 결과만 확인.
////////////////////////////////////////////
		unsigned int IMOD_TimeOut = 0;
		MakeMsg_AAVCS(&oCanMsg, 4, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
		CAN_TimeOutcnt = 0;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }

		pMainDlg->m_TXT_GUIDE.m_strMsg = "영상인식 확인.";
		pMainDlg->PostMessage(_WM_THREAD_UPDATE);

		//ui_FStartNumber=1;
		//ui_FStopNumber=375;

		FDrawStop=false;

		//Sleep(6500);
		wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
		while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
		{
			Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
		}
		bool recognition = false;

		// 전방으로 전환.
        MakeMsg_AAVCS(&oCanMsg, 4, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
		CAN_TimeOutcnt = 0;
        pCan->Write(&oCanMsg);
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }
		ui_FStartNumber=1;
		ui_FStopNumber=290;

		while(IMOD_TimeOut<31)
		{
			IMOD_TimeOut++;
			stMsg.Format("%s", pMainDlg->m_EDIT_AAVM_WARNING_FRONT);

			if ((stMsg == "주의")  && (recognition == false))
				recognition = true;
			pFImgDlg->FrontDraw(strFImagePath,ui_FStartNumber,ui_FStopNumber);
			pReImgDlg->RearDraw(strReImagePath,ui_ReStartNumber,ui_ReStopNumber);
			pLImgDlg->LeftDraw(strLImagePath,ui_LStartNumber,ui_LStopNumber);
			pRImgDlg->RightDraw(strRImagePath,ui_RStartNumber,ui_RStopNumber);
			Sleep(100);
		}
		nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
		pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "영상인식 구간 A");
		pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "주의");
		if(recognition == true)
		{
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "주의");
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
			//HWtest[7] += 1;
		}
		else
		{
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
		}
		recognition = false;

		while(IMOD_TimeOut<61)
		{
			IMOD_TimeOut++;
			stMsg.Format("%s", pMainDlg->m_EDIT_AAVM_WARNING_FRONT);

			if ((stMsg == "위험") && (recognition == false)) 
				recognition = true;
			pFImgDlg->FrontDraw(strFImagePath,ui_FStartNumber,ui_FStopNumber);
			pReImgDlg->RearDraw(strReImagePath,ui_ReStartNumber,ui_ReStopNumber);
			pLImgDlg->LeftDraw(strLImagePath,ui_LStartNumber,ui_LStopNumber);
			pRImgDlg->RightDraw(strRImagePath,ui_RStartNumber,ui_RStopNumber);
			Sleep(100);
		}
		nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
		pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "영상인식 구간 B");
		pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "위험");
		if(recognition == true)
		{
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "위험");
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
		}
		else
		{
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
		}
		recognition = false;

		while(IMOD_TimeOut<156)
		{
			IMOD_TimeOut++;
			stMsg.Format("%s", pMainDlg->m_EDIT_AAVM_WARNING_FRONT);

			if ((stMsg == "주의") && (recognition == false)) 
				recognition = true;
			pFImgDlg->FrontDraw(strFImagePath,ui_FStartNumber,ui_FStopNumber);
			pReImgDlg->RearDraw(strReImagePath,ui_ReStartNumber,ui_ReStopNumber);
			pLImgDlg->LeftDraw(strLImagePath,ui_LStartNumber,ui_LStopNumber);
			pRImgDlg->RightDraw(strRImagePath,ui_RStartNumber,ui_RStopNumber);
			Sleep(100);
		}
		nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
		pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "영상인식 구간 C");
		pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "주의");
		if(recognition == true)
		{
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "주의");
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
		}
		else
		{
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
		}
		recognition = false;

		while(IMOD_TimeOut<191)
		{
			IMOD_TimeOut++;
			stMsg.Format("%s", pMainDlg->m_EDIT_AAVM_WARNING_FRONT);

			if ((stMsg == "위험") && (recognition == false)) 
				recognition = true;
			pFImgDlg->FrontDraw(strFImagePath,ui_FStartNumber,ui_FStopNumber);
			pReImgDlg->RearDraw(strReImagePath,ui_ReStartNumber,ui_ReStopNumber);
			pLImgDlg->LeftDraw(strLImagePath,ui_LStartNumber,ui_LStopNumber);
			pRImgDlg->RightDraw(strRImagePath,ui_RStartNumber,ui_RStopNumber);
			Sleep(100);
		}
		nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
		pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "영상인식 구간 D");
		pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "위험");
		if(recognition == true)
		{
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "위험");
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
			//HWtest[7] += 8;
		}
		else
		{
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
		}
		recognition = false;

		while(IMOD_TimeOut<291)
		{
			IMOD_TimeOut++;
			stMsg.Format("%s", pMainDlg->m_EDIT_AAVM_WARNING_FRONT);

			if ((stMsg == "주의") && (recognition == false)) 
				recognition = true;
			pFImgDlg->FrontDraw(strFImagePath,ui_FStartNumber,ui_FStopNumber);
			pReImgDlg->RearDraw(strReImagePath,ui_ReStartNumber,ui_ReStopNumber);
			pLImgDlg->LeftDraw(strLImagePath,ui_LStartNumber,ui_LStopNumber);
			pRImgDlg->RightDraw(strRImagePath,ui_RStartNumber,ui_RStopNumber);
			Sleep(100);
		}
		nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
		pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "영상인식 구간 E");
		pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "주의");
		if(recognition == true)
		{
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "주의");
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
			//HWtest[8] = 1;
		}
		else
		{
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
		}
		recognition = false;
		/*

		while(IMOD_TimeOut<326)
		{
			IMOD_TimeOut++;
			stMsg.Format("%s", pMainDlg->m_EDIT_AAVM_WARNING_FRONT);

			if ((stMsg == "위험") && (recognition == false)) 
				recognition = true;
			pFImgDlg->FrontDraw(strFImagePath,ui_FStartNumber,ui_FStopNumber);
			pReImgDlg->RearDraw(strReImagePath,ui_ReStartNumber,ui_ReStopNumber);
			pLImgDlg->LeftDraw(strLImagePath,ui_LStartNumber,ui_LStopNumber);
			pRImgDlg->RightDraw(strRImagePath,ui_RStartNumber,ui_RStopNumber);
			Sleep(100);
		}
		nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
		pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "영상인식 구간 F");
		pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "위험");
		if(recognition == true)
		{
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "위험");
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
		}
		else
		{
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
		}
		recognition = false;

		while(IMOD_TimeOut<376)
		{
			IMOD_TimeOut++;
			stMsg.Format("%s", pMainDlg->m_EDIT_AAVM_WARNING_FRONT);

			if ((stMsg == "주의") && (recognition == false)) 
				recognition = true;
			pFImgDlg->FrontDraw(strFImagePath,ui_FStartNumber,ui_FStopNumber);
			pReImgDlg->RearDraw(strReImagePath,ui_ReStartNumber,ui_ReStopNumber);
			pLImgDlg->LeftDraw(strLImagePath,ui_LStartNumber,ui_LStopNumber);
			pRImgDlg->RightDraw(strRImagePath,ui_RStartNumber,ui_RStopNumber);
			Sleep(100);
		}
		nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
		pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "영상인식 구간 G");
		pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "주의");
		if(recognition == true)
		{
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "주의");
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
		}
		else
		{
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
		}
		recognition = false;
		*/
		IMOD_TimeOut=0;

		/*
		nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
		pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "영상인식 테스트");
		pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "위험구간 인식");


						//RECOGNIZE
		//Cal_REC = 90 * REN_PERCENT;
		if(IMOD_Cnt >= 5) // IMOD_Cnt == 7
		{
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "주의-위험-주의-위험-주의-위험-주의");
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
		}
		else
		{
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
			pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			tError = true;
			break;
		}
		IMOD_Cnt=0; */

		ui_FStartNumber=0;
		ui_FStopNumber=0;
		Sleep(500);
		MakeMsg_AAVCS(&oCanMsg, 0, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
		CAN_TimeOutcnt = 0;
        pCan->Write(&oCanMsg);
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
            Sleep(10);
			if (CAN_TimeOutcnt++ > 1000)
			{
				tError = true;
				nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
				pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "CAN Time out");
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
				dprintf(("CAN Time out"));
				break;
			}
        }
#endif
	} while(false);
	return (tError == true)? false: true;
}


UINT MainThread(LPVOID pParam)
{
	bool tCriticalError = false;
	FILE *pFileLog;
	char szBarcodePCB[128], szBarcodeCase[128];
	CString strMsg;
	U32 wTimeMsec;


	//TPCANMsg oCanMsg;
	//U32 wCAN_Prev_GetCnt;

	//강제로 Log 파일 이름 지정: mook 2011.10.17
	CString szTemp;
	CTime t = CTime::GetCurrentTime();
	szTemp.Format("D:\\AAVM_LOG\\%s %d-%02d-%02d.log", pSetupDlg->m_COMBO_CAR_TYPE, t.GetYear(), t.GetMonth(), t.GetDay());
	pSetupDlg->m_EDIT_FILENAME_LOG = szTemp;


	dprintf(("Main Thread Start\n"));

	// Init Test Mode
	//    iMainThreadTestMode = _MODE_START_DEFAULT;
	if (pMainDlg->m_COMBO_PROGRAM_TYPE=="최종검사1(PCB ASSY)")
	{
		iMainThreadTestMode = _MODE_MAIN;
	}
	else if(pMainDlg->m_COMBO_PROGRAM_TYPE=="최종검사2(ASSY)")
	{
		iMainThreadTestMode = _MODE_MAIN_2TH;

		szTemp.Format("D:\\AAVM_LOG(2TH)\\%s %d-%02d-%02d.log", pSetupDlg->m_COMBO_CAR_TYPE, t.GetYear(), t.GetMonth(), t.GetDay());
		pSetupDlg->m_EDIT_FILENAME_LOG = szTemp;
	}
	else
	{
		iMainThreadTestMode = _MODE_MASTER_GOOD;	//마스터샘플 5회 체크
	}

	//Init sleep time
	if (pSetupDlg->m_COMBO_TIME_DELAY!="")
	{
		m_sleep_time = (DWORD)atoi(pSetupDlg->m_COMBO_TIME_DELAY);
	}
	else
	{
		m_sleep_time = 100;
	}


	
	U16 CANSHORT;
	// Init CAN
	if (tCriticalError == false)
	{
		if (OpenCan() == true)
		{
			dprintf(("CAN Initialize OK\n"));
		}
		else
		{
			AfxMessageBox("CAN Initialize Error!");
			dprintf(("CAN Initialize Error\n"));
			tCriticalError = true;
		}
	}
	
#ifdef Sigle_PC_TEST
	// Init Barcode1
	if (tCriticalError == false)
	{
		pBarCode1 = new CBarcodeMgt;
		if (pBarCode1 == NULL)
		{
			tCriticalError = true;
		}
		else
		{
			if (pBarCode1->Open((LPCSTR)pSetupDlg->m_EDIT_COM_BARCODE1) == true)
			{
				pBarCode1->Thread_Begin();
				dprintf(("Barcode1 Open Success!\n"));
			}
			else
			{
				AfxMessageBox("Barcode1 Open Fail!\n");
				dprintf(("Barcode1 Open Fail!\n"));
				tCriticalError = true;
			}
		}
	}

	// Init Barcode2
	if (tCriticalError == false)
	{
		pBarCode2 = new CBarcodeMgt;
		if (pBarCode2 == NULL)
		{
			tCriticalError = true;
		}
		else
		{
			if (pBarCode2->Open((LPCSTR)pSetupDlg->m_EDIT_COM_BARCODE2) == true)
			{
				pBarCode2->Thread_Begin();
				dprintf(("Barcode2 Open Success!\n"));
			}
			else
			{
				AfxMessageBox("Barcode2 Open Fail!\n");
				dprintf(("Barcode2 Open Fail!\n"));
				tCriticalError = true;
			}
		}
	}

	// Init Screw Driver
	if (tCriticalError == false)
	{
		pScrew = new CScrewMgt;
		if (pScrew == NULL)
		{
			tCriticalError = true;
		}
		else
		{
			if (pScrew->Open((LPCSTR)pSetupDlg->m_EDIT_COM_SCREW) == true)
			{
				pScrew->Thread_Begin();
#if _SCREW_SERIAL
				pScrew->m_nScrew_State=_SCREW_RST;
#endif
				dprintf(("Screw Open Success!\n"));
			}
			else
			{
				AfxMessageBox("Screw Open Fail!\n");
				dprintf(("Screw Open Fail!\n"));
				tCriticalError = true;
			}
		}
	}
#if 1
	// Init IO Board
	if (tCriticalError == false)
	{
		if (OpenIoBoard() == true)
		{
			dprintf(("IO Board Open Success!\n"));
		}
		else
		{
			AfxMessageBox("IO Board Open Fail!\n");
			dprintf(("IO Board Open Fail!\n"));
			tCriticalError = true;
		}
	}
#endif

#if 1
	//psj AmMeter Check
	// Init AmMeter
	if (tCriticalError == false)
	{
		pAmMeter = new CAmMeterMgt;
		if (pAmMeter == NULL)
		{
			tCriticalError = true;
		}
		else
		{
			if (pAmMeter->Open((LPCSTR)pSetupDlg->m_EDIT_COM_AMMETER) == true)
			{
				pAmMeter->Thread_Begin();
				dprintf(("AmMeter Open Success!\n"));
			}
			else
			{
				AfxMessageBox("AmMeter Open Fail!\n");
				dprintf(("AmMeter Open Fail!\n"));
				tCriticalError = true;
			}
		}
	}
#endif

#endif
	// Init Log File
	pFileLog = fopen(pSetupDlg->m_EDIT_FILENAME_LOG, "a+t");
	if (pFileLog != NULL)
	{
		CTime oTime = CTime::GetCurrentTime();

		fseek(pFileLog, SEEK_END, 0);
		fprintf(pFileLog, "\nLDWS 테스트 시작: %04d-%02d-%02d %02d:%02d:%02d\n",
			oTime.GetYear(), oTime.GetMonth(), oTime.GetDay(),
			oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());
		fprintf(pFileLog, "테스트 라인: %s,    ", pMainDlg->m_EDIT_TESTER_LINE);
		fprintf(pFileLog, "검사자: %s\n", pMainDlg->m_EDIT_TESTER_NAME);
		fclose(pFileLog);

		dprintf(("Log File <%s> Open Success: Start.\n", (LPCTSTR)pSetupDlg->m_EDIT_FILENAME_LOG));
	}
	else
	{
		strMsg.Format("Log File <%s> Open Fail: Start!\n", (LPCTSTR)pSetupDlg->m_EDIT_FILENAME_LOG);
		//AfxMessageBox(strMsg);
		dprintf(((LPCTSTR)strMsg));
		tCriticalError = true;
	}
	
#ifdef Sigle_PC_TEST
	for(int i=0; i<3; i++)
	{

		CANSHORT = pIoBoard->GetOutPort() & 0x40;
	}


	// 장착 해제 스위치를 끄고 황색등을 켠다.
	for(int k=0; k<3; k++)
	{
		pIoBoard->PutOutPort(_PIN_OUT_DISCONNECT|_PIN_OUT_LED_Y | CANSHORT);
	}
#endif
	pMainDlg->m_TXT_GUIDE.SetBgColor(RGB(255, 255, 0));

	pMainDlg->m_EDIT_TEST_NUM_OK   = 0;
	pMainDlg->m_EDIT_TEST_NUM_ALL  = 0;
	pMainDlg->m_EDIT_TEST_NUM_RATE = "  0.0%";

	tBoxBarcodeState = true;

	while ( (tCriticalError == false) && (tMainThreadStop == false) )
	{
		wMainThreadTestStep = _STEP_INIT;
		m_nImgView = 1;

		tMainThreadTestError = false;
		tMainThreadManulError = false;
		iMainThreadTestError = _ERR_OK;

		if (iMainThreadTestMode == _MODE_MAIN)
		{
			EnableWindow(GetDlgItem(pMainDlg->m_hWnd, IDC_BTN_MAUAL_ERROR), TRUE);
		}

		pMainDlg->m_TXT_GUIDE.SetBgColor(RGB(255, 255, 0));
#ifdef Sigle_PC_TEST
		pIoBoard->PutOutPort(_PIN_OUT_DISCONNECT|_PIN_OUT_LED_Y | CANSHORT);
#endif
		// 초기화
		pMainDlg->m_LIST_PROCESS.DeleteAllItems();
		pMainDlg->PostMessage(_WM_THREAD_UPDATE);

		//psj 바코드 작업 수정 진행 필요.
#ifdef Sigle_PC_TEST

#if 0
		// BOX 바코드 등록
		if ((tBoxBarcodeState == true) || (pMainDlg->m_EDIT_BOX_UNIT_CNT == pMainDlg->m_EDIT_BOX_UNIT_MAX))
		{
			if (pMainDlg->m_EDIT_BOX_UNIT_CNT == pMainDlg->m_EDIT_BOX_UNIT_MAX)
			{
				pMainDlg->m_EDIT_BOX_UNIT_CNT = 0;
			}

			if ( (tCriticalError != false) || (tMainThreadStop != false) )
			{
				break;
			}
			if ( (tMainThreadTestError == false) && (iMainThreadTestMode == _MODE_MAIN) )
			{
				if (Step_ReadBarcode1_BOX(szBarcodeBOX) == false)
				{
					tMainThreadTestError = true;
				}
				else
				{
					if (strcmp(szBarcodeBOX, pBarCode1->GetDataAt(0)) != 0)
					{
						strcpy_s(szBarcodeBOX, pBarCode1->GetDataAt(0));
					}
					else
					{
						tMainThreadTestError = true;
					}
				}
			}
			if (tMainThreadManulError == true)
			{
				pBarCode1->DelAllData();
				pScrew->DelAllCount();
				continue;
			}
			else
			{
				if (iMainThreadTestMode == _MODE_MAIN)
				{
					tBoxBarcodeState = false;
				}
			}
		}
		else
		{
			if (iMainThreadTestMode == _MODE_MAIN)
			{
				pBarCode1->AddData(szBarcodeBOX);
			}
		}
#endif

		// PCB 바코드 등록
		if ( (tCriticalError != false) || (tMainThreadStop != false) )
		{
			break;
		}
		if ( (tMainThreadTestError == false) && (iMainThreadTestMode == _MODE_MAIN) )
		{
			if (Step_ReadBarcode1_PCB() == false)
			{
				tMainThreadTestError = true;
			}
		}
		if (tMainThreadManulError == true)
		{
			pBarCode1->DelAllData();
			pScrew->DelAllCount();
			continue;
		}

		// 스크류 작업
		if ( (tCriticalError != false) || (tMainThreadStop != false) )
		{
			break;
		}

		if (iMainThreadTestMode == _MODE_MAIN)
		{
			if (tMainThreadTestError == false)
			{
				if (Step_Screw_Work() == false)
				{
					tMainThreadTestError = true;
				}
			}
			if (tMainThreadManulError == true)
			{
				pBarCode1->DelAllData();
				pScrew->DelAllCount();
				continue;
			}
		}



		// 케이스 바코드 등록
		if ( (tCriticalError != false) || (tMainThreadStop != false) )
		{
			break;
		}
		if ( (tMainThreadTestError == false) && (iMainThreadTestMode == _MODE_MAIN) )
		{
			if (Step_ReadBarcode1_Case() == false)
			{
				tMainThreadTestError = true;
			}

			if (tMainThreadManulError == true)
			{
				pBarCode1->DelAllData();
				pScrew->DelAllCount();
				continue;
			}
		}
		else if( (tMainThreadTestError == false) && (iMainThreadTestMode == _MODE_MAIN_2TH) )
		{
			if (Step_ReadBarcode1_Case2() == false)
			{
				tMainThreadTestError = true;
			}

			if (tMainThreadManulError == true)
			{
				pBarCode1->DelAllData();
				pScrew->DelAllCount();
				continue;
			}
		}
#endif

		//////////////////// 장치 테스트 시작 //////////////////////////

		// 장치 연결 작업
		if ( (tCriticalError != false) || (tMainThreadStop != false) )
		{
			break;
		}
		if (tMainThreadTestError == false)
		{
			if (Step_Connect() == false)
			{
				tMainThreadTestError = true;
			}

			if (tMainThreadManulError == true)
			{
				pBarCode1->DelAllData();
				pScrew->DelAllCount();
				continue;
			}
		}
	    if ( (tCriticalError != false) || (tMainThreadStop != false) )
		{
			break;
		}

		if (tMainThreadTestError == false)
		{
			if (Step_Test_Start() == false)
			{
				tMainThreadTestError = true;
			}
		}
		// ECU ID 테스트를 포함한 LDWS 테스트
		if ( (tCriticalError != false) || (tMainThreadStop != false) )
		{
			break;
		}

		if (tMainThreadTestError == false)
		{

#if 0
			if ( (pSetupDlg->m_COMBO_CAR_TYPE != "") && (pSetupDlg->m_COMBO_LOCATION == "통합") )
			{
				if (Step_Test_AAVM_ALL() == false)
				{
					tMainThreadTestError = true;
				}
			}
			else
			{
				pMainDlg->m_TXT_GUIDE.m_strMsg = "지원하지 않는 장치 입니다.";
				pMainDlg->PostMessage(_WM_THREAD_UPDATE);
				tCriticalError = true;
			}
#endif
			
			if (Step_Test_AAVM_ALL() == false)
			{
				tMainThreadTestError = true;
			}
			


		}

		// 테스트 종료

		if ( (tCriticalError != false) || (tMainThreadStop != false) )
		{
			break;
		}


		if ((wMainThreadTestStep & _STEP_TEST_START) != 0)
		{
			//Sleep(1000);
			Step_Test_End();
		}

		//////////////////// 장치 테스트 종료 //////////////////////////

		// 장치 테스트 결과 표시
		if (tMainThreadTestError == false)
		{
			pMainDlg->m_TXT_GUIDE.SetBgColor(RGB(0, 255, 0));
			pMainDlg->m_TXT_GUIDE.m_strMsg = "양품";
#ifdef Sigle_PC_TEST
			pIoBoard->PutOutBit(_BIT_OUT_LED_G, true);
			pIoBoard->PutOutBit(_BIT_OUT_LED_R, false);
#endif
			CString strFile;
			strFile = "tada.wav";
			sndPlaySound(strFile, SND_ASYNC);

		}
		else
		{
			pMainDlg->m_TXT_GUIDE.SetBgColor(RGB(255, 0, 0));
			pMainDlg->m_TXT_GUIDE.m_strMsg = "불량";
#ifdef Sigle_PC_TEST
			pIoBoard->PutOutBit(_BIT_OUT_LED_G, false);
			pIoBoard->PutOutBit(_BIT_OUT_LED_R, true);
#endif
		}

		pMainDlg->PostMessage(_WM_THREAD_UPDATE);

		// 케이스 바코드 확인
		szBarcodeCase[0] = 0;
		szBarcodePCB [0] = 0;
#if 0
		if ( (tCriticalError != false) || (tMainThreadStop != false) )
		{
			break;
		}
#endif
		bool tBarcodeOk = true;
		if (iMainThreadTestMode == _MODE_MAIN)
		{
			tBarcodeOk = Step_ReadBarcode2_Case(szBarcodeCase, szBarcodePCB);
			if (tBarcodeOk == false)
			{
				tMainThreadTestError = true;
			}
		}
		else if(iMainThreadTestMode == _MODE_MAIN_2TH)
		{
			tBarcodeOk = Step_ReadBarcode2_Case2(szBarcodeCase);
			if (tBarcodeOk == false)
			{
				tMainThreadTestError = true;
			}
		}

		bool tDisplayUpdate = false;
		if ( ((wMainThreadTestStep & _STEP_BARCODE2_CASE) == 0) || (tBarcodeOk == false) )
		{
			tDisplayUpdate = true;
		}

		if ( (tMainThreadManulError == true) && (tMainThreadTestError == false) )
		{
			tDisplayUpdate = true;
			tMainThreadTestError = true;
		}

		if (Step_SaveLogFile(szBarcodeCase, szBarcodePCB) == false)
		{
			dprintf(("Log File Saving Error! Test Terminated!\n"));
			AfxMessageBox("로그 파일 저장에서 에러가 발생 했습니다.\n 테스트 중단합니다.");
			pMainDlg->m_TXT_GUIDE.m_strMsg = "로그 파일 저장 에러";

			tCriticalError = true;
		}

#if 0
		// kbr cpk
		// 로그 파일 기록
		if ( (tCriticalError != false) || (tMainThreadStop != false) )
		{
			break;
		}

		if (Step_SaveCPKFile(szBarcodeCase, szBarcodePCB, szBarcodeBOX) == false)
		{
			dprintf(("CPK File Saving Error! Test Terminated!\n"));
			AfxMessageBox("로그 파일 저장에서 에러가 발생 했습니다.\n 테스트 중단합니다.");
			pMainDlg->m_TXT_GUIDE.m_strMsg = "로그 파일 저장 에러";

			tCriticalError = true;
		}
#endif
		wMainThreadTestStep |= _STEP_END;

		// 검사 최종 결과 표시 및 검사 통계 처리
		if (tDisplayUpdate == true)
		{
			if (tMainThreadTestError == false)
			{
				pMainDlg->m_TXT_GUIDE.SetBgColor(RGB(0, 255, 0));
			}
			else
			{
				pMainDlg->m_TXT_GUIDE.SetBgColor(RGB(255, 0, 0));
			}

			pMainDlg->PostMessage(_WM_THREAD_UPDATE);

			wTimeMsec = GetTickCount();
			while (tMainThreadStop == false)
			{
				Sleep(10);

				if (tMainThreadStop != false)
				{
					break;
				}

				if (GetTickCount() >= (wTimeMsec + (1000*1)))
				{
					break;
				}
			}
		}

		if (tMainThreadTestError == false)
		{
			if (tDisplayUpdate == true)
			{
				pMainDlg->m_TXT_GUIDE.SetBgColor(RGB(0, 255, 0));
				pMainDlg->m_TXT_GUIDE.m_strMsg = "양품";
#ifdef Sigle_PC_TEST
				pIoBoard->PutOutBit(_BIT_OUT_LED_G, true);
				pIoBoard->PutOutBit(_BIT_OUT_LED_R, false);
#endif
			}
			pMainDlg->m_EDIT_TEST_NUM_OK++;

		}
		else
		{
			if (tDisplayUpdate == true)
			{
				pMainDlg->m_TXT_GUIDE.SetBgColor(RGB(255, 0, 0));
				pMainDlg->m_TXT_GUIDE.m_strMsg = "불량";
#ifdef Sigle_PC_TEST
				pIoBoard->PutOutBit(_BIT_OUT_LED_G, false);
				pIoBoard->PutOutBit(_BIT_OUT_LED_R, true);
#endif
			}
		}

		if ((iMainThreadTestMode != _MODE_MASTER_GOOD)
			&& (iMainThreadTestMode != _MODE_MASTER_BAD)
			&& (tMainThreadTestError == false))
		{
			pMainDlg->m_EDIT_BOX_UNIT_CNT++;
		}

		pMainDlg->m_EDIT_TEST_NUM_ALL++;

		pMainDlg->m_EDIT_TEST_NUM_RATE.Format("%5.1f%%",
			(double)pMainDlg->m_EDIT_TEST_NUM_OK*100.0/(double)pMainDlg->m_EDIT_TEST_NUM_ALL);

		pMainDlg->PostMessage(_WM_THREAD_UPDATE);

		if (tDisplayUpdate == true)
		{
			wTimeMsec = GetTickCount();
			while (tMainThreadStop == false)
			{
				Sleep(10);

				if (tMainThreadStop != false)
				{
					break;
				}

				if (GetTickCount() >= (wTimeMsec + (1000*2)))
				{
					break;
				}
			}
		}

		pMainDlg->m_TXT_GUIDE.SetBgColor(RGB(255, 255, 0));
#ifdef Sigle_PC_TEST
		pIoBoard->PutOutBit(_BIT_OUT_LED_Y, true);
		pIoBoard->PutOutBit(_BIT_OUT_LED_G, false);
		pIoBoard->PutOutBit(_BIT_OUT_LED_R, false);
#endif
		pMainDlg->PostMessage(_WM_THREAD_UPDATE);

		// 모드 체크 및 변경
		if (iMainThreadTestMode == _MODE_MASTER_GOOD)
		{
			if (tMainThreadTestError == true)
			{
				pMainDlg->m_TXT_GUIDE.m_strMsg = "양품 마스터 샘플 검사 실패";
				AfxMessageBox("양품 마스터 샘플 검사에서 불량이 발생했습니다.\n검사 공정을 중단합니다.");
				tCriticalError = true;
			}
			else
			{
				if (pMainDlg->m_EDIT_TEST_NUM_ALL >= 5)
				{
					tMainThreadStop = true;
					/*
					iMainThreadTestMode = _MODE_MASTER_BAD;
					pMainDlg->m_EDIT_TEST_NUM_OK  = 0;
					pMainDlg->m_EDIT_TEST_NUM_ALL = 0;
					pMainDlg->m_EDIT_TEST_NUM_RATE = "0.0%%";
					*/
				}
			}

		}
		else if (iMainThreadTestMode == _MODE_MASTER_BAD)
		{
			if (tMainThreadTestError == false)
			{
				pMainDlg->m_TXT_GUIDE.m_strMsg = "불량 마스터 샘플 검사 실패";
				AfxMessageBox("불량 마스터 샘플을 양품으로 판정했습니다.\n검사 공정을 중단합니다.");
				tCriticalError = true;
			}
			else
			{
				if (pMainDlg->m_EDIT_TEST_NUM_ALL >= 5)
				{
					iMainThreadTestMode = _MODE_MAIN;
					pMainDlg->m_EDIT_TEST_NUM_OK  = 0;
					pMainDlg->m_EDIT_TEST_NUM_ALL = 0;
					pMainDlg->m_EDIT_TEST_NUM_RATE = "0.0%%";

					pBarCode1->DelAllData();
					pBarCode2->DelAllData();
					pScrew->DelAllCount();
				}
			}
		}
	}
#ifdef Sigle_PC_TEST
	pIoBoard->PutOutBit(_BIT_OUT_LED_Y, false);
	pIoBoard->PutOutBit(_BIT_OUT_LED_G, false);
	pIoBoard->PutOutBit(_BIT_OUT_LED_R, false);
#endif
	dprintf(("Main Thread Step 2\n"));

	
	if (pCan != NULL)
	{
		delete pCan;
		pCan = NULL;
	}
	
#ifdef Sigle_PC_TEST
	// 전원 끔 & 장착 해제
	pIoBoard->PutOutBit(_BIT_OUT_POWER, false);

	if ((pIoBoard->GetInPort() & _PIN_IN_CONNECT) == 0)
	{
		pIoBoard->PutOutBit(_BIT_OUT_DISCONNECT, false);
		Sleep(100);
		pIoBoard->PutOutBit(_BIT_OUT_DISCONNECT, true);
	}

	if (pBarCode1 != NULL)
	{
		delete pBarCode1;
		pBarCode1 = NULL;
	}

	if (pBarCode2 != NULL)
	{
		delete pBarCode2;
		pBarCode2 = NULL;
	}

	if (pScrew != NULL)
	{
		delete pScrew;
		pScrew = NULL;
	}

	if (pIoBoard != NULL)
	{
		delete pIoBoard;
		pIoBoard = NULL;
	}

	if (pAmMeter != NULL)
	{
		delete pAmMeter;
		pAmMeter = NULL;
	}
#endif
	pFileLog = fopen(pSetupDlg->m_EDIT_FILENAME_LOG, "a+t");
	if (pFileLog != NULL)
	{
		/*
		if (iMainThreadTestMode != _MODE_MAIN)
		{
			pMainDlg->m_EDIT_TEST_NUM_OK  = 0;
			pMainDlg->m_EDIT_TEST_NUM_ALL = 0;
			pMainDlg->m_EDIT_TEST_NUM_RATE = "0.0%%";
		}
		*/

		CTime oTime = CTime::GetCurrentTime();

		fseek(pFileLog, SEEK_END, 0);

		fprintf(pFileLog, "\nLDWS 테스트 통계: 양품 %d, 불량 %d, 전체 %d, 양품률 %s\n",
			pMainDlg->m_EDIT_TEST_NUM_OK,
			pMainDlg->m_EDIT_TEST_NUM_ALL - pMainDlg->m_EDIT_TEST_NUM_OK,
			pMainDlg->m_EDIT_TEST_NUM_ALL,
			pMainDlg->m_EDIT_TEST_NUM_RATE);

		fprintf(pFileLog, "LDWS 테스트 종료: %04d-%02d-%02d %02d:%02d:%02d\n\n",
			oTime.GetYear(), oTime.GetMonth(), oTime.GetDay(),
			oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());

		fclose(pFileLog);

		dprintf(("Log File <%s> Open Success: End.\n", (LPCTSTR)pSetupDlg->m_EDIT_FILENAME_LOG));
	}
	else
	{
		strMsg.Format("Log File <%s> Open Fail: End!\n", (LPCTSTR)pSetupDlg->m_EDIT_FILENAME_LOG);
		AfxMessageBox(strMsg);
		dprintf(((LPCTSTR)strMsg));
	}

	dprintf(("Main Thread End\n\n"));

	pMainDlg->PostMessage(_WM_THREAD_END);
	return 0;
}
