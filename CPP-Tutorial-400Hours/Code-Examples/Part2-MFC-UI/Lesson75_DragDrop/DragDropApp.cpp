// DragDropApp.cpp
#include "StdAfx.h"
#include "DragDropApp.h"
#include "DragDropDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CDragDropApp, CWinApp)
END_MESSAGE_MAP()

CDragDropApp::CDragDropApp() {}
CDragDropApp theApp;

BOOL CDragDropApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CDragDropDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
