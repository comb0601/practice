// CalculatorDlg.cpp : implementation file
//
// CALCULATOR LOGIC:
// 1. User enters first number (digits accumulate in display)
// 2. User clicks operator (+, -, *, /)
// 3. First number is stored, operator is remembered
// 4. User enters second number
// 5. User clicks = or another operator
// 6. Calculate() performs the operation
// 7. Result is displayed and becomes the new first operand

#include "StdAfx.h"
#include "CalculatorApp.h"
#include "CalculatorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCalculatorDlg dialog

CCalculatorDlg::CCalculatorDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_CALCULATOR_DIALOG, pParent)
    , m_dOperand1(0.0)
    , m_dOperand2(0.0)
    , m_cOperator(0)
    , m_strDisplay(_T("0"))
    , m_bNewNumber(true)
    , m_bDecimalEntered(false)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCalculatorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCalculatorDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()

    // Number buttons
    ON_BN_CLICKED(IDC_BUTTON_0, &CCalculatorDlg::OnButton0)
    ON_BN_CLICKED(IDC_BUTTON_1, &CCalculatorDlg::OnButton1)
    ON_BN_CLICKED(IDC_BUTTON_2, &CCalculatorDlg::OnButton2)
    ON_BN_CLICKED(IDC_BUTTON_3, &CCalculatorDlg::OnButton3)
    ON_BN_CLICKED(IDC_BUTTON_4, &CCalculatorDlg::OnButton4)
    ON_BN_CLICKED(IDC_BUTTON_5, &CCalculatorDlg::OnButton5)
    ON_BN_CLICKED(IDC_BUTTON_6, &CCalculatorDlg::OnButton6)
    ON_BN_CLICKED(IDC_BUTTON_7, &CCalculatorDlg::OnButton7)
    ON_BN_CLICKED(IDC_BUTTON_8, &CCalculatorDlg::OnButton8)
    ON_BN_CLICKED(IDC_BUTTON_9, &CCalculatorDlg::OnButton9)

    // Operator buttons
    ON_BN_CLICKED(IDC_BUTTON_ADD, &CCalculatorDlg::OnButtonAdd)
    ON_BN_CLICKED(IDC_BUTTON_SUB, &CCalculatorDlg::OnButtonSub)
    ON_BN_CLICKED(IDC_BUTTON_MUL, &CCalculatorDlg::OnButtonMul)
    ON_BN_CLICKED(IDC_BUTTON_DIV, &CCalculatorDlg::OnButtonDiv)
    ON_BN_CLICKED(IDC_BUTTON_EQUALS, &CCalculatorDlg::OnButtonEquals)

    // Special buttons
    ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CCalculatorDlg::OnButtonClear)
    ON_BN_CLICKED(IDC_BUTTON_DECIMAL, &CCalculatorDlg::OnButtonDecimal)
    ON_BN_CLICKED(IDC_BUTTON_PLUSMINUS, &CCalculatorDlg::OnButtonPlusMinus)
END_MESSAGE_MAP()

// CCalculatorDlg message handlers

BOOL CCalculatorDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    // Initialize display
    SetDlgItemText(IDC_EDIT_DISPLAY, m_strDisplay);

    return TRUE;
}

void CCalculatorDlg::OnPaint()
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

HCURSOR CCalculatorDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

// HELPER FUNCTION: Update the display control
void CCalculatorDlg::UpdateDisplay()
{
    SetDlgItemText(IDC_EDIT_DISPLAY, m_strDisplay);
}

// HELPER FUNCTION: Handle number button clicks
void CCalculatorDlg::HandleNumber(int nNumber)
{
    if (m_bNewNumber)
    {
        // Start new number
        m_strDisplay.Format(_T("%d"), nNumber);
        m_bNewNumber = false;
        m_bDecimalEntered = false;
    }
    else
    {
        // Append digit to existing number
        if (m_strDisplay == _T("0"))
            m_strDisplay.Format(_T("%d"), nNumber);
        else
            m_strDisplay.AppendFormat(_T("%d"), nNumber);
    }
    UpdateDisplay();
}

