#include "stdafx.h"
#include "y_LanguagePack.h"

CString EOL_LANGUAGE[N_LANGUAGE_INDEX_MAX][L_LANGUAGE_TYPE_MAX] = 
{
	// KOREAN
	// CHINESE
	// ENGLISH
	{ _T("R7 EOL 검사 프로그램"), _T("R7 EOL 检查程序"), _T("R7 EOL Test Program") },
	{ _T("입력 데이터"), _T("输入数据"), _T("Input Data") },
	{ _T("차량 속도"), _T("车辆速度"), _T("Vehicle Speed") },
	{ _T("방향지시등 신호"), _T("转向灯信号"), _T("Winker") },
	{ _T("좌"), _T("左"), _T("Left") },
	{ _T("우"), _T("右"), _T("Right") },
	{ _T("출력 데이터"), _T("输出数据"), _T("Output Data") },
	{ _T("차선 인식"), _T("车线识别"), _T("Lane Detect") },
	{ _T("차선 이탈 경보"), _T("车线脱离预警"), _T("Lane Warning") },
	{ _T("HMW 경보"), _T("HMW预警"), _T("HMW Warning") },
	{ _T("FCW 경보"), _T("FCW预警"), _T("FCW Warning") },
	{ _T("공정 카운트"), _T("工程次数"), _T("Process Count") },
	{ _T("라인명"), _T("线（流程）名"), _T("Factory Lane") },
	{ _T("검사자"), _T("检查者"), _T("Tester") },
	{ _T("검사 시간"), _T("检查时间"), _T("Test Time") },
	{ _T("제품명"), _T("产品名"), _T("Product Name") },
	{ _T("양품"), _T("良品"), _T("PASS") },
	{ _T("불량"), _T("不良品"), _T("FAIL") },
	{ _T("전체"), _T("全部"), _T("TOTAL") },
	{ _T("양품률"), _T("良品率"), _T("PASS Rate") },
	{ _T("검사항목"), _T("检查项目"), _T("Test Content") },
	{ _T("목표값"), _T("目标值"), _T("Target") },
	{ _T("측정값"), _T("测定值"), _T("Check") },
	{ _T("결과"), _T("结果"), _T("Result") },
	{ _T("양산 최종검사"), _T("量产最终检查"), _T(" EOL TEST") },
	{ _T("검사 대기중"), _T("检查待机"), _T("Test Waiting") },
	{ _T("IO 보드"), _T("IO板"), _T("IO Board") },
	{ _T("바코드 스캐너1"), _T("条码扫描仪1"), _T("Barcode1") },
	{ _T("바코드 스캐너2"), _T("条码扫描仪2"), _T("Barcode2") },
	{ _T("케이스 바코드"), _T("外壳条码识别"), _T("Case Barcode") },
	{ _T("전원"), _T("电源"), _T("Power") },	
	{ _T("연결 실패"), _T("连接失败"), _T("Connection FAIL") },
	{ _T("통신 실패"), _T("通信失败"), _T("Communication FAIL") },
	{ _T("검사중"), _T("检查中"), _T("Testing") },
	{ _T("시리얼 번호"), _T("序列号"), _T("Serial Number") },
	{ _T("소프트웨어 버전"), _T("软件版本"), _T("Software Version") },
	{ _T("MCU 버전"), _T("MCU版本"), _T("MCU Version") },
	{ _T("가속도값"), _T("加速度值确认失败"), _T("Acceleration") },
	{ _T("속도 펄스값"), _T("速度脉冲值确认失败"), _T("Speed Pulse") },
	{ _T("확인 실패"), _T("确认失败"), _T("Confirm FAIL") },
	{ _T("차선 인식 실패"), _T("车道线识别失败"), _T("Lane Detect FAIL") },
	{ _T("차선 좌측 경보 실패"), _T("车道线左侧预警失败"), _T("Lane Warning L FAIL") },
	{ _T("차선 우측 경보 실패"), _T("车道线右侧预警失败"), _T("Lane Warning R FAIL") },
	{ _T("방향 지시등 (좌)"), _T("转向灯（左）"), _T("Winker L") },
	{ _T("방향 지시등 (우)"), _T("转向灯（右）"), _T("Winker R") },
	{ _T("비상 정지등"), _T("紧急信号灯"), _T("Emergency Lamp") },
	{ _T("차량 충돌 경보 실패"), _T("车辆碰撞预警失败"), _T("FCW Warning FAIL") },
	{ _T("경보 실패"), _T("预警失败"), _T("Warning FAIL") },
	{ _T("DTC 삭제 실패"), _T("DTC删除失败"), _T("DTC Delete FAIL") },
	{ _T("전원 해제 실패"), _T("电源解除失败"), _T("Power Off FAIL") },
	{ _T("케이스 바코드 실패"), _T("外壳条码识别失败"), _T("Case Barcode FAIL") },
	{ _T("검사 통과"), _T("检查通过"), _T("Test PASS") },
	{ _T("검사 실패"), _T("检查失败"), _T("Test FAIL") },
	{ _T("불량 발생"), _T("不良发生"), _T("Fail Occurs") },
	{ _T("검사 종료"), _T("检查结束"), _T("Test End") },
	{ _T("연결중"), _T("连接中"), _T("Connecting") },
	{ _T("제품을 안착 시키고 연결 한 후에\r\n시작 버튼을 누르세요"), _T("安置并链接产品后，\r\n请按开始按钮"), _T("Set and Connect product,\r\nand then Push the start button") },
	{ _T("전원을 해제 합니다"), _T("解除电源"), _T("Power Off") },
	{ _T("통신연결 확인"), _T("确认通信连接"), _T("Connection Check") },
	{ _T("LDWS 차선 좌측 이탈"), _T("LDWS车线左侧脱离"), _T("LDWS Warning Left") },
	{ _T("LDWS 차선 우측 이탈"), _T("LDWS车线右侧脱离"), _T("LDWS Warning Right") },
	{ _T("시작"), _T("开始"), _T("START") },
	{ _T("닫기"), _T("关"), _T("CLOSE") },
	{ _T("중단"), _T("暂停"), _T("PAUSE") }
};

