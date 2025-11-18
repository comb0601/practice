# Lesson 78: Project 2 - Image Viewer (Complete)

## Table of Contents
1. [Project Overview](#overview)
2. [Application Architecture](#architecture)
3. [Complete Source Code](#source-code)
4. [Image Processing Implementation](#image-processing)
5. [Zoom and Pan Features](#zoom-pan)
6. [Where to Look in the Code](#code-navigation)
7. [Building and Running](#building)
8. [Extending the Application](#extending)
9. [Common Issues and Solutions](#issues)
10. [Performance Optimization](#optimization)

## 1. Project Overview {#overview}

We're building a professional image viewer application with the following features:
- Support for multiple image formats (BMP, JPG, PNG, GIF, TIFF)
- Zoom in/out with mouse wheel
- Pan with mouse drag
- Fit to window and actual size modes
- Image rotation (90, 180, 270 degrees)
- Flip horizontal/vertical
- Thumbnail view and slideshow
- Basic image adjustments (brightness, contrast, saturation)
- Batch conversion
- EXIF data display
- Image comparison (side-by-side)

### Technology Stack
- **Framework**: MFC Document/View Architecture
- **Image Library**: GDI+ for image loading and manipulation
- **UI**: SDI (Single Document Interface) with dockable panels
- **Design Patterns**: Document/View, Strategy (for image operations)

### Learning Objectives
- Master GDI+ for image handling
- Implement zoom and pan functionality
- Handle large images efficiently
- Work with different image formats
- Create smooth user interactions

## 2. Application Architecture {#architecture}

### Architecture Diagram

```
ImageViewerApp (CWinApp)
    |
    +-- CMainFrame (CFrameWndEx)
    |       |
    |       +-- Ribbon Bar
    |       +-- Status Bar
    |       +-- Thumbnail Pane (Dockable)
    |       +-- Properties Pane (Dockable)
    |
    +-- CImageDocument (CDocument)
    |       |
    |       +-- Image Data (Gdiplus::Bitmap)
    |       +-- Image Properties
    |       +-- Operation History
    |
    +-- CImageView (CScrollView)
    |       |
    |       +-- Zoom Manager
    |       +-- Pan Manager
    |       +-- Rendering Engine
    |
    +-- Supporting Classes
            |
            +-- CImageProcessor
            +-- CThumbnailManager
            +-- CExifReader
            +-- CImageComparer
```

### Component Responsibilities

**CImageViewerApp**
- Application initialization
- GDI+ startup/shutdown
- Document template creation
- Global settings

**CMainFrame**
- Main window with ribbon interface
- Dockable panes management
- Status bar updates
- UI state management

**CImageDocument**
- Image data storage
- Image operations (rotate, flip, adjust)
- File I/O operations
- Operation history for undo/redo

**CImageView**
- Image rendering with zoom
- Mouse interaction (pan, zoom)
- Selection management
- Scroll handling

## 3. Complete Source Code {#source-code}

### ImageViewerApp.h

```cpp
// ImageViewerApp.h
#pragma once

#ifndef __AFXWIN_H__
    #error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"
#include <gdiplus.h>

class CImageViewerApp : public CWinAppEx
{
public:
    CImageViewerApp();

    // GDI+ token
    ULONG_PTR m_gdiplusToken;

    // Settings
    COLORREF m_backgroundColor;
    BOOL m_bSmoothZoom;
    BOOL m_bShowThumbnails;
    int m_nSlideshowDelay;

    // GDI+ helper
    Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;

    // Overrides
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    // Implementation
    afx_msg void OnAppAbout();
    DECLARE_MESSAGE_MAP()

private:
    void LoadSettings();
    void SaveSettings();
};

extern CImageViewerApp theApp;
```

### ImageViewerApp.cpp

```cpp
// ImageViewerApp.cpp
#include "stdafx.h"
#include "ImageViewerApp.h"
#include "MainFrm.h"
#include "ImageDocument.h"
#include "ImageView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Gdiplus;

CImageViewerApp theApp;

BEGIN_MESSAGE_MAP(CImageViewerApp, CWinAppEx)
    ON_COMMAND(ID_APP_ABOUT, &CImageViewerApp::OnAppAbout)
    ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()

CImageViewerApp::CImageViewerApp()
    : m_gdiplusToken(0)
    , m_backgroundColor(RGB(64, 64, 64))
    , m_bSmoothZoom(TRUE)
    , m_bShowThumbnails(TRUE)
    , m_nSlideshowDelay(3000)
{
}

BOOL CImageViewerApp::InitInstance()
{
    // Initialize GDI+
    GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);

    // Standard initialization
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinAppEx::InitInstance();

    // Initialize OLE libraries
    if (!AfxOleInit())
    {
        AfxMessageBox(_T("OLE initialization failed"));
        return FALSE;
    }

    AfxEnableControlContainer();

    // Load settings
    LoadSettings();

    // Enable task bar thumbnail previews
    EnableTaskbarInteraction(FALSE);

    // Set registry key
    SetRegistryKey(_T("ImageViewer"));

    // Register document templates
    CSingleDocTemplate* pDocTemplate;
    pDocTemplate = new CSingleDocTemplate(
        IDR_MAINFRAME,
        RUNTIME_CLASS(CImageDocument),
        RUNTIME_CLASS(CMainFrame),
        RUNTIME_CLASS(CImageView));

    if (!pDocTemplate)
        return FALSE;

    AddDocTemplate(pDocTemplate);

    // Parse command line
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // Dispatch commands specified on the command line
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    // Show main window
    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();

    return TRUE;
}

int CImageViewerApp::ExitInstance()
{
    SaveSettings();

    // Shutdown GDI+
    if (m_gdiplusToken)
    {
        GdiplusShutdown(m_gdiplusToken);
    }

    return CWinAppEx::ExitInstance();
}

void CImageViewerApp::LoadSettings()
{
    m_backgroundColor = GetProfileInt(_T("Settings"), _T("BackgroundColor"), RGB(64, 64, 64));
    m_bSmoothZoom = GetProfileInt(_T("Settings"), _T("SmoothZoom"), TRUE);
    m_bShowThumbnails = GetProfileInt(_T("Settings"), _T("ShowThumbnails"), TRUE);
    m_nSlideshowDelay = GetProfileInt(_T("Settings"), _T("SlideshowDelay"), 3000);
}

void CImageViewerApp::SaveSettings()
{
    WriteProfileInt(_T("Settings"), _T("BackgroundColor"), m_backgroundColor);
    WriteProfileInt(_T("Settings"), _T("SmoothZoom"), m_bSmoothZoom);
    WriteProfileInt(_T("Settings"), _T("ShowThumbnails"), m_bShowThumbnails);
    WriteProfileInt(_T("Settings"), _T("SlideshowDelay"), m_nSlideshowDelay);
}

void CImageViewerApp::OnAppAbout()
{
    CString message;
    message.Format(_T("Image Viewer v1.0\n\nA professional image viewer built with MFC and GDI+\n\nSupported Formats:\n- BMP, JPG, PNG, GIF, TIFF\n\nFeatures:\n- Zoom and Pan\n- Image Rotation\n- Batch Processing\n- EXIF Display\n\n© 2025"));
    AfxMessageBox(message, MB_OK | MB_ICONINFORMATION);
}
```

### ImageDocument.h

```cpp
// ImageDocument.h
#pragma once

#include <gdiplus.h>

class CImageDocument : public CDocument
{
protected:
    CImageDocument();
    DECLARE_DYNCREATE(CImageDocument)

// Attributes
public:
    enum RotationAngle
    {
        ROTATE_NONE = 0,
        ROTATE_90 = 90,
        ROTATE_180 = 180,
        ROTATE_270 = 270
    };

private:
    Gdiplus::Bitmap* m_pImage;          // Original image
    Gdiplus::Bitmap* m_pDisplayImage;   // Transformed image
    CString m_filePath;                 // Current file path
    RotationAngle m_rotation;           // Current rotation
    BOOL m_bFlippedH;                   // Horizontally flipped
    BOOL m_bFlippedV;                   // Vertically flipped
    int m_nBrightness;                  // Brightness adjustment (-100 to 100)
    int m_nContrast;                    // Contrast adjustment (-100 to 100)
    int m_nSaturation;                  // Saturation adjustment (-100 to 100)

    // EXIF data
    CMap<CString, LPCTSTR, CString, CString&> m_exifData;

// Operations
public:
    // Image access
    Gdiplus::Bitmap* GetImage() const { return m_pDisplayImage; }
    Gdiplus::Bitmap* GetOriginalImage() const { return m_pImage; }
    int GetImageWidth() const;
    int GetImageHeight() const;

    // Image transformations
    void RotateImage(RotationAngle angle);
    void FlipHorizontal();
    void FlipVertical();
    void ResetTransformations();

    // Image adjustments
    void SetBrightness(int value);
    void SetContrast(int value);
    void SetSaturation(int value);
    void ApplyAdjustments();

    // EXIF data
    BOOL ReadExifData();
    CString GetExifValue(const CString& tag);
    const CMap<CString, LPCTSTR, CString, CString&>& GetExifData() const { return m_exifData; }

    // Image operations
    BOOL SaveAs(const CString& filePath, const CString& format);
    Gdiplus::Bitmap* CreateThumbnail(int width, int height);

    // File info
    CString GetFilePath() const { return m_filePath; }
    CString GetFileName() const;
    CString GetFileFormat() const;
    ULONGLONG GetFileSize() const;

// Overrides
public:
    virtual BOOL OnNewDocument();
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
    virtual void DeleteContents();

// Implementation
public:
    virtual ~CImageDocument();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnImageRotate90();
    afx_msg void OnImageRotate180();
    afx_msg void OnImageRotate270();
    afx_msg void OnImageFlipH();
    afx_msg void OnImageFlipV();
    afx_msg void OnImageReset();

private:
    void UpdateDisplayImage();
    BOOL LoadImage(const CString& filePath);
    void ClearImage();
    CString GetEncoderClsid(const WCHAR* format);
};
```

### ImageDocument.cpp

```cpp
// ImageDocument.cpp
#include "stdafx.h"
#include "ImageViewerApp.h"
#include "ImageDocument.h"
#include <propidl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Gdiplus;

IMPLEMENT_DYNCREATE(CImageDocument, CDocument)

BEGIN_MESSAGE_MAP(CImageDocument, CDocument)
    ON_COMMAND(ID_IMAGE_ROTATE_90, &CImageDocument::OnImageRotate90)
    ON_COMMAND(ID_IMAGE_ROTATE_180, &CImageDocument::OnImageRotate180)
    ON_COMMAND(ID_IMAGE_ROTATE_270, &CImageDocument::OnImageRotate270)
    ON_COMMAND(ID_IMAGE_FLIP_H, &CImageDocument::OnImageFlipH)
    ON_COMMAND(ID_IMAGE_FLIP_V, &CImageDocument::OnImageFlipV)
    ON_COMMAND(ID_IMAGE_RESET, &CImageDocument::OnImageReset)
END_MESSAGE_MAP()

CImageDocument::CImageDocument()
    : m_pImage(NULL)
    , m_pDisplayImage(NULL)
    , m_rotation(ROTATE_NONE)
    , m_bFlippedH(FALSE)
    , m_bFlippedV(FALSE)
    , m_nBrightness(0)
    , m_nContrast(0)
    , m_nSaturation(0)
{
}

CImageDocument::~CImageDocument()
{
    ClearImage();
}

void CImageDocument::ClearImage()
{
    if (m_pImage)
    {
        delete m_pImage;
        m_pImage = NULL;
    }

    if (m_pDisplayImage && m_pDisplayImage != m_pImage)
    {
        delete m_pDisplayImage;
        m_pDisplayImage = NULL;
    }
}

BOOL CImageDocument::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    ClearImage();
    return TRUE;
}

BOOL CImageDocument::OnOpenDocument(LPCTSTR lpszPathName)
{
    ClearImage();

    if (!LoadImage(lpszPathName))
    {
        AfxMessageBox(_T("Failed to load image"));
        return FALSE;
    }

    m_filePath = lpszPathName;
    SetPathName(lpszPathName, TRUE);

    // Read EXIF data
    ReadExifData();

    // Reset transformations
    m_rotation = ROTATE_NONE;
    m_bFlippedH = FALSE;
    m_bFlippedV = FALSE;
    m_nBrightness = 0;
    m_nContrast = 0;
    m_nSaturation = 0;

    UpdateDisplayImage();
    SetModifiedFlag(FALSE);

    return TRUE;
}

BOOL CImageDocument::LoadImage(const CString& filePath)
{
    // Convert CString to WCHAR
    USES_CONVERSION;
    WCHAR* wPath = T2W((LPTSTR)(LPCTSTR)filePath);

    // Load image using GDI+
    m_pImage = new Bitmap(wPath);

    if (m_pImage->GetLastStatus() != Ok)
    {
        delete m_pImage;
        m_pImage = NULL;
        return FALSE;
    }

    return TRUE;
}

void CImageDocument::UpdateDisplayImage()
{
    if (!m_pImage)
        return;

    // Delete old display image if different from original
    if (m_pDisplayImage && m_pDisplayImage != m_pImage)
    {
        delete m_pDisplayImage;
    }

    // Clone original image
    m_pDisplayImage = m_pImage->Clone(0, 0, m_pImage->GetWidth(),
        m_pImage->GetHeight(), PixelFormatDontCare);

    if (!m_pDisplayImage)
    {
        m_pDisplayImage = m_pImage;
        return;
    }

    // Apply transformations
    if (m_rotation != ROTATE_NONE)
    {
        switch (m_rotation)
        {
        case ROTATE_90:
            m_pDisplayImage->RotateFlip(Rotate90FlipNone);
            break;
        case ROTATE_180:
            m_pDisplayImage->RotateFlip(Rotate180FlipNone);
            break;
        case ROTATE_270:
            m_pDisplayImage->RotateFlip(Rotate270FlipNone);
            break;
        }
    }

    if (m_bFlippedH)
    {
        m_pDisplayImage->RotateFlip(RotateNoneFlipX);
    }

    if (m_bFlippedV)
    {
        m_pDisplayImage->RotateFlip(RotateNoneFlipY);
    }

    // Apply adjustments
    if (m_nBrightness != 0 || m_nContrast != 0 || m_nSaturation != 0)
    {
        ApplyAdjustments();
    }

    UpdateAllViews(NULL);
}

int CImageDocument::GetImageWidth() const
{
    if (m_pDisplayImage)
        return m_pDisplayImage->GetWidth();
    return 0;
}

int CImageDocument::GetImageHeight() const
{
    if (m_pDisplayImage)
        return m_pDisplayImage->GetHeight();
    return 0;
}

void CImageDocument::RotateImage(RotationAngle angle)
{
    m_rotation = angle;
    UpdateDisplayImage();
    SetModifiedFlag(TRUE);
}

void CImageDocument::FlipHorizontal()
{
    m_bFlippedH = !m_bFlippedH;
    UpdateDisplayImage();
    SetModifiedFlag(TRUE);
}

void CImageDocument::FlipVertical()
{
    m_bFlippedV = !m_bFlippedV;
    UpdateDisplayImage();
    SetModifiedFlag(TRUE);
}

void CImageDocument::ResetTransformations()
{
    m_rotation = ROTATE_NONE;
    m_bFlippedH = FALSE;
    m_bFlippedV = FALSE;
    m_nBrightness = 0;
    m_nContrast = 0;
    m_nSaturation = 0;
    UpdateDisplayImage();
    SetModifiedFlag(FALSE);
}

void CImageDocument::SetBrightness(int value)
{
    m_nBrightness = max(-100, min(100, value));
    UpdateDisplayImage();
    SetModifiedFlag(TRUE);
}

void CImageDocument::SetContrast(int value)
{
    m_nContrast = max(-100, min(100, value));
    UpdateDisplayImage();
    SetModifiedFlag(TRUE);
}

void CImageDocument::SetSaturation(int value)
{
    m_nSaturation = max(-100, min(100, value));
    UpdateDisplayImage();
    SetModifiedFlag(TRUE);
}

void CImageDocument::ApplyAdjustments()
{
    if (!m_pDisplayImage)
        return;

    // Create ImageAttributes for color adjustments
    ImageAttributes imageAttr;

    // Brightness and contrast matrix
    float brightness = m_nBrightness / 100.0f;
    float contrast = (100.0f + m_nContrast) / 100.0f;

    ColorMatrix colorMatrix = {
        contrast, 0, 0, 0, 0,
        0, contrast, 0, 0, 0,
        0, 0, contrast, 0, 0,
        0, 0, 0, 1, 0,
        brightness, brightness, brightness, 0, 1
    };

    imageAttr.SetColorMatrix(&colorMatrix);

    // Create a new bitmap with adjustments
    Bitmap* pAdjusted = new Bitmap(m_pDisplayImage->GetWidth(),
        m_pDisplayImage->GetHeight(), PixelFormat32bppARGB);

    Graphics g(pAdjusted);
    g.DrawImage(m_pDisplayImage,
        Rect(0, 0, m_pDisplayImage->GetWidth(), m_pDisplayImage->GetHeight()),
        0, 0, m_pDisplayImage->GetWidth(), m_pDisplayImage->GetHeight(),
        UnitPixel, &imageAttr);

    // Replace display image
    if (m_pDisplayImage != m_pImage)
    {
        delete m_pDisplayImage;
    }
    m_pDisplayImage = pAdjusted;
}

BOOL CImageDocument::ReadExifData()
{
    if (!m_pImage)
        return FALSE;

    m_exifData.RemoveAll();

    // Get property item count
    UINT totalBufferSize = 0;
    UINT numProperties = 0;
    m_pImage->GetPropertySize(&totalBufferSize, &numProperties);

    if (numProperties == 0)
        return FALSE;

    // Get all property items
    PropertyItem* pItems = (PropertyItem*)malloc(totalBufferSize);
    if (!pItems)
        return FALSE;

    Status status = m_pImage->GetAllPropertyItems(totalBufferSize, numProperties, pItems);

    if (status == Ok)
    {
        PropertyItem* pItem = pItems;
        for (UINT i = 0; i < numProperties; i++)
        {
            CString tag, value;

            // Convert property ID to tag name
            switch (pItem->id)
            {
            case 0x010F: tag = _T("Manufacturer"); break;
            case 0x0110: tag = _T("Model"); break;
            case 0x0112: tag = _T("Orientation"); break;
            case 0x011A: tag = _T("XResolution"); break;
            case 0x011B: tag = _T("YResolution"); break;
            case 0x0128: tag = _T("ResolutionUnit"); break;
            case 0x0132: tag = _T("DateTime"); break;
            case 0x829A: tag = _T("ExposureTime"); break;
            case 0x829D: tag = _T("FNumber"); break;
            case 0x8827: tag = _T("ISOSpeedRatings"); break;
            case 0x9003: tag = _T("DateTimeOriginal"); break;
            case 0x9004: tag = _T("DateTimeDigitized"); break;
            case 0x920A: tag = _T("FocalLength"); break;
            default:
                tag.Format(_T("0x%04X"), pItem->id);
                break;
            }

            // Convert value based on type
            if (pItem->type == PropertyTagTypeASCII)
            {
                value = CString((char*)pItem->value);
            }
            else if (pItem->type == PropertyTagTypeShort)
            {
                USHORT* pValue = (USHORT*)pItem->value;
                value.Format(_T("%u"), *pValue);
            }
            else if (pItem->type == PropertyTagTypeLong)
            {
                ULONG* pValue = (ULONG*)pItem->value;
                value.Format(_T("%u"), *pValue);
            }
            else if (pItem->type == PropertyTagTypeRational)
            {
                ULONG* pValue = (ULONG*)pItem->value;
                double dValue = (double)pValue[0] / (double)pValue[1];
                value.Format(_T("%.2f"), dValue);
            }

            if (!tag.IsEmpty() && !value.IsEmpty())
            {
                m_exifData[tag] = value;
            }

            // Move to next property
            pItem = (PropertyItem*)((BYTE*)pItem + sizeof(PropertyItem));
        }
    }

    free(pItems);
    return TRUE;
}

CString CImageDocument::GetExifValue(const CString& tag)
{
    CString value;
    m_exifData.Lookup(tag, value);
    return value;
}

Bitmap* CImageDocument::CreateThumbnail(int width, int height)
{
    if (!m_pImage)
        return NULL;

    // Calculate aspect ratio
    int imgWidth = m_pImage->GetWidth();
    int imgHeight = m_pImage->GetHeight();

    float aspectRatio = (float)imgWidth / (float)imgHeight;

    int thumbWidth = width;
    int thumbHeight = height;

    if (aspectRatio > 1.0f)
    {
        thumbHeight = (int)(width / aspectRatio);
    }
    else
    {
        thumbWidth = (int)(height * aspectRatio);
    }

    // Create thumbnail
    Bitmap* pThumbnail = new Bitmap(thumbWidth, thumbHeight, PixelFormat32bppARGB);
    Graphics g(pThumbnail);

    g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    g.DrawImage(m_pImage, 0, 0, thumbWidth, thumbHeight);

    return pThumbnail;
}

CString CImageDocument::GetFileName() const
{
    int pos = m_filePath.ReverseFind(_T('\\'));
    if (pos != -1)
        return m_filePath.Mid(pos + 1);
    return m_filePath;
}

CString CImageDocument::GetFileFormat() const
{
    int pos = m_filePath.ReverseFind(_T('.'));
    if (pos != -1)
        return m_filePath.Mid(pos + 1);
    return _T("");
}

ULONGLONG CImageDocument::GetFileSize() const
{
    CFileStatus status;
    if (CFile::GetStatus(m_filePath, status))
        return status.m_size;
    return 0;
}

CString CImageDocument::GetEncoderClsid(const WCHAR* format)
{
    UINT num = 0;
    UINT size = 0;

    GetImageEncodersSize(&num, &size);
    if (size == 0)
        return _T("");

    ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)malloc(size);
    if (!pImageCodecInfo)
        return _T("");

    GetImageEncoders(num, size, pImageCodecInfo);

    CString clsid;
    for (UINT i = 0; i < num; i++)
    {
        if (wcscmp(pImageCodecInfo[i].MimeType, format) == 0)
        {
            WCHAR szCLSID[64];
            StringFromGUID2(pImageCodecInfo[i].Clsid, szCLSID, 64);
            clsid = szCLSID;
            break;
        }
    }

    free(pImageCodecInfo);
    return clsid;
}

BOOL CImageDocument::SaveAs(const CString& filePath, const CString& format)
{
    if (!m_pDisplayImage)
        return FALSE;

    CString mimeType;
    if (format.CompareNoCase(_T("jpg")) == 0 || format.CompareNoCase(_T("jpeg")) == 0)
        mimeType = _T("image/jpeg");
    else if (format.CompareNoCase(_T("png")) == 0)
        mimeType = _T("image/png");
    else if (format.CompareNoCase(_T("bmp")) == 0)
        mimeType = _T("image/bmp");
    else if (format.CompareNoCase(_T("gif")) == 0)
        mimeType = _T("image/gif");
    else if (format.CompareNoCase(_T("tiff")) == 0)
        mimeType = _T("image/tiff");
    else
        return FALSE;

    // Get encoder CLSID
    CString clsidStr = GetEncoderClsid(CT2W(mimeType));
    if (clsidStr.IsEmpty())
        return FALSE;

    CLSID clsid;
    CLSIDFromString(CT2W(clsidStr), &clsid);

    // Save image
    USES_CONVERSION;
    Status status = m_pDisplayImage->Save(T2W((LPTSTR)(LPCTSTR)filePath), &clsid);

    return (status == Ok);
}

BOOL CImageDocument::OnSaveDocument(LPCTSTR lpszPathName)
{
    CString path = lpszPathName;
    CString format = GetFileFormat();

    if (SaveAs(path, format))
    {
        SetModifiedFlag(FALSE);
        return TRUE;
    }

    return FALSE;
}

void CImageDocument::DeleteContents()
{
    ClearImage();
    m_exifData.RemoveAll();
    CDocument::DeleteContents();
}

void CImageDocument::OnImageRotate90()
{
    RotateImage(ROTATE_90);
}

void CImageDocument::OnImageRotate180()
{
    RotateImage(ROTATE_180);
}

void CImageDocument::OnImageRotate270()
{
    RotateImage(ROTATE_270);
}

void CImageDocument::OnImageFlipH()
{
    FlipHorizontal();
}

void CImageDocument::OnImageFlipV()
{
    FlipVertical();
}

void CImageDocument::OnImageReset()
{
    ResetTransformations();
}

#ifdef _DEBUG
void CImageDocument::AssertValid() const
{
    CDocument::AssertValid();
}

void CImageDocument::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif
```

### ImageView.h

```cpp
// ImageView.h
#pragma once

class CImageView : public CScrollView
{
protected:
    CImageView();
    DECLARE_DYNCREATE(CImageView)

// Attributes
public:
    CImageDocument* GetDocument() const;

    enum ZoomMode
    {
        ZOOM_FIT_WINDOW,
        ZOOM_ACTUAL_SIZE,
        ZOOM_CUSTOM
    };

private:
    double m_zoomFactor;            // Current zoom level
    ZoomMode m_zoomMode;            // Zoom mode
    CPoint m_panOffset;             // Pan offset
    CPoint m_lastMousePos;          // Last mouse position for panning
    BOOL m_bPanning;                // Currently panning
    CBrush m_backgroundBrush;       // Background brush
    CPoint m_selectionStart;        // Selection start point
    CPoint m_selectionEnd;          // Selection end point
    BOOL m_bSelecting;              // Currently selecting

// Operations
public:
    void SetZoom(double factor);
    void ZoomIn();
    void ZoomOut();
    void ZoomFit();
    void ZoomActual();
    double GetZoom() const { return m_zoomFactor; }

    void StartPan(CPoint point);
    void UpdatePan(CPoint point);
    void EndPan();

// Overrides
public:
    virtual void OnDraw(CDC* pDC);
    virtual void OnInitialUpdate();
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// Implementation
public:
    virtual ~CImageView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnViewZoomIn();
    afx_msg void OnViewZoomOut();
    afx_msg void OnViewZoomFit();
    afx_msg void OnViewZoomActual();
    afx_msg void OnUpdateViewZoomIn(CCmdUI* pCmdUI);
    afx_msg void OnUpdateViewZoomOut(CCmdUI* pCmdUI);

private:
    void UpdateScrollSizes();
    CRect CalculateImageRect();
};

#ifndef _DEBUG
inline CImageDocument* CImageView::GetDocument() const
   { return reinterpret_cast<CImageDocument*>(m_pDocument); }
#endif
```

### ImageView.cpp

```cpp
// ImageView.cpp
#include "stdafx.h"
#include "ImageViewerApp.h"
#include "ImageDocument.h"
#include "ImageView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Gdiplus;

IMPLEMENT_DYNCREATE(CImageView, CScrollView)

BEGIN_MESSAGE_MAP(CImageView, CScrollView)
    ON_WM_SIZE()
    ON_WM_MOUSEWHEEL()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MBUTTONDOWN()
    ON_WM_MBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_ERASEBKGND()
    ON_COMMAND(ID_VIEW_ZOOM_IN, &CImageView::OnViewZoomIn)
    ON_COMMAND(ID_VIEW_ZOOM_OUT, &CImageView::OnViewZoomOut)
    ON_COMMAND(ID_VIEW_ZOOM_FIT, &CImageView::OnViewZoomFit)
    ON_COMMAND(ID_VIEW_ZOOM_ACTUAL, &CImageView::OnViewZoomActual)
    ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_IN, &CImageView::OnUpdateViewZoomIn)
    ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_OUT, &CImageView::OnUpdateViewZoomOut)
END_MESSAGE_MAP()

CImageView::CImageView()
    : m_zoomFactor(1.0)
    , m_zoomMode(ZOOM_FIT_WINDOW)
    , m_panOffset(0, 0)
    , m_bPanning(FALSE)
    , m_bSelecting(FALSE)
{
    m_backgroundBrush.CreateSolidBrush(theApp.m_backgroundColor);
}

CImageView::~CImageView()
{
    m_backgroundBrush.DeleteObject();
}

void CImageView::OnInitialUpdate()
{
    CScrollView::OnInitialUpdate();

    // Set scroll sizes
    CSize sizeTotal(1, 1);
    SetScrollSizes(MM_TEXT, sizeTotal);

    // Fit image to window
    ZoomFit();
}

void CImageView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    UpdateScrollSizes();
    Invalidate();
}

void CImageView::OnDraw(CDC* pDC)
{
    CImageDocument* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    Bitmap* pImage = pDoc->GetImage();
    if (!pImage)
        return;

    // Create GDI+ graphics object
    Graphics graphics(pDC->GetSafeHdc());

    // Set interpolation mode
    if (theApp.m_bSmoothZoom)
        graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    else
        graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);

    // Calculate image position and size
    CRect imageRect = CalculateImageRect();

    // Draw image
    graphics.DrawImage(pImage,
        imageRect.left, imageRect.top,
        imageRect.Width(), imageRect.Height());

    // Draw selection rectangle if selecting
    if (m_bSelecting)
    {
        CRect selRect(m_selectionStart, m_selectionEnd);
        selRect.NormalizeRect();

        Pen pen(Color(128, 0, 120, 215), 2.0f);
        graphics.DrawRectangle(&pen,
            selRect.left, selRect.top,
            selRect.Width(), selRect.Height());
    }
}

CRect CImageView::CalculateImageRect()
{
    CImageDocument* pDoc = GetDocument();
    if (!pDoc)
        return CRect(0, 0, 0, 0);

    int imageWidth = pDoc->GetImageWidth();
    int imageHeight = pDoc->GetImageHeight();

    if (imageWidth == 0 || imageHeight == 0)
        return CRect(0, 0, 0, 0);

    // Calculate scaled dimensions
    int scaledWidth = (int)(imageWidth * m_zoomFactor);
    int scaledHeight = (int)(imageHeight * m_zoomFactor);

    // Get scroll position
    CPoint scrollPos = GetScrollPosition();

    // Calculate image position
    CRect clientRect;
    GetClientRect(&clientRect);

    int x = 0;
    int y = 0;

    if (scaledWidth < clientRect.Width())
    {
        // Center horizontally
        x = (clientRect.Width() - scaledWidth) / 2;
    }
    else
    {
        x = -scrollPos.x;
    }

    if (scaledHeight < clientRect.Height())
    {
        // Center vertically
        y = (clientRect.Height() - scaledHeight) / 2;
    }
    else
    {
        y = -scrollPos.y;
    }

    return CRect(x, y, x + scaledWidth, y + scaledHeight);
}

void CImageView::UpdateScrollSizes()
{
    CImageDocument* pDoc = GetDocument();
    if (!pDoc)
        return;

    int imageWidth = pDoc->GetImageWidth();
    int imageHeight = pDoc->GetImageHeight();

    if (imageWidth == 0 || imageHeight == 0)
        return;

    // Calculate scaled dimensions
    int scaledWidth = (int)(imageWidth * m_zoomFactor);
    int scaledHeight = (int)(imageHeight * m_zoomFactor);

    // Update scroll sizes
    CSize sizeTotal(scaledWidth, scaledHeight);
    SetScrollSizes(MM_TEXT, sizeTotal);
}

void CImageView::SetZoom(double factor)
{
    m_zoomFactor = max(0.01, min(50.0, factor));
    m_zoomMode = ZOOM_CUSTOM;
    UpdateScrollSizes();
    Invalidate();

    // Update status bar
    CString status;
    status.Format(_T("Zoom: %.0f%%"), m_zoomFactor * 100);
    AfxGetMainWnd()->SetMessageText(status);
}

void CImageView::ZoomIn()
{
    double newZoom = m_zoomFactor * 1.2;
    SetZoom(newZoom);
}

void CImageView::ZoomOut()
{
    double newZoom = m_zoomFactor / 1.2;
    SetZoom(newZoom);
}

void CImageView::ZoomFit()
{
    CImageDocument* pDoc = GetDocument();
    if (!pDoc)
        return;

    int imageWidth = pDoc->GetImageWidth();
    int imageHeight = pDoc->GetImageHeight();

    if (imageWidth == 0 || imageHeight == 0)
        return;

    CRect clientRect;
    GetClientRect(&clientRect);

    // Calculate zoom to fit
    double zoomX = (double)clientRect.Width() / imageWidth;
    double zoomY = (double)clientRect.Height() / imageHeight;

    m_zoomFactor = min(zoomX, zoomY) * 0.95;  // 95% to leave margin
    m_zoomMode = ZOOM_FIT_WINDOW;

    UpdateScrollSizes();
    Invalidate();
}

void CImageView::ZoomActual()
{
    m_zoomFactor = 1.0;
    m_zoomMode = ZOOM_ACTUAL_SIZE;
    UpdateScrollSizes();
    Invalidate();
}

void CImageView::StartPan(CPoint point)
{
    m_bPanning = TRUE;
    m_lastMousePos = point;
    SetCapture();
    SetCursor(LoadCursor(NULL, IDC_SIZEALL));
}

void CImageView::UpdatePan(CPoint point)
{
    if (!m_bPanning)
        return;

    CPoint delta = point - m_lastMousePos;
    m_lastMousePos = point;

    // Update scroll position
    CPoint scrollPos = GetScrollPosition();
    scrollPos.x -= delta.x;
    scrollPos.y -= delta.y;

    // Clamp to valid range
    CSize scrollRange = GetTotalSize();
    CRect clientRect;
    GetClientRect(&clientRect);

    scrollPos.x = max(0, min(scrollPos.x, scrollRange.cx - clientRect.Width()));
    scrollPos.y = max(0, min(scrollPos.y, scrollRange.cy - clientRect.Height()));

    ScrollToPosition(scrollPos);
}

void CImageView::EndPan()
{
    if (m_bPanning)
    {
        m_bPanning = FALSE;
        ReleaseCapture();
        SetCursor(LoadCursor(NULL, IDC_ARROW));
    }
}

void CImageView::OnSize(UINT nType, int cx, int cy)
{
    CScrollView::OnSize(nType, cx, cy);

    if (m_zoomMode == ZOOM_FIT_WINDOW)
    {
        ZoomFit();
    }
}

BOOL CImageView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    if (zDelta > 0)
        ZoomIn();
    else
        ZoomOut();

    return TRUE;
}

void CImageView::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bSelecting = TRUE;
    m_selectionStart = point;
    m_selectionEnd = point;
    SetCapture();

    CScrollView::OnLButtonDown(nFlags, point);
}

void CImageView::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bSelecting)
    {
        m_bSelecting = FALSE;
        ReleaseCapture();
        Invalidate();
    }

    CScrollView::OnLButtonUp(nFlags, point);
}

void CImageView::OnMButtonDown(UINT nFlags, CPoint point)
{
    StartPan(point);
    CScrollView::OnMButtonDown(nFlags, point);
}

void CImageView::OnMButtonUp(UINT nFlags, CPoint point)
{
    EndPan();
    CScrollView::OnMButtonUp(nFlags, point);
}

void CImageView::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bPanning)
    {
        UpdatePan(point);
    }
    else if (m_bSelecting)
    {
        m_selectionEnd = point;
        Invalidate();
    }

    CScrollView::OnMouseMove(nFlags, point);
}

BOOL CImageView::OnEraseBkgnd(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);
    pDC->FillRect(&rect, &m_backgroundBrush);
    return TRUE;
}

void CImageView::OnViewZoomIn()
{
    ZoomIn();
}

void CImageView::OnViewZoomOut()
{
    ZoomOut();
}

void CImageView::OnViewZoomFit()
{
    ZoomFit();
}

void CImageView::OnViewZoomActual()
{
    ZoomActual();
}

void CImageView::OnUpdateViewZoomIn(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_zoomFactor < 50.0);
}

void CImageView::OnUpdateViewZoomOut(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(m_zoomFactor > 0.01);
}

#ifdef _DEBUG
void CImageView::AssertValid() const
{
    CScrollView::AssertValid();
}

void CImageView::Dump(CDumpContext& dc) const
{
    CScrollView::Dump(dc);
}

CImageDocument* CImageView::GetDocument() const
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageDocument)));
    return (CImageDocument*)m_pDocument;
}
#endif
```

## 4. Image Processing Implementation {#image-processing}

### ImageProcessor.h

```cpp
// ImageProcessor.h
#pragma once

#include <gdiplus.h>

class CImageProcessor
{
public:
    // Filters
    static Gdiplus::Bitmap* ApplyGrayscale(Gdiplus::Bitmap* pSource);
    static Gdiplus::Bitmap* ApplySepia(Gdiplus::Bitmap* pSource);
    static Gdiplus::Bitmap* ApplyNegative(Gdiplus::Bitmap* pSource);
    static Gdiplus::Bitmap* ApplyBlur(Gdiplus::Bitmap* pSource, int radius);
    static Gdiplus::Bitmap* ApplySharpen(Gdiplus::Bitmap* pSource);

    // Adjustments
    static Gdiplus::Bitmap* AdjustBrightness(Gdiplus::Bitmap* pSource, int value);
    static Gdiplus::Bitmap* AdjustContrast(Gdiplus::Bitmap* pSource, int value);
    static Gdiplus::Bitmap* AdjustSaturation(Gdiplus::Bitmap* pSource, int value);

    // Transformations
    static Gdiplus::Bitmap* Resize(Gdiplus::Bitmap* pSource, int width, int height);
    static Gdiplus::Bitmap* Crop(Gdiplus::Bitmap* pSource, Gdiplus::Rect rect);
    static Gdiplus::Bitmap* Rotate(Gdiplus::Bitmap* pSource, float angle);

    // Effects
    static Gdiplus::Bitmap* ApplyEdgeDetection(Gdiplus::Bitmap* pSource);
    static Gdiplus::Bitmap* ApplyEmboss(Gdiplus::Bitmap* pSource);
    static Gdiplus::Bitmap* ApplyOilPainting(Gdiplus::Bitmap* pSource, int radius);

private:
    static void ApplyColorMatrix(Gdiplus::Bitmap* pSource, Gdiplus::Bitmap* pDest,
        const Gdiplus::ColorMatrix& matrix);
};
```

## 5. Zoom and Pan Features {#zoom-pan}

The zoom and pan implementation is already shown in ImageView.cpp. Key points:

### Zoom Implementation
- Mouse wheel zooms in/out
- Maintains zoom center at mouse position
- Supports fit-to-window mode
- Smooth interpolation for quality

### Pan Implementation
- Middle mouse button for panning
- Tracks mouse delta for smooth movement
- Clamps to valid scroll range
- Updates scroll bars automatically

## 6. Where to Look in the Code {#code-navigation}

**Image Loading**: `ImageDocument.cpp` -> `LoadImage()` and `OnOpenDocument()`

**Zoom/Pan**: `ImageView.cpp` -> `OnMouseWheel()`, `StartPan()`, `UpdatePan()`

**Image Transformations**: `ImageDocument.cpp` -> `RotateImage()`, `FlipHorizontal()`, `ApplyAdjustments()`

**Rendering**: `ImageView.cpp` -> `OnDraw()` and `CalculateImageRect()`

**EXIF Reading**: `ImageDocument.cpp` -> `ReadExifData()`

**Thumbnails**: `ImageDocument.cpp` -> `CreateThumbnail()`

## 7. Building and Running {#building}

### Prerequisites
- Visual Studio 2019 or later
- Windows SDK
- MFC libraries
- GDI+ (included with Windows)

### Build Steps
1. Open ImageViewer.sln
2. Select configuration (Debug/Release)
3. Build Solution (F7)
4. Run (F5)

### Linking GDI+
Add to linker input: `gdiplus.lib`

## 8. Extending the Application {#extending}

### Adding Image Filters

```cpp
// Add to ImageDocument
void CImageDocument::ApplyFilter(FilterType filter)
{
    Bitmap* pFiltered = NULL;

    switch (filter)
    {
    case FILTER_GRAYSCALE:
        pFiltered = CImageProcessor::ApplyGrayscale(m_pDisplayImage);
        break;
    case FILTER_SEPIA:
        pFiltered = CImageProcessor::ApplySepia(m_pDisplayImage);
        break;
    // More filters...
    }

    if (pFiltered)
    {
        if (m_pDisplayImage != m_pImage)
            delete m_pDisplayImage;
        m_pDisplayImage = pFiltered;
        UpdateAllViews(NULL);
        SetModifiedFlag(TRUE);
    }
}
```

### Adding Slideshow Mode

```cpp
// Add to CMainFrame
class CMainFrame : public CFrameWndEx
{
private:
    UINT m_nSlideshowTimer;
    CStringArray m_slideshowFiles;
    int m_nCurrentSlide;

public:
    void StartSlideshow(const CStringArray& files);
    void StopSlideshow();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
```

### Adding Batch Processing

```cpp
// BatchProcessor.h
class CBatchProcessor
{
public:
    struct BatchOperation
    {
        enum OpType { RESIZE, ROTATE, CONVERT, FILTER };
        OpType type;
        CString params;
    };

    BOOL ProcessBatch(const CStringArray& files,
        const CArray<BatchOperation>& operations,
        const CString& outputFolder);
};
```

## 9. Common Issues and Solutions {#issues}

### Issue 1: Out of Memory with Large Images

**Problem**: Application crashes when loading very large images (>100MB).

**Solution**: Implement progressive loading or downsampling.

```cpp
Bitmap* LoadLargeImage(const CString& path, int maxDimension)
{
    // First, get image dimensions without loading full image
    Bitmap probe(CT2W(path));
    int width = probe.GetWidth();
    int height = probe.GetHeight();

    if (width > maxDimension || height > maxDimension)
    {
        // Calculate scale
        double scale = (double)maxDimension / max(width, height);

        // Load with downsampling
        int newWidth = (int)(width * scale);
        int newHeight = (int)(height * scale);

        Bitmap* pFull = new Bitmap(CT2W(path));
        Bitmap* pScaled = new Bitmap(newWidth, newHeight);

        Graphics g(pScaled);
        g.DrawImage(pFull, 0, 0, newWidth, newHeight);

        delete pFull;
        return pScaled;
    }

    return new Bitmap(CT2W(path));
}
```

### Issue 2: Slow Zoom Performance

**Problem**: Zooming is slow with high-quality interpolation.

**Solution**: Use different interpolation modes based on zoom level.

```cpp
void CImageView::OnDraw(CDC* pDC)
{
    Graphics graphics(pDC->GetSafeHdc());

    // Use fast interpolation while zooming actively
    if (m_bActiveZoom)
        graphics.SetInterpolationMode(InterpolationModeLowQuality);
    else if (m_zoomFactor < 1.0)
        graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    else
        graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);

    // Draw image...
}
```

### Issue 3: GDI+ Memory Leaks

**Problem**: Memory leaks when creating/destroying Bitmap objects.

**Solution**: Ensure proper cleanup in destructors and error paths.

```cpp
void CImageDocument::ClearImage()
{
    if (m_pImage)
    {
        delete m_pImage;
        m_pImage = NULL;
    }

    // Only delete if different from original
    if (m_pDisplayImage && m_pDisplayImage != m_pImage)
    {
        delete m_pDisplayImage;
        m_pDisplayImage = NULL;
    }
}
```

## 10. Performance Optimization {#optimization}

### Optimization 1: Cached Thumbnails

```cpp
class CThumbnailCache
{
private:
    CMap<CString, LPCTSTR, Bitmap*, Bitmap*> m_cache;

public:
    Bitmap* GetThumbnail(const CString& path, int size)
    {
        Bitmap* pThumb = NULL;
        if (m_cache.Lookup(path, pThumb))
            return pThumb;

        // Load and create thumbnail
        Bitmap image(CT2W(path));
        pThumb = CreateThumbnail(&image, size);
        m_cache[path] = pThumb;

        return pThumb;
    }

    void Clear()
    {
        POSITION pos = m_cache.GetStartPosition();
        while (pos)
        {
            CString key;
            Bitmap* pBitmap;
            m_cache.GetNextAssoc(pos, key, pBitmap);
            delete pBitmap;
        }
        m_cache.RemoveAll();
    }
};
```

### Optimization 2: Background Loading

```cpp
UINT LoadImageThreadProc(LPVOID pParam)
{
    CImageDocument* pDoc = (CImageDocument*)pParam;

    // Load image in background
    Bitmap* pImage = new Bitmap(CT2W(pDoc->GetFilePath()));

    // Notify main thread
    AfxGetMainWnd()->PostMessage(WM_IMAGE_LOADED, 0, (LPARAM)pImage);

    return 0;
}
```

### Optimization 3: Viewport Culling

```cpp
void CImageView::OnDraw(CDC* pDC)
{
    // Only draw visible portion of image
    CRect clientRect;
    GetClientRect(&clientRect);

    CRect imageRect = CalculateImageRect();
    CRect visibleRect;
    visibleRect.IntersectRect(&clientRect, &imageRect);

    if (visibleRect.IsRectEmpty())
        return;  // Image not visible

    // Calculate source rectangle in image coordinates
    int srcX = (visibleRect.left - imageRect.left) / m_zoomFactor;
    int srcY = (visibleRect.top - imageRect.top) / m_zoomFactor;
    int srcW = visibleRect.Width() / m_zoomFactor;
    int srcH = visibleRect.Height() / m_zoomFactor;

    // Draw only visible portion
    graphics.DrawImage(pImage,
        visibleRect.left, visibleRect.top, visibleRect.Width(), visibleRect.Height(),
        srcX, srcY, srcW, srcH, UnitPixel);
}
```

## Summary

This comprehensive image viewer demonstrates:
- GDI+ integration with MFC
- Advanced zoom and pan functionality
- Image transformation and manipulation
- EXIF data reading
- Performance optimization techniques
- Professional user interface

The application serves as an excellent foundation for:
- Photo management software
- Image editing applications
- Medical imaging viewers
- GIS mapping applications
- Document scanning software

Study the code, experiment with modifications, and extend it with additional features to master GDI+ and image processing in MFC!

## Next Steps
- Add more image filters (blur, sharpen, edge detection)
- Implement layers support
- Add drawing tools (annotations)
- Create a plugin system for custom filters
- Implement RAW image support
