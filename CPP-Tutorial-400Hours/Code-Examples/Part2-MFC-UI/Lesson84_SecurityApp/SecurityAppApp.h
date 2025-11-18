// SecurityAppApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CSecurityAppApp : public CWinApp
{
public:
    CSecurityAppApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CSecurityAppApp theApp;
