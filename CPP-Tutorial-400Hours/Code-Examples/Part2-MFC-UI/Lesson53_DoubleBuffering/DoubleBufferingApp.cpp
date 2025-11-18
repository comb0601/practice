// DoubleBufferingApp.cpp
#include "StdAfx.h"
#include "DoubleBufferingApp.h"
#include "DoubleBufferingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CDoubleBufferingApp, CWinApp)
END_MESSAGE_MAP()

CDoubleBufferingApp::CDoubleBufferingApp() {}
CDoubleBufferingApp theApp;

BOOL CDoubleBufferingApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CDoubleBufferingDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
