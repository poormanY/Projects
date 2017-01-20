// JTP_Com.h: interface for the JTP_Com class.
//
//////////////////////////////////////////////////////////////////////
#ifndef Y_JTP_COM_H
#define Y_JTP_COM_H

#include "y_JDEFINE.h"
/*****************************************
***	  VARIABLE TYPE DEFINITION		 ***
*****************************************/
/*
#define S08	signed char
#define U08 unsigned char
#define S16	signed short int
#define U16 unsigned short int
#define S32	signed long
#define U32 unsigned long
#define S64	signed long long
#define U64 unsigned long long
*/
/****************************************/




////////////////////// Common Base Commands: 0x0000 ~ 0x0010 ///////////////////////////////////
#define _JTP_CMD_BOOT						0x0000   // Boot 완료 통보 (*주1)
#define _JTP_CMD_CONNECT					0x0001   // 연결 확인
#define _JTP_CMD_BUFFSIZE					0x0004   // Receivable Data Size

#define _JTP_CMD_DEV_INFO_GET				0x0100   // Get Device Info (*주2)
#define _JTP_CMD_MFT_CFG_GET				0x0110   // Get Manufacturer Config
#define _JTP_CMD_MFT_CFG_SET				0x0112   // Set Manufacturer Config

#define _JTP_CMD_USER_CFG_GET				0x0114   // Get User Config
#define _JTP_CMD_USER_CFG_SET				0x0116   // Set User Config
#define _JTP_CMD_USER_RESET					0x0118   // Init for User Config

#define _JTP_CMD_MODE_SET					0x0120   // Set Run-Time Mode (*주3)
#define _JTP_CMD_BBOX_ENABLE				0x0122   // Enable Blackbox (*주4)
#define _JTP_CMD_MAIN_CODE_GET				0x0124   // Get Main Code Addr
#define _JTP_CMD_HW_TEST					0x0130   // Run H/W Test (*주5)
#define _JTP_CMD_UNINSTALLED_SET			0x0140   // Set Uninstalled State

////////////////////////////////////////////////////////////////////////////////

#define _JTP_CMD_NOR_INFO					0x1100   // NOR Flash Info (*주6)
#define _JTP_CMD_NOR_READ					0x1102   // NOR Flash Read
#define _JTP_CMD_NOR_WRITE					0x1104   // NOR Flash Write
#define _JTP_CMD_NOR_ERASE					0x1106   // NOR Flash Erase

#define _JTP_CMD_SPI_INFO					0x1110   // SPI Flash Info (*주6)
#define _JTP_CMD_SPI_READ					0x1112   // SPI Flash Read
#define _JTP_CMD_SPI_WRITE					0x1114   // SPI Flash Write
#define _JTP_CMD_SPI_ERASE					0x1116   // SPI Flash Erase

#define _JTP_CMD_NAND_INFO					0x1200   // NAND Flash Info (*주7)
#define _JTP_CMD_NAND_READ					0x1202   // NAND Flash Read(SW)
#define _JTP_CMD_NAND_WRITE					0x1204   // NAND Flash Write(SW)
#define _JTP_CMD_NAND_ERASE					0x1206   // NAND Flash Erase(SW)
#define _JTP_CMD_NAND_FORMAT				0x1208   // NAND Flash Format(SW)
#define _JTP_CMD_NAND_HW_READ				0x120A   // NAND Flash Read(HW)
#define _JTP_CMD_NAND_HW_WRITE				0x120C   // NAND Flash Write(HW)
#define _JTP_CMD_NAND_HW_ERASE				0x120E   // NAND Flash Erase(HW)

#define _JTP_CMD_RAM_READ					0x1402   // RAM Read
#define _JTP_CMD_RAM_WRITE					0x1404   // RAM Write

#define _JTP_CMD_I2C_READ					0x1802   // I2C(8bits) Read
#define _JTP_CMD_I2C_WRITE					0x1804   // I2C(8bits) Write

#define _JTP_CMD_SAY2						0x1C00   // U32 Say2(S32*, U32)
#define _JTP_CMD_SND_PUT					0x1C40   // void SndPut(U16, bool)
#define _JTP_CMD_SND_GAP					0x1C42   // void SndGap(U16, bool)
#define _JTP_CMD_SND_STOP					0x1C80   // void SndStop()

//////////////////////////////////////////////////////////////////////////////

#define _JTP_CMD_LED_PUT					0x1E00  // LED Put

#define _JTP_CMD_OLED_PUT					0x1F00  // OLED Put
#define _JTP_CMD_OLED_PUT_CMD				0x1FF0  // OLED Put Cmd
#define _JTP_CMD_OLED_GET_RGB				0x1FF2  // OLED Get RGB
#define _JTP_CMD_OLED_PUT_RGB				0x1FF4  // OLED Put RGB

//////////////////////////////////////////////////////////////////////////////

#define _JTP_CMD_BBOX_SEND_HEADERS			0x2000  // Get Blackbox Headers at Send Mode
#define _JTP_CMD_BBOX_SEND_SELECT			0x2001  // Select Blackbox Block at Send Mode
#define _JTP_CMD_BBOX_SEND_DATA				0x2002  // Get Next Blackbox Data at Send Mode
#define _JTP_CMD_BBOX_CHECK					0x2008  // Get no. of Saved Blackbox

#define _JTP_CMD_SSHOT_SEND_HEADERS			0x2010  // Get Blackbox Headers at Send Mode
#define _JTP_CMD_SSHOT_SEND_DATA			0x2012  // Get Next Blackbox Data at Send Mode
#define _JTP_CMD_SSHOT_CHECK				0x2018  // Get no. of Saved Blackbox

#define _JTP_CMD_RTC_GET					0x2100  // Get RTC Value
#define _JTP_CMD_RTC_SET					0x2101  // Set RTC Value

#define _JTP_CMD_ACC_RESET					0x2110  // Reset ACC

////////////////////////////////////////////////////////////////////////////

#define _JTP_CMD_USER_LDWS_ON_GET			0x4000  // User Get LDWS On/Off
#define _JTP_CMD_USER_LDWS_ON_SET			0x4001  // User Set LDWS On/Off
#define _JTP_CMD_USER_LKAS_ON_GET			0x4002  // User Get LKAS On/Off
#define _JTP_CMD_USER_LKAS_ON_SET			0x4003  // User Set LKAS On/Off
#define _JTP_CMD_USER_G2G_ON_GET			0x4004  // User Get G2G On/Off
#define _JTP_CMD_USER_G2G_ON_SET			0x4005  // User Set G2G On/Off
#define _JTP_CMD_USER_MUTE_ON_GET			0x4006  // User Get Mute On/Off
#define _JTP_CMD_USER_MUTE_ON_SET			0x4007  // User Set Mute On/Off

#define _JTP_CMD_USER_VOL_GET				0x4100  // User Get Volume
#define _JTP_CMD_USER_VOL_SET				0x4101  // User Set Volume

#define _JTP_CMD_USER_RUN_MODE_GET			0x4102  // User Get Run Mode
#define _JTP_CMD_USER_RUN_MODE_SET			0x4103  // User Set Run Mode

#define _JTP_CMD_USER_CAR_TYPE_GET			0x4200  // User Get Car Type
#define _JTP_CMD_USER_CAR_TYPE_SET			0x4201  // User Set Car Type
#define _JTP_CMD_USER_LDWS_LLVL_GET			0x4202  // User Get LDWS L-Level
#define _JTP_CMD_USER_LDWS_LLVL_SET			0x4203  // User Set LDWS L-Level
#define _JTP_CMD_USER_LDWS_RLVL_GET			0x4204  // User Get LDWS R-Level
#define _JTP_CMD_USER_LDWS_RLVL_SET			0x4205  // User Set LDWS R-Level
#define _JTP_CMD_USER_LDWS_WARN_GET			0x4210  // User Get LDWS Warning
#define _JTP_CMD_USER_LDWS_WARN_SET			0x4211  // User Set LDWS Warning

