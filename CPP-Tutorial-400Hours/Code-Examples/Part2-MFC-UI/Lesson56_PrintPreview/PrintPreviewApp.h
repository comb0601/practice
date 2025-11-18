// PrintPreviewApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CPrintPreviewApp : public CWinApp
{
public:
    CPrintPreviewApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CPrintPreviewApp theApp;
