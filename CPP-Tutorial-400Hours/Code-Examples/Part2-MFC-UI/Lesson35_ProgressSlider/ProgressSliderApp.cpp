// ProgressSliderApp.cpp
#include "StdAfx.h"
#include "ProgressSliderApp.h"
#include "ProgressSliderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CProgressSliderApp, CWinApp)
END_MESSAGE_MAP()

CProgressSliderApp::CProgressSliderApp() {}
CProgressSliderApp theApp;

BOOL CProgressSliderApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CProgressSliderDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
