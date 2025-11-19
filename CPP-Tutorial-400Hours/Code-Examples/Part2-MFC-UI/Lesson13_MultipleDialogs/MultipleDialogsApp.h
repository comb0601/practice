// MultipleDialogsApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CMultipleDialogsApp : public CWinApp
{
public:
    CMultipleDialogsApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CMultipleDialogsApp theApp;
