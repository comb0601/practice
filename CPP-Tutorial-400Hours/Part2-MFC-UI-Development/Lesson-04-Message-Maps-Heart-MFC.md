# Lesson 04: Message Maps - The Heart of MFC

**Duration**: 8-10 hours
**Difficulty**: Intermediate to Advanced

## Table of Contents
1. Windows Messages Fundamentals
2. What Are Message Maps?
3. The Anatomy of a Message Map
4. How Message Maps Work Under the Hood
5. Common Message Map Macros
6. Adding Message Handlers Manually vs Class Wizard
7. Message Routing and Propagation
8. Reflected Messages
9. Custom Messages
10. Message Maps vs Virtual Functions
11. Complete Real-World Examples
12. Debugging Message Maps

---

## 1. Windows Messages Fundamentals

### What is a Windows Message?

Every interaction with a Windows application is driven by **messages**:

```cpp
// A message is just a structure with these components:
typedef struct tagMSG {
    HWND   hwnd;      // Which window receives the message
    UINT   message;   // Message ID (e.g., WM_PAINT, WM_LBUTTONDOWN)
    WPARAM wParam;    // Additional info (usage depends on message)
    LPARAM lParam;    // Additional info (usage depends on message)
    DWORD  time;      // When the message was posted
    POINT  pt;        // Mouse position
} MSG;
```

### Common Windows Messages

```cpp
// Window lifecycle messages
WM_CREATE         // Window is being created
WM_DESTROY        // Window is being destroyed
WM_CLOSE          // User clicked X button

// Paint messages
WM_PAINT          // Window needs redrawing
WM_ERASEBKGND     // Background needs erasing

// Input messages
WM_LBUTTONDOWN    // Left mouse button pressed
WM_LBUTTONUP      // Left mouse button released
WM_MOUSEMOVE      // Mouse moved
WM_KEYDOWN        // Key pressed
WM_KEYUP          // Key released
WM_CHAR           // Character typed

// Control messages
WM_COMMAND        // Menu item or button clicked
WM_NOTIFY         // Complex control notification

// Sizing messages
WM_SIZE           // Window size changed
WM_MOVE           // Window moved

// System messages
WM_TIMER          // Timer elapsed
WM_INITDIALOG     // Dialog is initializing
```

### Traditional Win32 Message Handling

**Without MFC**, you'd handle messages like this:

```cpp
// Win32 API - Pure C approach
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        // Window created
        return 0;

    case WM_LBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        // Handle click at (x, y)
        return 0;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        // Paint code here
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}
```

**Problems with this approach**:
- ONE giant switch statement for ALL messages
- Hard to organize as applications grow
- No type safety
- Difficult to maintain
- Not object-oriented

### How MFC Improves This

**MFC uses message maps** to dispatch messages to individual functions:

```cpp
// MFC approach - much cleaner
class CMyDialog : public CDialogEx
{
protected:
    // Individual handler functions
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg void OnBnClickedButton1();

    DECLARE_MESSAGE_MAP()  // ← Declares message map
};

// In .cpp file - message map connects messages to handlers
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_WM_LBUTTONDOWN()                                          // WM_LBUTTONDOWN → OnLButtonDown
    ON_WM_PAINT()                                               // WM_PAINT → OnPaint
    ON_BN_CLICKED(IDC_BUTTON1, &CMyDialog::OnBnClickedButton1) // Button click → OnBnClickedButton1
END_MESSAGE_MAP()

// Implementations
void CMyDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
    // Handle mouse click
}

void CMyDialog::OnPaint()
{
    // Handle painting
}

void CMyDialog::OnBnClickedButton1()
{
    // Handle button click
}
```

**Advantages**:
- ✓ Organized - each message has its own function
- ✓ Type-safe - correct parameters automatically
- ✓ Object-oriented - methods on classes
- ✓ Easy to maintain
- ✓ Compiler-checked

---

## 2. What Are Message Maps?

### The Concept

A **message map** is a table that connects Windows messages to C++ member functions.

```
┌─────────────────────┐
│  Windows Message    │
│   (WM_LBUTTONDOWN) │
└─────────┬───────────┘
          │ Message Map
          │ looks up handler
          ↓
┌─────────────────────┐
│  Your Function      │
│  OnLButtonDown()    │
└─────────────────────┘
```

### WHERE Message Maps Are

Every MFC class that handles messages has a message map:

```cpp
// In .h file - DECLARATION
class CMyDialog : public CDialogEx
{
    // ... class members ...

protected:
    DECLARE_MESSAGE_MAP()  // ← This macro declares the message map
};
```

```cpp
// In .cpp file - DEFINITION
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    // Message map entries go here
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_BN_CLICKED(IDC_BUTTON1, &CMyDialog::OnBnClickedButton1)
END_MESSAGE_MAP()
```

### The Three Parts of a Message Map

1. **Declaration** (in .h): `DECLARE_MESSAGE_MAP()`
2. **Definition** (in .cpp): `BEGIN_MESSAGE_MAP` ... `END_MESSAGE_MAP`
3. **Handler functions** (in .cpp): The actual function implementations

---

## 3. The Anatomy of a Message Map

### Complete Example

