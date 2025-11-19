// ExtensionDLLApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CExtensionDLLApp : public CWinApp
{
public:
    CExtensionDLLApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CExtensionDLLApp theApp;
