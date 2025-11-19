// StatusBarApp.cpp
#include "StdAfx.h"
#include "StatusBarApp.h"
#include "StatusBarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CStatusBarApp, CWinApp)
END_MESSAGE_MAP()

CStatusBarApp::CStatusBarApp() {}
CStatusBarApp theApp;

BOOL CStatusBarApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CStatusBarDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
