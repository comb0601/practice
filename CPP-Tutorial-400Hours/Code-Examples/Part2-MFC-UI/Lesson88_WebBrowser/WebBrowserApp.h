// WebBrowserApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CWebBrowserApp : public CWinApp
{
public:
    CWebBrowserApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CWebBrowserApp theApp;
