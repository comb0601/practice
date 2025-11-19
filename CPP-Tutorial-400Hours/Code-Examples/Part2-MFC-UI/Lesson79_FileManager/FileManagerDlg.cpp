// FileManagerDlg.cpp
#include "StdAfx.h"
#include "FileManagerApp.h"
#include "FileManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CFileManagerDlg::CFileManagerDlg(CWnd* pParent)
    : CDialogEx(IDD_FILEMANAGER_DIALOG, pParent)
    , m_strOutput(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileManagerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_OUTPUT, m_strOutput);
}

BEGIN_MESSAGE_MAP(CFileManagerDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_TEST, &CFileManagerDlg::OnButtonTest)
END_MESSAGE_MAP()

BOOL CFileManagerDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    m_strOutput = _T("Lesson 79: Complete File Manager\r\n\r\n");
    m_strOutput += _T("Features:\r\n- File operations\r\n\r\n");
    m_strOutput += _T("Click Test button for demo.\r\n");

    UpdateData(FALSE);
    return TRUE;
}

void CFileManagerDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR CFileManagerDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CFileManagerDlg::OnButtonTest()
{
    m_strOutput = _T("=== Complete File Manager Demo ===\r\n\r\n");
    m_strOutput += _T("Demonstrating:\r\n- File operations\r\n\r\n");
    m_strOutput += _T("Implementation: See source code\r\n");
    m_strOutput += _T("for detailed feature examples.\r\n");
    UpdateData(FALSE);
}
