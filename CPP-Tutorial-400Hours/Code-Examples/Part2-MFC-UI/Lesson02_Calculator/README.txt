================================================================================
LESSON 02: MFC CALCULATOR APPLICATION
================================================================================

WHAT THIS DEMONSTRATES:
- Complex message handling with multiple buttons
- State machine design pattern
- Calculator logic implementation
- String formatting and conversion
- Button layout in dialog resources

================================================================================
CALCULATOR FEATURES:
================================================================================

1. Basic Operations: +, -, *, /
2. Decimal point support
3. Plus/minus toggle for negative numbers
4. Clear function to reset
5. Chained operations (5 + 3 * 2 = ...)
6. Division by zero error handling

================================================================================
FILE STRUCTURE:
================================================================================

CalculatorApp.h / CalculatorApp.cpp
    - Standard MFC application class
    - Initializes and displays calculator dialog

CalculatorDlg.h / CalculatorDlg.cpp
    - Main calculator dialog class
    - STATE VARIABLES:
        * m_dOperand1 - First number
        * m_dOperand2 - Second number
        * m_cOperator - Current operator
        * m_strDisplay - Display string
        * m_bNewNumber - Flag for new number entry
        * m_bDecimalEntered - Decimal point flag

    - KEY METHODS:
        * HandleNumber() - Process digit button clicks
        * HandleOperator() - Process operator clicks
        * Calculate() - Perform the actual calculation
        * UpdateDisplay() - Refresh display control

Calculator.rc
    - Dialog resource with button layout
    - 10 number buttons (0-9)
    - 4 operator buttons (+, -, *, /)
    - Special buttons (=, C, ., +/-)
    - Read-only edit control for display

================================================================================
CALCULATOR LOGIC FLOW:
================================================================================

1. INITIALIZATION:
   - Display shows "0"
   - All operands set to 0
   - No operator selected
   - NewNumber flag is TRUE

2. USER ENTERS FIRST NUMBER:
   Example: User clicks 1, 2, 3
   - OnButton1() -> HandleNumber(1)
   - Display = "1", NewNumber = FALSE
   - OnButton2() -> HandleNumber(2)
   - Display = "12" (append)
   - OnButton3() -> HandleNumber(3)
   - Display = "123" (append)

3. USER CLICKS OPERATOR:
   Example: User clicks +
   - OnButtonAdd() -> HandleOperator('+')
   - m_dOperand1 = 123 (store first number)
   - m_cOperator = '+' (remember operator)
   - NewNumber = TRUE (ready for second number)

4. USER ENTERS SECOND NUMBER:
   Example: User clicks 4, 5
   - OnButton4() -> HandleNumber(4)
   - Display = "4" (new number, replaces 123)
   - OnButton5() -> HandleNumber(5)
   - Display = "45" (append)

5. USER CLICKS EQUALS:
   - OnButtonEquals() -> Calculate()
   - m_dOperand2 = 45 (get from display)
   - Result = 123 + 45 = 168
   - Display = "168"
   - m_dOperand1 = 168 (for chaining)
   - m_cOperator = 0 (clear operator)

6. CHAINED OPERATIONS:
   If user clicks another operator instead of =:
   - Calculate is called first
   - Result becomes new first operand
   - New operator is stored
   - Ready for next number

================================================================================
KEY MFC CONCEPTS DEMONSTRATED:
================================================================================

1. MULTIPLE MESSAGE HANDLERS:
   - Each button has its own ON_BN_CLICKED entry
   - Message map connects control IDs to handlers
   - Example:
     ON_BN_CLICKED(IDC_BUTTON_5, &CCalculatorDlg::OnButton5)

2. STATE MANAGEMENT:
   - Member variables maintain calculator state
   - State persists across button clicks
   - Flags control behavior (m_bNewNumber)

3. STRING CONVERSION:
   - CString for display text
   - _ttof() converts string to double
   - Format() converts double to string
   - String manipulation (TrimRight, AppendFormat)

