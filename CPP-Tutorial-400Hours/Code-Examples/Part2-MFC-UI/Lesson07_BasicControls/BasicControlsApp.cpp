// BasicControlsApp.cpp
#include "StdAfx.h"
#include "BasicControlsApp.h"
#include "BasicControlsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CBasicControlsApp, CWinApp)
END_MESSAGE_MAP()

CBasicControlsApp::CBasicControlsApp() {}
CBasicControlsApp theApp;

BOOL CBasicControlsApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CBasicControlsDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
