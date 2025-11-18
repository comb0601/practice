// CalculatorApp.cpp : Defines the class behaviors for the application.

#include "StdAfx.h"
#include "CalculatorApp.h"
#include "CalculatorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCalculatorApp

BEGIN_MESSAGE_MAP(CCalculatorApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// CCalculatorApp construction

CCalculatorApp::CCalculatorApp()
{
    // TODO: add construction code here
}

// The one and only CCalculatorApp object

CCalculatorApp theApp;

// CCalculatorApp initialization

BOOL CCalculatorApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();

    SetRegistryKey(_T("MFC Calculator Tutorial"));

    CCalculatorDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();

    // Since dialog is closed, return FALSE to exit
    return FALSE;
}
