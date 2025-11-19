// ListControlReportApp.cpp
#include "StdAfx.h"
#include "ListControlReportApp.h"
#include "ListControlReportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CListControlReportApp, CWinApp)
END_MESSAGE_MAP()

CListControlReportApp::CListControlReportApp() {}
CListControlReportApp theApp;

BOOL CListControlReportApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CListControlReportDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
