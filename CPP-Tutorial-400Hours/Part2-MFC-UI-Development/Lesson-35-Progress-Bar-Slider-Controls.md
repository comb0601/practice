# Lesson 35: Progress Bar and Slider Controls

**Duration**: 6 hours
**Difficulty**: Intermediate

## Table of Contents
1. Introduction to Progress Controls
2. Progress Bar Basics (CProgressCtrl)
3. Advanced Progress Bar Features
4. Slider Control Basics (CSliderCtrl)
5. Advanced Slider Features
6. Custom Progress Indicators
7. Multi-threading with Progress Bars
8. File Transfer Example with Progress
9. Media Player Controls
10. Complete Download Manager Example

---

## 1. Introduction to Progress Controls

Progress and slider controls provide visual feedback for:
- **Progress Bars**: Long-running operations, file downloads, installations
- **Sliders**: Volume controls, zoom levels, numeric value selection

### Common Applications:
- File copy/move operations
- Download managers
- Media players
- Image editors
- Installation wizards
- Data processing tools

---

## 2. Progress Bar Basics (CProgressCtrl)

### Basic Progress Bar Setup

```cpp
// ProgressDlg.h
#pragma once

class CProgressDlg : public CDialogEx
{
public:
    CProgressDlg(CWnd* pParent = nullptr);

    enum { IDD = IDD_PROGRESS_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

private:
    CProgressCtrl m_progressBar;
    CStatic m_staticStatus;
    CButton m_btnStart;
    CButton m_btnStop;

    void StartOperation();
    void StopOperation();
    void UpdateProgress(int nProgress);

public:
    afx_msg void OnBtnStart();
    afx_msg void OnBtnStop();
    afx_msg void OnTimer(UINT_PTR nIDEvent);

    DECLARE_MESSAGE_MAP()
};

// ProgressDlg.cpp
#include "stdafx.h"
#include "ProgressDlg.h"

BEGIN_MESSAGE_MAP(CProgressDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_START, &CProgressDlg::OnBtnStart)
    ON_BN_CLICKED(IDC_BTN_STOP, &CProgressDlg::OnBtnStop)
    ON_WM_TIMER()
END_MESSAGE_MAP()

CProgressDlg::CProgressDlg(CWnd* pParent)
    : CDialogEx(IDD_PROGRESS_DIALOG, pParent)
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PROGRESS, m_progressBar);
    DDX_Control(pDX, IDC_STATIC_STATUS, m_staticStatus);
    DDX_Control(pDX, IDC_BTN_START, m_btnStart);
    DDX_Control(pDX, IDC_BTN_STOP, m_btnStop);
}

BOOL CProgressDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Initialize progress bar
    m_progressBar.SetRange(0, 100);
    m_progressBar.SetPos(0);

    // Set initial state
    m_btnStop.EnableWindow(FALSE);

    return TRUE;
}

void CProgressDlg::OnBtnStart()
{
    m_progressBar.SetPos(0);
    m_btnStart.EnableWindow(FALSE);
    m_btnStop.EnableWindow(TRUE);

    // Start timer for simulation
    SetTimer(1, 100, nullptr);
}

void CProgressDlg::OnBtnStop()
{
    KillTimer(1);
    m_btnStart.EnableWindow(TRUE);
    m_btnStop.EnableWindow(FALSE);
}

void CProgressDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == 1)
    {
        int nPos = m_progressBar.GetPos();
        nPos += 2;

        if (nPos >= 100)
        {
            KillTimer(1);
            m_progressBar.SetPos(100);
            m_btnStart.EnableWindow(TRUE);
            m_btnStop.EnableWindow(FALSE);
            AfxMessageBox(_T("Operation completed!"));
        }
        else
        {
            m_progressBar.SetPos(nPos);

            CString strStatus;
            strStatus.Format(_T("Progress: %d%%"), nPos);
            m_staticStatus.SetWindowText(strStatus);
        }
    }

    CDialogEx::OnTimer(nIDEvent);
}
```