#define _JTP_CMD_USER_BBOX_SAVE				0x4300  // User Save Blackbox Data
#define _JTP_CMD_USER_BBOX_DEL				0x4302  // User Delete Blackbox Data

#define _JTP_CMD_USER_SSHOT_SAVE			0x4310  // User Save Snap-Shot
#define _JTP_CMD_USER_SSHOT_DEL				0x4312  // User Delete Snap-Shot

////////////////////////////////////////////////////////////////////

#define _JTP_CMD_MONITOR_VIDEO				0x7000  // Get Video Monitor
#define _JTP_CMD_MONITOR_DATA				0x7002  // Get Data Monitor

////////////////////////////////////////////////////////////////////

#define _JTP_CMD_DRIVE_STATE_GET			0x7004  // Get Drive State
#define _JTP_CMD_DOWNLOAD_FIRMWARE			0x700A  // Download firmware
#define _JTP_CMD_CTRL_VIBRATOR				0x700B  // Control Vibrator
#define _JTP_CMD_CTRL_HIGHBEAM				0x700C  // Control High beam
#define _JTP_CMD_GPS_DATA_GET				0x700D  // Get GPS Data
#define _JTP_CMD_SEND_VIRTUAL_SPEED			0x7010  // Send Virtual Speed
#define _JTP_CMD_SEND_VIRTUAL_SIGNAL		0x7012  // Send Virtual Signal
#define _JTP_CMD_BLUETOOTH_INFO_GET			0x7014  // Get Bluetooth Info

////////////////////////////////////////////////////////////////////
#define _JTP_CMD_DISPLAY_M_INFO_GET			0x7016  // Get ADAS Info
#define _JTP_CMD_ADAS_INFO_GET				0x7018  // Get ADAS Info

////////////////////////////////////////////////////////////////////

#define _JTP_CMD_MCU_GET					0x7020  // Get Data from MCU

////////////////////////////////////////////////////////////////////
// for MA Micom
#define _JTP_CMD_MA_GET_DATA				0x7021  // Set MA LIN information to BF
#define _JTP_CMD_MA_SEND_DATA				0x7022  // Set BF Alert to MA Display
#define _JTP_CMD_MA_CONFIG_GET				0x7023  // Get BF Current Config
#define _JTP_CMD_MA_CONFIG_SET				0x7024  // Set MA Config to BF
#define _JTP_CMD_MA_SEND_VER				0x7025  // Get BF Version Info
#define _JTP_CMD_MA_UPDATE_READY			0x7026  // DSP Upgrade Ready
#define _JTP_CMD_MA_UPDATE_RUN				0x7027  // DSP Upgrade Bin
#define _JTP_CMD_MA_UPDATE_END				0x7028  // DSP Upgrade End
#define _JTP_CMD_MA_FACTORY_START			0x7029  // DSP Factory Mode Start
#define _JTP_CMD_MA_FACTORY_END				0x702A  // DSP Factory Mode End
#define _JTP_CMD_MA_INSTALL_START			0x702B  // DSP Install Mode Start
#define _JTP_CMD_MA_INSTALL_STATUS			0x702C  // DSP Install Mode Status
#define _JTP_CMD_MA_INSTALL_END				0x702D  // DSP Install Mode End
#define _JTP_CMD_MA_SETUP_READ				0x702E  // MA Setup Mode Read
#define _JTP_CMD_MA_SETUP_WRITE				0x702F  // MA Setup Mode Write
/////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
#define _JTP_CMD_GET_SAVED_ADAS_DATA	0x7048  // Get Saved ADAS Data
#define _JTP_CMD_GET_DTC_DATA			0x704A  // Get DTC Data


#define JTP_pData(pJTP_FRAME) (&(((JTP_FRAME*)(pJTP_FRAME))->uChkSum))

#define JTP_HEAD_VALUE  0xAAF0	// 0xF0 0xAA
#define JTP_FOOT_VALUE  0x55F0	// 0xF0 0x55

#define JTP_ID_BF ( ('B'<<0) | ('F'<<8) )
#define JTP_ID_PC ( ('P'<<0) | ('C'<<8) )
#define JTP_ID_AL ( ('A'<<0) | ('L'<<8) )
#define JTP_ID_MA ( ('M'<<0) | ('A'<<8) )
#define JTP_ID_HUB ( ('H'<<0) | ('U'<<8) )

#define JTP_CMD_NO_CHKSUM 0x8000	// if (uCmd & JTP_CMD_NO_CHKSUM) && (uDataLen != 0) -> uChkSum = uDataLen

#define JTP_DATALEN_MAX (32*1024-16)

#define _MK_ALIGN_2(lData) (((U32)(lData)+1)&0xFFFFFFFE)
#define _MK_ALIGN_4(lData) (((U32)(lData)+3)&0xFFFFFFFC)

////////////////////// Response or Received State //////////////////////////////////////////////
#define _JTP_RESP_OK				 0x0000	// No Error		  All OK				: 모두 정상적으로 처리됨

#define _JTP_RESP_TRANS_TIMEOUT  0xFFFF	// Transmit Error::Time out		 : 데이터 수신중 0.1초 동안 데이터 수신이 없었음
#define _JTP_RESP_TRANS_TOO_BIG  0xFFFE	// Transmit Error::Too Big Data	: 전송되는 데이터의 크기가 너무 커 처리할 수 없음
#define _JTP_RESP_TRANS_CHKSUM	0xFFFD	// Transmit Error::Invalid ChkSum : 체크섬 에러
#define _JTP_RESP_TRANS_MSGEND	0xFFFC	// Transmit Error::Invalid MsgEnd : 메시지 종료값이 맞지 않음
#define _JTP_RESP_TRANS_ERR		0xFFF0	// Transmit Error::Others			: 기타 전송 에러(0xFFF0~0xFFFB)

#define _JTP_RESP_CMD_RSV		  0xFFEF	// Command Error::Reserved Cmd : 아직 지원되지 않은 명령어임
#define _JTP_RESP_CMD_BAD		  0xFFEE	// Command Error::Unknown Cmd  : 잘못된 명령어임
#define _JTP_RESP_CMD_ERR		  0xFFE0	// Command Error::Others		 : 기타 명령어 에러(0xFFE0~0xFFED)

#define _JTP_RESP_BAD_PARA		 0xFFD1	// Parameter Error:: Invalid Parameter : 명령어에 따른 파마메터 에러
#define _JTP_RESP_BAD_DATALEN	 0xFFD2	// Parameter Error:: DataLen			  : 명령어에 따른 데이터 크기 에러
#define _JTP_RESP_BAD_DATA		 0xFFD4	// Parameter Error:: Data				  : 잘못된 데이터

#define _JTP_RESP_ERROR			 0xFFD0	// Run Time Error(0xFFD0~0xFFFF)

#define _JTP_RESP_WARN			  0x0001	// Run Time Warning:: 명령어에 따라 다르게 해석됨. (0x0001~0xFFCF)


/////////////////// Common Data Format ///////////////////////
typedef struct JTP_FRAME_Tag
{
	U16  uHead;		 // Always 0xF0 0xAA = 0xAAF0

	U16  uTgtId;
	U16  uSrcId;

	U16  uCmd;
	U16  uPara;

	U16  uDataLen;	 // 0x0000 ~ 0x7FE0: Valid Data Length

// U08  arbData[((lDataLen+1)>>1)<<1]; // 0 ~ (32KB-16B)

	U16  uChkSum;
	U16  uFoot;		 // Always 0xF0 0x55 = 0x55F0
} JTP_FRAME;


