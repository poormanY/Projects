#include "stdafx.h"
#include "resource.h"
#include "EOL_TEST_R7.h"
#include "R7EOLvs2010Dlg.h"
#include "SettingDlg.h"
#include <mmsystem.h>	// sndPlaySound

#include "y_IoBoardMgt.h"
#include "y_BarcodeMgt.h"
#include "y_Debugging.h"
#include "y_Color.h"
#include "y_JTP_Com.h"

extern CR7EOLvs2010Dlg	*pMainDlg;
extern CEOL_R7		*EOL_TEST_R7;

static CIoBoardMgt	*pIoBoard = NULL;		// IO Board 클래스
static CBarcodeMgt	*pBarCode1 = NULL;		// Barcode1 클래스
static CBarcodeMgt	*pBarCode2 = NULL;		// Barcode2 클래스
static JTP_Com		m_JTP_Com;				// JTP Protocol 클래스

CEOL_R7::CEOL_R7()
{
	m_tThreadStop = false;
	bLogStart = false;
}

CEOL_R7::~CEOL_R7()
{
	m_tThreadStop = true;
}


BOOL CEOL_R7::START_EOL_TEST(void)
{	
	// Init
	//m_nVehicleLdwSpd = 


    // New Start Thread
    m_tThreadStop = false;
    m_pThread = AfxBeginThread(ThreadEolTest_R7, this);

    if (m_pThread == NULL)
    {
        END_EOL_TEST();
        return false;
    }

    return true;
}

BOOL CEOL_R7::END_EOL_TEST()
{
	// ADAS 테스트 종료
	m_tThreadStop = true;
	
	return TRUE;
}


