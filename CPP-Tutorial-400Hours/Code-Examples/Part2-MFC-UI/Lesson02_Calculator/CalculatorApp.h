// CalculatorApp.h : main header file for the Calculator application
//
// WHERE TO LOOK:
// 1. Standard MFC application class structure
// 2. InitInstance() creates the calculator dialog

#pragma once

#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file for PCH"
#endif

#include "Resource.h"

// CCalculatorApp:
// Application class for the calculator

class CCalculatorApp : public CWinApp
{
public:
    CCalculatorApp();

// Overrides
public:
    virtual BOOL InitInstance();

// Implementation
    DECLARE_MESSAGE_MAP()
};

extern CCalculatorApp theApp;
