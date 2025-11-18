// AdvancedSerializationDlg.cpp
#include "StdAfx.h"
#include "AdvancedSerializationApp.h"
#include "AdvancedSerializationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAdvancedSerializationDlg::CAdvancedSerializationDlg(CWnd* pParent)
    : CDialogEx(IDD_ADVANCEDSERIALIZATION_DIALOG, pParent)
    , m_strOutput(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAdvancedSerializationDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_OUTPUT, m_strOutput);
}

BEGIN_MESSAGE_MAP(CAdvancedSerializationDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_TEST, &CAdvancedSerializationDlg::OnButtonTest)
END_MESSAGE_MAP()

BOOL CAdvancedSerializationDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    m_strOutput = _T("Lesson 21: Advanced Serialization\r\n\r\n");
    m_strOutput += _T("Features:\r\n- Versioning\r\n\r\n");
    m_strOutput += _T("Click Test button for demo.\r\n");

    UpdateData(FALSE);
    return TRUE;
}

void CAdvancedSerializationDlg::OnPaint()
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

HCURSOR CAdvancedSerializationDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CAdvancedSerializationDlg::OnButtonTest()
{
    m_strOutput = _T("=== Advanced Serialization Demo ===\r\n\r\n");
    m_strOutput += _T("Demonstrating:\r\n- Versioning\r\n\r\n");
    m_strOutput += _T("Implementation: See source code\r\n");
    m_strOutput += _T("for detailed feature examples.\r\n");
    UpdateData(FALSE);
}
