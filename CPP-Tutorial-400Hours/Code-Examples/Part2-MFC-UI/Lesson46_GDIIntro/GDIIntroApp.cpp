// GDIIntroApp.cpp
#include "StdAfx.h"
#include "GDIIntroApp.h"
#include "GDIIntroDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CGDIIntroApp, CWinApp)
END_MESSAGE_MAP()

CGDIIntroApp::CGDIIntroApp() {}
CGDIIntroApp theApp;

BOOL CGDIIntroApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CGDIIntroDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
