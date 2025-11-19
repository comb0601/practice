// BitmapsApp.cpp
#include "StdAfx.h"
#include "BitmapsApp.h"
#include "BitmapsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CBitmapsApp, CWinApp)
END_MESSAGE_MAP()

CBitmapsApp::CBitmapsApp() {}
CBitmapsApp theApp;

BOOL CBitmapsApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CBitmapsDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
