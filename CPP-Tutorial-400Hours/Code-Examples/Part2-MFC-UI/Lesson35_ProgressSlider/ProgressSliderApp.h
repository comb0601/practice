// ProgressSliderApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CProgressSliderApp : public CWinApp
{
public:
    CProgressSliderApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CProgressSliderApp theApp;
