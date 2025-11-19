// DateTimePickerApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CDateTimePickerApp : public CWinApp
{
public:
    CDateTimePickerApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CDateTimePickerApp theApp;
