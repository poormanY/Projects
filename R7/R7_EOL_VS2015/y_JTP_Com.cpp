// JTP_Com.cpp: implementation of the JTP_Com class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "y_JTP_Com.h"
#include "y_Uart4Pc.h"
#include "y_Debugging.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//extern FW_FILE_INFO		  oFW_File_info;
#define JTP_BAUDRATE	9600		// BF539	: 460800,	BF609 : 912600, ZIG : 115200


JTP_Com::JTP_Com()
{
	m_hCom = INVALID_HANDLE_VALUE;
	m_strMsg = "Disconnected";

	m_pRxBuff = (JTP_FRAME*)malloc(64*1024);
	m_pTxBuff = (JTP_FRAME*)malloc(64*1024);

	if ( (m_pRxBuff == NULL) || (m_pTxBuff == NULL) )
	{
		if (m_pRxBuff != NULL) free(m_pRxBuff);
		if (m_pTxBuff != NULL) free(m_pTxBuff);
		m_pRxBuff = m_pTxBuff = NULL;

		m_strMsg = "Memory Allocation Error!";
	}
}

JTP_Com::~JTP_Com()
{
	CloseCom();

	if (m_pRxBuff != NULL) free(m_pRxBuff);
	if (m_pTxBuff != NULL) free(m_pTxBuff);
	m_pRxBuff = m_pTxBuff = NULL;
}


BOOL JTP_Com::AutoOpenCom()
{
	CString strComName;
	
	if (m_hCom != INVALID_HANDLE_VALUE)
	{
		if ( SendMsg_Connect(500) == TRUE )
		{
			return TRUE;
		}
		else
		{
			CloseCom();
		}
	}

	for (int i = 1; i <= 20; i++)
	{
		strComName.Format(_T("COM%d"), i);
		if (OpenCom(strComName) == FALSE) continue;

		if ( SendMsg_Connect(500) == TRUE )
		{			
			dp("R7 USART Port : COM%d \n", i);

			return TRUE;
		}
		else
		{
			CloseCom();
			continue;
		}
	}

	m_strMsg = "\r\n厘摹甫 惯斑窍瘤 给沁嚼聪促.";
	return FALSE;
}

BOOL JTP_Com::OpenCom(CString a_pszCom)
{
	CString strMsg;

	m_strMsg = "OpenCom() ";
	if ( (m_pRxBuff == NULL) || (m_pTxBuff == NULL) )
	{
		strMsg = "Memory Allocation Error!";
		m_strMsg += strMsg;
		return FALSE;
	}

	m_hCom = UartSetup((CStringA)a_pszCom, JTP_BAUDRATE);

	if (m_hCom == INVALID_HANDLE_VALUE)
	{
		strMsg.Format(_T("%s Open Error! <%08X>"), a_pszCom, dwUartError);
		m_strMsg += strMsg;
		return FALSE;
	}
	else
	{
		m_strComName = a_pszCom;
		strMsg.Format(_T("%s Open Success!"), a_pszCom);
		m_strMsg += strMsg;
		return TRUE;
	}
}

