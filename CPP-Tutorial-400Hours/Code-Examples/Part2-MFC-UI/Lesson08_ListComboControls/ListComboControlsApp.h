// ListComboControlsApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CListComboControlsApp : public CWinApp
{
public:
    CListComboControlsApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CListComboControlsApp theApp;
