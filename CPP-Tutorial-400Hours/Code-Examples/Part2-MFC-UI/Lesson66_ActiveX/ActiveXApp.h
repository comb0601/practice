// ActiveXApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CActiveXApp : public CWinApp
{
public:
    CActiveXApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CActiveXApp theApp;
