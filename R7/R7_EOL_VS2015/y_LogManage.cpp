// Save Log Function

#include "stdafx.h"
#include "y_LogManage.h"
#include "y_Debugging.h"

CLogManage::CLogManage()
{
	m_strFilePath = "";
}
CLogManage::~CLogManage()
{
	m_strFilePath = "";
}

void CLogManage::SetFilePath(CString a_FilePath)
{
	m_strFilePath = a_FilePath;
}

int CLogManage::WriteLog(CString a_Log)
{
	FILE   *pFileLog;

	if ( m_strFilePath == "")
	{
		dp("Not Allocate File Path\n");
		return 0;
	}

    pFileLog    = fopen((CStringA)m_strFilePath, "a+t");
    if (pFileLog != NULL)
    {
        //dp("File Open Success <%s>\n, m_strFilePath");
    }
    else
    {
		dp("File Open Fail <%s>\n", m_strFilePath);
        return 0;
    }

    fseek(pFileLog, SEEK_END, 0);
	fprintf(pFileLog, "%s", (CStringA)a_Log);

    fclose(pFileLog);
	return 1;
}

int CLogManage::WriteLogTime(CString a_Log)
{
	FILE   *pFileLog;
	CTime   oTime;

	if ( m_strFilePath == "")
	{
		dp("Not Allocate File Path\n");
		return 0;
	}

    pFileLog    = fopen((CStringA)m_strFilePath, "a+t");
    if (pFileLog != NULL)
    {
        //dp("File Open Success <%s>\n, m_strFilePath");
    }
    else
    {
		dp("File Open Fail <%s>\n", m_strFilePath);
        return 0;
    }

    fseek(pFileLog, SEEK_END, 0);

	oTime = CTime::GetCurrentTime();
	fprintf(pFileLog, "[%04d-%02d-%02d, %02d:%02d:%02d] ", 
		oTime.GetYear(), oTime.GetMonth(), oTime.GetDay(), oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());
	fprintf(pFileLog, "%s", (CStringA)a_Log);

    fclose(pFileLog);

	return 1;
}

int CLogManage::CopyLog(CString a_CopyFilePath)
{
	FILE	*pFileLog;
	FILE	*pCopyFileLog;
	char szLog[1024];

	if ( m_strFilePath == "")
	{
		dp("Not Allocate File Path\n");
		return 0;
	}

    pFileLog    = fopen((CStringA)m_strFilePath, "rt");
    if (pFileLog != NULL)
    {
        //dp("File Open Success <%s>\n, m_strFilePath");
    }
    else
    {
		dp("File Open Fail <%s>\n", m_strFilePath);
        return 0;
    }

	pCopyFileLog    = fopen((CStringA)a_CopyFilePath, "wt");
    if (pCopyFileLog != NULL)
    {
        //dp("File Open Success <%s>\n, a_CopyFilePath");
    }
    else
    {
		dp("File Open Fail <%s>\n", a_CopyFilePath);
        return 0;
    }

	while ( fgets( szLog, 1024-1, pFileLog ) )
	{
		fputs ( szLog, pCopyFileLog );
	}

    fclose(pFileLog);
	fclose(pCopyFileLog);

	return 1;
}

