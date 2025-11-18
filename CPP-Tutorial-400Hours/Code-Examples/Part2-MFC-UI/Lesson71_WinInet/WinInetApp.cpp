// WinInetApp.cpp
#include "StdAfx.h"
#include "WinInetApp.h"
#include "WinInetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CWinInetApp, CWinApp)
END_MESSAGE_MAP()

CWinInetApp::CWinInetApp() {}
CWinInetApp theApp;

BOOL CWinInetApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CWinInetDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
