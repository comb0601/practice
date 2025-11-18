# Lesson 12: Common Dialogs (File, Color, Font)

**Duration**: 6 hours
**Difficulty**: Beginner to Intermediate

## Table of Contents
1. File Open/Save Dialogs (CFileDialog)
2. Color Picker Dialog (CColorDialog)
3. Font Picker Dialog (CFontDialog)
4. Customizing Common Dialogs
5. Complete Examples

---

## 1. File Open/Save Dialogs (CFileDialog)

### Basic File Open Dialog

```cpp
void CMyDialog::OnBnClickedButtonOpen()
{
    CFileDialog dlg(TRUE,  // TRUE = Open, FALSE = Save
                   _T("txt"),  // Default extension
                   NULL,  // Default filename
                   OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,  // Flags
                   _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));  // Filter

    if (dlg.DoModal() == IDOK)
    {
        CString strPath = dlg.GetPathName();  // Full path
        CString strFileName = dlg.GetFileName();  // Filename only

        // Open file
        CFile file;
        if (file.Open(strPath, CFile::modeRead))
        {
            // Read file
            ULONGLONG nLength = file.GetLength();
            char* pBuffer = new char[(size_t)nLength + 1];
            file.Read(pBuffer, (UINT)nLength);
            pBuffer[nLength] = '\0';

            CString strContent(pBuffer);
            SetDlgItemText(IDC_EDIT_CONTENT, strContent);

            delete[] pBuffer;
            file.Close();
        }
    }
}
```

### File Save Dialog

```cpp
void CMyDialog::OnBnClickedButtonSave()
{
    CFileDialog dlg(FALSE,  // FALSE = Save dialog
                   _T("txt"),
                   _T("untitled.txt"),  // Default filename
                   OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                   _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));

    if (dlg.DoModal() == IDOK)
    {
        CString strPath = dlg.GetPathName();

        // Save file
        CFile file;
        if (file.Open(strPath, CFile::modeCreate | CFile::modeWrite))
        {
            CString strContent;
            GetDlgItemText(IDC_EDIT_CONTENT, strContent);

            file.Write((LPCTSTR)strContent, strContent.GetLength() * sizeof(TCHAR));
            file.Close();

            MessageBox(_T("File saved successfully!"));
        }
    }
}
```

### File Dialog Flags

```cpp
OFN_READONLY              // Read-only checkbox
OFN_OVERWRITEPROMPT       // Warn if file exists (Save dialog)
OFN_HIDEREADONLY          // Hide read-only checkbox
OFN_NOCHANGEDIR           // Don't change current directory
OFN_FILEMUSTEXIST         // File must exist (Open dialog)
OFN_PATHMUSTEXIST         // Path must exist
OFN_ALLOWMULTISELECT      // Allow multiple file selection
OFN_EXPLORER              // Windows Explorer style
OFN_NOVALIDATE            // Don't validate filename
```

### Multiple File Selection

```cpp
void CMyDialog::OnBnClickedButtonOpenMultiple()
{
    CFileDialog dlg(TRUE, NULL, NULL,
                   OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST,
                   _T("All Files (*.*)|*.*||"));

    // Allocate buffer for multiple filenames
    TCHAR szFiles[10000];
    szFiles[0] = '\0';
    dlg.m_ofn.lpstrFile = szFiles;
    dlg.m_ofn.nMaxFile = sizeof(szFiles) / sizeof(TCHAR);

    if (dlg.DoModal() == IDOK)
    {
        POSITION pos = dlg.GetStartPosition();
        while (pos != NULL)
        {
            CString strPath = dlg.GetNextPathName(pos);
            // Process each file
            ProcessFile(strPath);
        }
    }
}
```

---

## 2. Color Picker Dialog (CColorDialog)

### Basic Color Dialog

```cpp
void CMyDialog::OnBnClickedButtonPickColor()
{
    CColorDialog dlg(RGB(255, 0, 0),  // Initial color (red)
                    CC_FULLOPEN);      // Show full dialog

    if (dlg.DoModal() == IDOK)
    {
        COLORREF color = dlg.GetColor();

        // Use color
        m_brushBackground.DeleteObject();
        m_brushBackground.CreateSolidBrush(color);
        Invalidate();  // Repaint dialog
    }
}
```

