// can_msg_app.cpp: Functions for AAVM CAN Messages. Kim JinWoo, 2014-08-08.

#include "stdafx.h"
#include "MainApp.h"
#include "MainDlg.h"
#include "wm_user.h"
#include "pcan_inf.h"
#include "sys_ctimer.h"
#include "GetTickCountEx.h"
#include "can_msg.h"
#include <io.h>

extern MainDlg *pMainDlg;
extern CPcan   *pCan;

void MakeMsg_ESROS(CAN_MSG* a_pCanMsg)
{
    U16 uData;

    //////////////////////////////////////
    // CAN data length를 설정한다. 0~8
    //////////////////////////////////////
    a_pCanMsg->bLen = 8;

    //////////////////////////////////////
    // CAN data를 설정한다.
    //////////////////////////////////////
    uData = ((pMainDlg->m_EDIT_ESROS_WIPER & 0x03)<<4) | ((pMainDlg->m_EDIT_ESROS_INTWIPER & 0x03)<<6);
    a_pCanMsg->arbData[0] = (U08)uData;
    a_pCanMsg->arbData[1] = 0x00;
    a_pCanMsg->arbData[2] = 0x00;
    a_pCanMsg->arbData[3] = 0x00;

    a_pCanMsg->arbData[4] = 0xFF;
    a_pCanMsg->arbData[5] = 0xFF;
    a_pCanMsg->arbData[6] = 0xFF;
    a_pCanMsg->arbData[7] = 0xFF;
}


void MakeMsg_MMS(CAN_MSG* a_pCanMsg)
{
    U16 uData;

    //////////////////////////////////////
    // CAN data length를 설정한다. 0~8
    //////////////////////////////////////
    a_pCanMsg->bLen = 8;

    //////////////////////////////////////
    // CAN data를 설정한다.
    //////////////////////////////////////
    a_pCanMsg->arbData[0] = 0x00;
    a_pCanMsg->arbData[1] = 0x00;
    a_pCanMsg->arbData[2] = 0x00;
    a_pCanMsg->arbData[3] = 0x00;
    a_pCanMsg->arbData[4] = 0x00;
    a_pCanMsg->arbData[5] = 0x00;
    a_pCanMsg->arbData[6] = 0x00;

    uData = pMainDlg->m_EDIT_MMS_MOVE;
    a_pCanMsg->arbData[7] = 0xFC | (U08)(uData&0x03);
}

void MakeMsg_HP1(CAN_MSG* a_pCanMsg)
{
    U16 uData;

    //////////////////////////////////////
    // CAN data length를 설정한다. 0~8
    //////////////////////////////////////
    a_pCanMsg->bLen = 8;

    //////////////////////////////////////
    // CAN data를 설정한다.
    //////////////////////////////////////
    a_pCanMsg->arbData[0] = 0x00;
    a_pCanMsg->arbData[1] = 0x00;
    a_pCanMsg->arbData[2] = 0x00;
    a_pCanMsg->arbData[3] = 0x00;
    a_pCanMsg->arbData[4] = 0x00;
    a_pCanMsg->arbData[5] = 0x00;

    uData = pMainDlg->m_EDIT_HP1_ARM;
    a_pCanMsg->arbData[6] = (U08)uData;
    a_pCanMsg->arbData[7] = (U08)(uData>>8);
}


void MakeMsg_HP3(CAN_MSG* a_pCanMsg)
{
    U16 uData;

    //////////////////////////////////////
    // CAN data length를 설정한다. 0~8
    //////////////////////////////////////
    a_pCanMsg->bLen = 8;

    //////////////////////////////////////
    // CAN data를 설정한다.
    //////////////////////////////////////
    uData = pMainDlg->m_EDIT_HP3_BOOM_UP;
    a_pCanMsg->arbData[0] = (U08)uData;
    a_pCanMsg->arbData[1] = (U08)(uData>>8);

    uData = pMainDlg->m_EDIT_HP3_BOOM_DN;
    a_pCanMsg->arbData[2] = (U08)uData;
    a_pCanMsg->arbData[3] = (U08)(uData>>8);

    uData = pMainDlg->m_EDIT_HP3_SWING;
    a_pCanMsg->arbData[4] = (U08)uData;
    a_pCanMsg->arbData[5] = (U08)(uData>>8);

    a_pCanMsg->arbData[6] = 0;
    a_pCanMsg->arbData[7] = 0;
}