BOOL JTP_Com::CloseCom()
{
	if (m_hCom != INVALID_HANDLE_VALUE)
	{
		PurgeComm(m_hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		CloseHandle(m_hCom);

		m_hCom = INVALID_HANDLE_VALUE;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


///////////////////// Transmit Functions ///////////////////////
JTP_FRAME *JTP_Com::JTP_TxMakeBaseFrame(void *a_pTxBuff, U16 a_uDataLen)
{
	JTP_FRAME *pFrame;

	pFrame = (JTP_FRAME*)_MK_ALIGN_4(a_pTxBuff);

	pFrame->uHead = JTP_HEAD_VALUE;
	pFrame->uDataLen = a_uDataLen;
	pFrame->uFoot = JTP_FOOT_VALUE;

	return pFrame;
}

JTP_FRAME *JTP_Com::JTP_TxMakeFullFrame(void *a_pTxBuff, U16 a_uDataLen)
{
	JTP_FRAME *pFrame;
	U16 uDataFieldLen;

	if (a_uDataLen > JTP_DATALEN_MAX) return NULL;

	pFrame = (JTP_FRAME*)_MK_ALIGN_4(a_pTxBuff);
	uDataFieldLen = _MK_ALIGN_2(a_uDataLen);

	pFrame->uHead = JTP_HEAD_VALUE;
	pFrame->uDataLen  = a_uDataLen;

	*(U16*)((U08*)(&pFrame->uFoot) + uDataFieldLen) = JTP_FOOT_VALUE;

	return pFrame;
}

void JTP_Com::JTP_TxMakeMsg(JTP_FRAME *a_pFrame, U16 a_uTgtId, U16 a_uSrcId, U16 a_uCmd, U16 a_uPara)
{
	// Write Target ID and Command, Parameter
	if (a_pFrame == NULL) return;

	a_pFrame->uTgtId = a_uTgtId;
	a_pFrame->uSrcId = a_uSrcId;
	a_pFrame->uCmd = a_uCmd;
	a_pFrame->uPara  = a_uPara;
}

////////////////////

void JTP_Com::JTP_TxMakeChkSumBaseFrame(JTP_FRAME *a_pFrame, const void *a_pData)
{
	U16 uDataFieldLen;

	if (a_pFrame == NULL) return;

	uDataFieldLen = _MK_ALIGN_2(a_pFrame->uDataLen);
	a_pFrame->uChkSum = JTP_CalcChkSumBaseFrame(a_pFrame, a_pData);
}

U16 JTP_Com::JTP_CalcChkSumBaseFrame(JTP_FRAME *a_pFrame, const void *a_pData)
{
	U16 *puData;
	U16 uData;
	U16 uMax;
	U16 i;

	uData = 0;

	uMax = (U16)((((U32)&a_pFrame->uChkSum - (U32)&a_pFrame->uHead)) / sizeof(U16));
	puData = (U16*)(&a_pFrame->uHead);

	for (i = 0; i < uMax; i++)
	{
		uData ^= puData[i];
	}

	uMax = _MK_ALIGN_2(a_pFrame->uDataLen) / 2;
	puData = (U16*)(a_pData);

	for (i = 0; i < uMax; i++)
	{
		uData ^= puData[i];
	}

	return (~uData);
}

////////////////////

void JTP_Com::JTP_TxMakeChkSumFullFrame(JTP_FRAME *a_pFrame)
{
	U16 uDataFieldLen;

	if (a_pFrame == NULL) return;

	uDataFieldLen = _MK_ALIGN_2(a_pFrame->uDataLen);
	*(U16*)((U08*)(&a_pFrame->uChkSum) + uDataFieldLen) = JTP_CalcChkSumFullFrame(a_pFrame);
}

U16 JTP_Com::JTP_CalcChkSumFullFrame(JTP_FRAME *a_pFrame)
{
	U16 uDataFieldLen;
	U16 *puData;
	U16 uData;
	U16 uMax;
	U16 i;

	uDataFieldLen = _MK_ALIGN_2(a_pFrame->uDataLen);

	uMax = (U16)((((U32)&a_pFrame->uChkSum - (U32)&a_pFrame->uHead) + uDataFieldLen) / sizeof(U16));

	puData = (U16*)(&a_pFrame->uHead);
	uData = 0;

	for (i = 0; i < uMax; i++)
	{
		uData ^= puData[i];
	}

	return (~uData);
}

//////////////////////////// Util ///////////////////////////
U16 JTP_Com::JTP_GetSizeFullFrame(const JTP_FRAME *a_pFrame)
{
	U16 uDataFieldLen = _MK_ALIGN_2(a_pFrame->uDataLen);

	return (sizeof(JTP_FRAME) + uDataFieldLen);
}


///////////////////// Receive Functions ///////////////////////

static const U16 aruAcceptId[] = {('P'<<0)+('C'<<8), ('A'<<0)+('L'<<8), 0};

BOOL JTP_Com::JTP_ChkAcceptId(U16 a_uRxTgtId)
{
	U16 uAcceptId;
	U16 i;

	i = 0;
	while (true)
	{
		uAcceptId = aruAcceptId[i];
		if (uAcceptId == 0			) return false;
		if (uAcceptId == a_uRxTgtId) return true;
		i++;
	}
}

U16 JTP_Com::JTP_ReadChkSum(JTP_FRAME *a_pFrame)
{
	U16 uDataFieldLen = _MK_ALIGN_2(a_pFrame->uDataLen);

	return (*(U16*)((U08*)(&a_pFrame->uChkSum) + uDataFieldLen));
}

U16 JTP_Com::JTP_ReadFoot (JTP_FRAME *a_pFrame)
{
	U16 uDataFieldLen = _MK_ALIGN_2(a_pFrame->uDataLen);

	return (*(U16*)((U08*)&a_pFrame->uFoot + uDataFieldLen));
}





int JTP_Com::TransferMsg(JTP_FRAME *a_pTx, int a_uWaitMs)
{
	DWORD lRxWaitTime;

	if ( (m_pRxBuff == NULL) || (a_pTx == NULL) )
	{
		m_uRespTrans = _JTP_RESP_TRANS_ERR;
		return m_uRespTrans;
	}

	// Clear Dummy Rx Buffer
	lRxWaitTime = GetTickCount()
				+ a_uWaitMs;
	int iRxCount = UartRxCount(m_hCom);
	while (iRxCount > 0)
	{
		UartRead(m_hCom, m_pRxBuff, iRxCount);
		Sleep(10);
		iRxCount = UartRxCount(m_hCom);
		if ( GetTickCount() > lRxWaitTime )
		{
			m_uRespTrans = _JTP_RESP_TRANS_TIMEOUT;
			return m_uRespTrans;
		}
	}

	// Transmit Start
	U16 uTransLen = 0;

	// TX
	if (a_pTx != NULL)
	{
		uTransLen = JTP_GetSizeFullFrame(a_pTx);
		UartWrite(m_hCom, a_pTx, uTransLen);//////////////////////////////////
	}

	// Calc Rx Wait Time
	uTransLen += sizeof(JTP_FRAME);

	lRxWaitTime = GetTickCount()
				+ UartCalcTransTime(dwUartBaudRate, uTransLen)
				+ a_uWaitMs;

	// Wait Rx Frame
	while ( UartRxCount(m_hCom) < (int)(sizeof(JTP_FRAME)) )
	{
		Sleep(10);
		if ( GetTickCount() > lRxWaitTime )
		{
			m_uRespTrans = _JTP_RESP_TRANS_TIMEOUT;
			return m_uRespTrans;
		}
	}

	// Check Rx Message
	UartRead(m_hCom, m_pRxBuff, sizeof(JTP_FRAME));
	
	if ( ( m_pRxBuff->uHead  != JTP_HEAD_VALUE) ||
		( m_pRxBuff->uTgtId != a_pTx->uSrcId ) ||
		( (m_pRxBuff->uCmd & (~JTP_CMD_NO_CHKSUM))  != (a_pTx->uCmd & (~JTP_CMD_NO_CHKSUM)) ) )
	{
		m_uRespTrans = _JTP_RESP_TRANS_ERR;
		return m_uRespTrans;
	}

	// Wait Rx Data and Receive Data
	if (m_pRxBuff->uDataLen > 0)
	{
		uTransLen = _MK_ALIGN_2(m_pRxBuff->uDataLen) - 1;

		lRxWaitTime = GetTickCount()
					+ UartCalcTransTime(dwUartBaudRate, uTransLen)
					+ 1000;
		while ( UartRxCount(m_hCom) < uTransLen )
		{
			Sleep(10);
			if ( GetTickCount() >  lRxWaitTime )
			{
				m_uRespTrans = _JTP_RESP_TRANS_TIMEOUT;
				return m_uRespTrans;
			}
		}

		// Append Data to m_pRxBuff
		UartRead(m_hCom, (U08*)m_pRxBuff+sizeof(JTP_FRAME), uTransLen);
	}
#if 0
	if ( (m_pRxBuff->uCmd & JTP_CMD_NO_CHKSUM) &&
		(m_pRxBuff->uDataLen != 0 ) )
	{
		if ( m_pRxBuff->uDataLen != JTP_ReadChkSum(m_pRxBuff) )
		{
			m_uRespTrans = _JTP_RESP_TRANS_CHKSUM;
			return m_uRespTrans;
		}
	}
	else
	{
		if ( JTP_CalcChkSumFullFrame(m_pRxBuff) != JTP_ReadChkSum(m_pRxBuff) )
		{
			m_uRespTrans = _JTP_RESP_TRANS_CHKSUM;
			return m_uRespTrans;
		}
	}

	if (JTP_ReadFoot(m_pRxBuff) != JTP_FOOT_VALUE)
	{
		m_uRespTrans = _JTP_RESP_TRANS_MSGEND;
		return m_uRespTrans;
	}
#endif

	m_uRespTrans = _JTP_RESP_OK;
	return m_uRespTrans;
}


void JTP_Com::CvtRespMessage(CString &a_strMsg, int a_uResp)
{
	CString strTmp;

	if (a_uResp == 0)
	{
		a_strMsg = "OK";
	}
	else
	if (a_uResp >= _JTP_RESP_TRANS_ERR)
	{
		a_strMsg = "Transmit Error: ";
		switch (a_uResp)
		{
		case _JTP_RESP_TRANS_TIMEOUT:
			a_strMsg += "Time out";
			break;
		case _JTP_RESP_TRANS_TOO_BIG:
			a_strMsg += "Too Big Data";
			break;
		case _JTP_RESP_TRANS_CHKSUM:
			a_strMsg += "Invalid ChkSum";
			break;
		case _JTP_RESP_TRANS_MSGEND:
			a_strMsg += "Invalid End ID";
			break;
		default:
			strTmp.Format(_T("0x%04X"), a_uResp);
			a_strMsg += strTmp;
			break;
		}
	}
	else
	if (a_uResp >= _JTP_RESP_CMD_ERR)
	{
		a_strMsg = "Command Error: ";
		switch (a_uResp)
		{
		case _JTP_RESP_CMD_RSV:
			a_strMsg += "Reserved Command";
			break;
		case _JTP_RESP_CMD_BAD:
			a_strMsg += "Unknown Command";
			break;
		default:
			strTmp.Format(_T("0x%04X"), a_uResp);
			a_strMsg += strTmp;
			break;
		}
	}
	else
	if (a_uResp >= _JTP_RESP_BAD_PARA)
	{
		a_strMsg = "Parameter Error: ";

		if ( a_uResp & (_JTP_RESP_BAD_PARA&0x000F) )
		{
			a_strMsg += "uPara";
		}
		if ( a_uResp & (_JTP_RESP_BAD_DATALEN&0x000F) )
		{
			a_strMsg += "DataLen";
		}
		if ( a_uResp & (_JTP_RESP_BAD_DATA&0x000F) )
		{
			a_strMsg += "Data";
		}
	}
	else
	{
		a_strMsg = "Run Time Error: ";
		strTmp.Format(_T("0x%04X"), a_uResp);
		a_strMsg += strTmp;
	}
}

BOOL JTP_Com::SendMsg_Connect(int uWaitDelay)			// 楷搬 犬牢
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_Connect() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_HUB, _JTP_CMD_CONNECT, 0);
	JTP_TxMakeChkSumFullFrame(pTx);

	// Modify neo. 2012.02.06
	if ( TransferMsg(pTx, uWaitDelay) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_Buffsize()		  // Receivable Data Size
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_DevInfoGet(void *a_pSysInfoData, int *a_puDataLen)		// Get Device Info (*林2)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_DevInfoGet() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_HUB, _JTP_CMD_DEV_INFO_GET, 0);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		if (a_pSysInfoData != NULL)
		{
			memcpy(a_pSysInfoData, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
		}

		if( a_puDataLen != NULL )
		{
			*a_puDataLen = (int)m_pRxBuff->uDataLen;
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_GetMAVersion(int a_uMode, void *a_pData)
{
	JTP_FRAME *pTx;
	CString strMsg;
	
	m_strMsg = "SendMsg_GetGPSInfo() ";
	
	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_MA_SEND_VER, (U16)a_uMode);
	JTP_TxMakeChkSumFullFrame(pTx);
	
	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}
	
	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;
	
	if (m_uRespMsg == _JTP_RESP_OK)
	{
		memcpy(a_pData, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_MftCfgGet(void *a_pCfgData, int *a_puDataLen)		 // Get Manufacturer Config
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_MftCfgGet() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_MFT_CFG_GET, 0);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		if (a_pCfgData != NULL) memcpy(a_pCfgData, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
		if (a_puDataLen!= NULL) *a_puDataLen = (int)m_pRxBuff->uDataLen;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_MftCfgSet(const void *a_pCfgData, int a_uDataLen)		 // Set Manufacturer Config
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_MftCfgSet() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, a_uDataLen);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_MFT_CFG_SET, 0);
	memcpy( JTP_pData(pTx), a_pCfgData, a_uDataLen);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_UserCfgGet(void *a_pCfgData, int *a_puDataLen)		// Get User Config
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_UserCfgGet() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_USER_CFG_GET, 0);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		if (a_pCfgData != NULL) memcpy(a_pCfgData, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
		if (a_puDataLen!= NULL) *a_puDataLen = (int)m_pRxBuff->uDataLen;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_UserCfgSet(const void *a_pCfgData, int a_uDataLen)		// Set User Config
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_UserCfgSet() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, a_uDataLen);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_USER_CFG_SET, 0);
	memcpy( JTP_pData(pTx), a_pCfgData, a_uDataLen);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )//////////////////////
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_UserReset()		// // Init for User
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_UserReset() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_USER_RESET, 0);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 5000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_UserFwUpdateStart(int a_uDataLen, int nPara)		//* User Firmware Update
{
	JTP_FRAME *pTx;
	CString strMsg;

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, sizeof(int));
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_DOWNLOAD_FIRMWARE, nPara);
	memcpy( JTP_pData(pTx), &a_uDataLen, sizeof(int));
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )//////////////////////
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_UserFwUpdateData(void *a_pData, int a_uDataLen, int a_uIndex)		//* User Firmware Update
{
	JTP_FRAME *pTx;
	CString strMsg;

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, a_uDataLen);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_DOWNLOAD_FIRMWARE, 0x8000 | a_uIndex);
	memcpy( JTP_pData(pTx), (char *)a_pData, a_uDataLen);

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )//////////////////////
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);

	return TRUE;
}

