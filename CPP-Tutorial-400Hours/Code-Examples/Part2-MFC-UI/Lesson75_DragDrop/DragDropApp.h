// DragDropApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CDragDropApp : public CWinApp
{
public:
    CDragDropApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CDragDropApp theApp;
