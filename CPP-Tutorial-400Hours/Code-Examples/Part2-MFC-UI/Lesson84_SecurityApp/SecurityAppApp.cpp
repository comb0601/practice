// SecurityAppApp.cpp
#include "StdAfx.h"
#include "SecurityAppApp.h"
#include "SecurityAppDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CSecurityAppApp, CWinApp)
END_MESSAGE_MAP()

CSecurityAppApp::CSecurityAppApp() {}
CSecurityAppApp theApp;

BOOL CSecurityAppApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CSecurityAppDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
