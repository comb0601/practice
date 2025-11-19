// MessageBoxApp.h - Demonstrates all MessageBox types in MFC

#pragma once

#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file for PCH"
#endif

#include "Resource.h"

class CMessageBoxApp : public CWinApp
{
public:
    CMessageBoxApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CMessageBoxApp theApp;
