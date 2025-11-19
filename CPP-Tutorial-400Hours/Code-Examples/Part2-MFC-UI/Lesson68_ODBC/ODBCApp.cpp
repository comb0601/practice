// ODBCApp.cpp
#include "StdAfx.h"
#include "ODBCApp.h"
#include "ODBCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CODBCApp, CWinApp)
END_MESSAGE_MAP()

CODBCApp::CODBCApp() {}
CODBCApp theApp;

BOOL CODBCApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CODBCDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