```cpp
// ===== In MyDialog.h =====
class CMyDialog : public CDialogEx
{
public:
    CMyDialog(CWnd* pParent = nullptr);

protected:
    // Handler function declarations
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedOk();

    DECLARE_MESSAGE_MAP()  // ← PART 1: Declaration
};

// ===== In MyDialog.cpp =====

// PART 2: Message Map Definition
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    //    ↑ This class   ↑ Base class
    ON_WM_LBUTTONDOWN()
    ON_WM_PAINT()
    ON_WM_TIMER()
    ON_BN_CLICKED(IDOK, &CMyDialog::OnBnClickedOk)
END_MESSAGE_MAP()

// PART 3: Handler Implementations
void CMyDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
    MessageBox(_T("You clicked the mouse!"));
    CDialogEx::OnLButtonDown(nFlags, point);  // Optional base call
}

void CMyDialog::OnPaint()
{
    CPaintDC dc(this);
    dc.TextOut(10, 10, _T("Hello!"));
}

void CMyDialog::OnTimer(UINT_PTR nIDEvent)
{
    // Timer fired
    CDialogEx::OnTimer(nIDEvent);
}

void CMyDialog::OnBnClickedOk()
{
    // OK button clicked
    CDialogEx::OnOK();  // Close dialog
}
```

### Understanding Each Macro

#### DECLARE_MESSAGE_MAP()

**WHERE**: In .h file, inside class declaration

**WHAT IT DOES**: Declares three things:

```cpp
// What DECLARE_MESSAGE_MAP() expands to (simplified):
protected:
    static const AFX_MSGMAP_ENTRY _messageEntries[];
    static const AFX_MSGMAP messageMap;
    virtual const AFX_MSGMAP* GetMessageMap() const;
```

**YOU DON'T NEED TO KNOW THIS** - just know that it declares the message map infrastructure.

#### BEGIN_MESSAGE_MAP(ThisClass, BaseClass)

**WHERE**: In .cpp file

**PARAMETERS**:
- `ThisClass` - Your class name (e.g., `CMyDialog`)
- `BaseClass` - Your immediate base class (e.g., `CDialogEx`)

**WHAT IT DOES**: Starts the message map table

```cpp
// What it expands to (simplified):
const AFX_MSGMAP* CMyDialog::GetMessageMap() const
{
    return GetThisMessageMap();
}

const AFX_MSGMAP* CMyDialog::GetThisMessageMap()
{
    static const AFX_MSGMAP_ENTRY _messageEntries[] =
    {
        // Message map entries go here...
```

#### END_MESSAGE_MAP()

**WHERE**: In .cpp file, after all message map entries

**WHAT IT DOES**: Closes the message map table

```cpp
// What it expands to (simplified):
        { 0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 }
    };
    static const AFX_MSGMAP messageMap =
    {
        &BaseClass::GetThisMessageMap,
        &_messageEntries[0]
    };
    return &messageMap;
}
```

### Message Map Entry Macros

These go **between** `BEGIN_MESSAGE_MAP` and `END_MESSAGE_MAP`:

#### Standard Windows Messages (ON_WM_xxx)

```cpp
ON_WM_CREATE()              // WM_CREATE
ON_WM_DESTROY()             // WM_DESTROY
ON_WM_PAINT()               // WM_PAINT
ON_WM_SIZE()                // WM_SIZE
ON_WM_MOVE()                // WM_MOVE
ON_WM_LBUTTONDOWN()         // WM_LBUTTONDOWN
ON_WM_LBUTTONUP()           // WM_LBUTTONUP
ON_WM_RBUTTONDOWN()         // WM_RBUTTONDOWN
ON_WM_RBUTTONUP()           // WM_RBUTTONUP
ON_WM_MOUSEMOVE()           // WM_MOUSEMOVE
ON_WM_KEYDOWN()             // WM_KEYDOWN
ON_WM_KEYUP()               // WM_KEYUP
ON_WM_CHAR()                // WM_CHAR
ON_WM_TIMER()               // WM_TIMER
ON_WM_CLOSE()               // WM_CLOSE
```

**SYNTAX**: Just `ON_WM_MESSAGENAME()` - no parameters needed

**REQUIREMENTS**: Your class must have matching handler:

```cpp
// For ON_WM_LBUTTONDOWN()
afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

// For ON_WM_PAINT()
afx_msg void OnPaint();

// For ON_WM_TIMER()
afx_msg void OnTimer(UINT_PTR nIDEvent);
```

**WHERE TO FIND HANDLER SIGNATURES**: Right-click in message map → Class Wizard → Messages tab

#### Control Notifications (ON_BN_CLICKED, ON_EN_CHANGE, etc.)

```cpp
// Button clicked
ON_BN_CLICKED(controlID, &ClassName::HandlerFunction)

// Edit control changed
ON_EN_CHANGE(controlID, &ClassName::HandlerFunction)

// List box selection changed
ON_LBN_SELCHANGE(controlID, &ClassName::HandlerFunction)

// Combo box selection changed
ON_CBN_SELCHANGE(controlID, &ClassName::HandlerFunction)
```

**SYNTAX**: `ON_XX_NOTIFICATION(controlID, &ClassName::HandlerName)`

