#ifndef Y_LOG_MANAGE_H
#define Y_LOG_MANAGE_H

class CLogManage
{
public:
	CLogManage();
	~CLogManage();

	//--------- 외부 환경 변수 ------------------------------------//

	//--------- 외부 사용 함수 ------------------------------------//
	void SetFilePath(CString a_FilePath);
	int WriteLog(CString a_Log);
	int WriteLogTime(CString a_Log);
	int CopyLog(CString a_CopyFilePath);

	int		WriteSetting(CString a_Setting, CString a_Value);
	CString ReadSetting(CString a_Setting);
protected:
	//--------- 내부 환경 변수 ------------------------------------//
	CString m_strFilePath;      // 파일 경로
	//--------- 내부 사용 함수 ------------------------------------//
	
};





#endif //  !Y_LOG_MANAGE_H  //