BOOL JTP_Com::SendMsg_UserFwUpdateEnd(int nPara)		//* User Firmware Update End
{
	JTP_FRAME *pTx;
	CString strMsg;

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_DOWNLOAD_FIRMWARE, nPara);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )//////////////////////
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	if (m_pRxBuff->uPara == 0)
	{
		CvtRespMessage(strMsg, m_uRespMsg);
	}

	return TRUE;
}

BOOL JTP_Com::SendMsg_UserFwUpdateRequest(int nPara)		//* User Firmware Update Request
{
	JTP_FRAME *pTx;
	CString strMsg;

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_DOWNLOAD_FIRMWARE, nPara);
	JTP_TxMakeChkSumFullFrame(pTx);

	for (;;)
	{
		if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )//////////////////////
		{
			CvtRespMessage(strMsg, m_uRespTrans);
		}

		m_uRespMsg = (int)m_pRxBuff->uPara;
		if (m_pRxBuff->uPara == 0)
		{
			CvtRespMessage(strMsg, m_uRespMsg);
			break;
		}
	}

	return TRUE;
}

BOOL JTP_Com::SendMsg_ModeSet(int a_uNewMode, int *a_puCurMode)	// Set Run-Time Mode (*林3)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_ModeSet() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_MODE_SET, a_uNewMode);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	if (a_uNewMode == 0)
	{
		*a_puCurMode = m_uRespMsg;
		strMsg.Format(_T("Current Run Mode = 0x%04X"), m_uRespMsg);
		m_strMsg += strMsg;
		return TRUE;
	}

	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_BboxEnable(BOOL a_tOn)		// Enable Blackbox (*林4)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_BboxEnable() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_BBOX_ENABLE, a_tOn);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_MainCodeGet(int *a_puAddr)	  // Get Main Code Addr
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_MainCodeGet() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_MAIN_CODE_GET, 0);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		*a_puAddr = *(U32*)JTP_pData(m_pRxBuff);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_HwTest(U32 *a_puHwTestReport)			 // Run H/W Test (*林5)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_MainCodeGet() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_HW_TEST, 0);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg < _JTP_RESP_ERROR)
	{
		if (a_puHwTestReport != NULL) *a_puHwTestReport = *(U32*)JTP_pData(m_pRxBuff);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_UninstalledSet()  // Set Uninstalled State
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}


