#ifndef _EOL_TEST_R7_H
#define _EOL_TEST_R7_H

#define PASS true
#define FAIL false

class CEOL_R7
{
public:
	CEOL_R7();
	~CEOL_R7();

public:
	//--------- 외부 환경 변수 ------------------------------------//
	bool m_EOLTestResult;
	//--------- 외부 사용 함수 ------------------------------------//
	BOOL START_EOL_TEST();
	BOOL END_EOL_TEST();

	// Thread for Run
    bool m_tThreadStop;
	CWinThread * m_pThread;

    friend UINT ThreadEolTest_R7(LPVOID pParam);
private:
	//--------- 내부 환경 변수 ------------------------------------//
	BOOL bTestRunning;
	BOOL bLogStart;
	CString strSerialNum;

	int m_nVehicleSpeed;
	BOOL m_bWinkL;
	BOOL m_bWinkR;
	//--------- 내부 사용 함수 ------------------------------------//
	BOOL STEP_CONNECT_IOBOARD();
	BOOL STEP_CONNECT_BARCODE1();
	BOOL STEP_CONNECT_BARCODE2();
	BOOL CloseIoBoard();
	BOOL CloseBarcode1();
	BOOL CloseBarcode2();

	BOOL START_EOL_TEST_LOG();
	BOOL SAVE_EOL_TEST_LOG();
	BOOL END_EOL_TEST_LOG();

	BOOL DISPLAY_ADAS_INFO(void *a_pAdasInfoData, int a_puDataLen);

	BOOL STEP_CONNECT_POWER();
	BOOL STEP_DISCONNECT_POWER();
	BOOL STEP_CONNECT_USART();
	BOOL STEP_CHK_SERIAL();
	BOOL STEP_CHK_SW_VERSION();
	BOOL STEP_CHK_MCU_VERSION();
	BOOL STEP_CHK_VANISH();
	BOOL STEP_CHK_SPEED_PULSE();
	BOOL STEP_ADAS_LDW_DETECT();
	BOOL STEP_ADAS_LDW_LEFT_WARN();
	BOOL STEP_ADAS_LDW_RIGHT_WARN();
	BOOL STEP_ADAS_LDW_LEFT_WARN_WINK_L();
	BOOL STEP_ADAS_LDW_LEFT_WARN_WINK_R();
	BOOL STEP_ADAS_LDW_LEFT_WARN_WINK_LR();
	BOOL STEP_ADAS_LDW_RIGHT_WARN_WINK_L();
	BOOL STEP_ADAS_LDW_RIGHT_WARN_WINK_R();
	BOOL STEP_ADAS_LDW_RIGHT_WARN_WINK_LR();
	BOOL STEP_ADAS_FCDA_WARN();
	BOOL STEP_ADAS_FCW_WARN();
	BOOL STEP_ADAS_FCW_WARN2();	
	BOOL STEP_ADAS_FCW_WARN3();
	BOOL STEP_DTC_ALL_CLEAR();
	BOOL STEP_CHECK_BARCODE2_SERIAL();
};





#endif //  !_EOL_TEST_R7_H  //