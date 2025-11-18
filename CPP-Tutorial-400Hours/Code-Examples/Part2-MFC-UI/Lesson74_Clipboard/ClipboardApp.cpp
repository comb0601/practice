// ClipboardApp.cpp
#include "StdAfx.h"
#include "ClipboardApp.h"
#include "ClipboardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CClipboardApp, CWinApp)
END_MESSAGE_MAP()

CClipboardApp::CClipboardApp() {}
CClipboardApp theApp;

BOOL CClipboardApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CClipboardDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
