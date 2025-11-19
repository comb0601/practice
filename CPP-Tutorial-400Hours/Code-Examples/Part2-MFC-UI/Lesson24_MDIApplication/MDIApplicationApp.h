// MDIApplicationApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CMDIApplicationApp : public CWinApp
{
public:
    CMDIApplicationApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CMDIApplicationApp theApp;
