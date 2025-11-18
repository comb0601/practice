// ListControlIconsApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CListControlIconsApp : public CWinApp
{
public:
    CListControlIconsApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CListControlIconsApp theApp;
