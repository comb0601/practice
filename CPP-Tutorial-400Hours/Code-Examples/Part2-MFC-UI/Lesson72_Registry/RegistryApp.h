// RegistryApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CRegistryApp : public CWinApp
{
public:
    CRegistryApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CRegistryApp theApp;
