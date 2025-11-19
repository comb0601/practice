// MultipleDialogsApp.cpp
#include "StdAfx.h"
#include "MultipleDialogsApp.h"
#include "MultipleDialogsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMultipleDialogsApp, CWinApp)
END_MESSAGE_MAP()

CMultipleDialogsApp::CMultipleDialogsApp() {}
CMultipleDialogsApp theApp;

BOOL CMultipleDialogsApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CMultipleDialogsDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
