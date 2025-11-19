// ListControlReportApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CListControlReportApp : public CWinApp
{
public:
    CListControlReportApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CListControlReportApp theApp;
