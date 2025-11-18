// OptimizationApp.cpp
#include "StdAfx.h"
#include "OptimizationApp.h"
#include "OptimizationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(COptimizationApp, CWinApp)
END_MESSAGE_MAP()

COptimizationApp::COptimizationApp() {}
COptimizationApp theApp;

BOOL COptimizationApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    COptimizationDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
