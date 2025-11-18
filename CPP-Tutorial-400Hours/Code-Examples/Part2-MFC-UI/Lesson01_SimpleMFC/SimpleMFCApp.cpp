// SimpleMFCApp.cpp : Defines the class behaviors for the application.
//
// EXECUTION FLOW:
// 1. Windows calls WinMain (provided by MFC)
// 2. MFC's WinMain creates theApp object
// 3. WinMain calls theApp.InitInstance()
// 4. InitInstance creates and displays the main dialog
// 5. DoModal() runs the message loop
// 6. When dialog closes, InitInstance returns FALSE (app exits)

#include "StdAfx.h"
#include "SimpleMFCApp.h"
#include "SimpleMFCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSimpleMFCApp

BEGIN_MESSAGE_MAP(CSimpleMFCApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSimpleMFCApp construction

CSimpleMFCApp::CSimpleMFCApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


// The one and only CSimpleMFCApp object
// CRITICAL: This global object must exist
// MFC framework uses this to initialize the application

CSimpleMFCApp theApp;


// CSimpleMFCApp initialization

BOOL CSimpleMFCApp::InitInstance()
{
    // ARCHITECTURE NOTE:
    // InitInstance is the entry point for your application code
    // Return TRUE to continue running, FALSE to exit immediately

    // InitCommonControlsEx() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // Set this to include all the common control classes you want to use
    // in your application.
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    // of your final executable, you should remove from the following
    // the specific initialization routines you do not need
    // Change the registry key under which our settings are stored
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization
    SetRegistryKey(_T("Local AppWizard-Generated Applications"));

    // Create the main dialog
    // For dialog-based apps, this is typically a CDialog-derived class
    CSimpleMFCDlg dlg;
    m_pMainWnd = &dlg;

    // DoModal() displays the dialog and runs its message loop
    // It returns when the dialog is closed (IDOK or IDCANCEL)
    INT_PTR nResponse = dlg.DoModal();

    if (nResponse == IDOK)
    {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with OK
    }
    else if (nResponse == IDCANCEL)
    {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with Cancel
    }

    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    // For dialog-based apps, we return FALSE to exit
    // For SDI/MDI apps with CFrameWnd, we return TRUE to keep running
    return FALSE;
}
