// DocumentDemoApp.cpp
#include "StdAfx.h"
#include "DocumentDemoApp.h"
#include "DocumentDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CDocumentDemoApp, CWinApp)
END_MESSAGE_MAP()

CDocumentDemoApp::CDocumentDemoApp() {}
CDocumentDemoApp theApp;

BOOL CDocumentDemoApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CDocumentDemoDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
