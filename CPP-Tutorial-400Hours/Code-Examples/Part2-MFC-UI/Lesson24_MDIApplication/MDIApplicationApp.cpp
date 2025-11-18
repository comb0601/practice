// MDIApplicationApp.cpp
#include "StdAfx.h"
#include "MDIApplicationApp.h"
#include "MDIApplicationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMDIApplicationApp, CWinApp)
END_MESSAGE_MAP()

CMDIApplicationApp::CMDIApplicationApp() {}
CMDIApplicationApp theApp;

BOOL CMDIApplicationApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CMDIApplicationDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
