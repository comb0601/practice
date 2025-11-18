// BitmapsApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CBitmapsApp : public CWinApp
{
public:
    CBitmapsApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CBitmapsApp theApp;
