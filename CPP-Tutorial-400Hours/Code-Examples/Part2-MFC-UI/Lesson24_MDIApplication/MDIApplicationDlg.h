// MDIApplicationDlg.h
#pragma once

class CMDIApplicationDlg : public CDialogEx
{
public:
    CMDIApplicationDlg(CWnd* pParent = nullptr);
    enum { IDD = IDD_MDIAPPLICATION_DIALOG };

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