BOOL JTP_Com::SendMsg_NorInfoName(CString &a_strDeviceName)			// NOR Flash Info (*林6)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_NorInfoName() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_NOR_INFO, 0);

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
	return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		a_strDeviceName = (char*)JTP_pData(m_pRxBuff);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_NorInfoID(int *a_uID)			// NOR Flash Info (*林6)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_NorInfoID() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_NOR_INFO, 1);

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		*a_uID = *(U16*)JTP_pData(m_pRxBuff);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_NorInfoSize(int *a_uSize)			// NOR Flash Info (*林6)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_NorInfoSize() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_NOR_INFO, 2);

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		*a_uSize = *(U32*)JTP_pData(m_pRxBuff);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_NorInfoNoOfSects(int *a_uNoOfSects)			// NOR Flash Info (*林6)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_NorInfoNoOfSects() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_NOR_INFO, 3);

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		*a_uNoOfSects = *(U16*)JTP_pData(m_pRxBuff);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_NorInfoSect(int a_uSectNum, int *a_uStartAddr, int *a_uEndAddr)			// NOR Flash Info (*林6)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_NorInfoSect() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, sizeof(U16));
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_NOR_INFO, 4);

	*(U16*)JTP_pData(pTx) = (U16)a_uSectNum;

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		*a_uStartAddr = *(U32*)( (U08*)JTP_pData(m_pRxBuff) + 0 );
		*a_uEndAddr	= *(U32*)( (U08*)JTP_pData(m_pRxBuff) + sizeof(U32) );
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_NorRead(int a_uAddr, void *a_pData, int a_uLen)			// NOR Flash Read
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_NorRead() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, sizeof(U32)+sizeof(U16));
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_NOR_READ, 0);

	*(U32*)( (U08*)JTP_pData(pTx)+0			 ) = a_uAddr;
	*(U16*)( (U08*)JTP_pData(pTx)+sizeof(U32) ) = a_uLen;

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		if (a_pData != NULL)
		{
			memcpy(a_pData, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_NorWrite(int a_uAddr, void *a_pData, int a_uLen)		  // NOR Flash Write
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_NorWrite() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, sizeof(U32)+a_uLen);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_NOR_WRITE, 0);

	*(U32*)( (U08*)JTP_pData(pTx)+0			  ) = a_uAddr;
	memcpy( ( (U08*)JTP_pData(pTx)+sizeof(U32) ), a_pData, a_uLen );

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 4000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_NorErase(int a_uAddr)		  // NOR Flash Erase
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_NorErase() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, sizeof(U32));
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_NOR_ERASE, 0);
	*(U32*)JTP_pData(pTx) = a_uAddr;
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 4000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