const char *arszAAVO_ViewMode[16] =
{
    "옴니(전)",
    "옴니(후)",
    "버드뷰",
    "4채널",
    "전방",
    "후방",
    "좌측",
    "우측",
    "옴니(좌)",
    "옴니(우)",
    "미정의10",
    "미정의11",
    "미정의12",
    "미정의13",
    "미정의14",
    "미정의15"
};

const char *arszAAVO_Warning[4] =
{
    "-",
    "주의",
    "위험",
    "미정의"
};

const char *arszAAVO_Model_Status[8] =
{
    "설정완료",
    "신호없음",
    "신호이상",
    "미정모델",
    "설정실패",
    "미정의5",
    "미정의6",
    "미정의7"
};

const char *arszAAVO_IMOD_Status[8] =
{
    "정상작동",
    "자동 Off",
    "수동 Off",
    "미정의3",
};


U32 wCAN_AAVO_GetCnt;

void UseMsg_AAVO(CAN_MSG* a_pCanMsg)
{
    U08 bData;

    //////////////////////////////////////
    // CAN data를 화면에 출력한다.
    //////////////////////////////////////
    bData = a_pCanMsg->arbData[0] & 0x0F;
    pMainDlg->m_EDIT_AAVM_VIEW_MODE = arszAAVO_ViewMode[bData];

    bData = (a_pCanMsg->arbData[0] >> 4) & 0x03;
    pMainDlg->m_EDIT_AAVM_IMOD_STATUS = arszAAVO_IMOD_Status[bData];
   
    bData = (a_pCanMsg->arbData[1] >> 0) & 0x03;
    pMainDlg->m_EDIT_AAVM_WARNING_FRONT = arszAAVO_Warning[bData];

    bData = (a_pCanMsg->arbData[1] >> 2) & 0x03;
    pMainDlg->m_EDIT_AAVM_WARNING_REAR = arszAAVO_Warning[bData];

    bData = (a_pCanMsg->arbData[1] >> 4) & 0x03;
    pMainDlg->m_EDIT_AAVM_WARNING_LEFT = arszAAVO_Warning[bData];

    bData = (a_pCanMsg->arbData[1] >> 6) & 0x03;
    pMainDlg->m_EDIT_AAVM_WARNING_RIGHT = arszAAVO_Warning[bData];

    bData = (((a_pCanMsg->arbData[5] & 0x1F) == 0)? 0: 1) | (((a_pCanMsg->arbData[6] & 0x1F) == 0)? 0: 1);
    pMainDlg->m_EDIT_AAVM_DTC = (bData == 0)? "정상": "고장";

    bData = (a_pCanMsg->arbData[5] >> 5) & 0x07;
    pMainDlg->m_EDIT_AAVM_MODEL_STATUS = arszAAVO_Model_Status[bData];

    //////////////////////////////////////
    // CAN Message Data를 Window 화면에 출력한다.
    //////////////////////////////////////
    pMainDlg->m_EDIT_CAN_AAVO.Format("%02X %02X %02X %02X %02X %02X %02X %02X",
        a_pCanMsg->arbData[0],a_pCanMsg->arbData[1],a_pCanMsg->arbData[2],a_pCanMsg->arbData[3],
        a_pCanMsg->arbData[4],a_pCanMsg->arbData[5],a_pCanMsg->arbData[6],a_pCanMsg->arbData[7]);

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

    //////////////////////////////////////
    // CAN Message 수신 카운터를 갱신한다.
    //////////////////////////////////////
    wCAN_AAVO_GetCnt++;
}

