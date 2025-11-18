// DocumentDemoApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CDocumentDemoApp : public CWinApp
{
public:
    CDocumentDemoApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CDocumentDemoApp theApp;
