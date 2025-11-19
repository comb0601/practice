// DeviceContextsApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CDeviceContextsApp : public CWinApp
{
public:
    CDeviceContextsApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CDeviceContextsApp theApp;