### Color Dialog Flags

```cpp
CC_ANYCOLOR              // Allow any color
CC_FULLOPEN              // Show full dialog (custom colors)
CC_PREVENTFULLOPEN       // Hide custom color section
CC_RGBINIT              // Use initial color
CC_SHOWHELP             // Show Help button
CC_SOLIDCOLOR           // Only solid colors
```

### Using Selected Color

```cpp
class CMyDialog : public CDialogEx
{
private:
    COLORREF m_clrBackground;
    CBrush m_brushBackground;

protected:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnBnClickedButtonColor();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_COLOR, &CMyDialog::OnBnClickedButtonColor)
END_MESSAGE_MAP()

void CMyDialog::OnBnClickedButtonColor()
{
    CColorDialog dlg(m_clrBackground, CC_FULLOPEN);

    if (dlg.DoModal() == IDOK)
    {
        m_clrBackground = dlg.GetColor();
        m_brushBackground.DeleteObject();
        m_brushBackground.CreateSolidBrush(m_clrBackground);
        Invalidate();
    }
}

HBRUSH CMyDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    if (nCtlColor == CTLCOLOR_DLG)
    {
        return m_brushBackground;
    }

    return hbr;
}
```

---

## 3. Font Picker Dialog (CFontDialog)

### Basic Font Dialog

```cpp
void CMyDialog::OnBnClickedButtonFont()
{
    CFontDialog dlg;

    if (dlg.DoModal() == IDOK)
    {
        // Get font characteristics
        CString strFontName = dlg.GetFaceName();
        int nSize = dlg.GetSize() / 10;  // Size in points
        COLORREF color = dlg.GetColor();
        BOOL bBold = dlg.IsBold();
        BOOL bItalic = dlg.IsItalic();
        BOOL bUnderline = dlg.IsUnderline();
        BOOL bStrikeOut = dlg.IsStrikeOut();

        // Create font
        m_font.DeleteObject();
        m_font.CreatePointFont(dlg.GetSize(), strFontName);

        // Apply to edit control
        CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TEXT);
        if (pEdit != nullptr)
        {
            pEdit->SetFont(&m_font);
        }
    }
}
```

### Font Dialog with Initial Settings

```cpp
void CMyDialog::OnBnClickedButtonFont()
{
    // Get current font from edit control
    CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TEXT);
    CFont* pFont = pEdit->GetFont();

    LOGFONT lf;
    pFont->GetLogFont(&lf);

    CFontDialog dlg(&lf,  // Initial font
                   CF_EFFECTS | CF_SCREENFONTS);

    if (dlg.DoModal() == IDOK)
    {
        dlg.GetCurrentFont(&lf);

        m_font.DeleteObject();
        m_font.CreateFontIndirect(&lf);

        pEdit->SetFont(&m_font);
    }
}
```

### Font Dialog Flags

```cpp
CF_BOTH                  // Screen and printer fonts
CF_SCREENFONTS          // Screen fonts only
CF_PRINTERFONTS         // Printer fonts only
CF_EFFECTS              // Show strikeout, underline, color
CF_FIXEDPITCHONLY       // Fixed-pitch fonts only
CF_NOSCRIPTSEL          // Don't show script selection
CF_NOVECTORFONTS        // No vector fonts
CF_TTONLY               // TrueType fonts only
```

---

## 4. Customizing Common Dialogs

### Subclassing CFileDialog

```cpp
class CMyFileDialog : public CFileDialog
{
public:
    CMyFileDialog(BOOL bOpenFileDialog);

protected:
    virtual void OnInitDone();
    virtual void OnFileNameChange();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CMyFileDialog, CFileDialog)
END_MESSAGE_MAP()

CMyFileDialog::CMyFileDialog(BOOL bOpenFileDialog)
    : CFileDialog(bOpenFileDialog)
{
}

void CMyFileDialog::OnInitDone()
{
    // Dialog is initialized
    // Can add custom controls here

    CFileDialog::OnInitDone();
}

void CMyFileDialog::OnFileNameChange()
{
    // Selection changed
    CString strPath = GetPathName();
    TRACE(_T("Selected: %s\n"), strPath);

    CFileDialog::OnFileNameChange();
}

// Usage
void CMyDialog::OnButtonOpen()
{
    CMyFileDialog dlg(TRUE);
    if (dlg.DoModal() == IDOK)
    {
        // Use file
    }
}
```

