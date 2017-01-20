#ifndef Y_LANGUAGEPACK_H
#define Y_LANGUAGEPACK_H

enum _LANGUAGE_TYPE
{
	L_KOREAN = 0,
	L_CHINESE,
	L_ENGLISH,
	L_LANGUAGE_TYPE_MAX
};

enum _LANGUAGE_INDEX
{
	N_R7_EOL_PROGRAM = 0,
	N_INPUT_DATA,
	N_VEHICLE_SPEED,
	N_WINKER,
	N_LEFT,
	N_RIGHT,
	N_OUTPUT_DATA,
	N_LANE_DETECT,
	N_LANE_WARNING,
	N_HMW_WARNING,
	N_FCW_WARNING,
	N_PROCESS_COUNT,
	N_FACTORY_LANE,
	N_TESTER,
	N_TEST_TIME,
	N_PRODUCT_NAME,
	N_PASS,
	N_FAIL,
	N_TOTAL,
	N_PASS_RATE,
	N_TEST_INDEX,
	N_TARGET,
	N_CHECK,
	N_RESULT,
	N_LAST_TEST,
	N_TEST_WAITING,
	N_IO_BOARD,
	N_BARCODE1,
	N_BARCODE2,
	N_CASE_BARCODE,
	N_POWER,
	N_CONNECT_FAIL,
	N_COMMUNICATION_FAIL,
	N_TESTING,
	N_SERIAL_NUMBER,
	N_SOFTWARE_VERSION,
	N_MCU_VERSION,
	N_ACCELERATION,
	N_SPEED_PULSE,
	N_CONFIRM_FAIL,
	N_LANE_DETECT_FAIL,
	N_LANE_WARNING_L_FAIL,
	N_LANE_WARNING_R_FAIL,
	N_WINKER_L,
	N_WINKER_R,
	N_EMERGENCY_LAMP,
	N_FCW_WARNING_FAIL,
	N_WARNING_FAIL,
	N_DTC_DELETE_FAIL,
	N_POWER_OFF_FAIL,
	N_CASE_BARCODE_FAIL,
	N_TEST_PASS,
	N_TEST_FAIL,
	N_FAIL_OCCURS,
	N_TEST_END,
	N_CONNECTING,
	N_TEST_PROCESS_START,
	N_POWER_OFF,
	N_CONNECTION_CHECK,
	N_LANE_WARNING_L,
	N_LANE_WARNING_R,
	N_START,
	N_CLOSE,
	N_PAUSE,
	N_LANGUAGE_INDEX_MAX
};

class CLanguagePack
{
public:
	CLanguagePack();
	~CLanguagePack();

	//--------- 외부 환경 변수 ------------------------------------//
	CString LP_R7_EOL_PROGRAM;		// R7 EOL 검사 프로그램
	CString LP_INPUT_DATA;			// 입력 데이터
	CString LP_VEHICLE_SPEED;		// 차량 속도
	CString LP_WINKER;				// 깜박이 신호
	CString LP_LEFT;				// 좌
	CString LP_RIGHT;				// 우
	CString LP_OUTPUT_DATA;			// 출력 데이터
	CString LP_LANE_DETECT;			// 차선 인식
	CString LP_LANE_WARNING;		// 차선 이탈 경보
	CString LP_HMW_WARNING;			// HMW 경보
	CString LP_FCW_WARNING;			// FCW 경보
	CString LP_PROCESS_COUNT;		// 공정 카운트
	CString LP_FACTORY_LANE;		// 라인명
	CString LP_TESTER;				// 검사자
	CString	LP_TEST_TIME;			// 검사 시간
	CString LP_PRODUCT_NAME;		// 제품명
	CString LP_PASS;				// 양품
	CString LP_FAIL;				// 불량
	CString LP_TOTAL;				// 전체
	CString LP_PASS_RATE;			// 양품률
	CString LP_TEST_INDEX;			// 검사 항목
	CString LP_TARGET;				// 목표값
	CString LP_CHECK;				// 측정값
	CString LP_RESULT;				// 결과값
	CString LP_LAST_TEST;			// 양산 최종결과
	CString LP_TEST_WAITING;		// 검사 대기중
	CString LP_IO_BOARD;			// IO 보드
	CString LP_BARCODE1;			// 바코드 스캐너1
	CString LP_BARCODE2;			// 바코드 스캐너2
	CString LP_CASE_BARCODE;		// 케이스 바코드
	CString LP_POWER;				// 전원
	CString LP_CONNECT_FAIL;		// 연결 실패
	CString LP_COMMUNICATION_FAIL;	// 통신 실패
	CString LP_TESTING;				// 검사중
	CString LP_SERIAL_NUMBER;		// 시리얼 번호
	CString LP_SOFTWARE_VERSION;	// 소프트웨어 버전
	CString LP_MCU_VERSION;			// MCU 버전
	CString LP_ACCELERATION;		// 가속도값
	CString LP_SPEED_PULSE;			// 속도 펄스값
	CString LP_CONFIRM_FAIL;		// 확인 실패
	CString LP_LANE_DETECT_FAIL;	// 차선 인식 실패
	CString LP_LANE_WARNING_L_FAIL;	// 차선 좌측 경보 실패
	CString LP_LANE_WARNING_R_FAIL;	// 차선 우측 경보 실패
	CString LP_WINKER_L;			// 방향 지시등 (좌)
	CString LP_WINKER_R;			// 방향 지시등 (우)
	CString LP_EMERGENCY_LAMP;		// 비상 정지등
	CString LP_FCW_WARNING_FAIL;	// 차량 충돌 경보 실패
	CString LP_WARNING_FAIL;		// 경보 실패
	CString LP_DTC_DELETE_FAIL;		// DTC 삭제 실패
	CString LP_POWER_OFF_FAIL;		// 파워 해제 실패
	CString LP_CASE_BARCODE_FAIL;	// 케이스 바코드 실패
	CString LP_TEST_PASS;			// 검사 통과
	CString LP_TEST_FAIL;			// 검사 실패
	CString LP_FAIL_OCCURS;			// 불량 발생
	CString LP_TEST_END;			// 검사 종료
	CString LP_CONNECTING;			// 연결중
	CString LP_TEST_PROCESS_START;	// 제품을 안착 시키고 연결 한 후에 시작 버튼을 누르세요
	CString LP_POWER_OFF;			// 전원을 해제 합니다
	CString LP_CONNECTION_CHECK;	// 통신연결 확인
	CString LP_LANE_WARNING_L;		// LDWS 차선 좌측 이탈	
	CString LP_LANE_WARNING_R;		// LDWS 차선 우측 이탈
	CString LP_START;				// 시작
	CString LP_CLOSE;				// 닫기
	CString LP_PAUSE;				// 중단

	//--------- 외부 사용 함수 ------------------------------------//
	int SET_LANGUAE_PACK(int languageType);
protected:
	//--------- 내부 환경 변수 ------------------------------------//

	//--------- 내부 사용 함수 ------------------------------------//
};


#endif //  !Y_LANGUAGEPACK_H  //
