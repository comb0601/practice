// TabControlApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CTabControlApp : public CWinApp
{
public:
    CTabControlApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CTabControlApp theApp;