UINT ThreadEolTest_R7(LPVOID pParam)
{
	pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, pMainDlg->strProductName + pMainDlg->EOLLanguage.LP_TEST_WAITING, UPDATE_ON);

	CString strProcess;
	int nTotalProcess = 0;
	int nCurProcess = 1;
	int nPassNum = 0, nFailNum = 0, nTotalTest;
	CString strPassNum = _T("0"), strFailNum = _T("0"), strTotalNum, strPassPercent, strTotalTest;

	EOL_TEST_R7->m_EOLTestResult = PASS;

	// ADAS 입력 데이터
	EOL_TEST_R7->m_nVehicleSpeed = 0;
	EOL_TEST_R7->m_bWinkL = 0;
	EOL_TEST_R7->m_bWinkR = 0;
	
    pMainDlg->m_list_test_process.DeleteAllItems();	// 리스트 모든 아이템 지우기
	pMainDlg->m_edit_pass_num.SetText(SOFT_BLUE, GREY_ROAD, _T("0"), UPDATE_ON);			// 양품갯수 표시
	pMainDlg->m_edit_fail_num.SetText(SOFT_BLUE, GREY_ROAD, _T("0"), UPDATE_ON);			// 불량갯수 표시
	pMainDlg->m_edit_total_num.SetText(SOFT_BLUE, GREY_ROAD, _T("0"), UPDATE_ON);		// 전체갯수 표시
	pMainDlg->m_edit_pass_percent.SetText(SOFT_BLUE, GREY_ROAD, _T("0"), UPDATE_ON);		// 양품비율 표시

	nTotalTest = _ttoi(pMainDlg->m_edit_total_test.m_strMsg);	// 전체 테스트 갯수 가져오기

	pMainDlg->m_edit_in_speed.SetText(SOFT_BLUE, GREY_ROAD, _T("0"), UPDATE_ON);				// 입력속도 표시
	pMainDlg->m_edit_in_wink_l.SetText(SOFT_BLUE, GREY_ROAD, _T("0"), UPDATE_ON);			// 입력방향지시등 표시
	pMainDlg->m_edit_in_wink_r.SetText(SOFT_BLUE, GREY_ROAD, _T("0"), UPDATE_ON);			// 입력방향지시등 표시
	pMainDlg->m_edit_out_ldw_detetc_l.SetText(SOFT_BLUE, GREY_ROAD, _T("0"), UPDATE_ON);		// 차선인식 표시
	pMainDlg->m_edit_out_m_ldw_detect_r.SetText(SOFT_BLUE, GREY_ROAD, _T("0"), UPDATE_ON);	// 차선인식 표시
	pMainDlg->m_edit_out_ldw_warn_l.SetText(SOFT_BLUE, GREY_ROAD, _T("0"), UPDATE_ON);		// 차선경보 표시
	pMainDlg->m_edit_out_ldw_warn_r.SetText(SOFT_BLUE, GREY_ROAD, _T("0"), UPDATE_ON);		// 차선경보 표시
	pMainDlg->m_edit_out_hmw.SetText(SOFT_BLUE, GREY_ROAD, _T("0"), UPDATE_ON);				// HMW경보 표시
	pMainDlg->m_edit_out_fcw.SetText(SOFT_BLUE, GREY_ROAD, _T("0"), UPDATE_ON);				// FCW경보 표시

	// 검사 스텝 수
	if ( pMainDlg->bEnStep_McuVer     == 1 )	nTotalProcess++;
	if ( pMainDlg->bEnStep_SwVer	  == 1 ) 	nTotalProcess++;
	if ( pMainDlg->bEnStep_SerialNum  == 1 )	nTotalProcess++;
	//if ( pMainDlg->bEnStep_IoBoard  == 1 )	nTotalProcess++;
	//if ( pMainDlg->bEnStep_Screw	  == 1 ) 	nTotalProcess++;
	//if ( pMainDlg->bEnStep_Bar1	  == 1 )	nTotalProcess++;
	if ( pMainDlg->bEnStep_Bar2	  	  == 1 )	nTotalProcess++;
	if ( pMainDlg->bEnStep_Accleration== 1 )	nTotalProcess++;
	if ( pMainDlg->bEnStep_SpeedPulse == 1 )	nTotalProcess++;
	if ( pMainDlg->bEnStep_DTC_Clear  == 1 )	nTotalProcess++;

	if ( pMainDlg->bEnStep_LDW_Detect == 1 )	nTotalProcess++;
	if ( pMainDlg->bEnStep_LDW_Warn_L == 1 )	nTotalProcess++;
	if ( pMainDlg->bEnStep_LDW_Warn_R == 1 )	nTotalProcess++;
	if ( pMainDlg->bEnStep_FCW_Warn	  == 1 )	nTotalProcess++;
	if ( pMainDlg->bEnStep_FCW_Warn2	  == 1 )	nTotalProcess++;
	if ( pMainDlg->bEnStep_FCW_Warn3	  == 1 )	nTotalProcess++;
	if ( pMainDlg->bEnStep_FCDA_Warn == 1 )		nTotalProcess++;
	if ( pMainDlg->bEnStep_LDW_WarnL_WinkL == 1 )	nTotalProcess++;
	if ( pMainDlg->bEnStep_LDW_WarnL_WinkR == 1 )	nTotalProcess++;
	if ( pMainDlg->bEnStep_LDW_WarnL_WinkLR == 1 )	nTotalProcess++;
	if ( pMainDlg->bEnStep_LDW_WarnR_WinkL == 1 )	nTotalProcess++;
	if ( pMainDlg->bEnStep_LDW_WarnR_WinkR == 1 )	nTotalProcess++;
	if ( pMainDlg->bEnStep_LDW_WarnR_WinkLR == 1 )	nTotalProcess++;

	// ADAS 이미지 설정
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->CALIBRATION);

	if ( pMainDlg->bEnStep_IoBoard == 1 )
	{
		if ( EOL_TEST_R7->m_tThreadStop == false )
		{
			if ( EOL_TEST_R7->STEP_CONNECT_IOBOARD() == FALSE )
			{
				EOL_TEST_R7->m_tThreadStop = true;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_IO_BOARD + _T("\r\n") + pMainDlg->EOLLanguage.LP_CONNECT_FAIL, UPDATE_ON);
				AfxMessageBox(pMainDlg->EOLLanguage.LP_IO_BOARD + _T("\r\n") + pMainDlg->EOLLanguage.LP_CONNECT_FAIL);
			}
		}

		if ( (EOL_TEST_R7->m_tThreadStop == false)  && (pMainDlg->bEnStep_Bar1) )
		{
			if ( EOL_TEST_R7->STEP_CONNECT_BARCODE1() == FALSE )
			{
				EOL_TEST_R7->m_tThreadStop = true;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_BARCODE1 + _T("\r\n") + pMainDlg->EOLLanguage.LP_CONNECT_FAIL, UPDATE_ON);
				AfxMessageBox(pMainDlg->EOLLanguage.LP_BARCODE1 + _T("\r\n") + pMainDlg->EOLLanguage.LP_CONNECT_FAIL);
			}
		}

		if ( (EOL_TEST_R7->m_tThreadStop == false)  && (pMainDlg->bEnStep_Bar2) )
		{
			if ( EOL_TEST_R7->STEP_CONNECT_BARCODE2() == FALSE )
			{
				EOL_TEST_R7->m_tThreadStop = true;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_BARCODE2 + _T("\r\n") + pMainDlg->EOLLanguage.LP_CONNECT_FAIL, UPDATE_ON);
				AfxMessageBox(pMainDlg->EOLLanguage.LP_BARCODE2 + _T("\r\n") + pMainDlg->EOLLanguage.LP_CONNECT_FAIL);
			}
		}
	}

	// 테스트 로그 기록 시작
	if ( EOL_TEST_R7->m_tThreadStop == false )
	{
		EOL_TEST_R7->START_EOL_TEST_LOG();
		EOL_TEST_R7->bLogStart = true;
	}
	while ( EOL_TEST_R7->m_tThreadStop == false )
	{
		pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_TEST_WAITING, UPDATE_ON);

		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( pMainDlg->bEnStep_IoBoard == 1 )
		{
			if ( EOL_TEST_R7->STEP_CONNECT_POWER() == FALSE )
			{
				EOL_TEST_R7->m_tThreadStop = true;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\n") + pMainDlg->EOLLanguage.LP_POWER + _T("\r\n") + pMainDlg->EOLLanguage.LP_CONNECT_FAIL, UPDATE_ON);
			}
		}

		// 테스트 프로세스 카운트
		nCurProcess = 1;
		EOL_TEST_R7->m_EOLTestResult = PASS;
		// List Item All Clear
		pMainDlg->m_list_test_process.DeleteAllItems();
		
		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( EOL_TEST_R7->m_EOLTestResult == PASS )
		{
			if ( EOL_TEST_R7->STEP_CONNECT_USART() == FALSE )
			{
				EOL_TEST_R7->m_EOLTestResult = FAIL;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\nUSART ") + pMainDlg->EOLLanguage.LP_COMMUNICATION_FAIL, UPDATE_ON);
			}
			else
			{
				strProcess.Format(_T("R7\r\nTesting %d/%d"), nCurProcess++, nTotalProcess);
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, strProcess, UPDATE_ON);
			}
		}

		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( (EOL_TEST_R7->m_EOLTestResult) == PASS && (pMainDlg->bEnStep_SerialNum == 1) )
		{
			if ( EOL_TEST_R7->STEP_CHK_SERIAL() == FALSE )
			{
				EOL_TEST_R7->m_EOLTestResult = FAIL;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\n") + pMainDlg->EOLLanguage.LP_SERIAL_NUMBER + pMainDlg->EOLLanguage.LP_CONFIRM_FAIL, UPDATE_ON);
			}
			else
			{
				strProcess.Format(_T("R7\r\nTesting %d/%d"), nCurProcess++, nTotalProcess);
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, strProcess, UPDATE_ON);
			}
		}

		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( (EOL_TEST_R7->m_EOLTestResult) == PASS && (pMainDlg->bEnStep_SwVer == 1) )
		{
			if ( EOL_TEST_R7->STEP_CHK_SW_VERSION() == FALSE )
			{
				EOL_TEST_R7->m_EOLTestResult = FAIL;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\n") + pMainDlg->EOLLanguage.LP_SOFTWARE_VERSION + pMainDlg->EOLLanguage.LP_CONFIRM_FAIL, UPDATE_ON);
			}
			else
			{
				strProcess.Format(_T("R7\r\nTesting %d/%d"), nCurProcess++, nTotalProcess);
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, strProcess, UPDATE_ON);
			}
		}
		
		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( (EOL_TEST_R7->m_EOLTestResult) == PASS && (pMainDlg->bEnStep_McuVer == 1) )
		{
			if ( EOL_TEST_R7->STEP_CHK_MCU_VERSION() == FALSE )
			{
				EOL_TEST_R7->m_EOLTestResult = FAIL;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\nMCU") + pMainDlg->EOLLanguage.LP_MCU_VERSION + pMainDlg->EOLLanguage.LP_CONFIRM_FAIL, UPDATE_ON);
			}
			else
			{
				strProcess.Format(_T("R7\r\nTesting %d/%d"), nCurProcess++, nTotalProcess);
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, strProcess, UPDATE_ON);
			}
		}
		
		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( (EOL_TEST_R7->m_EOLTestResult == PASS) && (pMainDlg->bEnStep_Accleration == 1) )
		{
			if ( EOL_TEST_R7->STEP_CHK_VANISH() == FALSE )
			{
				EOL_TEST_R7->m_EOLTestResult = FAIL;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\n") + pMainDlg->EOLLanguage.LP_ACCELERATION + pMainDlg->EOLLanguage.LP_CONFIRM_FAIL, UPDATE_ON);
			}
			else
			{
				strProcess.Format(_T("R7\r\nTesting %d/%d"), nCurProcess++, nTotalProcess);
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, strProcess, UPDATE_ON);
			}
		}
		
		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( (EOL_TEST_R7->m_EOLTestResult == PASS) && (pMainDlg->bEnStep_SpeedPulse == 1) )
		{
			if ( EOL_TEST_R7->STEP_CHK_SPEED_PULSE() == FALSE )
			{
				EOL_TEST_R7->m_EOLTestResult = FAIL;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\n") + pMainDlg->EOLLanguage.LP_SPEED_PULSE + pMainDlg->EOLLanguage.LP_CONFIRM_FAIL, UPDATE_ON);
			}
			else
			{
				strProcess.Format(_T("R7\r\nTesting %d/%d"), nCurProcess++, nTotalProcess);
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, strProcess, UPDATE_ON);
			}
		}

		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( (EOL_TEST_R7->m_EOLTestResult == PASS) && (pMainDlg->bEnStep_LDW_Detect == 1) )
		{
			if ( EOL_TEST_R7->STEP_ADAS_LDW_DETECT() == FALSE )
			{
				EOL_TEST_R7->m_EOLTestResult = FAIL;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\n") + pMainDlg->EOLLanguage.LP_LANE_DETECT_FAIL, UPDATE_ON);
			}
			else
			{
				strProcess.Format(_T("R7\r\nTesting %d/%d"), nCurProcess++, nTotalProcess);
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, strProcess, UPDATE_ON);
			}
		}
		
		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( (EOL_TEST_R7->m_EOLTestResult == PASS) && (pMainDlg->bEnStep_LDW_Warn_L == 1) )
		{
			if ( EOL_TEST_R7->STEP_ADAS_LDW_LEFT_WARN() == FALSE )
			{
				EOL_TEST_R7->m_EOLTestResult = FAIL;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\n") + pMainDlg->EOLLanguage.LP_LANE_WARNING_L_FAIL, UPDATE_ON);
			}
			else
			{
				strProcess.Format(_T("R7\r\nTesting %d/%d"), nCurProcess++, nTotalProcess);
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, strProcess, UPDATE_ON);
			}
			// 경보 검사 간의 딜레이
			Sleep(pMainDlg->m_nAlarmDelay);
		}
		
		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( (EOL_TEST_R7->m_EOLTestResult == PASS) && (pMainDlg->bEnStep_LDW_Warn_R == 1) )
		{
			if ( EOL_TEST_R7->STEP_ADAS_LDW_RIGHT_WARN() == FALSE )
			{
				EOL_TEST_R7->m_EOLTestResult = FAIL;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\n") + pMainDlg->EOLLanguage.LP_LANE_WARNING_R_FAIL, UPDATE_ON);
			}
			else
			{
				strProcess.Format(_T("R7\r\nTesting %d/%d"), nCurProcess++, nTotalProcess);
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, strProcess, UPDATE_ON);
			}
			// 경보 검사 간의 딜레이
			Sleep(pMainDlg->m_nAlarmDelay);
		}

		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( (EOL_TEST_R7->m_EOLTestResult == PASS) && (pMainDlg->bEnStep_LDW_WarnL_WinkL == 1) )
		{
			if ( EOL_TEST_R7->STEP_ADAS_LDW_LEFT_WARN_WINK_L() == FALSE )
			{
				EOL_TEST_R7->m_EOLTestResult = FAIL;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\n") + pMainDlg->EOLLanguage.LP_LANE_WARNING_L_FAIL + _T("\r\n") +  + pMainDlg->EOLLanguage.LP_WINKER_L, UPDATE_ON);
			}
			else
			{
				strProcess.Format(_T("R7\r\nTesting %d/%d"), nCurProcess++, nTotalProcess);
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, strProcess, UPDATE_ON);
			}
			// 경보 검사 간의 딜레이
			Sleep(pMainDlg->m_nAlarmDelay);
		}

		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( (EOL_TEST_R7->m_EOLTestResult == PASS) && (pMainDlg->bEnStep_LDW_WarnR_WinkR == 1) )
		{
			if ( EOL_TEST_R7->STEP_ADAS_LDW_RIGHT_WARN_WINK_R() == FALSE )
			{
				EOL_TEST_R7->m_EOLTestResult = FAIL;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\n") + pMainDlg->EOLLanguage.LP_LANE_WARNING_R_FAIL + _T("\r\n") +  + pMainDlg->EOLLanguage.LP_WINKER_R, UPDATE_ON);
			}
			else
			{
				strProcess.Format(_T("R7\r\nTesting %d/%d"), nCurProcess++, nTotalProcess);
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, strProcess, UPDATE_ON);
			}
			// 경보 검사 간의 딜레이
			Sleep(pMainDlg->m_nAlarmDelay);
		}

		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( (EOL_TEST_R7->m_EOLTestResult == PASS) && (pMainDlg->bEnStep_LDW_WarnL_WinkR == 1) )
		{
			if ( EOL_TEST_R7->STEP_ADAS_LDW_LEFT_WARN_WINK_R() == FALSE )
			{
				EOL_TEST_R7->m_EOLTestResult = FAIL;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\n") + pMainDlg->EOLLanguage.LP_LANE_WARNING_L_FAIL + _T("\r\n") +  + pMainDlg->EOLLanguage.LP_WINKER_R, UPDATE_ON);
			}
			else
			{
				strProcess.Format(_T("R7\r\nTesting %d/%d"), nCurProcess++, nTotalProcess);
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, strProcess, UPDATE_ON);
			}
			// 경보 검사 간의 딜레이
			Sleep(pMainDlg->m_nAlarmDelay);
		}

		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( (EOL_TEST_R7->m_EOLTestResult == PASS) && (pMainDlg->bEnStep_LDW_WarnR_WinkL == 1) )
		{
			if ( EOL_TEST_R7->STEP_ADAS_LDW_RIGHT_WARN_WINK_L() == FALSE )
			{
				EOL_TEST_R7->m_EOLTestResult = FAIL;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\n") + pMainDlg->EOLLanguage.LP_LANE_WARNING_R_FAIL + _T("\r\n") +  + pMainDlg->EOLLanguage.LP_WINKER_L, UPDATE_ON);
			}
			else
			{
				strProcess.Format(_T("R7\r\nTesting %d/%d"), nCurProcess++, nTotalProcess);
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, strProcess, UPDATE_ON);
			}
			// 경보 검사 간의 딜레이
			Sleep(pMainDlg->m_nAlarmDelay);
		}

		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( (EOL_TEST_R7->m_EOLTestResult == PASS) && (pMainDlg->bEnStep_LDW_WarnL_WinkLR == 1) )
		{
			if ( EOL_TEST_R7->STEP_ADAS_LDW_LEFT_WARN_WINK_LR() == FALSE )
			{
				EOL_TEST_R7->m_EOLTestResult = FAIL;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\n") + pMainDlg->EOLLanguage.LP_LANE_WARNING_L_FAIL + _T("\r\n") +  + pMainDlg->EOLLanguage.LP_EMERGENCY_LAMP, UPDATE_ON);
			}
			else
			{
				strProcess.Format(_T("R7\r\nTesting %d/%d"), nCurProcess++, nTotalProcess);
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, strProcess, UPDATE_ON);
			}
			// 경보 검사 간의 딜레이
			Sleep(pMainDlg->m_nAlarmDelay);
		}

		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( (EOL_TEST_R7->m_EOLTestResult == PASS) && (pMainDlg->bEnStep_LDW_WarnR_WinkLR == 1) )
		{
			if ( EOL_TEST_R7->STEP_ADAS_LDW_RIGHT_WARN_WINK_LR() == FALSE )
			{
				EOL_TEST_R7->m_EOLTestResult = FAIL;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\n") + pMainDlg->EOLLanguage.LP_LANE_WARNING_R_FAIL + _T("\r\n") +  + pMainDlg->EOLLanguage.LP_EMERGENCY_LAMP, UPDATE_ON);
			}
			else
			{
				strProcess.Format(_T("R7\r\nTesting %d/%d"), nCurProcess++, nTotalProcess);
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, strProcess, UPDATE_ON);
			}
			// 경보 검사 간의 딜레이
			Sleep(pMainDlg->m_nAlarmDelay);
		}
		
		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( (EOL_TEST_R7->m_EOLTestResult == PASS) && (pMainDlg->bEnStep_FCW_Warn == 1) )
		{
			if ( EOL_TEST_R7->STEP_ADAS_FCW_WARN() == FALSE )
			{
				EOL_TEST_R7->m_EOLTestResult = FAIL;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\n") + pMainDlg->EOLLanguage.LP_FCW_WARNING_FAIL, UPDATE_ON);
			}
			else
			{
				strProcess.Format(_T("R7\r\nTesting %d/%d"), nCurProcess++, nTotalProcess);
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, strProcess, UPDATE_ON);
			}
			// 경보 검사 간의 딜레이
			Sleep(pMainDlg->m_nAlarmDelay);
		}

		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( (EOL_TEST_R7->m_EOLTestResult == PASS) && (pMainDlg->bEnStep_FCW_Warn2 == 1) )
		{
			if ( EOL_TEST_R7->STEP_ADAS_FCW_WARN2() == FALSE )
			{
				EOL_TEST_R7->m_EOLTestResult = FAIL;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\n") + pMainDlg->EOLLanguage.LP_FCW_WARNING_FAIL + (_T("2")), UPDATE_ON);
			}
			else
			{
				strProcess.Format(_T("R7\r\nTesting %d/%d"), nCurProcess++, nTotalProcess);
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, strProcess, UPDATE_ON);
			}
			// 경보 검사 간의 딜레이
			Sleep(pMainDlg->m_nAlarmDelay);
		}

		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( (EOL_TEST_R7->m_EOLTestResult == PASS) && (pMainDlg->bEnStep_FCW_Warn3 == 1) )
		{
			if ( EOL_TEST_R7->STEP_ADAS_FCW_WARN3() == FALSE )
			{
				EOL_TEST_R7->m_EOLTestResult = FAIL;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\n") + pMainDlg->EOLLanguage.LP_FCW_WARNING_FAIL + (_T("3")), UPDATE_ON);
			}
			else
			{
				strProcess.Format(_T("R7\r\nTesting %d/%d"), nCurProcess++, nTotalProcess);
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, strProcess, UPDATE_ON);
			}
			// 경보 검사 간의 딜레이
			Sleep(pMainDlg->m_nAlarmDelay);
		}

		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( (EOL_TEST_R7->m_EOLTestResult == PASS) && (pMainDlg->bEnStep_FCDA_Warn == 1) )
		{
			if ( EOL_TEST_R7->STEP_ADAS_FCDA_WARN() == FALSE )
			{
				EOL_TEST_R7->m_EOLTestResult = FAIL;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\nFCDA") + pMainDlg->EOLLanguage.LP_WARNING_FAIL, UPDATE_ON);
			}
			else
			{
				strProcess.Format(_T("R7\r\nTesting %d/%d"), nCurProcess++, nTotalProcess);
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, strProcess, UPDATE_ON);
			}
			// 경보 검사 간의 딜레이
			Sleep(pMainDlg->m_nAlarmDelay);
		}
		
		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( (EOL_TEST_R7->m_EOLTestResult == PASS) && (pMainDlg->bEnStep_DTC_Clear == 1) )
		{
			if ( EOL_TEST_R7->STEP_DTC_ALL_CLEAR() == FALSE )
			{
				EOL_TEST_R7->m_EOLTestResult = FAIL;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\n") + pMainDlg->EOLLanguage.LP_DTC_DELETE_FAIL, UPDATE_ON);
			}
			else
			{
				strProcess.Format(_T("R7\r\nTesting %d/%d"), nCurProcess++, nTotalProcess);
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, strProcess, UPDATE_ON);
			}
		}

		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( (EOL_TEST_R7->m_EOLTestResult == PASS) && (pMainDlg->bEnStep_IoBoard == 1) )
		{
			if ( EOL_TEST_R7->STEP_DISCONNECT_POWER() == FALSE )
			{
				EOL_TEST_R7->m_EOLTestResult = FAIL;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\n") + pMainDlg->EOLLanguage.LP_POWER_OFF_FAIL, UPDATE_ON);
			}
		}

		if ( EOL_TEST_R7->m_tThreadStop == true )
		{
			break;
		}
		else if ( (EOL_TEST_R7->m_EOLTestResult) == PASS && (pMainDlg->bEnStep_Bar2) )
		{
			if ( EOL_TEST_R7->STEP_CHECK_BARCODE2_SERIAL() == FALSE )
			{
				EOL_TEST_R7->m_EOLTestResult = FAIL;
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, _T("R7\r\n") + pMainDlg->EOLLanguage.LP_CASE_BARCODE_FAIL, UPDATE_ON);
			}
			else
			{
				strProcess.Format(_T("R7\r\nTesting %d/%d"), nCurProcess++, nTotalProcess);
				pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, strProcess, UPDATE_ON);
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////
		// 검사 결과 - 양품/불량 판단
		if ( EOL_TEST_R7->m_EOLTestResult == PASS )
		{
			pMainDlg->m_edit_notice.SetText(GREEN, BLACK, pMainDlg->EOLLanguage.LP_PASS, UPDATE_ON);
			pMainDlg->m_edit_notice2.SetText(GREEN, BLACK, pMainDlg->strProductName + pMainDlg->EOLLanguage.LP_TEST_PASS, UPDATE_ON);

			if ( pMainDlg->bEnStep_IoBoard == 1 )
			{
				pIoBoard->PutOutBit(_BIT_OUT_LED_G_OK, true);
				pIoBoard->PutOutBit(_BIT_OUT_LED_R_NG, false);
			}
			CString strWavFile;
			strWavFile = _T("pass.wav");
			sndPlaySound (strWavFile, SND_ASYNC);

			nPassNum++;
			strPassNum.Format(_T("%d"), nPassNum);
		}
		else if ( EOL_TEST_R7->m_EOLTestResult == FAIL )
		{
			pMainDlg->m_edit_notice.SetText(RED, BLACK, pMainDlg->EOLLanguage.LP_FAIL, UPDATE_ON);
			pMainDlg->m_edit_notice2.SetText(RED, BLACK, pMainDlg->strProductName + pMainDlg->EOLLanguage.LP_TEST_FAIL, UPDATE_ON);
			
			if ( pMainDlg->bEnStep_IoBoard == 1 )
			{
				pIoBoard->PutOutBit(_BIT_OUT_LED_G_OK, false);
				pIoBoard->PutOutBit(_BIT_OUT_LED_R_NG, true);
			}
			AfxMessageBox(pMainDlg->EOLLanguage.LP_FAIL_OCCURS);

			nFailNum++;
			strFailNum.Format(_T("%d"), nFailNum);
		}
		strTotalNum.Format(_T("%d"), nPassNum+nFailNum);
		strPassPercent.Format(_T("%f"), (nPassNum/1.0)/(nFailNum/1.0));

		pMainDlg->m_edit_pass_num.SetText(SOFT_BLUE, GREY_ROAD, strPassNum, UPDATE_ON);			// 양품갯수 표시
		pMainDlg->m_edit_fail_num.SetText(SOFT_BLUE, GREY_ROAD,strFailNum, UPDATE_ON);			// 불량갯수 표시
		pMainDlg->m_edit_total_num.SetText(SOFT_BLUE, GREY_ROAD, strTotalNum, UPDATE_ON);		// 전체갯수 표시
		pMainDlg->m_edit_pass_percent.SetText(SOFT_BLUE, GREY_ROAD, strPassPercent, UPDATE_ON);	// 양품비율 표시
		// 전체 테스트 갯수
		nTotalTest++;
		strTotalTest.Format(_T("%d"), nTotalTest);
		pMainDlg->m_edit_total_test.SetText(SOFT_BLUE, GREY_ROAD, strTotalTest, UPDATE_ON);	// 전체 테스트 갯수 표시

		////////////////////////////////////////////////////////////////////////////////////////
		// 테스트 로그 기록
		EOL_TEST_R7->SAVE_EOL_TEST_LOG();

		Sleep(1000);
	}
	// 테스트 로그 기록 끝
	if ( EOL_TEST_R7->bLogStart == TRUE )
	{
		EOL_TEST_R7->END_EOL_TEST_LOG();
		EOL_TEST_R7->bLogStart = FALSE;
	}
	
	// 마무리
	if ( pIoBoard != NULL )
	{
		EOL_TEST_R7->STEP_DISCONNECT_POWER();
	}
	EOL_TEST_R7->CloseIoBoard();
	EOL_TEST_R7->CloseBarcode1();
	EOL_TEST_R7->CloseBarcode2();

	
	// wait to finish thread
	delete EOL_TEST_R7;
	EOL_TEST_R7 = NULL;

	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, _T("PLK Technologies\r\nLDWS ") + pMainDlg->strProductName + _T("\r\n") + pMainDlg->EOLLanguage.LP_LAST_TEST, UPDATE_ON);
	pMainDlg->m_edit_notice2.SetText(SOFT_GREEN, BLACK, pMainDlg->strProductName + _T("\r\n") + pMainDlg->EOLLanguage.LP_TEST_END, UPDATE_ON);

	pMainDlg->PostMessageW(_WM_MAIN_DLG_END);

	return TRUE;
}

