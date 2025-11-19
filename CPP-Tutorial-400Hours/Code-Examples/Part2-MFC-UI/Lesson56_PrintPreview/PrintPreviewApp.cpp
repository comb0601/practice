// PrintPreviewApp.cpp
#include "StdAfx.h"
#include "PrintPreviewApp.h"
#include "PrintPreviewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CPrintPreviewApp, CWinApp)
END_MESSAGE_MAP()

CPrintPreviewApp::CPrintPreviewApp() {}
CPrintPreviewApp theApp;

BOOL CPrintPreviewApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CPrintPreviewDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
