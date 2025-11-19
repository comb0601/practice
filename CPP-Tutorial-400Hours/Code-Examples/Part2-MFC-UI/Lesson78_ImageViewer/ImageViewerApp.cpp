// ImageViewerApp.cpp
#include "StdAfx.h"
#include "ImageViewerApp.h"
#include "ImageViewerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CImageViewerApp, CWinApp)
END_MESSAGE_MAP()

CImageViewerApp::CImageViewerApp() {}
CImageViewerApp theApp;

BOOL CImageViewerApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CImageViewerDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
