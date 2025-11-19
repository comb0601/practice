// MessageBoxDlg.cpp - Implementation of MessageBox demonstrations
//
// MESSAGE BOX TYPES:
// MB_OK, MB_OKCANCEL, MB_YESNO, MB_YESNOCANCEL, MB_RETRYCANCEL, MB_ABORTRETRYIGNORE
//
// ICONS:
// MB_ICONINFORMATION, MB_ICONWARNING, MB_ICONERROR, MB_ICONQUESTION
//
// RETURN VALUES:
// IDOK, IDCANCEL, IDYES, IDNO, IDABORT, IDRETRY, IDIGNORE

#include "StdAfx.h"
#include "MessageBoxApp.h"
#include "MessageBoxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMessageBoxDlg::CMessageBoxDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MESSAGEBOX_DIALOG, pParent)
    , m_strResult(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMessageBoxDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_RESULT, m_strResult);
}

BEGIN_MESSAGE_MAP(CMessageBoxDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_INFO, &CMessageBoxDlg::OnBtnInfo)
    ON_BN_CLICKED(IDC_BTN_WARNING, &CMessageBoxDlg::OnBtnWarning)
    ON_BN_CLICKED(IDC_BTN_ERROR, &CMessageBoxDlg::OnBtnError)
    ON_BN_CLICKED(IDC_BTN_QUESTION, &CMessageBoxDlg::OnBtnQuestion)
    ON_BN_CLICKED(IDC_BTN_OKCANCEL, &CMessageBoxDlg::OnBtnOkCancel)
    ON_BN_CLICKED(IDC_BTN_YESNO, &CMessageBoxDlg::OnBtnYesNo)
    ON_BN_CLICKED(IDC_BTN_YESNOCANCEL, &CMessageBoxDlg::OnBtnYesNoCancel)
    ON_BN_CLICKED(IDC_BTN_ABORTRETRYIGNORE, &CMessageBoxDlg::OnBtnAbortRetryIgnore)
    ON_BN_CLICKED(IDC_BTN_RETRYCANCEL, &CMessageBoxDlg::OnBtnRetryCancel)
    ON_BN_CLICKED(IDC_BTN_CUSTOM, &CMessageBoxDlg::OnBtnCustom)
    ON_BN_CLICKED(IDC_BTN_SYSTEM, &CMessageBoxDlg::OnBtnSystem)
END_MESSAGE_MAP()

BOOL CMessageBoxDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    m_strResult = _T("Click buttons to see different MessageBox types.\r\n");
    m_strResult += _T("The result of each MessageBox will be displayed here.\r\n\r\n");
    m_strResult += _T("ICON TYPES:\r\n");
    m_strResult += _T("- Information (i icon)\r\n");
    m_strResult += _T("- Warning (! icon)\r\n");
    m_strResult += _T("- Error (X icon)\r\n");
    m_strResult += _T("- Question (? icon)\r\n\r\n");
    m_strResult += _T("BUTTON TYPES:\r\n");
    m_strResult += _T("- OK / OK-Cancel\r\n");
    m_strResult += _T("- Yes-No / Yes-No-Cancel\r\n");
    m_strResult += _T("- Retry-Cancel\r\n");
    m_strResult += _T("- Abort-Retry-Ignore\r\n");

    UpdateData(FALSE);
    return TRUE;
}

void CMessageBoxDlg::OnPaint()
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

HCURSOR CMessageBoxDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

// INFORMATION ICON
void CMessageBoxDlg::OnBtnInfo()
{
    int result = MessageBox(
        _T("This is an informational message.\n\nUsed for general information."),
        _T("Information"),
        MB_OK | MB_ICONINFORMATION
    );

    m_strResult = _T("Information MessageBox displayed.\r\n");
    m_strResult += _T("Result: IDOK\r\n");
    m_strResult += _T("Use: Showing information to the user.");
    UpdateData(FALSE);
}

// WARNING ICON
void CMessageBoxDlg::OnBtnWarning()
{
    int result = MessageBox(
        _T("This is a warning message!\n\nProceed with caution."),
        _T("Warning"),
        MB_OK | MB_ICONWARNING
    );

    m_strResult = _T("Warning MessageBox displayed.\r\n");
    m_strResult += _T("Result: IDOK\r\n");
    m_strResult += _T("Use: Warning users about potential issues.");
    UpdateData(FALSE);
}

// ERROR ICON
void CMessageBoxDlg::OnBtnError()
{
    int result = MessageBox(
        _T("This is an error message!\n\nSomething went wrong."),
        _T("Error"),
        MB_OK | MB_ICONERROR
    );

    m_strResult = _T("Error MessageBox displayed.\r\n");
    m_strResult += _T("Result: IDOK\r\n");
    m_strResult += _T("Use: Notifying users of errors.");
    UpdateData(FALSE);
}

// QUESTION ICON
void CMessageBoxDlg::OnBtnQuestion()
{
    int result = MessageBox(
        _T("This is a question.\n\nDo you want to proceed?"),
        _T("Question"),
        MB_YESNO | MB_ICONQUESTION
    );

    m_strResult.Format(_T("Question MessageBox displayed.\r\nResult: %s\r\n"),
        result == IDYES ? _T("IDYES") : _T("IDNO"));
    m_strResult += _T("Use: Asking questions that require yes/no answer.");
    UpdateData(FALSE);
}

