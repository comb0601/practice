// OwnerDrawApp.cpp
#include "StdAfx.h"
#include "OwnerDrawApp.h"
#include "OwnerDrawDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(COwnerDrawApp, CWinApp)
END_MESSAGE_MAP()

COwnerDrawApp::COwnerDrawApp() {}
COwnerDrawApp theApp;

BOOL COwnerDrawApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    COwnerDrawDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