---

## 5. Complete Examples

### Example: Text Editor with File Operations

```cpp
class CTextEditorDialog : public CDialogEx
{
private:
    CString m_strCurrentFile;
    BOOL m_bModified;

protected:
    afx_msg void OnBnClickedButtonNew();
    afx_msg void OnBnClickedButtonOpen();
    afx_msg void OnBnClickedButtonSave();
    afx_msg void OnBnClickedButtonSaveAs();
    afx_msg void OnEnChangeEditText();

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CTextEditorDialog, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_NEW, &CTextEditorDialog::OnBnClickedButtonNew)
    ON_BN_CLICKED(IDC_BUTTON_OPEN, &CTextEditorDialog::OnBnClickedButtonOpen)
    ON_BN_CLICKED(IDC_BUTTON_SAVE, &CTextEditorDialog::OnBnClickedButtonSave)
    ON_BN_CLICKED(IDC_BUTTON_SAVE_AS, &CTextEditorDialog::OnBnClickedButtonSaveAs)
    ON_EN_CHANGE(IDC_EDIT_TEXT, &CTextEditorDialog::OnEnChangeEditText)
END_MESSAGE_MAP()

void CTextEditorDialog::OnBnClickedButtonNew()
{
    if (m_bModified)
    {
        int result = MessageBox(_T("Save changes?"), _T("Confirm"),
                               MB_YESNOCANCEL | MB_ICONQUESTION);
        if (result == IDYES)
        {
            OnBnClickedButtonSave();
        }
        else if (result == IDCANCEL)
        {
            return;
        }
    }

    SetDlgItemText(IDC_EDIT_TEXT, _T(""));
    m_strCurrentFile = _T("");
    m_bModified = FALSE;
}

void CTextEditorDialog::OnBnClickedButtonOpen()
{
    CFileDialog dlg(TRUE, _T("txt"), NULL,
                   OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
                   _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));

    if (dlg.DoModal() == IDOK)
    {
        CStdioFile file;
        if (file.Open(dlg.GetPathName(), CFile::modeRead | CFile::typeText))
        {
            CString strContent, strLine;
            while (file.ReadString(strLine))
            {
                strContent += strLine + _T("\r\n");
            }
            file.Close();

            SetDlgItemText(IDC_EDIT_TEXT, strContent);
            m_strCurrentFile = dlg.GetPathName();
            m_bModified = FALSE;
        }
    }
}

void CTextEditorDialog::OnBnClickedButtonSave()
{
    if (m_strCurrentFile.IsEmpty())
    {
        OnBnClickedButtonSaveAs();
        return;
    }

    CStdioFile file;
    if (file.Open(m_strCurrentFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
    {
        CString strContent;
        GetDlgItemText(IDC_EDIT_TEXT, strContent);

        file.WriteString(strContent);
        file.Close();

        m_bModified = FALSE;
        MessageBox(_T("File saved!"));
    }
}

void CTextEditorDialog::OnBnClickedButtonSaveAs()
{
    CFileDialog dlg(FALSE, _T("txt"), _T("untitled.txt"),
                   OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                   _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));

    if (dlg.DoModal() == IDOK)
    {
        m_strCurrentFile = dlg.GetPathName();
        OnBnClickedButtonSave();
    }
}

void CTextEditorDialog::OnEnChangeEditText()
{
    m_bModified = TRUE;
}
```

---

## Summary

### Key Takeaways

1. **CFileDialog**: File open/save with filters
2. **CColorDialog**: Color picker with custom colors
3. **CFontDialog**: Font selection with preview
4. **Common dialog flags**: Customize behavior
5. **Subclassing**: Extend common dialogs

### Next Steps

In **Lesson 13: Multiple Dialogs and Navigation**, you'll learn:
- Creating child dialogs
- Passing data between dialogs
- Dialog navigation
- Modeless dialogs

---

**Next Lesson**: [Lesson 13: Multiple Dialogs and Navigation](Lesson-13-Multiple-Dialogs-Navigation.md)
