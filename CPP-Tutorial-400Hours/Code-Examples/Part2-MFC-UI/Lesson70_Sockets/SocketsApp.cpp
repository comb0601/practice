// SocketsApp.cpp
#include "StdAfx.h"
#include "SocketsApp.h"
#include "SocketsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CSocketsApp, CWinApp)
END_MESSAGE_MAP()

CSocketsApp::CSocketsApp() {}
CSocketsApp theApp;

BOOL CSocketsApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CSocketsDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
