// SDIApplicationApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CSDIApplicationApp : public CWinApp
{
public:
    CSDIApplicationApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CSDIApplicationApp theApp;
