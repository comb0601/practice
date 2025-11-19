// ReportGeneratorApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CReportGeneratorApp : public CWinApp
{
public:
    CReportGeneratorApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CReportGeneratorApp theApp;
