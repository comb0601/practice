// SerializationApp.cpp
#include "StdAfx.h"
#include "SerializationApp.h"
#include "SerializationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CSerializationApp, CWinApp)
END_MESSAGE_MAP()

CSerializationApp::CSerializationApp() {}
CSerializationApp theApp;

BOOL CSerializationApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CSerializationDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
