// MultipleViewsApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CMultipleViewsApp : public CWinApp
{
public:
    CMultipleViewsApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CMultipleViewsApp theApp;
