// ArchitectureApp.cpp
#include "StdAfx.h"
#include "ArchitectureApp.h"
#include "ArchitectureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CArchitectureApp, CWinApp)
END_MESSAGE_MAP()

CArchitectureApp::CArchitectureApp() {}
CArchitectureApp theApp;

BOOL CArchitectureApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CArchitectureDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
