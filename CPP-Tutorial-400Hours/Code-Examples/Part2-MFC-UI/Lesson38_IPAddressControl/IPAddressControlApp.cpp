// IPAddressControlApp.cpp
#include "StdAfx.h"
#include "IPAddressControlApp.h"
#include "IPAddressControlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CIPAddressControlApp, CWinApp)
END_MESSAGE_MAP()

CIPAddressControlApp::CIPAddressControlApp() {}
CIPAddressControlApp theApp;

BOOL CIPAddressControlApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CIPAddressControlDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
