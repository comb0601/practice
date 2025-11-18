// GameEngineApp.cpp
#include "StdAfx.h"
#include "GameEngineApp.h"
#include "GameEngineDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CGameEngineApp, CWinApp)
END_MESSAGE_MAP()

CGameEngineApp::CGameEngineApp() {}
CGameEngineApp theApp;

BOOL CGameEngineApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    SetRegistryKey(_T("MFC Tutorial"));

    CGameEngineDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}
