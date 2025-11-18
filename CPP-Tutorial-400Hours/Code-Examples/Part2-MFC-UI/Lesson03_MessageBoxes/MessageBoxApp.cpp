// MessageBoxApp.cpp - Application class implementation

#include "StdAfx.h"
#include "MessageBoxApp.h"
#include "MessageBoxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMessageBoxApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CMessageBoxApp::CMessageBoxApp()
{
}

CMessageBoxApp theApp;

BOOL CMessageBoxApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC MessageBox Tutorial"));

    CMessageBoxDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
