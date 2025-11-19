// ViewDemoApp.cpp
#include "StdAfx.h"
#include "ViewDemoApp.h"
#include "ViewDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CViewDemoApp, CWinApp)
END_MESSAGE_MAP()

CViewDemoApp::CViewDemoApp() {}
CViewDemoApp theApp;

BOOL CViewDemoApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CViewDemoDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