typedef struct FW_UPDATE_tag
{
	S08	bFlag;
	S08	bIndex;
	S08 arbFilePath[MAX_PATH];
	U32 nFWFileSize;
	U32 nTotalRecvSize;
	U32 nTotalSendSize;
	U32 nPartSendSize;
} FW_FILE_INFO;


class JTP_Com
{
public:
	JTP_Com();
	virtual ~JTP_Com();

	BOOL OpenCom(CString a_pszCom);
	BOOL CloseCom();

	 void CvtRespMessage(CString &a_strMsg, int a_uResp);

protected:
	 int TransferMsg(JTP_FRAME *a_pTx, int a_uWaitMs);

public:
	JTP_FRAME *JTP_TxMakeBaseFrame(void *a_pTxBuff, U16 a_uDataLen);
	JTP_FRAME *JTP_TxMakeFullFrame(void *a_pTxBuff, U16 a_uDataLen);
	void JTP_TxMakeMsg(JTP_FRAME *a_pFrame, U16 a_uTgtId, U16 a_uSrcId, U16 a_uCmd, U16 a_uPara);
	void JTP_TxMakeChkSumBaseFrame(JTP_FRAME *a_pFrame, const void *a_pData);
	U16 JTP_CalcChkSumBaseFrame(JTP_FRAME *a_pFrame, const void *a_pData);
	void JTP_TxMakeChkSumFullFrame(JTP_FRAME *a_pFrame);
	U16 JTP_CalcChkSumFullFrame(JTP_FRAME *a_pFrame);
	U16 JTP_GetSizeFullFrame(const JTP_FRAME *a_pFrame);
	BOOL JTP_ChkAcceptId(U16 a_uRxTgtId);
	U16 JTP_ReadChkSum(JTP_FRAME *a_pFrame);
	U16 JTP_ReadFoot (JTP_FRAME *a_pFrame);


	BOOL SendMsg_GetGPSInfo(void *a_pData);
	BOOL SendMsg_GetBTInfo(void *a_pData);
	BOOL SendMsg_MainDataGet(void *a_pCfgData, int *a_puDataLen);
	BOOL SendMsg_SendVirtualSpeed(int a_uSpeed);
	BOOL SendMsg_SendVirtualSignal(const void *a_pCfgData, int a_uDataLen);		 // Set Virtual Signal
	BOOL SendMsg_ControlHighBeam(int a_uOnOff);
	BOOL SendMsg_ControlVibrator(int a_uOnOff);
	BOOL AutoOpenCom();
	int  GetOpenComNum(void);
	/////////////// Commands ////////////////////
	BOOL SendMsg_Connect(int uWaitDelay);				//* 연결 확인
	BOOL SendMsg_Buffsize();											//* Receivable Data Size

	BOOL SendMsg_DevInfoGet(void *a_pSysInfoData, int *a_puDataLen);	//* Get Device Info (*주2)
	BOOL SendMsg_GetMAVersion(int a_uMode, void *a_pData);		//* Get MCU Version Info

	BOOL SendMsg_MftCfgGet (void		 *a_pCfgData, int *a_puDataLen);  //* Get Manufacturer Config
	BOOL SendMsg_MftCfgSet (const void *a_pCfgData, int  a_uDataLen);	//* Set Manufacturer Config

	BOOL SendMsg_UserCfgGet(void		 *a_pCfgData, int *a_puDataLen);  //* Get User Config
	BOOL SendMsg_UserCfgSet(const void *a_pCfgData, int  a_uDataLen);	//* Set User Config
	BOOL SendMsg_UserReset();											// Init for User

	BOOL SendMsg_UserFwUpdateStart  (int  a_uDataLen, int nPara); //* User Firmware Update Start
	BOOL SendMsg_UserFwUpdateData	(void *a_pData,	int a_uDataLen, int a_uIndex); //* User Firmware Update Data send
	BOOL SendMsg_UserFwUpdateEnd	 (int nPara); //* User Firmware Update End
	BOOL SendMsg_UserFwUpdateRequest(int nPara); //* User Firmware Update Request

	BOOL SendMsg_ModeSet(int a_uNewMode, int *a_puCurMode);				//* Set Run-Time Mode (*주3)

	BOOL SendMsg_BboxEnable(BOOL a_tOn);								//* Enable Blackbox (*주4)
	BOOL SendMsg_MainCodeGet(int *a_puAddr);							//* Get Main Code Addr
	BOOL SendMsg_HwTest(U32 *a_puHwTestReport);							//* Run H/W Test (*주5)
	BOOL SendMsg_UninstalledSet();										// Set Uninstalled State

	BOOL SendMsg_NorInfoName	  (CString &a_strDeviceName);			//* NOR Flash Info (*주6)
	BOOL SendMsg_NorInfoID		 (int *a_uID);
	BOOL SendMsg_NorInfoSize	  (int *a_uSize);
	BOOL SendMsg_NorInfoNoOfSects(int *a_uNoOfSects);
	BOOL SendMsg_NorInfoSect	  (int a_uSectNum, int *a_uStartAddr, int *a_uEndAddr);

	BOOL SendMsg_NorRead (int a_uAddr, void *a_pData, int a_uLen);		//* NOR Flash Read
	BOOL SendMsg_NorWrite(int a_uAddr, void *a_pData, int a_uLen);		//* NOR Flash Write
	BOOL SendMsg_NorErase(int a_uAddr);									//* NOR Flash Erase

	BOOL SendMsg_SpifInfoName	  (CString &a_strDeviceName);			//* SPI Flash Info (*주6)
	BOOL SendMsg_SpifInfoID		 (int *a_uID);
	BOOL SendMsg_SpifInfoSize	  (int *a_uSize);
	BOOL SendMsg_SpifInfoNoOfSects(int *a_uNoOfSects);
	BOOL SendMsg_SpifInfoSect	  (int a_uSectNum, int *a_uStartAddr, int *a_uEndAddr);

	BOOL SendMsg_SpifRead (int a_uAddr, void *a_pData, int a_uLen);		//* SPI Flash Read
	BOOL SendMsg_SpifWrite(int a_uAddr, void *a_pData, int a_uLen);		//* SPI Flash Write
	BOOL SendMsg_SpifErase(int a_uAddr);								//* SPI Flash Erase

	BOOL SendMsg_NandInfoName		  (CString &a_strDeviceName);		// NAND Flash Info (*주7)
	BOOL SendMsg_NandInfoID			 (int *a_lID);
	BOOL SendMsg_NandInfoSize		  (int *a_lSize);
	BOOL SendMsg_NandInfoNoOfBlocks  (int *a_uNoOfBlocks);
	BOOL SendMsg_NandInfoPagesOfBlock(int *a_uPagesOfBlock);
	BOOL SendMsg_NandInfoBytesOfPage (int *a_uBytesOfPage);

	BOOL SendMsg_NandRead	(int a_lAddr, void *a_pData, int a_uLen);				 // NAND Flash Read(SW)
	BOOL SendMsg_NandWrite  (int a_lAddr, void *a_pData, int a_uLen);				 // NAND Flash Write(SW)
	BOOL SendMsg_NandErase  (int a_lAddr);													 // NAND Flash Erase(SW)
	BOOL SendMsg_NandFormat (int a_nFormatType);											 // NAND Flash Format(SW)
	BOOL SendMsg_NandHwRead (int a_uSeg, int a_uOff, void *a_pData, int a_uLen);  // NAND Flash Read(HW)
	BOOL SendMsg_NandHwWrite(int a_uSeg, int a_uOff, void *a_pData, int a_uLen);  // NAND Flash Write(HW)
	BOOL SendMsg_NandHwErase(int a_uSeg);													  // NAND Flash Erase(HW)

	BOOL SendMsg_RamRead();			// RAM Read
	BOOL SendMsg_RamWrite();		  // RAM Write