///////////////////////////////////////////////////////////////////////////////////////
BOOL JTP_Com::SendMsg_SpifInfoName(CString &a_strDeviceName)			// SPI Flash Info (*林6)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_SpifInfoName() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_SPI_INFO, 0);

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		a_strDeviceName = (char*)JTP_pData(m_pRxBuff);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_SpifInfoID(int *a_uID)			// SPI Flash Info (*林6)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_SpifInfoID() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_SPI_INFO, 1);

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		*a_uID = *(U16*)JTP_pData(m_pRxBuff);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_SpifInfoSize(int *a_uSize)			// SPI Flash Info (*林6)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_SpifInfoSize() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_SPI_INFO, 2);

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		*a_uSize = *(U32*)JTP_pData(m_pRxBuff);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_SpifInfoNoOfSects(int *a_uNoOfSects)			// SPI Flash Info (*林6)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_SpifInfoNoOfSects() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_SPI_INFO, 3);

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		*a_uNoOfSects = *(U16*)JTP_pData(m_pRxBuff);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_SpifInfoSect(int a_uSectNum, int *a_uStartAddr, int *a_uEndAddr)			// SPI Flash Info (*林6)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_SpifInfoSect() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, sizeof(U16));
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_SPI_INFO, 4);

	*(U16*)JTP_pData(pTx) = (U16)a_uSectNum;

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		*a_uStartAddr = *(U32*)( (U08*)JTP_pData(m_pRxBuff) + 0			  );
		*a_uEndAddr	= *(U32*)( (U08*)JTP_pData(m_pRxBuff) + sizeof(U32) );
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_SpifRead(int a_uAddr, void *a_pData, int a_uLen)			// SPI Flash Read
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_SpifRead() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, sizeof(U32)+sizeof(U16));
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_SPI_READ, 0);

	*(U32*)( (U08*)JTP_pData(pTx)+0			  ) = a_uAddr;
	*(U16*)( (U08*)JTP_pData(pTx)+sizeof(U32) ) = a_uLen;

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		if (a_pData != NULL)
		{
			memcpy(a_pData, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_SpifWrite(int a_uAddr, void *a_pData, int a_uLen)		  // SPI Flash Write
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_SpifWrite() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, sizeof(U32)+a_uLen);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_SPI_WRITE, 0);

	*(U32*)( (U08*)JTP_pData(pTx)+0			  ) = a_uAddr;
	memcpy( ( (U08*)JTP_pData(pTx)+sizeof(U32) ), a_pData, a_uLen );

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 4000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_SpifErase(int a_uAddr)		  // SPI Flash Erase
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_SpifErase() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, sizeof(U32));
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_SPI_ERASE, 0);
	*(U32*)JTP_pData(pTx) = a_uAddr;
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 4000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

///////////////////////////////////////////////////////////////////////////////////////
BOOL JTP_Com::SendMsg_NandInfoName(CString &a_strDeviceName)	  // NAND Flash Info (*林7)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_NandInfoName() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_NAND_INFO, 0);

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		a_strDeviceName = (char*)JTP_pData(m_pRxBuff);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_NandInfoID(int *a_lID)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_NandInfoID() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_NAND_INFO, 1);

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		*a_lID = *(U32*)JTP_pData(m_pRxBuff);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_NandInfoSize(int *a_lSize)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_NandInfoSize() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_NAND_INFO, 2);

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		*a_lSize = *(U32*)JTP_pData(m_pRxBuff);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_NandInfoNoOfBlocks(int *a_uNoOfBlocks)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_NandInfoNoOfBlocks() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_NAND_INFO, 3);

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		*a_uNoOfBlocks = *(U16*)JTP_pData(m_pRxBuff);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_NandInfoPagesOfBlock(int *a_uPagesOfBlock)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_NandInfoPagesOfBlock() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_NAND_INFO, 4);

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		*a_uPagesOfBlock = *(U16*)JTP_pData(m_pRxBuff);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_NandInfoBytesOfPage(int *a_uBytesOfPage)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_NandInfoBytesOfPage() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_NAND_INFO, 5);

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		*a_uBytesOfPage = *(U16*)JTP_pData(m_pRxBuff);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_NandRead(int a_lAddr, void *a_pData, int a_uLen)				 // NAND Flash Read(SW)
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_NandWrite(int a_lAddr, void *a_pData, int a_uLen)				 // NAND Flash Write(SW)
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_NandErase(int a_lAddr)													 // NAND Flash Erase(SW)
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

// a_nFormatType == 0 -> Normal Format
//					== 1 -> Hard	Format
BOOL JTP_Com::SendMsg_NandFormat(int a_nFormatType)	// NAND Flash Format(SW)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_NandFormat() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_NAND_FORMAT, (U16)a_nFormatType);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 120000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_NandHwRead(int a_uSeg, int a_uOff, void *a_pData, int a_uLen)  // NAND Flash Read(HW)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_NandHwRead() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, sizeof(U32)+sizeof(U16));
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_NAND_HW_READ, 0);

	*(U32*)( (U08*)JTP_pData(pTx)+0			  ) = (a_uSeg << 16) | (a_uOff & 0x0000FFFF);
	*(U16*)( (U08*)JTP_pData(pTx)+sizeof(U32) ) = a_uLen;

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		if (a_pData != NULL)
		{
			memcpy(a_pData, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_NandHwWrite(int a_uSeg, int a_uOff, void *a_pData, int a_uLen)  // NAND Flash Write(HW)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_NandHwWrite() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, sizeof(U32)+a_uLen);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_NAND_HW_WRITE, 0);

	*(U32*)( (U08*)JTP_pData(pTx)+0			  ) = (a_uSeg << 16) | (a_uOff & 0x0000FFFF);
	memcpy( ( (U08*)JTP_pData(pTx)+sizeof(U32) ), a_pData, a_uLen );

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 4000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_NandHwErase(int a_uSeg)													  // NAND Flash Erase(HW)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_NandHwErase() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, sizeof(U16));
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_NAND_HW_ERASE, 0);

	*(U16*)( (U08*)JTP_pData(pTx)+0			  ) = a_uSeg;

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 4000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

///////////////////////////////////////////////////////////////////////////////////////
BOOL JTP_Com::SendMsg_RamRead()			// RAM Read
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_RamWrite()		  // RAM Write
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}


