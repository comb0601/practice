// TextEditorApp.h
#pragma once
#ifndef __AFXWIN_H__
    #error "include 'StdAfx.h' before including this file"
#endif
#include "Resource.h"

class CTextEditorApp : public CWinApp
{
public:
    CTextEditorApp();
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CTextEditorApp theApp;