	BOOL SendMsg_I2cRead (int a_bDevID, int a_bStartReg, void *a_pData, int a_bLen);	// I2C(8bits) Read
	BOOL SendMsg_I2cWrite(int a_bDevID, int a_bStartReg, void *a_pData, int a_bLen);	// I2C(8bits) Write

	BOOL SendMsg_Say2(int *a_arData, int a_nDataLen, BOOL a_tNew);	//* U32 Say2(S32*, U32)
	BOOL SendMsg_SndPut(int a_nSndNum, BOOL a_tNew);					  //* void SndPut(U16, bool)
	BOOL SendMsg_SndGap(int a_nGapLen, BOOL a_tNew);					  //* void SndGap(U16, bool)
	BOOL SendMsg_SndStop();														//* void SndStop()

	BOOL SendMsg_LedPut(int a_uLedState); //* LED Put

	BOOL SendMsg_OledPut();			// OLED Put
	BOOL SendMsg_OledPutCmd();		// OLED Put Cmd
	BOOL SendMsg_OledGetRgb();		// OLED Get RGB
	BOOL SendMsg_OledPutRgb();		// OLED Put RGB

	BOOL SendMsg_BboxGetHeaders(void *a_parBboxSaveHeader, int *a_puDataLen);	//* Get Blackbox Headers at Send Mode
	BOOL SendMsg_BboxSelect	 (int	a_uSelNum, void *a_pBboxSaveHeader);		//* Select Blackbox Block at Send Mode
	BOOL SendMsg_BboxGetData	(void *a_pJgpImg, int *a_puJpgSize);				 //* Get Next Blackbox Data at Send Mode
	BOOL SendMsg_BboxCheck	  (int  *a_puSavedBbox);									//* Get no. of Saved Blackbox

	BOOL SendMsg_SshotSendHeaders();// Get Blackbox Headers at Send Mode
	BOOL SendMsg_SshotSendData();	// Get Next Blackbox Data at Send Mode
	BOOL SendMsg_SshotCheck();		// Get no. of Saved Blackbox

	BOOL SendMsg_RtcGet(int *a_puRtcValue);  //* Get RTC Value
	BOOL SendMsg_RtcSet(int  a_uRtcValue);	//* Set RTC Value

	BOOL SendMsg_UserLdwsOnGet();	// User Get LDWS On/Off
	BOOL SendMsg_UserLdwsOnSet();	// User Set LDWS On/Off
	BOOL SendMsg_UserLkasOnGet();	// User Get LKAS On/Off
	BOOL SendMsg_UserLkasOnSet();	// User Set LKAS On/Off
	BOOL SendMsg_UserG2gOnGet();	 // User Get G2G On/Off
	BOOL SendMsg_UserG2gOnSet();	 // User Set G2G On/Off
	BOOL SendMsg_UserMuteOnGet();	// User Get Mute On/Off
	BOOL SendMsg_UserMuteOnSet();	// User Set Mute On/Off

	BOOL SendMsg_UserVolGet(int *a_puVolume);  //* User Get Volume
	BOOL SendMsg_UserVolSet(int  a_uVolume);	//* User Set Volume

	BOOL SendMsg_UserRunModeGet();  // User Get Run Mode
	BOOL SendMsg_UserRunModeSet();  // User Set Run Mode

	BOOL SendMsg_UserCarTypeGet();  // User Get Car Type
	BOOL SendMsg_UserCarTypeSet();  // User Set Car Type
	BOOL SendMsg_UserLdwsLLvlGet(); // User Get LDWS L-Level
	BOOL SendMsg_UserLdwsLLvlSet(); // User Set LDWS L-Level
	BOOL SendMsg_UserLdwsRLvlGet(); // User Get LDWS R-Level
	BOOL SendMsg_UserLdwsRLvlSet(); // User Set LDWS R-Level
	BOOL SendMsg_UserLdwsWarnGet(); // User Get LDWS Warning
	BOOL SendMsg_UserLdwsWarnSet(); // User Set LDWS Warning

	BOOL SendMsg_UserBboxSave();			 // User Save Blackbox Data
	BOOL SendMsg_UserBboxDel(int a_iTgt); //* User Delete Blackbox Data

	BOOL SendMsg_UserSshotSave();	// User Save Snap-Shot
	BOOL SendMsg_UserSshotDel();	 // User Delete Snap-Shot

	////////////////////////////////////////////////////////////////////

	BOOL SendMsg_MonitorVideo(void *a_pData, int *a_puDataLen);	//* Get Video Monitor
	BOOL SendMsg_MonitorData();											  // Get Data Monitor

	////////////////////////////////////////////////////////////////////

	BOOL SendMsg_FactoryModeStart(void);
	BOOL JTP_Com::SendMsg_FactoryModeEnd(int a_uMode, void *a_pData, int *a_puDataLen);
	BOOL SendMsg_InstallModeStart(int a_uModeLevel, void *a_pData);
	BOOL SendMsg_InstallModeEnd(int a_uMode);
	BOOL JTP_Com::SendMsg_GetBFversionInfo(void *a_pData, int *a_puDataLen);
	BOOL JTP_Com::SendMsg_SetMALINinfo(void *a_pData, int a_puDataLen);
	BOOL JTP_Com::SendMsg_SetBFalertInfo(void *a_pData, int *a_puDataLen);
	BOOL JTP_Com::SendMsg_PowerBtnChk(void);

	///////////////////////////////////////////////////
	BOOL JTP_Com::SendMsg_ADASInfoGet(void *a_pAdasInfoData, int *a_puDataLen);		// Get ADAS information (*주15)
	BOOL JTP_Com::SendMsg_ADASInfoGet_R7(void *a_pAdasInfoData, int *a_puDataLen, int nInSpeed, bool bInTurnL, bool bInTurnR);		// Get ADAS information (*주15)

	BOOL JTP_Com::SendMsg_GetDTCData(int a_uParam, int a_uLength, void *a_pGetData, int *a_puDataLen);

	CString m_strComName;

	JTP_FRAME *m_pRxBuff;
	JTP_FRAME *m_pTxBuff;

	 int m_uRespTrans;
	int m_uRespMsg;
	CString m_strMsg;

protected:
	HANDLE m_hCom;

};

// Wiper_high 상태값 9 이상일시 High
typedef struct MA_Input_COMM_Tag
{
           unsigned velocity                                :12;       // 0~350km/h, 0.1*(Hex) [kph], 0FFFh [Invalid]
           unsigned rain_sensor                : 8;       // 0~127%, 0.5*(Hex) [Nm], FFh [Invalid]
           unsigned wiper_speed               : 4;       // 35~61 cycle/min, 2*(Hex)+3 [cycle/min], Fh [Ivalid]
           unsigned indicate_dir                : 3;       // 0 [Off], 1[Left], 2[Right], 3[Both], 7h [Ivalid]
           unsigned crash_warning             : 2;       // 0 [No warning], 1[Warning], 2[Urgent warning], 3[Ivalid]
           unsigned Wheel_type                           : 2;       // 0 [left-hand], 1[right-hand], 2[Ivalid]
           unsigned dummy1                                         : 1;       // dummy

           unsigned steering_angle             :16;       // -90~90, 0.002747*(Hex) - 90.0, FFFFh [Invalid]
           unsigned beem_state                           : 8;       // see below comment
           unsigned dsp_luminance            : 8;       // 0~100%, (Hex)*100/254 [%], FFh [Ivalid]

           unsigned longi_accel                 :16;       // -65~65, 0.002*(Hex) - 65.0 [m_s2]
           unsigned braking_torque            :16;       // -32000~32000 [Nm], (Hex) - 32000 [Nm], FFFFh [Invalid]

           unsigned ambient_temp             : 8;       // -40~85, 0.5*(Hex) - 40, FFh [Invalid]
           unsigned type_of_car                 : 8;       // 0~61, FFh [Invalid]
           unsigned country_code              : 8;       // see below comment
           unsigned dummy2                                         : 8;       // dummy
} MA_Input_COMM;

