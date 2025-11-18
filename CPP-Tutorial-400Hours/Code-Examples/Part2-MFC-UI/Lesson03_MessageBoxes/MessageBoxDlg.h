// MessageBoxDlg.h - Dialog demonstrating various MessageBox types
//
// WHERE TO LOOK:
// 1. Handler functions for each button (OnBtnXXX)
// 2. MessageBox() calls with different parameters
// 3. Return value handling (IDOK, IDCANCEL, IDYES, etc.)

#pragma once

class CMessageBoxDlg : public CDialogEx
{
public:
    CMessageBoxDlg(CWnd* pParent = nullptr);

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MESSAGEBOX_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

private:
    HICON m_hIcon;
    CString m_strResult;  // Display results from MessageBox

public:
    // Message box demonstrations
    afx_msg void OnBtnInfo();           // MB_ICONINFORMATION
    afx_msg void OnBtnWarning();        // MB_ICONWARNING
    afx_msg void OnBtnError();          // MB_ICONERROR
    afx_msg void OnBtnQuestion();       // MB_ICONQUESTION
    afx_msg void OnBtnOkCancel();       // MB_OKCANCEL
    afx_msg void OnBtnYesNo();          // MB_YESNO
    afx_msg void OnBtnYesNoCancel();    // MB_YESNOCANCEL
    afx_msg void OnBtnAbortRetryIgnore();// MB_ABORTRETRYIGNORE
    afx_msg void OnBtnRetryCancel();    // MB_RETRYCANCEL
    afx_msg void OnBtnCustom();         // Custom formatted message
    afx_msg void OnBtnSystem();         // System modality
};
