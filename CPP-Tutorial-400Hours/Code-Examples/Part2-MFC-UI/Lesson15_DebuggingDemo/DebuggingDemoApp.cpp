// DebuggingDemoApp.cpp
#include "StdAfx.h"
#include "DebuggingDemoApp.h"
#include "DebuggingDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CDebuggingDemoApp, CWinApp)
END_MESSAGE_MAP()

CDebuggingDemoApp::CDebuggingDemoApp() {}
CDebuggingDemoApp theApp;

BOOL CDebuggingDemoApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CDebuggingDemoDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
