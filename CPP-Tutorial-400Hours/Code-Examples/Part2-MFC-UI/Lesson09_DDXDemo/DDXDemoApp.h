// DDXDemoApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CDDXDemoApp : public CWinApp
{
public:
    CDDXDemoApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CDDXDemoApp theApp;