// HELPER FUNCTION: Handle operator button clicks
void CCalculatorDlg::HandleOperator(char cOp)
{
    if (m_cOperator != 0)
    {
        // If there's a pending operation, calculate it first
        Calculate();
    }
    else
    {
        // Store the first operand
        m_dOperand1 = _ttof(m_strDisplay);
    }

    m_cOperator = cOp;
    m_bNewNumber = true;
    m_bDecimalEntered = false;
}

// CORE FUNCTION: Perform the calculation
void CCalculatorDlg::Calculate()
{
    // Get the second operand from display
    m_dOperand2 = _ttof(m_strDisplay);

    double dResult = 0.0;
    bool bError = false;

    // Perform operation based on operator
    switch (m_cOperator)
    {
    case '+':
        dResult = m_dOperand1 + m_dOperand2;
        break;

    case '-':
        dResult = m_dOperand1 - m_dOperand2;
        break;

    case '*':
        dResult = m_dOperand1 * m_dOperand2;
        break;

    case '/':
        if (m_dOperand2 != 0.0)
            dResult = m_dOperand1 / m_dOperand2;
        else
        {
            m_strDisplay = _T("Error: Div by 0");
            bError = true;
        }
        break;

    default:
        dResult = m_dOperand2;
        break;
    }

    if (!bError)
    {
        // Format result, removing trailing zeros
        m_strDisplay.Format(_T("%.10f"), dResult);

        // Remove trailing zeros after decimal point
        if (m_strDisplay.Find('.') != -1)
        {
            m_strDisplay.TrimRight('0');
            m_strDisplay.TrimRight('.');
        }

        m_dOperand1 = dResult;
    }

    m_cOperator = 0;
    m_bNewNumber = true;
    UpdateDisplay();
}

// NUMBER BUTTON HANDLERS
void CCalculatorDlg::OnButton0() { HandleNumber(0); }
void CCalculatorDlg::OnButton1() { HandleNumber(1); }
void CCalculatorDlg::OnButton2() { HandleNumber(2); }
void CCalculatorDlg::OnButton3() { HandleNumber(3); }
void CCalculatorDlg::OnButton4() { HandleNumber(4); }
void CCalculatorDlg::OnButton5() { HandleNumber(5); }
void CCalculatorDlg::OnButton6() { HandleNumber(6); }
void CCalculatorDlg::OnButton7() { HandleNumber(7); }
void CCalculatorDlg::OnButton8() { HandleNumber(8); }
void CCalculatorDlg::OnButton9() { HandleNumber(9); }

// OPERATOR BUTTON HANDLERS
void CCalculatorDlg::OnButtonAdd() { HandleOperator('+'); }
void CCalculatorDlg::OnButtonSub() { HandleOperator('-'); }
void CCalculatorDlg::OnButtonMul() { HandleOperator('*'); }
void CCalculatorDlg::OnButtonDiv() { HandleOperator('/'); }

// EQUALS BUTTON
void CCalculatorDlg::OnButtonEquals()
{
    if (m_cOperator != 0)
    {
        Calculate();
    }
}

// CLEAR BUTTON: Reset calculator to initial state
void CCalculatorDlg::OnButtonClear()
{
    m_dOperand1 = 0.0;
    m_dOperand2 = 0.0;
    m_cOperator = 0;
    m_strDisplay = _T("0");
    m_bNewNumber = true;
    m_bDecimalEntered = false;
    UpdateDisplay();
}

// DECIMAL BUTTON: Add decimal point
void CCalculatorDlg::OnButtonDecimal()
{
    if (m_bNewNumber)
    {
        m_strDisplay = _T("0.");
        m_bNewNumber = false;
        m_bDecimalEntered = true;
    }
    else if (!m_bDecimalEntered)
    {
        m_strDisplay += _T(".");
        m_bDecimalEntered = true;
    }
    UpdateDisplay();
}

// PLUS/MINUS BUTTON: Toggle sign
void CCalculatorDlg::OnButtonPlusMinus()
{
    double dValue = _ttof(m_strDisplay);
    dValue = -dValue;

    m_strDisplay.Format(_T("%.10f"), dValue);

    // Remove trailing zeros
    if (m_strDisplay.Find('.') != -1)
    {
        m_strDisplay.TrimRight('0');
        m_strDisplay.TrimRight('.');
    }

    UpdateDisplay();
}
