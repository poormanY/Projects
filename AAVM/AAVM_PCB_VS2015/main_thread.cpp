// test_main_thread.cpp: Main Program Emulate. Kim JinWoo, 2014-03-13.
#include "stdafx.h"
#include "MainApp.h"
#include "MainDlg.h"
#include "wm_user.h"
#include "pcan_inf.h"
#include "jstring.h"
#include "sys_ctimer.h"
#include "GetTickCountEx.h"
#include "can_msg.h"
#include <io.h>
//#include <fcntl.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <stdio.h>
//#include <share.h>

extern MainDlg *pMainDlg;

CPcan* pCan;    // 사용할 PCAN 개체

bool ongoing = true;

// Test Main Thread Management
CWinThread *   pfMain_Thread = NULL;
TOF            tMain_Thread_Stop = false;
UINT AFX_CDECL Main_Thread(LPVOID pParam);

UINT AFX_CDECL Main_Thread(LPVOID pParam)
{
    int nListPos;
    CAN_MSG oCanMsg;
    U32 wCAN_Prev_GetCnt;
    CString stMsg;
    const CHR *pszSrc, *pszTgt;
	ongoing = true;
	int programtype = 0;
	

	bool ret = true;


	FILE *pFileLog;
    CTime oTime;



	pFileLog = fopen("AAVM_PCB.log", "a+t");//fopen("D:\\PLK_AAVM\\AAVM_PCB.log", "a+t");
	if (pFileLog != NULL)
	{
		CTime oTime = CTime::GetCurrentTime();

		fseek(pFileLog, SEEK_END, 0);
		fprintf(pFileLog, "\AAVM PCB 테스트 시작: %04d-%02d-%02d %02d:%02d:%02d\n",
		oTime.GetYear(), oTime.GetMonth(), oTime.GetDay(),
		oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());

		fprintf(pFileLog, "Serial Number: %s\n",pMainDlg->m_EDIT_Barcode);

		fclose(pFileLog);
	}

    //while (tMain_Thread_Stop == false)
	if (pMainDlg->m_COMBO_PROGRAM_TYPE == "버전")
		programtype = 1;
	else if (pMainDlg->m_COMBO_PROGRAM_TYPE == "카메라")
		programtype = 2;
	else if (pMainDlg->m_COMBO_PROGRAM_TYPE == "통합")
		programtype = 3;
	else
		programtype = -1;

	if ( programtype > 0)
	{
		pMainDlg->m_LIST_PROCESS.DeleteAllItems();
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        /////////////////////////////////////
        /////////////////////////////////////
        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "AAVM CID 제품 코드");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "85");

        MakeMsg_REQ_CID(&oCanMsg);
        wCAN_Prev_GetCnt = wCAN_CID_GetCnt;
        pCan->Write(&oCanMsg);
        while ( wCAN_Prev_GetCnt == wCAN_CID_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }
        stMsg.Format("%d", bCAN_CID_CC);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (bCAN_CID_CC == 85)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }


        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "AAVM CID 공급자 코드");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "22");

        stMsg.Format("%d", bCAN_CID_MC);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (bCAN_CID_MC == 22)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "AAVM CID 제조일자");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "2016-08-22");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, szCAN_CID_MDay);
        if (strcmp(szCAN_CID_MDay, "2016-08-22") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "AAVM CID 버전");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "2.8.0");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, szCAN_CID_Ver);
        if (strcmp(szCAN_CID_Ver, "2.8.0") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "AAVM CID 모델이름");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "AAVMHW60");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, szCAN_CID_ModelName);
        if (strcmp(szCAN_CID_ModelName, "AAVMHW60") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        /////////////////////////////////////
        /////////////////////////////////////
        tCAN_VER1_Complete = false;
        MakeMsg_REQ_VER1(&oCanMsg);
        pCan->Write(&oCanMsg);
        while ( tCAN_VER1_Complete == false)
        {
			if(ongoing == false) return 0;
            Sleep(100);
            pCan->Write(&oCanMsg);
        }
        
        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "AAVM H/W 버전");
        pszSrc = szCAN_Ver_HW;
        pszTgt = "1.5";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "BF609 F/W 버전");
        pszSrc = szCAN_Ver_BF;
        pszTgt = "2.8";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "에머랄드 F/W 버전");
        pszSrc = szCAN_Ver_EL_Main;
        pszTgt = "1.12";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "에머랄드 합성 버전");
        pszSrc = szCAN_Ver_EL_Sub;
        pszTgt = "없음";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
		}
	}
	if ( programtype > 1)
	{
        /////////////////////////////////////
        /////////////////////////////////////
        /////////////////////////////////////
        tCAN_VER2_Complete = false;
        MakeMsg_REQ_VER2(&oCanMsg);
        pCan->Write(&oCanMsg);
        while ( tCAN_VER2_Complete == false)
        {
			if(ongoing == false) return 0;
            Sleep(100);
            pCan->Write(&oCanMsg);
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "카메라 버전 - 전방");
        pszSrc = szCAN_Ver_Camera_Front;
        pszTgt = "3.3";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "카메라 버전 - 후방");
        pszSrc = szCAN_Ver_Camera_Rear;
        pszTgt = "3.3";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "카메라 버전 - 좌측");
        pszSrc = szCAN_Ver_Camera_Left;
        pszTgt = "3.3";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "카메라 버전 - 우측");
        pszSrc = szCAN_Ver_Camera_Right;
        pszTgt = "3.3";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }
	}
	if ( programtype > 0)
	{
		/////////////////////////////////////
        /////////////////////////////////////
        /////////////////////////////////////
		int Car_Types[5] = {0, 0, 0, 0, 0};
		if (pMainDlg->m_EDIT_CAR_NAME == "21k8-46000")
		{
			Car_Types[0] = 2;
			Car_Types[1] = 5;
			Car_Types[2] = 7;
			Car_Types[3] = 10;
		}
		else if (pMainDlg->m_EDIT_CAR_NAME == "21k8-46100")
		{
			Car_Types[0] = 4;
			Car_Types[1] = 3;
			Car_Types[2] = 6;
			Car_Types[3] = 8;
			Car_Types[4] = 9;
		}
		else if (pMainDlg->m_EDIT_CAR_NAME == "21k8-43000")
		{
			Car_Types[0] = 13;
			Car_Types[1] = 14;
			Car_Types[2] = 15;
			Car_Types[3] = 16;
		}
		else if (pMainDlg->m_EDIT_CAR_NAME == "21k8-46200")
		{
			Car_Types[0] = 17;
			Car_Types[1] = 18;
			Car_Types[2] = 19;
		}
		else
		{
		}
        MakeMsg_REQ_MLST(&oCanMsg);
        wCAN_Prev_GetCnt = wCAN_MLST_GetCnt;
        pCan->Write(&oCanMsg);
        while ( wCAN_MLST_GetCnt < (wCAN_Prev_GetCnt+4) )
        {
			if(ongoing == false) return 0;
            Sleep(1000);
            pCan->Write(&oCanMsg);
        }

        /////////////////////////////////////
		for (int ii = 0; ii < 5; ii++)
		{
			nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
			pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "내장된 합성 모델");
			if (Car_Types[ii] == 0)
			{
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "예정된 합성 모델 없음");
				continue;
			}
			else
			{
				pszTgt = arpszModelName[Car_Types[ii]];
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
				for (int jj = 0; jj<5; jj++)
				{
					pszSrc = arszCAN_MLST[jj];
					if (strcmp(pszSrc, pszTgt) == 0)
					{
						pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
						pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
						break;
					}
				}
				if (strcmp(pszSrc, pszTgt) != 0)
				{
					pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "저장된 합성 모델 없음");
					pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
					ret = false;
					break;
				}
			}
		}
		/*
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "내장된 합성 모델 1");
        pszSrc = arszCAN_MLST[0];
        pszTgt = arpszModelName[13];
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "내장된 합성 모델 2");
        pszSrc = arszCAN_MLST[1];
        pszTgt = arpszModelName[14];
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "내장된 합성 모델 3");
        pszSrc = arszCAN_MLST[2];
        pszTgt = arpszModelName[15];
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "내장된 합성 모델 4");
        pszSrc = arszCAN_MLST[3];
        pszTgt = arpszModelName[16];
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "내장된 합성 모델 5");
        pszSrc = arszCAN_MLST[4];
        pszTgt = arpszModelName[19];
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }


        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "내장된 합성 모델 6");
        pszSrc = arszCAN_MLST[5];
        pszTgt = "없음";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }
		*/
        /////////////////////////////////////
        /////////////////////////////////////
        /////////////////////////////////////
        wCAN_Prev_GetCnt = wCAN_AAVST_GetCnt;
        while ( wCAN_AAVST_GetCnt <= (wCAN_Prev_GetCnt+4) )
        {
			if(ongoing == false) return 0;
            Sleep(100);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "BF609 영상 입력 상태");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "정상");

        if ((bCAN_AAVST_BF609_Stat & _BF_ERR_CAMERA) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "정상");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "에러");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "BF609 vs 에메랄드 통신 상태");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "정상");

        if ((bCAN_AAVST_BF609_Stat & _BF_ERR_EMERALD_ST) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "정상");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "에러");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "BF609 CAN ESROS 통신 상태");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "정상");

        if ((bCAN_AAVST_BF609_Stat & _BF_ERR_CAN_ESROS) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "정상");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "에러");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "BF609 CAN HP2 통신 상태");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "정상");

        if ((bCAN_AAVST_BF609_Stat & _BF_ERR_CAN_HP2) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "정상");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "에러");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "BF609 CAN HP3 통신 상태");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "정상");

        if ((bCAN_AAVST_BF609_Stat & _BF_ERR_CAN_HP3) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "정상");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "에러");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "BF609 처리 속도");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "5 이상");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_BF609_FPS);
        if (bCAN_AAVST_BF609_FPS >= 50)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }


        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "에메랄드 상태");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "미보정");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_EMERALD_STAT);
        if (strcmp(pMainDlg->m_EDIT_EMERALD_STAT, "미보정") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }
	}

	if ( programtype > 1)
	{   
        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "카메라 상태 - 전방");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "정상");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_CAMERA_STAT_FRONT);
        if (strcmp(pMainDlg->m_EDIT_CAMERA_STAT_FRONT, "정상") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "카메라 상태 - 후방");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "정상");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_CAMERA_STAT_REAR);
        if (strcmp(pMainDlg->m_EDIT_CAMERA_STAT_REAR, "정상") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "카메라 상태 - 좌측");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "정상");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_CAMERA_STAT_LEFT);
        if (strcmp(pMainDlg->m_EDIT_CAMERA_STAT_LEFT, "정상") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "카메라 상태 - 우측");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "정상");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_CAMERA_STAT_RIGHT);
        if (strcmp(pMainDlg->m_EDIT_CAMERA_STAT_RIGHT, "정상") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }
	}

	if ( programtype > 2)
	{
        /////////////////////////////////////
        /////////////////////////////////////
        /////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 0, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "출력 화면 뷰 모드 - 옴니(전)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "옴니(전)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "옴니(전)") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 1, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "출력 화면 뷰 모드 - 옴니(후)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "옴니(후)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "옴니(후)") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 8, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "출력 화면 뷰 모드 - 옴니(좌)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "옴니(좌)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "옴니(좌)") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 9, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "출력 화면 뷰 모드 - 옴니(우)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "옴니(우)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "옴니(우)") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 2, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "출력 화면 뷰 모드 - 버드뷰");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "버드뷰");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "버드뷰") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }
	}
	if ( programtype > 1)
	{
        /////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 4, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "출력 화면 뷰 모드 - 전방");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "전방");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "전방") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 5, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "출력 화면 뷰 모드 - 후방");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "후방");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "후방") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 6, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "출력 화면 뷰 모드 - 좌측");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "좌측");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "좌측") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 7, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "출력 화면 뷰 모드 - 우측");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "우측");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "우측") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

		/////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 3, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "출력 화면 뷰 모드 - 4채널");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "4채널");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "4채널") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }
	}


	if ( programtype > 2)
	{
		/////// Return Default View Mode //////
        MakeMsg_AAVCS(&oCanMsg, 0, 0);
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        pCan->Write(&oCanMsg);
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }


        /////////////////////////////////////
        /////////////////////////////////////
        /////////////////////////////////////
        pMainDlg->m_EDIT_ESROS_WIPER = 0;
        pMainDlg->m_EDIT_ESROS_INTWIPER = 0;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        MakeMsg_AAVCS(&oCanMsg, 0, 2);
        pCan->Write(&oCanMsg);

        // 인식모드
        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "인식모드 - 사용자 스위치 Off");
        pszTgt = "사용자 Off(On)";
        stMsg.Format("사용자 Off(%s)", 
            (pMainDlg->m_CHECK_MODE_MANUAL_OFF == FALSE)? "Off": "On");
        pszSrc = stMsg;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        // IMOD 상태
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "IMOD 상태 - 사용자 스위치 Off");
        pszTgt = "수동 Off";
        pszSrc = pMainDlg->m_EDIT_AAVM_IMOD_STATUS;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 0, 1);
        pCan->Write(&oCanMsg);

        // 인식모드
        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "인식모드 - 사용자 스위치 On");
        pszTgt = "사용자 Off(Off)";
        stMsg.Format("사용자 Off(%s)", 
            (pMainDlg->m_CHECK_MODE_MANUAL_OFF == FALSE)? "Off": "On");
        pszSrc = stMsg;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        // IMOD 상태
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "IMOD 상태 - 사용자 스위치 On");
        pszTgt = "정상작동";
        pszSrc = pMainDlg->m_EDIT_AAVM_IMOD_STATUS;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