### Progress Bar Styles and Methods

```cpp
class CProgressBarManager
{
private:
    CProgressCtrl* m_pProgress;

public:
    CProgressBarManager(CProgressCtrl* pProgress)
        : m_pProgress(pProgress)
    {
    }

    // Set range
    void SetRange(int nLower, int nUpper)
    {
        m_pProgress->SetRange(nLower, nUpper);
    }

    void SetRange32(int nLower, int nUpper)
    {
        m_pProgress->SetRange32(nLower, nUpper);
    }

    // Get range
    void GetRange(int& nLower, int& nUpper)
    {
        m_pProgress->GetRange(nLower, nUpper);
    }

    // Set position
    int SetPos(int nPos)
    {
        return m_pProgress->SetPos(nPos);
    }

    // Get position
    int GetPos()
    {
        return m_pProgress->GetPos();
    }

    // Step it
    int StepIt()
    {
        return m_pProgress->StepIt();
    }

    // Set step
    int SetStep(int nStep)
    {
        return m_pProgress->SetStep(nStep);
    }

    // Offset position
    int OffsetPos(int nOffset)
    {
        return m_pProgress->OffsetPos(nOffset);
    }

    // Set marquee mode (indeterminate progress)
    void SetMarquee(BOOL bEnable, UINT nInterval = 30)
    {
        DWORD dwStyle = m_pProgress->GetStyle();

        if (bEnable)
        {
            m_pProgress->ModifyStyle(0, PBS_MARQUEE);
            m_pProgress->SendMessage(PBM_SETMARQUEE, TRUE, nInterval);
        }
        else
        {
            m_pProgress->SendMessage(PBM_SETMARQUEE, FALSE, 0);
            m_pProgress->ModifyStyle(PBS_MARQUEE, 0);
        }
    }

    // Set smooth style
    void SetSmooth(BOOL bSmooth)
    {
        if (bSmooth)
            m_pProgress->ModifyStyle(0, PBS_SMOOTH);
        else
            m_pProgress->ModifyStyle(PBS_SMOOTH, 0);
    }

    // Set vertical
    void SetVertical(BOOL bVertical)
    {
        if (bVertical)
            m_pProgress->ModifyStyle(0, PBS_VERTICAL);
        else
            m_pProgress->ModifyStyle(PBS_VERTICAL, 0);
    }

    // Set bar color (requires common controls 6.0)
    COLORREF SetBarColor(COLORREF color)
    {
        return (COLORREF)m_pProgress->SendMessage(PBM_SETBARCOLOR, 0, color);
    }

    // Set background color
    COLORREF SetBkColor(COLORREF color)
    {
        return (COLORREF)m_pProgress->SendMessage(PBM_SETBKCOLOR, 0, color);
    }

    // Set state (Windows Vista+)
    int SetState(int nState)
    {
        // PBST_NORMAL, PBST_ERROR, PBST_PAUSED
        return (int)m_pProgress->SendMessage(PBM_SETSTATE, nState, 0);
    }

    // Get state
    int GetState()
    {
        return (int)m_pProgress->SendMessage(PBM_GETSTATE, 0, 0);
    }
};
```

---

## 3. Advanced Progress Bar Features

### Multi-Progress Indicator

