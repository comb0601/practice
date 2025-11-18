// MessageMapsApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CMessageMapsApp : public CWinApp
{
public:
    CMessageMapsApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CMessageMapsApp theApp;
