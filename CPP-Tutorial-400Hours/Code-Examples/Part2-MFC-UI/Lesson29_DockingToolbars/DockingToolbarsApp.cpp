// DockingToolbarsApp.cpp
#include "StdAfx.h"
#include "DockingToolbarsApp.h"
#include "DockingToolbarsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CDockingToolbarsApp, CWinApp)
END_MESSAGE_MAP()

CDockingToolbarsApp::CDockingToolbarsApp() {}
CDockingToolbarsApp theApp;

BOOL CDockingToolbarsApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CDockingToolbarsDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
