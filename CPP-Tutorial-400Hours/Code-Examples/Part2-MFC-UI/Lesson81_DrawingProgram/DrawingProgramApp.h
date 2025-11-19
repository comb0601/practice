// DrawingProgramApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CDrawingProgramApp : public CWinApp
{
public:
    CDrawingProgramApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CDrawingProgramApp theApp;
