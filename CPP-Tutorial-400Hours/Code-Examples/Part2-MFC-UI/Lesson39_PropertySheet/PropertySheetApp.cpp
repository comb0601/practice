// PropertySheetApp.cpp
#include "StdAfx.h"
#include "PropertySheetApp.h"
#include "PropertySheetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CPropertySheetApp, CWinApp)
END_MESSAGE_MAP()

CPropertySheetApp::CPropertySheetApp() {}
CPropertySheetApp theApp;

BOOL CPropertySheetApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CPropertySheetDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
