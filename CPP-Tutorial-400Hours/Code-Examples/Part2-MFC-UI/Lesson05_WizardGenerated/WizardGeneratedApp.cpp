// WizardGeneratedApp.cpp
#include "StdAfx.h"
#include "WizardGeneratedApp.h"
#include "WizardGeneratedDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CWizardGeneratedApp, CWinApp)
END_MESSAGE_MAP()

CWizardGeneratedApp::CWizardGeneratedApp() {}
CWizardGeneratedApp theApp;

BOOL CWizardGeneratedApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CWizardGeneratedDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
