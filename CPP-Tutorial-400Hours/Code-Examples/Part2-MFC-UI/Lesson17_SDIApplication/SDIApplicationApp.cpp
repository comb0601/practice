// SDIApplicationApp.cpp
#include "StdAfx.h"
#include "SDIApplicationApp.h"
#include "SDIApplicationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CSDIApplicationApp, CWinApp)
END_MESSAGE_MAP()

CSDIApplicationApp::CSDIApplicationApp() {}
CSDIApplicationApp theApp;

BOOL CSDIApplicationApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CSDIApplicationDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