**Example**:
```cpp
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_SAVE, &CMyDialog::OnBnClickedButtonSave)
    ON_EN_CHANGE(IDC_EDIT_NAME, &CMyDialog::OnEnChangeEditName)
    ON_LBN_SELCHANGE(IDC_LIST_ITEMS, &CMyDialog::OnLbnSelchangeListItems)
END_MESSAGE_MAP()

// Handler signatures (all void, no parameters)
void CMyDialog::OnBnClickedButtonSave() { }
void CMyDialog::OnEnChangeEditName() { }
void CMyDialog::OnLbnSelchangeListItems() { }
```

#### Command Messages (ON_COMMAND)

```cpp
ON_COMMAND(commandID, &ClassName::HandlerFunction)
```

**WHAT IT'S FOR**: Menu items, toolbar buttons, accelerators

**Example**:
```cpp
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
    ON_COMMAND(ID_FILE_SAVE, &CMainFrame::OnFileSave)
    ON_COMMAND(ID_EDIT_COPY, &CMainFrame::OnEditCopy)
END_MESSAGE_MAP()

void CMainFrame::OnFileOpen()
{
    // Open file dialog
}
```

#### Update UI Messages (ON_UPDATE_COMMAND_UI)

```cpp
ON_UPDATE_COMMAND_UI(commandID, &ClassName::HandlerFunction)
```

**WHAT IT'S FOR**: Enable/disable menu items and toolbar buttons

**Example**:
```cpp
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_COMMAND(ID_EDIT_PASTE, &CMainFrame::OnEditPaste)
    ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CMainFrame::OnUpdateEditPaste)
END_MESSAGE_MAP()

void CMainFrame::OnEditPaste()
{
    // Paste data
}

void CMainFrame::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
    // Enable Paste only if clipboard has data
    BOOL bHasData = IsClipboardFormatAvailable(CF_TEXT);
    pCmdUI->Enable(bHasData);
}
```

#### Custom Messages (ON_MESSAGE)

```cpp
ON_MESSAGE(messageID, &ClassName::HandlerFunction)
```

**WHAT IT'S FOR**: User-defined messages (covered in section 9)

---

## 4. How Message Maps Work Under the Hood

### The Message Flow

```
1. Windows generates message (e.g., user clicks button)
   ↓
2. MFC message loop receives message
   ↓
3. DispatchMessage() sends to WindowProc
   ↓
4. CWnd::WindowProc() calls OnWndMsg()
   ↓
5. OnWndMsg() searches message map
   ↓
6. Message map found? → Call handler function
   │
   ├─ Yes → Call your handler (e.g., OnBnClickedButton1)
   │         Return, message handled
   │
   └─ No → Search base class message map
             ├─ Found? → Call base handler
             └─ Not found? → DefWindowProc (default handling)
```

### The Search Process

**MFC searches message maps in this order**:

```cpp
1. CMyDialog message map       ← Your handlers
2. CDialogEx message map        ← Usually empty (just passes through)
3. CDialog message map          ← Dialog-specific handlers (OnInitDialog, etc.)
4. CWnd message map             ← Window handlers (OnPaint, OnLButtonDown, etc.)
5. CCmdTarget message map       ← Command routing
6. CObject message map          ← No message map (base class)

If no handler found → DefWindowProc (default Windows behavior)
```

### Example: Tracing ON_BN_CLICKED

**When you click a button**:

```cpp
1. Windows generates WM_COMMAND message
   wParam = MAKEWPARAM(IDC_BUTTON1, BN_CLICKED)
   lParam = (HWND of button)

2. Message loop dispatches to dialog's WindowProc

3. CWnd::OnWndMsg() checks message map for WM_COMMAND

4. Finds entry: ON_BN_CLICKED(IDC_BUTTON1, &CMyDialog::OnBnClickedButton1)

5. Checks:
   - Message is WM_COMMAND? ✓
   - Control ID matches IDC_BUTTON1? ✓
   - Notification code is BN_CLICKED? ✓

6. Calls CMyDialog::OnBnClickedButton1()

7. Your function executes
```

### Macro Expansion Example

**What you write**:

```cpp
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CMyDialog::OnBnClickedButton1)
END_MESSAGE_MAP()
```

**What the compiler sees** (simplified):

```cpp
const AFX_MSGMAP* CMyDialog::GetMessageMap() const
{
    return GetThisMessageMap();
}

const AFX_MSGMAP* CMyDialog::GetThisMessageMap()
{
    static const AFX_MSGMAP_ENTRY _messageEntries[] =
    {
        {
            WM_COMMAND,                        // Message
            BN_CLICKED,                        // Notification code
            IDC_BUTTON1,                       // Control ID
            0,                                 // Extra data
            AfxSig_vv,                         // Signature (void, void)
            (AFX_PMSG)(static_cast<void (AFX_MSG_CALL CMyDialog::*)(void)>
                (&CMyDialog::OnBnClickedButton1))  // Function pointer
        },
        { 0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 }  // End marker
    };

    static const AFX_MSGMAP messageMap =
    {
        &CDialogEx::GetThisMessageMap,  // Parent class message map
        &_messageEntries[0]             // This class's entries
    };

    return &messageMap;
}
```

**YOU DON'T NEED TO UNDERSTAND THIS** - but it helps demystify the "magic."

---

## 5. Common Message Map Macros

### Quick Reference Guide

#### Standard Window Messages

