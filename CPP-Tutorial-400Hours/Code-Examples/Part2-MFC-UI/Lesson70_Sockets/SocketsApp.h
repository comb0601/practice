// SocketsApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CSocketsApp : public CWinApp
{
public:
    CSocketsApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CSocketsApp theApp;