BOOL CEOL_R7::START_EOL_TEST_LOG()
{
	CString strLogFileName;
	CString strLogDay;
	CString strLogContent;
	CString strTester;
	CTime   oTime;

	oTime = CTime::GetCurrentTime();
	strLogDay.Format(_T("%d%d%d"), oTime.GetYear(), oTime.GetMonth(), oTime.GetDay());

	strLogFileName = pMainDlg->strLogPath;
	strLogFileName += _T("EOL_");
	strLogFileName += pMainDlg->strProductName;
	strLogFileName += _T("_");
	strLogFileName += strLogDay;
	strLogFileName += _T(".log");
	// EOL_R7_HMC_20161114.log
	pMainDlg->EOLSaveLog.SetFilePath(strLogFileName);

	strLogContent = pMainDlg->EOLLanguage.LP_FACTORY_LANE;
	strLogContent += _T(" : ");
	strLogContent += pMainDlg->strProductLine;
	strLogContent += _T(", ");
	strLogContent += pMainDlg->EOLLanguage.LP_TESTER;
	strLogContent += _T(" : ");
	pMainDlg->m_combo_tester.GetLBText(pMainDlg->m_combo_tester.GetCurSel(), strTester);
	strLogContent += strTester;
	strLogContent += _T("\r\n");

	pMainDlg->EOLSaveLog.WriteLogTime(strLogContent);

	return true;
}

BOOL CEOL_R7::SAVE_EOL_TEST_LOG()
{
	int nListPos, nListSubPos, nListMax;
	int nItemLength, nSpaceNum;
	CString strItemTitle;
	CString strItemText;
	
	if ( EOL_TEST_R7->m_EOLTestResult == PASS )
	{
		strItemTitle.Format(_T("Test%d [PASS]\r\n"), _ttoi(pMainDlg->m_edit_total_num.m_strMsg));
	}
	else if ( EOL_TEST_R7->m_EOLTestResult == FAIL )
	{
		strItemTitle.Format(_T("Test%d [FAIL]\r\n"), _ttoi(pMainDlg->m_edit_total_num.m_strMsg));
	}
	pMainDlg->EOLSaveLog.WriteLogTime(strItemTitle);

	// Test 결과 저장
	nListMax = pMainDlg->m_list_test_process.GetItemCount();
    for (nListPos = 0; nListPos < nListMax; nListPos++)
    {
		for (nListSubPos = 0; nListSubPos < 4; nListSubPos++)
		{
			strItemText = pMainDlg->m_list_test_process.GetItemText(nListPos, nListSubPos);
			pMainDlg->EOLSaveLog.WriteLog(strItemText);
						
			nSpaceNum = 26; // txt파일의 글자 길이(영문기준)
			nItemLength = strItemText.GetLength();
			while ( nItemLength-- > 0 )
			{
				// CString이 한글인 경우
				if ( strItemText.GetAt(nItemLength) <= 0 || 127 < strItemText.GetAt(nItemLength) )
				{
					nSpaceNum -= 2;
				}
				else
				{
					nSpaceNum -= 1;
				}
			}
			while( nSpaceNum-- > 0 )
			{
				pMainDlg->EOLSaveLog.WriteLog(_T(" "));
			}
		}
		pMainDlg->EOLSaveLog.WriteLog(_T("\r\n"));
	}

	return true;
}

BOOL CEOL_R7::END_EOL_TEST_LOG()
{
	CString strLogContent;

	strLogContent = _T("PASS : ");
	strLogContent += pMainDlg->m_edit_pass_num.m_strMsg;
	strLogContent += _T(", FAIL : ");
	strLogContent += pMainDlg->m_edit_fail_num.m_strMsg;
	strLogContent += _T(", TOTAL : ");
	strLogContent += pMainDlg->m_edit_total_num.m_strMsg;
	strLogContent += _T(", PASS RATE : ");
	strLogContent += pMainDlg->m_edit_pass_percent.m_strMsg;
	strLogContent += _T("\r\n**********************************************************************************\r\n");
	strLogContent += _T("\r\n\r\n");

	pMainDlg->EOLSaveLog.WriteLogTime(strLogContent);

	return true;
}

