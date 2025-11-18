// TextDrawingApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CTextDrawingApp : public CWinApp
{
public:
    CTextDrawingApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CTextDrawingApp theApp;
