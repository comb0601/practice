// MouseDrawingApp.cpp
#include "StdAfx.h"
#include "MouseDrawingApp.h"
#include "MouseDrawingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMouseDrawingApp, CWinApp)
END_MESSAGE_MAP()

CMouseDrawingApp::CMouseDrawingApp() {}
CMouseDrawingApp theApp;

BOOL CMouseDrawingApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CMouseDrawingDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
