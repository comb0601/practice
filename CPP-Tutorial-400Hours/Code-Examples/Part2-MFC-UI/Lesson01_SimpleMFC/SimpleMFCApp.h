// SimpleMFCApp.h : main header file for the PROJECT_NAME application
//
// WHERE TO LOOK:
// 1. CSimpleMFCApp class - Derived from CWinApp (the application class)
// 2. InitInstance() - Application initialization, creates and shows main dialog
// 3. theApp - Global application object (REQUIRED for MFC apps)

#pragma once

#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file for PCH"
#endif

#include "Resource.h"        // main symbols


// CSimpleMFCApp:
// See SimpleMFCApp.cpp for the implementation of this class
//

// ARCHITECTURE NOTE:
// Every MFC application has ONE CWinApp-derived object
// This object represents the application itself
// The application object controls:
//   - Application initialization
//   - Message loop
//   - Application termination

class CSimpleMFCApp : public CWinApp
{
public:
    CSimpleMFCApp();

// Overrides
public:
    // InitInstance is called once when the application starts
    // This is where you create your main window/dialog
    virtual BOOL InitInstance();

// Implementation

    DECLARE_MESSAGE_MAP()
};

// Global application object
// Every MFC app MUST have exactly one CWinApp-derived object
// The framework uses this object to access application-level functions
extern CSimpleMFCApp theApp;
