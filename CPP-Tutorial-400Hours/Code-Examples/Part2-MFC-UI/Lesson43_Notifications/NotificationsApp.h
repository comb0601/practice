// NotificationsApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CNotificationsApp : public CWinApp
{
public:
    CNotificationsApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CNotificationsApp theApp;
