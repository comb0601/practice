// ResourceEditorApp.cpp
#include "StdAfx.h"
#include "ResourceEditorApp.h"
#include "ResourceEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CResourceEditorApp, CWinApp)
END_MESSAGE_MAP()

CResourceEditorApp::CResourceEditorApp() {}
CResourceEditorApp theApp;

BOOL CResourceEditorApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CResourceEditorDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