U32 wCAN_AAVM_GetCnt;

void UseMsg_AAVM(CAN_MSG* a_pCanMsg)
{
    U08 bData;

    //////////////////////////////////////
    // CAN data를 화면에 출력한다.
    //////////////////////////////////////
    bData = a_pCanMsg->arbData[0];
    pMainDlg->m_CHECK_MODE_NIGHT = (bData == 0)? FALSE: TRUE;

    bData = a_pCanMsg->arbData[1];
    pMainDlg->m_CHECK_MODE_MOVE  = (bData == 0)? FALSE: TRUE;

    bData = a_pCanMsg->arbData[2];
    pMainDlg->m_CHECK_MODE_SWING = (bData == 0)? FALSE: TRUE;

    bData = a_pCanMsg->arbData[3];
    pMainDlg->m_CHECK_MODE_WORK  = (bData == 0)? FALSE: TRUE;

    bData = a_pCanMsg->arbData[4];
    pMainDlg->m_CHECK_MODE_WIPER = (bData == 0)? FALSE: TRUE;

    bData = a_pCanMsg->arbData[5];
    pMainDlg->m_CHECK_MODE_INTWIPER = (bData == 0)? FALSE: TRUE;

    bData = a_pCanMsg->arbData[6];
    pMainDlg->m_CHECK_MODE_MANUAL_OFF = (bData == 0)? FALSE: TRUE;

    //////////////////////////////////////
    // CAN Message Data를 Window 화면에 출력한다.
    //////////////////////////////////////

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

    //////////////////////////////////////
    // CAN Message 수신 카운터를 갱신한다.
    //////////////////////////////////////
    wCAN_AAVM_GetCnt++;
}

const char *arszAAVST_CameraStatus[4] =
{
    "초기화",
    "정상",
    "고장",
    "무응답"
};

const char *arszAAVST_EmeraldStatus[16] =
{
    "초기화",
    "정상",
    "SW장애",
    "미보정",
    "SW에러",
    "미지정5",
    "미지정6",
    "미지정7",
    "HW에러8",
    "HW에러9",
    "HW에러10",
    "HW에러11",
    "HW에러12",
    "HW에러13",
    "HW에러14",
    "HW에러15",
};

U32 wCAN_AAVST_GetCnt;

U08 bCAN_AAVST_CameraGain_Front;
U08 bCAN_AAVST_CameraGain_Rear;
U08 bCAN_AAVST_CameraGain_Left;
U08 bCAN_AAVST_CameraGain_Right;
U08 bCAN_AAVST_BF609_Stat;
U08 bCAN_AAVST_BF609_FPS;