#if 1
        /////////////////////////////////////
        pMainDlg->m_EDIT_ESROS_WIPER = 1;
        pMainDlg->m_EDIT_ESROS_INTWIPER = 0;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        // 인식모드
        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "인식모드 - Wiper On");
        pszTgt = "Wiper(On)";
        stMsg.Format("Wiper(%s)", 
            (pMainDlg->m_CHECK_MODE_WIPER == FALSE)? "Off": "On");
        pszSrc = stMsg;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        // IMOD 상태
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+7) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "IMOD 상태 - Wiper On");
        pszTgt = "자동 Off";
        pszSrc = pMainDlg->m_EDIT_AAVM_IMOD_STATUS;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        pMainDlg->m_EDIT_ESROS_WIPER = 0;
        pMainDlg->m_EDIT_ESROS_INTWIPER = 0;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        // 인식모드
        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "인식모드 - Wiper Off");
        pszTgt = "Wiper(Off)";
        stMsg.Format("Wiper(%s)", 
            (pMainDlg->m_CHECK_MODE_WIPER == FALSE)? "Off": "On");
        pszSrc = stMsg;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        // IMOD 상태
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+7) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "IMOD 상태 - Wiper Off");
        pszTgt = "정상작동";
        pszSrc = pMainDlg->m_EDIT_AAVM_IMOD_STATUS;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        pMainDlg->m_EDIT_ESROS_WIPER = 0;
        pMainDlg->m_EDIT_ESROS_INTWIPER = 1;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        // 인식모드
        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "인식모드 - Int.Wiper On");
        pszTgt = "Int.Wiper(On)";
        stMsg.Format("Int.Wiper(%s)", 
            (pMainDlg->m_CHECK_MODE_INTWIPER == FALSE)? "Off": "On");
        pszSrc = stMsg;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        // IMOD 상태
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+7) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "IMOD 상태 - Int.Wiper On");
        pszTgt = "자동 Off";
        pszSrc = pMainDlg->m_EDIT_AAVM_IMOD_STATUS;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        pMainDlg->m_EDIT_ESROS_WIPER = 0;
        pMainDlg->m_EDIT_ESROS_INTWIPER = 0;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        // 인식모드
        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "인식모드 - Int.Wiper Off");
        pszTgt = "Int.Wiper(Off)";
        stMsg.Format("Int.Wiper(%s)", 
            (pMainDlg->m_CHECK_MODE_INTWIPER == FALSE)? "Off": "On");
        pszSrc = stMsg;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        // IMOD 상태
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+11) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "IMOD 상태 - Int.Wiper Off");
        pszTgt = "정상작동";
        pszSrc = pMainDlg->m_EDIT_AAVM_IMOD_STATUS;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        pMainDlg->m_EDIT_MMS_MOVE    = 1;
        pMainDlg->m_EDIT_HP1_ARM     = 80;
        pMainDlg->m_EDIT_HP3_SWING   = 80;
        pMainDlg->m_EDIT_HP3_BOOM_UP = 80;
        pMainDlg->m_EDIT_HP3_BOOM_DN = 80;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "인식모드 - 주행중");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "주행(On), 회전(Off), 작업(Off)");

        stMsg.Format("주행(%s), 회전(%s), 작업(%s)", 
            (pMainDlg->m_CHECK_MODE_MOVE  == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_SWING == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_WORK  == FALSE)? "Off": "On");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "주행(On), 회전(Off), 작업(Off)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        pMainDlg->m_EDIT_MMS_MOVE    = 0;
        pMainDlg->m_EDIT_HP1_ARM     = 80;
        pMainDlg->m_EDIT_HP3_SWING   = 81;
        pMainDlg->m_EDIT_HP3_BOOM_UP = 80;
        pMainDlg->m_EDIT_HP3_BOOM_DN = 80;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "인식모드 - 회전중");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "주행(Off), 회전(On), 작업(Off)");

        stMsg.Format("주행(%s), 회전(%s), 작업(%s)", 
            (pMainDlg->m_CHECK_MODE_MOVE  == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_SWING == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_WORK  == FALSE)? "Off": "On");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "주행(Off), 회전(On), 작업(Off)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        pMainDlg->m_EDIT_MMS_MOVE    = 0;
        pMainDlg->m_EDIT_HP1_ARM     = 81;
        pMainDlg->m_EDIT_HP3_SWING   = 80;
        pMainDlg->m_EDIT_HP3_BOOM_UP = 80;
        pMainDlg->m_EDIT_HP3_BOOM_DN = 80;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+7) >= wCAN_AAVM_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "인식모드 - 작업중");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "주행(Off), 회전(Off), 작업(On)");

        stMsg.Format("주행(%s), 회전(%s), 작업(%s)", 
            (pMainDlg->m_CHECK_MODE_MOVE  == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_SWING == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_WORK  == FALSE)? "Off": "On");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "주행(Off), 회전(Off), 작업(On)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        pMainDlg->m_EDIT_MMS_MOVE    = 0;
        pMainDlg->m_EDIT_HP1_ARM     = 80;
        pMainDlg->m_EDIT_HP3_SWING   = 80;
        pMainDlg->m_EDIT_HP3_BOOM_UP = 81;
        pMainDlg->m_EDIT_HP3_BOOM_DN = 80;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "인식모드 - 작업중");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "주행(Off), 회전(Off), 작업(On)");

        stMsg.Format("주행(%s), 회전(%s), 작업(%s)", 
            (pMainDlg->m_CHECK_MODE_MOVE  == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_SWING == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_WORK  == FALSE)? "Off": "On");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "주행(Off), 회전(Off), 작업(On)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        pMainDlg->m_EDIT_MMS_MOVE    = 0;
        pMainDlg->m_EDIT_HP1_ARM     = 80;
        pMainDlg->m_EDIT_HP3_SWING   = 80;
        pMainDlg->m_EDIT_HP3_BOOM_UP = 80;
        pMainDlg->m_EDIT_HP3_BOOM_DN = 81;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "인식모드 - 작업중");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "주행(Off), 회전(Off), 작업(On)");

        stMsg.Format("주행(%s), 회전(%s), 작업(%s)", 
            (pMainDlg->m_CHECK_MODE_MOVE  == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_SWING == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_WORK  == FALSE)? "Off": "On");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "주행(Off), 회전(Off), 작업(On)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        /////////////////////////////////////
        pMainDlg->m_EDIT_MMS_MOVE    = 0;
        pMainDlg->m_EDIT_HP1_ARM     = 0;
        pMainDlg->m_EDIT_HP3_SWING   = 0;
        pMainDlg->m_EDIT_HP3_BOOM_UP = 0;
        pMainDlg->m_EDIT_HP3_BOOM_DN = 0;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }
	}
