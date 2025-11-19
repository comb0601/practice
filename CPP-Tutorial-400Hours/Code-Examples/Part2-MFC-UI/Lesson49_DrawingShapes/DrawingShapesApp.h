// DrawingShapesApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CDrawingShapesApp : public CWinApp
{
public:
    CDrawingShapesApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CDrawingShapesApp theApp;