// OK-CANCEL BUTTONS
void CMessageBoxDlg::OnBtnOkCancel()
{
    int result = MessageBox(
        _T("Do you want to continue this operation?"),
        _T("Confirmation"),
        MB_OKCANCEL | MB_ICONQUESTION
    );

    m_strResult.Format(_T("OK-Cancel MessageBox displayed.\r\nResult: %s\r\n"),
        result == IDOK ? _T("IDOK (User confirmed)") : _T("IDCANCEL (User cancelled)"));
    m_strResult += _T("Use: Confirming operations with OK/Cancel choice.");
    UpdateData(FALSE);
}

// YES-NO BUTTONS
void CMessageBoxDlg::OnBtnYesNo()
{
    int result = MessageBox(
        _T("Do you want to save changes?"),
        _T("Save Changes"),
        MB_YESNO | MB_ICONQUESTION
    );

    m_strResult.Format(_T("Yes-No MessageBox displayed.\r\nResult: %s\r\n"),
        result == IDYES ? _T("IDYES (Save)") : _T("IDNO (Don't save)"));
    m_strResult += _T("Use: Binary yes/no decisions.");
    UpdateData(FALSE);
}

// YES-NO-CANCEL BUTTONS
void CMessageBoxDlg::OnBtnYesNoCancel()
{
    int result = MessageBox(
        _T("Do you want to save changes before closing?"),
        _T("Save Changes"),
        MB_YESNOCANCEL | MB_ICONQUESTION
    );

    CString strResult;
    switch (result)
    {
    case IDYES:    strResult = _T("IDYES (Save and close)"); break;
    case IDNO:     strResult = _T("IDNO (Close without saving)"); break;
    case IDCANCEL: strResult = _T("IDCANCEL (Don't close)"); break;
    }

    m_strResult.Format(_T("Yes-No-Cancel MessageBox displayed.\r\nResult: %s\r\n"), strResult);
    m_strResult += _T("Use: Save dialogs with three options.");
    UpdateData(FALSE);
}

// ABORT-RETRY-IGNORE BUTTONS
void CMessageBoxDlg::OnBtnAbortRetryIgnore()
{
    int result = MessageBox(
        _T("An error occurred while saving the file.\n\nAbort: Stop operation\nRetry: Try again\nIgnore: Continue anyway"),
        _T("File Save Error"),
        MB_ABORTRETRYIGNORE | MB_ICONERROR
    );

    CString strResult;
    switch (result)
    {
    case IDABORT:  strResult = _T("IDABORT (Stop operation)"); break;
    case IDRETRY:  strResult = _T("IDRETRY (Try again)"); break;
    case IDIGNORE: strResult = _T("IDIGNORE (Continue anyway)"); break;
    }

    m_strResult.Format(_T("Abort-Retry-Ignore MessageBox displayed.\r\nResult: %s\r\n"), strResult);
    m_strResult += _T("Use: Error recovery with multiple options.");
    UpdateData(FALSE);
}

// RETRY-CANCEL BUTTONS
void CMessageBoxDlg::OnBtnRetryCancel()
{
    int result = MessageBox(
        _T("Connection to server failed.\n\nClick Retry to try again, or Cancel to give up."),
        _T("Connection Error"),
        MB_RETRYCANCEL | MB_ICONWARNING
    );

    m_strResult.Format(_T("Retry-Cancel MessageBox displayed.\r\nResult: %s\r\n"),
        result == IDRETRY ? _T("IDRETRY (Try again)") : _T("IDCANCEL (Give up)"));
    m_strResult += _T("Use: Retryable operations.");
    UpdateData(FALSE);
}

// CUSTOM FORMATTED MESSAGE
void CMessageBoxDlg::OnBtnCustom()
{
    CString strMessage;
    strMessage = _T("╔═══════════════════════════════╗\n");
    strMessage += _T("║   CUSTOM FORMATTED MESSAGE    ║\n");
    strMessage += _T("╠═══════════════════════════════╣\n");
    strMessage += _T("║                               ║\n");
    strMessage += _T("║  You can format MessageBox    ║\n");
    strMessage += _T("║  text with:                   ║\n");
    strMessage += _T("║                               ║\n");
    strMessage += _T("║  • Multiple lines (\\n)        ║\n");
    strMessage += _T("║  • Tabs (\\t)                  ║\n");
    strMessage += _T("║  • Unicode characters         ║\n");
    strMessage += _T("║  • Format specifiers          ║\n");
    strMessage += _T("║                               ║\n");
    strMessage += _T("╚═══════════════════════════════╝\n");

    MessageBox(strMessage, _T("Custom Format"), MB_OK | MB_ICONINFORMATION);

    m_strResult = _T("Custom formatted MessageBox displayed.\r\n");
    m_strResult += _T("You can use \\n, \\t, and Unicode characters.\r\n");
    m_strResult += _T("Useful for structured information display.");
    UpdateData(FALSE);
}

// SYSTEM MODAL (blocks all applications)
void CMessageBoxDlg::OnBtnSystem()
{
    // Note: MB_SYSTEMMODAL requires admin privileges in modern Windows
    int result = MessageBox(
        _T("This is a SYSTEM MODAL message box.\n\nOn older systems, this would block ALL applications.\nOn modern Windows (Vista+), requires admin privileges."),
        _T("System Modal"),
        MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL
    );

    m_strResult = _T("System Modal MessageBox displayed.\r\n");
    m_strResult += _T("MB_SYSTEMMODAL flag used.\r\n");
    m_strResult += _T("Note: Modern Windows restricts this for security.\r\n");
    m_strResult += _T("Use: Critical system messages (rarely used).");
    UpdateData(FALSE);
}
