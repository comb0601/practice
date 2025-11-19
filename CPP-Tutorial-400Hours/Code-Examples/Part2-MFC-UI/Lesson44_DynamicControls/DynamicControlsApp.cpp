// DynamicControlsApp.cpp
#include "StdAfx.h"
#include "DynamicControlsApp.h"
#include "DynamicControlsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CDynamicControlsApp, CWinApp)
END_MESSAGE_MAP()

CDynamicControlsApp::CDynamicControlsApp() {}
CDynamicControlsApp theApp;

BOOL CDynamicControlsApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CDynamicControlsDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