typedef struct MA_Output_COMM_Tag
{
           unsigned cw_video                              : 1;       // collision warning
           unsigned cw_audio                              : 1;       // collision warning
           unsigned ldw_arrow                            : 2;       // lane departure warning
           unsigned ldw_lane                              : 2;       // lane departure warning
           unsigned ldw_audio                             : 1;       // lane departure warning
           unsigned di_video                               : 3;       // distance information
           unsigned di_audio                               : 1;       // distance information
           unsigned tsi_video_alert		: 1;       // traffic signal alert
           unsigned tsi_video                              : 8;       // traffic signal information
           unsigned tsi_audio                              : 1;       // traffic signal information
           unsigned aa_video                              : 2;       // attention assistant
           unsigned aa_audio                              : 1;       // attention assistant
           unsigned hba_video                             : 2;       // high beam assistant
           unsigned hba_audio                            : 1;       // high beam assistant
           unsigned adas_available             : 1;       // ADAS Availability
           unsigned adas_invalid                : 1;       // ADAS Invalid
           unsigned reserved                              : 3;       // reserved
} MA_Output_COMM;


typedef struct
{
	U08 szVersionData	[16];   // Fixed string "$VerSion DaTa$", for searching this data
	U08 szModel			[16];
	U08 szHardwareVer	[16];
	U08 szFirmwareVer	[16];
	U08 szFileSysVer	[16];
	U08 szProcessor	[16];
	U08 szCompiler		[16];
	U08 szCompilerVer	[16];
	U08 szBuildDate	[16];
	U08 szBuildTime	[16];
} SYSTEM_VERSION;

typedef struct CFG_SETUP_Tag
{
	/////////////////////////////////////////////////
	// Header Part
	/////////////////////////////////////////////////
	char arcIdH[2];				// Header
	char arcVer[2];				// Version of CFG_SETUP
	U16  uLen;						// = 256 fixed

	/////////////////////////////////////////////////
	// Body Part
	/////////////////////////////////////////////////

	// User Interface
	U08 tLDWS_ManualOff; // true = Manual Off, false = Manual On
	U08 tFCWS_ManualOff; // true = Manual Off, false = Manual On

	// General Vehicle Spec
	U08 uCarType;						// Default : 0
	U08 uAutoCode;						// Default : 0  // 0(한국(KOR)), 1(미국(USA)), 2(중국(CHN)), 3(유럽(EUR))
	U08 uHandleCode;					// Default : 0	// 0(왼쪽(LHD)), 1(오른쪽(RHD))
	U08 bCalibration_Result;			// Default : 0  // 0(=수행않됨), 1(수행후 성공), 2(수행후 실패)
	U16 uCarHeight;						// Default : 1300
	U16 uCarWidth;						// Default : 1800
	S16 uCameraOffset;					// Default : 0
	U16 uHoodLength;					// Default : 1500
	U08 uSelectedKPHMPH;				// Default : 0	// 0(Km/h), 1(Mile/h)

	U08 uReserved1;						// Default : 0

	// General IO Setting Value
	U16 iIOSpeedPulse;					// Default : 2548	// Speed pulse value
	U08 uIOTurnActiveState;				// Default : 1	// Turn Signal Active State : 0(Active Low), 1(Active High)
	U08 uIOWiperActiveState;			// Default : 1	// Wiper Signal Active State : 0(Active Low), 1(Active High)
	U08 uIOBrakeActiveState;			// Default : 1	// Brake Signal Active State : 0(Active Low), 1(Active High)

	// Function Setting Value
	U08 uLDWSFunctionON;				// Default : 1	// Lane Departure Warning Function ON/OFF : 0(Function Off), 1(Function ON)
	U08 uFCWFunctionON;					// Default : 1	// Forward Collision Warning Function ON/OFF : 0(Function Off), 1(Function ON)
	U08 uHMWFunctionON;					// Default : 1	// HeadWay Monitor Warning Function ON/OFF : 0(Function Off), 1(Function ON)
	U08 uFCDAFunctionON;				// Default : 1	// Front Car Departure Alert Function ON/OFF : 0(Function Off), 1(Function ON)
	U08 uVBFunctionON;					// Default : 1	// Virtual Bumper Warning Function ON/OFF : 0(Function Off), 1(Function ON)
	U08 uHBAFunctionON;					// Default : 1	// High Beam Assist Function ON/OFF : 0(Function Off), 1(Function ON)
	U08 uPDFunctionON;					// Default : 1	// Pedestrain Detection Function ON/OFF : 0(Function Off), 1(Function ON)
	U08 uTSRFunctionON;					// Default : 1	// Traffic Sign Recognition Function ON/OFF : 0(Function Off), 1(Function ON)
	U08 uTLRFunctionON;					// Default : 1	// Traffic Light Recognition Function ON/OFF : 0(Function Off), 1(Function ON)

	// Sound Setting Value
	U08 uSoundType;						// Default : 1	// All System Sound ON/OFF and Type : 0(Sound Off), 1(Beep), 2(Voice), 3(Beep and Voice)
	U08 uSoundVolume;					// Default : 7	// Sound Volume Value : 1-15 (1=Min, 15=Max)
	U08 uLDWSSoundON;					// Default : 1	// Lane Departure Warning Sound ON/OFF : 0(Sound Off), 1(Sound ON)
	U08 uFCWSoundON;					// Default : 1	// Forward Collision Warning Sound ON/OFF : 0(Sound Off), 1(Sound ON)
	U08 uHMWSoundON;					// Default : 1	// HeadWay Monitor Warning Sound ON/OFF : 0(Sound Off), 1(Sound ON)
	U08 uFCDASoundON;					// Default : 1	// Front Car Departure Alert Sound ON/OFF : 0(Sound Off), 1(Sound ON)
	U08 uVBSoundON;						// Default : 1	// Virtual Bumper Warning Sound ON/OFF : 0(Sound Off), 1(Sound ON)
	U08 uHBASoundON;					// Default : 1	// High Beam Assist Sound ON/OFF : 0(Sound Off), 1(Sound ON)
	U08 uPDSoundON;						// Default : 1	// Pedestrain Detection Sound ON/OFF : 0(Sound Off), 1(Sound ON)
	U08 uTSRSoundON;					// Default : 1	// Traffic Sign Recognition Sound ON/OFF : 0(Sound Off), 1(Sound ON)
	U08 uTLRSoundON;					// Default : 1	// Traffic Light Recognition Sound ON/OFF : 0(Sound Off), 1(Sound ON)

	// Display Module Setting Value
	U08 uDisplayModuleON;				// Default : 1	// Display Module On State : 0(Off), 1(On)

	// Accelerator Setting value
	S16 iAccX;							// Default : 0	// Accelerator X value -16383 ~ +16383
	S16 iAccY;							// Default : 0	// Accelerator Y value -16383 ~ +16383
	S16 iAccZ;							// Default : 0	// Accelerator Z value -16383 ~ +16383
	U08 uAccSavedState;					// Default : 0	// Accelerator Setting Saved State : 0(Not Setted), 1(OK)

	U08 uReserved2;						// Default : 0

	// LDW & FCW
	U08 uLdw_CalibrationResult;			// Default : 0	// Calibration : 0(Not Setted), 1(OK), 2(Error)
	U08 bCenterX;						// Default : 160
	U08 bCenterY;						// Default : 90
	U08 bVanishX;						// Default : 160
	U08 bVanishY;						// Default : 90
	U08 bVanishCalcY;					// Default : 0	// Calculated by G-Sensor
	U08 bVanishAutoY;					// Default : 0	// Calculated by Auto Calibration Logic
	U08 uAutoCalibrationState;			// Default : 0	// Number of Auto Calibration Running

	U08 bLDWS_KphHi;					// Default : 160
	U08 bLDWS_KphLo;					// Default : 40
	U08 bFCWS_KphHi;					// Default : 160
	U08 bFCWS_KphLo;					// Default : 20

	// HMW, VB, FCDA
	U08 uHMW_WarningLevel; 				// Default : 7 (1=0.1sec, 0.4 ~ 2.0)
	U08 bVB_KphWarn;					// Default : 30Kph	// Virtual Bumper Warning End Speed
	U16 uFCDA_DelayTime;				// Default : 200	// FCDA warning delay time msec

	S08 cLDWS_StepL;					// Default : 0
	S08 cLDWS_StepR;					// Default : 0

	U08 bSetCameraStartY;				// Default : 124

	U08 bLDWS_KphWarn;					// Default : 40
	U08 bFCWS_KphWarn;					// Default : 20

	S08 cLDWS_SensLevel_L;				// Default : 0
	S08 cLDWS_SensLevel_R;				// Default : 0
	U16 uLDWS_SensLevel_Set_mm;			// Default : 0
	S08 cFCWS_SensLevel_HMW;			// Default : 0
	U16 uFCWS_SensLevel_HMW_Set[3];		// Absolute SensLevel Setting Values
	S16 cFCWS_SensLevel_FCW;			// Default : 0
	U16 uFCWS_SensLevel_FCW_Set[3];		// Relative SensLevel Setting Values

	U16 uAA_SensLevel_ChkTime;			// Default : 30 Check Time (Steering, Departure) -> minute
	U08 bAA_SensLevel_valStrChk;		// Default : 5
	U08 bAA_SensLevel_nStrChk;			// Default : 10
	U08 bAA_SensLevel_nDptChk;			// Default : 10
	U08 bAA_SensLevel_tDriving;			// Default : 180 Driving Time -> minute

	// PD
	U08 ubPD_WarningLevel;				// Default : 0	// Person Dectect Warning Level : Not defined

	// VB distance
	U08 uVB_Warning_Distance;			// Default : 2m (1:0.1m, 0 ~ 3m)

	U32 arwLumaThreshold[8];			// Luma Threshold Table
	U32 wLuma_Threshold_Day;			// Threshold Value for Day
	U32 wLuma_Threshold_Night;			// Threshold Value for Night
	U32 wLuma_Delay_Day;				// Delay time for Day.   (Unit: 100ms). ex) 2 minutes and 25 seconds -> (2*60+25)*100
	U32 wLuma_Delay_Night;				// Delay time for Night. (Unit: 100ms). ex) 100 seconds -> (100)*100

	// Watex 제품정보
	U08 arbDeviceSerial[16];
	U08 arbSetDate[16];


	///// Spare Data Bytes
	U08 arbRsv[256-2-152-36];				// sizeof(CFG_SETUP) must equal 256 (2016.03.23 Default 152Byte)

	/////////////////////////////////////////////////
	// Footer Part
	/////////////////////////////////////////////////
	char arcIdT[2];
} CFG_SETUP;