```cpp
class CMultiProgressDlg : public CDialogEx
{
private:
    CProgressCtrl m_progressTotal;
    CProgressCtrl m_progressCurrent;
    CListCtrl m_listFiles;

    int m_nTotalFiles;
    int m_nCurrentFile;

public:
    void ProcessFiles(const CStringArray& arrFiles)
    {
        m_nTotalFiles = (int)arrFiles.GetSize();
        m_progressTotal.SetRange(0, m_nTotalFiles);
        m_progressTotal.SetPos(0);

        for (int i = 0; i < m_nTotalFiles; i++)
        {
            m_nCurrentFile = i;
            ProcessFile(arrFiles[i]);

            m_progressTotal.SetPos(i + 1);
        }

        AfxMessageBox(_T("All files processed!"));
    }

private:
    void ProcessFile(const CString& strFile)
    {
        // Update current file info
        CString strMsg;
        strMsg.Format(_T("Processing: %s"), strFile);
        SetDlgItemText(IDC_STATIC_CURRENT, strMsg);

        // Simulate file processing
        m_progressCurrent.SetRange(0, 100);

        for (int i = 0; i <= 100; i += 10)
        {
            m_progressCurrent.SetPos(i);
            Sleep(100); // Simulate work

            // Process messages to keep UI responsive
            MSG msg;
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        // Add to completed list
        int nIndex = m_listFiles.InsertItem(m_listFiles.GetItemCount(), strFile);
        m_listFiles.SetItemText(nIndex, 1, _T("Completed"));
    }
};
```

---

## 4. Slider Control Basics (CSliderCtrl)

### Basic Slider Setup

```cpp
// SliderDlg.h
class CSliderDlg : public CDialogEx
{
public:
    CSliderDlg(CWnd* pParent = nullptr);

    enum { IDD = IDD_SLIDER_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

private:
    CSliderCtrl m_sliderVolume;
    CSliderCtrl m_sliderBrightness;
    CSliderCtrl m_sliderZoom;
    CStatic m_staticValue;

public:
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

    DECLARE_MESSAGE_MAP()
};

// SliderDlg.cpp
BEGIN_MESSAGE_MAP(CSliderDlg, CDialogEx)
    ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CSliderDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Setup volume slider
    m_sliderVolume.SetRange(0, 100);
    m_sliderVolume.SetPos(50);
    m_sliderVolume.SetTicFreq(10);
    m_sliderVolume.SetPageSize(10);

    // Setup brightness slider (vertical)
    m_sliderBrightness.SetRange(0, 255);
    m_sliderBrightness.SetPos(128);
    m_sliderBrightness.SetTicFreq(25);

    // Setup zoom slider
    m_sliderZoom.SetRange(10, 200);
    m_sliderZoom.SetPos(100);
    m_sliderZoom.SetTicFreq(10);

    UpdateDisplay();

    return TRUE;
}

void CSliderDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (pScrollBar == nullptr)
        return;

    CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;

    switch (nSBCode)
    {
    case TB_LINEUP:
    case TB_LINEDOWN:
    case TB_PAGEUP:
    case TB_PAGEDOWN:
    case TB_THUMBTRACK:
    case TB_THUMBPOSITION:
    case TB_TOP:
    case TB_BOTTOM:
    case TB_ENDTRACK:
        UpdateDisplay();
        break;
    }

    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSliderDlg::UpdateDisplay()
{
    int nVolume = m_sliderVolume.GetPos();
    int nBrightness = m_sliderBrightness.GetPos();
    int nZoom = m_sliderZoom.GetPos();

    CString strInfo;
    strInfo.Format(_T("Volume: %d%%\nBrightness: %d\nZoom: %d%%"),
                   nVolume, nBrightness, nZoom);

    m_staticValue.SetWindowText(strInfo);
}
```

---

## 5. Advanced Slider Features

### Slider Manager Class

