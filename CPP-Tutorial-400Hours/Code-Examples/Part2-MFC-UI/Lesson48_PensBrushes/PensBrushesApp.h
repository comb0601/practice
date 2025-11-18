// PensBrushesApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CPensBrushesApp : public CWinApp
{
public:
    CPensBrushesApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CPensBrushesApp theApp;
