// CoordinatesApp.cpp
#include "StdAfx.h"
#include "CoordinatesApp.h"
#include "CoordinatesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CCoordinatesApp, CWinApp)
END_MESSAGE_MAP()

CCoordinatesApp::CCoordinatesApp() {}
CCoordinatesApp theApp;

BOOL CCoordinatesApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CCoordinatesDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
