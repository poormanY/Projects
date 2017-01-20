#ifndef Y_SERIAL_COMM_H
#define Y_SERIAL_COMM_H

#include "y_wm_user.h"

#define BUFF_SIZE			4192
#define THREAD_BUFF_SIZE	204800

//	통신 클래스	CCommThread 

// 포트에서 읽기 :
//   포트를 연 후에 포트에 자료가 도착하면 WM_COMM_READ 메시지가 메인 
//   윈도우에 전달된다. ON_MESSAGE 매크로를 이용, 함수를 연결하고
//   m_ReadData String에 저장된 데이터를 이용 읽기
// 포트에 쓰기 : WriteComm(buff, 30)과 같이 버퍼와 그 크기를 건네면 된다.
class CQueue
{
public:
	CQueue();
	~CQueue();

	BYTE buff[BUFF_SIZE];//큐 버퍼 사이즈는 4192로 만듬 
	int m_iHead, m_iTail;//큐 버퍼에 데이터를 넣고 빼 올때 사용할 변수 
	void Clear();//버퍼를 초기화 시킴
	int GetSize();//현재 버퍼에 들어있는 데이터의 size를 리턴
	BOOL PutByte(BYTE b);//큐버퍼에 1바이트를 넣음
	BOOL GetByte(BYTE *pb);//큐버퍼에서 1바이트를 빼 옴
};


class CSerialComm
{
public:
	CSerialComm();
	~CSerialComm();

	//--------- 외부 환경 변수 ------------------------------------//
	BOOL        check;
	HANDLE		m_hComm;				// 통신 포트 파일 핸들
	CString		m_sPortName;			// 포트 이름 (COM1 ..)
	BOOL		m_bConnected;			// 포트가 열렸는지 유무를 나타냄.
	OVERLAPPED	m_osRead, m_osWrite;	// 포트 파일 Overlapped structure
	HANDLE		m_hThreadWatchComm;		// Watch함수 Thread 핸들.
	WORD		m_wPortID;				// WM_COMM_READ와 함께 보내는 인수.
	CQueue      m_QueueRead;			//큐버퍼
	//--------- 외부 사용 함수 ------------------------------------//
	BOOL	OpenPort(CString strPortName, DWORD dwBaud, 
		BYTE byData, BYTE byStop, BYTE byParity ); //포트 열기
	void	ClosePort();				           //포트 닫기
	DWORD	WriteComm(BYTE *pBuff, DWORD nToWrite);//포트에 데이터 쓰기
	DWORD	ReadComm(BYTE *pBuff, DWORD nToRead);//포트에서 데이터 읽어오기
	//--------- OpenPort 변수 형 변환용 ---------------------------//
	CString byIndexComPort(int xPort);
	DWORD   byIndexBaud(int xBaud);
	BYTE    byIndexData(int xData);
	BYTE    byIndexStop(int xStop);
	BYTE    byIndexParity(int xParity);
	BYTE    byCode2AsciiValue(char cData);

protected:
	//--------- 내부 환경 변수 ------------------------------------//

	//--------- 내부 사용 함수 ------------------------------------//
};

// Thread로 사용할 함수 
DWORD	ThreadWatchComm(CSerialComm* pComm);

#endif //  !Y_SERIAL_COMM_H  //