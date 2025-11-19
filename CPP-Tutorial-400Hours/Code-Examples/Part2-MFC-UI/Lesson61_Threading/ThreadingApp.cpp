// ThreadingApp.cpp
#include "StdAfx.h"
#include "ThreadingApp.h"
#include "ThreadingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CThreadingApp, CWinApp)
END_MESSAGE_MAP()

CThreadingApp::CThreadingApp() {}
CThreadingApp theApp;

BOOL CThreadingApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CThreadingDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
