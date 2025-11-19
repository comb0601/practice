// PrintingBasicApp.cpp
#include "StdAfx.h"
#include "PrintingBasicApp.h"
#include "PrintingBasicDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CPrintingBasicApp, CWinApp)
END_MESSAGE_MAP()

CPrintingBasicApp::CPrintingBasicApp() {}
CPrintingBasicApp theApp;

BOOL CPrintingBasicApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CPrintingBasicDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