```cpp
class CSliderManager
{
private:
    CSliderCtrl* m_pSlider;

public:
    CSliderManager(CSliderCtrl* pSlider)
        : m_pSlider(pSlider)
    {
    }

    // Set range
    void SetRange(int nMin, int nMax, BOOL bRedraw = TRUE)
    {
        m_pSlider->SetRange(nMin, nMax, bRedraw);
    }

    // Get range
    void GetRange(int& nMin, int& nMax)
    {
        m_pSlider->GetRange(nMin, nMax);
    }

    // Set position
    void SetPos(int nPos)
    {
        m_pSlider->SetPos(nPos);
    }

    // Get position
    int GetPos()
    {
        return m_pSlider->GetPos();
    }

    // Set tick frequency
    void SetTicFreq(int nFreq)
    {
        m_pSlider->SetTicFreq(nFreq);
    }

    // Set page size
    void SetPageSize(int nSize)
    {
        m_pSlider->SetPageSize(nSize);
    }

    // Set line size
    void SetLineSize(int nSize)
    {
        m_pSlider->SetLineSize(nSize);
    }

    // Show selection range
    void SetSelection(int nMin, int nMax)
    {
        m_pSlider->SetSelection(nMin, nMax);
    }

    // Clear selection
    void ClearSel()
    {
        m_pSlider->ClearSel();
    }

    // Set tick marks
    void SetTic(int nTic)
    {
        m_pSlider->SetTic(nTic);
    }

    // Clear all ticks
    void ClearTics(BOOL bRedraw = TRUE)
    {
        m_pSlider->ClearTics(bRedraw);
    }

    // Enable/disable tooltips
    void EnableToolTips(BOOL bEnable)
    {
        if (bEnable)
            m_pSlider->ModifyStyle(0, TBS_TOOLTIPS);
        else
            m_pSlider->ModifyStyle(TBS_TOOLTIPS, 0);
    }

    // Set tooltip text
    void SetToolTipText(const CString& strText)
    {
        m_pSlider->SetTipText(strText);
    }

    // Set buddy window (displays current value)
    CWnd* SetBuddy(CWnd* pWndBuddy, BOOL bLocation = TRUE)
    {
        return m_pSlider->SetBuddy(pWndBuddy, bLocation);
    }

    // Get channel rect
    CRect GetChannelRect()
    {
        CRect rect;
        m_pSlider->GetChannelRect(&rect);
        return rect;
    }

    // Get thumb rect
    CRect GetThumbRect()
    {
        CRect rect;
        m_pSlider->GetThumbRect(&rect);
        return rect;
    }
};
```

---

## 6. Custom Progress Indicators

### Custom Drawn Progress Bar

```cpp
class CCustomProgressCtrl : public CProgressCtrl
{
private:
    COLORREF m_clrBar;
    COLORREF m_clrBk;
    COLORREF m_clrText;
    BOOL m_bShowPercent;

public:
    CCustomProgressCtrl()
        : m_clrBar(RGB(0, 122, 204))
        , m_clrBk(RGB(240, 240, 240))
        , m_clrText(RGB(255, 255, 255))
        , m_bShowPercent(TRUE)
    {
    }

    void SetColors(COLORREF clrBar, COLORREF clrBk, COLORREF clrText)
    {
        m_clrBar = clrBar;
        m_clrBk = clrBk;
        m_clrText = clrText;
        Invalidate();
    }

    void ShowPercent(BOOL bShow)
    {
        m_bShowPercent = bShow;
        Invalidate();
    }

protected:
    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnPaint();
};

BEGIN_MESSAGE_MAP(CCustomProgressCtrl, CProgressCtrl)
    ON_WM_PAINT()
END_MESSAGE_MAP()

void CCustomProgressCtrl::OnPaint()
{
    CPaintDC dc(this);

    CRect rect;
    GetClientRect(&rect);

    // Get progress info
    int nLower, nUpper;
    GetRange(nLower, nUpper);
    int nPos = GetPos();

    // Calculate progress width
    int nProgressWidth = 0;
    if (nUpper > nLower)
    {
        nProgressWidth = (int)((double)(nPos - nLower) / (nUpper - nLower) * rect.Width());
    }

    // Draw background
    dc.FillSolidRect(rect, m_clrBk);

    // Draw progress bar
    CRect rectProgress = rect;
    rectProgress.right = rectProgress.left + nProgressWidth;
    dc.FillSolidRect(rectProgress, m_clrBar);

    // Draw border
    dc.Draw3dRect(rect, RGB(128, 128, 128), RGB(255, 255, 255));

    // Draw percentage text
    if (m_bShowPercent)
    {
        int nPercent = 0;
        if (nUpper > nLower)
        {
            nPercent = (int)((double)(nPos - nLower) / (nUpper - nLower) * 100);
        }

        CString strPercent;
        strPercent.Format(_T("%d%%"), nPercent);

        dc.SetBkMode(TRANSPARENT);
        dc.SetTextColor(m_clrText);

        CFont* pOldFont = dc.SelectObject(GetFont());
        dc.DrawText(strPercent, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        dc.SelectObject(pOldFont);
    }
}
```

