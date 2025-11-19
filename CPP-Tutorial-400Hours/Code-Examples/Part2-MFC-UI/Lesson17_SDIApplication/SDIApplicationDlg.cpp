// SDIApplicationDlg.cpp
#include "StdAfx.h"
#include "SDIApplicationApp.h"
#include "SDIApplicationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSDIApplicationDlg::CSDIApplicationDlg(CWnd* pParent)
    : CDialogEx(IDD_SDIAPPLICATION_DIALOG, pParent)
    , m_strOutput(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSDIApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_OUTPUT, m_strOutput);
}

BEGIN_MESSAGE_MAP(CSDIApplicationDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_TEST, &CSDIApplicationDlg::OnButtonTest)
END_MESSAGE_MAP()

BOOL CSDIApplicationDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    m_strOutput = _T("Lesson 17: SDI Application\r\n\r\n");
    m_strOutput += _T("Features:\r\n- Full SDI\r\n- File operations\r\n\r\n");
    m_strOutput += _T("Click Test button for demo.\r\n");

    UpdateData(FALSE);
    return TRUE;
}

void CSDIApplicationDlg::OnPaint()
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

HCURSOR CSDIApplicationDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CSDIApplicationDlg::OnButtonTest()
{
    m_strOutput = _T("=== SDI Application Demo ===\r\n\r\n");
    m_strOutput += _T("Demonstrating:\r\n- Full SDI\r\n- File operations\r\n\r\n");
    m_strOutput += _T("Implementation: See source code\r\n");
    m_strOutput += _T("for detailed feature examples.\r\n");
    UpdateData(FALSE);
}
