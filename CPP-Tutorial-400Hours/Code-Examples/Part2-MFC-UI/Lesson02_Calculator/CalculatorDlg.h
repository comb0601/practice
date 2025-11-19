// CalculatorDlg.h : header file
//
// WHERE TO LOOK:
// 1. Calculator state variables (m_dOperand1, m_dOperand2, m_cOperator)
// 2. Message handlers for all buttons
// 3. Calculate() method that performs operations
// 4. UpdateDisplay() method that shows results

#pragma once

// CCalculatorDlg dialog
// CALCULATOR ARCHITECTURE:
// - Uses two operands and one operator
// - State machine approach
// - Display is updated after each button click

class CCalculatorDlg : public CDialogEx
{
// Construction
public:
    CCalculatorDlg(CWnd* pParent = nullptr);

// Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_CALCULATOR_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

// Implementation
protected:
    HICON m_hIcon;

    // Calculator state
    double m_dOperand1;          // First operand
    double m_dOperand2;          // Second operand
    char m_cOperator;            // Current operator (+, -, *, /)
    CString m_strDisplay;        // Current display value
    bool m_bNewNumber;           // Flag for starting new number entry
    bool m_bDecimalEntered;      // Flag to track if decimal point entered

    // Helper functions
    void UpdateDisplay();
    void Calculate();
    void HandleNumber(int nNumber);
    void HandleOperator(char cOp);

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

public:
    // Number button handlers
    afx_msg void OnButton0();
    afx_msg void OnButton1();
    afx_msg void OnButton2();
    afx_msg void OnButton3();
    afx_msg void OnButton4();
    afx_msg void OnButton5();
    afx_msg void OnButton6();
    afx_msg void OnButton7();
    afx_msg void OnButton8();
    afx_msg void OnButton9();

    // Operator button handlers
    afx_msg void OnButtonAdd();
    afx_msg void OnButtonSub();
    afx_msg void OnButtonMul();
    afx_msg void OnButtonDiv();
    afx_msg void OnButtonEquals();

    // Special button handlers
    afx_msg void OnButtonClear();
    afx_msg void OnButtonDecimal();
    afx_msg void OnButtonPlusMinus();
};
