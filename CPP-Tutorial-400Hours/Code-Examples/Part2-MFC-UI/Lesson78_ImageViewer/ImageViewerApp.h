// ImageViewerApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CImageViewerApp : public CWinApp
{
public:
    CImageViewerApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CImageViewerApp theApp;
