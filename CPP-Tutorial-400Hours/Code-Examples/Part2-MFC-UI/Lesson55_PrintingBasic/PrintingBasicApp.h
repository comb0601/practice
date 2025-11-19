// PrintingBasicApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CPrintingBasicApp : public CWinApp
{
public:
    CPrintingBasicApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CPrintingBasicApp theApp;
