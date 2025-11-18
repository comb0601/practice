// AllControlsApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CAllControlsApp : public CWinApp
{
public:
    CAllControlsApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CAllControlsApp theApp;
