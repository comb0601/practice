// FinalProjectApp.cpp
#include "StdAfx.h"
#include "FinalProjectApp.h"
#include "FinalProjectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CFinalProjectApp, CWinApp)
END_MESSAGE_MAP()

CFinalProjectApp::CFinalProjectApp() {}
CFinalProjectApp theApp;

BOOL CFinalProjectApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CFinalProjectDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
