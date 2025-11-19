// DockingToolbarsApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CDockingToolbarsApp : public CWinApp
{
public:
    CDockingToolbarsApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CDockingToolbarsApp theApp;
