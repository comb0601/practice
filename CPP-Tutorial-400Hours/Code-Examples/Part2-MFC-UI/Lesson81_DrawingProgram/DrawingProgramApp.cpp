// DrawingProgramApp.cpp
#include "StdAfx.h"
#include "DrawingProgramApp.h"
#include "DrawingProgramDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CDrawingProgramApp, CWinApp)
END_MESSAGE_MAP()

CDrawingProgramApp::CDrawingProgramApp() {}
CDrawingProgramApp theApp;

BOOL CDrawingProgramApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CDrawingProgramDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