| Macro | Windows Message | Handler Signature | Use |
|-------|----------------|-------------------|-----|
| `ON_WM_CREATE()` | WM_CREATE | `int OnCreate(LPCREATESTRUCT lpcs)` | Window created |
| `ON_WM_DESTROY()` | WM_DESTROY | `void OnDestroy()` | Window destroyed |
| `ON_WM_PAINT()` | WM_PAINT | `void OnPaint()` | Repaint window |
| `ON_WM_SIZE()` | WM_SIZE | `void OnSize(UINT nType, int cx, int cy)` | Window resized |
| `ON_WM_MOVE()` | WM_MOVE | `void OnMove(int x, int y)` | Window moved |
| `ON_WM_TIMER()` | WM_TIMER | `void OnTimer(UINT_PTR nIDEvent)` | Timer elapsed |
| `ON_WM_CLOSE()` | WM_CLOSE | `void OnClose()` | Window closing |

#### Mouse Messages

| Macro | Windows Message | Handler Signature |
|-------|----------------|-------------------|
| `ON_WM_LBUTTONDOWN()` | WM_LBUTTONDOWN | `void OnLButtonDown(UINT nFlags, CPoint point)` |
| `ON_WM_LBUTTONUP()` | WM_LBUTTONUP | `void OnLButtonUp(UINT nFlags, CPoint point)` |
| `ON_WM_LBUTTONDBLCLK()` | WM_LBUTTONDBLCLK | `void OnLButtonDblClk(UINT nFlags, CPoint point)` |
| `ON_WM_RBUTTONDOWN()` | WM_RBUTTONDOWN | `void OnRButtonDown(UINT nFlags, CPoint point)` |
| `ON_WM_MOUSEMOVE()` | WM_MOUSEMOVE | `void OnMouseMove(UINT nFlags, CPoint point)` |
| `ON_WM_MOUSEWHEEL()` | WM_MOUSEWHEEL | `BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)` |

**nFlags** can be:
```cpp
MK_CONTROL   // Ctrl key pressed
MK_SHIFT     // Shift key pressed
MK_LBUTTON   // Left mouse button down
MK_RBUTTON   // Right mouse button down
MK_MBUTTON   // Middle mouse button down
```

#### Keyboard Messages

| Macro | Windows Message | Handler Signature |
|-------|----------------|-------------------|
| `ON_WM_KEYDOWN()` | WM_KEYDOWN | `void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)` |
| `ON_WM_KEYUP()` | WM_KEYUP | `void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)` |
| `ON_WM_CHAR()` | WM_CHAR | `void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)` |
| `ON_WM_SYSKEYDOWN()` | WM_SYSKEYDOWN | `void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)` |

**nChar** virtual key codes:
```cpp
VK_RETURN    // Enter
VK_ESCAPE    // Esc
VK_SPACE     // Space
VK_LEFT      // Left arrow
VK_RIGHT     // Right arrow
VK_UP        // Up arrow
VK_DOWN      // Down arrow
VK_F1..VK_F12  // Function keys
'A'..'Z'     // Letter keys
'0'..'9'     // Number keys
```

#### Button Notifications

| Macro | Notification | Handler Signature |
|-------|-------------|-------------------|
| `ON_BN_CLICKED(id, func)` | Button clicked | `void func()` |
| `ON_BN_DOUBLECLICKED(id, func)` | Button double-clicked | `void func()` |
| `ON_BN_SETFOCUS(id, func)` | Button got focus | `void func()` |
| `ON_BN_KILLFOCUS(id, func)` | Button lost focus | `void func()` |

#### Edit Control Notifications

| Macro | Notification | Handler Signature |
|-------|-------------|-------------------|
| `ON_EN_CHANGE(id, func)` | Text changed | `void func()` |
| `ON_EN_UPDATE(id, func)` | About to redraw | `void func()` |
| `ON_EN_SETFOCUS(id, func)` | Got focus | `void func()` |
| `ON_EN_KILLFOCUS(id, func)` | Lost focus | `void func()` |
| `ON_EN_MAXTEXT(id, func)` | Max length reached | `void func()` |

#### List Box Notifications

| Macro | Notification | Handler Signature |
|-------|-------------|-------------------|
| `ON_LBN_SELCHANGE(id, func)` | Selection changed | `void func()` |
| `ON_LBN_DBLCLK(id, func)` | Double-clicked | `void func()` |
| `ON_LBN_SETFOCUS(id, func)` | Got focus | `void func()` |
| `ON_LBN_KILLFOCUS(id, func)` | Lost focus | `void func()` |

#### Combo Box Notifications

| Macro | Notification | Handler Signature |
|-------|-------------|-------------------|
| `ON_CBN_SELCHANGE(id, func)` | Selection changed | `void func()` |
| `ON_CBN_EDITCHANGE(id, func)` | Edit text changed | `void func()` |
| `ON_CBN_DROPDOWN(id, func)` | Dropdown opened | `void func()` |
| `ON_CBN_CLOSEUP(id, func)` | Dropdown closed | `void func()` |
| `ON_CBN_SETFOCUS(id, func)` | Got focus | `void func()` |
| `ON_CBN_KILLFOCUS(id, func)` | Lost focus | `void func()` |

---

## 6. Adding Message Handlers Manually vs Class Wizard

