// WorkerThreadsApp.cpp
#include "StdAfx.h"
#include "WorkerThreadsApp.h"
#include "WorkerThreadsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CWorkerThreadsApp, CWinApp)
END_MESSAGE_MAP()

CWorkerThreadsApp::CWorkerThreadsApp() {}
CWorkerThreadsApp theApp;

BOOL CWorkerThreadsApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CWorkerThreadsDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
