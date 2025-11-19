// ListComboControlsApp.cpp
#include "StdAfx.h"
#include "ListComboControlsApp.h"
#include "ListComboControlsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CListComboControlsApp, CWinApp)
END_MESSAGE_MAP()

CListComboControlsApp::CListComboControlsApp() {}
CListComboControlsApp theApp;

BOOL CListComboControlsApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CListComboControlsDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
