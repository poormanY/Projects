// can_msg.h: CAN In/Out Functions. Kim JinWoo, 2014-08-06.

#ifndef _CAN_MSG_H_
#define _CAN_MSG_H_

#define _CAN_ADDR_AAVM      0xDE
#define _CAN_ADDR_MONITOR   0x28
#define _CAN_ADDR_ENGINE    0x02
#define _CAN_ADDR_SWITCH    0xDC
#define _CAN_ADDR_EOL       0xFF
#define _CAN_ADDR_EOL2      0xFE

#define _CAN_PGN_AAVO   0xFFF8
#define _CAN_PGN_CID    0xFF32
#define _CAN_PGN_MMS    0xFF46
#define _CAN_PGN_HP1    0xFF47
#define _CAN_PGN_HP3    0xFF49
#define _CAN_PGN_ESROS  0xFFF7
#define _CAN_PGN_AAVCS  0xEF00

#define _CAN_PGN_DM1    0xFECA
#define _CAN_PGN_DM2    0xFECB
#define _CAN_PGN_DM3    0xFECC
#define _CAN_PGN_DM5    0xFECE

#define _CAN_PGN_VER1   0xFFE0
#define _CAN_PGN_VER2   0xFFE1
#define _CAN_PGN_AAVM   0xFFE2
#define _CAN_PGN_MLST   0xFFE3
#define _CAN_PGN_AAVST  0xFFE7

#define _CAN_PGN_ACK    0xE800
#define _CAN_PGN_REQ    0xEA00
#define _CAN_PGN_TPDT   0xEB00
#define _CAN_PGN_TPCM   0xEC00

#define _BITP_J1939_ID_PRIO     26  // Priority
#define _BITP_J1939_ID_EDP      25  // Extended Data Page
#define _BITP_J1939_ID_DP       24  // Data Page
#define _BITP_J1939_ID_PF       16  // PDU Format
#define _BITP_J1939_ID_PS        8  // PDU Specific
#define _BITP_J1939_ID_PDU       8  // Protocol Data Unit
#define _BITP_J1939_ID_PGN       8  // Parameter Group Number
#define _BITP_J1939_ID_DA        8  // Destination Address
#define _BITP_J1939_ID_SA        0  // Source Address


#define _CAN_ID_AAVCS   (((_CAN_PGN_AAVCS+_CAN_ADDR_AAVM)<<8)+_CAN_ADDR_MONITOR)
#define _CAN_ID_REQ     ((_CAN_PGN_REQ+_CAN_ADDR_AAVM)<<8)
#define _CAN_ID_MMS     ((_CAN_PGN_MMS<<8)+_CAN_ADDR_ENGINE)
#define _CAN_ID_HP1     ((_CAN_PGN_HP1<<8)+_CAN_ADDR_ENGINE)
#define _CAN_ID_HP3     ((_CAN_PGN_HP3<<8)+_CAN_ADDR_ENGINE)
#define _CAN_ID_ESROS   ((_CAN_PGN_ESROS<<8)+_CAN_ADDR_SWITCH)


void MakeMsg_ESROS(CAN_MSG* a_pCanMsg);
void MakeMsg_MMS  (CAN_MSG* a_pCanMsg);
void MakeMsg_HP1  (CAN_MSG* a_pCanMsg);
void MakeMsg_HP3  (CAN_MSG* a_pCanMsg);

extern U32 wCAN_AAVO_GetCnt;
extern const char *arszAAVO_ViewMode[16];
extern const char *arszAAVO_Warning[4];
void UseMsg_AAVO(CAN_MSG* a_pCanMsg);

extern U32 wCAN_AAVM_GetCnt;
void UseMsg_AAVM(CAN_MSG* a_pCanMsg);

extern U32 wCAN_AAVST_GetCnt;
extern const char *arszAAVST_CameraStatus[4];
extern const char *arszAAVST_EmeraldStatus[16];
extern U08 bCAN_AAVST_CameraGain_Front;
extern U08 bCAN_AAVST_CameraGain_Rear;
extern U08 bCAN_AAVST_CameraGain_Left;
extern U08 bCAN_AAVST_CameraGain_Right;
extern U08 bCAN_AAVST_BF609_Stat;
extern U08 bCAN_AAVST_BF609_FPS;
#define _BF_ERR_CAMERA     0x01
#define _BF_ERR_EMERALD_ST 0x02
#define _BF_ERR_EMERALD_VR 0x04
#define _BF_ERR_CAN_HP2    0x08
#define _BF_ERR_CAN_HP3    0x10
#define _BF_ERR_CAN_ESROS  0x20
void UseMsg_AAVST(CAN_MSG* a_pCanMsg);

extern U32 wCAN_CID_GetCnt;
extern U08 bCAN_CID_CC;
extern U08 bCAN_CID_MC;
extern CHR szCAN_CID_MDay[12];
extern CHR szCAN_CID_Ver[8];
extern CHR szCAN_CID_SrlNo[16];        // ex) "*": ÀÏ·Ã¹øÈ£ ¾øÀ½. "123456768*": ÀÏ·Ã¹øÈ£ "12345678"
extern CHR szCAN_CID_ModelName[16];    // ex) "AAVM8*": 8Åæ¿ë AAVM, "AAVM25*": 25Åæ¿ë AAVM, "AAVM128": 128Åæ¿ë AAVM
void UseMsg_CID(CAN_MSG* a_pCanMsg);

extern CHR szCAN_Ver_HW[8];
extern CHR szCAN_Ver_BF[8];
extern CHR szCAN_Ver_EL_Main[8];
extern CHR szCAN_Ver_EL_Sub [8];
extern CHR szCAN_Ver_Camera_Front[8];
extern CHR szCAN_Ver_Camera_Rear [8];
extern CHR szCAN_Ver_Camera_Left [8];
extern CHR szCAN_Ver_Camera_Right[8];

extern U32 wCAN_VER1_GetCnt;
extern TOF tCAN_VER1_Complete;
void UseMsg_VER1(CAN_MSG* a_pCanMsg);

extern U32 wCAN_VER2_GetCnt;
extern TOF tCAN_VER2_Complete;
void UseMsg_VER2(CAN_MSG* a_pCanMsg);

extern const CHR *arpszModelName[22];
extern U32 wCAN_MLST_GetCnt;
extern const CHR *arszCAN_MLST[8];

void UseMsg_MLST(CAN_MSG* a_pCanMsg);

void UseMsg_TPCM_BAM(CAN_MSG* a_pCanMsg);
void UseMsg_TPDT_BAM(CAN_MSG* a_pCanMsg);

void MakeMsg_AAVCS(CAN_MSG* a_pCanMsg, U08 a_bViewMode, U08 a_bSwitch);

void MakeMsg_REQ(CAN_MSG* a_pCanMsg, U32 a_wPGN);
void MakeMsg_REQ_CID(CAN_MSG* a_pCanMsg);
void MakeMsg_REQ_VER1(CAN_MSG* a_pCanMsg);
void MakeMsg_REQ_VER2(CAN_MSG* a_pCanMsg);
void MakeMsg_REQ_MLST(CAN_MSG* a_pCanMsg);
void MakeMsg_REQ_AAVM(CAN_MSG* a_pCanMsg);
void MakeMsg_REQ_AAVST(CAN_MSG* a_pCanMsg);

void MakeMsg_REQ_AAVM_Auto(CAN_MSG* a_pCanMsg);
void MakeMsg_REQ_AAVST_Auto(CAN_MSG* a_pCanMsg);

#endif  // _CAN_MSG_H_

