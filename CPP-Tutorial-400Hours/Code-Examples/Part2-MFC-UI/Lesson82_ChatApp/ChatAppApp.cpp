// ChatAppApp.cpp
#include "StdAfx.h"
#include "ChatAppApp.h"
#include "ChatAppDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CChatAppApp, CWinApp)
END_MESSAGE_MAP()

CChatAppApp::CChatAppApp() {}
CChatAppApp theApp;

BOOL CChatAppApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CChatAppDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
