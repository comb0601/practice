// PropertySheetApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CPropertySheetApp : public CWinApp
{
public:
    CPropertySheetApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CPropertySheetApp theApp;
