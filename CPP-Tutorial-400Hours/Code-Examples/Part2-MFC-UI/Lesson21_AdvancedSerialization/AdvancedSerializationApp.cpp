// AdvancedSerializationApp.cpp
#include "StdAfx.h"
#include "AdvancedSerializationApp.h"
#include "AdvancedSerializationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CAdvancedSerializationApp, CWinApp)
END_MESSAGE_MAP()

CAdvancedSerializationApp::CAdvancedSerializationApp() {}
CAdvancedSerializationApp theApp;

BOOL CAdvancedSerializationApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CAdvancedSerializationDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
