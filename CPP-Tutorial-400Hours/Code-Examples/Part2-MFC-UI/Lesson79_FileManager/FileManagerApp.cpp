// FileManagerApp.cpp
#include "StdAfx.h"
#include "FileManagerApp.h"
#include "FileManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CFileManagerApp, CWinApp)
END_MESSAGE_MAP()

CFileManagerApp::CFileManagerApp() {}
CFileManagerApp theApp;

BOOL CFileManagerApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CFileManagerDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