BOOL CEOL_R7::STEP_CONNECT_IOBOARD()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_IO_BOARD + _T("\r\n") + pMainDlg->EOLLanguage.LP_CONNECTING, UPDATE_ON);
	
	// IO 보드 초기화
	pIoBoard = new CIoBoardMgt;
	CString strIoBdProduct;
	// 포트 불러오기
	CString strIoBdCom;
	CLogManage	*pEolSetting;
	pEolSetting = new CLogManage;
	pEolSetting->SetFilePath(SETTING_FILE_PATH);
	strIoBdCom = pEolSetting->ReadSetting(COM_IO);
	delete pEolSetting;
    pEolSetting = NULL;

	char *pbIoBdCom = new char[strIoBdCom.GetLength()+1];
	strcpy(pbIoBdCom, CT2A(strIoBdCom));
	pbIoBdCom[strIoBdCom.GetLength()] = '\0';
	
    if (pIoBoard == NULL)
    {
        return FALSE;
    }

	if (pIoBoard->Open(pbIoBdCom) == false)
    {
        dp("[%s] IO Board Open Fail!\n",pbIoBdCom);

		CloseIoBoard();
        return FALSE;
    }
		

	strIoBdProduct = _T("KM ");
    if (pIoBoard->TransferMsg("$01GA001\r") == false)
    {
        dp("[%s] IO Board communication error!!\n", pbIoBdCom);

		CloseIoBoard();
        return FALSE;
    }
	else
	{
		strIoBdProduct += (pIoBoard->GetMsg()+9);
	}

	strIoBdProduct += _T(" Ver ");
    if (strcmp(pIoBoard->GetMsg()+9, "6050") != 0)
    {
        dp("[%s] IO Board model %s error!\n",pbIoBdCom, pIoBoard->GetMsg()+9);

		CloseIoBoard();
        return FALSE;
    }
	else
	{
		strIoBdProduct += (pIoBoard->GetMsg()+9);
	}
		
	delete pbIoBdCom;
	pbIoBdCom= NULL;
	
	return TRUE;
}

BOOL CEOL_R7::STEP_CONNECT_BARCODE1()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_BARCODE1 + pMainDlg->EOLLanguage.LP_CONNECTING, UPDATE_ON);

	// 포트 불러오기
	CString strBar1Com;
	CLogManage	*pEolSetting;
	pEolSetting = new CLogManage;
	pEolSetting->SetFilePath(SETTING_FILE_PATH);
	strBar1Com = pEolSetting->ReadSetting(COM_BAR1);
	delete pEolSetting;
    pEolSetting = NULL;
		
	if ( pBarCode1 == NULL )
	{
		// 바코드 초기화
		pBarCode1 = new CBarcodeMgt;

		if (pBarCode1->Open((CStringA)strBar1Com) == true)
		{
			dp("Barcode1 : %s Open Success\n", (CStringA)strBar1Com);

			// 바코드 Thread 시작
			pBarCode1->Thread_Begin();
		}
		else
		{
			dp("Barcode1 : %s Open Fail\n", (CStringA)strBar1Com);

			CloseBarcode1();
		}
	}
	else
	{
		CloseBarcode1();
	}

	return TRUE;
}

BOOL CEOL_R7::STEP_CONNECT_BARCODE2()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_BARCODE2 + pMainDlg->EOLLanguage.LP_CONNECTING, UPDATE_ON);

	// 포트 불러오기
	CString strBar2Com;
	CLogManage	*pEolSetting;
	pEolSetting = new CLogManage;
	pEolSetting->SetFilePath(SETTING_FILE_PATH);
	strBar2Com = pEolSetting->ReadSetting(COM_BAR2);
	delete pEolSetting;
    pEolSetting = NULL;
	
	if ( pBarCode2 == NULL )
	{
		// 바코드 초기화
		pBarCode2 = new CBarcodeMgt;

		if (pBarCode2->Open((CStringA)strBar2Com) == true)
		{
			dp("Barcode2 : %s Open Success\n", (CStringA)strBar2Com);

			// 바코드 Thread 시작
			pBarCode2->Thread_Begin();
		}
		else
		{
			dp("Barcode2 : %s Open Fail\n", (CStringA)strBar2Com);

			CloseBarcode2();
		}
	}
	else
	{
		CloseBarcode2();
	}

	return TRUE;
}


BOOL CEOL_R7::CloseIoBoard()
{
	if (pIoBoard == NULL)
    {
        return FALSE;
    }

	delete pIoBoard;
    pIoBoard = NULL;
	dp("IO Board Close\n");

	return TRUE;
}
BOOL CEOL_R7::CloseBarcode1()
{
	if ( pBarCode1 == NULL )
	{
		return FALSE;
	}

	pBarCode1->Close();
	pBarCode1->DelAllData();
	delete pBarCode1;
	pBarCode1 = NULL;

	dp("Barcode1 Close\n");

	return TRUE;
}
BOOL CEOL_R7::CloseBarcode2()
{
	if ( pBarCode2 == NULL )
	{
		return FALSE;
	}

	pBarCode2->Close();
	pBarCode2->DelAllData();
	delete pBarCode2;
	pBarCode2 = NULL;

	dp("Barcode2 Close\n");

	return TRUE;
}

BOOL CEOL_R7::STEP_CONNECT_POWER()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_TEST_PROCESS_START, UPDATE_ON);
	
	BOOL bResult = FALSE;

	// Power Off
	pIoBoard->PutOutBit(_BIT_OUT_POWER, false);
	Sleep(500);
	// Silinder Disenable
	pIoBoard->PutOutBit(_BIT_OUT_SILINDER_EN, false);
	// Init Test Settings
	pIoBoard->PutOutBit(_BIT_OUT_LED_Y_POWER, false);
	pIoBoard->PutOutBit(_BIT_OUT_LED_G_OK, false);
	pIoBoard->PutOutBit(_BIT_OUT_LED_R_NG, false);

	// Silinder Enable
	pIoBoard->PutOutBit(_BIT_OUT_SILINDER_EN, false);
	Sleep(200);
	pIoBoard->PutOutBit(_BIT_OUT_SILINDER_EN, true);
	while ( m_tThreadStop == false )
	{
		// 진입 완료 신호 HIGH
		if ((pIoBoard->GetInPort() & _PIN_IN_CONNECT) == 1)
        {
			bResult = TRUE;
            break;
        }
	}

	// Power On
	Sleep(200);
	pIoBoard->PutOutBit(_BIT_OUT_POWER, true);
	pIoBoard->PutOutBit(_BIT_OUT_LED_Y_POWER, true);

	return bResult;
}


BOOL CEOL_R7::STEP_DISCONNECT_POWER()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_POWER_OFF, UPDATE_ON);

	BOOL bResult = FALSE;

	// Power Off
	pIoBoard->PutOutBit(_BIT_OUT_POWER, false);
	Sleep(500);
	// Silinder Disenable
	pIoBoard->PutOutBit(_BIT_OUT_SILINDER_EN, false);
	// Init Test Settings
	pIoBoard->PutOutBit(_BIT_OUT_LED_Y_POWER, false);
	pIoBoard->PutOutBit(_BIT_OUT_LED_G_OK, false);
	pIoBoard->PutOutBit(_BIT_OUT_LED_R_NG, false);

	bResult = TRUE;

	return bResult;
}


BOOL CEOL_R7::STEP_CONNECT_USART()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_CONNECTION_CHECK, UPDATE_ON);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->CALIBRATION);

	BOOL bResult = FALSE;
	DWORD nStartTime = GetTickCount();
	CString strTestItem = pMainDlg->EOLLanguage.LP_CONNECTION_CHECK;
	CString strTgtMsg = _T("1");
	CString strSrcMsg = _T("0");
	int nListPos;

	nListPos = pMainDlg->m_list_test_process.GetItemCount();
	pMainDlg->m_list_test_process.InsertItem(nListPos, strTestItem);
	pMainDlg->m_list_test_process.SetItemText(nListPos, 1, strTgtMsg);

	while ( m_tThreadStop == false )
	{
		if ( m_JTP_Com.AutoOpenCom() == TRUE )
		{
			dp("R7 JTP Comm Success\n");
			strSrcMsg = _T("1");
			bResult = TRUE;
			break;
		}

		// 5초간 연결이 되지 않으면 Fail
		if ( (GetTickCount() - nStartTime) > 5000 )
		{
			dp("R7 JTP Comm Failed\n");
			strSrcMsg = _T("Comm Error");
			break;
		}
	}

	pMainDlg->m_list_test_process.SetItemText(nListPos, 2, strSrcMsg);

	if ( bResult == TRUE )
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("PASS"));
	}
	else
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("FAIL"));
	}

	return bResult;
}

BOOL CEOL_R7::STEP_CHK_SERIAL()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_SERIAL_NUMBER, UPDATE_ON);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->CALIBRATION);

	BOOL bResult = FALSE;
	DWORD nStartTime = GetTickCount();
	CString strTestItem = pMainDlg->EOLLanguage.LP_SERIAL_NUMBER;
	CString strTgtMsg = _T("-");
	CString strSrcMsg = _T("0");
	int nListPos;
	int nRxLength;
	BYTE arbRxData[256];
	CFG_SETUP_R7 *oCFG_Setup;

	nListPos = pMainDlg->m_list_test_process.GetItemCount();
	pMainDlg->m_list_test_process.InsertItem(nListPos, strTestItem);
	pMainDlg->m_list_test_process.SetItemText(nListPos, 1, strTgtMsg);

	strSerialNum = _T("");
	while ( m_tThreadStop == false )
	{
		if ( m_JTP_Com.SendMsg_UserCfgGet(arbRxData, &nRxLength) == TRUE )
		{
			oCFG_Setup = new CFG_SETUP_R7;
			memcpy(oCFG_Setup, arbRxData, nRxLength);

			strSrcMsg.Format(_T("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"),
				oCFG_Setup->arbDeviceSerial[0],
				oCFG_Setup->arbDeviceSerial[1],
				oCFG_Setup->arbDeviceSerial[2],
				oCFG_Setup->arbDeviceSerial[3],
				oCFG_Setup->arbDeviceSerial[4],
				oCFG_Setup->arbDeviceSerial[5],
				oCFG_Setup->arbDeviceSerial[6],
				oCFG_Setup->arbDeviceSerial[7],
				oCFG_Setup->arbDeviceSerial[8],
				oCFG_Setup->arbDeviceSerial[9],
				oCFG_Setup->arbDeviceSerial[10],
				oCFG_Setup->arbDeviceSerial[11],
				oCFG_Setup->arbDeviceSerial[12],
				oCFG_Setup->arbDeviceSerial[13],
				oCFG_Setup->arbDeviceSerial[14],
				oCFG_Setup->arbDeviceSerial[15]);
			strSerialNum = strSrcMsg;
			delete oCFG_Setup;
			oCFG_Setup = NULL;
			bResult = TRUE;
			break;
		}

		// 5초간 연결이 되지 않으면 Fail
		if ( (GetTickCount() - nStartTime) > 5000 )
		{
			strSrcMsg = _T("Comm Error");
			break;
		}
	}

	pMainDlg->m_list_test_process.SetItemText(nListPos, 2, strSrcMsg);
	
	if ( bResult == TRUE )
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("PASS"));
	}
	else
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("FAIL"));
	}

	return bResult;
}


