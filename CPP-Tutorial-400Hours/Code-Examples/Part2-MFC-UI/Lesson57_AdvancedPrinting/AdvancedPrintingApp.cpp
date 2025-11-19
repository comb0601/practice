// AdvancedPrintingApp.cpp
#include "StdAfx.h"
#include "AdvancedPrintingApp.h"
#include "AdvancedPrintingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CAdvancedPrintingApp, CWinApp)
END_MESSAGE_MAP()

CAdvancedPrintingApp::CAdvancedPrintingApp() {}
CAdvancedPrintingApp theApp;

BOOL CAdvancedPrintingApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CAdvancedPrintingDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
