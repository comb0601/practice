// CommonDialogsApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CCommonDialogsApp : public CWinApp
{
public:
    CCommonDialogsApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CCommonDialogsApp theApp;