void UseMsg_AAVST(CAN_MSG* a_pCanMsg)
{
    U08 bData;

    //////////////////////////////////////
    // CAN data를 화면에 출력한다.
    //////////////////////////////////////
    // Camera Status
    bData = (a_pCanMsg->arbData[0] >> 0) & 0x03;
    pMainDlg->m_EDIT_CAMERA_STAT_FRONT = arszAAVST_CameraStatus[bData];

    bData = (a_pCanMsg->arbData[0] >> 2) & 0x03;
    pMainDlg->m_EDIT_CAMERA_STAT_REAR  = arszAAVST_CameraStatus[bData];

    bData = (a_pCanMsg->arbData[0] >> 4) & 0x03;
    pMainDlg->m_EDIT_CAMERA_STAT_LEFT  = arszAAVST_CameraStatus[bData];

    bData = (a_pCanMsg->arbData[0] >> 6) & 0x03;
    pMainDlg->m_EDIT_CAMERA_STAT_RIGHT = arszAAVST_CameraStatus[bData];

    // Camera Luma(Gain)
    bCAN_AAVST_CameraGain_Front = bData = a_pCanMsg->arbData[1];
    pMainDlg->m_EDIT_CAMERA_GAIN_FRONT.Format("%d", bData);

    bCAN_AAVST_CameraGain_Rear = bData = a_pCanMsg->arbData[2];
    pMainDlg->m_EDIT_CAMERA_GAIN_REAR.Format("%d", bData);

    bCAN_AAVST_CameraGain_Left = bData = a_pCanMsg->arbData[3];
    pMainDlg->m_EDIT_CAMERA_GAIN_LEFT.Format("%d", bData);

    bCAN_AAVST_CameraGain_Right = bData = a_pCanMsg->arbData[4];
    pMainDlg->m_EDIT_CAMERA_GAIN_RIGHT.Format("%d", bData);

    // Emerald Status
    bData = a_pCanMsg->arbData[5];
    if (bData < 16)
    {
        pMainDlg->m_EDIT_EMERALD_STAT = arszAAVST_EmeraldStatus[bData];
    }
    else
    {
        pMainDlg->m_EDIT_EMERALD_STAT = "미정의";
    }

    // BF609 Status
    bCAN_AAVST_BF609_Stat = bData = a_pCanMsg->arbData[6] & ~_BF_ERR_EMERALD_VR;
    if (bData == 0)
    {
        pMainDlg->m_EDIT_BF609_STAT = "정상";
    }
    else
    {
        pMainDlg->m_EDIT_BF609_STAT = "";
        if (bData & _BF_ERR_CAMERA)     pMainDlg->m_EDIT_BF609_STAT += "영상입력에러,";
        if (bData & _BF_ERR_EMERALD_ST) pMainDlg->m_EDIT_BF609_STAT += "에메랄드통신에러,";
        if (bData & _BF_ERR_CAN_HP2)    pMainDlg->m_EDIT_BF609_STAT += "HP2통신에러,";
        if (bData & _BF_ERR_CAN_HP3)    pMainDlg->m_EDIT_BF609_STAT += "HP3통신에러,";
    }

    // BF609 FPS
    bCAN_AAVST_BF609_FPS = bData = a_pCanMsg->arbData[7];
    pMainDlg->m_EDIT_BF609_FPS.Format("%d.%d", bData/10, bData%10);

    //////////////////////////////////////
    // CAN Message Data를 Window 화면에 출력한다.
    //////////////////////////////////////

    pMainDlg->PostMessage(_WM_THREAD_UPDATE);

    //////////////////////////////////////
    // CAN Message 수신 카운터를 갱신한다.
    //////////////////////////////////////
    wCAN_AAVST_GetCnt++;
}

U32 wCAN_CID_GetCnt;

U08 bCAN_CID_CC;
U08 bCAN_CID_MC;
CHR szCAN_CID_MDay[12];
CHR szCAN_CID_Ver[8];
CHR szCAN_CID_SrlNo[16];        // ex) "*": 일련번호 없음. "123456768*": 일련번호 "12345678"
CHR szCAN_CID_ModelName[16];    // ex) "AAVM8*": 8톤용 AAVM, "AAVM25*": 25톤용 AAVM, "AAVM128": 128톤용 AAVM

