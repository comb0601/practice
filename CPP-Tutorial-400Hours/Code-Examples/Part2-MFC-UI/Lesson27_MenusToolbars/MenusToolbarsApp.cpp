// MenusToolbarsApp.cpp
#include "StdAfx.h"
#include "MenusToolbarsApp.h"
#include "MenusToolbarsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMenusToolbarsApp, CWinApp)
END_MESSAGE_MAP()

CMenusToolbarsApp::CMenusToolbarsApp() {}
CMenusToolbarsApp theApp;

BOOL CMenusToolbarsApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CMenusToolbarsDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
