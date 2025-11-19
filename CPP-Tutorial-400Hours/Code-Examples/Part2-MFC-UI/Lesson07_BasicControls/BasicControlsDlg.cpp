// BasicControlsDlg.cpp
#include "StdAfx.h"
#include "BasicControlsApp.h"
#include "BasicControlsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBasicControlsDlg::CBasicControlsDlg(CWnd* pParent)
    : CDialogEx(IDD_BASICCONTROLS_DIALOG, pParent)
    , m_strOutput(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBasicControlsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_OUTPUT, m_strOutput);
}

BEGIN_MESSAGE_MAP(CBasicControlsDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_TEST, &CBasicControlsDlg::OnButtonTest)
END_MESSAGE_MAP()

BOOL CBasicControlsDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    m_strOutput = _T("Lesson 07: Basic Controls\r\n\r\n");
    m_strOutput += _T("Features:\r\n- CButton\r\n- CEdit\r\n- CStatic\r\n\r\n");
    m_strOutput += _T("Click Test button for demo.\r\n");

    UpdateData(FALSE);
    return TRUE;
}

void CBasicControlsDlg::OnPaint()
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

HCURSOR CBasicControlsDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CBasicControlsDlg::OnButtonTest()
{
    m_strOutput = _T("=== Basic Controls Demo ===\r\n\r\n");
    m_strOutput += _T("Demonstrating:\r\n- CButton\r\n- CEdit\r\n- CStatic\r\n\r\n");
    m_strOutput += _T("Implementation: See source code\r\n");
    m_strOutput += _T("for detailed feature examples.\r\n");
    UpdateData(FALSE);
}
