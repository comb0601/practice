// DatabaseAppApp.cpp
#include "StdAfx.h"
#include "DatabaseAppApp.h"
#include "DatabaseAppDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CDatabaseAppApp, CWinApp)
END_MESSAGE_MAP()

CDatabaseAppApp::CDatabaseAppApp() {}
CDatabaseAppApp theApp;

BOOL CDatabaseAppApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CDatabaseAppDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