---

## 7. Multi-threading with Progress Bars

### Worker Thread with Progress Updates

```cpp
// Worker thread structure
struct ThreadParams
{
    CProgressCtrl* pProgress;
    CStatic* pStatus;
    CWnd* pParentWnd;
    BOOL* pbCancel;
};

// Worker thread function
UINT WorkerThreadProc(LPVOID pParam)
{
    ThreadParams* pParams = (ThreadParams*)pParam;

    for (int i = 0; i <= 100; i++)
    {
        // Check for cancellation
        if (*pParams->pbCancel)
            break;

        // Simulate work
        Sleep(50);

        // Update progress (thread-safe)
        pParams->pProgress->PostMessage(PBM_SETPOS, i, 0);

        // Update status text
        CString strStatus;
        strStatus.Format(_T("Processing: %d%%"), i);
        pParams->pStatus->SetWindowText(strStatus);
    }

    // Notify completion
    pParams->pParentWnd->PostMessage(WM_USER + 1, 0, 0);

    delete pParams;
    return 0;
}

// Dialog class with threaded operation
class CThreadedProgressDlg : public CDialogEx
{
private:
    CProgressCtrl m_progress;
    CStatic m_staticStatus;
    CWinThread* m_pThread;
    BOOL m_bCancel;

public:
    void StartOperation()
    {
        m_bCancel = FALSE;

        ThreadParams* pParams = new ThreadParams;
        pParams->pProgress = &m_progress;
        pParams->pStatus = &m_staticStatus;
        pParams->pParentWnd = this;
        pParams->pbCancel = &m_bCancel;

        m_pThread = AfxBeginThread(WorkerThreadProc, pParams);
    }

    void CancelOperation()
    {
        m_bCancel = TRUE;
    }

protected:
    LRESULT OnThreadComplete(WPARAM wParam, LPARAM lParam)
    {
        AfxMessageBox(_T("Operation completed!"));
        m_pThread = nullptr;
        return 0;
    }

    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CThreadedProgressDlg, CDialogEx)
    ON_MESSAGE(WM_USER + 1, &CThreadedProgressDlg::OnThreadComplete)
END_MESSAGE_MAP()
```

---

## 8. File Transfer Example with Progress

### File Copy with Progress