static void Analysis_CID(U08 *a_pMsgData, U16 a_uMsgLen)
{
    U08 bData;
    U08 *pbTgt;
    U16 uMsgIdx;
    CHR szData[32];

    // Component Code
    bData = a_pMsgData[0];
    bCAN_CID_CC = bData;

    // Manufacturer Code
    bData = a_pMsgData[1];
    bCAN_CID_MC = bData;

    // Manufacture Day
    sprintf_s(szCAN_CID_MDay, "%4d-%02d-%02d",
        a_pMsgData[2]+2000,
        a_pMsgData[3],
        a_pMsgData[4]);

    // SW Version
    bData = a_pMsgData[5];
    sprintf_s(szCAN_CID_Ver, "%d.%d", bData>>4, bData&0x0F);

    // Serial Number
    uMsgIdx = 6;
    pbTgt = (U08*)szCAN_CID_SrlNo;
    pbTgt[0] = 0;
    while (true)
    {
        bData = a_pMsgData[uMsgIdx++];
        if ( (bData == '*') || (bData < ' ') || (bData > '~') || (uMsgIdx > a_uMsgLen) ) // '*' or not ASCII
        {
            *pbTgt = 0;
            break;
        }
        else
        {
            *pbTgt++ = bData;
        }
    }

    // Model Name
    pbTgt = (U08*)szCAN_CID_ModelName;
    pbTgt[0] = 0;
    while (true)
    {
        bData = a_pMsgData[uMsgIdx++];
        if ( (bData == '*') || (bData < ' ') || (bData > '~') || (uMsgIdx > a_uMsgLen) ) // '*' or not ASCII
        {
            *pbTgt = 0;
            break;
        }
        else
        {
            *pbTgt++ = bData;
        }
    }

    // SW Sub Version
    bData = a_pMsgData[uMsgIdx];
    if ( (bData & 0xF0) != 0xF0 )
    {
        if ((bData & 0x0F) == 0x0F)
        {
            sprintf_s(szData, ".%d", (bData >> 4) & 0x0F);
        }
        else
        {
            sprintf_s(szData, ".%d.%d", (bData >> 4) & 0x0F, bData & 0x0F);
        }

        strcat_s(szCAN_CID_Ver, sizeof(szCAN_CID_Ver), szData);
    }

                 
    //////////////////////////////////////
    // CAN Message 수신 카운터를 갱신한다.
    //////////////////////////////////////
    wCAN_CID_GetCnt++;
}

void UseMsg_CID(CAN_MSG* a_pCanMsg)
{
    //////////////////////////////////////
    // CAN data를 변환한다.
    //////////////////////////////////////
    Analysis_CID(a_pCanMsg->arbData, 8);
}

///////////////////////////////////////
//  Version Data
///////////////////////////////////////
CHR szCAN_Ver_HW[8];
CHR szCAN_Ver_BF[8];
CHR szCAN_Ver_EL_Main[8];
CHR szCAN_Ver_EL_Sub [8];
CHR szCAN_Ver_Camera_Front[8];
CHR szCAN_Ver_Camera_Rear [8];
CHR szCAN_Ver_Camera_Left [8];
CHR szCAN_Ver_Camera_Right[8];

static void UseMsg_Version_CvtHex2Str(CHR *a_pszVer, U08 a_bVerMinor, U08 a_bVerMajor)
{
    if ( (a_bVerMajor == 0xFF) && (a_bVerMinor == 0xFF) )
    {
        strcpy_s(a_pszVer, 8, "미확정");
    }
    else
    if ( (a_bVerMajor == 0xFF) && (a_bVerMinor == 0xFE) )
    {
        strcpy_s(a_pszVer, 8, "없음");
    }
    else
    {
        wsprintf(a_pszVer, "%d.%d", a_bVerMajor, a_bVerMinor);
    }
}

U32 wCAN_VER1_GetCnt;
TOF tCAN_VER1_Complete;

void UseMsg_VER1(CAN_MSG* a_pCanMsg)
{
    U16 uVer;
    TOF tVerComplete = true;

    //////////////////////////////////////
    // CAN data를 변환한다.
    //////////////////////////////////////
    // Hardware Version
    UseMsg_Version_CvtHex2Str(szCAN_Ver_HW, a_pCanMsg->arbData[0], a_pCanMsg->arbData[1]);

    // BF609 Version
    UseMsg_Version_CvtHex2Str(szCAN_Ver_BF, a_pCanMsg->arbData[2], a_pCanMsg->arbData[3]);

    // Emerald-L Main Version
    UseMsg_Version_CvtHex2Str(szCAN_Ver_EL_Main, a_pCanMsg->arbData[4], a_pCanMsg->arbData[5]);
    uVer = ((U16)a_pCanMsg->arbData[5] << 8) + ((U16)a_pCanMsg->arbData[4] << 0);
    if (uVer == 0xFFFF)
    {
        tVerComplete = false;
    }

    // Emerald-L Sub Version
    UseMsg_Version_CvtHex2Str(szCAN_Ver_EL_Sub, a_pCanMsg->arbData[6], a_pCanMsg->arbData[7]);
    uVer = ((U16)a_pCanMsg->arbData[7] << 8) + ((U16)a_pCanMsg->arbData[6] << 0);
    if (uVer == 0xFFFF)
    {
        tVerComplete = false;
    }

    tCAN_VER1_Complete = tVerComplete;

    //////////////////////////////////////
    // CAN Message 수신 카운터를 갱신한다.
    //////////////////////////////////////
    wCAN_VER1_GetCnt++;
}