int CLanguagePack::SET_LANGUAE_PACK(int languageType)
{
	if ( languageType >= L_LANGUAGE_TYPE_MAX )	languageType = L_ENGLISH;

	LP_R7_EOL_PROGRAM = EOL_LANGUAGE[ N_R7_EOL_PROGRAM][ languageType ];
	LP_INPUT_DATA = EOL_LANGUAGE[ N_INPUT_DATA][ languageType ];
	LP_VEHICLE_SPEED = EOL_LANGUAGE[ N_VEHICLE_SPEED][ languageType ];
	LP_WINKER = EOL_LANGUAGE[ N_WINKER][ languageType ];
	LP_LEFT = EOL_LANGUAGE[ N_LEFT][ languageType ];
	LP_RIGHT = EOL_LANGUAGE[ N_RIGHT][ languageType ];
	LP_OUTPUT_DATA = EOL_LANGUAGE[ N_OUTPUT_DATA][ languageType ];
	LP_LANE_DETECT = EOL_LANGUAGE[ N_LANE_DETECT][ languageType ];
	LP_LANE_WARNING = EOL_LANGUAGE[ N_LANE_WARNING][ languageType ];
	LP_HMW_WARNING = EOL_LANGUAGE[ N_HMW_WARNING][ languageType ];
	LP_FCW_WARNING = EOL_LANGUAGE[ N_FCW_WARNING][ languageType ];
	LP_PROCESS_COUNT = EOL_LANGUAGE[ N_PROCESS_COUNT][ languageType ];
	LP_FACTORY_LANE = EOL_LANGUAGE[ N_FACTORY_LANE][ languageType ];
	LP_TESTER = EOL_LANGUAGE[ N_TESTER][ languageType ];
	LP_TEST_TIME = EOL_LANGUAGE[ N_TEST_TIME][ languageType ];
	LP_PRODUCT_NAME = EOL_LANGUAGE[ N_PRODUCT_NAME][ languageType ];
	LP_PASS = EOL_LANGUAGE[ N_PASS][ languageType ];
	LP_FAIL = EOL_LANGUAGE[ N_FAIL][ languageType ];
	LP_TOTAL = EOL_LANGUAGE[ N_TOTAL][ languageType ];
	LP_PASS_RATE = EOL_LANGUAGE[ N_PASS_RATE][ languageType ];
	LP_TEST_INDEX = EOL_LANGUAGE[ N_TEST_INDEX][ languageType ];
	LP_TARGET = EOL_LANGUAGE[ N_TARGET][ languageType ];
	LP_CHECK = EOL_LANGUAGE[ N_CHECK][ languageType ];
	LP_RESULT = EOL_LANGUAGE[ N_RESULT][ languageType ];
	LP_LAST_TEST = EOL_LANGUAGE[ N_LAST_TEST][ languageType ];
	LP_TEST_WAITING = EOL_LANGUAGE[ N_TEST_WAITING][ languageType ];
	LP_IO_BOARD = EOL_LANGUAGE[ N_IO_BOARD][ languageType ];
	LP_BARCODE1 = EOL_LANGUAGE[ N_BARCODE1][ languageType ];
	LP_BARCODE2 = EOL_LANGUAGE[ N_BARCODE2][ languageType ];
	LP_CASE_BARCODE = EOL_LANGUAGE[ N_CASE_BARCODE][ languageType ];
	LP_POWER = EOL_LANGUAGE[ N_POWER][ languageType ];
	LP_CONNECT_FAIL = EOL_LANGUAGE[ N_CONNECT_FAIL][ languageType ];
	LP_COMMUNICATION_FAIL = EOL_LANGUAGE[ N_COMMUNICATION_FAIL][ languageType ];
	LP_TESTING = EOL_LANGUAGE[ N_TESTING][ languageType ];
	LP_SERIAL_NUMBER = EOL_LANGUAGE[ N_SERIAL_NUMBER][ languageType ];
	LP_SOFTWARE_VERSION = EOL_LANGUAGE[ N_SOFTWARE_VERSION][ languageType ];
	LP_MCU_VERSION = EOL_LANGUAGE[ N_MCU_VERSION][ languageType ];
	LP_ACCELERATION = EOL_LANGUAGE[ N_ACCELERATION][ languageType ];
	LP_SPEED_PULSE = EOL_LANGUAGE[ N_SPEED_PULSE][ languageType ];
	LP_CONFIRM_FAIL = EOL_LANGUAGE[ N_CONFIRM_FAIL][ languageType ];
	LP_LANE_DETECT_FAIL = EOL_LANGUAGE[ N_LANE_DETECT_FAIL][ languageType ];
	LP_LANE_WARNING_L_FAIL = EOL_LANGUAGE[ N_LANE_WARNING_L_FAIL][ languageType ];
	LP_LANE_WARNING_R_FAIL = EOL_LANGUAGE[ N_LANE_WARNING_R_FAIL][ languageType ];
	LP_WINKER_L = EOL_LANGUAGE[ N_WINKER_L][ languageType ];
	LP_WINKER_R = EOL_LANGUAGE[ N_WINKER_R][ languageType ];
	LP_EMERGENCY_LAMP = EOL_LANGUAGE[ N_EMERGENCY_LAMP][ languageType ];
	LP_FCW_WARNING_FAIL = EOL_LANGUAGE[ N_FCW_WARNING_FAIL][ languageType ];
	LP_WARNING_FAIL = EOL_LANGUAGE[ N_WARNING_FAIL][ languageType ];
	LP_DTC_DELETE_FAIL = EOL_LANGUAGE[ N_DTC_DELETE_FAIL][ languageType ];
	LP_POWER_OFF_FAIL = EOL_LANGUAGE[ N_POWER_OFF_FAIL][ languageType ];
	LP_CASE_BARCODE_FAIL = EOL_LANGUAGE[ N_CASE_BARCODE_FAIL][ languageType ];
	LP_TEST_PASS = EOL_LANGUAGE[ N_TEST_PASS][ languageType ];
	LP_TEST_FAIL = EOL_LANGUAGE[ N_TEST_FAIL][ languageType ];
	LP_FAIL_OCCURS = EOL_LANGUAGE[ N_FAIL_OCCURS][ languageType ];
	LP_TEST_END = EOL_LANGUAGE[ N_TEST_END][ languageType ];
	LP_CONNECTING = EOL_LANGUAGE[ N_CONNECTING][ languageType ];
	LP_TEST_PROCESS_START = EOL_LANGUAGE[ N_TEST_PROCESS_START][ languageType ];
	LP_POWER_OFF = EOL_LANGUAGE[ N_POWER_OFF][ languageType ];
	LP_CONNECTION_CHECK = EOL_LANGUAGE[ N_CONNECTION_CHECK][ languageType ];
	LP_LANE_WARNING_L = EOL_LANGUAGE[ N_LANE_WARNING_L][ languageType ];
	LP_LANE_WARNING_R = EOL_LANGUAGE[ N_LANE_WARNING_R][ languageType ];
	LP_START = EOL_LANGUAGE[ N_START][ languageType ];
	LP_CLOSE = EOL_LANGUAGE[ N_CLOSE][ languageType ];
	LP_PAUSE = EOL_LANGUAGE[ N_PAUSE][ languageType ];

	return 1;
}


CLanguagePack::CLanguagePack()
{
}
CLanguagePack::~CLanguagePack()
{
}