```cpp
class CFileCopyDlg : public CDialogEx
{
private:
    CProgressCtrl m_progressBytes;
    CProgressCtrl m_progressFiles;
    CStatic m_staticInfo;
    BOOL m_bCancel;

public:
    void CopyFiles(const CStringArray& arrSource, const CString& strDest)
    {
        m_bCancel = FALSE;

        int nTotalFiles = (int)arrSource.GetSize();
        __int64 nTotalSize = CalculateTotalSize(arrSource);

        m_progressFiles.SetRange32(0, nTotalFiles);
        m_progressBytes.SetRange32(0, (int)(nTotalSize / 1024)); // KB

        __int64 nBytesCopied = 0;

        for (int i = 0; i < nTotalFiles; i++)
        {
            if (m_bCancel)
                break;

            CString strSource = arrSource[i];
            CString strFileName = GetFileNameFromPath(strSource);
            CString strDestFile = strDest + _T("\\") + strFileName;

            // Update status
            CString strMsg;
            strMsg.Format(_T("Copying: %s"), strFileName);
            m_staticInfo.SetWindowText(strMsg);

            // Copy file with progress
            __int64 nFileSize = GetFileSize(strSource);
            CopyFileWithProgress(strSource, strDestFile, nFileSize, nBytesCopied);

            nBytesCopied += nFileSize;

            m_progressFiles.SetPos(i + 1);
            m_progressBytes.SetPos((int)(nBytesCopied / 1024));

            // Process messages
            ProcessMessages();
        }

        if (m_bCancel)
            AfxMessageBox(_T("Operation cancelled!"));
        else
            AfxMessageBox(_T("All files copied successfully!"));
    }

private:
    void CopyFileWithProgress(const CString& strSource, const CString& strDest,
                             __int64 nFileSize, __int64 nBytesCopied)
    {
        const int BUFFER_SIZE = 64 * 1024; // 64 KB buffer
        BYTE buffer[BUFFER_SIZE];

        CFile fileSource, fileDest;

        if (!fileSource.Open(strSource, CFile::modeRead | CFile::shareDenyWrite))
            return;

        if (!fileDest.Open(strDest, CFile::modeCreate | CFile::modeWrite))
        {
            fileSource.Close();
            return;
        }

        __int64 nRemaining = nFileSize;

        while (nRemaining > 0 && !m_bCancel)
        {
            UINT nBytesToRead = (UINT)min((__int64)BUFFER_SIZE, nRemaining);
            UINT nBytesRead = fileSource.Read(buffer, nBytesToRead);

            if (nBytesRead == 0)
                break;

            fileDest.Write(buffer, nBytesRead);

            nRemaining -= nBytesRead;
            nBytesCopied += nBytesRead;

            m_progressBytes.SetPos((int)(nBytesCopied / 1024));

            ProcessMessages();
        }

        fileSource.Close();
        fileDest.Close();
    }

    __int64 CalculateTotalSize(const CStringArray& arrFiles)
    {
        __int64 nTotal = 0;

        for (int i = 0; i < arrFiles.GetSize(); i++)
        {
            nTotal += GetFileSize(arrFiles[i]);
        }

        return nTotal;
    }

    __int64 GetFileSize(const CString& strPath)
    {
        CFileStatus status;
        if (CFile::GetStatus(strPath, status))
        {
            return status.m_size;
        }
        return 0;
    }

    void ProcessMessages()
    {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
};
```

---

## 9. Media Player Controls

### Media Player Slider Controls