#endif

#if 1
	if ( programtype > 1) 
	{
        /////////////////////////////////////
        /////////////////////////////////////
        /////////////////////////////////////
        // 전방 카메라는 백색, 나머지는 흑색
        wCAN_Prev_GetCnt = wCAN_AAVST_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVST_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "카메라 밝기값 측정");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "전(>5), 후(>5), 좌(>5), 우(>5)");

        stMsg.Format("전(%d), 후(%d), 좌(%d), 우(%d)",
            bCAN_AAVST_CameraGain_Front,
            bCAN_AAVST_CameraGain_Rear,
            bCAN_AAVST_CameraGain_Left,
            bCAN_AAVST_CameraGain_Right);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);

        if ( (bCAN_AAVST_CameraGain_Front > 5) &&
             (bCAN_AAVST_CameraGain_Rear  > 5) &&
             (bCAN_AAVST_CameraGain_Left  > 5) &&
             (bCAN_AAVST_CameraGain_Right > 5)   )
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }
#endif
#if 0

        /////////////////////////////////////
        /////////////////////////////////////
        /////////////////////////////////////
        // 전방 카메라에 사람이 안전선 밖에서 이동하는 동영상, 나머지는 정지영상
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "영상인식 테스트 - 전방");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "전(주의), 후(-), 좌(-), 우(-)");

        stMsg.Format("전(%s), 후(%s), 좌(%s), 우(%s)",
            pMainDlg->m_EDIT_AAVM_WARNING_FRONT,
            pMainDlg->m_EDIT_AAVM_WARNING_REAR,
            pMainDlg->m_EDIT_AAVM_WARNING_LEFT,
            pMainDlg->m_EDIT_AAVM_WARNING_RIGHT);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "전(주의), 후(-), 좌(-), 우(-)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        // 후방 카메라에 사람이 안전선 안에서 이동하는 동영상, 나머지는 정지영상
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "영상인식 테스트 - 후방");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "전(-), 후(위험), 좌(-), 우(-)");

        stMsg.Format("전(%s), 후(%s), 좌(%s), 우(%s)",
            pMainDlg->m_EDIT_AAVM_WARNING_FRONT,
            pMainDlg->m_EDIT_AAVM_WARNING_REAR,
            pMainDlg->m_EDIT_AAVM_WARNING_LEFT,
            pMainDlg->m_EDIT_AAVM_WARNING_RIGHT);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "전(-), 후(위험), 좌(-), 우(-)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        // 좌측 카메라에 사람이 안전선 밖에서 이동하는 동영상, 나머지는 정지영상
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "영상인식 테스트 - 좌측");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "전(-), 후(-), 좌(주의), 우(-)");

        stMsg.Format("전(%s), 후(%s), 좌(%s), 우(%s)",
            pMainDlg->m_EDIT_AAVM_WARNING_FRONT,
            pMainDlg->m_EDIT_AAVM_WARNING_REAR,
            pMainDlg->m_EDIT_AAVM_WARNING_LEFT,
            pMainDlg->m_EDIT_AAVM_WARNING_RIGHT);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "전(-), 후(-), 좌(주의), 우(-)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        // 우측 카메라에 사람이 안전선 안에서 이동하는 동영상, 나머지는 정지영상
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "영상인식 테스트 - 우측");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "전(-), 후(-), 좌(-), 우(위험)");

        stMsg.Format("전(%s), 후(%s), 좌(%s), 우(%s)",
            pMainDlg->m_EDIT_AAVM_WARNING_FRONT,
            pMainDlg->m_EDIT_AAVM_WARNING_REAR,
            pMainDlg->m_EDIT_AAVM_WARNING_LEFT,
            pMainDlg->m_EDIT_AAVM_WARNING_RIGHT);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "전(-), 후(-), 좌(-), 우(위험)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        // 모든 카메라에 사람이 안전선 안에서 이동하는 동영상
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "영상인식 테스트 - 모두 위험");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "전(위험), 후(위험), 좌(위험), 우(위험)");

        stMsg.Format("전(%s), 후(%s), 좌(%s), 우(%s)",
            pMainDlg->m_EDIT_AAVM_WARNING_FRONT,
            pMainDlg->m_EDIT_AAVM_WARNING_REAR,
            pMainDlg->m_EDIT_AAVM_WARNING_LEFT,
            pMainDlg->m_EDIT_AAVM_WARNING_RIGHT);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "전(위험), 후(위험), 좌(위험), 우(위험)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }

        // 모든 카메라에 사람이 안전선 밖에서 이동하는 동영상
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "영상인식 테스트 - 모두 주의");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "전(주의), 후(주의), 좌(주의), 우(주의)");

        stMsg.Format("전(%s), 후(%s), 좌(%s), 우(%s)",
            pMainDlg->m_EDIT_AAVM_WARNING_FRONT,
            pMainDlg->m_EDIT_AAVM_WARNING_REAR,
            pMainDlg->m_EDIT_AAVM_WARNING_LEFT,
            pMainDlg->m_EDIT_AAVM_WARNING_RIGHT);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "전(주의), 후(주의), 좌(주의), 우(주의)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
			ret = false;
        }
