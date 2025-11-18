# Lesson 34: Rich Edit Control

**Duration**: 8 hours
**Difficulty**: Advanced

## Table of Contents
1. Introduction to Rich Edit Controls
2. Creating and Initializing Rich Edit Controls
3. Text Formatting and Styling
4. Character and Paragraph Formatting
5. RTF Document Handling
6. Find and Replace Operations
7. Undo/Redo Functionality
8. Clipboard Operations
9. Printing Rich Edit Content
10. Complete Rich Text Editor Application

---

## 1. Introduction to Rich Edit Controls

The Rich Edit control (`CRichEditCtrl`) is a powerful text editing control that supports:
- Multiple fonts, colors, and styles
- RTF (Rich Text Format) documents
- Embedded objects (OLE)
- Advanced text formatting
- Find and replace
- Unlimited undo/redo
- URL detection
- Event notifications

### Rich Edit Versions:
- **RICHED20.DLL** (Rich Edit 2.0) - Windows 98+
- **RICHED32.DLL** (Rich Edit 1.0) - Legacy
- **MSFTEDIT.DLL** (Rich Edit 4.1+) - Windows XP SP1+

---

## 2. Creating and Initializing Rich Edit Controls

### Basic Setup

```cpp
// RichEditDlg.h
#pragma once
#include "afxcmn.h"

class CRichEditDlg : public CDialogEx
{
public:
    CRichEditDlg(CWnd* pParent = nullptr);

    enum { IDD = IDD_RICHEDIT_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

private:
    CRichEditCtrl m_richEdit;

    void InitializeRichEdit();
    void SetupToolbar();

public:
    afx_msg void OnBold();
    afx_msg void OnItalic();
    afx_msg void OnUnderline();
    afx_msg void OnFontColor();
    afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult);

    DECLARE_MESSAGE_MAP()
};

// RichEditDlg.cpp
#include "stdafx.h"
#include "RichEditDlg.h"

// Load Rich Edit DLL
#pragma comment(lib, "Riched20.lib")

BEGIN_MESSAGE_MAP(CRichEditDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_BOLD, &CRichEditDlg::OnBold)
    ON_BN_CLICKED(IDC_BTN_ITALIC, &CRichEditDlg::OnItalic)
    ON_BN_CLICKED(IDC_BTN_UNDERLINE, &CRichEditDlg::OnUnderline)
    ON_BN_CLICKED(IDC_BTN_COLOR, &CRichEditDlg::OnFontColor)
    ON_NOTIFY(EN_SELCHANGE, IDC_RICHEDIT, &CRichEditDlg::OnSelChange)
END_MESSAGE_MAP()

CRichEditDlg::CRichEditDlg(CWnd* pParent)
    : CDialogEx(IDD_RICHEDIT_DIALOG, pParent)
{
    // Load Rich Edit library
    AfxInitRichEdit2();
}

void CRichEditDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_RICHEDIT, m_richEdit);
}

BOOL CRichEditDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    InitializeRichEdit();
    SetupToolbar();

    return TRUE;
}

void CRichEditDlg::InitializeRichEdit()
{
    // Set options
    m_richEdit.SetOptions(ECOOP_OR, ECO_AUTOWORDSELECTION);
    m_richEdit.SetOptions(ECOOP_OR, ECO_AUTOVSCROLL);
    m_richEdit.SetOptions(ECOOP_OR, ECO_AUTOHSCROLL);

    // Enable automatic URL detection
    m_richEdit.SetEventMask(ENM_SELCHANGE | ENM_LINK);
    m_richEdit.SetAutoURLDetect(TRUE);

    // Set unlimited undo
    m_richEdit.SetUndoLimit(100);

    // Set background color
    m_richEdit.SetBackgroundColor(FALSE, RGB(255, 255, 255));

    // Set default font
    CHARFORMAT cf;
    ZeroMemory(&cf, sizeof(cf));
    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_FACE | CFM_SIZE | CFM_CHARSET;
    cf.yHeight = 200; // 10 points (200 twips)
    cf.bCharSet = DEFAULT_CHARSET;
    _tcscpy_s(cf.szFaceName, LF_FACESIZE, _T("Segoe UI"));

    m_richEdit.SetDefaultCharFormat(cf);
}
```

---

## 3. Text Formatting and Styling

### Character Formatting