BOOL CEOL_R7::STEP_CHK_SW_VERSION()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_SOFTWARE_VERSION, UPDATE_ON);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->CALIBRATION);

	BOOL bResult = FALSE;
	DWORD nStartTime = GetTickCount();
	CString strTestItem = pMainDlg->EOLLanguage.LP_SOFTWARE_VERSION;
	CString strTgtMsg = pMainDlg->strSwVer;
	CString strSrcMsg = _T("0");
	int nListPos;
	int nRxLength;
	BYTE arbRxData[256];
	SYSTEM_VERSION *oSYSTEM_VERSION;

	nListPos = pMainDlg->m_list_test_process.GetItemCount();
	pMainDlg->m_list_test_process.InsertItem(nListPos, strTestItem);
	pMainDlg->m_list_test_process.SetItemText(nListPos, 1, strTgtMsg);

	while ( m_tThreadStop == false )
	{
		if ( m_JTP_Com.SendMsg_DevInfoGet(arbRxData, &nRxLength) == TRUE )
		{
			oSYSTEM_VERSION = new SYSTEM_VERSION;
			memcpy(oSYSTEM_VERSION, arbRxData, nRxLength);

			strSrcMsg.Format(_T("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"),
					oSYSTEM_VERSION->szFirmwareVer[0],
					oSYSTEM_VERSION->szFirmwareVer[1],
					oSYSTEM_VERSION->szFirmwareVer[2],
					oSYSTEM_VERSION->szFirmwareVer[3],
					oSYSTEM_VERSION->szFirmwareVer[4],
					oSYSTEM_VERSION->szFirmwareVer[5],
					oSYSTEM_VERSION->szFirmwareVer[6],
					oSYSTEM_VERSION->szFirmwareVer[7],
					oSYSTEM_VERSION->szFirmwareVer[8],
					oSYSTEM_VERSION->szFirmwareVer[9],
					oSYSTEM_VERSION->szFirmwareVer[10],
					oSYSTEM_VERSION->szFirmwareVer[11],
					oSYSTEM_VERSION->szFirmwareVer[12],
					oSYSTEM_VERSION->szFirmwareVer[13],
					oSYSTEM_VERSION->szFirmwareVer[14],
					oSYSTEM_VERSION->szFirmwareVer[15]);
			delete oSYSTEM_VERSION;
			oSYSTEM_VERSION = NULL;

			if ( strSrcMsg.Compare(strTgtMsg) == 0 )
			{
				bResult = TRUE;
			}
			break;
		}

		// 5초간 연결이 되지 않으면 Fail
		if ( (GetTickCount() - nStartTime) > 5000 )
		{
			strSrcMsg = _T("Comm Error");
			break;
		}
	}

	pMainDlg->m_list_test_process.SetItemText(nListPos, 2, strSrcMsg);
	
	if ( bResult == TRUE )
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("PASS"));
	}
	else
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("FAIL"));
	}

	return bResult;
}

BOOL CEOL_R7::STEP_CHK_MCU_VERSION()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_MCU_VERSION, UPDATE_ON);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->CALIBRATION);

	BOOL bResult = FALSE;
	DWORD nStartTime = GetTickCount();
	CString strTestItem = pMainDlg->EOLLanguage.LP_MCU_VERSION;
	CString strTgtMsg = pMainDlg->strMcuVer;
	CString strSrcMsg = _T("0");
	int nListPos;
	BYTE arbRxData[256];

	nListPos = pMainDlg->m_list_test_process.GetItemCount();
	pMainDlg->m_list_test_process.InsertItem(nListPos, strTestItem);
	pMainDlg->m_list_test_process.SetItemText(nListPos, 1, strTgtMsg);

	while ( m_tThreadStop == false )
	{
		if ( m_JTP_Com.SendMsg_GetMAVersion(1, &arbRxData) == TRUE )
		{
			strSrcMsg.Format(_T("%c%d%d%d"),
				arbRxData[0],  arbRxData[1],  arbRxData[2],  arbRxData[3]);

			if ( strSrcMsg.Compare(strTgtMsg) == 0 )
			{
				bResult = TRUE;
			}
			break;
		}

		// 5초간 연결이 되지 않으면 Fail
		if ( (GetTickCount() - nStartTime) > 5000 )
		{
			strSrcMsg = _T("Comm Error");
			break;
		}
	}

	pMainDlg->m_list_test_process.SetItemText(nListPos, 2, strSrcMsg);
	
	if ( bResult == TRUE )
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("PASS"));
	}
	else
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("FAIL"));
	}

	return bResult;
}

BOOL CEOL_R7::STEP_CHK_VANISH()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_ACCELERATION, UPDATE_ON);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->CALIBRATION);

	BOOL bResult = FALSE;
	DWORD nStartTime = GetTickCount();
	CString strTestItem = pMainDlg->EOLLanguage.LP_ACCELERATION;
	CString strTgtMsg = _T("Acc X, Acc Z");
	CString strSrcMsg = _T("0");
	int nListPos;
	int nRxLength;
	BYTE arbRxData[256];
	CFG_SETUP_R7 *oCFG_Setup;

	nListPos = pMainDlg->m_list_test_process.GetItemCount();
	pMainDlg->m_list_test_process.InsertItem(nListPos, strTestItem);
	pMainDlg->m_list_test_process.SetItemText(nListPos, 1, strTgtMsg);

	while ( m_tThreadStop == false )
	{
		if ( m_JTP_Com.SendMsg_UserCfgGet(arbRxData, &nRxLength) == TRUE )
		{
			oCFG_Setup = new CFG_SETUP_R7;
			memcpy(oCFG_Setup, arbRxData, nRxLength);
						
			if ( oCFG_Setup->bAccSavedState == 1 )
			{
				strSrcMsg.Format(_T("%d, %d"), arbRxData[0],  arbRxData[1]);
				bResult = TRUE;
			}
			else
			{
				strSrcMsg = _T("No Saved");
			}

			delete oCFG_Setup;
			oCFG_Setup = NULL;
			break;
		}

		// 5초간 연결이 되지 않으면 Fail
		if ( (GetTickCount() - nStartTime) > 5000 )
		{
			strSrcMsg = _T("Comm Error");
			break;
		}
	}

	pMainDlg->m_list_test_process.SetItemText(nListPos, 2, strSrcMsg);
	
	if ( bResult == TRUE )
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("PASS"));
	}
	else
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("FAIL"));
	}

	return bResult;
}

BOOL CEOL_R7::STEP_CHK_SPEED_PULSE()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_SPEED_PULSE, UPDATE_ON);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->CALIBRATION);

	BOOL bResult = FALSE;
	DWORD nStartTime = GetTickCount();
	CString strTestItem = pMainDlg->EOLLanguage.LP_SPEED_PULSE;
	CString strTgtMsg = _T("-");
	CString strSrcMsg = _T("0");
	int nListPos;
	int nRxLength;
	BYTE arbRxData[256];
	CFG_SETUP_R7 *oCFG_Setup;

	nListPos = pMainDlg->m_list_test_process.GetItemCount();
	pMainDlg->m_list_test_process.InsertItem(nListPos, strTestItem);
	pMainDlg->m_list_test_process.SetItemText(nListPos, 1, strTgtMsg);

	while ( m_tThreadStop == false )
	{
		if ( m_JTP_Com.SendMsg_UserCfgGet(arbRxData, &nRxLength) == TRUE )
		{
			oCFG_Setup = new CFG_SETUP_R7;
			memcpy(oCFG_Setup, arbRxData, nRxLength);

			strSrcMsg.Format(_T("%d"), oCFG_Setup->uIOSpeedPulse );
			
			if ( oCFG_Setup->uIOSpeedPulse != 0 )
			{
				bResult = TRUE;
			}

			delete oCFG_Setup;
			oCFG_Setup = NULL;
			
			break;
		}

		// 5초간 연결이 되지 않으면 Fail
		if ( (GetTickCount() - nStartTime) > 5000 )
		{
			strSrcMsg = _T("Comm Error");
			break;
		}
	}

	pMainDlg->m_list_test_process.SetItemText(nListPos, 2, strSrcMsg);
	
	if ( bResult == TRUE )
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("PASS"));
	}
	else
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("FAIL"));
	}

	return bResult;
}



BOOL CEOL_R7::DISPLAY_ADAS_INFO(void *a_pAdasInfoData, int a_puDataLen)
{
	CString strInSpeed;
	HUB_INFO *oOutput_COMM;
	oOutput_COMM = new HUB_INFO;
	memcpy(oOutput_COMM, a_pAdasInfoData, a_puDataLen);

	// 입력속도 표시
	strInSpeed.Format(_T("%d"), m_nVehicleSpeed);
	pMainDlg->m_edit_in_speed.SetText(SOFT_BLUE, GREY_ROAD, strInSpeed, UPDATE_ON);
	// 입력방향지시등L 표시
	if ( m_bWinkL == 0 )
	{
		pMainDlg->m_edit_in_wink_l.SetText(SOFT_BLUE, GREY_ROAD, _T("0"), UPDATE_ON);
	}
	else
	{
		pMainDlg->m_edit_in_wink_l.SetText(SOFT_BLUE, GREY_ROAD, _T("1"), UPDATE_ON);
	}
	// 입력방향지시등R 표시
	if ( m_bWinkR == 0 )
	{
		pMainDlg->m_edit_in_wink_r.SetText(SOFT_BLUE, GREY_ROAD, _T("0"), UPDATE_ON);
	}
	else
	{
		pMainDlg->m_edit_in_wink_r.SetText(SOFT_BLUE, GREY_ROAD, _T("1"), UPDATE_ON);
	}


	// 차선인식 표시
	if ( (oOutput_COMM->bLaneDetectState & ADAS_LANE_DETECT) != 0 )
	{
		pMainDlg->m_edit_out_ldw_detetc_l.SetText(SOFT_BLUE, GREY_ROAD, _T("1"), UPDATE_ON);
		pMainDlg->m_edit_out_m_ldw_detect_r.SetText(SOFT_BLUE, GREY_ROAD, _T("1"), UPDATE_ON);
	}
	else
	{
		pMainDlg->m_edit_out_ldw_detetc_l.SetText(SOFT_BLUE, GREY_ROAD, _T("0"), UPDATE_ON);
		pMainDlg->m_edit_out_m_ldw_detect_r.SetText(SOFT_BLUE, GREY_ROAD, _T("0"), UPDATE_ON);
	}
	// 차선경보L 표시
	if ( (oOutput_COMM->bLaneDetectState & ADAS_LANE_LEFT_WARN) != 0 )
	{
		pMainDlg->m_edit_out_ldw_warn_l.SetText(SOFT_BLUE, GREY_ROAD, _T("1"), UPDATE_ON);
	}
	else
	{
		pMainDlg->m_edit_out_ldw_warn_l.SetText(SOFT_BLUE, GREY_ROAD, _T("0"), UPDATE_ON);
	}
	// 차선경보R 표시
	if ( (oOutput_COMM->bLaneDetectState & ADAS_LANE_RIGHT_WARN) != 0 )
	{
		pMainDlg->m_edit_out_ldw_warn_r.SetText(SOFT_BLUE, GREY_ROAD, _T("1"), UPDATE_ON);
	}
	else
	{
		pMainDlg->m_edit_out_ldw_warn_r.SetText(SOFT_BLUE, GREY_ROAD, _T("0"), UPDATE_ON);
	}
	// HMW경보 표시
	if ( (oOutput_COMM->uFcwState & ADAS_FCW_HMW_WARN) != 0 )
	{
		pMainDlg->m_edit_out_hmw.SetText(SOFT_BLUE, GREY_ROAD, _T("1"), UPDATE_ON);
	}
	else
	{
		pMainDlg->m_edit_out_hmw.SetText(SOFT_BLUE, GREY_ROAD, _T("0"), UPDATE_ON);
	}
	// FCW경보 표시
	if ( (oOutput_COMM->uFcwState & ADAS_FCW_HMW_WARN) != 0 )
	{
		pMainDlg->m_edit_out_fcw.SetText(SOFT_BLUE, GREY_ROAD, _T("1"), UPDATE_ON);
	}
	else
	{
		pMainDlg->m_edit_out_fcw.SetText(SOFT_BLUE, GREY_ROAD, _T("0"), UPDATE_ON);	
	}
	delete oOutput_COMM;
	oOutput_COMM = NULL;

	return true;
}

