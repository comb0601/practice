// ThreadSyncApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CThreadSyncApp : public CWinApp
{
public:
    CThreadSyncApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CThreadSyncApp theApp;
