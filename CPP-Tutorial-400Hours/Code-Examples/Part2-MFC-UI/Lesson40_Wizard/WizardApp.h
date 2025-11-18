// WizardApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CWizardApp : public CWinApp
{
public:
    CWizardApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CWizardApp theApp;
