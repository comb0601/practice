// SplitterWindowApp.cpp
#include "StdAfx.h"
#include "SplitterWindowApp.h"
#include "SplitterWindowDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CSplitterWindowApp, CWinApp)
END_MESSAGE_MAP()

CSplitterWindowApp::CSplitterWindowApp() {}
CSplitterWindowApp theApp;

BOOL CSplitterWindowApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CSplitterWindowDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