BOOL JTP_Com::SendMsg_I2cRead(int a_bDevID, int a_bStartReg, void *a_pData, int a_bLen)	// I2C(8bits) Read
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_I2cRead() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, sizeof(U08)+sizeof(U08));
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_I2C_READ, a_bDevID);

	*(U08*)( (U08*)JTP_pData(pTx)+0 ) = (U08)a_bStartReg;
	*(U08*)( (U08*)JTP_pData(pTx)+1 ) = (U08)a_bLen;

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 2000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		if (a_pData != NULL)
		{
			memcpy(a_pData, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_I2cWrite(int a_bDevID, int a_bStartReg, void *a_pData, int a_bLen)  // I2C(8bits) Write
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_I2cWrite() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, sizeof(U08)+a_bLen);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_I2C_WRITE, a_bDevID);

	*(U08*)( (U08*)JTP_pData(pTx)+0 ) = a_bStartReg;
	memcpy( ( (U08*)JTP_pData(pTx)+sizeof(U08) ), a_pData, a_bLen );

	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 2000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_Say2(int *a_arData, int a_nDataLen, BOOL a_tNew)				// U32 Say2(S32*, U32)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_Say2() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, a_nDataLen);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_SAY2, a_tNew);
	memcpy(JTP_pData(pTx), a_arData, a_nDataLen);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg < _JTP_RESP_ERROR)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_SndPut(int a_nSndNum, BOOL a_tNew)			 // void SndPut(U16, bool)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_SndPut() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, sizeof(U16));
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_SND_PUT, a_tNew);
	*(U16*)JTP_pData(pTx) = (U16)a_nSndNum;
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg < _JTP_RESP_ERROR)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_SndGap(int a_nGapLen, BOOL a_tNew)			 // void SndGap(U16, bool)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_SndGap() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, sizeof(U16));
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_SND_GAP, a_tNew);
	*(U16*)JTP_pData(pTx) = (U16)a_nGapLen;
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg < _JTP_RESP_ERROR)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_SndStop()			// void SndStop()
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_SndStop() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_SND_GAP, 0);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg < _JTP_RESP_ERROR)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL JTP_Com::SendMsg_LedPut(int a_uLedState)			 // LED Put
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_LedPut() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_LED_PUT, a_uLedState);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 100) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_OledPut()			// OLED Put
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_OledPutCmd()		// OLED Put Cmd
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_OledGetRgb()		// OLED Get RGB
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_OledPutRgb()		// OLED Put RGB
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}


BOOL JTP_Com::SendMsg_BboxGetHeaders(void *a_parBboxSaveHeader, int *a_puDataLen) // Get Blackbox Headers at Send Mode
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_BboxGetHeaders() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_BBOX_SEND_HEADERS, 0);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg < _JTP_RESP_ERROR)
	{
		if (a_parBboxSaveHeader != NULL) memcpy(a_parBboxSaveHeader, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
		if (a_puDataLen != NULL) *a_puDataLen = m_pRxBuff->uDataLen;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_BboxSelect(int a_uSelNum, void *a_pBboxSaveHeader)  // Select Blackbox Block at Send Mode
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_BboxSelect() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_BBOX_SEND_SELECT, a_uSelNum);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg < _JTP_RESP_ERROR)
	{
		if (a_pBboxSaveHeader != NULL) memcpy(a_pBboxSaveHeader, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_BboxGetData(void *a_pJgpImg, int *a_puJpgSize)	 // Get Next Blackbox Data at Send Mode
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_BboxGetData() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_BBOX_SEND_DATA, 0);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg < _JTP_RESP_ERROR)
	{
		if (m_pRxBuff->uDataLen > 0)
		{
			if (a_pJgpImg	!= NULL) memcpy(a_pJgpImg, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
			if (a_puJpgSize != NULL) *a_puJpgSize = (int)m_pRxBuff->uDataLen;
		}
		else
		{
			if (a_puJpgSize != NULL) *a_puJpgSize = 0;
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_BboxCheck(int *a_puSavedBbox)  // Get no. of Saved Blackbox
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_BboxCheck() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_BBOX_SEND_DATA, 0);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg < _JTP_RESP_ERROR)
	{
		if (a_puSavedBbox != NULL) *a_puSavedBbox = (int)m_pRxBuff->uPara;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL JTP_Com::SendMsg_SshotSendHeaders()// Get Blackbox Headers at Send Mode
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_SshotSendData()	// Get Next Blackbox Data at Send Mode
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_SshotCheck()		// Get no. of Saved Blackbox
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}


BOOL JTP_Com::SendMsg_RtcGet(int *a_puRtcValue)			 // Get RTC Value
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_RtcGet() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_RTC_GET, 0);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		if (a_puRtcValue != NULL) *a_puRtcValue = *(U32*)JTP_pData(m_pRxBuff);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_RtcSet(int a_uRtcValue)			 // Set RTC Value
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_RtcSet() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, sizeof(U32));
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_RTC_SET, 0);
	*(U32*)JTP_pData(pTx) = a_uRtcValue;
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 2000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_UserLdwsOnGet() // User Get LDWS On/Off
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_UserLdwsOnSet()  // User Set LDWS On/Off
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_UserLkasOnGet()  // User Get LKAS On/Off
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_UserLkasOnSet()  // User Set LKAS On/Off
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_UserG2gOnGet()  // User Get G2G On/Off
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_UserG2gOnSet()  // User Set G2G On/Off
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_UserMuteOnGet()  // User Get Mute On/Off
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_UserMuteOnSet()  // User Set Mute On/Off
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}