BOOL CEOL_R7::STEP_ADAS_LDW_DETECT()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_LANE_DETECT, UPDATE_ON);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->LDW_DETECT);

	BOOL bResult = FALSE;
	DWORD nStartTime = GetTickCount();
	CString strTestItem = pMainDlg->EOLLanguage.LP_LANE_DETECT;
	CString strTgtMsg = _T("1");
	CString strSrcMsg = _T("0");
	int nListPos;
	int nRxLength;
	BYTE arbRxData[256];
	HUB_INFO *oOutput_COMM;

	// ADAS 입력 데이터
	( pMainDlg->bEnStep_LDW_Speed == 1 )? (m_nVehicleSpeed = pMainDlg->m_nVehicleLdwSpd) : (m_nVehicleSpeed = 100);
	m_bWinkL = 0;
	m_bWinkR = 0;

	nListPos = pMainDlg->m_list_test_process.GetItemCount();
	pMainDlg->m_list_test_process.InsertItem(nListPos, strTestItem);
	pMainDlg->m_list_test_process.SetItemText(nListPos, 1, strTgtMsg);

	while ( m_tThreadStop == false )
	{
		if ( m_JTP_Com.SendMsg_ADASInfoGet_R7(arbRxData, &nRxLength, m_nVehicleSpeed, m_bWinkL, m_bWinkR) == TRUE )
		{
			DISPLAY_ADAS_INFO(arbRxData, nRxLength);

			oOutput_COMM = new HUB_INFO;
			memcpy(oOutput_COMM, arbRxData, nRxLength);

			if ( (oOutput_COMM->bLaneDetectState & ADAS_LANE_DETECT) != 0 )
			{
				strSrcMsg.Format(_T("%d"), oOutput_COMM->bLaneDetectState & ADAS_LANE_DETECT );
				bResult = TRUE;
			}

			delete oOutput_COMM;
			oOutput_COMM = NULL;

			if ( bResult == TRUE ) break;
		}

		// 5초간 연결이 되지 않으면 Fail
		if ( (GetTickCount() - nStartTime) > 5000 )
		{
			strSrcMsg = _T("Comm Error");
			break;
		}
	}

	pMainDlg->m_list_test_process.SetItemText(nListPos, 2, strSrcMsg);
	
	if ( bResult == TRUE )
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("PASS"));
	}
	else
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("FAIL"));
	}

	return bResult;
}

BOOL CEOL_R7::STEP_ADAS_LDW_LEFT_WARN()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_LANE_WARNING_L, UPDATE_ON);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->LDW_WARN_L);

	BOOL bResult = FALSE;
	DWORD nStartTime = GetTickCount();
	CString strTestItem = pMainDlg->EOLLanguage.LP_LANE_WARNING_L;
	CString strTgtMsg = _T("4");
	CString strSrcMsg = _T("0");
	int nListPos;
	int nRxLength;
	BYTE arbRxData[256];
	HUB_INFO *oOutput_COMM;

	// ADAS 입력 데이터
	( pMainDlg->bEnStep_LDW_Speed == 1 )? (m_nVehicleSpeed = pMainDlg->m_nVehicleLdwSpd) : (m_nVehicleSpeed = 100);
	m_bWinkL = 0;
	m_bWinkR = 0;

	nListPos = pMainDlg->m_list_test_process.GetItemCount();
	pMainDlg->m_list_test_process.InsertItem(nListPos, strTestItem);
	pMainDlg->m_list_test_process.SetItemText(nListPos, 1, strTgtMsg);

	while ( m_tThreadStop == false )
	{
		if ( m_JTP_Com.SendMsg_ADASInfoGet_R7(arbRxData, &nRxLength, m_nVehicleSpeed, m_bWinkL, m_bWinkR) == TRUE )
		{
			DISPLAY_ADAS_INFO(arbRxData, nRxLength);

			oOutput_COMM = new HUB_INFO;
			memcpy(oOutput_COMM, arbRxData, nRxLength);

			if ( (oOutput_COMM->bLaneDetectState & ADAS_LANE_LEFT_WARN) != 0 )
			{
				strSrcMsg.Format(_T("%d"), oOutput_COMM->bLaneDetectState & ADAS_LANE_LEFT_WARN );
				bResult = TRUE;
			}

			delete oOutput_COMM;
			oOutput_COMM = NULL;

			if ( bResult == TRUE ) break;
		}

		// 5초간 연결이 되지 않으면 Fail
		if ( (GetTickCount() - nStartTime) > 5000 )
		{
			strSrcMsg = _T("Comm Error");
			break;
		}
	}

	pMainDlg->m_list_test_process.SetItemText(nListPos, 2, strSrcMsg);
	
	if ( bResult == TRUE )
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("PASS"));
	}
	else
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("FAIL"));
	}

	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->LDW_DETECT);
	return bResult;
}

BOOL CEOL_R7::STEP_ADAS_LDW_RIGHT_WARN()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_LANE_WARNING_R, UPDATE_ON);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->LDW_WARN_R);

	BOOL bResult = FALSE;
	DWORD nStartTime = GetTickCount();
	CString strTestItem = pMainDlg->EOLLanguage.LP_LANE_WARNING_R;
	CString strTgtMsg = _T("8");
	CString strSrcMsg = _T("0");
	int nListPos;
	int nRxLength;
	BYTE arbRxData[256];
	HUB_INFO *oOutput_COMM;

	// ADAS 입력 데이터
	( pMainDlg->bEnStep_LDW_Speed == 1 )? (m_nVehicleSpeed = pMainDlg->m_nVehicleLdwSpd) : (m_nVehicleSpeed = 100);
	m_bWinkL = 0;
	m_bWinkR = 0;

	nListPos = pMainDlg->m_list_test_process.GetItemCount();
	pMainDlg->m_list_test_process.InsertItem(nListPos, strTestItem);
	pMainDlg->m_list_test_process.SetItemText(nListPos, 1, strTgtMsg);

	while ( m_tThreadStop == false )
	{
		if ( m_JTP_Com.SendMsg_ADASInfoGet_R7(arbRxData, &nRxLength, m_nVehicleSpeed, m_bWinkL, m_bWinkR) == TRUE )
		{
			DISPLAY_ADAS_INFO(arbRxData, nRxLength);

			oOutput_COMM = new HUB_INFO;
			memcpy(oOutput_COMM, arbRxData, nRxLength);

			if ( (oOutput_COMM->bLaneDetectState & ADAS_LANE_RIGHT_WARN) != 0 )
			{
				strSrcMsg.Format(_T("%d"), oOutput_COMM->bLaneDetectState & ADAS_LANE_RIGHT_WARN );
				bResult = TRUE;
			}

			delete oOutput_COMM;
			oOutput_COMM = NULL;

			if ( bResult == TRUE ) break;
		}

		// 5초간 연결이 되지 않으면 Fail
		if ( (GetTickCount() - nStartTime) > 5000 )
		{
			strSrcMsg = _T("Comm Error");
			break;
		}
	}

	pMainDlg->m_list_test_process.SetItemText(nListPos, 2, strSrcMsg);
	
	if ( bResult == TRUE )
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("PASS"));
	}
	else
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("FAIL"));
	}

	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->LDW_DETECT);
	return bResult;
}

BOOL CEOL_R7::STEP_ADAS_LDW_LEFT_WARN_WINK_L()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_LANE_WARNING_L + _T("\r\n") + pMainDlg->EOLLanguage.LP_WINKER_L, UPDATE_ON);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->LDW_WARN_L);

	BOOL bResult = TRUE;
	DWORD nStartTime = GetTickCount();
	CString strTestItem = pMainDlg->EOLLanguage.LP_LANE_WARNING_L + pMainDlg->EOLLanguage.LP_WINKER_L;
	CString strTgtMsg = _T("0");
	CString strSrcMsg = _T("0");
	int nListPos;
	int nRxLength;
	BYTE arbRxData[256];
	HUB_INFO *oOutput_COMM;

	// ADAS 입력 데이터
	( pMainDlg->bEnStep_LDW_Speed == 1 )? (m_nVehicleSpeed = pMainDlg->m_nVehicleLdwSpd) : (m_nVehicleSpeed = 100);
	m_bWinkL = 1;
	m_bWinkR = 0;

	nListPos = pMainDlg->m_list_test_process.GetItemCount();
	pMainDlg->m_list_test_process.InsertItem(nListPos, strTestItem);
	pMainDlg->m_list_test_process.SetItemText(nListPos, 1, strTgtMsg);

	while ( m_tThreadStop == false )
	{
		if ( m_JTP_Com.SendMsg_ADASInfoGet_R7(arbRxData, &nRxLength, m_nVehicleSpeed, m_bWinkL, m_bWinkR) == TRUE )
		{
			DISPLAY_ADAS_INFO(arbRxData, nRxLength);

			oOutput_COMM = new HUB_INFO;
			memcpy(oOutput_COMM, arbRxData, nRxLength);

			if ( (oOutput_COMM->bLaneDetectState & ADAS_LANE_LEFT_WARN) != 0 )
			{
				strSrcMsg.Format(_T("%d"), oOutput_COMM->bLaneDetectState & ADAS_LANE_LEFT_WARN );
				bResult = FALSE;
			}

			delete oOutput_COMM;
			oOutput_COMM = NULL;

			if ( bResult == FALSE ) break;
		}

		// 5초간 연결이 되지 않으면 Fail
		if ( (GetTickCount() - nStartTime) > 5000 )
		{
			strSrcMsg = _T("Comm Error");
			break;
		}
	}

	pMainDlg->m_list_test_process.SetItemText(nListPos, 2, strSrcMsg);
	
	if ( bResult == TRUE )
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("PASS"));
	}
	else
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("FAIL"));
	}

	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->LDW_DETECT);
	return bResult;
}

BOOL CEOL_R7::STEP_ADAS_LDW_LEFT_WARN_WINK_R()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_LANE_WARNING_L + _T("\r\n") + pMainDlg->EOLLanguage.LP_WINKER_R, UPDATE_ON);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->LDW_WARN_L);

	BOOL bResult = FALSE;
	DWORD nStartTime = GetTickCount();
	CString strTestItem = pMainDlg->EOLLanguage.LP_LANE_WARNING_L + pMainDlg->EOLLanguage.LP_WINKER_R;
	CString strTgtMsg = _T("4");
	CString strSrcMsg = _T("0");
	int nListPos;
	int nRxLength;
	BYTE arbRxData[256];
	HUB_INFO *oOutput_COMM;

	// ADAS 입력 데이터
	( pMainDlg->bEnStep_LDW_Speed == 1 )? (m_nVehicleSpeed = pMainDlg->m_nVehicleLdwSpd) : (m_nVehicleSpeed = 100);
	m_bWinkL = 0;
	m_bWinkR = 1;

	nListPos = pMainDlg->m_list_test_process.GetItemCount();
	pMainDlg->m_list_test_process.InsertItem(nListPos, strTestItem);
	pMainDlg->m_list_test_process.SetItemText(nListPos, 1, strTgtMsg);

	while ( m_tThreadStop == false )
	{
		if ( m_JTP_Com.SendMsg_ADASInfoGet_R7(arbRxData, &nRxLength, m_nVehicleSpeed, m_bWinkL, m_bWinkR) == TRUE )
		{
			DISPLAY_ADAS_INFO(arbRxData, nRxLength);

			oOutput_COMM = new HUB_INFO;
			memcpy(oOutput_COMM, arbRxData, nRxLength);

			if ( (oOutput_COMM->bLaneDetectState & ADAS_LANE_LEFT_WARN) != 0 )
			{
				strSrcMsg.Format(_T("%d"), oOutput_COMM->bLaneDetectState & ADAS_LANE_LEFT_WARN );
				bResult = TRUE;
			}

			delete oOutput_COMM;
			oOutput_COMM = NULL;

			if ( bResult == TRUE ) break;
		}

		// 5초간 연결이 되지 않으면 Fail
		if ( (GetTickCount() - nStartTime) > 5000 )
		{
			strSrcMsg = _T("Comm Error");
			break;
		}
	}

	pMainDlg->m_list_test_process.SetItemText(nListPos, 2, strSrcMsg);
	
	if ( bResult == TRUE )
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("PASS"));
	}
	else
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("FAIL"));
	}

	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->LDW_DETECT);
	return bResult;
}

