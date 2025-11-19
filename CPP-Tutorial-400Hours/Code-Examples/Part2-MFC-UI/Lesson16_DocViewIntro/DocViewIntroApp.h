// DocViewIntroApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CDocViewIntroApp : public CWinApp
{
public:
    CDocViewIntroApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CDocViewIntroApp theApp;