```cpp
class CRichEditFormatter
{
private:
    CRichEditCtrl* m_pRichEdit;

public:
    CRichEditFormatter(CRichEditCtrl* pRichEdit)
        : m_pRichEdit(pRichEdit)
    {
    }

    // Toggle bold
    void ToggleBold()
    {
        CHARFORMAT cf;
        cf.cbSize = sizeof(cf);
        cf.dwMask = CFM_BOLD;

        m_pRichEdit->GetSelectionCharFormat(cf);

        if (cf.dwEffects & CFE_BOLD)
            cf.dwEffects = 0; // Turn off bold
        else
            cf.dwEffects = CFE_BOLD; // Turn on bold

        m_pRichEdit->SetSelectionCharFormat(cf);
        m_pRichEdit->SetFocus();
    }

    // Toggle italic
    void ToggleItalic()
    {
        CHARFORMAT cf;
        cf.cbSize = sizeof(cf);
        cf.dwMask = CFM_ITALIC;

        m_pRichEdit->GetSelectionCharFormat(cf);

        if (cf.dwEffects & CFE_ITALIC)
            cf.dwEffects = 0;
        else
            cf.dwEffects = CFE_ITALIC;

        m_pRichEdit->SetSelectionCharFormat(cf);
        m_pRichEdit->SetFocus();
    }

    // Toggle underline
    void ToggleUnderline()
    {
        CHARFORMAT cf;
        cf.cbSize = sizeof(cf);
        cf.dwMask = CFM_UNDERLINE;

        m_pRichEdit->GetSelectionCharFormat(cf);

        if (cf.dwEffects & CFE_UNDERLINE)
            cf.dwEffects = 0;
        else
            cf.dwEffects = CFE_UNDERLINE;

        m_pRichEdit->SetSelectionCharFormat(cf);
        m_pRichEdit->SetFocus();
    }

    // Toggle strikethrough
    void ToggleStrikethrough()
    {
        CHARFORMAT cf;
        cf.cbSize = sizeof(cf);
        cf.dwMask = CFM_STRIKEOUT;

        m_pRichEdit->GetSelectionCharFormat(cf);

        if (cf.dwEffects & CFE_STRIKEOUT)
            cf.dwEffects = 0;
        else
            cf.dwEffects = CFE_STRIKEOUT;

        m_pRichEdit->SetSelectionCharFormat(cf);
        m_pRichEdit->SetFocus();
    }

    // Set font
    void SetFont(const CString& strFontName)
    {
        CHARFORMAT cf;
        cf.cbSize = sizeof(cf);
        cf.dwMask = CFM_FACE;
        _tcscpy_s(cf.szFaceName, LF_FACESIZE, strFontName);

        m_pRichEdit->SetSelectionCharFormat(cf);
        m_pRichEdit->SetFocus();
    }

    // Set font size (in points)
    void SetFontSize(int nSize)
    {
        CHARFORMAT cf;
        cf.cbSize = sizeof(cf);
        cf.dwMask = CFM_SIZE;
        cf.yHeight = nSize * 20; // Convert points to twips

        m_pRichEdit->SetSelectionCharFormat(cf);
        m_pRichEdit->SetFocus();
    }

    // Set text color
    void SetTextColor(COLORREF color)
    {
        CHARFORMAT cf;
        cf.cbSize = sizeof(cf);
        cf.dwMask = CFM_COLOR;
        cf.crTextColor = color;
        cf.dwEffects = 0; // Clear CFE_AUTOCOLOR

        m_pRichEdit->SetSelectionCharFormat(cf);
        m_pRichEdit->SetFocus();
    }

    // Set background color
    void SetBackColor(COLORREF color)
    {
        CHARFORMAT2 cf;
        ZeroMemory(&cf, sizeof(cf));
        cf.cbSize = sizeof(cf);
        cf.dwMask = CFM_BACKCOLOR;
        cf.crBackColor = color;
        cf.dwEffects = 0; // Clear CFE_AUTOBACKCOLOR

        m_pRichEdit->SetSelectionCharFormat((CHARFORMAT&)cf);
        m_pRichEdit->SetFocus();
    }

    // Get current formatting
    CHARFORMAT GetCurrentFormat()
    {
        CHARFORMAT cf;
        cf.cbSize = sizeof(cf);
        m_pRichEdit->GetSelectionCharFormat(cf);
        return cf;
    }

    // Check if selection is bold
    BOOL IsBold()
    {
        CHARFORMAT cf = GetCurrentFormat();
        return (cf.dwEffects & CFE_BOLD) != 0;
    }

    // Check if selection is italic
    BOOL IsItalic()
    {
        CHARFORMAT cf = GetCurrentFormat();
        return (cf.dwEffects & CFE_ITALIC) != 0;
    }

    // Check if selection is underlined
    BOOL IsUnderline()
    {
        CHARFORMAT cf = GetCurrentFormat();
        return (cf.dwEffects & CFE_UNDERLINE) != 0;
    }
};

// Usage in dialog
void CRichEditDlg::OnBold()
{
    CRichEditFormatter formatter(&m_richEdit);
    formatter.ToggleBold();

    // Update toolbar button state
    UpdateToolbarButtons();
}

void CRichEditDlg::OnFontColor()
{
    CColorDialog dlg;
    if (dlg.DoModal() == IDOK)
    {
        CRichEditFormatter formatter(&m_richEdit);
        formatter.SetTextColor(dlg.GetColor());
    }
}
```

