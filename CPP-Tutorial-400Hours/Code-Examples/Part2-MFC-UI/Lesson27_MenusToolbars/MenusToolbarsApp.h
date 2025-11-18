// MenusToolbarsApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CMenusToolbarsApp : public CWinApp
{
public:
    CMenusToolbarsApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CMenusToolbarsApp theApp;
