// MessageMapsApp.cpp
#include "StdAfx.h"
#include "MessageMapsApp.h"
#include "MessageMapsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMessageMapsApp, CWinApp)
END_MESSAGE_MAP()

CMessageMapsApp::CMessageMapsApp() {}
CMessageMapsApp theApp;

BOOL CMessageMapsApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CMessageMapsDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
