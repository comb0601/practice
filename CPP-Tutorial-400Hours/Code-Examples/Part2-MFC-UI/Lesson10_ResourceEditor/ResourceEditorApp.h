// ResourceEditorApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CResourceEditorApp : public CWinApp
{
public:
    CResourceEditorApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CResourceEditorApp theApp;
