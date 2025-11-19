// ExtensionDLLApp.cpp
#include "StdAfx.h"
#include "ExtensionDLLApp.h"
#include "ExtensionDLLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CExtensionDLLApp, CWinApp)
END_MESSAGE_MAP()

CExtensionDLLApp::CExtensionDLLApp() {}
CExtensionDLLApp theApp;

BOOL CExtensionDLLApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CExtensionDLLDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
