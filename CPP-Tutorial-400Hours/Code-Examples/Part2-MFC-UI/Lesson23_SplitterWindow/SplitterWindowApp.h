// SplitterWindowApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CSplitterWindowApp : public CWinApp
{
public:
    CSplitterWindowApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CSplitterWindowApp theApp;
