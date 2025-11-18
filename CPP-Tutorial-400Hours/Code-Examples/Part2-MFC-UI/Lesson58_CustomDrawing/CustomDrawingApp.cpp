// CustomDrawingApp.cpp
#include "StdAfx.h"
#include "CustomDrawingApp.h"
#include "CustomDrawingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CCustomDrawingApp, CWinApp)
END_MESSAGE_MAP()

CCustomDrawingApp::CCustomDrawingApp() {}
CCustomDrawingApp theApp;

BOOL CCustomDrawingApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CCustomDrawingDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