U32 wCAN_VER2_GetCnt;
TOF tCAN_VER2_Complete;

void UseMsg_VER2(CAN_MSG* a_pCanMsg)
{
    U16 uVer;
    TOF tVerComplete = true;

    //////////////////////////////////////
    // CAN data를 변환한다.
    //////////////////////////////////////
    // Camera Front Version
    UseMsg_Version_CvtHex2Str(szCAN_Ver_Camera_Front, a_pCanMsg->arbData[0], a_pCanMsg->arbData[1]);
    uVer = ((U16)a_pCanMsg->arbData[1] << 8) + ((U16)a_pCanMsg->arbData[0] << 0);
    if (uVer == 0xFFFF)
    {
        tVerComplete = false;
    }

    // Camera Rear Version
    UseMsg_Version_CvtHex2Str(szCAN_Ver_Camera_Rear , a_pCanMsg->arbData[2], a_pCanMsg->arbData[3]);
    uVer = ((U16)a_pCanMsg->arbData[3] << 8) + ((U16)a_pCanMsg->arbData[2] << 0);
    if (uVer == 0xFFFF)
    {
        tVerComplete = false;
    }

    // Camera Left Version
    UseMsg_Version_CvtHex2Str(szCAN_Ver_Camera_Left , a_pCanMsg->arbData[4], a_pCanMsg->arbData[5]);
    uVer = ((U16)a_pCanMsg->arbData[5] << 8) + ((U16)a_pCanMsg->arbData[4] << 0);
    if (uVer == 0xFFFF)
    {
        tVerComplete = false;
    }

    // Camera Right Version
    UseMsg_Version_CvtHex2Str(szCAN_Ver_Camera_Right, a_pCanMsg->arbData[6], a_pCanMsg->arbData[7]);
    uVer = ((U16)a_pCanMsg->arbData[7] << 8) + ((U16)a_pCanMsg->arbData[6] << 0);
    if (uVer == 0xFFFF)
    {
        tVerComplete = false;
    }

    tCAN_VER2_Complete = tVerComplete;

    //////////////////////////////////////
    // CAN Message 수신 카운터를 갱신한다.
    //////////////////////////////////////
    wCAN_VER2_GetCnt++;
}

const CHR *arpszModelName[22] =
{
    "---",		// 0x00
    "HW60",		//  1
    "HX140",	//  2
    "HW145",	//  3
    "HW140",	//  4
    "HX160",	//  5
    "HW160",	//  6
    "HX180",	//  7
    "HW180",	//  8
    "HW210",	//  9
    "HX220",	// 10
    "HX220",	// 11
    "HX235",	// 12
    "HX260",	// 13
    "HX300",	// 14
    "HX330",	// 15
    "HX380",	// 16
    "HX430",	// 17
    "HX480",	// 18
    "HX520",	// 19
    "HX900",	// 20
    "HXXXX",	// 21 Unknown Model
};

U32 wCAN_MLST_GetCnt;
const CHR *arszCAN_MLST[8];

