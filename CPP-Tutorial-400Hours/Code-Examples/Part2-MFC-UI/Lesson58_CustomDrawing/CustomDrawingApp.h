// CustomDrawingApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CCustomDrawingApp : public CWinApp
{
public:
    CCustomDrawingApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CCustomDrawingApp theApp;
