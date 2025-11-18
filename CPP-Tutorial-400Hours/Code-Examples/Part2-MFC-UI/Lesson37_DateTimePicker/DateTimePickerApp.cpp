// DateTimePickerApp.cpp
#include "StdAfx.h"
#include "DateTimePickerApp.h"
#include "DateTimePickerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CDateTimePickerApp, CWinApp)
END_MESSAGE_MAP()

CDateTimePickerApp::CDateTimePickerApp() {}
CDateTimePickerApp theApp;

BOOL CDateTimePickerApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CDateTimePickerDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
