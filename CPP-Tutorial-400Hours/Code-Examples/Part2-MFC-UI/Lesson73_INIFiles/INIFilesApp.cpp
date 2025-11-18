// INIFilesApp.cpp
#include "StdAfx.h"
#include "INIFilesApp.h"
#include "INIFilesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CINIFilesApp, CWinApp)
END_MESSAGE_MAP()

CINIFilesApp::CINIFilesApp() {}
CINIFilesApp theApp;

BOOL CINIFilesApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CINIFilesDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
