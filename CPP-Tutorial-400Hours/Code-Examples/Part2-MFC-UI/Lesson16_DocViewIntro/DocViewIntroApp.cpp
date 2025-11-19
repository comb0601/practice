// DocViewIntroApp.cpp
#include "StdAfx.h"
#include "DocViewIntroApp.h"
#include "DocViewIntroDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CDocViewIntroApp, CWinApp)
END_MESSAGE_MAP()

CDocViewIntroApp::CDocViewIntroApp() {}
CDocViewIntroApp theApp;

BOOL CDocViewIntroApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CDocViewIntroDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
