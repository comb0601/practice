// TreeControlApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CTreeControlApp : public CWinApp
{
public:
    CTreeControlApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CTreeControlApp theApp;
