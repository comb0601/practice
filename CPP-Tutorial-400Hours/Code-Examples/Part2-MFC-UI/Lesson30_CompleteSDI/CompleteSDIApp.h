// CompleteSDIApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CCompleteSDIApp : public CWinApp
{
public:
    CCompleteSDIApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CCompleteSDIApp theApp;
