// PensBrushesApp.cpp
#include "StdAfx.h"
#include "PensBrushesApp.h"
#include "PensBrushesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CPensBrushesApp, CWinApp)
END_MESSAGE_MAP()

CPensBrushesApp::CPensBrushesApp() {}
CPensBrushesApp theApp;

BOOL CPensBrushesApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CPensBrushesDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