#endif
    }
    //pMainDlg->PostMessage(_WM_THREAD_END);



	nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
	pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "검사  종료");
    pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "통합 검사");

	if (ret==true)
	{
		pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "정상");
		pMainDlg->m_EDIT_TEST_NUM_OK++;
		AfxMessageBox("정상. 검사완료");
	}
	else
	{
		pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "에러");
		Sleep(100);
		AfxMessageBox("불량. 통합검사 에러");
		//AfxMessageBox("Error 검출 검사 실패하였습니다");
	}


	pMainDlg->m_EDIT_TEST_NUM_ALL++;
	pMainDlg->m_EDIT_TEST_NUM_RATE.Format("%5.1f%%",
	(double)pMainDlg->m_EDIT_TEST_NUM_OK*100.0/(double)pMainDlg->m_EDIT_TEST_NUM_ALL);
	pMainDlg->PostMessage(_WM_THREAD_UPDATE);

	pMainDlg->PostMessage(_WM_THREAD_END);
    return 0;
}

bool Main_Thread_Begin(void)
{
    ///////////////////////////////////////////
    // 초기화 상태가 맞는지 확인한다.
    ///////////////////////////////////////////
    if ( (pCan != NULL) || (pfMain_Thread != NULL) )
    {
        dprintf(("<== Can't Open PCAN. CPcan ran!\n"));
        return false;
    }

    ///////////////////////////////////////////
    // 1ms Timer를 설치한다. (CPcan에서 사용됨)
    ///////////////////////////////////////////
    if (GetTickCountEx_Init() == false)
    {
        dprintf(("<== CTimer Init Error!\n"));
        return false;
    }
    ///////////////////////////////////////////
    // 10ms Timer를 설치한다. (j1939에서 사용됨)
    ///////////////////////////////////////////
    if (CTimerInit() == false)
    {
        dprintf(("<== CTimer Init Error!\n"));
        return false;
    }

    ///////////////////////////////////////////
    // CPCan 개체를 생성한다.
    ///////////////////////////////////////////
    pCan = new CPcan;
    if (pCan == NULL)
    {
        return false;
    }
    dprintf(("<== Create CPcan Complete!\n"));

    ///////////////////////////////////////////
    // PCANBasic.dll를 로드한다.
    ///////////////////////////////////////////
    if (pCan->Open() == false)
    {
        return false;
    }
    dprintf(("<== Open Complete!\n"));

    ///////////////////////////////////////////
    // 500kbps, Standard(11bits) or Extended(29bits) mode 로 연결한다.
    ///////////////////////////////////////////
    if (pCan->Connect(500, true) == false)
    {
        return false;
    }
    dprintf(("<== Connect Complete!\n"));

    ///////////////////////////////////////////
    // 자동으로 지정된 주기마다 CAN 메시지를 송신하는 개체를 등록한다.
    ///////////////////////////////////////////
    CPcanAutoTx *pAutoTx;
    pAutoTx = new CPcanAutoTx((6<<_BITP_J1939_ID_PRIO)+_CAN_ID_MMS, 200, MakeMsg_MMS);
    pCan->AutoTx_Add(pAutoTx);

    pAutoTx = new CPcanAutoTx((6<<_BITP_J1939_ID_PRIO)+_CAN_ID_HP1, 200, MakeMsg_HP1);
    pCan->AutoTx_Add(pAutoTx);

    pAutoTx = new CPcanAutoTx((6<<_BITP_J1939_ID_PRIO)+_CAN_ID_HP3, 200, MakeMsg_HP3);
    pCan->AutoTx_Add(pAutoTx);

    pAutoTx = new CPcanAutoTx((6<<_BITP_J1939_ID_PRIO)+_CAN_ID_ESROS, 1000, MakeMsg_ESROS);
    pCan->AutoTx_Add(pAutoTx);

    pAutoTx = new CPcanAutoTx((6<<_BITP_J1939_ID_PRIO)+((_CAN_PGN_REQ+_CAN_ADDR_AAVM)<<8)+_CAN_ADDR_EOL, 1000, MakeMsg_REQ_AAVM_Auto);
    pCan->AutoTx_Add(pAutoTx);

    pAutoTx = new CPcanAutoTx((6<<_BITP_J1939_ID_PRIO)+((_CAN_PGN_REQ+_CAN_ADDR_AAVM)<<8)+_CAN_ADDR_EOL2, 1000, MakeMsg_REQ_AAVST_Auto);
    pCan->AutoTx_Add(pAutoTx);

    ///////////////////////////////////////////
    // 지정된 ID로 수신된 CAN 메시지를 자동으로 처리하는 개체를 등록한다.
    ///////////////////////////////////////////
    CPcanAutoRx *pAutoRx;
    pAutoRx = new CPcanAutoRx((_CAN_PGN_AAVO<<8)+_CAN_ADDR_AAVM, 0x1F000000, UseMsg_AAVO);
    pCan->AutoRx_Add(pAutoRx);

    pAutoRx = new CPcanAutoRx((_CAN_PGN_AAVM<<8)+_CAN_ADDR_AAVM, 0x1F000000, UseMsg_AAVM);
    pCan->AutoRx_Add(pAutoRx);

    pAutoRx = new CPcanAutoRx((_CAN_PGN_AAVST<<8)+_CAN_ADDR_AAVM, 0x1F000000, UseMsg_AAVST);
    pCan->AutoRx_Add(pAutoRx);

    pAutoRx = new CPcanAutoRx((_CAN_PGN_CID<<8)+_CAN_ADDR_AAVM, 0x1F000000, UseMsg_CID);
    pCan->AutoRx_Add(pAutoRx);

    pAutoRx = new CPcanAutoRx((_CAN_PGN_VER1<<8)+_CAN_ADDR_AAVM, 0x1F000000, UseMsg_VER1);
    pCan->AutoRx_Add(pAutoRx);

    pAutoRx = new CPcanAutoRx((_CAN_PGN_VER2<<8)+_CAN_ADDR_AAVM, 0x1F000000, UseMsg_VER2);
    pCan->AutoRx_Add(pAutoRx);

    pAutoRx = new CPcanAutoRx((_CAN_PGN_MLST<<8)+_CAN_ADDR_AAVM, 0x1F000000, UseMsg_MLST);
    pCan->AutoRx_Add(pAutoRx);

    pAutoRx = new CPcanAutoRx(((_CAN_PGN_TPCM+0xFF)<<8)+_CAN_ADDR_AAVM, 0x1F000000, UseMsg_TPCM_BAM);
    pCan->AutoRx_Add(pAutoRx);

    pAutoRx = new CPcanAutoRx(((_CAN_PGN_TPDT+0xFF)<<8)+_CAN_ADDR_AAVM, 0x1F000000, UseMsg_TPDT_BAM);
    pCan->AutoRx_Add(pAutoRx);

    ///////////////////////////////////////////
	// 자동 송신/수신 기능을 수행하는 PCan의 쓰레드를 시작한다.
    ///////////////////////////////////////////
    pCan->Thread_Begin();
    dprintf(("<== PCAN Thread Begin.\n"));

    ///////////////////////////////////////////
	// 테스트용 메인 쓰레드를 시작한다.
    ///////////////////////////////////////////
    tMain_Thread_Stop = false;
    pfMain_Thread = AfxBeginThread(Main_Thread, NULL);
    dprintf(("<== Test Main Thread Begin.\n"));

    return true;
}

