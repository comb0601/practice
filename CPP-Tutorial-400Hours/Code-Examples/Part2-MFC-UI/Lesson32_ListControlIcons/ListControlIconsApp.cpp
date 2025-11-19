// ListControlIconsApp.cpp
#include "StdAfx.h"
#include "ListControlIconsApp.h"
#include "ListControlIconsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CListControlIconsApp, CWinApp)
END_MESSAGE_MAP()

CListControlIconsApp::CListControlIconsApp() {}
CListControlIconsApp theApp;

BOOL CListControlIconsApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CListControlIconsDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
