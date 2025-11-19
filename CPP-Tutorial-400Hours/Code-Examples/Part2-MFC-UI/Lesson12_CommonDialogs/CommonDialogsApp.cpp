// CommonDialogsApp.cpp
#include "StdAfx.h"
#include "CommonDialogsApp.h"
#include "CommonDialogsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CCommonDialogsApp, CWinApp)
END_MESSAGE_MAP()

CCommonDialogsApp::CCommonDialogsApp() {}
CCommonDialogsApp theApp;

BOOL CCommonDialogsApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CCommonDialogsDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
