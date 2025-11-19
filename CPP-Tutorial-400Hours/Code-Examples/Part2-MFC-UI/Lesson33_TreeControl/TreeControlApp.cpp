// TreeControlApp.cpp
#include "StdAfx.h"
#include "TreeControlApp.h"
#include "TreeControlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CTreeControlApp, CWinApp)
END_MESSAGE_MAP()

CTreeControlApp::CTreeControlApp() {}
CTreeControlApp theApp;

BOOL CTreeControlApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CTreeControlDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
