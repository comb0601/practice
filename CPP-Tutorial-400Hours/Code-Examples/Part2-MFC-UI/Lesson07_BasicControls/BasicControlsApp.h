// BasicControlsApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CBasicControlsApp : public CWinApp
{
public:
    CBasicControlsApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CBasicControlsApp theApp;
