// StatusBarApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CStatusBarApp : public CWinApp
{
public:
    CStatusBarApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CStatusBarApp theApp;
