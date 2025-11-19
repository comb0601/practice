// RegistryApp.cpp
#include "StdAfx.h"
#include "RegistryApp.h"
#include "RegistryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CRegistryApp, CWinApp)
END_MESSAGE_MAP()

CRegistryApp::CRegistryApp() {}
CRegistryApp theApp;

BOOL CRegistryApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CRegistryDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
