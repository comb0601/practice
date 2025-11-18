// ReportGeneratorApp.cpp
#include "StdAfx.h"
#include "ReportGeneratorApp.h"
#include "ReportGeneratorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CReportGeneratorApp, CWinApp)
END_MESSAGE_MAP()

CReportGeneratorApp::CReportGeneratorApp() {}
CReportGeneratorApp theApp;

BOOL CReportGeneratorApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CReportGeneratorDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
