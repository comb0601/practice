// SimpleMFCDlg.cpp : implementation file
//
// MESSAGE FLOW:
// 1. User clicks button
// 2. Windows sends WM_COMMAND message to dialog
// 3. MFC's message map routes it to OnButtonHello()
// 4. Handler updates member variable
// 5. UpdateData(FALSE) transfers data to control
// 6. User sees the update on screen

#include "StdAfx.h"
#include "SimpleMFCApp.h"
#include "SimpleMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSimpleMFCDlg dialog

CSimpleMFCDlg::CSimpleMFCDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_SIMPLEMFC_DIALOG, pParent)
    , m_strOutput(_T(""))  // Initialize member variable
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSimpleMFCDlg::DoDataExchange(CDataExchange* pDX)
{
    // DDX (Dialog Data Exchange):
    // This function transfers data between member variables and controls
    // UpdateData(TRUE)  - Transfer from controls to variables
    // UpdateData(FALSE) - Transfer from variables to controls

    CDialogEx::DoDataExchange(pDX);

    // Link the m_strOutput member variable to the IDC_EDIT_OUTPUT control
    // MFC will automatically synchronize them when UpdateData is called
    DDX_Text(pDX, IDC_EDIT_OUTPUT, m_strOutput);
}

BEGIN_MESSAGE_MAP(CSimpleMFCDlg, CDialogEx)
    // MESSAGE MAP:
    // This macro-based system connects Windows messages to handler functions
    // Format: ON_MESSAGE_TYPE(ControlID, HandlerFunction)

    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    // Connect button click to handler
    ON_BN_CLICKED(IDC_BUTTON_HELLO, &CSimpleMFCDlg::OnButtonHello)
END_MESSAGE_MAP()


// CSimpleMFCDlg message handlers

BOOL CSimpleMFCDlg::OnInitDialog()
{
    // OnInitDialog is called once when the dialog is created
    // This is where you initialize controls and set up the dialog

    CDialogEx::OnInitDialog();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);            // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    // TODO: Add extra initialization here
    m_strOutput = _T("Welcome to MFC!\r\nClick the button to see a greeting.");
    UpdateData(FALSE);  // Transfer data to controls

    return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSimpleMFCDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSimpleMFCDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


// EVENT HANDLER: Button Click
// This is the heart of user interaction in MFC
void CSimpleMFCDlg::OnButtonHello()
{
    // STEP 1: Get current time for personalized greeting
    CTime currentTime = CTime::GetCurrentTime();
    CString timeStr = currentTime.Format(_T("%I:%M:%S %p"));

    // STEP 2: Build output message
    m_strOutput = _T("Hello from MFC!\r\n\r\n");
    m_strOutput += _T("This is your first MFC application.\r\n");
    m_strOutput += _T("Current time: ") + timeStr + _T("\r\n\r\n");
    m_strOutput += _T("MFC Architecture:\r\n");
    m_strOutput += _T("- CWinApp: Application class\r\n");
    m_strOutput += _T("- CDialog: Dialog window class\r\n");
    m_strOutput += _T("- Message Maps: Event handling\r\n");
    m_strOutput += _T("- DDX/DDV: Data exchange\r\n");

    // STEP 3: Transfer data to control
    // UpdateData(FALSE) copies m_strOutput to the edit control
    UpdateData(FALSE);

    // Alternative without DDX:
    // SetDlgItemText(IDC_EDIT_OUTPUT, m_strOutput);
}
