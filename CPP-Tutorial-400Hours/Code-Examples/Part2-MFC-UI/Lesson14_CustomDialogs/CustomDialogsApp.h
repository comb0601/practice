// CustomDialogsApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CCustomDialogsApp : public CWinApp
{
public:
    CCustomDialogsApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CCustomDialogsApp theApp;
