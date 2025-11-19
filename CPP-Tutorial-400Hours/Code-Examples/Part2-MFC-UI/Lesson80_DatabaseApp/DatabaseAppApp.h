// DatabaseAppApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CDatabaseAppApp : public CWinApp
{
public:
    CDatabaseAppApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CDatabaseAppApp theApp;
