// DoubleBufferingApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CDoubleBufferingApp : public CWinApp
{
public:
    CDoubleBufferingApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CDoubleBufferingApp theApp;
