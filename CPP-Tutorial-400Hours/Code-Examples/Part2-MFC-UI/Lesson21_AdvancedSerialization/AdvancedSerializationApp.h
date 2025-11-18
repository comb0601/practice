// AdvancedSerializationApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CAdvancedSerializationApp : public CWinApp
{
public:
    CAdvancedSerializationApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CAdvancedSerializationApp theApp;