---

## 4. Character and Paragraph Formatting

### Paragraph Formatting

```cpp
class CRichEditParagraphFormatter
{
private:
    CRichEditCtrl* m_pRichEdit;

public:
    CRichEditParagraphFormatter(CRichEditCtrl* pRichEdit)
        : m_pRichEdit(pRichEdit)
    {
    }

    // Set paragraph alignment
    void SetAlignment(WORD wAlignment)
    {
        // wAlignment: PFA_LEFT, PFA_CENTER, PFA_RIGHT, PFA_JUSTIFY
        PARAFORMAT pf;
        pf.cbSize = sizeof(pf);
        pf.dwMask = PFM_ALIGNMENT;
        pf.wAlignment = wAlignment;

        m_pRichEdit->SetParaFormat(pf);
        m_pRichEdit->SetFocus();
    }

    // Set left indent
    void SetLeftIndent(int nIndent)
    {
        PARAFORMAT pf;
        pf.cbSize = sizeof(pf);
        pf.dwMask = PFM_STARTINDENT;
        pf.dxStartIndent = nIndent;

        m_pRichEdit->SetParaFormat(pf);
    }

    // Set right indent
    void SetRightIndent(int nIndent)
    {
        PARAFORMAT pf;
        pf.cbSize = sizeof(pf);
        pf.dwMask = PFM_RIGHTINDENT;
        pf.dxRightIndent = nIndent;

        m_pRichEdit->SetParaFormat(pf);
    }

    // Set first line indent
    void SetFirstLineIndent(int nIndent)
    {
        PARAFORMAT pf;
        pf.cbSize = sizeof(pf);
        pf.dwMask = PFM_OFFSET;
        pf.dxOffset = nIndent;

        m_pRichEdit->SetParaFormat(pf);
    }

    // Set line spacing
    void SetLineSpacing(BYTE bRule, int nSpacing)
    {
        // bRule: 0=single, 1=1.5, 2=double, 3=at least, 4=exactly, 5=multiple
        PARAFORMAT2 pf;
        ZeroMemory(&pf, sizeof(pf));
        pf.cbSize = sizeof(pf);
        pf.dwMask = PFM_LINESPACING;
        pf.bLineSpacingRule = bRule;
        pf.dyLineSpacing = nSpacing;

        m_pRichEdit->SetParaFormat((PARAFORMAT&)pf);
    }

    // Set bullet list
    void SetBulletList(BOOL bEnable)
    {
        PARAFORMAT pf;
        pf.cbSize = sizeof(pf);
        pf.dwMask = PFM_NUMBERING;

        if (bEnable)
        {
            pf.wNumbering = PFN_BULLET;
        }
        else
        {
            pf.wNumbering = 0;
        }

        m_pRichEdit->SetParaFormat(pf);
    }

    // Set numbered list
    void SetNumberedList(BOOL bEnable, WORD wNumberingStyle = PFN_ARABIC)
    {
        PARAFORMAT2 pf;
        ZeroMemory(&pf, sizeof(pf));
        pf.cbSize = sizeof(pf);
        pf.dwMask = PFM_NUMBERING | PFM_NUMBERINGSTART | PFM_NUMBERINGSTYLE;

        if (bEnable)
        {
            pf.wNumbering = PFN_ARABIC; // or PFN_LCLETTER, PFN_UCLETTER, PFN_LCROMAN, PFN_UCROMAN
            pf.wNumberingStart = 1;
            pf.wNumberingStyle = wNumberingStyle;
        }
        else
        {
            pf.wNumbering = 0;
        }

        m_pRichEdit->SetParaFormat((PARAFORMAT&)pf);
    }

    // Set paragraph spacing
    void SetParagraphSpacing(int nBefore, int nAfter)
    {
        PARAFORMAT2 pf;
        ZeroMemory(&pf, sizeof(pf));
        pf.cbSize = sizeof(pf);
        pf.dwMask = PFM_SPACEBEFORE | PFM_SPACEAFTER;
        pf.dySpaceBefore = nBefore;
        pf.dySpaceAfter = nAfter;

        m_pRichEdit->SetParaFormat((PARAFORMAT&)pf);
    }

    // Get current paragraph format
    PARAFORMAT GetCurrentFormat()
    {
        PARAFORMAT pf;
        pf.cbSize = sizeof(pf);
        m_pRichEdit->GetParaFormat(pf);
        return pf;
    }

    // Set tab stops
    void SetTabStops(const CArray<int, int>& arrTabs)
    {
        PARAFORMAT2 pf;
        ZeroMemory(&pf, sizeof(pf));
        pf.cbSize = sizeof(pf);
        pf.dwMask = PFM_TABSTOPS;
        pf.cTabCount = (SHORT)min(arrTabs.GetSize(), MAX_TAB_STOPS);

        for (int i = 0; i < pf.cTabCount; i++)
        {
            pf.rgxTabs[i] = arrTabs[i];
        }

        m_pRichEdit->SetParaFormat((PARAFORMAT&)pf);
    }
};

// Usage
void CRichEditDlg::OnAlignLeft()
{
    CRichEditParagraphFormatter formatter(&m_richEdit);
    formatter.SetAlignment(PFA_LEFT);
}

void CRichEditDlg::OnAlignCenter()
{
    CRichEditParagraphFormatter formatter(&m_richEdit);
    formatter.SetAlignment(PFA_CENTER);
}

void CRichEditDlg::OnAlignRight()
{
    CRichEditParagraphFormatter formatter(&m_richEdit);
    formatter.SetAlignment(PFA_RIGHT);
}

void CRichEditDlg::OnBulletList()
{
    CRichEditParagraphFormatter formatter(&m_richEdit);
    PARAFORMAT pf = formatter.GetCurrentFormat();
    BOOL bHasBullets = (pf.wNumbering == PFN_BULLET);
    formatter.SetBulletList(!bHasBullets);
}
```

