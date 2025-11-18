// SimpleMFCDlg.h : header file
//
// WHERE TO LOOK:
// 1. Class declaration - Derived from CDialogEx
// 2. DoDataExchange - DDX/DDV support for automatic data exchange
// 3. Message map - Links Windows messages to handler functions
// 4. OnButtonHello - Button click handler

#pragma once


// CSimpleMFCDlg dialog
// ARCHITECTURE NOTE:
// This class represents your main dialog window
// It handles all user interactions and displays the UI

class CSimpleMFCDlg : public CDialogEx
{
// Construction
public:
    CSimpleMFCDlg(CWnd* pParent = nullptr);    // standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_SIMPLEMFC_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

public:
    // Button click handler
    // This function is called when the user clicks the "Say Hello" button
    afx_msg void OnButtonHello();

    // Member variable linked to the edit control
    // DDX automatically transfers data between this variable and the control
    CString m_strOutput;
};
