// ADOApp.cpp
#include "StdAfx.h"
#include "ADOApp.h"
#include "ADODlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CADOApp, CWinApp)
END_MESSAGE_MAP()

CADOApp::CADOApp() {}
CADOApp theApp;

BOOL CADOApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CADODlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