int CLogManage::WriteSetting(CString a_Setting, CString a_Value)
{
	FILE   *pFileSetting, *pCopyFileSetting;
	char szSetting[50], szValue[50];
	CString strSetting, strValue;
	CString strCopyFilePath;
	int chkSettingExist = 0;
	int chkSettingChanged = 0;

	if ( m_strFilePath == "")
	{
		dp("Not Allocate File Path\n");
		return 0;
	}

    pFileSetting    = fopen((CStringA)m_strFilePath, "a+t");
    if (pFileSetting != NULL)
    {
        //dp("File Open Success <%s>\n, m_strFilePath");
    }
    else
    {
		dp("File Open Fail <%s>\n", m_strFilePath);
        return 0;
    }

	while( !feof(pFileSetting) )
	{
		fscanf(pFileSetting, "%s		%s", szSetting, szValue);
		strSetting = szSetting;
		strValue = szValue;

		// 파스칼의 eof는 '다음'에 읽어오는 부분이 파일의 끝이라 실패하는 경우에 true를 리턴합니다.
		// 반면 C의 feof는 '마지막' 함수가 실패하는 경우에 true를 리턴합니다.
		if(feof(pFileSetting)) break;

		// Setting 항목이 이미 존재했을 때
		if ( (strSetting.Compare(a_Setting)) == 0 )
		{
			chkSettingExist = 1;
			// Setting 값에 변화가 없을 때
			if ( (strValue.Compare(a_Value)) == 0 )
			{
				chkSettingChanged = 0;
			}
			else
			{
				chkSettingChanged = 1;
			}			
			break;
		}
	}

	if ( chkSettingExist == 0 )
	{
		// 새로운 세팅 값 추가
		fprintf(pFileSetting, "%-30s		%-30s\n", (CStringA)a_Setting, (CStringA)a_Value);
	}
	else
	{
		if ( chkSettingChanged == 1 )
		{
			// 세팅 값이 저장 값과 다른 경우에
			fclose(pFileSetting);

			// tmp 파일로 복사
			strCopyFilePath = m_strFilePath + _T(".tmp");
			CopyLog(strCopyFilePath);
			
			// tmp -> 원본 파일로 변경내용만 수정해서 쓰기
			pFileSetting     = fopen((CStringA)m_strFilePath, "wt");
			pCopyFileSetting = fopen((CStringA)strCopyFilePath, "rt");

			while( !feof(pCopyFileSetting) )
			{
				fscanf(pCopyFileSetting, "%s		%s", szSetting, szValue);
				strSetting = szSetting;
				strValue = szValue;

				// 파스칼의 eof는 '다음'에 읽어오는 부분이 파일의 끝이라 실패하는 경우에 true를 리턴합니다.
				// 반면 C의 feof는 '마지막' 함수가 실패하는 경우에 true를 리턴합니다.
				if(feof(pCopyFileSetting)) break;

				// Setting 항목이 이미 존재했을 때
				if ( (strSetting.Compare(a_Setting)) == 0 )
				{
					fprintf(pFileSetting, "%-30s		%-30s\n", (CStringA)a_Setting, (CStringA)a_Value);
				}
				else
				{
					fprintf(pFileSetting, "%-30s		%-30s\n", (CStringA)strSetting, (CStringA)strValue);
				}
			}

			fclose(pCopyFileSetting);
			// tmp 파일 삭제하기
			DeleteFile(strCopyFilePath);
		}
	}

    fclose(pFileSetting);
	return 1;
}

CString CLogManage::ReadSetting(CString a_Setting)
{
	FILE   *pFileSetting;
	char szSetting[50], szValue[50];
	CString strSetting, strValue;
	CString strFAIL = _T("");

	if ( m_strFilePath == "")
	{
		dp("Not Allocate File Path\n");
		return strFAIL;
	}

    pFileSetting    = fopen((CStringA)m_strFilePath, "rt");
    if (pFileSetting != NULL)
    {
        //dp("File Open Success <%s>\n, m_strFilePath");
    }
    else
    {
		dp("File Open Fail <%s>\n", m_strFilePath);
        return strFAIL;
    }

	while( !feof(pFileSetting) )
	{
		fscanf(pFileSetting, "%s		%s", szSetting, szValue);
		strSetting = szSetting;

		// 파스칼의 eof는 '다음'에 읽어오는 부분이 파일의 끝이라 실패하는 경우에 true를 리턴합니다.
		// 반면 C의 feof는 '마지막' 함수가 실패하는 경우에 true를 리턴합니다.
		if(feof(pFileSetting)) break;

		if ( (strSetting.Compare(a_Setting)) == 0 )
		{
			strValue = szValue;
			fclose(pFileSetting);
			return strValue;
		}
	}

    fclose(pFileSetting);
	return strFAIL;
}