BOOL JTP_Com::SendMsg_UserVolGet(int *a_puVolume)  // User Get Volume
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_UserVolGet() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_USER_VOL_GET, 0);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg < _JTP_RESP_ERROR)
	{
		if (a_puVolume != NULL) *a_puVolume = (int)m_pRxBuff->uPara;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_UserVolSet(int a_uVolume)  // User Set Volume
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_UserVolSet() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_USER_VOL_SET, a_uVolume);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg < _JTP_RESP_ERROR)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL JTP_Com::SendMsg_UserRunModeGet()  // User Get Run Mode
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_UserRunModeSet()  // User Set Run Mode
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}


BOOL JTP_Com::SendMsg_UserCarTypeGet()  // User Get Car Type
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_UserCarTypeSet()  // User Set Car Type
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_UserLdwsLLvlGet()  // User Get LDWS L-Level
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_UserLdwsLLvlSet()  // User Set LDWS L-Level
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_UserLdwsRLvlGet()  // User Get LDWS R-Level
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_UserLdwsRLvlSet()  // User Set LDWS R-Level
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_UserLdwsWarnGet()  // User Get LDWS Warning
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_UserLdwsWarnSet()  // User Set LDWS Warning
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}


BOOL JTP_Com::SendMsg_UserBboxSave()  // User Save Blackbox Data
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_UserBboxDel(int a_iTgt)  // User Delete Blackbox Data
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_UserBboxDel() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_USER_BBOX_DEL, 0xFFFF);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 10000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg < _JTP_RESP_ERROR)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL JTP_Com::SendMsg_UserSshotSave()  // User Save Snap-Shot
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_UserSshotDel()  // User Delete Snap-Shot
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}


