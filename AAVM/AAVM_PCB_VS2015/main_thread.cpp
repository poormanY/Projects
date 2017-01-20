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

CPcan* pCan;    // ����� PCAN ��ü

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
		fprintf(pFileLog, "\AAVM PCB �׽�Ʈ ����: %04d-%02d-%02d %02d:%02d:%02d\n",
		oTime.GetYear(), oTime.GetMonth(), oTime.GetDay(),
		oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());

		fprintf(pFileLog, "Serial Number: %s\n",pMainDlg->m_EDIT_Barcode);

		fclose(pFileLog);
	}

    //while (tMain_Thread_Stop == false)
	if (pMainDlg->m_COMBO_PROGRAM_TYPE == "����")
		programtype = 1;
	else if (pMainDlg->m_COMBO_PROGRAM_TYPE == "ī�޶�")
		programtype = 2;
	else if (pMainDlg->m_COMBO_PROGRAM_TYPE == "����")
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
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "AAVM CID ��ǰ �ڵ�");
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
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }


        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "AAVM CID ������ �ڵ�");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "22");

        stMsg.Format("%d", bCAN_CID_MC);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (bCAN_CID_MC == 22)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "AAVM CID ��������");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "2016-08-22");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, szCAN_CID_MDay);
        if (strcmp(szCAN_CID_MDay, "2016-08-22") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "AAVM CID ����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "2.8.0");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, szCAN_CID_Ver);
        if (strcmp(szCAN_CID_Ver, "2.8.0") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "AAVM CID ���̸�");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "AAVMHW60");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, szCAN_CID_ModelName);
        if (strcmp(szCAN_CID_ModelName, "AAVMHW60") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
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
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "AAVM H/W ����");
        pszSrc = szCAN_Ver_HW;
        pszTgt = "1.5";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "BF609 F/W ����");
        pszSrc = szCAN_Ver_BF;
        pszTgt = "2.8";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "���Ӷ��� F/W ����");
        pszSrc = szCAN_Ver_EL_Main;
        pszTgt = "1.12";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "���Ӷ��� �ռ� ����");
        pszSrc = szCAN_Ver_EL_Sub;
        pszTgt = "����";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
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
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "ī�޶� ���� - ����");
        pszSrc = szCAN_Ver_Camera_Front;
        pszTgt = "3.3";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "ī�޶� ���� - �Ĺ�");
        pszSrc = szCAN_Ver_Camera_Rear;
        pszTgt = "3.3";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "ī�޶� ���� - ����");
        pszSrc = szCAN_Ver_Camera_Left;
        pszTgt = "3.3";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "ī�޶� ���� - ����");
        pszSrc = szCAN_Ver_Camera_Right;
        pszTgt = "3.3";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
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
			pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "����� �ռ� ��");
			if (Car_Types[ii] == 0)
			{
				pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "������ �ռ� �� ����");
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
						pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
						break;
					}
				}
				if (strcmp(pszSrc, pszTgt) != 0)
				{
					pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "����� �ռ� �� ����");
					pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
					ret = false;
					break;
				}
			}
		}
		/*
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "����� �ռ� �� 1");
        pszSrc = arszCAN_MLST[0];
        pszTgt = arpszModelName[13];
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "����� �ռ� �� 2");
        pszSrc = arszCAN_MLST[1];
        pszTgt = arpszModelName[14];
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "����� �ռ� �� 3");
        pszSrc = arszCAN_MLST[2];
        pszTgt = arpszModelName[15];
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "����� �ռ� �� 4");
        pszSrc = arszCAN_MLST[3];
        pszTgt = arpszModelName[16];
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "����� �ռ� �� 5");
        pszSrc = arszCAN_MLST[4];
        pszTgt = arpszModelName[19];
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }


        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "����� �ռ� �� 6");
        pszSrc = arszCAN_MLST[5];
        pszTgt = "����";
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
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
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "BF609 ���� �Է� ����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "����");

        if ((bCAN_AAVST_BF609_Stat & _BF_ERR_CAMERA) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "����");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "����");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "BF609 vs ���޶��� ��� ����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "����");

        if ((bCAN_AAVST_BF609_Stat & _BF_ERR_EMERALD_ST) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "����");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "����");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "BF609 CAN ESROS ��� ����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "����");

        if ((bCAN_AAVST_BF609_Stat & _BF_ERR_CAN_ESROS) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "����");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "����");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "BF609 CAN HP2 ��� ����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "����");

        if ((bCAN_AAVST_BF609_Stat & _BF_ERR_CAN_HP2) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "����");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "����");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "BF609 CAN HP3 ��� ����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "����");

        if ((bCAN_AAVST_BF609_Stat & _BF_ERR_CAN_HP3) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "����");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, "����");
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "BF609 ó�� �ӵ�");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "5 �̻�");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_BF609_FPS);
        if (bCAN_AAVST_BF609_FPS >= 50)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }


        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "���޶��� ����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "�̺���");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_EMERALD_STAT);
        if (strcmp(pMainDlg->m_EDIT_EMERALD_STAT, "�̺���") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }
	}

	if ( programtype > 1)
	{   
        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "ī�޶� ���� - ����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "����");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_CAMERA_STAT_FRONT);
        if (strcmp(pMainDlg->m_EDIT_CAMERA_STAT_FRONT, "����") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "ī�޶� ���� - �Ĺ�");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "����");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_CAMERA_STAT_REAR);
        if (strcmp(pMainDlg->m_EDIT_CAMERA_STAT_REAR, "����") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "ī�޶� ���� - ����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "����");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_CAMERA_STAT_LEFT);
        if (strcmp(pMainDlg->m_EDIT_CAMERA_STAT_LEFT, "����") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "ī�޶� ���� - ����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "����");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_CAMERA_STAT_RIGHT);
        if (strcmp(pMainDlg->m_EDIT_CAMERA_STAT_RIGHT, "����") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
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
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "��� ȭ�� �� ��� - �ȴ�(��)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "�ȴ�(��)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "�ȴ�(��)") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
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
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "��� ȭ�� �� ��� - �ȴ�(��)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "�ȴ�(��)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "�ȴ�(��)") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
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
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "��� ȭ�� �� ��� - �ȴ�(��)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "�ȴ�(��)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "�ȴ�(��)") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
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
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "��� ȭ�� �� ��� - �ȴ�(��)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "�ȴ�(��)");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "�ȴ�(��)") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
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
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "��� ȭ�� �� ��� - �����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "�����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "�����") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
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
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "��� ȭ�� �� ��� - ����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "����") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
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
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "��� ȭ�� �� ��� - �Ĺ�");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "�Ĺ�");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "�Ĺ�") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
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
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "��� ȭ�� �� ��� - ����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "����") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
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
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "��� ȭ�� �� ��� - ����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "����") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
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
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "��� ȭ�� �� ��� - 4ä��");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "4ä��");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pMainDlg->m_EDIT_AAVM_VIEW_MODE);
        if (strcmp(pMainDlg->m_EDIT_AAVM_VIEW_MODE, "4ä��") == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
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

        // �νĸ��
        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "�νĸ�� - ����� ����ġ Off");
        pszTgt = "����� Off(On)";
        stMsg.Format("����� Off(%s)", 
            (pMainDlg->m_CHECK_MODE_MANUAL_OFF == FALSE)? "Off": "On");
        pszSrc = stMsg;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        // IMOD ����
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "IMOD ���� - ����� ����ġ Off");
        pszTgt = "���� Off";
        pszSrc = pMainDlg->m_EDIT_AAVM_IMOD_STATUS;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        MakeMsg_AAVCS(&oCanMsg, 0, 1);
        pCan->Write(&oCanMsg);

        // �νĸ��
        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "�νĸ�� - ����� ����ġ On");
        pszTgt = "����� Off(Off)";
        stMsg.Format("����� Off(%s)", 
            (pMainDlg->m_CHECK_MODE_MANUAL_OFF == FALSE)? "Off": "On");
        pszSrc = stMsg;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        // IMOD ����
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "IMOD ���� - ����� ����ġ On");
        pszTgt = "�����۵�";
        pszSrc = pMainDlg->m_EDIT_AAVM_IMOD_STATUS;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

#if 1
        /////////////////////////////////////
        pMainDlg->m_EDIT_ESROS_WIPER = 1;
        pMainDlg->m_EDIT_ESROS_INTWIPER = 0;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        // �νĸ��
        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "�νĸ�� - Wiper On");
        pszTgt = "Wiper(On)";
        stMsg.Format("Wiper(%s)", 
            (pMainDlg->m_CHECK_MODE_WIPER == FALSE)? "Off": "On");
        pszSrc = stMsg;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        // IMOD ����
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+7) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "IMOD ���� - Wiper On");
        pszTgt = "�ڵ� Off";
        pszSrc = pMainDlg->m_EDIT_AAVM_IMOD_STATUS;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        pMainDlg->m_EDIT_ESROS_WIPER = 0;
        pMainDlg->m_EDIT_ESROS_INTWIPER = 0;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        // �νĸ��
        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "�νĸ�� - Wiper Off");
        pszTgt = "Wiper(Off)";
        stMsg.Format("Wiper(%s)", 
            (pMainDlg->m_CHECK_MODE_WIPER == FALSE)? "Off": "On");
        pszSrc = stMsg;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        // IMOD ����
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+7) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "IMOD ���� - Wiper Off");
        pszTgt = "�����۵�";
        pszSrc = pMainDlg->m_EDIT_AAVM_IMOD_STATUS;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        pMainDlg->m_EDIT_ESROS_WIPER = 0;
        pMainDlg->m_EDIT_ESROS_INTWIPER = 1;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        // �νĸ��
        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "�νĸ�� - Int.Wiper On");
        pszTgt = "Int.Wiper(On)";
        stMsg.Format("Int.Wiper(%s)", 
            (pMainDlg->m_CHECK_MODE_INTWIPER == FALSE)? "Off": "On");
        pszSrc = stMsg;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        // IMOD ����
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+7) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "IMOD ���� - Int.Wiper On");
        pszTgt = "�ڵ� Off";
        pszSrc = pMainDlg->m_EDIT_AAVM_IMOD_STATUS;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        /////////////////////////////////////
        pMainDlg->m_EDIT_ESROS_WIPER = 0;
        pMainDlg->m_EDIT_ESROS_INTWIPER = 0;
        pMainDlg->PostMessage(_WM_THREAD_UPDATE);

        // �νĸ��
        wCAN_Prev_GetCnt = wCAN_AAVM_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVM_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "�νĸ�� - Int.Wiper Off");
        pszTgt = "Int.Wiper(Off)";
        stMsg.Format("Int.Wiper(%s)", 
            (pMainDlg->m_CHECK_MODE_INTWIPER == FALSE)? "Off": "On");
        pszSrc = stMsg;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        // IMOD ����
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+11) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "IMOD ���� - Int.Wiper Off");
        pszTgt = "�����۵�";
        pszSrc = pMainDlg->m_EDIT_AAVM_IMOD_STATUS;
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, pszTgt);
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, pszSrc);
        if (strcmp(pszSrc, pszTgt) == 0)
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
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
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "�νĸ�� - ������");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "����(On), ȸ��(Off), �۾�(Off)");

        stMsg.Format("����(%s), ȸ��(%s), �۾�(%s)", 
            (pMainDlg->m_CHECK_MODE_MOVE  == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_SWING == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_WORK  == FALSE)? "Off": "On");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "����(On), ȸ��(Off), �۾�(Off)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
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
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "�νĸ�� - ȸ����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "����(Off), ȸ��(On), �۾�(Off)");

        stMsg.Format("����(%s), ȸ��(%s), �۾�(%s)", 
            (pMainDlg->m_CHECK_MODE_MOVE  == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_SWING == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_WORK  == FALSE)? "Off": "On");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "����(Off), ȸ��(On), �۾�(Off)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
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
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "�νĸ�� - �۾���");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "����(Off), ȸ��(Off), �۾�(On)");

        stMsg.Format("����(%s), ȸ��(%s), �۾�(%s)", 
            (pMainDlg->m_CHECK_MODE_MOVE  == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_SWING == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_WORK  == FALSE)? "Off": "On");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "����(Off), ȸ��(Off), �۾�(On)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
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
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "�νĸ�� - �۾���");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "����(Off), ȸ��(Off), �۾�(On)");

        stMsg.Format("����(%s), ȸ��(%s), �۾�(%s)", 
            (pMainDlg->m_CHECK_MODE_MOVE  == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_SWING == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_WORK  == FALSE)? "Off": "On");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "����(Off), ȸ��(Off), �۾�(On)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
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
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "�νĸ�� - �۾���");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "����(Off), ȸ��(Off), �۾�(On)");

        stMsg.Format("����(%s), ȸ��(%s), �۾�(%s)", 
            (pMainDlg->m_CHECK_MODE_MOVE  == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_SWING == FALSE)? "Off": "On",
            (pMainDlg->m_CHECK_MODE_WORK  == FALSE)? "Off": "On");

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "����(Off), ȸ��(Off), �۾�(On)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
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
        // ���� ī�޶�� ���, �������� ���
        wCAN_Prev_GetCnt = wCAN_AAVST_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVST_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "ī�޶� ��Ⱚ ����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "��(>5), ��(>5), ��(>5), ��(>5)");

        stMsg.Format("��(%d), ��(%d), ��(%d), ��(%d)",
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
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }
#endif
#if 0

        /////////////////////////////////////
        /////////////////////////////////////
        /////////////////////////////////////
        // ���� ī�޶� ����� ������ �ۿ��� �̵��ϴ� ������, �������� ��������
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "�����ν� �׽�Ʈ - ����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "��(����), ��(-), ��(-), ��(-)");

        stMsg.Format("��(%s), ��(%s), ��(%s), ��(%s)",
            pMainDlg->m_EDIT_AAVM_WARNING_FRONT,
            pMainDlg->m_EDIT_AAVM_WARNING_REAR,
            pMainDlg->m_EDIT_AAVM_WARNING_LEFT,
            pMainDlg->m_EDIT_AAVM_WARNING_RIGHT);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "��(����), ��(-), ��(-), ��(-)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        // �Ĺ� ī�޶� ����� ������ �ȿ��� �̵��ϴ� ������, �������� ��������
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "�����ν� �׽�Ʈ - �Ĺ�");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "��(-), ��(����), ��(-), ��(-)");

        stMsg.Format("��(%s), ��(%s), ��(%s), ��(%s)",
            pMainDlg->m_EDIT_AAVM_WARNING_FRONT,
            pMainDlg->m_EDIT_AAVM_WARNING_REAR,
            pMainDlg->m_EDIT_AAVM_WARNING_LEFT,
            pMainDlg->m_EDIT_AAVM_WARNING_RIGHT);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "��(-), ��(����), ��(-), ��(-)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        // ���� ī�޶� ����� ������ �ۿ��� �̵��ϴ� ������, �������� ��������
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "�����ν� �׽�Ʈ - ����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "��(-), ��(-), ��(����), ��(-)");

        stMsg.Format("��(%s), ��(%s), ��(%s), ��(%s)",
            pMainDlg->m_EDIT_AAVM_WARNING_FRONT,
            pMainDlg->m_EDIT_AAVM_WARNING_REAR,
            pMainDlg->m_EDIT_AAVM_WARNING_LEFT,
            pMainDlg->m_EDIT_AAVM_WARNING_RIGHT);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "��(-), ��(-), ��(����), ��(-)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        // ���� ī�޶� ����� ������ �ȿ��� �̵��ϴ� ������, �������� ��������
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "�����ν� �׽�Ʈ - ����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "��(-), ��(-), ��(-), ��(����)");

        stMsg.Format("��(%s), ��(%s), ��(%s), ��(%s)",
            pMainDlg->m_EDIT_AAVM_WARNING_FRONT,
            pMainDlg->m_EDIT_AAVM_WARNING_REAR,
            pMainDlg->m_EDIT_AAVM_WARNING_LEFT,
            pMainDlg->m_EDIT_AAVM_WARNING_RIGHT);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "��(-), ��(-), ��(-), ��(����)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        // ��� ī�޶� ����� ������ �ȿ��� �̵��ϴ� ������
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "�����ν� �׽�Ʈ - ��� ����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "��(����), ��(����), ��(����), ��(����)");

        stMsg.Format("��(%s), ��(%s), ��(%s), ��(%s)",
            pMainDlg->m_EDIT_AAVM_WARNING_FRONT,
            pMainDlg->m_EDIT_AAVM_WARNING_REAR,
            pMainDlg->m_EDIT_AAVM_WARNING_LEFT,
            pMainDlg->m_EDIT_AAVM_WARNING_RIGHT);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "��(����), ��(����), ��(����), ��(����)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }

        // ��� ī�޶� ����� ������ �ۿ��� �̵��ϴ� ������
        wCAN_Prev_GetCnt = wCAN_AAVO_GetCnt;
        while ( (wCAN_Prev_GetCnt+1) >= wCAN_AAVO_GetCnt)
        {
			if(ongoing == false) return 0;
            Sleep(10);
        }

        nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
        pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "�����ν� �׽�Ʈ - ��� ����");
        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "��(����), ��(����), ��(����), ��(����)");

        stMsg.Format("��(%s), ��(%s), ��(%s), ��(%s)",
            pMainDlg->m_EDIT_AAVM_WARNING_FRONT,
            pMainDlg->m_EDIT_AAVM_WARNING_REAR,
            pMainDlg->m_EDIT_AAVM_WARNING_LEFT,
            pMainDlg->m_EDIT_AAVM_WARNING_RIGHT);

        pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 2, stMsg);
        if (stMsg == "��(����), ��(����), ��(����), ��(����)")
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
        }
        else
        {
            pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
			ret = false;
        }
#endif
    }
    //pMainDlg->PostMessage(_WM_THREAD_END);



	nListPos = pMainDlg->m_LIST_PROCESS.GetItemCount();
	pMainDlg->m_LIST_PROCESS.InsertItem(nListPos, "�˻�  ����");
    pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 1, "���� �˻�");

	if (ret==true)
	{
		pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
		pMainDlg->m_EDIT_TEST_NUM_OK++;
		AfxMessageBox("����. �˻�Ϸ�");
	}
	else
	{
		pMainDlg->m_LIST_PROCESS.SetItemText(nListPos, 3, "����");
		Sleep(100);
		AfxMessageBox("�ҷ�. ���հ˻� ����");
		//AfxMessageBox("Error ���� �˻� �����Ͽ����ϴ�");
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
    // �ʱ�ȭ ���°� �´��� Ȯ���Ѵ�.
    ///////////////////////////////////////////
    if ( (pCan != NULL) || (pfMain_Thread != NULL) )
    {
        dprintf(("<== Can't Open PCAN. CPcan ran!\n"));
        return false;
    }

    ///////////////////////////////////////////
    // 1ms Timer�� ��ġ�Ѵ�. (CPcan���� ����)
    ///////////////////////////////////////////
    if (GetTickCountEx_Init() == false)
    {
        dprintf(("<== CTimer Init Error!\n"));
        return false;
    }
    ///////////////////////////////////////////
    // 10ms Timer�� ��ġ�Ѵ�. (j1939���� ����)
    ///////////////////////////////////////////
    if (CTimerInit() == false)
    {
        dprintf(("<== CTimer Init Error!\n"));
        return false;
    }

    ///////////////////////////////////////////
    // CPCan ��ü�� �����Ѵ�.
    ///////////////////////////////////////////
    pCan = new CPcan;
    if (pCan == NULL)
    {
        return false;
    }
    dprintf(("<== Create CPcan Complete!\n"));

    ///////////////////////////////////////////
    // PCANBasic.dll�� �ε��Ѵ�.
    ///////////////////////////////////////////
    if (pCan->Open() == false)
    {
        return false;
    }
    dprintf(("<== Open Complete!\n"));

    ///////////////////////////////////////////
    // 500kbps, Standard(11bits) or Extended(29bits) mode �� �����Ѵ�.
    ///////////////////////////////////////////
    if (pCan->Connect(500, true) == false)
    {
        return false;
    }
    dprintf(("<== Connect Complete!\n"));

    ///////////////////////////////////////////
    // �ڵ����� ������ �ֱ⸶�� CAN �޽����� �۽��ϴ� ��ü�� ����Ѵ�.
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
    // ������ ID�� ���ŵ� CAN �޽����� �ڵ����� ó���ϴ� ��ü�� ����Ѵ�.
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
	// �ڵ� �۽�/���� ����� �����ϴ� PCan�� �����带 �����Ѵ�.
    ///////////////////////////////////////////
    pCan->Thread_Begin();
    dprintf(("<== PCAN Thread Begin.\n"));

    ///////////////////////////////////////////
	// �׽�Ʈ�� ���� �����带 �����Ѵ�.
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
	// �׽�Ʈ�� ���� �����带 �����Ѵ�.
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
		fprintf(pFileLog, "\AAVM PCB �׽�Ʈ ���: ��ǰ %d, �ҷ� %d, ��ü %d, ��ǰ�� %s\n",
			pMainDlg->m_EDIT_TEST_NUM_OK,
			pMainDlg->m_EDIT_TEST_NUM_ALL - pMainDlg->m_EDIT_TEST_NUM_OK,
			pMainDlg->m_EDIT_TEST_NUM_ALL,
			pMainDlg->m_EDIT_TEST_NUM_RATE);
#endif
		fprintf(pFileLog, "AAVM PCB �׽�Ʈ ����: %04d-%02d-%02d %02d:%02d:%02d\n\n",
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
    // CPCan ��ü�� ���ش�.
    //      CPCan ������ �� ������ ��ü��� ������� �ڵ����� �Ҹ�ȴ�.
    //      PCANBasic.dll�� �ڵ����� ��ε�ȴ�.
    ///////////////////////////////////////////
    if (pCan != NULL)
    {
        delete pCan;
        pCan = NULL;
        dprintf(("<== PCAN Destroyed.\n"));
    }
	ongoing = false;
    ///////////////////////////////////////////
    // 10ms Timer�� �����Ѵ�.
    ///////////////////////////////////////////
    CTimerClose();
    dprintf(("<== CTimer Released.\n\n"));

    return true;
}

