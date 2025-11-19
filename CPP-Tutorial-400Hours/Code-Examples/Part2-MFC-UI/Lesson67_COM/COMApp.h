// COMApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CCOMApp : public CWinApp
{
public:
    CCOMApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CCOMApp theApp;
