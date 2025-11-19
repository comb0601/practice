// DeviceContextsApp.cpp
#include "StdAfx.h"
#include "DeviceContextsApp.h"
#include "DeviceContextsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CDeviceContextsApp, CWinApp)
END_MESSAGE_MAP()

CDeviceContextsApp::CDeviceContextsApp() {}
CDeviceContextsApp theApp;

BOOL CDeviceContextsApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CDeviceContextsDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
