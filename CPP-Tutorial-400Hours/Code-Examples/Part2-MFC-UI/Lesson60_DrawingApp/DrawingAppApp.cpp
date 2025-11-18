// DrawingAppApp.cpp
#include "StdAfx.h"
#include "DrawingAppApp.h"
#include "DrawingAppDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CDrawingAppApp, CWinApp)
END_MESSAGE_MAP()

CDrawingAppApp::CDrawingAppApp() {}
CDrawingAppApp theApp;

BOOL CDrawingAppApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CDrawingAppDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
