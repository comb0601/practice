// DebuggingDemoApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CDebuggingDemoApp : public CWinApp
{
public:
    CDebuggingDemoApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CDebuggingDemoApp theApp;
