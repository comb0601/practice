// WebBrowserApp.cpp
#include "StdAfx.h"
#include "WebBrowserApp.h"
#include "WebBrowserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CWebBrowserApp, CWinApp)
END_MESSAGE_MAP()

CWebBrowserApp::CWebBrowserApp() {}
CWebBrowserApp theApp;

BOOL CWebBrowserApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CWebBrowserDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
