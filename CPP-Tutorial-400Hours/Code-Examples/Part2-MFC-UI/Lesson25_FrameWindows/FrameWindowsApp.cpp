// FrameWindowsApp.cpp
#include "StdAfx.h"
#include "FrameWindowsApp.h"
#include "FrameWindowsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CFrameWindowsApp, CWinApp)
END_MESSAGE_MAP()

CFrameWindowsApp::CFrameWindowsApp() {}
CFrameWindowsApp theApp;

BOOL CFrameWindowsApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CFrameWindowsDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
