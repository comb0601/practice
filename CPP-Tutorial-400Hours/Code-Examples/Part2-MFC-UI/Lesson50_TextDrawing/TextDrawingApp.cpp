// TextDrawingApp.cpp
#include "StdAfx.h"
#include "TextDrawingApp.h"
#include "TextDrawingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CTextDrawingApp, CWinApp)
END_MESSAGE_MAP()

CTextDrawingApp::CTextDrawingApp() {}
CTextDrawingApp theApp;

BOOL CTextDrawingApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CTextDrawingDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
