// TextEditorDlg.h
#pragma once

class CTextEditorDlg : public CDialogEx
{
public:
    CTextEditorDlg(CWnd* pParent = nullptr);
    enum { IDD = IDD_TEXTEDITOR_DIALOG };

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
