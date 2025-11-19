// RichEditApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CRichEditApp : public CWinApp
{
public:
    CRichEditApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CRichEditApp theApp;
