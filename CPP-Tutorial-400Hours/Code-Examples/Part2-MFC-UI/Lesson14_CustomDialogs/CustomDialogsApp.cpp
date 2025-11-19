// CustomDialogsApp.cpp
#include "StdAfx.h"
#include "CustomDialogsApp.h"
#include "CustomDialogsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CCustomDialogsApp, CWinApp)
END_MESSAGE_MAP()

CCustomDialogsApp::CCustomDialogsApp() {}
CCustomDialogsApp theApp;

BOOL CCustomDialogsApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CCustomDialogsDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
