// DrawingAppDlg.h
#pragma once

class CDrawingAppDlg : public CDialogEx
{
public:
    CDrawingAppDlg(CWnd* pParent = nullptr);
    enum { IDD = IDD_DRAWINGAPP_DIALOG };

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