### Using Class Wizard (RECOMMENDED)

**Step-by-Step**:

1. Open your dialog in Resource Editor
2. **Right-click the dialog** (not a control) → **Class Wizard** (or Ctrl+Shift+X)
3. **Messages tab** → Select message (e.g., WM_TIMER)
4. **Click "Add Handler"**
5. Accept default function name or rename
6. Click **OK**

**WHAT CLASS WIZARD DOES**:

```cpp
// Adds to .h file:
afx_msg void OnTimer(UINT_PTR nIDEvent);

// Adds to .cpp message map:
ON_WM_TIMER()

// Creates function in .cpp:
void CMyDialog::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here
    CDialogEx::OnTimer(nIDEvent);
}
```

**For control notifications**:

1. Open Resource Editor
2. **Double-click the control** (e.g., button)
3. Class Wizard opens automatically
4. Select event type (e.g., BN_CLICKED)
5. Click **Add Handler**

### Adding Manually (ADVANCED)

**Only do this if you know what you're doing!**

**Steps**:

1. **Add handler declaration** to .h file:
   ```cpp
   protected:
       afx_msg void OnBnClickedButtonTest();
       DECLARE_MESSAGE_MAP()
   ```

2. **Add message map entry** to .cpp file:
   ```cpp
   BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
       ON_BN_CLICKED(IDC_BUTTON_TEST, &CMyDialog::OnBnClickedButtonTest)
   END_MESSAGE_MAP()
   ```

3. **Implement function** in .cpp file:
   ```cpp
   void CMyDialog::OnBnClickedButtonTest()
   {
       MessageBox(_T("Button clicked!"));
   }
   ```