BOOL CEOL_R7::STEP_ADAS_LDW_LEFT_WARN_WINK_LR()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_LANE_WARNING_L + _T("\r\n") + pMainDlg->EOLLanguage.LP_EMERGENCY_LAMP, UPDATE_ON);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->LDW_WARN_L);

	BOOL bResult = TRUE;
	DWORD nStartTime = GetTickCount();
	CString strTestItem = pMainDlg->EOLLanguage.LP_LANE_WARNING_L + pMainDlg->EOLLanguage.LP_EMERGENCY_LAMP;
	CString strTgtMsg = _T("0");
	CString strSrcMsg = _T("0");
	int nListPos;
	int nRxLength;
	BYTE arbRxData[256];
	HUB_INFO *oOutput_COMM;

	// ADAS 입력 데이터
	( pMainDlg->bEnStep_LDW_Speed == 1 )? (m_nVehicleSpeed = pMainDlg->m_nVehicleLdwSpd) : (m_nVehicleSpeed = 100);
	m_bWinkL = 1;
	m_bWinkR = 1;

	nListPos = pMainDlg->m_list_test_process.GetItemCount();
	pMainDlg->m_list_test_process.InsertItem(nListPos, strTestItem);
	pMainDlg->m_list_test_process.SetItemText(nListPos, 1, strTgtMsg);

	while ( m_tThreadStop == false )
	{
		if ( m_JTP_Com.SendMsg_ADASInfoGet_R7(arbRxData, &nRxLength, m_nVehicleSpeed, m_bWinkL, m_bWinkR) == TRUE )
		{
			DISPLAY_ADAS_INFO(arbRxData, nRxLength);

			oOutput_COMM = new HUB_INFO;
			memcpy(oOutput_COMM, arbRxData, nRxLength);

			if ( (oOutput_COMM->bLaneDetectState & ADAS_LANE_LEFT_WARN) != 0 )
			{
				strSrcMsg.Format(_T("%d"), oOutput_COMM->bLaneDetectState & ADAS_LANE_LEFT_WARN );
				bResult = FALSE;
			}

			delete oOutput_COMM;
			oOutput_COMM = NULL;

			if ( bResult == FALSE ) break;
		}

		// 5초간 연결이 되지 않으면 Fail
		if ( (GetTickCount() - nStartTime) > 5000 )
		{
			strSrcMsg = _T("Comm Error");
			break;
		}
	}

	pMainDlg->m_list_test_process.SetItemText(nListPos, 2, strSrcMsg);
	
	if ( bResult == TRUE )
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("PASS"));
	}
	else
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("FAIL"));
	}

	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->LDW_DETECT);
	return bResult;
}

BOOL CEOL_R7::STEP_ADAS_LDW_RIGHT_WARN_WINK_L()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_LANE_WARNING_R + _T("\r\n") + pMainDlg->EOLLanguage.LP_WINKER_L, UPDATE_ON);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->LDW_WARN_R);

	BOOL bResult = FALSE;
	DWORD nStartTime = GetTickCount();
	CString strTestItem = pMainDlg->EOLLanguage.LP_LANE_WARNING_R + pMainDlg->EOLLanguage.LP_WINKER_L;
	CString strTgtMsg = _T("8");
	CString strSrcMsg = _T("0");
	int nListPos;
	int nRxLength;
	BYTE arbRxData[256];
	HUB_INFO *oOutput_COMM;

	// ADAS 입력 데이터
	( pMainDlg->bEnStep_LDW_Speed == 1 )? (m_nVehicleSpeed = pMainDlg->m_nVehicleLdwSpd) : (m_nVehicleSpeed = 100);
	m_bWinkL = 1;
	m_bWinkR = 0;

	nListPos = pMainDlg->m_list_test_process.GetItemCount();
	pMainDlg->m_list_test_process.InsertItem(nListPos, strTestItem);
	pMainDlg->m_list_test_process.SetItemText(nListPos, 1, strTgtMsg);

	while ( m_tThreadStop == false )
	{
		if ( m_JTP_Com.SendMsg_ADASInfoGet_R7(arbRxData, &nRxLength, m_nVehicleSpeed, m_bWinkL, m_bWinkR) == TRUE )
		{
			DISPLAY_ADAS_INFO(arbRxData, nRxLength);

			oOutput_COMM = new HUB_INFO;
			memcpy(oOutput_COMM, arbRxData, nRxLength);

			if ( (oOutput_COMM->bLaneDetectState & ADAS_LANE_RIGHT_WARN) != 0 )
			{
				strSrcMsg.Format(_T("%d"), oOutput_COMM->bLaneDetectState & ADAS_LANE_RIGHT_WARN );
				bResult = TRUE;
			}

			delete oOutput_COMM;
			oOutput_COMM = NULL;

			if ( bResult == TRUE ) break;
		}

		// 5초간 연결이 되지 않으면 Fail
		if ( (GetTickCount() - nStartTime) > 5000 )
		{
			strSrcMsg = _T("Comm Error");
			break;
		}
	}

	pMainDlg->m_list_test_process.SetItemText(nListPos, 2, strSrcMsg);
	
	if ( bResult == TRUE )
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("PASS"));
	}
	else
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("FAIL"));
	}

	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->LDW_DETECT);
	return bResult;
}

BOOL CEOL_R7::STEP_ADAS_LDW_RIGHT_WARN_WINK_R()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_LANE_WARNING_R + _T("\r\n") + pMainDlg->EOLLanguage.LP_WINKER_R, UPDATE_ON);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->LDW_WARN_R);

	BOOL bResult = TRUE;
	DWORD nStartTime = GetTickCount();
	CString strTestItem = pMainDlg->EOLLanguage.LP_LANE_WARNING_R + pMainDlg->EOLLanguage.LP_WINKER_R;
	CString strTgtMsg = _T("0");
	CString strSrcMsg = _T("0");
	int nListPos;
	int nRxLength;
	BYTE arbRxData[256];
	HUB_INFO *oOutput_COMM;

	// ADAS 입력 데이터
	( pMainDlg->bEnStep_LDW_Speed == 1 )? (m_nVehicleSpeed = pMainDlg->m_nVehicleLdwSpd) : (m_nVehicleSpeed = 100);
	m_bWinkL = 0;
	m_bWinkR = 1;

	nListPos = pMainDlg->m_list_test_process.GetItemCount();
	pMainDlg->m_list_test_process.InsertItem(nListPos, strTestItem);
	pMainDlg->m_list_test_process.SetItemText(nListPos, 1, strTgtMsg);

	while ( m_tThreadStop == false )
	{
		if ( m_JTP_Com.SendMsg_ADASInfoGet_R7(arbRxData, &nRxLength, m_nVehicleSpeed, m_bWinkL, m_bWinkR) == TRUE )
		{
			DISPLAY_ADAS_INFO(arbRxData, nRxLength);

			oOutput_COMM = new HUB_INFO;
			memcpy(oOutput_COMM, arbRxData, nRxLength);

			if ( (oOutput_COMM->bLaneDetectState & ADAS_LANE_RIGHT_WARN) != 0 )
			{
				strSrcMsg.Format(_T("%d"), oOutput_COMM->bLaneDetectState & ADAS_LANE_RIGHT_WARN );
				bResult = FALSE;
			}

			delete oOutput_COMM;
			oOutput_COMM = NULL;

			if ( bResult == FALSE ) break;
		}

		// 5초간 연결이 되지 않으면 Fail
		if ( (GetTickCount() - nStartTime) > 5000 )
		{
			strSrcMsg = _T("Comm Error");
			break;
		}
	}

	pMainDlg->m_list_test_process.SetItemText(nListPos, 2, strSrcMsg);
	
	if ( bResult == TRUE )
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("PASS"));
	}
	else
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("FAIL"));
	}

	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->LDW_DETECT);
	return bResult;
}

BOOL CEOL_R7::STEP_ADAS_LDW_RIGHT_WARN_WINK_LR()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_LANE_WARNING_R + _T("\r\n") + pMainDlg->EOLLanguage.LP_EMERGENCY_LAMP, UPDATE_ON);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->LDW_WARN_R);

	BOOL bResult = TRUE;
	DWORD nStartTime = GetTickCount();
	CString strTestItem = pMainDlg->EOLLanguage.LP_LANE_WARNING_R + pMainDlg->EOLLanguage.LP_EMERGENCY_LAMP;
	CString strTgtMsg = _T("0");
	CString strSrcMsg = _T("0");
	int nListPos;
	int nRxLength;
	BYTE arbRxData[256];
	HUB_INFO *oOutput_COMM;

	// ADAS 입력 데이터
	( pMainDlg->bEnStep_LDW_Speed == 1 )? (m_nVehicleSpeed = pMainDlg->m_nVehicleLdwSpd) : (m_nVehicleSpeed = 100);
	m_bWinkL = 1;
	m_bWinkR = 1;

	nListPos = pMainDlg->m_list_test_process.GetItemCount();
	pMainDlg->m_list_test_process.InsertItem(nListPos, strTestItem);
	pMainDlg->m_list_test_process.SetItemText(nListPos, 1, strTgtMsg);

	while ( m_tThreadStop == false )
	{
		if ( m_JTP_Com.SendMsg_ADASInfoGet_R7(arbRxData, &nRxLength, m_nVehicleSpeed, m_bWinkL, m_bWinkR) == TRUE )
		{
			DISPLAY_ADAS_INFO(arbRxData, nRxLength);

			oOutput_COMM = new HUB_INFO;
			memcpy(oOutput_COMM, arbRxData, nRxLength);

			if ( (oOutput_COMM->bLaneDetectState & ADAS_LANE_RIGHT_WARN) != 0 )
			{
				strSrcMsg.Format(_T("%d"), oOutput_COMM->bLaneDetectState & ADAS_LANE_RIGHT_WARN );
				bResult = FALSE;
			}

			delete oOutput_COMM;
			oOutput_COMM = NULL;

			if ( bResult == FALSE ) break;
		}

		// 5초간 연결이 되지 않으면 Fail
		if ( (GetTickCount() - nStartTime) > 5000 )
		{
			strSrcMsg = _T("Comm Error");
			break;
		}
	}

	pMainDlg->m_list_test_process.SetItemText(nListPos, 2, strSrcMsg);
	
	if ( bResult == TRUE )
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("PASS"));
	}
	else
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("FAIL"));
	}

	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->LDW_DETECT);
	return bResult;
}

BOOL CEOL_R7::STEP_ADAS_FCDA_WARN()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, _T("FCDA"), UPDATE_ON);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->FCDA_CAR_DETECT);
	Sleep(1000);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->FCDA_CAR_WARN);

	BOOL bResult = FALSE;
	DWORD nStartTime = GetTickCount();
	CString strTestItem = _T("FCDA");
	CString strTgtMsg = _T("256");
	CString strSrcMsg = _T("0");
	int nListPos;
	int nRxLength;
	BYTE arbRxData[256];
	HUB_INFO *oOutput_COMM;

	// ADAS 입력 데이터
	m_nVehicleSpeed = 0;
	m_bWinkL = 0;
	m_bWinkR = 0;

	nListPos = pMainDlg->m_list_test_process.GetItemCount();
	pMainDlg->m_list_test_process.InsertItem(nListPos, strTestItem);
	pMainDlg->m_list_test_process.SetItemText(nListPos, 1, strTgtMsg);

	while ( m_tThreadStop == false )
	{
		if ( m_JTP_Com.SendMsg_ADASInfoGet_R7(arbRxData, &nRxLength, m_nVehicleSpeed, m_bWinkL, m_bWinkR) == TRUE )
		{
			DISPLAY_ADAS_INFO(arbRxData, nRxLength);

			oOutput_COMM = new HUB_INFO;
			memcpy(oOutput_COMM, arbRxData, nRxLength);

			if ( (oOutput_COMM->uFcwState & ADAS_FCDA_WARN) != 0 )
			{
				strSrcMsg.Format(_T("%d"), oOutput_COMM->uFcwState & ADAS_FCDA_WARN );
				bResult = TRUE;
			}			

			delete oOutput_COMM;
			oOutput_COMM = NULL;

			if ( bResult == TRUE ) break;
		}

		// 5초간 연결이 되지 않으면 Fail
		if ( (GetTickCount() - nStartTime) > 5000 )
		{
			strSrcMsg = _T("Comm Error");
			break;
		}
	}

	pMainDlg->m_list_test_process.SetItemText(nListPos, 2, strSrcMsg);
	
	if ( bResult == TRUE )
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("PASS"));
	}
	else
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("FAIL"));
	}

	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->LDW_DETECT);
	return bResult;
}

