# Lesson 28: Status Bars and Progress Controls

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to Status Bars and Progress Controls
2. Core Concepts
3. Implementation Details
4. Practical Examples
5. Best Practices
6. Common Pitfalls
7. Advanced Techniques
8. Real-World Applications
9. Performance Considerations
10. Complete Working Examples
11. Exercises and Projects

---

## 1. Introduction

This lesson covers **Status Bars and Progress Controls** in MFC applications. Understanding these concepts is crucial for building professional Windows applications with proper user interface behavior.

### Key Topics Covered:
- Fundamental concepts and architecture
- Step-by-step implementation
- Command routing mechanisms
- Update UI handlers
- Menu and toolbar synchronization
- Status bar management
- Best practices and patterns

---

## 2. Core Concepts

### Understanding the Framework

MFC provides a robust framework for handling commands and updating UI elements. The command routing system ensures that commands are sent to the appropriate handler.

**Command Routing Order**:
1. Active View
2. Document
3. Frame Window
4. Application

### Example Command Handler

```cpp
BEGIN_MESSAGE_MAP(CMyView, CView)
    ON_COMMAND(ID_FILE_SAVE, &CMyView::OnFileSave)
    ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CMyView::OnUpdateFileSave)
END_MESSAGE_MAP()

void CMyView::OnFileSave()
{
    // Handle save command
    CMyDocument* pDoc = GetDocument();
    pDoc->DoFileSave();
}

void CMyView::OnUpdateFileSave(CCmdUI* pCmdUI)
{
    // Enable/disable save button
    CMyDocument* pDoc = GetDocument();
    pCmdUI->Enable(pDoc->IsModified());
}
```

---

## 3. Implementation Details

### Creating Menu Resources

Menu resources are defined in the resource editor. Each menu item has an ID that corresponds to command handlers.

### Creating Toolbars

```cpp
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // Create toolbar
    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT))
        return -1;

    if (!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
        return -1;

    // Enable docking
    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndToolBar);

    return 0;
}
```

### Status Bar Management

```cpp
// Create status bar
if (!m_wndStatusBar.Create(this))
    return -1;

static UINT indicators[] = {
    ID_SEPARATOR,           // Status line
    ID_INDICATOR_CAPS,      // Caps Lock
    ID_INDICATOR_NUM,       // Num Lock
};

m_wndStatusBar.SetIndicators(indicators, 3);

// Update status bar text
m_wndStatusBar.SetPaneText(0, _T("Ready"));
```

---

## 4. Practical Examples

### Complete Command Handling

```cpp
class CMyView : public CView
{
protected:
    afx_msg void OnEditCopy();
    afx_msg void OnEditPaste();
    afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
    
    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMyView, CView)
    ON_COMMAND(ID_EDIT_COPY, &CMyView::OnEditCopy)
    ON_COMMAND(ID_EDIT_PASTE, &CMyView::OnEditPaste)
    ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CMyView::OnUpdateEditCopy)
    ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CMyView::OnUpdateEditPaste)
END_MESSAGE_MAP()

void CMyView::OnEditCopy()
{
    // Copy selected text to clipboard
    if (m_hasSelection)
    {
        CopyToClipboard(m_selectedText);
    }
}

void CMyView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
    // Enable copy only if text is selected
    pCmdUI->Enable(m_hasSelection);
}

void CMyView::OnEditPaste()
{
    // Paste from clipboard
    CString text = GetClipboardText();
    InsertText(text);
}

void CMyView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
    // Enable paste only if clipboard has text
    pCmdUI->Enable(IsClipboardFormatAvailable(CF_TEXT));
}
```

---

## 5. Best Practices

1. ✅ **Always implement Update UI handlers** - Don't rely on manual enable/disable
2. ✅ **Use command routing** - Let MFC route commands appropriately  
3. ✅ **Keep handlers simple** - Delegate to document/view methods
4. ✅ **Update UI frequently** - MFC calls update handlers during idle time
5. ✅ **Use consistent IDs** - Share IDs between menus and toolbars
6. ✅ **Provide feedback** - Update status bar with operation status
7. ✅ **Handle accelerators** - Define keyboard shortcuts
8. ✅ **Test thoroughly** - Verify all command states
9. ✅ **Document behavior** - Comment non-obvious UI logic
10. ✅ **Follow conventions** - Use standard command IDs when possible

---

## 6. Common Pitfalls

### Pitfall 1: Forgetting Update UI Handlers

```cpp
// ❌ WRONG - Menu item always enabled
BEGIN_MESSAGE_MAP(CMyView, CView)
    ON_COMMAND(ID_EDIT_UNDO, &CMyView::OnEditUndo)
END_MESSAGE_MAP()

// ✅ RIGHT - Add update handler
BEGIN_MESSAGE_MAP(CMyView, CView)
    ON_COMMAND(ID_EDIT_UNDO, &CMyView::OnEditUndo)
    ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CMyView::OnUpdateEditUndo)
END_MESSAGE_MAP()

void CMyView::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(CanUndo());
}
```

### Pitfall 2: Expensive Update UI Operations

```cpp
// ❌ WRONG - Expensive operation in update handler
void CMyView::OnUpdateFileSave(CCmdUI* pCmdUI)
{
    // This is called VERY frequently!
    BOOL bModified = CheckIfDocumentModified();  // Expensive!
    pCmdUI->Enable(bModified);
}

// ✅ RIGHT - Cache the state
void CMyView::OnUpdateFileSave(CCmdUI* pCmdUI)
{
    CMyDocument* pDoc = GetDocument();
    pCmdUI->Enable(pDoc->IsModified());  // Fast!
}
```