---

## 5. RTF Document Handling

### Loading and Saving RTF

```cpp
class CRichEditDocument
{
private:
    CRichEditCtrl* m_pRichEdit;

public:
    CRichEditDocument(CRichEditCtrl* pRichEdit)
        : m_pRichEdit(pRichEdit)
    {
    }

    // Load RTF from file
    BOOL LoadFromFile(const CString& strPath)
    {
        EDITSTREAM es;
        es.dwCookie = (DWORD_PTR)&strPath;
        es.dwError = 0;
        es.pfnCallback = StreamInCallback;

        m_pRichEdit->StreamIn(SF_RTF, es);

        return es.dwError == 0;
    }

    // Save RTF to file
    BOOL SaveToFile(const CString& strPath)
    {
        EDITSTREAM es;
        es.dwCookie = (DWORD_PTR)&strPath;
        es.dwError = 0;
        es.pfnCallback = StreamOutCallback;

        m_pRichEdit->StreamOut(SF_RTF, es);

        return es.dwError == 0;
    }

    // Load plain text
    BOOL LoadTextFromFile(const CString& strPath)
    {
        EDITSTREAM es;
        es.dwCookie = (DWORD_PTR)&strPath;
        es.dwError = 0;
        es.pfnCallback = StreamInCallback;

        m_pRichEdit->StreamIn(SF_TEXT, es);

        return es.dwError == 0;
    }

    // Save plain text
    BOOL SaveTextToFile(const CString& strPath)
    {
        EDITSTREAM es;
        es.dwCookie = (DWORD_PTR)&strPath;
        es.dwError = 0;
        es.pfnCallback = StreamOutCallback;

        m_pRichEdit->StreamOut(SF_TEXT, es);

        return es.dwError == 0;
    }

    // Get RTF string
    CString GetRTF()
    {
        CString strRTF;
        EDITSTREAM es;
        es.dwCookie = (DWORD_PTR)&strRTF;
        es.dwError = 0;
        es.pfnCallback = StreamOutToString;

        m_pRichEdit->StreamOut(SF_RTF, es);

        return strRTF;
    }

    // Set RTF string
    void SetRTF(const CString& strRTF)
    {
        EDITSTREAM es;
        es.dwCookie = (DWORD_PTR)&strRTF;
        es.dwError = 0;
        es.pfnCallback = StreamInFromString;

        m_pRichEdit->StreamIn(SF_RTF, es);
    }

private:
    // Stream in callback (reading)
    static DWORD CALLBACK StreamInCallback(DWORD_PTR dwCookie, LPBYTE pbBuff,
                                          LONG cb, LONG* pcb)
    {
        CString* pstrPath = (CString*)dwCookie;
        CFile file;

        if (!file.Open(*pstrPath, CFile::modeRead))
            return 1;

        *pcb = file.Read(pbBuff, cb);
        file.Close();

        return 0;
    }

    // Stream out callback (writing)
    static DWORD CALLBACK StreamOutCallback(DWORD_PTR dwCookie, LPBYTE pbBuff,
                                           LONG cb, LONG* pcb)
    {
        CString* pstrPath = (CString*)dwCookie;
        CFile file;

        if (!file.Open(*pstrPath, CFile::modeCreate | CFile::modeWrite))
            return 1;

        file.Write(pbBuff, cb);
        *pcb = cb;
        file.Close();

        return 0;
    }

    // Stream out to string
    static DWORD CALLBACK StreamOutToString(DWORD_PTR dwCookie, LPBYTE pbBuff,
                                           LONG cb, LONG* pcb)
    {
        CString* pstr = (CString*)dwCookie;

        CString strChunk((LPCSTR)pbBuff, cb);
        *pstr += strChunk;
        *pcb = cb;

        return 0;
    }

    // Stream in from string
    static DWORD CALLBACK StreamInFromString(DWORD_PTR dwCookie, LPBYTE pbBuff,
                                            LONG cb, LONG* pcb)
    {
        CString* pstr = (CString*)dwCookie;

        int nLength = min(cb, pstr->GetLength());
        memcpy(pbBuff, (LPCSTR)*pstr, nLength);
        *pcb = nLength;

        *pstr = pstr->Mid(nLength);

        return 0;
    }
};

// Usage in dialog
void CRichEditDlg::OnFileOpen()
{
    CFileDialog dlg(TRUE, _T("rtf"), nullptr,
        OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
        _T("Rich Text Format (*.rtf)|*.rtf|Text Files (*.txt)|*.txt||"));

    if (dlg.DoModal() == IDOK)
    {
        CRichEditDocument doc(&m_richEdit);

        if (dlg.GetFileExt().CompareNoCase(_T("rtf")) == 0)
            doc.LoadFromFile(dlg.GetPathName());
        else
            doc.LoadTextFromFile(dlg.GetPathName());

        m_strCurrentFile = dlg.GetPathName();
    }
}

void CRichEditDlg::OnFileSave()
{
    if (m_strCurrentFile.IsEmpty())
    {
        OnFileSaveAs();
        return;
    }

    CRichEditDocument doc(&m_richEdit);
    doc.SaveToFile(m_strCurrentFile);
}

void CRichEditDlg::OnFileSaveAs()
{
    CFileDialog dlg(FALSE, _T("rtf"), _T("Document.rtf"),
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        _T("Rich Text Format (*.rtf)|*.rtf|Text Files (*.txt)|*.txt||"));

    if (dlg.DoModal() == IDOK)
    {
        CRichEditDocument doc(&m_richEdit);

        if (dlg.GetFileExt().CompareNoCase(_T("rtf")) == 0)
            doc.SaveToFile(dlg.GetPathName());
        else
            doc.SaveTextToFile(dlg.GetPathName());

        m_strCurrentFile = dlg.GetPathName();
    }
}
```

