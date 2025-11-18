// MediaPlayerApp.cpp
#include "StdAfx.h"
#include "MediaPlayerApp.h"
#include "MediaPlayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMediaPlayerApp, CWinApp)
END_MESSAGE_MAP()

CMediaPlayerApp::CMediaPlayerApp() {}
CMediaPlayerApp theApp;

BOOL CMediaPlayerApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CMediaPlayerDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
