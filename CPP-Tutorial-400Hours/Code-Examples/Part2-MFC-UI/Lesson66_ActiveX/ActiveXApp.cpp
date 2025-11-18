// ActiveXApp.cpp
#include "StdAfx.h"
#include "ActiveXApp.h"
#include "ActiveXDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CActiveXApp, CWinApp)
END_MESSAGE_MAP()

CActiveXApp::CActiveXApp() {}
CActiveXApp theApp;

BOOL CActiveXApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CActiveXDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
