// ColorsApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CColorsApp : public CWinApp
{
public:
    CColorsApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CColorsApp theApp;