---

## 6. Find and Replace Operations

### Find and Replace Implementation

```cpp
class CRichEditFindReplace
{
private:
    CRichEditCtrl* m_pRichEdit;
    CFindReplaceDialog* m_pFindDlg;
    CString m_strFindWhat;
    CString m_strReplaceWith;
    BOOL m_bMatchCase;
    BOOL m_bWholeWord;
    BOOL m_bSearchDown;

public:
    CRichEditFindReplace(CRichEditCtrl* pRichEdit)
        : m_pRichEdit(pRichEdit)
        , m_pFindDlg(nullptr)
        , m_bMatchCase(FALSE)
        , m_bWholeWord(FALSE)
        , m_bSearchDown(TRUE)
    {
    }

    ~CRichEditFindReplace()
    {
        if (m_pFindDlg)
            delete m_pFindDlg;
    }

    // Show find dialog
    void ShowFindDialog(CWnd* pParentWnd)
    {
        if (m_pFindDlg)
        {
            m_pFindDlg->SetFocus();
            return;
        }

        m_pFindDlg = new CFindReplaceDialog();
        m_pFindDlg->Create(TRUE, m_strFindWhat, nullptr,
            FR_DOWN, pParentWnd);
    }

    // Show replace dialog
    void ShowReplaceDialog(CWnd* pParentWnd)
    {
        if (m_pFindDlg)
        {
            m_pFindDlg->SetFocus();
            return;
        }

        m_pFindDlg = new CFindReplaceDialog();
        m_pFindDlg->Create(FALSE, m_strFindWhat, m_strReplaceWith,
            FR_DOWN, pParentWnd);
    }

    // Find next occurrence
    BOOL FindNext(const CString& strFind, BOOL bMatchCase, BOOL bWholeWord, BOOL bDown)
    {
        FINDTEXTEX ft;
        DWORD dwFlags = 0;

        if (bMatchCase)
            dwFlags |= FR_MATCHCASE;
        if (bWholeWord)
            dwFlags |= FR_WHOLEWORD;
        if (bDown)
            dwFlags |= FR_DOWN;

        // Get current selection
        long nStart, nEnd;
        m_pRichEdit->GetSel(nStart, nEnd);

        // Setup find range
        if (bDown)
        {
            ft.chrg.cpMin = nEnd;
            ft.chrg.cpMax = -1; // Search to end
        }
        else
        {
            ft.chrg.cpMin = nStart;
            ft.chrg.cpMax = 0; // Search to beginning
        }

        ft.lpstrText = strFind;

        // Perform find
        long nPos = m_pRichEdit->FindText(dwFlags, &ft);

        if (nPos >= 0)
        {
            // Select found text
            m_pRichEdit->SetSel(ft.chrgText);
            return TRUE;
        }

        return FALSE;
    }

    // Replace current selection
    void ReplaceCurrent(const CString& strReplace)
    {
        long nStart, nEnd;
        m_pRichEdit->GetSel(nStart, nEnd);

        if (nStart != nEnd)
        {
            m_pRichEdit->ReplaceSel(strReplace);
        }
    }

    // Replace all occurrences
    int ReplaceAll(const CString& strFind, const CString& strReplace,
                   BOOL bMatchCase, BOOL bWholeWord)
    {
        int nCount = 0;

        // Start from beginning
        m_pRichEdit->SetSel(0, 0);

        while (FindNext(strFind, bMatchCase, bWholeWord, TRUE))
        {
            ReplaceCurrent(strReplace);
            nCount++;
        }

        return nCount;
    }

    // Handle find/replace dialog messages
    void OnFindReplace(CFindReplaceDialog* pDlg)
    {
        m_strFindWhat = pDlg->GetFindString();
        m_strReplaceWith = pDlg->GetReplaceString();
        m_bMatchCase = pDlg->MatchCase();
        m_bWholeWord = pDlg->MatchWholeWord();
        m_bSearchDown = (pDlg->SearchDown() != 0);

        if (pDlg->IsTerminating())
        {
            m_pFindDlg = nullptr;
        }
        else if (pDlg->FindNext())
        {
            if (!FindNext(m_strFindWhat, m_bMatchCase, m_bWholeWord, m_bSearchDown))
            {
                AfxMessageBox(_T("Finished searching the document."));
            }
        }
        else if (pDlg->ReplaceCurrent())
        {
            ReplaceCurrent(m_strReplaceWith);
            FindNext(m_strFindWhat, m_bMatchCase, m_bWholeWord, m_bSearchDown);
        }
        else if (pDlg->ReplaceAll())
        {
            int nCount = ReplaceAll(m_strFindWhat, m_strReplaceWith,
                                   m_bMatchCase, m_bWholeWord);

            CString strMsg;
            strMsg.Format(_T("Replaced %d occurrence(s)."), nCount);
            AfxMessageBox(strMsg);
        }
    }
};

// In dialog class
LRESULT CRichEditDlg::OnFindReplace(WPARAM wParam, LPARAM lParam)
{
    CFindReplaceDialog* pDlg = CFindReplaceDialog::GetNotifier(lParam);
    if (pDlg)
    {
        m_findReplace.OnFindReplace(pDlg);
    }
    return 0;
}
```

