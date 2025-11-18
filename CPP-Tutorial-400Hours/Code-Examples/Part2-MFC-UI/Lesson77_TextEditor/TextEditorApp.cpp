// TextEditorApp.cpp
#include "StdAfx.h"
#include "TextEditorApp.h"
#include "TextEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CTextEditorApp, CWinApp)
END_MESSAGE_MAP()

CTextEditorApp::CTextEditorApp() {}
CTextEditorApp theApp;

BOOL CTextEditorApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CTextEditorDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
