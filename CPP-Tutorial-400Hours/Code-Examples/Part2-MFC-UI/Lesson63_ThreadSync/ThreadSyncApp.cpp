// ThreadSyncApp.cpp
#include "StdAfx.h"
#include "ThreadSyncApp.h"
#include "ThreadSyncDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CThreadSyncApp, CWinApp)
END_MESSAGE_MAP()

CThreadSyncApp::CThreadSyncApp() {}
CThreadSyncApp theApp;

BOOL CThreadSyncApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CThreadSyncDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
