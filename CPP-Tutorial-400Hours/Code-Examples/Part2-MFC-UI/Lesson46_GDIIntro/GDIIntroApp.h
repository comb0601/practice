// GDIIntroApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CGDIIntroApp : public CWinApp
{
public:
    CGDIIntroApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CGDIIntroApp theApp;
