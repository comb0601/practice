// DynamicControlsApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CDynamicControlsApp : public CWinApp
{
public:
    CDynamicControlsApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CDynamicControlsApp theApp;
