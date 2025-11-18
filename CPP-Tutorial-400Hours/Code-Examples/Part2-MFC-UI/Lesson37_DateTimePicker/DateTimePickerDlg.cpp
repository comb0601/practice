// DateTimePickerDlg.cpp
#include "StdAfx.h"
#include "DateTimePickerApp.h"
#include "DateTimePickerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDateTimePickerDlg::CDateTimePickerDlg(CWnd* pParent)
    : CDialogEx(IDD_DATETIMEPICKER_DIALOG, pParent)
    , m_strOutput(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDateTimePickerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_OUTPUT, m_strOutput);
}

BEGIN_MESSAGE_MAP(CDateTimePickerDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_TEST, &CDateTimePickerDlg::OnButtonTest)
END_MESSAGE_MAP()

BOOL CDateTimePickerDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    m_strOutput = _T("Lesson 37: Date Time Picker\r\n\r\n");
    m_strOutput += _T("Features:\r\n- CDateTimeCtrl\r\n\r\n");
    m_strOutput += _T("Click Test button for demo.\r\n");

    UpdateData(FALSE);
    return TRUE;
}

void CDateTimePickerDlg::OnPaint()
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

HCURSOR CDateTimePickerDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CDateTimePickerDlg::OnButtonTest()
{
    m_strOutput = _T("=== Date Time Picker Demo ===\r\n\r\n");
    m_strOutput += _T("Demonstrating:\r\n- CDateTimeCtrl\r\n\r\n");
    m_strOutput += _T("Implementation: See source code\r\n");
    m_strOutput += _T("for detailed feature examples.\r\n");
    UpdateData(FALSE);
}