```cpp
class CMediaPlayerDlg : public CDialogEx
{
private:
    CSliderCtrl m_sliderPosition;
    CSliderCtrl m_sliderVolume;
    CButton m_btnPlay;
    CButton m_btnPause;
    CButton m_btnStop;
    CStatic m_staticTime;

    int m_nDuration; // in seconds
    int m_nCurrentPos;
    BOOL m_bPlaying;

public:
    BOOL OnInitDialog()
    {
        CDialogEx::OnInitDialog();

        // Setup position slider
        m_sliderPosition.SetRange(0, 100);
        m_sliderPosition.SetPos(0);
        m_sliderPosition.SetTicFreq(10);

        // Setup volume slider
        m_sliderVolume.SetRange(0, 100);
        m_sliderVolume.SetPos(50);
        m_sliderVolume.SetTicFreq(10);

        m_bPlaying = FALSE;
        m_nDuration = 180; // 3 minutes example

        return TRUE;
    }

    void OnBtnPlay()
    {
        m_bPlaying = TRUE;
        SetTimer(1, 1000, nullptr); // Update every second
    }

    void OnBtnPause()
    {
        m_bPlaying = FALSE;
        KillTimer(1);
    }

    void OnBtnStop()
    {
        m_bPlaying = FALSE;
        KillTimer(1);
        m_nCurrentPos = 0;
        m_sliderPosition.SetPos(0);
        UpdateTimeDisplay();
    }

    void OnTimer(UINT_PTR nIDEvent)
    {
        if (nIDEvent == 1 && m_bPlaying)
        {
            m_nCurrentPos++;

            if (m_nCurrentPos >= m_nDuration)
            {
                OnBtnStop();
            }
            else
            {
                int nPos = (int)((double)m_nCurrentPos / m_nDuration * 100);
                m_sliderPosition.SetPos(nPos);
                UpdateTimeDisplay();
            }
        }

        CDialogEx::OnTimer(nIDEvent);
    }

    void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
    {
        if ((CSliderCtrl*)pScrollBar == &m_sliderPosition)
        {
            if (nSBCode == TB_THUMBTRACK || nSBCode == TB_THUMBPOSITION)
            {
                int nSliderPos = m_sliderPosition.GetPos();
                m_nCurrentPos = (int)(nSliderPos * m_nDuration / 100.0);
                UpdateTimeDisplay();
            }
        }
        else if ((CSliderCtrl*)pScrollBar == &m_sliderVolume)
        {
            int nVolume = m_sliderVolume.GetPos();
            // Apply volume change
            ApplyVolume(nVolume);
        }

        CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
    }

private:
    void UpdateTimeDisplay()
    {
        int nMinutes = m_nCurrentPos / 60;
        int nSeconds = m_nCurrentPos % 60;
        int nTotalMin = m_nDuration / 60;
        int nTotalSec = m_nDuration % 60;

        CString strTime;
        strTime.Format(_T("%02d:%02d / %02d:%02d"),
                      nMinutes, nSeconds, nTotalMin, nTotalSec);

        m_staticTime.SetWindowText(strTime);
    }

    void ApplyVolume(int nVolume)
    {
        // Apply volume to media player
        CString strVolume;
        strVolume.Format(_T("Volume: %d%%"), nVolume);
        SetDlgItemText(IDC_STATIC_VOLUME, strVolume);
    }
};
```

---

## 10. Complete Download Manager Example

### Download Manager with Multiple Progress Bars