4. HELPER FUNCTIONS:
   - HandleNumber() - Reusable for all digit buttons
   - HandleOperator() - Reusable for all operators
   - Calculate() - Centralized calculation logic
   - UpdateDisplay() - Single place to update UI

5. ERROR HANDLING:
   - Division by zero check
   - Display error message
   - Prevent crash

================================================================================
WHERE TO LOOK IN THE CODE:
================================================================================

CalculatorDlg.h:
    Lines 30-35: State variables
    Lines 38-41: Helper function declarations
    Lines 50-76: Button handler declarations

CalculatorDlg.cpp:
    Lines 35-75: Message map with all button connections
    Lines 115-120: UpdateDisplay() - UI update
    Lines 123-140: HandleNumber() - Digit processing
    Lines 143-160: HandleOperator() - Operator processing
    Lines 163-215: Calculate() - Core calculation logic
    Lines 218-226: Number button handlers (call HandleNumber)
    Lines 229-233: Operator button handlers (call HandleOperator)
    Lines 236-242: OnButtonEquals() - Trigger calculation
    Lines 245-255: OnButtonClear() - Reset state
    Lines 258-272: OnButtonDecimal() - Decimal point handling
    Lines 275-289: OnButtonPlusMinus() - Sign toggle

Calculator.rc:
    Lines 25-50: Dialog layout with button positions

================================================================================
EXTENDING THE CALCULATOR:
================================================================================

1. ADD MORE OPERATIONS:
   - Square root, power, percentage
   - Add buttons to .rc file
   - Add handlers to message map
   - Extend Calculate() function

2. ADD MEMORY FUNCTIONS:
   - M+, M-, MR, MC buttons
   - Add m_dMemory member variable
   - Store/recall/clear memory

3. ADD HISTORY:
   - List control to show calculations
   - Store each operation in CArray
   - Display history in separate area

4. ADD KEYBOARD SUPPORT:
   - Override PreTranslateMessage()
   - Map key codes to buttons
   - Support Enter, Backspace, etc.

5. ADD SCIENTIFIC MODE:
   - Sin, Cos, Tan, Log functions
   - Resize dialog for more buttons
   - Use <cmath> functions

================================================================================
TESTING THE CALCULATOR:
================================================================================

Test Cases:
1. Basic addition: 5 + 3 = 8
2. Basic subtraction: 10 - 4 = 6
3. Basic multiplication: 7 * 6 = 42
4. Basic division: 20 / 5 = 4
5. Division by zero: 5 / 0 = Error
6. Chained operations: 5 + 3 * 2 = 16
7. Decimal numbers: 5.5 + 2.3 = 7.8
8. Negative numbers: 5 +/- + 3 = -2
9. Clear function: Enter number, press C, verify 0
10. Multiple decimals: 5.5.5 should only add one decimal

================================================================================
COMMON ISSUES:
================================================================================

1. Buttons not responding:
   - Check message map connections
   - Verify control IDs match Resource.h
   - Check handler function signatures

2. Display not updating:
   - Ensure UpdateDisplay() is called
   - Check SetDlgItemText() control ID
   - Verify m_strDisplay is set correctly

3. Wrong calculations:
   - Check operator precedence
   - Verify Calculate() logic
   - Check order of operations

4. Decimal point issues:
   - Check m_bDecimalEntered flag
   - Verify decimal point detection
   - Test multiple decimal attempts

================================================================================
COMPILE IN VISUAL STUDIO:
================================================================================

1. Create new MFC Dialog-based Application
2. Copy all files to project directory
3. Replace generated files
4. Build Solution (Ctrl+Shift+B)
5. Run (F5)

Project Settings:
- Platform: Win32 or x64
- Configuration: Debug or Release
- Use MFC in a Shared DLL
- Precompiled Headers: Use StdAfx.h

================================================================================
