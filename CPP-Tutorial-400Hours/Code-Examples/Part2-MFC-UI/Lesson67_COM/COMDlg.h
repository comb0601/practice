// COMDlg.h
#pragma once

class CCOMDlg : public CDialogEx
{
public:
    CCOMDlg(CWnd* pParent = nullptr);
    enum { IDD = IDD_COM_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

private:
    HICON m_hIcon;
    CString m_strOutput;

public:
    afx_msg void OnButtonTest();
};
