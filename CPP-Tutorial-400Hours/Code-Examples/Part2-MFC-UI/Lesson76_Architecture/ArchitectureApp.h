// ArchitectureApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CArchitectureApp : public CWinApp
{
public:
    CArchitectureApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CArchitectureApp theApp;