BOOL CEOL_R7::STEP_ADAS_FCW_WARN()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_FCW_WARNING, UPDATE_ON);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->FCW_CAR_DETECT);
	Sleep(1000);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->FCW_CAR_WARN);

	BOOL bResult = FALSE;
	DWORD nStartTime = GetTickCount();
	CString strTestItem = pMainDlg->EOLLanguage.LP_FCW_WARNING;
	CString strTgtMsg = _T("2,8");
	CString strSrcMsg = _T("0");
	int nListPos;
	int nRxLength;
	BYTE arbRxData[256];
	HUB_INFO *oOutput_COMM;

	// ADAS 입력 데이터
	( pMainDlg->bEnStep_FCW_Speed == 1 )? (m_nVehicleSpeed = pMainDlg->m_nVehicleFcwSpd) : (m_nVehicleSpeed = 100);
	m_bWinkL = 0;
	m_bWinkR = 0;

	nListPos = pMainDlg->m_list_test_process.GetItemCount();
	pMainDlg->m_list_test_process.InsertItem(nListPos, strTestItem);
	pMainDlg->m_list_test_process.SetItemText(nListPos, 1, strTgtMsg);

	while ( m_tThreadStop == false )
	{
		if ( m_JTP_Com.SendMsg_ADASInfoGet_R7(arbRxData, &nRxLength, m_nVehicleSpeed, m_bWinkL, m_bWinkR) == TRUE )
		{
			DISPLAY_ADAS_INFO(arbRxData, nRxLength);

			oOutput_COMM = new HUB_INFO;
			memcpy(oOutput_COMM, arbRxData, nRxLength);

			if ( (oOutput_COMM->uFcwState & ADAS_FCW_HMW_WARN) != 0 )
			{
				strSrcMsg.Format(_T("%d"), oOutput_COMM->uFcwState & ADAS_FCW_HMW_WARN );
				bResult = TRUE;
			}
			else if ( (oOutput_COMM->uFcwState & ADAS_FCW_HMW_WARN) != 0 )
			{
				strSrcMsg.Format(_T("%d"), oOutput_COMM->uFcwState & ADAS_FCW_FCW_WARN );
				bResult = TRUE;
			}

			delete oOutput_COMM;
			oOutput_COMM = NULL;

			if ( bResult == TRUE ) break;
		}

		// 5초간 연결이 되지 않으면 Fail
		if ( (GetTickCount() - nStartTime) > 5000 )
		{
			strSrcMsg = _T("Comm Error");
			break;
		}
	}

	pMainDlg->m_list_test_process.SetItemText(nListPos, 2, strSrcMsg);
	
	if ( bResult == TRUE )
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("PASS"));
	}
	else
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("FAIL"));
	}

	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->LDW_DETECT);
	return bResult;
}

BOOL CEOL_R7::STEP_ADAS_FCW_WARN2()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_FCW_WARNING + _T("2"), UPDATE_ON);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->FCW_CAR_DETECT2);
	Sleep(1000);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->FCW_CAR_WARN2);

	BOOL bResult = FALSE;
	DWORD nStartTime = GetTickCount();
	CString strTestItem = pMainDlg->EOLLanguage.LP_FCW_WARNING + _T("2");
	CString strTgtMsg = _T("2,8");
	CString strSrcMsg = _T("0");
	int nListPos;
	int nRxLength;
	BYTE arbRxData[256];
	HUB_INFO *oOutput_COMM;

	// ADAS 입력 데이터
	( pMainDlg->bEnStep_FCW_Speed == 1 )? (m_nVehicleSpeed = pMainDlg->m_nVehicleFcwSpd) : (m_nVehicleSpeed = 100);
	m_bWinkL = 0;
	m_bWinkR = 0;

	nListPos = pMainDlg->m_list_test_process.GetItemCount();
	pMainDlg->m_list_test_process.InsertItem(nListPos, strTestItem);
	pMainDlg->m_list_test_process.SetItemText(nListPos, 1, strTgtMsg);

	while ( m_tThreadStop == false )
	{
		if ( m_JTP_Com.SendMsg_ADASInfoGet_R7(arbRxData, &nRxLength, m_nVehicleSpeed, m_bWinkL, m_bWinkR) == TRUE )
		{
			DISPLAY_ADAS_INFO(arbRxData, nRxLength);

			oOutput_COMM = new HUB_INFO;
			memcpy(oOutput_COMM, arbRxData, nRxLength);

			if ( (oOutput_COMM->uFcwState & ADAS_FCW_HMW_WARN) != 0 )
			{
				strSrcMsg.Format(_T("%d"), oOutput_COMM->uFcwState & ADAS_FCW_HMW_WARN );
				bResult = TRUE;
			}
			else if ( (oOutput_COMM->uFcwState & ADAS_FCW_HMW_WARN) != 0 )
			{
				strSrcMsg.Format(_T("%d"), oOutput_COMM->uFcwState & ADAS_FCW_FCW_WARN );
				bResult = TRUE;
			}

			delete oOutput_COMM;
			oOutput_COMM = NULL;

			if ( bResult == TRUE ) break;
		}

		// 5초간 연결이 되지 않으면 Fail
		if ( (GetTickCount() - nStartTime) > 5000 )
		{
			strSrcMsg = _T("Comm Error");
			break;
		}
	}

	pMainDlg->m_list_test_process.SetItemText(nListPos, 2, strSrcMsg);
	
	if ( bResult == TRUE )
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("PASS"));
	}
	else
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("FAIL"));
	}

	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->LDW_DETECT);
	return bResult;
}

BOOL CEOL_R7::STEP_ADAS_FCW_WARN3()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_FCW_WARNING + _T("3"), UPDATE_ON);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->FCW_CAR_DETECT3);
	Sleep(1000);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->FCW_CAR_WARN3);

	BOOL bResult = FALSE;
	DWORD nStartTime = GetTickCount();
	CString strTestItem = pMainDlg->EOLLanguage.LP_FCW_WARNING + _T("3");
	CString strTgtMsg = _T("2,8");
	CString strSrcMsg = _T("0");
	int nListPos;
	int nRxLength;
	BYTE arbRxData[256];
	HUB_INFO *oOutput_COMM;

	// ADAS 입력 데이터
	( pMainDlg->bEnStep_FCW_Speed == 1 )? (m_nVehicleSpeed = pMainDlg->m_nVehicleFcwSpd) : (m_nVehicleSpeed = 100);
	m_bWinkL = 0;
	m_bWinkR = 0;

	nListPos = pMainDlg->m_list_test_process.GetItemCount();
	pMainDlg->m_list_test_process.InsertItem(nListPos, strTestItem);
	pMainDlg->m_list_test_process.SetItemText(nListPos, 1, strTgtMsg);

	while ( m_tThreadStop == false )
	{
		if ( m_JTP_Com.SendMsg_ADASInfoGet_R7(arbRxData, &nRxLength, m_nVehicleSpeed, m_bWinkL, m_bWinkR) == TRUE )
		{
			DISPLAY_ADAS_INFO(arbRxData, nRxLength);

			oOutput_COMM = new HUB_INFO;
			memcpy(oOutput_COMM, arbRxData, nRxLength);

			if ( (oOutput_COMM->uFcwState & ADAS_FCW_HMW_WARN) != 0 )
			{
				strSrcMsg.Format(_T("%d"), oOutput_COMM->uFcwState & ADAS_FCW_HMW_WARN );
				bResult = TRUE;
			}
			else if ( (oOutput_COMM->uFcwState & ADAS_FCW_HMW_WARN) != 0 )
			{
				strSrcMsg.Format(_T("%d"), oOutput_COMM->uFcwState & ADAS_FCW_FCW_WARN );
				bResult = TRUE;
			}

			delete oOutput_COMM;
			oOutput_COMM = NULL;

			if ( bResult == TRUE ) break;
		}

		// 5초간 연결이 되지 않으면 Fail
		if ( (GetTickCount() - nStartTime) > 5000 )
		{
			strSrcMsg = _T("Comm Error");
			break;
		}
	}

	pMainDlg->m_list_test_process.SetItemText(nListPos, 2, strSrcMsg);
	
	if ( bResult == TRUE )
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("PASS"));
	}
	else
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("FAIL"));
	}

	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->LDW_DETECT);
	return bResult;
}

BOOL CEOL_R7::STEP_DTC_ALL_CLEAR()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, _T("DTC ALL CLEAR"), UPDATE_ON);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->CALIBRATION);

	BOOL bResult = FALSE;
	DWORD nStartTime = GetTickCount();
	CString strTestItem = _T("DTC ALL CLEAR");
	CString strTgtMsg = _T("1");
	CString strSrcMsg = _T("0");
	int nListPos;

	nListPos = pMainDlg->m_list_test_process.GetItemCount();
	pMainDlg->m_list_test_process.InsertItem(nListPos, strTestItem);
	pMainDlg->m_list_test_process.SetItemText(nListPos, 1, strTgtMsg);

	while ( m_tThreadStop == false )
	{
		if ( m_JTP_Com.SendMsg_GetDTCData(4, 0, NULL, NULL) == TRUE )
		{
			bResult = TRUE;
			break;
		}

		// 5초간 연결이 되지 않으면 Fail
		if ( (GetTickCount() - nStartTime) > 5000 )
		{
			strSrcMsg = _T("Comm Error");
			break;
		}
	}

	pMainDlg->m_list_test_process.SetItemText(nListPos, 2, strSrcMsg);
	
	if ( bResult == TRUE )
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("PASS"));
	}
	else
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("FAIL"));
	}

	return bResult;
}

BOOL CEOL_R7::STEP_CHECK_BARCODE2_SERIAL()
{
	pMainDlg->m_edit_notice.SetText(SOFT_GREEN, BLACK, pMainDlg->EOLLanguage.LP_CASE_BARCODE, UPDATE_ON);
	pMainDlg->pImgDlg->ImgSet(pMainDlg->pImgDlg->CALIBRATION);

	BOOL bResult = FALSE;
	DWORD nStartTime;
	CString strTestItem = pMainDlg->EOLLanguage.LP_CASE_BARCODE;
	CString strTgtMsg = _T("-");
	CString strSrcMsg = _T("0");
	int nListPos;
	int preBarcodeCount;

	nListPos = pMainDlg->m_list_test_process.GetItemCount();
	pMainDlg->m_list_test_process.InsertItem(nListPos, strTestItem);
	pMainDlg->m_list_test_process.SetItemText(nListPos, 1, strTgtMsg);

	pIoBoard->PutOutBit(_BIT_OUT_BARCODE2, false);
	preBarcodeCount = pBarCode2->GetCount();
	nStartTime = GetTickCount();
	while ( m_tThreadStop == false )
	{
		pIoBoard->PutOutBit(_BIT_OUT_BARCODE2, true);
		if ( pBarCode2->GetCount() > preBarcodeCount)
		{
			strSrcMsg = (LPCSTR)pBarCode2->GetDataAt(preBarcodeCount);

			if ( strSrcMsg.Compare(strSerialNum) == 0 )
			{
				bResult = TRUE;
			}
			break;
		}
#if 0
		// 30초간 연결이 되지 않으면 Fail
		if ( (GetTickCount() - nStartTime) > 30000 )
		{
			strSrcMsg = _T("Comm Error");
			break;
		}
#endif
	}
	pIoBoard->PutOutBit(_BIT_OUT_BARCODE2, false);

	pMainDlg->m_list_test_process.SetItemText(nListPos, 2, strSrcMsg);
	
	if ( bResult == TRUE )
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("PASS"));
	}
	else
	{
		pMainDlg->m_list_test_process.SetItemText(nListPos, 3, _T("FAIL"));
	}

	return bResult;
}