**COMMON MISTAKES**:
- ❌ Forgetting `afx_msg` keyword
- ❌ Wrong function signature
- ❌ Typo in function name (message map won't match declaration)
- ❌ Forgetting to add to message map
- ❌ Wrong class name in message map entry

**WHY CLASS WIZARD IS BETTER**:
- ✓ Generates correct signatures automatically
- ✓ No typos
- ✓ Adds all three parts correctly
- ✓ Checks for duplicates

---

## 7. Message Routing and Propagation

### Command Routing Order

For **command messages** (menu items, toolbar buttons, accelerators):

```
1. Active view
2. Document attached to view
3. Document template
4. Main frame window
5. Application object
```

**Example**:

```cpp
// Main frame has File → Open menu
class CMainFrame : public CFrameWnd
{
protected:
    afx_msg void OnFileOpen();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
END_MESSAGE_MAP()

void CMainFrame::OnFileOpen()
{
    // This handles ID_FILE_OPEN
}

// But if active view ALSO handles it:
class CMyView : public CView
{
protected:
    afx_msg void OnFileOpen();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMyView, CView)
    ON_COMMAND(ID_FILE_OPEN, &CMyView::OnFileOpen)
END_MESSAGE_MAP()

void CMyView::OnFileOpen()
{
    // This runs INSTEAD of frame's handler (view has priority)

    // To pass to frame:
    GetParentFrame()->SendMessage(WM_COMMAND, ID_FILE_OPEN, 0);
}
```

### Dialog Message Routing

**For dialog-based apps**, routing is simpler:

```
1. Dialog window
2. Application object
```

**Child controls** send notifications to parent dialog:

```cpp
// Button click notification:
Button → Parent Dialog → Application

// Only the parent dialog receives ON_BN_CLICKED
// (unless using reflected messages - see next section)
```

---

## 8. Reflected Messages

### The Problem

Controls send notifications to their **parent**, but sometimes you want the **control itself** to handle its own notifications.

**Traditional**:

```cpp
// Parent dialog handles button click
class CParentDlg : public CDialogEx
{
protected:
    afx_msg void OnBnClickedButton1();  // Button sends to parent
};
```

**Problem**: If you create a custom button class, you can't handle clicks inside the button class - they go to parent.

### The Solution: Reflected Messages

**Reflected messages** bounce notifications back to the control.

```cpp
// Custom button class handles its own click
class CMyButton : public CButton
{
protected:
    afx_msg void OnClicked();  // Handles its own click

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMyButton, CButton)
    ON_CONTROL_REFLECT(BN_CLICKED, &CMyButton::OnClicked)
END_MESSAGE_MAP()

void CMyButton::OnClicked()
{
    MessageBox(_T("I clicked myself!"));
}
```

### Reflection Macros

```cpp
ON_CONTROL_REFLECT(notification, memberFxn)
ON_NOTIFY_REFLECT(wNotifyCode, memberFxn)
ON_UPDATE_COMMAND_UI_REFLECT(memberFxn)
ON_WM_CTLCOLOR_REFLECT()
ON_WM_DRAWITEM_REFLECT()
ON_WM_MEASUREITEM_REFLECT()
```

**Example - Custom Edit Control**:

```cpp
class CMyEdit : public CEdit
{
protected:
    afx_msg void OnChange();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
    ON_CONTROL_REFLECT(EN_CHANGE, &CMyEdit::OnChange)
END_MESSAGE_MAP()

void CMyEdit::OnChange()
{
    // This edit control handles its own text changes
    CString str;
    GetWindowText(str);

    if (str.GetLength() > 10)
    {
        MessageBox(_T("Too long!"));
    }
}
```

**Message Flow with Reflection**:

```
1. User types in edit control
2. Edit control sends EN_CHANGE to parent
3. Parent's message map checked
   ├─ Has ON_EN_CHANGE? → Parent handles it
   └─ No ON_EN_CHANGE? → Reflects back to control
4. Control's message map checked
   └─ Has ON_CONTROL_REFLECT(EN_CHANGE)? → Control handles it
```

---

## 9. Custom Messages

### Why Custom Messages?

**Use cases**:
- Communication between threads
- Asynchronous operations
- Custom control notifications
- Plugin/module communication

### Defining Custom Messages

```cpp
// In header file (e.g., Messages.h)
#define WM_MY_CUSTOM_MESSAGE (WM_USER + 100)
// WM_USER = 0x0400 (first user-defined message)

// Or use WM_APP for application-specific messages
#define WM_MY_APP_MESSAGE (WM_APP + 1)
// WM_APP = 0x8000
```

**Range guidelines**:
- `WM_USER` (0x0400) to 0x7FFF - User-defined messages
- `WM_APP` (0x8000) to 0xBFFF - Application-specific
- 0xC000 to 0xFFFF - String messages (RegisterWindowMessage)

### Handling Custom Messages

```cpp
// In .h file
class CMyDialog : public CDialogEx
{
protected:
    afx_msg LRESULT OnMyCustomMessage(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()
};

// In .cpp file
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_MESSAGE(WM_MY_CUSTOM_MESSAGE, &CMyDialog::OnMyCustomMessage)
END_MESSAGE_MAP()

LRESULT CMyDialog::OnMyCustomMessage(WPARAM wParam, LPARAM lParam)
{
    // wParam and lParam contain your custom data
    int value = (int)wParam;
    CString* pStr = (CString*)lParam;

    MessageBox(*pStr);

    delete pStr;  // Clean up if you allocated on heap

    return 0;  // Return value (usage depends on your design)
}
```

### Sending Custom Messages

```cpp
// From same thread - SendMessage (synchronous)
SendMessage(WM_MY_CUSTOM_MESSAGE, 100, (LPARAM)"Hello");

// From different thread - PostMessage (asynchronous)
PostMessage(WM_MY_CUSTOM_MESSAGE, 100, (LPARAM)new CString(_T("Hello")));
```

### Complete Example: Progress Notification

```cpp
// Messages.h
#define WM_UPDATE_PROGRESS (WM_USER + 1)

// WorkerThread.cpp
void WorkerThread(HWND hWnd)
{
    for (int i = 0; i <= 100; i++)
    {
        // Do work...
        Sleep(100);

        // Send progress update to main window
        ::PostMessage(hWnd, WM_UPDATE_PROGRESS, i, 0);
    }
}

// MyDialog.h
class CMyDialog : public CDialogEx
{
protected:
    afx_msg LRESULT OnUpdateProgress(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
};

// MyDialog.cpp
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_MESSAGE(WM_UPDATE_PROGRESS, &CMyDialog::OnUpdateProgress)
END_MESSAGE_MAP()

LRESULT CMyDialog::OnUpdateProgress(WPARAM wParam, LPARAM lParam)
{
    int progress = (int)wParam;

    // Update progress bar
    CProgressCtrl* pProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
    pProgress->SetPos(progress);

    // Update text
    CString str;
    str.Format(_T("Progress: %d%%"), progress);
    SetDlgItemText(IDC_STATIC_PROGRESS, str);

    return 0;
}
```

---

## 10. Message Maps vs Virtual Functions

### When to Use Each

#### Use Message Maps For:

```cpp
// Standard Windows messages
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_TIMER()

// Control notifications
ON_BN_CLICKED(IDC_BUTTON1, ...)
ON_EN_CHANGE(IDC_EDIT1, ...)

// Menu/toolbar commands
ON_COMMAND(ID_FILE_OPEN, ...)

// Custom messages
ON_MESSAGE(WM_MY_MESSAGE, ...)
```

#### Use Virtual Functions For:

```cpp
// Dialog lifecycle
virtual BOOL OnInitDialog()      // Initialization
virtual void OnOK()              // OK button
virtual void OnCancel()          // Cancel button
virtual void DoDataExchange()    // DDX/DDV

// Application lifecycle
virtual BOOL InitInstance()      // App startup
virtual int ExitInstance()       // App shutdown

// View functions
virtual void OnDraw()            // Drawing
virtual void OnUpdate()          // View update
```

### Why the Difference?

**Virtual functions**:
- Designed to be overridden
- Part of class design
- Often require calling base class
- Compiler-enforced signature

**Message maps**:
- Handle Windows messages
- Can be added/removed easily
- Don't exist in base class necessarily
- Macro-based

### Comparison Example

```cpp
// Virtual function approach
class CDialog
{
public:
    virtual BOOL OnInitDialog()  // Designed to be overridden
    {
        // Base implementation
        return TRUE;
    }
};

class CMyDialog : public CDialog
{
    virtual BOOL OnInitDialog() override  // Override
    {
        CDialog::OnInitDialog();  // Call base
        // Your code
        return TRUE;
    }
};

// Message map approach
class CMyDialog : public CDialogEx
{
protected:
    afx_msg void OnBnClickedButton1();  // Not virtual
    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CMyDialog::OnBnClickedButton1)
    // CDialogEx doesn't have OnBnClickedButton1 - it's specific to YOUR dialog
END_MESSAGE_MAP()
```

---

## 11. Complete Real-World Examples

### Example 1: Mouse Drawing Application

```cpp
// MyDrawDialog.h
class CMyDrawDialog : public CDialogEx
{
public:
    CMyDrawDialog(CWnd* pParent = nullptr);

protected:
    CArray<CPoint> m_points;  // Store mouse positions
    BOOL m_bDrawing;          // Currently drawing?

    virtual BOOL OnInitDialog();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg void OnBnClickedButtonClear();

    DECLARE_MESSAGE_MAP()
};

// MyDrawDialog.cpp
BEGIN_MESSAGE_MAP(CMyDrawDialog, CDialogEx)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CMyDrawDialog::OnBnClickedButtonClear)
END_MESSAGE_MAP()

CMyDrawDialog::CMyDrawDialog(CWnd* pParent)
    : CDialogEx(IDD_MYDRAW_DIALOG, pParent)
    , m_bDrawing(FALSE)
{
}

BOOL CMyDrawDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    return TRUE;
}

void CMyDrawDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bDrawing = TRUE;
    m_points.RemoveAll();
    m_points.Add(point);

    SetCapture();  // Capture mouse even if it leaves window

    CDialogEx::OnLButtonDown(nFlags, point);
}

void CMyDrawDialog::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bDrawing)
    {
        m_bDrawing = FALSE;
        ReleaseCapture();
    }

    CDialogEx::OnLButtonUp(nFlags, point);
}

void CMyDrawDialog::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bDrawing)
    {
        m_points.Add(point);
        Invalidate();  // Request repaint
    }

    CDialogEx::OnMouseMove(nFlags, point);
}

void CMyDrawDialog::OnPaint()
{
    CPaintDC dc(this);

    if (m_points.GetCount() > 1)
    {
        // Draw lines connecting all points
        dc.MoveTo(m_points[0]);
        for (int i = 1; i < m_points.GetCount(); i++)
        {
            dc.LineTo(m_points[i]);
        }
    }
}

void CMyDrawDialog::OnBnClickedButtonClear()
{
    m_points.RemoveAll();
    Invalidate();
}
```

### Example 2: Keyboard Shortcuts

```cpp
// MyDialog.h
class CMyDialog : public CDialogEx
{
protected:
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

    DECLARE_MESSAGE_MAP()
};

// MyDialog.cpp
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_WM_KEYDOWN()
    ON_WM_CHAR()
END_MESSAGE_MAP()

void CMyDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // Handle special keys
    switch (nChar)
    {
    case VK_F1:
        MessageBox(_T("Help pressed!"));
        break;

    case VK_F5:
        MessageBox(_T("Refresh!"));
        break;

    case VK_ESCAPE:
        // Don't close dialog on Esc (override default)
        return;  // Don't call base class

    case 'S':
        if (GetKeyState(VK_CONTROL) & 0x8000)  // Ctrl+S
        {
            MessageBox(_T("Save!"));
            return;
        }
        break;
    }

    CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMyDialog::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // Handle character input
    TRACE(_T("Character: %c\n"), nChar);

    CDialogEx::OnChar(nChar, nRepCnt, nFlags);
}
```

### Example 3: Timer-Based Animation

```cpp
// MyDialog.h
class CMyDialog : public CDialogEx
{
protected:
    int m_nPosition;
    UINT_PTR m_nTimerID;

    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedButtonStart();
    afx_msg void OnBnClickedButtonStop();

    DECLARE_MESSAGE_MAP()
};

// MyDialog.cpp
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_WM_DESTROY()
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BUTTON_START, &CMyDialog::OnBnClickedButtonStart)
    ON_BN_CLICKED(IDC_BUTTON_STOP, &CMyDialog::OnBnClickedButtonStop)
END_MESSAGE_MAP()

BOOL CMyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_nPosition = 0;
    m_nTimerID = 0;

    return TRUE;
}

void CMyDialog::OnDestroy()
{
    if (m_nTimerID != 0)
    {
        KillTimer(m_nTimerID);
        m_nTimerID = 0;
    }

    CDialogEx::OnDestroy();
}

void CMyDialog::OnBnClickedButtonStart()
{
    if (m_nTimerID == 0)
    {
        // Start timer - fires every 100ms
        m_nTimerID = SetTimer(1, 100, NULL);
    }
}

void CMyDialog::OnBnClickedButtonStop()
{
    if (m_nTimerID != 0)
    {
        KillTimer(m_nTimerID);
        m_nTimerID = 0;
    }
}

void CMyDialog::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == m_nTimerID)
    {
        // Update position
        m_nPosition += 5;
        if (m_nPosition > 200)
            m_nPosition = 0;

        // Update display
        CString str;
        str.Format(_T("Position: %d"), m_nPosition);
        SetDlgItemText(IDC_STATIC_POS, str);

        // Move a control
        CWnd* pCtrl = GetDlgItem(IDC_STATIC_MOVING);
        if (pCtrl != nullptr)
        {
            pCtrl->SetWindowPos(NULL, m_nPosition, 50, 0, 0,
                               SWP_NOSIZE | SWP_NOZORDER);
        }
    }

    CDialogEx::OnTimer(nIDEvent);
}
```

---

## 12. Debugging Message Maps

### Common Problems

#### Problem 1: Handler Not Called

**Symptom**: You click a button, nothing happens

**Debugging**:

```cpp
// 1. Set breakpoint in handler
void CMyDialog::OnBnClickedButton1()
{
    MessageBox(_T("Test"));  // ← Breakpoint here
}

// 2. Click button
// 3. Breakpoint doesn't hit?

// CHECK:
// a) Is control ID correct in message map?
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CMyDialog::OnBnClickedButton1)
    //            ↑ Must match actual control ID in Resource Editor
END_MESSAGE_MAP()

// b) Is handler declared in .h file?
afx_msg void OnBnClickedButton1();  // Must be declared

// c) Is class name correct in message map entry?
ON_BN_CLICKED(IDC_BUTTON1, &CMyDialog::OnBnClickedButton1)
//                          ↑ Must be YOUR class name
```

#### Problem 2: Wrong Signature

**Symptom**: Compiler error or crash

```cpp
// WRONG signatures:
void OnLButtonDown();  // ← Missing parameters
void OnLButtonDown(int x, int y);  // ← Wrong parameter types
int OnLButtonDown(UINT nFlags, CPoint point);  // ← Wrong return type

// CORRECT:
void OnLButtonDown(UINT nFlags, CPoint point);
```

**HOW TO FIND CORRECT SIGNATURE**:
1. Right-click in editor
2. Class Wizard
3. Messages tab
4. Select message
5. Look at generated function

#### Problem 3: Multiple Handlers

**Symptom**: First handler works, second doesn't

```cpp
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CMyDialog::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON1, &CMyDialog::OnBnClickedButton1Alternative)
    // ↑ ONLY FIRST ONE RUNS
END_MESSAGE_MAP()
```

**SOLUTION**: Remove duplicate entries

#### Problem 4: Base Class Not Called

**Symptom**: Dialog doesn't close, controls don't work

```cpp
// WRONG - forgetting base class
BOOL CMyDialog::OnInitDialog()
{
    // CDialogEx::OnInitDialog();  ← FORGOT THIS

    SetDlgItemText(IDC_EDIT1, _T("Hello"));
    return TRUE;
}
```

**SOLUTION**: Always call base class first in OnInitDialog and DoDataExchange

### Debugging Tools

#### TRACE Macro

```cpp
void CMyDialog::OnBnClickedButton1()
{
    TRACE(_T("OnBnClickedButton1 called\n"));

    CString str;
    GetDlgItemText(IDC_EDIT1, str);
    TRACE(_T("Edit box contains: %s\n"), str);
}
```

**View Output**: Debug → Windows → Output (Ctrl+Alt+O)

#### Assert Macro

```cpp
void CMyDialog::OnBnClickedButton1()
{
    CWnd* pEdit = GetDlgItem(IDC_EDIT1);
    ASSERT(pEdit != nullptr);  // ← Breaks if NULL in debug builds

    if (pEdit != nullptr)
    {
        pEdit->SetWindowText(_T("Hello"));
    }
}
```

---

## Summary

### Key Takeaways

1. **Message maps** connect Windows messages to C++ functions
2. **Three parts**: DECLARE (in .h), BEGIN/END (in .cpp), handler implementations
3. **Class Wizard** is the safe way to add handlers
4. **Message routing**: Searches current class, then base classes
5. **Reflected messages** let controls handle their own notifications
6. **Custom messages** enable custom communication
7. **Virtual functions** for designed overrides, message maps for Windows messages

### Message Map Checklist

When adding a message handler:

- [ ] Handler declared in .h file with `afx_msg`
- [ ] Correct signature (use Class Wizard to ensure)
- [ ] Entry in message map (BEGIN_MESSAGE_MAP ... END_MESSAGE_MAP)
- [ ] Correct control ID or message constant
- [ ] Function implemented in .cpp file
- [ ] Base class called if needed

### Quick Reference

```cpp
// In .h file
class CMyDialog : public CDialogEx
{
protected:
    afx_msg void OnBnClickedButton1();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT_PTR nIDEvent);

    DECLARE_MESSAGE_MAP()
};

// In .cpp file
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CMyDialog::OnBnClickedButton1)
    ON_WM_LBUTTONDOWN()
    ON_WM_TIMER()
END_MESSAGE_MAP()

void CMyDialog::OnBnClickedButton1()
{
    // Handler code
}

void CMyDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
    // Handler code
    CDialogEx::OnLButtonDown(nFlags, point);
}

void CMyDialog::OnTimer(UINT_PTR nIDEvent)
{
    // Handler code
    CDialogEx::OnTimer(nIDEvent);
}
```

### Next Steps

In **Lesson 05: MFC Application Wizard - Understanding Generated Code**, you'll learn:
- Every file the wizard generates and WHY
- WHAT each section of code does
- WHERE to add your code safely
- HOW to customize wizard-generated projects
- Understanding precompiled headers and framework files

---

**Next Lesson**: [Lesson 05: MFC Application Wizard - Understanding Generated Code](Lesson-05-MFC-Wizard-Generated-Code.md)
