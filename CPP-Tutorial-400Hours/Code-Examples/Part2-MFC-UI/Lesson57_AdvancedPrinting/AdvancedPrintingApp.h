// AdvancedPrintingApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CAdvancedPrintingApp : public CWinApp
{
public:
    CAdvancedPrintingApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CAdvancedPrintingApp theApp;