BOOL JTP_Com::SendMsg_MonitorVideo(void *a_pData, int *a_puDataLen)	// Get Video Monitor
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_MonitorVideo() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_MONITOR_VIDEO | JTP_CMD_NO_CHKSUM, 0);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 2000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		if (a_pData != NULL) memcpy(a_pData, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
		if (a_puDataLen != NULL) *a_puDataLen = m_pRxBuff->uDataLen;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_MonitorData()	// Get Data Monitor
{
	m_uRespTrans = _JTP_RESP_CMD_RSV;
	m_uRespMsg   = _JTP_RESP_ERROR;
	return FALSE;
}

BOOL JTP_Com::SendMsg_ControlVibrator(int a_uOnOff)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_ControlVibrator() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_CTRL_VIBRATOR, a_uOnOff);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg < _JTP_RESP_ERROR)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_ControlHighBeam(int a_uOnOff)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_ControlHighBeam() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_CTRL_HIGHBEAM, a_uOnOff);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg < _JTP_RESP_ERROR)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_SendVirtualSpeed(int a_uSpeed)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_SendVirtualSpeed() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_SEND_VIRTUAL_SPEED, a_uSpeed);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg < _JTP_RESP_ERROR)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_SendVirtualSignal(const void *a_pCfgData, int a_uDataLen)		 // Set Virtual Signal
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_SendVirtualSignal() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, a_uDataLen);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_SEND_VIRTUAL_SIGNAL, 0);
	memcpy( JTP_pData(pTx), a_pCfgData, a_uDataLen);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL JTP_Com::SendMsg_MainDataGet(void *a_pCfgData, int *a_puDataLen)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_MainDataGet() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_DRIVE_STATE_GET, 0);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		if (a_pCfgData != NULL) memcpy(a_pCfgData, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
		if (a_puDataLen!= NULL) *a_puDataLen = (int)m_pRxBuff->uDataLen;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_GetBTInfo(void *a_pData)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_GetBTInfo() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_BLUETOOTH_INFO_GET, 0);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		memcpy(a_pData, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_GetGPSInfo(void *a_pData)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_GetGPSInfo() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_GPS_DATA_GET, 0);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		memcpy(a_pData, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL JTP_Com::SendMsg_FactoryModeStart(void)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_FactoryModeStart() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_MA, JTP_ID_PC, _JTP_CMD_MA_FACTORY_START, 0);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	printf("%s\n", m_strMsg);
	if (m_uRespMsg == _JTP_RESP_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL JTP_Com::SendMsg_FactoryModeEnd(int a_uMode, void *a_pData, int *a_puDataLen)
{
	JTP_FRAME *pTx;
	CString strMsg;
	int i = 0;

	m_strMsg = "SendMsg_FactoryModeEnd() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_MA, JTP_ID_PC, _JTP_CMD_MA_FACTORY_END, (U16)a_uMode);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	// debug
	printf("FactoryMode Rcv : ");
	printf("Para[%2x] Data[%2x]\n", m_uRespMsg, (int)m_pRxBuff->uChkSum);

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		if (a_pData != NULL) memcpy(a_pData, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
		if (a_puDataLen != NULL) *a_puDataLen = m_pRxBuff->uDataLen;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL JTP_Com::SendMsg_InstallModeStart(int a_uModeLevel, void *a_pData)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_InstallModeStart() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_MA_INSTALL_START, a_uModeLevel);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		memcpy(a_pData, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL JTP_Com::SendMsg_InstallModeEnd(int a_uMode)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_InstallModeEnd() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_MA_INSTALL_END, (U16)a_uMode);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


// Get BF Version Info
BOOL JTP_Com::SendMsg_GetBFversionInfo(void *a_pData, int *a_puDataLen)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_GetBFversionInfo() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_MA, JTP_ID_PC, _JTP_CMD_MA_SEND_VER, 0);
	JTP_TxMakeChkSumFullFrame(pTx);
	
	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;
	
	if (m_uRespMsg == _JTP_RESP_OK)
	{
		if (a_pData != NULL) memcpy(a_pData, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
		if (a_puDataLen != NULL) *a_puDataLen = m_pRxBuff->uDataLen;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// Set MA LIN information to BF
BOOL JTP_Com::SendMsg_SetMALINinfo(void *a_pData, int a_puDataLen)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_GetBFversionInfo() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, a_puDataLen);
	JTP_TxMakeMsg(pTx, JTP_ID_MA, JTP_ID_PC, _JTP_CMD_MA_GET_DATA, 0);
	memcpy(JTP_pData(pTx), a_pData, a_puDataLen);
	JTP_TxMakeChkSumFullFrame(pTx);


	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;
	
	if (m_uRespMsg == _JTP_RESP_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// Set BF Alert to MA Display
BOOL JTP_Com::SendMsg_SetBFalertInfo(void *a_pData, int *a_puDataLen)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_GetBFversionInfo() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_MA, JTP_ID_PC, _JTP_CMD_MA_SEND_DATA, 0);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;
	
	if (m_uRespMsg == _JTP_RESP_OK)
	{
		if (a_pData != NULL) memcpy(a_pData, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
		if (a_puDataLen != NULL) *a_puDataLen = m_pRxBuff->uDataLen;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL JTP_Com::SendMsg_PowerBtnChk(void)
{
	JTP_FRAME *pTx;
	CString strMsg;

	m_strMsg = "SendMsg_PowerBtnChk() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
	JTP_TxMakeMsg(pTx, JTP_ID_MA, JTP_ID_PC, _JTP_CMD_MA_INSTALL_STATUS, 0);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL JTP_Com::SendMsg_ADASInfoGet(void *a_pAdasInfoData, int *a_puDataLen)		// Get ADAS information (*林15)
{
	JTP_FRAME *pTx;
	CString strMsg;
	Input_COMM oInput_COMM[1];
	int a_uDataLen = sizeof(Input_COMM);

	memset(oInput_COMM, 0, sizeof(Input_COMM));
	oInput_COMM->uVehicleSpeed = 100;

	m_strMsg = "SendMsg_DevInfoGet() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, a_uDataLen);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_ADAS_INFO_GET, 0);
	memcpy( JTP_pData(pTx), oInput_COMM, a_uDataLen);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		if (a_pAdasInfoData != NULL)
		{
			memcpy(a_pAdasInfoData, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
		}

		if( a_pAdasInfoData != NULL )
		{
			*a_puDataLen = (int)m_pRxBuff->uDataLen;
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

#define ADC_VOLT_TURN 500
BOOL JTP_Com::SendMsg_ADASInfoGet_R7(void *a_pAdasInfoData, int *a_puDataLen, int nInSpeed, bool bInTurnL, bool bInTurnR)		// Get ADAS information (*林15)
{
	JTP_FRAME *pTx;
	CString strMsg;
	HUB_STAT3 oInput_COMM[1];
	int a_uDataLen = sizeof(HUB_STAT3);

	memset(oInput_COMM, 0, sizeof(HUB_STAT3));
	oInput_COMM->uVssTick = nInSpeed;
	if ( bInTurnL != 0 )	oInput_COMM->uADTurnL = ADC_VOLT_TURN;
	if ( bInTurnR != 0 )	oInput_COMM->uADTurnR = ADC_VOLT_TURN;

	m_strMsg = "SendMsg_DevInfoGet_R7() ";

	pTx = JTP_TxMakeFullFrame(m_pTxBuff, a_uDataLen);
	JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_ADAS_INFO_GET, 0);
	memcpy( JTP_pData(pTx), oInput_COMM, a_uDataLen);
	JTP_TxMakeChkSumFullFrame(pTx);

	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}

	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;

	if (m_uRespMsg == _JTP_RESP_OK)
	{
		if (a_pAdasInfoData != NULL)
		{
			memcpy(a_pAdasInfoData, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
		}

		if( a_pAdasInfoData != NULL )
		{
			*a_puDataLen = (int)m_pRxBuff->uDataLen;
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL JTP_Com::SendMsg_GetDTCData(int a_uParam, int a_uLength, void *a_pGetData, int *a_puDataLen)
{
	JTP_FRAME *pTx;
	CString strMsg;
	
	m_strMsg = "SendMsg_MainDataGet() ";

	if ( a_uParam == 0 )
	{
		pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
		JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_GET_DTC_DATA, (U16)a_uParam);
		*(U16*)JTP_pData(pTx) = a_uLength;
		JTP_TxMakeChkSumFullFrame(pTx);
	}
	else if ( a_uParam == 1 )
	{
		pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
		JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_GET_DTC_DATA, (U16)a_uParam);
		*(U16*)JTP_pData(pTx) = a_uLength;
		JTP_TxMakeChkSumFullFrame(pTx);
	}
	else if ( a_uParam == 2 )
	{
		pTx = JTP_TxMakeFullFrame(m_pTxBuff, sizeof(U16));
		JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_GET_DTC_DATA, (U16)a_uParam);
		*(U16*)JTP_pData(pTx) = a_uLength;
		JTP_TxMakeChkSumFullFrame(pTx);
	}
	else
	{
		pTx = JTP_TxMakeFullFrame(m_pTxBuff, 0);
		JTP_TxMakeMsg(pTx, JTP_ID_BF, JTP_ID_PC, _JTP_CMD_GET_DTC_DATA, (U16)a_uParam);
		JTP_TxMakeChkSumFullFrame(pTx);
	}
	
	if ( TransferMsg(pTx, 1000) != _JTP_RESP_OK )
	{
		CvtRespMessage(strMsg, m_uRespTrans);
		m_strMsg += strMsg;
		return FALSE;
	}
	
	m_uRespMsg = (int)m_pRxBuff->uPara;
	CvtRespMessage(strMsg, m_uRespMsg);
	m_strMsg += strMsg;
	
	if (m_uRespMsg == _JTP_RESP_OK)
	{
		if ( a_uParam == 0 )
		{
			if (a_pGetData != NULL) memcpy(a_pGetData, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
		}
		else if ( a_uParam == 1 )
		{
			if (a_puDataLen != NULL) memcpy(a_puDataLen, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
		}
		else if ( a_uParam == 2 )
		{
			if (a_pGetData != NULL) memcpy(a_pGetData, JTP_pData(m_pRxBuff), m_pRxBuff->uDataLen);
			if (a_puDataLen!= NULL) *a_puDataLen = (int)m_pRxBuff->uDataLen;
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}