// DDXDemoApp.cpp
#include "StdAfx.h"
#include "DDXDemoApp.h"
#include "DDXDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CDDXDemoApp, CWinApp)
END_MESSAGE_MAP()

CDDXDemoApp::CDDXDemoApp() {}
CDDXDemoApp theApp;

BOOL CDDXDemoApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CDDXDemoDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
