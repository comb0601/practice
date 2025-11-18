// TabControlApp.cpp
#include "StdAfx.h"
#include "TabControlApp.h"
#include "TabControlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CTabControlApp, CWinApp)
END_MESSAGE_MAP()

CTabControlApp::CTabControlApp() {}
CTabControlApp theApp;

BOOL CTabControlApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CTabControlDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
