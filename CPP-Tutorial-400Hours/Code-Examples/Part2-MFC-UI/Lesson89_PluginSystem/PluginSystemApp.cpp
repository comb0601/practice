// PluginSystemApp.cpp
#include "StdAfx.h"
#include "PluginSystemApp.h"
#include "PluginSystemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CPluginSystemApp, CWinApp)
END_MESSAGE_MAP()

CPluginSystemApp::CPluginSystemApp() {}
CPluginSystemApp theApp;

BOOL CPluginSystemApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CPluginSystemDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
