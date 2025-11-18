// RegularDLLApp.cpp
#include "StdAfx.h"
#include "RegularDLLApp.h"
#include "RegularDLLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CRegularDLLApp, CWinApp)
END_MESSAGE_MAP()

CRegularDLLApp::CRegularDLLApp() {}
CRegularDLLApp theApp;

BOOL CRegularDLLApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CRegularDLLDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