void UseMsg_MLST(CAN_MSG* a_pCanMsg)
{
    U08 bData;

    //////////////////////////////////////
    // CAN data를 변환한다.
    //////////////////////////////////////
    bData = a_pCanMsg->arbData[0];
    if (bData == 0xFF)
    {
        arszCAN_MLST[0] = "없음";
    }
    else
    {
        arszCAN_MLST[0] = arpszModelName[bData];
    }

    bData = a_pCanMsg->arbData[1];
    if (bData == 0xFF)
    {
        arszCAN_MLST[1] = "없음";
    }
    else
    {
        arszCAN_MLST[1] = arpszModelName[bData];
    }

    bData = a_pCanMsg->arbData[2];
    if (bData == 0xFF)
    {
        arszCAN_MLST[2] = "없음";
    }
    else
    {
        arszCAN_MLST[2] = arpszModelName[bData];
    }

    bData = a_pCanMsg->arbData[3];
    if (bData == 0xFF)
    {
        arszCAN_MLST[3] = "없음";
    }
    else
    {
        arszCAN_MLST[3] = arpszModelName[bData];
    }

    bData = a_pCanMsg->arbData[4];
    if (bData == 0xFF)
    {
        arszCAN_MLST[4] = "없음";
    }
    else
    {
        arszCAN_MLST[4] = arpszModelName[bData];
    }

    bData = a_pCanMsg->arbData[5];
    if (bData == 0xFF)
    {
        arszCAN_MLST[5] = "없음";
    }
    else
    {
        arszCAN_MLST[5] = arpszModelName[bData];
    }

    //////////////////////////////////////
    // CAN Message 수신 카운터를 갱신한다.
    //////////////////////////////////////
    wCAN_MLST_GetCnt++;
}

TOF tTP_On = false;
U32 wTP_PGN;
U08 bTP_SA;
U08 bTP_DA;
U16 uTP_DataTotal;
U08 bTP_PackTotal;
U08 bTP_PackCnt;
U08 arbTP_Buffer[1785]; // = 255packets*7bytes

void UseMsg_TPCM_BAM(CAN_MSG* a_pCanMsg)
{
    if (a_pCanMsg->arbData[0] != 32)    // Check BAM
    {
        tTP_On = false;
        return;
    }

    bTP_SA = (U08)a_pCanMsg->wId;
    bTP_DA = (U08)(a_pCanMsg->wId >> 8);

    uTP_DataTotal = ((U16)a_pCanMsg->arbData[1]) + (((U16)a_pCanMsg->arbData[2]) << 8);
    bTP_PackTotal = a_pCanMsg->arbData[3];
    wTP_PGN = ((U32)a_pCanMsg->arbData[5]) + (((U16)a_pCanMsg->arbData[6]) << 8) + (((U16)a_pCanMsg->arbData[7]) << 16);

    // Ready TPDT
    tTP_On = true;
    bTP_PackCnt = 0;
}


void UseMsg_TPDT_BAM(CAN_MSG* a_pCanMsg)
{
    U08 bSA, bDA;
    U08 *pbTgt, *pbSrc;

    if (tTP_On == false) return;

    bSA = (U08)a_pCanMsg->wId;
    bDA = (U08)(a_pCanMsg->wId >> 8);

    // Check DA, SA
    if (bSA != bTP_SA) return;
    if (bDA != bTP_DA) return;

    // Get Current Packet Number
    bTP_PackCnt = a_pCanMsg->arbData[0];
    
    // Copy Data
    pbSrc = &a_pCanMsg->arbData[1];
    pbTgt = &arbTP_Buffer[(bTP_PackCnt-1)*7];
    for (U08 i = 0; i < 7; i++)
    {
        *pbTgt++ = *pbSrc++;
    }

    // Check End of Transmit
    if (bTP_PackCnt == bTP_PackTotal)
    {
        tTP_On = false;

        if (wTP_PGN == _CAN_PGN_CID)
        {
            Analysis_CID(arbTP_Buffer, uTP_DataTotal);
        }
    }
}


