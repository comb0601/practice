// DrawingShapesApp.cpp
#include "StdAfx.h"
#include "DrawingShapesApp.h"
#include "DrawingShapesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CDrawingShapesApp, CWinApp)
END_MESSAGE_MAP()

CDrawingShapesApp::CDrawingShapesApp() {}
CDrawingShapesApp theApp;

BOOL CDrawingShapesApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CDrawingShapesDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