/*****************************************
***        Config Data Structure       ***
*****************************************/
typedef struct CFG_SETUP_R7_Tag
{
	/////////////////////////////////////////////////
	// Header Part
	/////////////////////////////////////////////////
	char arcIdH[2];						//0		 2		// Header
	char arcVer[2];						//2		 2		// Version of CFG_SETUP
	U16 uLen;							//4		 2		// Data Length => 256 fixed
	U08 arbReserved0[2];				//6		 2		// Reserved Memory 0

	/////////////////////////////////////////////////
	// Body Part
	/////////////////////////////////////////////////

	/// Product Information.
	U08 arbDeviceSerial[16];			//8		16		// Device Serial Number

	/// Production Date.
	U08 arbSetDate[16];					//24	16		// Set Production Date

	/// User Interface.
	U08 bLDWS_ManualOff;				//40	 1		// Default : 0		// 1: Manual Off, 0: Manual On
	U08 bFCWS_ManualOff;				//41	 1		// Default : 0		// 1: Manual Off, 0: Manual On
	U08 arbReserved1[2];				//42	 2		// Reserved Memory 1

	/// General Vehicle Spec.
	U08 bCarType;						//44	 1		// Default : 0		// 0: Sedan, 1: SUV, 2: Truck, 3: Bus
	U08 bAutoCode;						//45	 1		// Default : 0		// 0: KOR, 1: USA, 2: CHN, 3: EUR, 4: MES, 5: JAP
	U08 bHandleCode;					//46	 1		// Default : 0		// 0: LHD, 1: RHD
	U08 bCalibrationResult;				//47	 1		// Default : 0		// 0: Not setted, 1: Success, 2: Fail
	U16 uCarHeight;						//48	 2		// Default : 1300
	U16 uCarWidth;						//50	 2		// Default : 1800
	S16 iCameraOffset;					//52	 2		// Default : 0
	U16 uHoodLength;					//54	 2		// Default : 1700
	U08 bSelectedKPHMPH;				//56	 1		// Default : 0		// 0: Km/h, 1: Mile/h
	U08 arbReserved2[3];				//57	 3		// Reserved Memory 2

	/// General IO Setting Value.
	U16 uIOSpeedPulse;					//60	 2		// Default : 2548	// Speed pulse value
	U08 bIOTurnActiveState;				//62	 1		// Default : 1		// Turn Signal Active State : 0(Active Low), 1(Active High), 2(CAN)
	U08 bIOWiperActiveState;			//63	 1		// Default : 1		// Wiper Signal Active State : 0(Active Low), 1(Active High), 2(CAN)
	U08 bIOBrakeActiveState;			//64	 1		// Default : 1		// Brake Signal Active State : 0(Active Low), 1(Active High), 2(CAN)
	U08 bButtonON;						//65	 1		// Default : 0		// Button Enable State : 0(Using Button), 0x1(Function ON/OFF Button Not Use), 0x2(All Button Not Use)
	U08 arbReserved3[2];				//66	 2		// Reserved Memory 3

	/// Function Setting Value.
	U08 bLDWSFunctionON;				//68	 1		// Default : 1		// Lane Departure Warning Function ON/OFF : 0(Function Off), 1(Function ON)
	U08 bFCWFunctionON;					//69	 1		// Default : 1		// Forward Collision Warning Function ON/OFF : 0(Function Off), 1(Function ON)
	U08 bHMWFunctionON;					//70	 1		// Default : 1		// HeadWay Monitor Warning Function ON/OFF : 0(Function Off), 1(Function ON)
	U08 bFCDAFunctionON;				//71	 1		// Default : 1		// Front Car Departure Alert Function ON/OFF : 0(Function Off), 1(Function ON)
	U08 bVBFunctionON;					//72	 1		// Default : 1		// Virtual Bumper Warning Function ON/OFF : 0(Function Off), 1(Function ON)
	U08 bHBAFunctionON;					//73	 1		// Default : 1		// High Beam Assist Function ON/OFF : 0(Function Off), 1(Function ON)
	U08 bPDFunctionON;					//74	 1		// Default : 1		// Pedestrain Detection Function ON/OFF : 0(Function Off), 1(Function ON)
	U08 bTSRFunctionON;					//75	 1		// Default : 1		// Traffic Sign Recognition Function ON/OFF : 0(Function Off), 1(Function ON)
	U08 bTLRFunctionON;					//76	 1		// Default : 1		// Traffic Light Recognition Function ON/OFF : 0(Function Off), 1(Function ON)
	U08 arbReserved4[3];				//77	 3		// Reserved Memory 4

	/// Sound Setting Value.
	U08 bSoundType;						//80	 1		// Default : 1		// All System Sound ON/OFF and Type : 0(Sound Off), 1(Beep1), 2(Beep2), 3(Voice), 4(Beep and Voice)
	U08 bSoundVolume;					//81	 1		// Default : 2		// Sound Volume Value : 0-3 (0=Min, 3=Max)
	U08 bHapticOn;						//82	 1		// Default : 0		// Haptic Warning On/Off : 0(Haptic Off), 1(Haptic On)
	U08 bLDWSSoundON;					//83	 1		// Default : 1		// Lane Departure Warning Sound ON/OFF : 0(Sound Off), 1(Sound ON)
	U08 bFCWSoundON;					//84	 1		// Default : 1		// Forward Collision Warning Sound ON/OFF : 0(Sound Off), 1(Sound ON)
	U08 bHMWSoundON;					//85	 1		// Default : 1		// HeadWay Monitor Warning Sound ON/OFF : 0(Sound Off), 1(Sound ON)
	U08 bFCDASoundON;					//86	 1		// Default : 1		// Front Car Departure Alert Sound ON/OFF : 0(Sound Off), 1(Sound ON)
	U08 bVBSoundON;						//87	 1		// Default : 1		// Virtual Bumper Warning Sound ON/OFF : 0(Sound Off), 1(Sound ON)
	U08 bHBASoundON;					//88	 1		// Default : 1		// High Beam Assist Sound ON/OFF : 0(Sound Off), 1(Sound ON)
	U08 bPDSoundON;						//89	 1		// Default : 1		// Pedestrain Detection Sound ON/OFF : 0(Sound Off), 1(Sound ON)
	U08 bTSRSoundON;					//90	 1		// Default : 1		// Traffic Sign Recognition Sound ON/OFF : 0(Sound Off), 1(Sound ON)
	U08 bTLRSoundON;					//91	 1		// Default : 1		// Traffic Light Recognition Sound ON/OFF : 0(Sound Off), 1(Sound ON)
	U08 bReserved5[4];					//92	 4		// Reserved Memory 5

	/// Display Module Setting Value.
	U08 bDisplayModuleON;				//96	 1		// Default : 1		// Display Module On State : 0(Off), 1(On)
	U08 arbReserved6[3];				//97	 3		// Reserved Memory 6

	/// Acc & Vanish Setting value.
	S16 iAccX;							//100	 2		// Default : 0		// Accelerator X value -16383 ~ +16383
	S16 iAccY;							//102	 2		// Default : 0		// Accelerator Y value -16383 ~ +16383
	S16 iAccZ;							//104	 2		// Default : 0		// Accelerator Z value -16383 ~ +16383
	U08 bAccSavedState;					//106	 1		// Default : 0		// Accelerator Setting Saved State : 0(Not Setted), 1(OK)
	U08 bAccCalibResult;				//107	 1		// Default : 0		// Calibration : 0(Not Setted), 1(OK), 2(Error)
	U08 bSetCameraStartY;				//108	 1		// Default : 124
	U08 bVanishX;						//109	 1		// Default : 160
	U08 bVanishY;						//110	 1		// Default : 90
	U08 bAutoCalibStartKph;				//111	 1		// Default : 70
	U08 bAutoCalibState;				//112	 1		// Default : 0		// Number of Auto Calibration Running
	U08 bCenterX;						//113	 1		// Default : 160
	U08 bCenterY;						//114	 1		// Default : 90
	U08 arbReserved7;					//115	 1		// Reserved Memory 7

	/// LUMA setting.
	U32 arwLumaThreshold[8];			//116	32		// Default : 32, 132, 547, 2259, 9330, 38536, 159156, 0xFFFFFFFF	// Luma Threshold Table
	U32 wLuma_Threshold_Day;			//148	 4		// Default : 2259	// Threshold Value for Day
	U32 wLuma_Threshold_Night;			//152	 4		// Default : 547	// Threshold Value for Night
	U32 wLuma_Delay_Day;				//156	 4		// Default : 3*60*100	// Delay time for Day.   (Unit: 100ms). ex) 2 minutes and 25 seconds -> (2*60+25)*100
	U32 wLuma_Delay_Night;				//160	 4		// Default : 3*60*100	// Delay time for Night. (Unit: 100ms). ex) 100 seconds -> (100)*100

	/// LDW setting.
	S08 cLDW_StepL;						//164	 1		// Default : 0
	S08 cLDW_StepR;						//165	 1		// Default : 0
	S08 cLDW_SensLevel_L;				//166	 1		// Default : 0
	S08 cLDW_SensLevel_R;				//167	 1		// Default : 0
	U16 uLDW_SensLevel_Set_mm;			//168	 2		// Default : 100	(100 = 100mm)
	U08 bLDW_KphHi;						//170	 1		// Default : 160
	U08 bLDW_KphLo;						//171	 1		// Default : 20
	U08 bLDW_KphWarn;					//172	 1		// Default : 60
	U08 arbReserved8[3];				//173	 3		// Reserved Memory 8

	/// FCW setting.
	S08 cFCW_SensLevel;					//176	 1		// Default : -5	(-10 ~ 10, setted TTC + value*100ms)
	U08 bFCW_KphHi;						//177	 1		// Default : 160
	U08 bFCW_KphLo;						//178	 1		// Default : 0
	U08 bFCW_KphWarn;					//179	 1		// Default : 1
	U08 arbReserved9[4];				//180	 4		// Reserved Memory 9

	/// HMW setting.
	U08 bHMW_WarningTTT1st;				//184	 1		// Default : 0  (1=0.1sec, 0.5 ~ 1.2, 0 = disable)
	U08 bHMW_WarningTTT2nd;				//185	 1		// Default : 12 (1=0.1sec, 0.5 ~ 1.2, 0 = disable)
	U08 bHMW_WarningTTT3rd;				//186	 1		// Default : 7  (1=0.1sec, 0.5 ~ 1.2, 0 = disable)
	U08 bHMW_KphHi;						//187	 1		// Default : 160
	U08 bHMW_KphLo;						//188	 1		// Default : 20
	U08 bHMW_KphWarn;					//189	 1		// Default : 40
	U08 arbReserved10[2];				//190	 2		// Reserved Memory 10

	/// VB setting.
	U08 bVB_WarningDistance;			//192	 1		// Default : 40(4m) (3.0m~5.0m, 1=0.1m)
	U08 bVB_KphWarn;					//193	 1		// Default : 20Kph	// Virtual Bumper Warning End Speed
	U08 arbReserved11[2];				//194	 2		// Reserved Memory 11

	/// FCDA setting.
	U16 uFCDA_DelayTime;				//196	 2		// Default : 100	// FCDA warning delay time msec
	U08 arbReserved12[2];				//198	 2		// Reserved Memory 12

	/// PD setting.
	S08 cPD_SensLevel;					//200	 1		// Default : 0
	U08 bPD_KphHi;						//201	 1		// Default : 30
	U08 bPD_KphLo;						//202	 1		// Default : 0
	U08 bPD_KphWarn;					//203	 1		// Default : 30 // End Speed
	U08 arbReserved13[4];				//204	 4		// Reserved Memory 13

	/// AA setting.
	U16 uAA_SensLevel_ChkTime;			//208	 2		// Default : 30 Check Time (Steering, Departure) -> minute
	U08 bAA_SensLevel_StartSpeed;		//210	 1		// Default : 80
	U08 bAA_SensLevel_valStrChk;		//211	 1		// Default : 5
	U08 bAA_SensLevel_nStrChk;			//212	 1		// Default : 10
	U08 bAA_SensLevel_nDptChk;			//213	 1		// Default : 10
	U08 bAA_SensLevel_tDriving;			//214	 1		// Default : 180 Driving Time -> minute
	U08 arbReserved14;					//215	 1		// Reserved Memory 14

	/// Debug.
	S32	lDebug[4];						//216	16		// Reserved Memory for Debug

	/// Spare Data Bytes
	U08 arbRsv[256-232-2];				//232	22		// sizeof(CFG_SETUP) must equal 256 (2016.07.19 Default 190Byte)

	/////////////////////////////////////////////////
	// Footer Part
	/////////////////////////////////////////////////
	char arcIdT[2];
} CFG_SETUP_R7;