bool Main_Thread_End(void)
{
	char  szMsg[256];
	int nListMax;

	int nListPos;
	FILE *pFileLog;
    ///////////////////////////////////////////
	// 테스트용 메인 쓰레드를 종료한다.
    ///////////////////////////////////////////
    if (pfMain_Thread != NULL)
    {
        tMain_Thread_Stop = true;
        WaitForSingleObject(pfMain_Thread->m_hThread, INFINITE);
        pfMain_Thread = NULL;
        dprintf(("<== Test Main Thead End\n"));
    }

		////////Save Log//////
#if 1
	pFileLog = fopen("AAVM_PCB.log", "a+t");//fopen("D:\\PLK_AAVM\\AAVM_PCB.log", "a+t");
	if (pFileLog != NULL)
	{
		CTime oTime = CTime::GetCurrentTime();

		fseek(pFileLog, SEEK_END, 0);
#if 1
		fprintf(pFileLog, "\AAVM PCB 테스트 통계: 양품 %d, 불량 %d, 전체 %d, 양품률 %s\n",
			pMainDlg->m_EDIT_TEST_NUM_OK,
			pMainDlg->m_EDIT_TEST_NUM_ALL - pMainDlg->m_EDIT_TEST_NUM_OK,
			pMainDlg->m_EDIT_TEST_NUM_ALL,
			pMainDlg->m_EDIT_TEST_NUM_RATE);
#endif
		fprintf(pFileLog, "AAVM PCB 테스트 종료: %04d-%02d-%02d %02d:%02d:%02d\n\n",
			oTime.GetYear(), oTime.GetMonth(), oTime.GetDay(),
			oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());

		nListMax = pMainDlg->m_LIST_PROCESS.GetItemCount();
		for (nListPos = 0; nListPos < nListMax; nListPos++)
		{
			pMainDlg->m_LIST_PROCESS.GetItemText(nListPos, 0, szMsg, sizeof(szMsg));
			fprintf(pFileLog, "%s, ", szMsg);

			pMainDlg->m_LIST_PROCESS.GetItemText(nListPos, 1, szMsg, sizeof(szMsg));
			fprintf(pFileLog, "%s, ", szMsg);

			pMainDlg->m_LIST_PROCESS.GetItemText(nListPos, 2, szMsg, sizeof(szMsg));
			fprintf(pFileLog, "%s, ", szMsg);

			pMainDlg->m_LIST_PROCESS.GetItemText(nListPos, 3, szMsg, sizeof(szMsg));
			fprintf(pFileLog, "%s\n", szMsg);
		}
		fprintf(pFileLog, "----------------------------------------------------\n");
		fclose(pFileLog);

		//dprintf(("Log File <%s> Open Success: End.\n", (LPCTSTR)pSetupDlg->m_EDIT_FILENAME_LOG));
		pMainDlg->m_EDIT_Barcode.Format("");
	}
	else
	{
		//strMsg.Format("Log File <%s> Open Fail: End!\n", (LPCTSTR)pSetupDlg->m_EDIT_FILENAME_LOG);
		//AfxMessageBox(strMsg);
		//dprintf(((LPCTSTR)strMsg));
	}
#endif

    ///////////////////////////////////////////
    // CPCan 개체를 없앤다.
    //      CPCan 시작할 때 생성된 개체들과 쓰레드는 자동으로 소멸된다.
    //      PCANBasic.dll도 자동으로 언로드된다.
    ///////////////////////////////////////////
    if (pCan != NULL)
    {
        delete pCan;
        pCan = NULL;
        dprintf(("<== PCAN Destroyed.\n"));
    }
	ongoing = false;
    ///////////////////////////////////////////
    // 10ms Timer를 해제한다.
    ///////////////////////////////////////////
    CTimerClose();
    dprintf(("<== CTimer Released.\n\n"));

    return true;
}