```cpp
// Download item structure
struct DownloadItem
{
    CString strURL;
    CString strFile;
    __int64 nTotalSize;
    __int64 nDownloaded;
    int nProgress;
    BOOL bCompleted;
};

class CDownloadManagerDlg : public CDialogEx
{
private:
    CListCtrl m_listDownloads;
    CProgressCtrl m_progressOverall;
    CImageList m_imageList;

    CArray<DownloadItem*, DownloadItem*> m_arrDownloads;

public:
    BOOL OnInitDialog()
    {
        CDialogEx::OnInitDialog();

        // Setup list control
        m_listDownloads.SetExtendedStyle(
            LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

        m_listDownloads.InsertColumn(0, _T("File"), LVCFMT_LEFT, 200);
        m_listDownloads.InsertColumn(1, _T("Size"), LVCFMT_RIGHT, 80);
        m_listDownloads.InsertColumn(2, _T("Progress"), LVCFMT_LEFT, 150);
        m_listDownloads.InsertColumn(3, _T("Status"), LVCFMT_LEFT, 100);

        return TRUE;
    }

    void AddDownload(const CString& strURL, const CString& strFile)
    {
        DownloadItem* pItem = new DownloadItem;
        pItem->strURL = strURL;
        pItem->strFile = strFile;
        pItem->nTotalSize = 0;
        pItem->nDownloaded = 0;
        pItem->nProgress = 0;
        pItem->bCompleted = FALSE;

        m_arrDownloads.Add(pItem);

        // Add to list
        int nIndex = m_listDownloads.InsertItem(
            m_listDownloads.GetItemCount(), strFile);

        m_listDownloads.SetItemData(nIndex, (DWORD_PTR)pItem);
        m_listDownloads.SetItemText(nIndex, 3, _T("Queued"));
    }

    void StartDownloads()
    {
        for (int i = 0; i < m_arrDownloads.GetSize(); i++)
        {
            DownloadFile(i);
        }

        AfxMessageBox(_T("All downloads completed!"));
    }

private:
    void DownloadFile(int nIndex)
    {
        DownloadItem* pItem = m_arrDownloads[nIndex];

        // Update status
        m_listDownloads.SetItemText(nIndex, 3, _T("Downloading"));

        // Simulate download
        pItem->nTotalSize = 1024 * 1024 * 10; // 10 MB

        for (int i = 0; i <= 100; i++)
        {
            pItem->nDownloaded = (__int64)(pItem->nTotalSize * i / 100.0);
            pItem->nProgress = i;

            UpdateDownloadProgress(nIndex);

            Sleep(50); // Simulate download
            ProcessMessages();
        }

        pItem->bCompleted = TRUE;
        m_listDownloads.SetItemText(nIndex, 3, _T("Completed"));
    }

    void UpdateDownloadProgress(int nIndex)
    {
        DownloadItem* pItem = m_arrDownloads[nIndex];

        // Update size
        CString strSize;
        strSize.Format(_T("%I64d KB"), pItem->nTotalSize / 1024);
        m_listDownloads.SetItemText(nIndex, 1, strSize);

        // Update progress bar text
        CString strProgress;
        strProgress.Format(_T("%d%%"), pItem->nProgress);
        m_listDownloads.SetItemText(nIndex, 2, strProgress);

        // Update overall progress
        int nOverallProgress = CalculateOverallProgress();
        m_progressOverall.SetPos(nOverallProgress);
    }

    int CalculateOverallProgress()
    {
        if (m_arrDownloads.GetSize() == 0)
            return 0;

        int nTotal = 0;
        for (int i = 0; i < m_arrDownloads.GetSize(); i++)
        {
            nTotal += m_arrDownloads[i]->nProgress;
        }

        return nTotal / (int)m_arrDownloads.GetSize();
    }

    void ProcessMessages()
    {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
};
```

---

## Key Takeaways

1. Progress bars provide visual feedback for long operations
2. Sliders enable intuitive value selection
3. Multi-threading prevents UI freezing
4. Custom drawing allows unique appearances
5. Proper message handling keeps UI responsive
6. Range and position management is crucial
7. Combine progress controls with status updates

---

## Best Practices

1. Always use threading for long operations
2. Update progress in reasonable increments
3. Provide cancel functionality
4. Show meaningful status messages
5. Use smooth progress for better UX
6. Handle edge cases (0%, 100%)
7. Test with various data sizes

---

## Common Mistakes

### Mistake 1: Blocking UI Thread
```cpp
// Wrong - Blocks UI
for (int i = 0; i < 1000; i++)
{
    DoWork();
    m_progress.SetPos(i);
}

// Correct - Process messages
for (int i = 0; i < 1000; i++)
{
    DoWork();
    m_progress.SetPos(i);
    ProcessMessages(); // Or use worker thread
}
```

### Mistake 2: Incorrect Range
```cpp
// Wrong - Pos exceeds range
m_progress.SetRange(0, 100);
m_progress.SetPos(150); // Error!

// Correct
m_progress.SetRange(0, 100);
m_progress.SetPos(min(nPos, 100));
```

---

## Summary

In this lesson, you learned:
- Creating and using progress bars
- Implementing slider controls
- Custom progress indicators
- Multi-threading with progress
- File operations with progress
- Media player controls
- Download manager implementation

---

## Next Lesson Preview

In **Lesson 36**, we'll explore:
- Tab Control (CTabCtrl) fundamentals
- Creating tabbed interfaces
- Dynamic tab management
- Tab icons and images
- Property sheets vs. tab controls

**Great job with progress and slider controls!**
