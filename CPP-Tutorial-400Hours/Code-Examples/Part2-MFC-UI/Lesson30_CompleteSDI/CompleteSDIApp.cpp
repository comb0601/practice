// CompleteSDIApp.cpp
#include "StdAfx.h"
#include "CompleteSDIApp.h"
#include "CompleteSDIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CCompleteSDIApp, CWinApp)
END_MESSAGE_MAP()

CCompleteSDIApp::CCompleteSDIApp() {}
CCompleteSDIApp theApp;

BOOL CCompleteSDIApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CCompleteSDIDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
