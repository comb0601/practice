// MultipleViewsApp.cpp
#include "StdAfx.h"
#include "MultipleViewsApp.h"
#include "MultipleViewsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMultipleViewsApp, CWinApp)
END_MESSAGE_MAP()

CMultipleViewsApp::CMultipleViewsApp() {}
CMultipleViewsApp theApp;

BOOL CMultipleViewsApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CMultipleViewsDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
