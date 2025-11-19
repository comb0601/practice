// AllControlsApp.cpp
#include "StdAfx.h"
#include "AllControlsApp.h"
#include "AllControlsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CAllControlsApp, CWinApp)
END_MESSAGE_MAP()

CAllControlsApp::CAllControlsApp() {}
CAllControlsApp theApp;

BOOL CAllControlsApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CAllControlsDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