---

## 7. Undo/Redo Functionality

### Undo/Redo Manager

```cpp
class CRichEditUndoManager
{
private:
    CRichEditCtrl* m_pRichEdit;

public:
    CRichEditUndoManager(CRichEditCtrl* pRichEdit)
        : m_pRichEdit(pRichEdit)
    {
    }

    // Check if undo available
    BOOL CanUndo()
    {
        return m_pRichEdit->CanUndo();
    }

    // Check if redo available
    BOOL CanRedo()
    {
        return m_pRichEdit->CanRedo();
    }

    // Perform undo
    void Undo()
    {
        if (CanUndo())
            m_pRichEdit->Undo();
    }

    // Perform redo
    void Redo()
    {
        if (CanRedo())
            m_pRichEdit->Redo();
    }

    // Clear undo buffer
    void ClearUndo()
    {
        m_pRichEdit->EmptyUndoBuffer();
    }

    // Set undo limit
    void SetUndoLimit(UINT nLimit)
    {
        m_pRichEdit->SetUndoLimit(nLimit);
    }

    // Get undo limit
    UINT GetUndoLimit()
    {
        return m_pRichEdit->GetLimitText();
    }

    // Suspend undo collection
    void SuspendUndo()
    {
        m_pRichEdit->SetUndoLimit(0);
    }

    // Resume undo collection
    void ResumeUndo(UINT nLimit = 100)
    {
        m_pRichEdit->SetUndoLimit(nLimit);
    }
};

// Usage
void CRichEditDlg::OnEditUndo()
{
    CRichEditUndoManager undoMgr(&m_richEdit);
    undoMgr.Undo();
    UpdateEditButtons();
}

void CRichEditDlg::OnEditRedo()
{
    CRichEditUndoManager undoMgr(&m_richEdit);
    undoMgr.Redo();
    UpdateEditButtons();
}

void CRichEditDlg::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
    CRichEditUndoManager undoMgr(&m_richEdit);
    pCmdUI->Enable(undoMgr.CanUndo());
}

void CRichEditDlg::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
    CRichEditUndoManager undoMgr(&m_richEdit);
    pCmdUI->Enable(undoMgr.CanRedo());
}
```