void MakeMsg_REQ(CAN_MSG* a_pCanMsg, U32 a_wPGN)
{
    a_pCanMsg->wId = (6<<_BITP_J1939_ID_PRIO)+((_CAN_PGN_REQ+_CAN_ADDR_AAVM)<<8)+_CAN_ADDR_EOL;
    a_pCanMsg->bLen = 3;

    a_pCanMsg->arbData[0] = (U08)a_wPGN;
    a_pCanMsg->arbData[1] = (U08)(a_wPGN >> 8);
    a_pCanMsg->arbData[2] = (U08)(a_wPGN >>16);
    a_pCanMsg->arbData[3] = 0;
    a_pCanMsg->arbData[4] = 0;
    a_pCanMsg->arbData[5] = 0;
    a_pCanMsg->arbData[6] = 0;
    a_pCanMsg->arbData[7] = 0;
}

void MakeMsg_REQ_CID(CAN_MSG* a_pCanMsg)
{
    MakeMsg_REQ(a_pCanMsg, _CAN_PGN_CID);
}

void MakeMsg_AAVCS(CAN_MSG* a_pCanMsg, U08 a_bViewMode, U08 a_bSwitch)
{
    a_pCanMsg->wId = (6<<_BITP_J1939_ID_PRIO)+((_CAN_PGN_AAVCS+_CAN_ADDR_AAVM)<<8)+_CAN_ADDR_MONITOR;
    a_pCanMsg->bLen = 8;

    a_pCanMsg->arbData[0] = 232;
    a_pCanMsg->arbData[1] = ((a_bSwitch & 0x03) << 4) | (a_bViewMode & 0x0F);
    a_pCanMsg->arbData[2] = 0;
    a_pCanMsg->arbData[3] = 0;
    a_pCanMsg->arbData[4] = 0;
    a_pCanMsg->arbData[5] = 0;
    a_pCanMsg->arbData[6] = 0;
    a_pCanMsg->arbData[7] = 0;
}


void MakeMsg_REQ_VER1(CAN_MSG* a_pCanMsg)
{
    MakeMsg_REQ(a_pCanMsg, _CAN_PGN_VER1);
}

void MakeMsg_REQ_VER2(CAN_MSG* a_pCanMsg)
{
    MakeMsg_REQ(a_pCanMsg, _CAN_PGN_VER2);
}

void MakeMsg_REQ_MLST(CAN_MSG* a_pCanMsg)
{
    MakeMsg_REQ(a_pCanMsg, _CAN_PGN_MLST);
}

void MakeMsg_REQ_AAVM(CAN_MSG* a_pCanMsg)
{
    MakeMsg_REQ(a_pCanMsg, _CAN_PGN_AAVM);
}

void MakeMsg_REQ_AAVST(CAN_MSG* a_pCanMsg)
{
    MakeMsg_REQ(a_pCanMsg, _CAN_PGN_AAVST);
}

void MakeMsg_REQ_AAVM_Auto(CAN_MSG* a_pCanMsg)
{
    a_pCanMsg->bLen = 3;

    a_pCanMsg->arbData[0] = (U08)_CAN_PGN_AAVM;
    a_pCanMsg->arbData[1] = (U08)(_CAN_PGN_AAVM >> 8);
    a_pCanMsg->arbData[2] = (U08)(_CAN_PGN_AAVM >>16);
    a_pCanMsg->arbData[3] = 0;
    a_pCanMsg->arbData[4] = 0;
    a_pCanMsg->arbData[5] = 0;
    a_pCanMsg->arbData[6] = 0;
    a_pCanMsg->arbData[7] = 0;
}

void MakeMsg_REQ_AAVST_Auto(CAN_MSG* a_pCanMsg)
{
    a_pCanMsg->bLen = 3;

    a_pCanMsg->arbData[0] = (U08)_CAN_PGN_AAVST;
    a_pCanMsg->arbData[1] = (U08)(_CAN_PGN_AAVST >> 8);
    a_pCanMsg->arbData[2] = (U08)(_CAN_PGN_AAVST >>16);
    a_pCanMsg->arbData[3] = 0;
    a_pCanMsg->arbData[4] = 0;
    a_pCanMsg->arbData[5] = 0;
    a_pCanMsg->arbData[6] = 0;
    a_pCanMsg->arbData[7] = 0;
}
