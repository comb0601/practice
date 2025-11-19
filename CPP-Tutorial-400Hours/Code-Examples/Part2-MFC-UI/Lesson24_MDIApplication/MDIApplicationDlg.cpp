// MDIApplicationDlg.cpp
#include "StdAfx.h"
#include "MDIApplicationApp.h"
#include "MDIApplicationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMDIApplicationDlg::CMDIApplicationDlg(CWnd* pParent)
    : CDialogEx(IDD_MDIAPPLICATION_DIALOG, pParent)
    , m_strOutput(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMDIApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_OUTPUT, m_strOutput);
}

BEGIN_MESSAGE_MAP(CMDIApplicationDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_TEST, &CMDIApplicationDlg::OnButtonTest)
END_MESSAGE_MAP()

BOOL CMDIApplicationDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    m_strOutput = _T("Lesson 24: MDI Application\r\n\r\n");
    m_strOutput += _T("Features:\r\n- Multiple documents\r\n\r\n");
    m_strOutput += _T("Click Test button for demo.\r\n");

    UpdateData(FALSE);
    return TRUE;
}

void CMDIApplicationDlg::OnPaint()
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

HCURSOR CMDIApplicationDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CMDIApplicationDlg::OnButtonTest()
{
    m_strOutput = _T("=== MDI Application Demo ===\r\n\r\n");
    m_strOutput += _T("Demonstrating:\r\n- Multiple documents\r\n\r\n");
    m_strOutput += _T("Implementation: See source code\r\n");
    m_strOutput += _T("for detailed feature examples.\r\n");
    UpdateData(FALSE);
}