/* typedefs */
typedef struct Input_COMM_Tag
{
	U16 uDisplayInfo;		// Display Button Status
	U16 uReserved0;
	U16 uVehicleSpeed;		// Vehicle speed. 0~255kph
	U16 uWiper;				// Wiper Status (0: OFF, 1: ON)
	U16 uTurnL;				// Left  Turn signal On/Off (0: OFF, 1: ON)
	U16 uTurnR;				// Right Turn signal On/Off (0: OFF, 1: ON)
	U16 uRearLamp;
	U16 uBrake;				// Brake signal On/Off (0: OFF, 1: ON)
	U16 uIGchk;
	S08 sTemp;
	U08 bResrved1;
} Input_COMM;

// comment 16, 16-1
// HUB => BF
typedef struct
{
    U16 uDispBtn;
    U16 uReserved;
    U16 uVssTick;
    U16 uADWiper;
    U16 uADTurnL;
    U16 uADTurnR;
    U16 uADRearLamp;
    U16 uADBrake;
    U16 uADIg;
    U08 bADTemp;
    U08 bReserved1;
} HUB_STAT3;

/////////////////////////////////////////////////////
//CMD_SEND_ADAS HUB INFO
/////////////////////////////////////////////////////
#define DISPLAY_ON           0x01

#define DEVICE_ON            0x01
#define DEVICE_INSTALL_MODE  0x02