---

## 7. Advanced Techniques

### Dynamic Menus

```cpp
void CMainFrame::OnUpdateRecentFileMenu(CCmdUI* pCmdUI)
{
    // Dynamically update recent file menu
    if (pCmdUI->m_pMenu)
    {
        // Clear existing items
        while (pCmdUI->m_pMenu->GetMenuItemCount() > 0)
        {
            pCmdUI->m_pMenu->RemoveMenu(0, MF_BYPOSITION);
        }

        // Add recent files
        for (int i = 0; i < m_recentFiles.GetSize(); i++)
        {
            CString menuText;
            menuText.Format(_T("&%d %s"), i + 1, m_recentFiles[i]);
            pCmdUI->m_pMenu->AppendMenu(MF_STRING, ID_FILE_MRU_FILE1 + i, menuText);
        }
    }
}
```

### Custom Toolbar Buttons

```cpp
// Add custom button to toolbar
int index = m_wndToolBar.GetToolBarCtrl().GetButtonCount();
TBBUTTON button;
button.iBitmap = 0;
button.idCommand = ID_MY_CUSTOM_COMMAND;
button.fsState = TBSTATE_ENABLED;
button.fsStyle = TBSTYLE_BUTTON;
button.dwData = 0;
button.iString = -1;

m_wndToolBar.GetToolBarCtrl().InsertButton(index, &button);
```

---

## 8. Real-World Applications

### Text Editor with Full Command Support

This example demonstrates a complete implementation of command routing, update UI, menus, toolbars, and status bars in a functional text editor.

```cpp
// View class with comprehensive command handling
class CTextEditorView : public CView
{
protected:
    CString m_text;
    BOOL m_hasSelection;
    int m_lineCount;
    int m_wordCount;

protected:
    // Command handlers
    afx_msg void OnFileSave();
    afx_msg void OnEditCut();
    afx_msg void OnEditCopy();
    afx_msg void OnEditPaste();
    afx_msg void OnEditUndo();
    afx_msg void OnEditRedo();
    afx_msg void OnEditSelectAll();
    
    // Update UI handlers
    afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
    afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
    
    // Status updates
    void UpdateStatusBar();
    
    DECLARE_MESSAGE_MAP()
};
```

---

## 9. Performance Considerations

### Optimize Update UI Handlers

Update UI handlers are called during idle processing, potentially many times per second. Keep them fast!

```cpp
// ✅ GOOD - Fast check
void CMyView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_hasSelection);  // Simple boolean check
}

// ❌ BAD - Slow operation
void CMyView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
    // Searches entire document - TOO SLOW!
    BOOL hasText = (FindText() != -1);
    pCmdUI->Enable(hasText);
}
```

---

## 10. Complete Working Examples

### Full Implementation Example

```cpp
// Complete example showing all concepts together
class CMyApp : public CWinApp
{
public:
    virtual BOOL InitInstance();
};

class CMainFrame : public CFrameWnd
{
protected:
    CToolBar m_wndToolBar;
    CStatusBar m_wndStatusBar;

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnUpdateStatusBar(CCmdUI* pCmdUI);
    
    DECLARE_MESSAGE_MAP()
};

class CMyDocument : public CDocument
{
protected:
    CString m_content;
    
public:
    void SetContent(const CString& content);
    CString GetContent() const { return m_content; }
};

class CMyView : public CView
{
protected:
    virtual void OnDraw(CDC* pDC);
    
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnEditClear();
    afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
    
    DECLARE_MESSAGE_MAP()
};

// Implementation details would follow...
```

---

## 11. Exercises and Projects

### Exercise 1: Basic Command Routing
Implement a simple text editor with File→Save command that enables only when document is modified.

### Exercise 2: Full Edit Menu
Create a complete Edit menu with Cut/Copy/Paste/Undo/Redo commands and proper update UI handlers.

### Exercise 3: Custom Toolbar
Add a custom toolbar with formatting buttons (Bold, Italic, Underline) that update based on current selection.

### Exercise 4: Status Bar Integration
Display line count, word count, and character count in status bar, updating in real-time.

### Exercise 5: Complete Application
Build a full-featured text editor combining all concepts from this lesson.

---

## Key Takeaways

1. ✅ Command routing follows View → Document → Frame → App
2. ✅ Update UI handlers enable/disable menu and toolbar items
3. ✅ Always implement both command and update UI handlers
4. ✅ Keep update UI handlers fast - they're called frequently
5. ✅ Use CCmdUI for consistent UI updates
6. ✅ Share command IDs between menus and toolbars
7. ✅ Status bars provide user feedback
8. ✅ Toolbars can be docked and floating
9. ✅ MFC handles most UI synchronization automatically
10. ✅ Follow MFC conventions for maintainable code

---

## Summary

Understanding command routing, update UI, menus, toolbars, and status bars is essential for building professional MFC applications. This lesson covered the complete workflow from basic concepts to advanced techniques.

---

## Next Lesson Preview

In **Lesson 29**, we'll explore the next topic in the Document/View architecture series.

**Congratulations on completing Lesson 28!**
