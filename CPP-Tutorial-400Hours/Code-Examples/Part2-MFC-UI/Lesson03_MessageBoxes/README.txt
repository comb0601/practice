================================================================================
LESSON 03: MESSAGE BOX TYPES IN MFC
================================================================================

WHAT THIS DEMONSTRATES:
- All MessageBox icon types (Information, Warning, Error, Question)
- All MessageBox button combinations
- Return value handling (IDOK, IDCANCEL, IDYES, IDNO, etc.)
- Custom formatted messages
- System modal messages

================================================================================
MESSAGE BOX SYNTAX:
================================================================================

int MessageBox(
    LPCTSTR lpszText,      // Message text
    LPCTSTR lpszCaption,   // Title bar text
    UINT nType             // Icon and buttons
);

ICON FLAGS:
- MB_ICONINFORMATION  - Blue (i) icon
- MB_ICONWARNING      - Yellow (!) icon
- MB_ICONERROR        - Red (X) icon
- MB_ICONQUESTION     - Blue (?) icon (deprecated in modern Windows)

BUTTON FLAGS:
- MB_OK               - Single OK button
- MB_OKCANCEL         - OK and Cancel buttons
- MB_YESNO            - Yes and No buttons
- MB_YESNOCANCEL      - Yes, No, and Cancel buttons
- MB_RETRYCANCEL      - Retry and Cancel buttons
- MB_ABORTRETRYIGNORE - Abort, Retry, and Ignore buttons

MODALITY FLAGS:
- MB_APPLMODAL        - Blocks current application (default)
- MB_SYSTEMMODAL      - Blocks all applications (requires admin)
- MB_TASKMODAL        - Similar to APPLMODAL

RETURN VALUES:
- IDOK       = 1   // OK button pressed
- IDCANCEL   = 2   // Cancel button pressed
- IDABORT    = 3   // Abort button pressed
- IDRETRY    = 4   // Retry button pressed
- IDIGNORE   = 5   // Ignore button pressed
- IDYES      = 6   // Yes button pressed
- IDNO       = 7   // No button pressed

================================================================================
COMMON USAGE PATTERNS:
================================================================================

1. SIMPLE NOTIFICATION:
   MessageBox(_T("Operation completed!"), _T("Success"), MB_OK | MB_ICONINFORMATION);

2. YES/NO QUESTION:
   if (MessageBox(_T("Save changes?"), _T("Confirm"), MB_YESNO | MB_ICONQUESTION) == IDYES)
   {
       SaveFile();
   }

3. SAVE BEFORE CLOSING:
   int result = MessageBox(_T("Save before closing?"), _T("Save"), MB_YESNOCANCEL | MB_ICONQUESTION);
   if (result == IDYES)
       SaveAndClose();
   else if (result == IDNO)
       CloseWithoutSaving();
   // IDCANCEL: Don't close

4. ERROR WITH RETRY:
   while (true)
   {
       if (TryOperation())
           break;

       if (MessageBox(_T("Failed. Retry?"), _T("Error"), MB_RETRYCANCEL | MB_ICONERROR) == IDCANCEL)
           break;
   }

================================================================================
WHERE TO LOOK IN THE CODE:
================================================================================

MessageBoxDlg.cpp:
    Lines 80-95:   OnBtnInfo()      - Information icon
    Lines 97-110:  OnBtnWarning()   - Warning icon
    Lines 112-125: OnBtnError()     - Error icon
    Lines 127-140: OnBtnQuestion()  - Question icon with Yes/No
    Lines 142-155: OnBtnOkCancel()  - OK/Cancel buttons
    Lines 157-170: OnBtnYesNo()     - Yes/No buttons
    Lines 172-190: OnBtnYesNoCancel() - Three button example
    Lines 192-215: OnBtnAbortRetryIgnore() - Error recovery pattern
    Lines 217-230: OnBtnRetryCancel() - Retry pattern
    Lines 232-265: OnBtnCustom()    - Custom formatting
    Lines 267-280: OnBtnSystem()    - System modal

================================================================================
KEY CONCEPTS:
================================================================================

1. BLOCKING BEHAVIOR:
   - MessageBox is modal - blocks until user responds
   - Application continues only after user clicks button
   - Use for critical decisions

2. RETURN VALUE HANDLING:
   - Always check return value for multi-button dialogs
   - Use switch statement for 3+ buttons
   - Use if statement for 2 buttons

3. ICON SELECTION:
   - Information: General messages, success
   - Warning: Caution, potential issues
   - Error: Failures, problems
   - Question: Yes/No decisions (use sparingly)

4. BUTTON SELECTION:
   - OK: Simple acknowledgment
   - OK/Cancel: Confirm or abort action
   - Yes/No: Binary decision
   - Yes/No/Cancel: Save dialogs
   - Retry/Cancel: Retryable operations
   - Abort/Retry/Ignore: Error recovery

================================================================================
BEST PRACTICES:
================================================================================

1. Use appropriate icons:
   - Don't use Error for questions
   - Don't use Information for warnings

2. Clear message text:
   - State the problem/question clearly
   - Explain what each button does
   - Keep it concise

3. Meaningful titles:
   - Use descriptive caption
   - Application name + action
   - Example: "MyApp - Save File"

4. Consider alternatives:
   - Toast notifications for non-critical info
   - Custom dialogs for complex choices
   - Status bar for progress updates

5. Avoid overuse:
   - Don't interrupt workflow unnecessarily
   - Batch multiple messages if possible
   - Use asynchronous notifications when appropriate

================================================================================
COMPILE IN VISUAL STUDIO:
================================================================================

1. Create new MFC Dialog-based Application
2. Copy these files to project directory
3. Replace generated files
4. Build Solution (Ctrl+Shift+B)
5. Run (F5)

================================================================================
