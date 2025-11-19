// OwnerDrawApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class COwnerDrawApp : public CWinApp
{
public:
    COwnerDrawApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern COwnerDrawApp theApp;
