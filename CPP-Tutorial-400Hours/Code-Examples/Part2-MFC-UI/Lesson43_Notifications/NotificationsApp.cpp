// NotificationsApp.cpp
#include "StdAfx.h"
#include "NotificationsApp.h"
#include "NotificationsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CNotificationsApp, CWinApp)
END_MESSAGE_MAP()

CNotificationsApp::CNotificationsApp() {}
CNotificationsApp theApp;

BOOL CNotificationsApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CNotificationsDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
