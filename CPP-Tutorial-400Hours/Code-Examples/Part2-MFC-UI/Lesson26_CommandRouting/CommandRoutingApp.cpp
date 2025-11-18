// CommandRoutingApp.cpp
#include "StdAfx.h"
#include "CommandRoutingApp.h"
#include "CommandRoutingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CCommandRoutingApp, CWinApp)
END_MESSAGE_MAP()

CCommandRoutingApp::CCommandRoutingApp() {}
CCommandRoutingApp theApp;

BOOL CCommandRoutingApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CCommandRoutingDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
