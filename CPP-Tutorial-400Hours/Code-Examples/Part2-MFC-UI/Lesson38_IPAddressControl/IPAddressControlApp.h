// IPAddressControlApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CIPAddressControlApp : public CWinApp
{
public:
    CIPAddressControlApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CIPAddressControlApp theApp;
