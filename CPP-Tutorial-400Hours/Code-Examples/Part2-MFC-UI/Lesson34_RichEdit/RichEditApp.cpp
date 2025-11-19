// RichEditApp.cpp
#include "StdAfx.h"
#include "RichEditApp.h"
#include "RichEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CRichEditApp, CWinApp)
END_MESSAGE_MAP()

CRichEditApp::CRichEditApp() {}
CRichEditApp theApp;

BOOL CRichEditApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CRichEditDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
