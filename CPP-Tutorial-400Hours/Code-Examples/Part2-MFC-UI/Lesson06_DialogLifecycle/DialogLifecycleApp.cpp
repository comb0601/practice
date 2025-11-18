// DialogLifecycleApp.cpp
#include "StdAfx.h"
#include "DialogLifecycleApp.h"
#include "DialogLifecycleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CDialogLifecycleApp, CWinApp)
END_MESSAGE_MAP()

CDialogLifecycleApp::CDialogLifecycleApp() {}
CDialogLifecycleApp theApp;

BOOL CDialogLifecycleApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CDialogLifecycleDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