---

## 8. Clipboard Operations

### Enhanced Clipboard Support

```cpp
class CRichEditClipboard
{
private:
    CRichEditCtrl* m_pRichEdit;

public:
    CRichEditClipboard(CRichEditCtrl* pRichEdit)
        : m_pRichEdit(pRichEdit)
    {
    }

    // Cut
    void Cut()
    {
        m_pRichEdit->Cut();
    }

    // Copy
    void Copy()
    {
        m_pRichEdit->Copy();
    }

    // Paste
    void Paste()
    {
        m_pRichEdit->Paste();
    }

    // Paste special (choose format)
    void PasteSpecial(UINT nFormat)
    {
        // CF_TEXT, CF_RTF, CF_UNICODETEXT, etc.
        m_pRichEdit->PasteSpecial(nFormat);
    }

    // Can paste?
    BOOL CanPaste()
    {
        return m_pRichEdit->CanPaste();
    }

    // Can paste format?
    BOOL CanPasteFormat(UINT nFormat)
    {
        return m_pRichEdit->CanPaste(nFormat);
    }

    // Clear selection
    void Clear()
    {
        m_pRichEdit->Clear();
    }

    // Select all
    void SelectAll()
    {
        m_pRichEdit->SetSel(0, -1);
    }

    // Get selected text
    CString GetSelectedText()
    {
        CString strText;
        int nStart, nEnd;
        m_pRichEdit->GetSel(nStart, nEnd);

        if (nStart != nEnd)
        {
            GETTEXTLENGTHEX gtl;
            gtl.flags = GTL_DEFAULT;
            gtl.codepage = CP_ACP;

            int nLength = (int)m_pRichEdit->SendMessage(EM_GETTEXTLENGTHEX,
                (WPARAM)&gtl);

            LPTSTR pszText = strText.GetBuffer(nLength + 1);

            GETTEXTEX gt;
            gt.cb = (nLength + 1) * sizeof(TCHAR);
            gt.flags = GT_SELECTION;
            gt.codepage = CP_ACP;
            gt.lpDefaultChar = nullptr;
            gt.lpUsedDefChar = nullptr;

            m_pRichEdit->SendMessage(EM_GETTEXTEX, (WPARAM)&gt, (LPARAM)pszText);
            strText.ReleaseBuffer();
        }

        return strText;
    }
};
```

---

## 9. Printing Rich Edit Content

### Print Support

```cpp
class CRichEditPrinter
{
private:
    CRichEditCtrl* m_pRichEdit;

public:
    CRichEditPrinter(CRichEditCtrl* pRichEdit)
        : m_pRichEdit(pRichEdit)
    {
    }

    void Print()
    {
        CDC dcPrint;
        CPrintDialog printDlg(FALSE);

        if (printDlg.DoModal() == IDOK)
        {
            dcPrint.Attach(printDlg.GetPrinterDC());
            PrintDocument(&dcPrint);
            dcPrint.Detach();
        }
    }

    void PrintPreview()
    {
        // Implement print preview
    }

private:
    void PrintDocument(CDC* pDC)
    {
        FORMATRANGE fr;
        ZeroMemory(&fr, sizeof(fr));

        // Get printer characteristics
        fr.hdc = pDC->GetSafeHdc();
        fr.hdcTarget = pDC->GetSafeHdc();

        // Set page size
        fr.rcPage.left = 0;
        fr.rcPage.top = 0;
        fr.rcPage.right = pDC->GetDeviceCaps(HORZRES);
        fr.rcPage.bottom = pDC->GetDeviceCaps(VERTRES);

        // Set margins
        fr.rc.left = 1000;
        fr.rc.top = 1000;
        fr.rc.right = fr.rcPage.right - 1000;
        fr.rc.bottom = fr.rcPage.bottom - 1000;

        // Get text length
        fr.chrg.cpMin = 0;
        fr.chrg.cpMax = -1;

        pDC->StartDoc(_T("Rich Edit Document"));

        while (fr.chrg.cpMin < m_pRichEdit->GetTextLength())
        {
            pDC->StartPage();

            fr.chrg.cpMin = (LONG)m_pRichEdit->FormatRange(&fr, TRUE);

            pDC->EndPage();

            if (fr.chrg.cpMin >= m_pRichEdit->GetTextLength())
                break;

            fr.chrg.cpMax = -1;
        }

        pDC->EndDoc();

        // Reset formatting
        m_pRichEdit->FormatRange(nullptr, FALSE);
    }
};
```

