// ClipboardApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CClipboardApp : public CWinApp
{
public:
    CClipboardApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CClipboardApp theApp;
