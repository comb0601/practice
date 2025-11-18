// EventHandlingApp.cpp
#include "StdAfx.h"
#include "EventHandlingApp.h"
#include "EventHandlingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CEventHandlingApp, CWinApp)
END_MESSAGE_MAP()

CEventHandlingApp::CEventHandlingApp() {}
CEventHandlingApp theApp;

BOOL CEventHandlingApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CEventHandlingDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
