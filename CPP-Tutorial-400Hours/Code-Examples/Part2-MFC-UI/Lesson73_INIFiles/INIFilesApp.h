// INIFilesApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CINIFilesApp : public CWinApp
{
public:
    CINIFilesApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CINIFilesApp theApp;