#define GPS_STATUS_INVALID   0x08
#define GPS_STATUS_VALID     0x10
#define GPS_STATUS_SPEED     0x20
#define GPS_STATUS_OK        0x30
#define DEVICE_ERROR         0x40

#define ADAS_LANE_DETECT     0x01
#define ADAS_LANE_LEFT_WARN  0x04
#define ADAS_LANE_RIGHT_WARN 0x08
#define ADAS_LANE_DETECT_NO_DISPLAY 0x20
#define ADAS_LANE_DETECT_ERROR 0x40

#define ADAS_PD_DETECT       0x01
#define ADAS_PD_WARN         0x02

#define ADAS_FCW_HMW_DETECT  0x0001
#define ADAS_FCW_HMW_WARN    0x0002
#define ADAS_FCW_FCW_WARN    0x0010
#define ADAS_FCW_VB_WARN     0x0020
#define ADAS_FCDA_RUNNING    0x0040
#define ADAS_FCDA_DETECT     0x0080
#define ADAS_FCDA_WARN       0x0100

#define ADAS_HBA_ON          0x0001
#define ADAS_HBA_OFF         0x0002

#define ADAS_TSR_KPH         0x00FF
#define ADAS_TSR_ON          0xFF00

#define ADAS_BRAKE_ON        0x01
#define ADAS_TURN_LEFT_ON    0X02
#define ADAS_TURN_RIGHT_ON   0X04
#define ADAS_MODE_NIGHT      0x08
#define ADAS_MODE_SAVING     0x10
#define ADAS_SIGNAL_DISPLAY  0x20

#define ADAS_MODE_DAY_NIGHT  0x08
#define ADAS_MODE_DAY        0x00


typedef struct Output_COMM_Tag
{
	// Total 16Byte
	// Device State (1Byte)
	unsigned DeviceON					: 1;	// Device ON/OFF (0: 대기중, 1: 준비완료)
	unsigned InstallMode				: 2;	// Install Mode (0: 일반 모드, 1: 설치 모드)
	unsigned GPSState					: 3;	// GPS 수신상태 (0: 신호가 전혀 수신되지 않음, 1: 신호가 수신되지만 유효하지 않음, 2: 신호가 정상적으로 수신됨, 4: 속도 수신됨)
	unsigned DeviceState			: 2;	// Device State (0: Normail State, 1: Error State)

	// Lane Detecte State (1Byte)
	unsigned LaneDetect				: 2;	// Lane Detect (0: 차선 미인식, 1: 차선 인식)
	unsigned LaneDepartureWarning	: 3;	// Lane Daparture Warning (0: 차선 이탈경보 OFF, 1: 왼쪽차선 이탈경보, 2: 오른쪽차선 이탈경보)
	unsigned Reserved2				: 3;	// Reserved

	// FCW State (2Byte)
	unsigned HMWWarning				: 4;	// HMW Warning (0: 전방 차량 미인식, 1: 전방 차량 인식, 2: 1차 알림, 4: 2차 알림, 8: 3차 알림)
	unsigned FCWWarning				: 1;	// FCW Warning (0: 미경보, 1: 전방 차량 근접 경보)
	unsigned VirtualBumper			: 1;	// Virtual Bumper (0: 미경보, 1: 전방 차량 근접 경보)
	unsigned LeaveFrontCar			: 3;	// Leave front car (FCDA) (0: 보통상태, 1: 주행중, 2: 감시중, 4: 전방차량 출발 알림)
	unsigned Reserved3				: 7;	// Reserved

	// HMW TTT (2Byte)
	unsigned TTT					:16;	// HMW Warning (msec 단위 충돌 예상시간, 0 ~ 65500 msec)
	
	// PD State (1Byte)
	unsigned PersonDetect				: 2;	// Person Detect (0: 사람 미인식, 1: 사람 인식, 2: 사람 접근 경보)
	unsigned Reserved4				: 6;	// Reserved

	// HBA State (1Byte)
	unsigned HBAStatus				: 2;	// HBA State (0: HBA 동작안함, 1: HBA 동작중, 2: HBA OFF 경보)
	unsigned Reserved5				: 6;	// Reserved

	// TSR State (2Byte)
	unsigned DetectSpeed				: 8;	// TSR 속도값(Kph 단위 속도값) (0: 미인식, 1~254 Kph)
	unsigned TSRSignType			: 8;	// TSR 인식 종류 (0: 미인식, 1: 속도표시, 2: 진입금지)

	// TSR Over Speed				//TLR State (1Byte)
	unsigned TSROverSpeed			: 8;	// TSR 초과 속도 (Kph 단위 속도값) //unsigned TLRSignType

	// IO Output State (1Byte)
	unsigned Vibrator				: 1;	// Vibrator ON/OFF (0: Vibrator OFF, 1: Vibrator ON)
	unsigned Trigger				: 1;	// Trigger ON/OFF (0: Trigger OFF, 1: Trigger ON)
	unsigned TriggerEnable			: 1;	// TriggerEnable ON/OFF (0: Direct Trigger Mode OFF, 1: Direct Trigger Mode ON)
	unsigned Reserved6				: 5;	// Reserved

	// PD Distance (2Byte)
	unsigned PDDistance				:16;	// PD Distance (1 == 0.1m, 0 ~ 65500 * 0.1m)

	// Setup State (1Byte)
	unsigned DisplayModuleOn		: 1;		// Display Module On State (0: Off, 1: On)
	unsigned Reserved7				: 7;	// Reserved

	// Signal (1Byte)
	unsigned Brake					: 1;	// Brake Signal State
	unsigned TurnL					: 1;	// Turn Left Signal State
	unsigned TurnR					: 1;	// Turn Right Signal State
	unsigned DayMode				: 1;	// DayMode
	unsigned SavingState			: 1;	// SavingState
	unsigned Reserved8				: 3;	// Reserved
} Output_COMM;

////////////////// CMD_SEND_ADAS //////////////////
// Hub -> New Disaplay
typedef struct
{
    U08 bDeviceState;
    U08 bLaneDetectState;
    U16 uFcwState;
    U16 uHmwTTT;
    U08 bPdState;
    U08 bHBA;
    U16 uTSR;
    U08 bTsrOverSpd;
    U08 bIoOutput;
    U16 uPdDistance;
    U08 bDisplayModuleState;
    U08 bSignal;
} HUB_INFO;

#endif //  !Y_JTP_COM_H  //