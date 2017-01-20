
#include "stdafx.h"
#include "MainApp.h"
#include "jdefine.h"
#include "DialogSet.h"
#include "wm_user.h"
#include "barcodemgt.h"
#include "ScrewCounterMgt.h"
#include "IoBoardMgt.h"

#ifdef _DEBUG
#define dprintf(a) printf a
#else
#define dprintf(a)
#endif

extern CDialogSet *pSetupDlg;

bool tSetupThreadStop = false;

static void SetDialogFromPortIn(U16 a_uPortData)
{
	pSetupDlg->m_CHECK_IO_IN_CONNECT   = (a_uPortData & 0x01)? true: false;
}

static U16 GetPortOutFromDialog(void)
{
    U16 uPortData;

    uPortData = 0x0000;

	uPortData |= (pSetupDlg->m_CHECK_IO_OUT_POWER      == false)? 0x00: 0x01;
	uPortData |= (pSetupDlg->m_CHECK_IO_OUT_DISCONNECT == false)? 0x00: 0x02;
	uPortData |= (pSetupDlg->m_CHECK_IO_OUT_LED_Y      == false)? 0x00: 0x04;
	uPortData |= (pSetupDlg->m_CHECK_IO_OUT_LED_G      == false)? 0x00: 0x08;
	uPortData |= (pSetupDlg->m_CHECK_IO_OUT_LED_R      == false)? 0x00: 0x10;
 	//uPortData |= (pSetupDlg->m_CHECK_IO_OUT_BARCODE1   == false)? 0x00: 0x40;
 	//uPortData |= (pSetupDlg->m_CHECK_IO_OUT_BARCODE2   == false)? 0x00: 0x80;
	uPortData |= (pSetupDlg->m_CHECK_IO_OUT_BARCODE1   == false)? 0x00: 0x2000;  //KBR  데이터 수정
	uPortData |= (pSetupDlg->m_CHECK_IO_OUT_BARCODE2   == false)? 0x00: 0x4000;  //KBR  데이터 수정

    return uPortData;
}

UINT SetupThread(LPVOID pParam)
{
    CBarcodeMgt oBarCode1;
    CBarcodeMgt oBarCode2;
    CScrewMgt   oScrew;
    CIoBoardMgt oIoBoard;

    S16 nScrewCountOld;
    U16 uPortInOld, uPortOutOld;
    U16 uPortInNew, uPortOutNew;

    bool tError = false;

    dprintf(("Setup Thread Start\n"));

    if (tError == false)
    {
        if (oBarCode1.Open((LPCSTR)pSetupDlg->m_EDIT_COM_BARCODE1) == true)
        {
            dprintf(("Barcode1 Open Success!\n"));
        }
        else
        {
            AfxMessageBox("Barcode1 Open Fail!\n");
            dprintf(("Barcode1 Open Fail!\n"));
            tError = true && tError;
        }

        oBarCode1.Thread_Begin();
    }

    if (tError == false)
    {
        if (oBarCode2.Open((LPCSTR)pSetupDlg->m_EDIT_COM_BARCODE2) == true)
        {
            dprintf(("Barcode2 Open Success!\n"));
        }
        else
        {
            AfxMessageBox("Barcode2 Open Fail!\n");
            dprintf(("Barcode2 Open Fail!\n"));
            tError = true && tError;
        }

        oBarCode2.Thread_Begin();
    }

    if (tError == false)
    {
        if (oScrew.Open((LPCSTR)pSetupDlg->m_EDIT_COM_SCREW) == true)
        {
            dprintf(("Screw Open Success!\n"));
        }
        else
        {
            AfxMessageBox("Screw Open Fail!\n");
            dprintf(("Screw Open Fail!\n"));
            tError = true && tError;
        }

        oScrew.Thread_Begin();
    }

    if (tError == false)
    {
        if (oIoBoard.Open((LPCSTR)pSetupDlg->m_EDIT_COM_IOBOARD) == true)
        {
            dprintf(("IO Board Open Success!\n"));
        }
        else
        {
            AfxMessageBox("IO Board Open Fail!\n");
            dprintf(("IO Board Open Fail!\n"));
            tError = true && tError;
        }

        pSetupDlg->m_EDIT_TEST_IOBOARD = "KM ";
        oIoBoard.TransferMsg("$01GA001\r");   // 6050(모델명), 4B53(제품군:KM), 0020(Ver2.0)
        pSetupDlg->m_EDIT_TEST_IOBOARD += (oIoBoard.GetMsg()+9);

        pSetupDlg->m_EDIT_TEST_IOBOARD += "  Ver ";
        oIoBoard.TransferMsg("$01GA201\r");   // 6050(모델명), 4B53(제품군:KM), 0020(Ver2.0)
        pSetupDlg->m_EDIT_TEST_IOBOARD += (oIoBoard.GetMsg()+9);

        uPortInOld = oIoBoard.GetInPort();
        SetDialogFromPortIn(uPortInOld);

        uPortOutOld = GetPortOutFromDialog();
        oIoBoard.PutOutPort((U16)uPortOutOld); //KBR
    }

    pSetupDlg->PostMessage(_WM_THREAD_UPDATE);
    nScrewCountOld = 0;

    while ( (tError == false) && (tSetupThreadStop == false) )
    {
        Sleep(10);

        if (oBarCode1.GetCount() != 0)
        {
            pSetupDlg->m_EDIT_TEST_BARCODE1 = (LPCSTR)oBarCode1.GetDataAt(0);
            dprintf(("%s\n", oBarCode1.GetDataAt(0)));

            oBarCode1.DelData();
            pSetupDlg->PostMessage(_WM_THREAD_UPDATE);
        }

        if (oBarCode2.GetCount() != 0)
        {
            pSetupDlg->m_EDIT_TEST_BARCODE2 = (LPCSTR)oBarCode2.GetDataAt(0);
            dprintf(("%s\n", oBarCode2.GetDataAt(0)));

            oBarCode2.DelData();
            pSetupDlg->PostMessage(_WM_THREAD_UPDATE);
        }

        if (oScrew.GetCount() != nScrewCountOld)
        {
            nScrewCountOld = oScrew.GetCount();

            pSetupDlg->m_EDIT_TEST_SCREW.Format("%d", oScrew.GetCount());
            dprintf(("%s\n", oScrew.GetMsg()));

            pSetupDlg->PostMessage(_WM_THREAD_UPDATE);
        }

        uPortInNew = oIoBoard.GetInPort();
        if (uPortInNew != uPortInOld)
        {
            SetDialogFromPortIn(uPortInNew);
            uPortInOld = uPortInNew;

            pSetupDlg->PostMessage(_WM_THREAD_UPDATE);
        }

        uPortOutNew = GetPortOutFromDialog();
        if (uPortOutNew != uPortOutOld)
        {
            oIoBoard.PutOutPort((U16)uPortOutNew);
            uPortOutOld = uPortInNew;
        }
    }

    oBarCode1.Close();
    oBarCode2.Close();
    oScrew.Close();
    oIoBoard.Close();

    dprintf(("Setup Thread End\n\n"));

    if (tError == false)
    {
        pSetupDlg->PostMessage(_WM_THREAD_END, 0);
        return 0;
    }
    else
    {
        pSetupDlg->PostMessage(_WM_THREAD_END, 1);
        return 1;
    }
}

