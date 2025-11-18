// MouseDrawingApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CMouseDrawingApp : public CWinApp
{
public:
    CMouseDrawingApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CMouseDrawingApp theApp;
