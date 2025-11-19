// DrawingAppApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CDrawingAppApp : public CWinApp
{
public:
    CDrawingAppApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CDrawingAppApp theApp;
