// PluginSystemApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CPluginSystemApp : public CWinApp
{
public:
    CPluginSystemApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CPluginSystemApp theApp;