---

## 10. Complete Rich Text Editor Application

### Full Editor Implementation

```cpp
// Complete RichTextEditor with toolbar and formatting
class CRichTextEditorDlg : public CDialogEx
{
private:
    CRichEditCtrl m_richEdit;
    CToolBar m_toolbar;
    CComboBox m_comboFont;
    CComboBox m_comboSize;
    CStatusBar m_statusBar;

    CRichEditFormatter m_formatter;
    CRichEditParagraphFormatter m_paraFormatter;
    CRichEditDocument m_document;
    CRichEditFindReplace m_findReplace;
    CRichEditUndoManager m_undoManager;
    CRichEditClipboard m_clipboard;

    CString m_strCurrentFile;
    BOOL m_bModified;

public:
    CRichTextEditorDlg(CWnd* pParent = nullptr);

protected:
    BOOL OnInitDialog();
    void SetupToolbar();
    void UpdateFormatButtons();
    void LoadSettings();
    void SaveSettings();

    afx_msg void OnFileNew();
    afx_msg void OnFileOpen();
    afx_msg void OnFileSave();
    afx_msg void OnFileSaveAs();
    afx_msg void OnFilePrint();

    afx_msg void OnEditUndo();
    afx_msg void OnEditRedo();
    afx_msg void OnEditCut();
    afx_msg void OnEditCopy();
    afx_msg void OnEditPaste();
    afx_msg void OnEditSelectAll();
    afx_msg void OnEditFind();
    afx_msg void OnEditReplace();

    afx_msg void OnFormatFont();
    afx_msg void OnFormatBold();
    afx_msg void OnFormatItalic();
    afx_msg void OnFormatUnderline();
    afx_msg void OnFormatColor();
    afx_msg void OnFormatAlignLeft();
    afx_msg void OnFormatAlignCenter();
    afx_msg void OnFormatAlignRight();
    afx_msg void OnFormatBullets();

    afx_msg void OnFontChange();
    afx_msg void OnSizeChange();

    afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnChange();

    DECLARE_MESSAGE_MAP()
};
```

---

## Key Takeaways

1. Rich Edit controls support advanced text formatting
2. RTF format preserves formatting information
3. Character and paragraph formatting provide flexibility
4. Find/Replace operations enhance usability
5. Undo/Redo support improves user experience
6. Clipboard integration enables data exchange
7. Printing support completes document editing

---

## Best Practices

1. Always call AfxInitRichEdit2() before using
2. Enable double buffering for smooth rendering
3. Implement proper undo/redo support
4. Handle text overflow gracefully
5. Provide keyboard shortcuts
6. Save user preferences
7. Validate RTF content before loading

---

## Common Mistakes

### Mistake 1: Forgetting to Initialize Rich Edit
```cpp
// Wrong
CRichEditCtrl m_richEdit; // Won't work!

// Correct
AfxInitRichEdit2(); // Call in InitInstance or dialog constructor
CRichEditCtrl m_richEdit;
```

### Mistake 2: Incorrect Character Format Size
```cpp
// Wrong
CHARFORMAT cf;
cf.cbSize = sizeof(CHARFORMAT2); // Mismatch!

// Correct
CHARFORMAT cf;
cf.cbSize = sizeof(CHARFORMAT);
```

---

## Summary

In this lesson, you learned:
- Creating and initializing Rich Edit controls
- Character and paragraph formatting
- RTF document handling
- Find and replace operations
- Undo/Redo functionality
- Clipboard operations
- Printing support
- Building complete text editors

---

## Next Lesson Preview

In **Lesson 35**, we'll explore:
- Progress Bar control (CProgressCtrl)
- Slider control (CSliderCtrl)
- Animation and feedback
- Custom progress indicators
- Multi-threading with progress

**Excellent work with Rich Edit controls!**
