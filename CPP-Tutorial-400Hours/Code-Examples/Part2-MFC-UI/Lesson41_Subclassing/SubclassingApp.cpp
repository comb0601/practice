// SubclassingApp.cpp
#include "StdAfx.h"
#include "SubclassingApp.h"
#include "SubclassingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CSubclassingApp, CWinApp)
END_MESSAGE_MAP()

CSubclassingApp::CSubclassingApp() {}
CSubclassingApp theApp;

BOOL CSubclassingApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CSubclassingDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
