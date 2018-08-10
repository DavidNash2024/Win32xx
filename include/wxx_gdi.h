// Win32++   Version 8.5.1
// Release Date: TBA
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2017  David Nash
//
// Permission is hereby granted, free of charge, to
// any person obtaining a copy of this software and
// associated documentation files (the "Software"),
// to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom
// the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice
// shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////


////////////////////////////////////////////////////////
// wxx_gdi.h
//  Declaration of the CDC class, and CBitmapInfoPtr class

// The CDC class provides a device context, along with the various associated
//  objects such as Bitmaps, Brushes, Bitmaps, Fonts and Pens. This class
//  handles the creation, selection, de-selection and deletion of these objects
//  automatically. It also automatically deletes or releases the device context
//  itself as appropriate. Any failure to create the new GDI object throws an
//  exception.
//
// The CDC class is sufficient for most GDI programming needs. Sometimes
//  however we need to have the GDI object separated from the device context.
//  Wrapper classes for GDI objects are provided for this purpose. The classes
//  are CBitmap, CBrush, CFont, CPalette, CPen and CRgn. These classes
//  automatically delete the GDI resource assigned to them when their destructor
//  is called. These wrapper class objects can be attached to the CDC as
//  shown below.
//
// Coding Example without CDC ...
//  void DrawLine()
//  {
//    HDC hdcClient = ::GetDC(GetHwnd());
//    HDC hdcMem = ::CreateCompatibleDC(hdcClient);
//    HBITMAP hBitmap = ::CreateCompatibleBitmap(hdcClient, cx, cy);
//    HBITMAP hOldBitmap = reinterpret_cast<HBITMAP>(::SelectObject(hdcMem, hBitmap));
//    HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(255,0,0);
//    HPEN hOldPen = reinterpret_cast<HPEN>(::SelectObject(hdcMem, hPen));
//    ::MoveToEx(hdcMem, 0, 0, NULL);
//    ::LineTo(hdcMem, 50, 50);
//    ::SelectObject(hdcMem, hOldPen);
//    ::DeleteObject(hPen);
//    hPen = ::CreatePen(PS_SOLID, 1, RGB(0,255,0);
//    hOldPen = reinterpret_cast<HPEN>(::SelectObject(hdcMem, hPen));
//    ::LineTo(hdcMem, 80, 80);
//    ::BitBlt(hdcClient, 0, 0, cx, cy, hdcMem, 0, 0);
//    ::SelectObject(hdcMem, hOldPen);
//    ::DeleteObject(hPen);
//    ::SelectObject(hdcMem, hOldBitmap);
//    ::DeleteObject(hBitmap);
//    ::DeleteDC(hdcMem);
//    ::ReleaseDC(GetHwnd(), hdcClient);
//  }
//
// Coding Example with CDC classes ...
//  void DrawLine()
//  {
//    CClientDC dcClient(*this)
//    CMemDC dcMem(dcClient);
//    dcMem.CreateCompatibleBitmap(dcClient, cx, cy);
//    dcMem.CreatePen(PS_SOLID, 1, RGB(255,0,0);
//    dcMem.MoveTo(0, 0);
//    dcMem.LineTo(50, 50);
//    dcMem.CreatePen(PS_SOLID, 1, RGB(0,255,0));
//    dcMem.LineTo(80,80);
//    dcClient.BitBlt(0, 0, cx, cy, dcMem, 0, 0);
//  }
//
// Coding Example with CDC classes and CPen ...
//  void DrawLine()
//  {
//    CClientDC dcClient(*this)
//    CMemDC dcMem(dcClient);
//    dcMem.CreateCompatibleBitmap(dcClient, cx, cy);
//    CPen MyPen(PS_SOLID, 1, RGB(255,0,0));
//    CPen OldPen = dcMem.SelectObject(MyPen);
//    dcMem.MoveTo(0, 0);
//    dcMem.LineTo(50, 50);
//
//    // Only need to select the OldPen if MyPen is changed while its selected into dcMem.
//    dcMem.SelectObject(OldPen);
//
//    // Change MyPen and then re-select it into dcMem
//    MyPen.CreatePen(PS_SOLID, 1, RGB(0,255,0));
//    dcMem.SelectObject(MyPen);
//
//    dcMem.LineTo(80,80);
//    dcClient.BitBlt(0, 0, cx, cy, dcMem, 0, 0);
//  }

// Notes:
//  * When the CDC object drops out of scope, its destructor is called, releasing
//     or deleting the device context if Win32++ created it.
//  * When the destructor for CBitmap, CBrush, CPalette, CPen and CRgn are called,
//     the destructor is called deleting their GDI object if Win32++ created it.
//  * When the CDC object's destructor is called, any GDI objects created by one of
//     the CDC member functions (CDC::CreatePen for example) will be deleted.
//  * Bitmaps can only be selected into one device context at a time.
//  * Other GDI resources can be selected into more than one device context at a time.
//  * Palettes use SelectPalatte to select them into device the context.
//  * Regions use SelectClipRgn to select them into the device context.
//  * All the GDI classes are reference counted. This allows functions to safely
//     pass these objects by value, as well as by pointer or by reference.
//  * If SelectObject is used to select say a CPen into a device context, the
//     CPen shouldn't be changed while device context is valid. Use SelectObject to
//     select the old pen back into the device context before changing the pen.

// The CBitmapInfoPtr class is a convenient wrapper for the BITMAPINFO structure.
// The size of the BITMAPINFO structure is dependant on the type of HBITMAP, and its
// space needs to be allocated dynamically. CBitmapInfoPtr automatically allocates
// and deallocates the memory for the structure. A CBitmapInfoPtr object can be
// used anywhere in place of a LPBITMAPINFO. LPBITMAPINFO is used in functions like
// GetDIBits and SetDIBits.
//
// Coding example ...
//  CDC dcMem = CreateCompatibleDC(NULL);
//  CBitmapInfoPtr pbmi(hBitmap);
//  dcMem.GetDIBits(hBitmap, 0, pbmi->bmiHeader.biHeight, NULL, pbmi, DIB_RGB_COLORS);


#ifndef _WIN32XX_GDI_H_
#define _WIN32XX_GDI_H_


#include "wxx_appcore0.h"
#include "wxx_wincore0.h"
#include "wxx_exception.h"
#include "wxx_metafile.h"


// Disable macros from Windowsx.h
#undef CopyRgn

namespace Win32xx
{

    ///////////////////////////////////////////////
    // CObject provides the functionality of a HGDIOBJECT
    // This is the base class for other classes managing GDI objects.
    class CGDIObject
    {
    public:
        CGDIObject();
        CGDIObject(const CGDIObject& rhs);
        virtual ~CGDIObject();
        CGDIObject& operator = (const CGDIObject& rhs);
        void operator = (const HGDIOBJ hObject);

        void    Attach(HGDIOBJ hObject);
        void    DeleteObject();
        HGDIOBJ Detach();
        HGDIOBJ GetHandle() const;
        int     GetObject(int count, LPVOID pObject) const;

    protected:
        void    Release();
        void SetManaged(bool IsManaged) const { m_pData->isManagedObject = IsManaged; }

    private:
        void    AddToMap();
        BOOL    RemoveFromMap();

        CGDI_Data* m_pData;
    };


    ///////////////////////////////////////////////
    // CBitmap provides the functionality of a bitmap GDI object.
    class CBitmap : public CGDIObject
    {

      public:
        CBitmap();
        CBitmap(HBITMAP hBitmap);
        CBitmap(LPCTSTR pResName);
        CBitmap(int resourceID);
        operator HBITMAP() const;
        virtual ~CBitmap();

        // Create and load methods
        BOOL LoadBitmap(LPCTSTR pResName);
        BOOL LoadBitmap(int resID);
        BOOL LoadImage(LPCTSTR pResName, UINT flags);
        BOOL LoadImage(UINT resID, UINT flags);
        BOOL LoadOEMBitmap(UINT bitmapID);
        HBITMAP CreateBitmap(int width, int height, UINT planes, UINT bitsPerPixel, LPCVOID pBits);
        HBITMAP CreateCompatibleBitmap(HDC hdc, int width, int height);
        HBITMAP CreateDIBSection(HDC hdc, const LPBITMAPINFO pBMI, UINT colorUse, LPVOID* pBits, HANDLE hSection, DWORD offset);

#ifndef _WIN32_WCE
        void    ConvertToDisabled(COLORREF mask) const;
        HBITMAP CreateDIBitmap(HDC hdc, const BITMAPINFOHEADER* pBMIH, DWORD init, LPCVOID pInit, const LPBITMAPINFO pBMI, UINT colorUse);
        HBITMAP CreateMappedBitmap(UINT bitmapID, UINT flags = 0, LPCOLORMAP pColorMap = NULL, int mapSize = 0);
        HBITMAP CreateBitmapIndirect(const BITMAP& bitmap);
        void GrayScaleBitmap();
        void TintBitmap (int cRed, int cGreen, int cBlue);
        int GetDIBits(HDC hdc, UINT startScan, UINT scanLines,  LPVOID pBits, LPBITMAPINFO pBMI, UINT colorUse) const;
        int SetDIBits(HDC hdc, UINT startScan, UINT scanLines, LPCVOID pBits, const LPBITMAPINFO pBMI, UINT colorUse) const;
        CSize GetBitmapDimensionEx() const;
        CSize SetBitmapDimensionEx(int width, int height) const;
#endif // !_WIN32_WCE

        // Attributes
        BITMAP GetBitmapData() const;
    };


    ///////////////////////////////////////////////
    // CBrush provides the functionality of a brush GDI object.
    class CBrush : public CGDIObject
    {
      public:
        CBrush();
        CBrush(HBRUSH hBrush);
        CBrush(COLORREF color);
        operator HBRUSH() const;
        virtual ~CBrush();

        HBRUSH CreateSolidBrush(COLORREF color);
        HBRUSH CreatePatternBrush(HBITMAP hBitmap);
        LOGBRUSH GetLogBrush() const;

#ifndef _WIN32_WCE
        HBRUSH CreateHatchBrush(int index, COLORREF color);
        HBRUSH CreateBrushIndirect(const LOGBRUSH& logBrush);
        HBRUSH CreateDIBPatternBrush(HGLOBAL hDIBPacked, UINT colorSpec);
        HBRUSH CreateDIBPatternBrushPt(LPCVOID pPackedDIB, UINT usage);
#endif // !defined(_WIN32_WCE)

    };


    ///////////////////////////////////////////////
    // CFont provides the functionality of a font GDI object.
    class CFont : public CGDIObject
    {
    public:
        CFont();
        CFont(HFONT hFont);
        CFont(const LOGFONT& logFont);
        operator HFONT() const;
        virtual ~CFont();

        // Create methods
        HFONT CreateFontIndirect(const LOGFONT& logFont);
        HFONT CreatePointFont(int pointSize, LPCTSTR pFaceName, HDC hdc = NULL, BOOL isBold = FALSE, BOOL isItalic = FALSE);
        HFONT CreatePointFontIndirect(const LOGFONT& logFont, HDC hdc = NULL);

#ifndef _WIN32_WCE
        HFONT CreateFont(int height, int width, int escapement,
                int orientation, int weight, DWORD italic, DWORD underline,
                DWORD strikeOut, DWORD charSet, DWORD outPrecision,
                DWORD clipPrecision, DWORD quality, DWORD pitchAndFamily,
                LPCTSTR pFacename);
#endif // #ifndef _WIN32_WCE

        // Attributes
        LOGFONT GetLogFont() const;
    };


    ///////////////////////////////////////////////
    // CPalette provides the functionality of a palette GDI object.
    class CPalette : public CGDIObject
    {
      public:
        CPalette();
        CPalette(HPALETTE hPalette);
        operator HPALETTE() const;
        virtual ~CPalette();

        // Create methods
        HPALETTE CreatePalette(LPLOGPALETTE pLogPalette);

#ifndef _WIN32_WCE
        HPALETTE CreateHalftonePalette(HDC hdc);
#endif // !_WIN32_WCE

        // Attributes
        int GetEntryCount() const;
        UINT GetPaletteEntries(UINT startIndex, UINT entries, LPPALETTEENTRY pPaletteColors) const;
        UINT SetPaletteEntries(UINT nStartIndex, UINT entries, LPPALETTEENTRY pPaletteColors) const;

        // Operations
#ifndef _WIN32_WCE
        BOOL ResizePalette(UINT entries) const;
        void AnimatePalette(UINT startIndex, UINT entries, LPPALETTEENTRY pPaletteColors) const;
#endif // !_WIN32_WCE

        UINT GetNearestPaletteIndex (COLORREF color) const;

    };


    ///////////////////////////////////////////////
    // CPen provides the functionality of a pen GDI object.
    class CPen : public CGDIObject
    {
    public:
        CPen();
        CPen(HPEN hPen);
        CPen(int penStyle, int width, COLORREF color);
#ifndef _WIN32_WCE
        CPen(int penStyle, int width, const LOGBRUSH& logBrush, int styleCount = 0, const DWORD* pStyle = NULL);
#endif // !_WIN32_WCE
        operator HPEN() const;
        virtual ~CPen();

        HPEN CreatePen(int penStyle, int width, COLORREF color);
        HPEN CreatePenIndirect(const LOGPEN& logPen);
        LOGPEN GetLogPen() const;

#ifndef _WIN32_WCE
        HPEN ExtCreatePen(int penStyle, int width, const LOGBRUSH& logBrush, int styleCount = 0, const DWORD* pStyle = NULL) ;
        EXTLOGPEN GetExtLogPen() const;
#endif // !_WIN32_WCE

    };


    ///////////////////////////////////////////////
    // CRgn provides the functionality of a region GDI object.
    class CRgn : public CGDIObject
    {
      public:
        CRgn();
        CRgn(HRGN hRgn);
        operator HRGN() const;
        virtual ~CRgn ();

        // Create methods
        HRGN CreateRectRgn(int x1, int y1, int x2, int y2);
        HRGN CreateRectRgnIndirect(const RECT& rc);
        HRGN CreateFromData(const XFORM* pXForm, int count, const RGNDATA* pRgnData);

#ifndef _WIN32_WCE
        HRGN CreateEllipticRgn(int x1, int y1, int x2, int y2);
        HRGN CreateEllipticRgnIndirect(const RECT& rc);
        HRGN CreatePolygonRgn(LPPOINT pPoints, int count, int mode);
        HRGN CreatePolyPolygonRgn(LPPOINT pPoints, LPINT pPolyCounts, int count, int polyFillMode);
        HRGN CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3);
        HRGN CreateFromPath(HDC hDC);
#endif // !_WIN32_WCE

        // Operations
        void SetRectRgn(int x1, int y1, int x2, int y2) const;
        void SetRectRgn(const RECT& rc) const;
        int CombineRgn(HRGN hSrc1, HRGN Src2, int combineMode) const;
        int CombineRgn(HRGN hSrc, int combineMode) const;
        int CopyRgn(HRGN hSrc) const;
        BOOL EqualRgn(HRGN hRgn) const;
        int OffsetRgn(int x, int y) const;
        int OffsetRgn(POINT& pt) const;
        int GetRgnBox(RECT& rc) const;
        BOOL PtInRegion(int x, int y) const;
        BOOL PtInRegion(POINT& pt) const;
        BOOL RectInRegion(const RECT& rc) const;
        int GetRegionData(LPRGNDATA pRgnData, int dataSize) const;
    };


    // A structure that contains the data members for CDC.
    struct CDC_Data
    {
        // Constructor
        CDC_Data() : hDC(0), count(1L), isManagedHDC(FALSE), hWnd(0), savedDCState(0) {}

        std::vector<GDIPtr> m_vGDIObjects;  // Smart pointers to internally created Bitmaps, Brushes, Fonts, Bitmaps and Regions
        CBitmap bitmap;
        CBrush  brush;
        CFont   font;
        CPalette palette;
        CPen    pen;
        CRgn    rgn;
        HDC     hDC;            // The HDC belonging to this CDC
        long    count;          // Reference count
        bool    isManagedHDC;   // Delete/Release the HDC on destruction
        HWND    hWnd;           // The HWND of a Window or Client window DC
        int     savedDCState;  // The save state of the HDC.
    };


    ///////////////////////////////////////////////
    // CDC provides the functionality of a GDI device context.
    class CDC
    {
        friend class CWinApp;
        friend class CWnd;

    public:
        CDC();                                  // Constructs a new CDC without assigning a HDC
        CDC(HDC hDC, HWND hWnd = 0);            // Constructs a new CDC and assigns a HDC
        CDC(const CDC& rhs);                    // Constructs a new copy of the CDC
        virtual ~CDC();
        operator HDC() const { return m_pData->hDC; }   // Converts a CDC to a HDC
        CDC& operator = (const CDC& rhs);       // Assigns a CDC to an existing CDC

        void Attach(HDC hDC, HWND hWnd = 0);
        void Destroy();
        HDC  Detach();
        HDC GetHDC() const { return m_pData->hDC; }
        BOOL RestoreDC(int savedDC) const;
        int SaveDC() const;
        HGDIOBJ SelectObject(HGDIOBJ hObject) const;
        HBITMAP SelectObject(HBITMAP hBitmap) const;
        HBRUSH SelectObject(HBRUSH hBrush) const;
        HFONT SelectObject(HFONT hFont) const;
        HPEN SelectObject(HPEN hPen) const;
        int SelectObject(HRGN hRgn) const;

#ifndef _WIN32_WCE
        void operator = (const HDC hDC);
#endif

        // Initialization
        HDC CreateCompatibleDC(HDC hSource);
        HDC CreateDC(LPCTSTR pDriver, LPCTSTR pDevice, LPCTSTR pOutput, const DEVMODE* pInitData);
        int GetDeviceCaps(int index) const;
#ifndef _WIN32_WCE
        HDC CreateIC(LPCTSTR pDriver, LPCTSTR pDevice, LPCTSTR pOutput, const DEVMODE* pInitData);
#endif

        // Create Bitmaps
        void CreateBitmap(int cx, int cy, UINT planes, UINT bitsPerPixel, LPCVOID pColors);
        void CreateCompatibleBitmap(HDC hdc, int cx, int cy);
        void CreateDIBSection(HDC hdc, const LPBITMAPINFO pBMI, UINT usage, LPVOID* pBits,
                                        HANDLE hSection, DWORD offset);
        CBitmap DetachBitmap();
        BITMAP  GetBitmapData() const;
        HBITMAP GetCurrentBitmap() const;
        BOOL LoadBitmap(UINT id);
        BOOL LoadBitmap(LPCTSTR pResName);
        BOOL LoadImage(UINT id, UINT flags);
        BOOL LoadImage(LPCTSTR pResName, UINT flags);
        BOOL LoadOEMBitmap(UINT bitmapID); // for OBM_/OCR_/OIC

#ifndef _WIN32_WCE
        void CreateBitmapIndirect(const BITMAP& bitmap);
        void CreateDIBitmap(HDC hdc, const BITMAPINFOHEADER& pBMIH, DWORD init, LPCVOID pInit,
                                        const LPBITMAPINFO pBMI, UINT usage);
        void CreateMappedBitmap(UINT BitmapID, UINT flags /*= 0*/, LPCOLORMAP pColorMap /*= NULL*/, int mapSize /*= 0*/);
#endif

        // Create Brushes
        void CreatePatternBrush(HBITMAP hBitmap);
        void CreateSolidBrush(COLORREF color);
        HBRUSH GetCurrentBrush() const;
        LOGBRUSH GetLogBrush() const;

#ifndef _WIN32_WCE
        void CreateBrushIndirect(const LOGBRUSH& logBrush);
        void CreateHatchBrush(int fnStyle, COLORREF color);
        void CreateDIBPatternBrush(HGLOBAL hDIBPacked, UINT colorSpec);
        void CreateDIBPatternBrushPt(LPCVOID pPackedDIB, UINT usage);
#endif

        // Create Fonts
        void CreateFontIndirect(const LOGFONT& lf);
        HFONT GetCurrentFont() const;
        LOGFONT GetLogFont() const;

#ifndef _WIN32_WCE
        void CreateFont(int height, int width, int escapement, int orientation, int weight,
                            DWORD italic, DWORD underline, DWORD strikeOut, DWORD charSet,
                            DWORD outputPrecision, DWORD clipPrecision, DWORD quality,
                            DWORD pitchAndFamily, LPCTSTR pFaceName);
#endif

        // Create and select Palettes
        void CreatePalette(LPLOGPALETTE pLogPalette, BOOL forceBkgnd);
        HPALETTE SelectPalette(HPALETTE hPalette, BOOL forceBkgnd);

#ifndef _WIN32_WCE
        void CreateHalftonePalette(BOOL forceBkgnd);
#endif

        // Create Pens
        void CreatePen(int style, int width, COLORREF color);
        void CreatePenIndirect(const LOGPEN& logPen);
        HPEN GetCurrentPen() const;
        LOGPEN GetLogPen() const;

        // Retrieve and Select Stock Objects
        HGDIOBJ GetStockObject(int index) const;
        HGDIOBJ SelectStockObject(int index) const;

        // Create Regions
        int CreateRectRgn(int left, int top, int right, int bottom);
        int CreateRectRgnIndirect(const RECT& rc);
        int CreateFromData(const XFORM* pXform, DWORD count, const RGNDATA* pRgnData);

#ifndef _WIN32_WCE
        int CreateEllipticRgn(int left, int top, int right, int bottom);
        int CreateEllipticRgnIndirect(const RECT& rc);
        int CreatePolygonRgn(LPPOINT pPointArray, int points, int polyFillMode);
        int CreatePolyPolygonRgn(LPPOINT pPointArray, LPINT pPolyCounts, int count, int polyFillMode);
#endif

        // Wrappers for WinAPI functions
#ifndef _WIN32_WCE
        int EnumObjects(int objectType, GOBJENUMPROC pObjectFunc, LPARAM lparam) const;
#endif

        // Point and Line Drawing Functions
        CPoint GetCurrentPosition() const;
        CPoint MoveTo(int x, int y) const;
        CPoint MoveTo(POINT pt) const;
        BOOL LineTo(int x, int y) const;
        BOOL LineTo(POINT pt) const;
        COLORREF GetPixel(int x, int y) const;
        COLORREF GetPixel(POINT pt) const;
        COLORREF SetPixel(int x, int y, COLORREF color) const;
        COLORREF SetPixel(POINT pt, COLORREF color) const;
        int SetROP2(int drawMode) const;

#ifndef _WIN32_WCE
        BOOL Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const;
        BOOL Arc(const RECT& rc, POINT start, POINT end) const;
        BOOL ArcTo(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const;
        BOOL ArcTo(const RECT& rc, POINT start, POINT end) const;
        BOOL AngleArc(int x, int y, int radius, float startAngle, float sweepAngle) const;
        BOOL CloseFigure() const;
        int  GetROP2() const;
        int  GetArcDirection() const;
        int  SetArcDirection(int arcDirection) const;
        BOOL PolyDraw(const POINT* pPointArray, const BYTE* pTypes, int count) const;
        BOOL Polyline(LPPOINT pPointArray, int count) const;
        BOOL PolyPolyline(const POINT* pPointArray, const DWORD* pPolyPoints, int count) const;
        BOOL PolylineTo(const POINT* pPointArray, int count) const;
        BOOL PolyBezier(const POINT* pPointArray, int count) const;
        BOOL PolyBezierTo(const POINT* pPointArray, int count) const;
        BOOL SetPixelV(int x, int y, COLORREF color) const;
        BOOL SetPixelV(POINT pt, COLORREF color) const;
#endif

        // Shape Drawing Functions
        void DrawFocusRect(const RECT& rc) const;
        BOOL Ellipse(int x1, int y1, int x2, int y2) const;
        BOOL Ellipse(const RECT& rc) const;
        BOOL Polygon(LPPOINT pPointArray, int count) const;
        BOOL Rectangle(int x1, int y1, int x2, int y2) const;
        BOOL Rectangle(const RECT& rc) const;
        BOOL RoundRect(int x1, int y1, int x2, int y2, int width, int height) const;
        BOOL RoundRect(const RECT& rc, int width, int height) const;

#ifndef _WIN32_WCE
        BOOL Chord(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const;
        BOOL Chord(const RECT& rc, POINT start, POINT end) const;
        BOOL Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const;
        BOOL Pie(const RECT& rc, POINT start, POINT end) const;
        BOOL PolyPolygon(LPPOINT pPointArray, LPINT pPolyCounts, int count) const;
#endif

        // Fill and Image Drawing functions
        BOOL DrawEdge(const RECT& rc, UINT edge, UINT flags) const;
        BOOL DrawIconEx(int xLeft, int yTop, HICON hIcon, int cxWidth, int cyWidth,
                        UINT index, HBRUSH hFlickerFreeDraw, UINT flags) const;

        BOOL DrawFrameControl(const RECT& rc, UINT type, UINT state) const;
        BOOL FillRect(const RECT& rc, HBRUSH hBrush) const;
        BOOL FillRgn(HRGN hRgn, HBRUSH hBrush) const;
        void GradientFill(COLORREF Color1, COLORREF Color2, const RECT& rc, BOOL isVertical) const;
        BOOL InvertRect(const RECT& rc) const;
        void SolidFill(COLORREF Color, const RECT& rc) const;

#if (WINVER >= 0x0410)
        BOOL GradientFill(PTRIVERTEX pVertex, ULONG vertex, PVOID pMesh, ULONG mesh, ULONG mode) const;
#endif

#ifndef _WIN32_WCE
        BOOL DrawIcon(int x, int y, HICON hIcon) const;
        BOOL DrawIcon(POINT point, HICON hIcon) const;
        BOOL FrameRect(const RECT& rc, HBRUSH hBrush) const;
        BOOL FrameRgn(HRGN hRgn, HBRUSH hBrush, int width, int height) const;
        int  GetPolyFillMode() const;
        BOOL PaintRgn(HRGN hRgn) const;
        int  SetPolyFillMode(int polyFillMode) const;
#endif

        // Bitmap Functions
        BOOL BitBlt(int x, int y, int width, int height, HDC hSrc, int xSrc, int ySrc, DWORD rop) const;
        void DrawBitmap(int x, int y, int cx, int cy, HBITMAP hImage, COLORREF mask) const;
        BOOL MaskBlt(int xDest, int yDest, int width, int height, HDC hSrc,
                           int xSrc, int ySrc, HBITMAP hMask, int xMask, int yMask,
                           DWORD rop) const;

        BOOL PatBlt(int x, int y, int width, int height, DWORD rop) const;
        int  StretchDIBits(int xDest, int yDest, int destWidth, int destHeight,
                           int xSrc, int ySrc, int srcWidth, int srcHeight,
                           LPCVOID pBits, const LPBITMAPINFO pBMI, UINT usage, DWORD rop) const;

        BOOL StretchBlt(int x, int y, int width, int height, HDC hSrc,
                           int xSrc, int ySrc, int srcWidth, int srcHeight,
                           DWORD rop) const;

#ifndef _WIN32_WCE
        BOOL ExtFloodFill(int x, int y, COLORREF color, UINT fillType) const;
        BOOL FloodFill(int x, int y, COLORREF color) const;
        int  GetDIBits(HBITMAP hBitmap, UINT startScan, UINT scanLines, LPVOID pBits,
                        LPBITMAPINFO pBMI, UINT usage) const;

        int  GetStretchBltMode() const;
        int  SetDIBits(HBITMAP hBitmap, UINT startScan, UINT scanLines, LPCVOID pBits,
                        LPBITMAPINFO pBMI, UINT colorUse) const;

        int  SetStretchBltMode(int stretchMode) const;
  #if (WINVER >= 0x0410)
        BOOL TransparentBlt(int x, int y, int width, int height, HDC hSrc,
                           int xSrc, int ySrc, int widthSrc, int heightSrc,
                           UINT transparent) const;
  #endif
#endif

        // Brush Functions
#if (_WIN32_WINNT >= 0x0500)
        COLORREF GetDCBrushColor() const;
        COLORREF SetDCBrushColor(COLORREF color) const;
#endif

        // Font Functions
#ifndef _WIN32_WCE
        DWORD GetFontData(DWORD table, DWORD offset, LPVOID pBuffer,  DWORD data) const;
        DWORD GetFontLanguageInfo() const;
        DWORD GetGlyphOutline(UINT query, UINT format, LPGLYPHMETRICS pGM, DWORD bufferSize,
                              LPVOID pBuffer, const MAT2* pMAT2) const;

        DWORD GetKerningPairs(DWORD numPairs, LPKERNINGPAIR pKrnPair) const;
        DWORD SetMapperFlags(DWORD flag) const;
#endif

        // Palette and color functions
        HPALETTE GetCurrentPalette() const;
        COLORREF GetNearestColor(COLORREF color) const;
        void RealizePalette() const;

#ifndef _WIN32_WCE
        BOOL GetColorAdjustment(LPCOLORADJUSTMENT pCA) const;
        BOOL SetColorAdjustment(const COLORADJUSTMENT* pCA) const;
        BOOL UpdateColors() const;
#endif

#ifndef _WIN32_WCE
        // Pen Functions
        BOOL GetMiterLimit(PFLOAT limit) const;
        BOOL SetMiterLimit(FLOAT newLimit, PFLOAT oldLimit) const;
#endif

        // Clipping and Region Functions
        int  ExcludeClipRect(int left, int top, int right, int bottom) const;
        int  ExcludeClipRect(const RECT& rc) const;
        int  GetClipBox(RECT& rc) const;
        int  IntersectClipRect(int left, int top, int right, int bottom) const;
        int  IntersectClipRect(const RECT& rc) const;
        BOOL RectVisible(const RECT& rc) const;
        int  SelectClipRgn(HRGN hRgn) const;

#ifndef _WIN32_WCE
        BOOL BeginPath() const;
        BOOL EndPath() const;
        int  ExtSelectClipRgn(HRGN hRgn, int mode) const;
        BOOL FlattenPath() const;
        int  GetPath(POINT* pPointArray, BYTE* pTypes, int count) const;
        int  OffsetClipRgn(int xOffset, int yOffset) const;
        BOOL PtVisible(int x, int y) const;
        BOOL SelectClipPath(int mode) const;
        BOOL WidenPath() const;
#endif

        // Co-ordinate Functions
#ifndef _WIN32_WCE
        BOOL DPtoLP(LPPOINT pPointArray, int count)  const;
        BOOL DPtoLP(RECT& rc)  const;
        BOOL LPtoDP(LPPOINT pPointArray, int count)  const;
        BOOL LPtoDP(RECT& rc)  const;
#endif

        // Layout Functions
        DWORD GetLayout() const;
        DWORD SetLayout(DWORD layout) const;

        // Mapping functions
#ifndef _WIN32_WCE
        int  GetMapMode() const;
        int  SetMapMode(int mapMode) const;
        BOOL GetViewportOrgEx(LPPOINT pPoint)  const;
        BOOL SetViewportOrgEx(int x, int y, LPPOINT pPoint = NULL) const;
        BOOL SetViewportOrgEx(POINT point, LPPOINT pPointRet = NULL) const;
        BOOL OffsetViewportOrgEx(int width, int height, LPPOINT pPoint = NULL) const;
        BOOL GetViewportExtEx(LPSIZE pSize)  const;
        BOOL SetViewportExtEx(int x, int y, LPSIZE pSize) const;
        BOOL SetViewportExtEx(SIZE size, LPSIZE pSizeRet) const;
        BOOL ScaleViewportExtEx(int xNum, int xDenom, int yNum, int yDenom, LPSIZE pSize) const;
        BOOL GetWindowExtEx(LPSIZE pSize)  const;
        BOOL SetWindowExtEx(int x, int y, LPSIZE pSize) const;
        BOOL SetWindowExtEx(SIZE size, LPSIZE pSizeRet) const;
        BOOL ScaleWindowExtEx(int xNum, int xDenom, int yNum, int yDenom, LPSIZE pSize) const;
        BOOL GetWindowOrgEx(LPPOINT pPoint)  const;
        BOOL SetWindowOrgEx(int x, int y, LPPOINT pPoint) const;
        BOOL SetWindowOrgEx(POINT point, LPPOINT pPointRet) const;
        BOOL OffsetWindowOrgEx(int width, int height, LPPOINT pPoint) const;
#endif

        // MetaFile Functions
#ifndef _WIN32_WCE
        BOOL PlayMetaFile(HMETAFILE hMetaFile) const;
        BOOL PlayMetaFile(HENHMETAFILE hEnhMetaFile, const RECT& bounds) const;
#endif

        // Printer Functions
        int StartDoc(LPDOCINFO pDocInfo) const;
        int EndDoc() const;
        int StartPage() const;
        int EndPage() const;
        int AbortDoc() const;
        int SetAbortProc(BOOL (CALLBACK* pfn)(HDC, int)) const;

        // Text Functions
        int   DrawText(LPCTSTR pString, int count, const RECT& rc, UINT format) const;
        BOOL  ExtTextOut(int x, int y, UINT options, const RECT& rc, LPCTSTR pString, int count = -1, LPINT pDxWidths = NULL) const;
        COLORREF GetBkColor() const;
        int   GetBkMode() const;
        UINT  GetTextAlign() const;
        int   GetTextFace(int count, LPTSTR pFacename) const;
        COLORREF GetTextColor() const;
        BOOL  GetTextMetrics(TEXTMETRIC& metrics) const;
        COLORREF SetBkColor(COLORREF color) const;
        int   SetBkMode(int bkMode) const;
        UINT  SetTextAlign(UINT flags) const;
        COLORREF SetTextColor(COLORREF color) const;

#ifndef _WIN32_WCE
        int   DrawTextEx(LPTSTR pString, int count, const RECT& rc, UINT format, LPDRAWTEXTPARAMS pDTParams) const;
        DWORD GetCharacterPlacement(LPCTSTR pString, int count, int maxExtent,
                                    LPGCP_RESULTS pResults, DWORD flags) const;

        BOOL  GetCharABCWidths(UINT firstChar, UINT lastChar, LPABC pABC) const;
        BOOL  GetCharWidth(UINT firstChar, UINT lastChar, float* pBuffer) const;
        CSize GetTabbedTextExtent(LPCTSTR pString, int count, int tabPositions, LPINT pTabStopPositions) const;
        int   GetTextCharacterExtra() const;
        CSize GetTextExtentPoint32(LPCTSTR pString, int count) const;
        CSize GetTextExtentPoint32(LPCTSTR pString) const;
        BOOL  GrayString(HBRUSH hBrush, GRAYSTRINGPROC pOutputFunc, LPARAM pData, int count, int x, int y, int width, int height) const;
        int   SetTextCharacterExtra(int charExtra) const;
        int   SetTextJustification(int breakExtra, int breakCount) const;
        CSize TabbedTextOut(int x, int y, LPCTSTR pString, int count, int tabPositions, LPINT pTabStopPositions, int tabOrigin) const;
        BOOL  TextOut(int x, int y, LPCTSTR pString, int count = -1) const;

  #if (_WIN32_WINNT >= 0x0500) && !defined(__GNUC__)
        BOOL  GetCharABCWidthsI(UINT giFirst, UINT cgi, LPWORD pGI, LPABC pABC) const;
        BOOL  GetCharWidthI(UINT giFirst, UINT cgi, LPWORD pGI, int* pBuffer) const;
  #endif // (_WIN32_WINNT >= 0x0500) && !defined(__GNUC__)
#endif  // _WIN32_WCE

    protected:
        void Release();
        void SetManaged(bool IsManaged) { m_pData->isManagedHDC = IsManaged; }

    private:
        void AddToMap();
        BOOL RemoveFromMap();

        CDC_Data* m_pData;      // pointer to the class's data members
    };


    ///////////////////////////////////////////////
    // CClientDC provides the functionality of a GDI device context for a client window.
    class CClientDC : public CDC
    {
    public:
        CClientDC(HWND hWnd)
        {
            if (0 == hWnd) hWnd = GetDesktopWindow();
            assert(::IsWindow(hWnd));

            try
            {
                HDC hDC = ::GetDC(hWnd);
                if (hDC == 0)
                    throw CResourceException(_T("GetDC failed"));

                Attach(hDC, hWnd);
                SetManaged(true);
            }

            catch(...)
            {
                Release();  // Cleanup
                throw;      // Rethrow
            }
        }

        virtual ~CClientDC() {}
    };


    ///////////////////////////////////////////////
    // CClientDCEx provides the functionality of a GDI device context for the client area of a window.
    class CClientDCEx : public CDC
    {
    public:
        CClientDCEx(HWND hWnd, HRGN hrgnClip, DWORD flags)
        {
            if (0 == hWnd) hWnd = GetDesktopWindow();
            assert(::IsWindow(hWnd));

            try
            {
                HDC hDC = ::GetDCEx(hWnd, hrgnClip, flags);
                if (hDC == 0)
                    throw CResourceException(_T("GetDCEx failed"));

                Attach(hDC, hWnd);
                SetManaged(true);
            }

            catch(...)
            {
                Release();  // Cleanup
                throw;      // Rethrow
            }
        }

        virtual ~CClientDCEx() {}
    };


    ///////////////////////////////////////////////
    // CMemDC provides the functionality of a memory device context.
    class CMemDC : public CDC
    {
    public:
        CMemDC(HDC hdc)
        {
            try
            {
                CreateCompatibleDC(hdc);
            }

            catch(...)
            {
                Release();  // Cleanup
                throw;      // Rethrow
            }
        }
        virtual ~CMemDC() {}
    };


    ///////////////////////////////////////////////
    // CPaintDC provides the functionality of a GDI device used by WM_PAINT.
    class CPaintDC : public CDC
    {
    public:
        CPaintDC(HWND hWnd) : m_hWndPaint(hWnd)
        {
            assert(::IsWindow(hWnd));

            try
            {
                HDC hDC = ::BeginPaint(hWnd, &m_ps);
                if (hDC == 0)
                    throw CResourceException(_T("BeginPaint failed"));

                Attach(hDC, hWnd);
                SetManaged(true);
            }

            catch(...)
            {
                Release();  // Cleanup
                throw;      // Rethrow
            }
        }

        CPaintDC(const CPaintDC& rhs)  : CDC(rhs) // Copy constructor
        {
            m_hWndPaint = rhs.m_hWndPaint;
            m_ps = rhs.m_ps;
        }

        CPaintDC& operator = (const CPaintDC& rhs)
        {
            if (this != &rhs)
            {
                CDC::operator=(rhs);
                m_hWndPaint = rhs.m_hWndPaint;
                m_ps = rhs.m_ps;
            }

            return *this;
        }

        virtual ~CPaintDC()
        {
            ::EndPaint(m_hWndPaint, &m_ps);
        }

    private:
        HWND m_hWndPaint;
        PAINTSTRUCT m_ps;
    };


    ///////////////////////////////////////////////
    // CWindowDC provides the functionality of a GDI device context for the entire window.
    class CWindowDC : public CDC
    {
    public:
        CWindowDC(HWND hWnd)
        {
            if (0 == hWnd) hWnd = GetDesktopWindow();
            assert(::IsWindow(hWnd));

            try
            {
                HDC hDC = ::GetWindowDC(hWnd);
                if (hDC == 0)
                    throw CResourceException(_T("GetWindowDC failed"));

                Attach(hDC, hWnd);
                SetManaged(true);
            }

            catch(...)
            {
                Release();  // Cleanup
                throw;      // Rethrow
            }

        }
        virtual ~CWindowDC() {}
    };

#ifndef _WIN32_WCE

    ///////////////////////////////////////////////
    // CMetaFileDC provides the functionality of a GDI device context for a metafile.
    class CMetaFileDC : public CDC
    {
    public:
        CMetaFileDC() {}
        virtual ~CMetaFileDC()
        {
            if (GetHDC())
            {
                // Note we should not get here.
                TRACE("Warning! A MetaFile or EnhMetaFile was created but not closed\n");
                ::DeleteMetaFile(Close());
                Detach();
            }
        }

        void Create(LPCTSTR pFilename = NULL)
        {
            try
            {
                assert(GetHDC() == 0);
                HDC hDC = ::CreateMetaFile(pFilename);
                if (hDC == 0)
                    throw CResourceException(_T("Failed to create a DC for the MetaFile"));

                Attach(hDC);
                SetManaged(true);
            }
            catch (...)
            {
                Release();  // Cleanup
                throw;      // Rethrow
            }
        }

        // Closes the metafile and returns a CMetaFile object.
        // The CMetaFile object automatically deletes the HMETAFILE when the last copy
        // of the CMetaFile goes out of scope.
        CMetaFile Close()
        {
            assert(GetHDC());

            HMETAFILE hMeta = ::CloseMetaFile(GetHDC());
            Detach();
            return CMetaFile(hMeta);
        }

    };

    ///////////////////////////////////////////////
    // CEnhMetaFileDC provides the functionality of a GDI device context for an enhanced metafile.
    class CEnhMetaFileDC : public CDC
    {
    public:
        CEnhMetaFileDC() {}
        virtual ~CEnhMetaFileDC()
        {
            if (GetHDC())
            {
                // Note we should not get here.
                TRACE("Warning! An EnhMetaFile was created but not closed\n");
                ::DeleteEnhMetaFile(CloseEnhanced());
                Detach();
            }
        }

        void CreateEnhanced(HDC hRef, LPCTSTR pFileName, const RECT* pBounds, LPCTSTR pDescription)
        {
            try
            {
                assert(GetHDC() == 0);
                HDC hDC = ::CreateEnhMetaFile(hRef, pFileName, pBounds, pDescription);
                if (hDC == 0)
                    throw CResourceException(_T("Failed to create a DC for the EnhMetaFile"));

                Attach(hDC);
                SetManaged(true);
            }
            catch (...)
            {
                Release();  // Cleanup
                throw;      // Rethrow
            }
        }

        // Closes the enhanced metafile and returns a CEnhMetaFile object.
        // The CEnhMetaFile object automatically deletes the HENHMETAFILE when the last copy
        // of the CEnhMetaFile goes out of scope.
        CEnhMetaFile CloseEnhanced()
        {
            assert(GetHDC());

            HENHMETAFILE enhMeta = ::CloseEnhMetaFile(GetHDC());
            Detach();
            return CEnhMetaFile(enhMeta);
        }

    };


#endif


    ///////////////////////////////////////////////
    // The CBitmapInfoPtr class is a convenient wrapper for the BITMAPINFO structure.
    class CBitmapInfoPtr
    {
    public:
        CBitmapInfoPtr(HBITMAP hBitmap)
        {
            BITMAP bmSource;
            VERIFY(::GetObject(hBitmap, sizeof(bmSource), &bmSource) != 0);

            // Convert the color format to a count of bits.
            WORD cClrBits = static_cast<WORD>(bmSource.bmPlanes * bmSource.bmBitsPixel);
            if (cClrBits == 1)       cClrBits = 1;
            else if (cClrBits <= 4)  cClrBits = 4;
            else if (cClrBits <= 8)  cClrBits = 8;
            else if (cClrBits <= 16) cClrBits = 16;
            else if (cClrBits <= 24) cClrBits = 24;
            else                     cClrBits = 32;

            // Allocate memory for the BITMAPINFO structure.
            UINT uQuadSize = (cClrBits == 24)? 0 : sizeof(RGBQUAD) * static_cast<int>(1 << cClrBits);
            m_bmi.assign(sizeof(BITMAPINFOHEADER) + uQuadSize, 0);
            m_pbmiArray = (LPBITMAPINFO) &m_bmi[0];

            m_pbmiArray->bmiHeader.biSize       = sizeof(BITMAPINFOHEADER);
            m_pbmiArray->bmiHeader.biHeight     = bmSource.bmHeight;
            m_pbmiArray->bmiHeader.biWidth      = bmSource.bmWidth;
            m_pbmiArray->bmiHeader.biPlanes     = bmSource.bmPlanes;
            m_pbmiArray->bmiHeader.biBitCount   = bmSource.bmBitsPixel;
            m_pbmiArray->bmiHeader.biCompression = BI_RGB;
            if (cClrBits < 24)
                m_pbmiArray->bmiHeader.biClrUsed = (1<<cClrBits);
        }
        LPBITMAPINFO get() const { return m_pbmiArray; }
        operator LPBITMAPINFO() const { return m_pbmiArray; }
        LPBITMAPINFO operator->() const { return m_pbmiArray; }

    private:
        CBitmapInfoPtr(const CBitmapInfoPtr&);              // Disable copy construction
        CBitmapInfoPtr& operator = (const CBitmapInfoPtr&); // Disable assignment operator
        LPBITMAPINFO m_pbmiArray;
        std::vector<byte> m_bmi;
    };


}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{

    ///////////////////////////////////////////////
    // Declarations for the CGDIObject class
    //

    // Constructs the CGDIObject
    inline CGDIObject::CGDIObject()
    {
        m_pData = new CGDI_Data;
    }


    // Note: A copy of a CGDIObject is a clone of the original.
    //       Both objects manipulate the one HGDIOBJ.
    inline CGDIObject::CGDIObject(const CGDIObject& rhs)
    {
        m_pData = rhs.m_pData;
        InterlockedIncrement(&m_pData->count);
    }


    // Deconstructs the CGDIObject
    inline CGDIObject::~CGDIObject()
    {
        Release();
    }


    // Note: A copy of a CGDIObject is a clone of the original.
    //       Both objects manipulate the one HGDIOBJ.
    inline CGDIObject& CGDIObject::operator = ( const CGDIObject& rhs )
    {
        if (this != &rhs)
        {
            InterlockedIncrement(&rhs.m_pData->count);
            Release();
            m_pData = rhs.m_pData;
        }

        return *this;
    }


    inline void CGDIObject::operator = (const HGDIOBJ hObject)
    {
        Attach(hObject);
    }


    // Store the HDC and CDC pointer in the HDC map
    inline void CGDIObject::AddToMap()
    {
        assert( &GetApp() );
        assert(m_pData->hGDIObject);

        GetApp().AddCGDIData(m_pData->hGDIObject, m_pData);
    }


    // Attaches a GDI HANDLE to the CGDIObject.
    inline void CGDIObject::Attach(HGDIOBJ hObject)
    {
        assert(m_pData);

        if (hObject != m_pData->hGDIObject)
        {
            // Release any existing GDI object
            if (m_pData->hGDIObject != 0)
            {
                Release();
                m_pData = new CGDI_Data;
            }

            if (hObject)
            {
                // Add the GDI object to this CCGDIObject
                CGDI_Data* pCGDIData = GetApp().GetCGDIData(hObject);
                if (pCGDIData)
                {
                    delete m_pData;
                    m_pData = pCGDIData;
                    InterlockedIncrement(&m_pData->count);
                }
                else
                {
                    m_pData->hGDIObject = hObject;
                    AddToMap();
                }
            }
        }
    }


    inline void CGDIObject::DeleteObject()
    {
        assert(m_pData);

        if (m_pData->hGDIObject != 0)
        {
            RemoveFromMap();

            ::DeleteObject(m_pData->hGDIObject);
            m_pData->hGDIObject = 0;
        }
    }


    // Detaches the HGDIOBJ from all objects.
    inline HGDIOBJ CGDIObject::Detach()
    {
        assert(m_pData);
        assert(m_pData->hGDIObject);

        HGDIOBJ hObject = m_pData->hGDIObject;
        RemoveFromMap();
        m_pData->hGDIObject = 0;

        if (m_pData->count > 0)
        {
            if (InterlockedDecrement(&m_pData->count) == 0)
            {
                delete m_pData;
            }
        }

        m_pData = new CGDI_Data;
        return hObject;
    }


    // Returns the GDI handle (HGDIOBJ) associated with this object.
    inline HGDIOBJ CGDIObject::GetHandle() const
    {
        assert(m_pData);
        return m_pData->hGDIObject;
    }

    inline int CGDIObject::GetObject(int count, LPVOID pObject) const
    // Retrieves information for the specified graphics object.
    {
        assert(m_pData);
        return ::GetObject(m_pData->hGDIObject, count, pObject);
    }

    inline void CGDIObject::Release()
    {
        assert(m_pData);

        if (InterlockedDecrement(&m_pData->count) == 0)
        {
            if (m_pData->hGDIObject != NULL)
            {
                if (m_pData->isManagedObject)
                {
                    ::DeleteObject(m_pData->hGDIObject);
                }

                RemoveFromMap();
            }

            delete m_pData;
            m_pData = 0;
        }
    }


    inline BOOL CGDIObject::RemoveFromMap()
    {
        BOOL Success = FALSE;

        if( &GetApp() )
        {
            // Allocate an iterator for our HDC map
            std::map<HGDIOBJ, CGDI_Data*, CompareGDI>::iterator m;

            CWinApp& App = GetApp();
            CThreadLock mapLock(App.m_gdiLock);
            m = App.m_mapCGDIData.find(m_pData->hGDIObject);
            if (m != App.m_mapCGDIData.end())
            {
                // Erase the CGDIObject pointer entry from the map
                App.m_mapCGDIData.erase(m);
                Success = TRUE;
            }

        }

        return Success;
    }


    ///////////////////////////////////////////////
    // Declarations for the CBitmap class
    //

    inline CBitmap::CBitmap()
    {
    }


    inline CBitmap::CBitmap(HBITMAP hBitmap)
    {
        Attach(hBitmap);
    }


    inline CBitmap::CBitmap(LPCTSTR pResName)
    {
        LoadBitmap(pResName);
    }


    inline CBitmap::CBitmap(int resID)
    {
        LoadBitmap(resID);
    }


    inline CBitmap::operator HBITMAP() const
    {
        return static_cast<HBITMAP>(GetHandle());
    }


    inline CBitmap::~CBitmap()
    {
    }


    // Loads a bitmap from a resource using the resource ID.
    inline BOOL CBitmap::LoadBitmap(int resID)
    {
        return LoadBitmap(MAKEINTRESOURCE(resID));
    }


    // Loads a bitmap from a resource using the resource string.
    inline BOOL CBitmap::LoadBitmap(LPCTSTR pResName)
    {
        assert( &GetApp() );

        HBITMAP hBitmap = reinterpret_cast<HBITMAP>(::LoadImage(GetApp().GetResourceHandle(), pResName, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR));
        if (hBitmap != 0)
        {
            Attach(hBitmap);
            SetManaged(true);
        }
        return (0 != hBitmap);  // boolean expression
    }


    // Loads a bitmap from a resource using the resource ID.
    inline BOOL CBitmap::LoadImage(UINT resID, UINT flags)
    {
        return LoadImage(MAKEINTRESOURCE(resID), flags);
    }


    // Loads a bitmap from a resource using the resource string.
    inline BOOL CBitmap::LoadImage(LPCTSTR pResName, UINT flags)
    {
        assert( &GetApp() );

        HBITMAP hBitmap = reinterpret_cast<HBITMAP>(::LoadImage(GetApp().GetResourceHandle(), pResName, IMAGE_BITMAP, 0, 0, flags));
        if (hBitmap != 0)
        {
            Attach(hBitmap);
            SetManaged(true);
        }
        return (0 != hBitmap);  // boolean expression
    }


    // Loads a predefined bitmap.
    // Predefined bitmaps include: OBM_BTNCORNERS, OBM_BTSIZE, OBM_CHECK, OBM_CHECKBOXES, OBM_CLOSE, OBM_COMBO
    //  OBM_DNARROW, OBM_DNARROWD, OBM_DNARROWI, OBM_LFARROW, OBM_LFARROWD, OBM_LFARROWI, OBM_MNARROW,OBM_OLD_CLOSE
    //  OBM_OLD_DNARROW, OBM_OLD_LFARROW, OBM_OLD_REDUCE, OBM_OLD_RESTORE, OBM_OLD_RGARROW, OBM_OLD_UPARROW
    //  OBM_OLD_ZOOM, OBM_REDUCE, OBM_REDUCED, OBM_RESTORE, OBM_RESTORED, OBM_RGARROW, OBM_RGARROWD, OBM_RGARROWI
    //  OBM_SIZE, OBM_UPARROW, OBM_UPARROWD, OBM_UPARROWI, OBM_ZOOM, OBM_ZOOMD
    inline BOOL CBitmap::LoadOEMBitmap(UINT bitmapID) // for OBM_/OCR_/OIC_
    {
        HBITMAP hBitmap = ::LoadBitmap(NULL, MAKEINTRESOURCE(bitmapID));
        if (hBitmap != 0)
        {
            Attach( ::LoadBitmap(NULL, MAKEINTRESOURCE(bitmapID)) );
            SetManaged(true);
        }
        return (0 != hBitmap);  // boolean expression
    }

#ifndef _WIN32_WCE

    // Rapidly converts the bitmap image to pale grayscale image suitable for disabled icons.
    // The mask is the transparent color. Pixels with this color are not converted.
    // Supports 32 bit, 24 bit, 16 bit and 8 bit colors.
    // For 16 and 8 bit colors, ensure the mask is a color in the current palette.
    inline void CBitmap::ConvertToDisabled(COLORREF mask) const
    {
        BITMAP bm = GetBitmapData();

        // Requires 8 bits per pixel
        if (bm.bmBitsPixel < 8)
            return;

        // Copy the image data into the 'bits' byte array.
        CWindowDC dc(HWND_DESKTOP);
        CBitmapInfoPtr pbmi(*this);
        BITMAPINFOHEADER& bmiHeader = pbmi->bmiHeader;
        bmiHeader.biBitCount = 24;
        dc.GetDIBits(*this, 0, bm.bmHeight, NULL, pbmi, DIB_RGB_COLORS);
        DWORD size = pbmi->bmiHeader.biSizeImage;
        std::vector<byte> vBits(size, 0);
        byte* bits = &vBits[0];
        dc.GetDIBits(*this, 0, bm.bmHeight, bits, pbmi, DIB_RGB_COLORS);

        UINT nWidthBytes = bmiHeader.biSizeImage / bmiHeader.biHeight;
        int yOffset = 0;
        int xOffset;
        size_t Index;

        for (int Row = 0; Row < bmiHeader.biHeight; ++Row)
        {
            xOffset = 0;

            for (int Column = 0; Column < bmiHeader.biWidth; ++Column)
            {
                // Calculate Index
                Index = yOffset + xOffset;

                // skip for colors matching the mask
                if ((bits[Index + 0] != GetRValue(mask)) &&
                    (bits[Index + 1] != GetGValue(mask)) &&
                    (bits[Index + 2] != GetBValue(mask)))
                {
                    BYTE byGray = BYTE(95 + (bits[Index + 2] * 3 + bits[Index + 1] * 6 + bits[Index + 0]) / 20);
                    bits[Index] = byGray;
                    bits[Index + 1] = byGray;
                    bits[Index + 2] = byGray;
                }

                // Increment the horizontal offset
                xOffset += bmiHeader.biBitCount >> 3;
            }

            // Increment vertical offset
            yOffset += nWidthBytes;
        }

        dc.SetDIBits(*this, 0, bm.bmHeight, bits, pbmi, DIB_RGB_COLORS);
    }
    
    // Creates a new bitmap using the bitmap data and colors specified by the bitmap resource and the color mapping information.
    inline HBITMAP CBitmap::CreateMappedBitmap(UINT bitmapID, UINT flags /*= 0*/, LPCOLORMAP pColorMap /*= NULL*/, int mapSize /*= 0*/)
    {
        assert(&GetApp());
        HBITMAP hBitmap = ::CreateMappedBitmap(GetApp().GetResourceHandle(), bitmapID, static_cast<WORD>(flags), pColorMap, mapSize);
        if (hBitmap == 0)
            throw CResourceException(_T("CreateMappedBitmap failed"));

        Attach(hBitmap);
        SetManaged(true);
        return hBitmap;
    }

#endif // !_WIN32_WCE

    // Creates a bitmap with the specified width, height, and color format (color planes and bits-per-pixel).
    inline HBITMAP CBitmap::CreateBitmap(int width, int height, UINT planes, UINT bitsPerPixel, LPCVOID pBits)
    {
        HBITMAP hBitmap = ::CreateBitmap(width, height, planes, bitsPerPixel, pBits);
        if (hBitmap == 0)
            throw CResourceException(_T("CreateBitmap failed"));

        Attach(hBitmap);
        SetManaged(true);
        return hBitmap;
    }

#ifndef _WIN32_WCE

    // Creates a bitmap with the width, height, and color format specified in the BITMAP structure.
    inline HBITMAP CBitmap::CreateBitmapIndirect(const BITMAP& bitmap)
    {
        HBITMAP hBitmap = ::CreateBitmapIndirect(&bitmap);
        if (hBitmap == 0)
            throw CResourceException(_T("CreateBitmapIndirect failed"));

        Attach(hBitmap);
        SetManaged(true);
        return hBitmap;
    }

#endif // !_WIN32_WCE

    // Creates a bitmap compatible with the device that is associated with the specified device context.
    inline HBITMAP CBitmap::CreateCompatibleBitmap(HDC hdc, int width, int height)
    {
        HBITMAP hBitmap = ::CreateCompatibleBitmap(hdc, width, height);
        if (hBitmap == 0)
            throw CResourceException(_T("CreateCompatibleBitmap"));

        Attach(hBitmap);
        SetManaged(true);
        return hBitmap;
    }


    // Retrieves the BITMAP structure.
    inline BITMAP CBitmap::GetBitmapData() const
    {
        assert(GetHandle() != NULL);
        BITMAP bmp;
        ZeroMemory(&bmp, sizeof(bmp));
        VERIFY(::GetObject(GetHandle(), sizeof(bmp), &bmp) != 0);
        return bmp;
    }

#ifndef _WIN32_WCE

    // Retrieves the dimensions of a compatible bitmap.
    // The retrieved dimensions must have been set by the SetBitmapDimensionEx function.
    inline CSize CBitmap::GetBitmapDimensionEx() const
    {
        assert(GetHandle() != NULL);
        CSize Size;
        ::GetBitmapDimensionEx(reinterpret_cast<HBITMAP>(GetHandle()), &Size);
        return Size;
    }


    // The SetBitmapDimensionEx function assigns preferred dimensions to a bitmap.
    // These dimensions can be used by applications; however, they are not used by the system.
    inline CSize CBitmap::SetBitmapDimensionEx(int width, int height) const
    {
        assert(GetHandle() != NULL);
        CSize Size;
        ::SetBitmapDimensionEx(reinterpret_cast<HBITMAP>(GetHandle()), width, height, Size);
        return Size;
    }


    // Creates a compatible bitmap (DDB) from a DIB and, optionally, sets the bitmap bits.
    // A CBitmapInfoPtr object can be used for the LPBITMAPINFO parameter.
    inline HBITMAP CBitmap::CreateDIBitmap(HDC hdc, const BITMAPINFOHEADER* pBMIH, DWORD init, LPCVOID pInit, const LPBITMAPINFO pBMI, UINT colorUse)
    {
        HBITMAP hBitmap = ::CreateDIBitmap(hdc, pBMIH, init, pInit, pBMI, colorUse);
        Attach(hBitmap);
        SetManaged(true);
        return hBitmap;
    }


    // Convert a bitmap image to gray scale.
    inline void CBitmap::GrayScaleBitmap()
    {
        // Requires 8 bits per pixel
        BITMAP bm = GetBitmapData();
        if (bm.bmBitsPixel < 8)
            return;

        // Create our LPBITMAPINFO object
        CBitmapInfoPtr pbmi(*this);
        BITMAPINFOHEADER& bmiHeader = pbmi->bmiHeader;
        bmiHeader.biBitCount = 24;

        // Create the reference DC for GetDIBits to use
        CMemDC dcMem(NULL);

        // Use GetDIBits to create a DIB from our DDB, and extract the colour data
        GetDIBits(dcMem, 0, bmiHeader.biHeight, NULL, pbmi, DIB_RGB_COLORS);
        std::vector<byte> vBits(bmiHeader.biSizeImage, 0);
        byte* pByteArray = &vBits[0];

        dcMem.GetDIBits(*this, 0, bmiHeader.biHeight, pByteArray, pbmi, DIB_RGB_COLORS);
        UINT nWidthBytes = bmiHeader.biSizeImage/bmiHeader.biHeight;

        int yOffset = 0;
        int xOffset;
        size_t Index;

        for (int Row=0; Row < bmiHeader.biHeight; ++Row)
        {
            xOffset = 0;

            for (int Column=0; Column < bmiHeader.biWidth; ++Column)
            {
                // Calculate Index
                Index = yOffset + xOffset;

                BYTE byGray = (BYTE) ((pByteArray[Index] + pByteArray[Index+1]*6 + pByteArray[Index+2] *3)/10);
                pByteArray[Index]   = byGray;
                pByteArray[Index+1] = byGray;
                pByteArray[Index+2] = byGray;

                // Increment the horizontal offset
                xOffset += bmiHeader.biBitCount >> 3;
            }

            // Increment vertical offset
            yOffset += nWidthBytes;
        }

        // Save the modified colour back into our source DDB
        SetDIBits(dcMem, 0, bmiHeader.biHeight, pByteArray, pbmi, DIB_RGB_COLORS);
    }


    // Modifies the colour of the Device Dependant Bitmap, by the colour.
    // correction values specified. The correction values can range from -255 to +255.
    // This function gains its speed by accessing the bitmap colour information
    // directly, rather than using GetPixel/SetPixel.
    inline void CBitmap::TintBitmap (int cRed, int cGreen, int cBlue)
    {
        // Create our LPBITMAPINFO object
        CBitmapInfoPtr pbmi(*this);
        BITMAPINFOHEADER& bmiHeader = pbmi->bmiHeader;
        bmiHeader.biBitCount = 24;

        // Create the reference DC for GetDIBits to use
        CMemDC dcMem(NULL);

        // Use GetDIBits to create a DIB from our DDB, and extract the colour data
        GetDIBits(dcMem, 0, bmiHeader.biHeight, NULL, pbmi, DIB_RGB_COLORS);
        std::vector<byte> vBits(bmiHeader.biSizeImage, 0);
        byte* pByteArray = &vBits[0];

        dcMem.GetDIBits(*this, 0, bmiHeader.biHeight, pByteArray, pbmi, DIB_RGB_COLORS);
        UINT nWidthBytes = bmiHeader.biSizeImage/bmiHeader.biHeight;

        // Ensure sane colour correction values
        cBlue  = MIN(cBlue, 255);
        cBlue  = MAX(cBlue, -255);
        cRed   = MIN(cRed, 255);
        cRed   = MAX(cRed, -255);
        cGreen = MIN(cGreen, 255);
        cGreen = MAX(cGreen, -255);

        // Pre-calculate the RGB modification values
        int b1 = 256 - cBlue;
        int g1 = 256 - cGreen;
        int r1 = 256 - cRed;

        int b2 = 256 + cBlue;
        int g2 = 256 + cGreen;
        int r2 = 256 + cRed;

        // Modify the colour
        int yOffset = 0;
        int xOffset;
        int Index;
        for (int Row=0; Row < bmiHeader.biHeight; ++Row)
        {
            xOffset = 0;

            for (int Column=0; Column < bmiHeader.biWidth; ++Column)
            {
                // Calculate Index
                Index = yOffset + xOffset;

                // Adjust the colour values
                if (cBlue > 0)
                    pByteArray[Index]   = (BYTE)(cBlue + (((pByteArray[Index] *b1)) >>8));
                else if (cBlue < 0)
                    pByteArray[Index]   = (BYTE)((pByteArray[Index] *b2) >>8);

                if (cGreen > 0)
                    pByteArray[Index+1] = (BYTE)(cGreen + (((pByteArray[Index+1] *g1)) >>8));
                else if (cGreen < 0)
                    pByteArray[Index+1] = (BYTE)((pByteArray[Index+1] *g2) >>8);

                if (cRed > 0)
                    pByteArray[Index+2] = (BYTE)(cRed + (((pByteArray[Index+2] *r1)) >>8));
                else if (cRed < 0)
                    pByteArray[Index+2] = (BYTE)((pByteArray[Index+2] *r2) >>8);

                // Increment the horizontal offset
                xOffset += bmiHeader.biBitCount >> 3;
            }

            // Increment vertical offset
            yOffset += nWidthBytes;
        }

        // Save the modified colour back into our source DDB
        SetDIBits(dcMem, 0, bmiHeader.biHeight, pByteArray, pbmi, DIB_RGB_COLORS);
    }

#endif // !_WIN32_WCE

    // Creates a DIB that applications can write to directly. The function gives you
    // a pointer to the location of the bitmap bit values. You can supply a handle to
    // a file-mapping object that the function will use to create the bitmap,
    // or you can let the system allocate the memory for the bitmap.
    // A CBitmapInfoPtr object can be used for the LPBITMAPINFO parameter.
    inline HBITMAP CBitmap::CreateDIBSection(HDC hdc, const LPBITMAPINFO pBMI, UINT colorUse, LPVOID* pBits, HANDLE hSection, DWORD offset)
    {
        HBITMAP hBitmap = ::CreateDIBSection(hdc, pBMI, colorUse, pBits, hSection, offset);
        Attach(hBitmap);
        SetManaged(true);
        return hBitmap;
    }

#ifndef _WIN32_WCE

    // Retrieves the bits of the specified compatible bitmap and copies them
    // into a buffer as a DIB using the specified format.
    // A CBitmapInfoPtr object can be used for the LPBITMAPINFO parameter.
    inline int CBitmap::GetDIBits(HDC hdc, UINT startScan, UINT scanLines,  LPVOID pBits, LPBITMAPINFO pBMI, UINT colorUse) const
    {
        assert(GetHandle() != NULL);
        return ::GetDIBits(hdc, reinterpret_cast<HBITMAP>(GetHandle()), startScan, scanLines,  pBits, pBMI, colorUse);
    }


    // Sets the pixels in a compatible bitmap (DDB) using the color data found in the specified DIB.
    // A CBitmapInfoPtr object can be used for the LPBITMAPINFO parameter.
    inline int CBitmap::SetDIBits(HDC hdc, UINT startScan, UINT scanLines, LPCVOID pBits, const LPBITMAPINFO pBMI, UINT colorUse) const
    {
        assert(GetHandle() != NULL);
        return ::SetDIBits(hdc, reinterpret_cast<HBITMAP>(GetHandle()), startScan, scanLines, pBits, pBMI, colorUse);
    }

#endif // !_WIN32_WCE


    ///////////////////////////////////////////////
    // Definitions of the CBrush class
    //
    inline CBrush::CBrush()
    {
    }


    inline CBrush::CBrush(HBRUSH hBrush)
    {
        Attach(hBrush);
    }


    inline CBrush::CBrush(COLORREF color)
    {
        try
        {
            CreateSolidBrush(color);
        }

        catch(...)
        {
            Release();  // Cleanup
            throw;      // Rethrow
        }
    }


    inline CBrush::operator HBRUSH() const
    {
        return static_cast<HBRUSH>(GetHandle());
    }


    inline CBrush::~CBrush()
    {
    }


    // Creates a logical brush that has the specified solid color.
    inline HBRUSH CBrush::CreateSolidBrush(COLORREF color)
    {
        HBRUSH hBrush = ::CreateSolidBrush(color);
        if (hBrush == 0)
            throw CResourceException(_T("CreateSolidBrush failed"));

        Attach(hBrush);
        SetManaged(true);
        return hBrush;
    }

#ifndef _WIN32_WCE

    // Creates a logical brush that has the specified hatch pattern and color.
    inline HBRUSH CBrush::CreateHatchBrush(int index, COLORREF color)
    {
        HBRUSH hBrush = ::CreateHatchBrush(index, color);
        if (hBrush == 0)
            throw CResourceException(_T("CreateHatchBrush failed"));

        Attach(hBrush);
        SetManaged(true);
        return hBrush;
    }


    // Creates a logical brush from style, color, and pattern specified in the LOGPRUSH struct.
    inline HBRUSH CBrush::CreateBrushIndirect(const LOGBRUSH& logBrush)
    {
        HBRUSH hBrush = ::CreateBrushIndirect(&logBrush);
        if (hBrush == 0)
            throw CResourceException(_T("CreateBrushIndirect failed"));

        Attach(hBrush);
        SetManaged(true);
        return hBrush;
    }


    // Creates a logical brush that has the pattern specified by the specified device-independent bitmap (DIB).
    inline HBRUSH CBrush::CreateDIBPatternBrush(HGLOBAL hDIBPacked, UINT colorSpec)
    {
        HBRUSH hBrush = ::CreateDIBPatternBrush(hDIBPacked, colorSpec);
        if (hBrush == 0)
            throw CResourceException(_T("CreateDIBPatternBrush failed"));

        Attach(hBrush);
        SetManaged(true);
        return hBrush;
    }


    // Creates a logical brush that has the pattern specified by the device-independent bitmap (DIB).
    inline HBRUSH CBrush::CreateDIBPatternBrushPt(LPCVOID pPackedDIB, UINT usage)
    {
        HBRUSH hBrush = ::CreateDIBPatternBrushPt(pPackedDIB, usage);
        if (hBrush == 0)
            throw CResourceException(_T("CreateDIBPatternBrushPt failed"));

        Attach(hBrush);
        SetManaged(true);
        return hBrush;
    }

#endif // !defined(_WIN32_WCE)

    // Creates a logical brush with the specified bitmap pattern. The bitmap can be a DIB section bitmap,
    // which is created by the CreateDIBSection function, or it can be a device-dependent bitmap.
    inline HBRUSH CBrush::CreatePatternBrush(HBITMAP hBitmap)
    {
        HBRUSH hBrush = ::CreatePatternBrush(hBitmap);
        if (hBrush == 0)
            throw CResourceException(_T("CreatePatternBrush failed"));

        Attach(hBrush);
        SetManaged(true);
        return hBrush;
    }


    // Retrieves the LOGBRUSH structure that defines the style, color, and pattern of a physical brush.
    inline LOGBRUSH CBrush::GetLogBrush() const
    {
        assert(GetHandle() != NULL);
        LOGBRUSH logBrush;
        ZeroMemory(&logBrush, sizeof(logBrush));
        VERIFY(::GetObject (GetHandle(), sizeof(logBrush), &logBrush) != 0);
        return logBrush;
    }


    ///////////////////////////////////////////////
    // Definitions of the CFont class
    //
    inline CFont::CFont()
    {
    }


    inline CFont::CFont(HFONT hFont)
    {
        Attach(hFont);
    }


    inline CFont::CFont(const LOGFONT& logFont)
    {
        try
        {
            CreateFontIndirect(logFont);
        }

        catch(...)
        {
            Release();  // Cleanup
            throw;      // Rethrow;
        }
    }


    inline CFont::operator HFONT() const
    {
        return static_cast<HFONT>(GetHandle());
    }


    inline CFont::~CFont()
    {
    }


    // Creates a logical font that has the specified characteristics.
    inline HFONT CFont::CreateFontIndirect(const LOGFONT& logFont)
    {
        HFONT hFont = ::CreateFontIndirect(&logFont);
        if (hFont == 0)
            throw CResourceException(_T("CreateFontIndirect"));

        Attach(hFont);
        SetManaged(true);
        return hFont;
    }


    // Creates a font of a specified typeface and point size.
    inline HFONT CFont::CreatePointFont(int pointSize, LPCTSTR pFaceName, HDC hdc /*= NULL*/, BOOL isBold /*= FALSE*/, BOOL isItalic /*= FALSE*/)
    {
        LOGFONT logFont;
        ZeroMemory(&logFont, sizeof(logFont));
        logFont.lfCharSet = DEFAULT_CHARSET;
        logFont.lfHeight = pointSize;

        lstrcpyn(logFont.lfFaceName, pFaceName, LF_FACESIZE);

        if (isBold)
            logFont.lfWeight = FW_BOLD;
        if (isItalic)
            logFont.lfItalic = 1;

        return CreatePointFontIndirect(logFont, hdc);
    }


    // Creates a font of a specified typeface and point size.
    // This function automatically converts the height in lfHeight to logical units
    // using the specified device context.
    inline HFONT CFont::CreatePointFontIndirect(const LOGFONT& logFont, HDC hdc /* = NULL*/)
    {
        HDC hDC1 = (hdc != NULL) ? hdc : ::GetDC(HWND_DESKTOP);
        CDC dc(hDC1);

        // convert nPointSize to logical units based on hDC
        LOGFONT logFont1 = logFont;

#ifndef _WIN32_WCE
        POINT pt = { 0, 0 };
        pt.y = ::MulDiv(::GetDeviceCaps(dc, LOGPIXELSY), logFont.lfHeight, 720);   // 72 points/inch, 10 decipoints/point
        ::DPtoLP(dc, &pt, 1);

        POINT ptOrg = { 0, 0 };
        ::DPtoLP(dc, &ptOrg, 1);

        logFont1.lfHeight = -abs(pt.y - ptOrg.y);
#else // CE specific
        // DP and LP are always the same on CE
        logFont1.lfHeight = -abs(((::GetDeviceCaps(hDC1, LOGPIXELSY)* logFont.lfHeight)/ 720));
#endif // _WIN32_WCE

        return CreateFontIndirect (logFont1);
    }

#ifndef _WIN32_WCE

    // Creates a logical font with the specified characteristics.
    inline HFONT CFont::CreateFont(int height, int width, int escapement,
            int orientation, int weight, DWORD italic, DWORD underline,
            DWORD strikeOut, DWORD charSet, DWORD outPrecision,
            DWORD clipPrecision, DWORD quality, DWORD pitchAndFamily,
            LPCTSTR pFacename)
    {
        HFONT hFont = ::CreateFont(height, width, escapement,
            orientation, weight, italic, underline, strikeOut,
            charSet, outPrecision, clipPrecision, quality,
            pitchAndFamily, pFacename);

        if (hFont == 0)
            throw CResourceException(_T("CreateFont failed"));

        Attach(hFont);
        SetManaged(true);
        return hFont;
    }

#endif // #ifndef _WIN32_WCE

    // Retrieves the Logfont structure that contains font attributes.
    inline LOGFONT CFont::GetLogFont() const
    {
        assert(GetHandle() != NULL);
        LOGFONT logFont;
        ZeroMemory(&logFont, sizeof(logFont));
        VERIFY(::GetObject(GetHandle(), sizeof(logFont), &logFont) != 0);
        return logFont;
    }


    ///////////////////////////////////////////////
    // Definitions of the CPalette class
    //
    inline CPalette::CPalette()
    {
    }


    inline CPalette::CPalette(HPALETTE hPalette)
    {
        Attach(hPalette);
    }


    inline CPalette::operator HPALETTE() const
    {
        return static_cast<HPALETTE>(GetHandle());
    }


    inline CPalette::~CPalette ()
    {
    }


    // Creates a logical palette from the information in the specified LOGPALETTE structure.
    inline HPALETTE CPalette::CreatePalette(LPLOGPALETTE lpLogPalette)
    {
        HPALETTE hPalette = ::CreatePalette (lpLogPalette);
        if (hPalette == 0)
            throw CResourceException(_T("CreatePalette failed"));

        Attach(hPalette);
        SetManaged(true);
        return hPalette;
    }

#ifndef _WIN32_WCE

    // Creates a halftone palette for the specified device context (DC).
    inline HPALETTE CPalette::CreateHalftonePalette(HDC hdc)
    {
        HPALETTE hPalette = ::CreateHalftonePalette(hdc);
        if (hPalette == 0)
            throw CResourceException(_T("CreateHalftonePalette failed"));

        Attach(hPalette);
        ::RealizePalette(hdc);
        SetManaged(true);
        return hPalette;
    }

#endif // !_WIN32_WCE

    // Retrieve the number of entries in the palette.
    inline int CPalette::GetEntryCount() const
    {
        assert(GetHandle() != NULL);
        WORD nEntries = 0;
        VERIFY(::GetObject(GetHandle(), sizeof(WORD), &nEntries) != 0);
        return static_cast<int>(nEntries);
    }


    // Retrieves a specified range of palette entries from the palette.
    inline UINT CPalette::GetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) const
    {
        assert(GetHandle() != NULL);
        return ::GetPaletteEntries(reinterpret_cast<HPALETTE>(GetHandle()), nStartIndex, nNumEntries, lpPaletteColors);
    }


    // Sets RGB (red, green, blue) color values and flags in a range of entries in the palette.
    inline UINT CPalette::SetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) const
    {
        assert(GetHandle() != NULL);
        return ::SetPaletteEntries(reinterpret_cast<HPALETTE>(GetHandle()), nStartIndex, nNumEntries, lpPaletteColors);
    }

#ifndef _WIN32_WCE

    // Replaces entries in the palette.
    inline void CPalette::AnimatePalette(UINT startIndex, UINT entries, LPPALETTEENTRY pPaletteColors) const
    {
        assert(GetHandle() != NULL);
        ::AnimatePalette(reinterpret_cast<HPALETTE>(GetHandle()), startIndex, entries, pPaletteColors);
    }


    //  Increases or decreases the size of the palette based on the specified value.
    inline BOOL CPalette::ResizePalette(UINT entries) const
    {
        assert(GetHandle() != NULL);
        return ::ResizePalette(reinterpret_cast<HPALETTE>(GetHandle()), entries);
    }

#endif // !_WIN32_WCE

    // Retrieves the index for the entry in the palette most closely matching a specified color value.
    inline UINT CPalette::GetNearestPaletteIndex(COLORREF color) const
    {
        assert(GetHandle() != NULL);
        return ::GetNearestPaletteIndex(reinterpret_cast<HPALETTE>(GetHandle()), color);
    }


    ///////////////////////////////////////////////
    // Declarations for the CPen class
    //
    inline CPen::CPen()
    {
    }


    inline CPen::CPen(HPEN hPen)
    {
        Attach(hPen);
    }


    inline CPen::CPen(int penStyle, int width, COLORREF color)
    {
        try
        {
            CreatePen(penStyle, width, color);
        }

        catch(...)
        {
            Release();  // Cleanup
            throw;      // Rethrow
        }
    }

#ifndef _WIN32_WCE

    inline CPen::CPen(int penStyle, int width, const LOGBRUSH& logBrush, int styleCount /*= 0*/, const DWORD* pStyle /*= NULL*/)
    {
        try
        {
            Attach(::ExtCreatePen(penStyle, width, &logBrush, styleCount, pStyle));
        }

        catch(...)
        {
            Release();  // Cleanup
            throw;      // Rethrow
        }
    }

#endif // !_WIN32_WCE

    inline CPen::operator HPEN () const
    {
        return static_cast<HPEN>(GetHandle());
    }


    inline CPen::~CPen()
    {
    }


    // Creates a logical pen that has the specified style, width, and color.
    inline HPEN CPen::CreatePen(int penStyle, int width, COLORREF color)
    {
        HPEN hPen = ::CreatePen(penStyle, width, color);
        Attach(hPen);
        SetManaged(true);
        return hPen;
    }


    // Creates a logical pen that has the style, width, and color specified in a structure.
    inline HPEN CPen::CreatePenIndirect(const LOGPEN& logPen)
    {
        HPEN hPen = ::CreatePenIndirect(&logPen);
        Attach(hPen);
        SetManaged(true);
        return hPen;
    }


    // Retrieves the LOGPEN struct that specifies the pen's style, width, and color.
    inline LOGPEN CPen::GetLogPen() const
    {
        assert(GetHandle() != NULL);

        LOGPEN logPen;
        ZeroMemory(&logPen, sizeof(logPen));
        VERIFY(::GetObject(GetHandle(), sizeof(logPen), &logPen) != 0);
        return logPen;
    }

#ifndef _WIN32_WCE

    // Creates a logical cosmetic or geometric pen that has the specified style, width, and brush attributes.
    inline HPEN CPen::ExtCreatePen(int penStyle, int width, const LOGBRUSH& logBrush, int styleCount /* = 0*/, const DWORD* pStyle /*= NULL*/)
    {
        HPEN hPen = ::ExtCreatePen(penStyle, width, &logBrush, styleCount, pStyle);
        Attach(hPen);
        SetManaged(true);
        return hPen;
    }


    // Retrieves the EXTLOGPEN struct that specifies the pen's style, width, color and brush attributes.
    inline EXTLOGPEN CPen::GetExtLogPen() const
    {
        assert(GetHandle() != NULL);

        EXTLOGPEN exLogPen;
        ZeroMemory(&exLogPen, sizeof(exLogPen));
        VERIFY(::GetObject(GetHandle(), sizeof(exLogPen), &exLogPen) != 0);
        return exLogPen;
    }

#endif // !_WIN32_WCE


    ///////////////////////////////////////////////
    // Definitions of the CRgn class
    //
    inline CRgn::CRgn()
    {
    }


    inline CRgn::CRgn(HRGN hRgn)
    {
        Attach(hRgn);
    }


    inline CRgn::operator HRGN() const
    {
        return static_cast<HRGN>(GetHandle());
    }


    inline CRgn::~CRgn()
    {
    }


    // Creates a rectangular region.
    inline HRGN CRgn::CreateRectRgn(int x1, int y1, int x2, int y2)
    {
        HRGN hRgn = ::CreateRectRgn(x1, y1, x2, y2);
        if (hRgn == 0)
            throw CResourceException(_T("CreateRectRgn failed"));

        Attach(hRgn);
        SetManaged(true);
        return hRgn;
    }

    inline HRGN CRgn::CreateRectRgnIndirect(const RECT& rc)
    // Creates a rectangular region.
    {
        HRGN hRgn = ::CreateRectRgnIndirect(&rc);
        if (hRgn == 0)
            throw CResourceException(_T("CreateRectRgnIndirect failed"));

        Attach(hRgn);
        SetManaged(true);
        return hRgn;
    }

#ifndef _WIN32_WCE

    // Creates an elliptical region.
    inline HRGN CRgn::CreateEllipticRgn(int x1, int y1, int x2, int y2)
    {
        HRGN hRgn = ::CreateEllipticRgn(x1, y1, x2, y2);
        if (hRgn == 0)
            throw CResourceException(_T("CreateEllipticRgn failed"));

        Attach(hRgn);
        SetManaged(true);
        return hRgn;
    }


    // Creates an elliptical region.
    inline HRGN CRgn::CreateEllipticRgnIndirect(const RECT& rc)
    {
        HRGN hRgn = ::CreateEllipticRgnIndirect(&rc);
        if (hRgn == 0)
            throw CResourceException(_T("CreateEllipticRgnIndirect failed"));

        Attach(hRgn);
        SetManaged(true);
        return hRgn;
    }


    // Creates a polygonal region.
    inline HRGN CRgn::CreatePolygonRgn(LPPOINT pPoints, int count, int mode)
    {
        HRGN hRgn = ::CreatePolygonRgn(pPoints, count, mode);
        if (hRgn == 0)
            throw CResourceException(_T("CreatePolygonRgn failed"));

        Attach(hRgn);
        SetManaged(true);
        return hRgn;
    }


    // Creates a region consisting of a series of polygons. The polygons can overlap.
    inline HRGN CRgn::CreatePolyPolygonRgn(LPPOINT pPoints, LPINT pPolyCounts, int count, int polyFillMode)
    {
        HRGN hRgn = ::CreatePolyPolygonRgn(pPoints, pPolyCounts, count, polyFillMode);
        if (hRgn == 0)
            throw CResourceException(_T("CreatePolyPolygonRgn failed"));

        Attach(hRgn);
        SetManaged(true);
        return hRgn;
    }


    // Creates a rectangular region with rounded corners.
    inline HRGN CRgn::CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3)
    {
        HRGN hRgn = ::CreateRoundRectRgn(x1, y1, x2, y2, x3, y3);
        if (hRgn == 0)
            throw CResourceException(_T("CreateRoundRectRgn failed"));

        Attach(hRgn);
        SetManaged(true);
        return hRgn;
    }


    // Creates a region from the path that is selected into the specified device context.
    // The resulting region uses device coordinates.
    inline HRGN CRgn::CreateFromPath(HDC hDC)
    {
        assert(hDC != NULL);
        HRGN hRgn = ::PathToRegion(hDC);
        if (hRgn == 0)
            throw CResourceException(_T("PathToRegion failed"));

        Attach(hRgn);
        SetManaged(true);
        return hRgn;
    }

#endif // !_WIN32_WCE

    // Creates a region from the specified region and transformation data.
    // If the XFORM pointer is NULL, the identity transformation is used.
    inline HRGN CRgn::CreateFromData(const XFORM* pXForm, int count, const RGNDATA* pRgnData)
    {
        HRGN hRgn = ::ExtCreateRegion(pXForm, count, pRgnData);
        if (hRgn == 0)
            throw CResourceException(_T("ExtCreateRegion failed"));

        Attach(hRgn);
        SetManaged(true);
        return hRgn;
    }


    // Converts the region into a rectangular region with the specified coordinates.
    inline void CRgn::SetRectRgn(int x1, int y1, int x2, int y2) const
    {
        assert(GetHandle() != NULL);
        ::SetRectRgn(reinterpret_cast<HRGN>(GetHandle()), x1, y1, x2, y2);
    }


    // Converts the region into a rectangular region with the specified coordinates.
    inline void CRgn::SetRectRgn(const RECT& rc) const
    {
        assert(GetHandle() != NULL);
        ::SetRectRgn(reinterpret_cast<HRGN>(GetHandle()), rc.left, rc.top, rc.right, rc.bottom);
    }


    // Combines two specified regions and stores the result.
    inline int CRgn::CombineRgn(HRGN hSrc1, HRGN hSrc2, int combineMode) const
    {
        assert(GetHandle() != NULL);
        return ::CombineRgn(reinterpret_cast<HRGN>(GetHandle()), hSrc1, hSrc2, combineMode);
    }


    // Combines the specified region with the current region.
    inline int CRgn::CombineRgn(HRGN hSrc, int combineMode) const
    {
        assert(GetHandle() != NULL);
        return ::CombineRgn(reinterpret_cast<HRGN>(GetHandle()), (HRGN)GetHandle(), hSrc, combineMode);
    }


    // Assigns the specified region to the current region.
    inline int CRgn::CopyRgn(HRGN hSrc) const
    {
        assert(GetHandle() != NULL);
        assert(hSrc);
        return ::CombineRgn(reinterpret_cast<HRGN>(GetHandle()), hSrc, NULL, RGN_COPY);
    }


    // Checks the two specified regions to determine whether they are identical.
    inline BOOL CRgn::EqualRgn(HRGN hRgn) const
    {
        assert(GetHandle() != NULL);
        return ::EqualRgn(reinterpret_cast<HRGN>(GetHandle()), hRgn);
    }


    // Moves a region by the specified offsets.
    inline int CRgn::OffsetRgn(int x, int y) const
    {
        assert(GetHandle() != NULL);
        return ::OffsetRgn(reinterpret_cast<HRGN>(GetHandle()), x, y);
    }


    // Moves a region by the specified offsets.
    inline int CRgn::OffsetRgn(POINT& pt) const
    {
        assert(GetHandle() != NULL);
        return ::OffsetRgn(reinterpret_cast<HRGN>(GetHandle()), pt.x, pt.y);
    }


    // Retrieves the bounding rectangle of the region, and stores it in the specified RECT.
    // The return value indicates the region's complexity: NULLREGION;SIMPLEREGION; or COMPLEXREGION.
    inline int CRgn::GetRgnBox(RECT& rc) const
    {
        assert(GetHandle() != NULL);
        return ::GetRgnBox(reinterpret_cast<HRGN>(GetHandle()), &rc);
    }


    // Fills the specified buffer with data describing a region.
    inline int CRgn::GetRegionData(LPRGNDATA pRgnData, int dataSize) const
    {
        assert(GetHandle() != NULL);
        return static_cast<int>(::GetRegionData(reinterpret_cast<HRGN>(GetHandle()), dataSize, pRgnData));
    }


    // Determines whether the specified point is inside the specified region.
    inline BOOL CRgn::PtInRegion(int x, int y) const
    {
        assert(GetHandle() != NULL);
        return ::PtInRegion(reinterpret_cast<HRGN>(GetHandle()), x, y);
    }


    // Determines whether the specified point is inside the specified region.
    inline BOOL CRgn::PtInRegion(POINT& pt) const
    {
        assert(GetHandle() != NULL);
        return ::PtInRegion(reinterpret_cast<HRGN>(GetHandle()), pt.x, pt.y);
    }


    // Determines whether the specified rect is inside the specified region.
    inline BOOL CRgn::RectInRegion(const RECT& rc) const
    {
        assert(GetHandle() != NULL);
        return ::RectInRegion(reinterpret_cast<HRGN>(GetHandle()), &rc);
    }


    ///////////////////////////////////////////////
    // Definitions of the CDC class
    //

    inline CDC::CDC()
    {
        // Allocate memory for our data members
        m_pData = new CDC_Data;
    }


    // This constructor assigns an existing HDC to the CDC
    // The HDC WILL be released or deleted when the CDC object is destroyed
    // The hWnd parameter is only used in WindowsCE. It specifies the HWND of a Window or
    // Window Client DC.
    // Note: this constructor permits a call like this:
    // CDC MyCDC = SomeHDC;
    inline CDC::CDC(HDC hDC, HWND hWnd /*= 0*/)
    {
        m_pData = new CDC_Data;
        Attach(hDC, hWnd);
    }

#ifndef _WIN32_WCE

    // Note: this assignment operator permits a call like this:
    // CDC MyCDC;
    // MyCDC = SomeHDC;
    inline void CDC::operator = (const HDC hDC)
    {
        Attach(hDC);
    }

#endif

    // The copy constructor is called when a temporary copy of the CDC needs to be created.
    // This can happen when a CDC is passed by value in a function call. Each CDC copy manages
    // the same Device Context and GDI objects.
    inline CDC::CDC(const CDC& rhs) // Copy constructor
    {
        m_pData = rhs.m_pData;
        InterlockedIncrement(&m_pData->count);
    }


    // Note: A copy of a CDC is a clone of the original.
    //       Both objects manipulate the one HDC
    inline CDC& CDC::operator = (const CDC& rhs)
    {
        if (this != &rhs)
        {
            InterlockedIncrement(&rhs.m_pData->count);
            Release();
            m_pData = rhs.m_pData;
        }

        return *this;
    }


    inline CDC::~CDC ()
    {
        Release();
    }


    // Store the HDC and CDC pointer in the HDC map
    inline void CDC::AddToMap()
    {
        assert( &GetApp() );
        assert(m_pData->hDC);

        GetApp().AddCDCData(m_pData->hDC, m_pData);
    }


    // Attaches a HDC to the CDC object.
    // The hWnd parameter is only required on WinCE.
    inline void CDC::Attach(HDC hDC, HWND hWnd /* = 0*/)
    {
        UNREFERENCED_PARAMETER(hWnd);
        assert(m_pData);

        if (hDC != m_pData->hDC)
        {
            if (m_pData->hDC)
            {
                Release();

                // Assign values to our data members
                m_pData = new CDC_Data;
            }

            if (hDC)
            {
                CDC_Data* pCDCData = GetApp().GetCDCData(hDC);
                if (pCDCData)
                {
                    delete m_pData;
                    m_pData = pCDCData;
                    InterlockedIncrement(&m_pData->count);
                }
                else
                {
                    m_pData->hDC = hDC;

        #ifndef _WIN32_WCE
                    m_pData->hWnd = ::WindowFromDC(hDC);
        #else
                    m_pData->hWnd = hWnd;
        #endif

                    AddToMap();
                    m_pData->savedDCState = ::SaveDC(hDC);
                }
            }
        }
    }


    // Detaches the HDC from all CDC objects.
    inline HDC CDC::Detach()
    {
        assert(m_pData);
        assert(m_pData->hDC);

        HDC hDC = m_pData->hDC;
        RemoveFromMap();
        m_pData->hDC = 0;
        ::RestoreDC(hDC, m_pData->savedDCState);

        if (m_pData->count > 0)
        {
            if (InterlockedDecrement(&m_pData->count) == 0)
            {
                delete m_pData;
            }
        }

        // Assign values to our data members
        m_pData = new CDC_Data;

        return hDC;
    }

#ifndef _WIN32_WCE

    // Enumerates the pens or brushes available for the device context. This function calls
    // the application-defined callback function once for each available object, supplying
    // data describing that object.
    inline int CDC::EnumObjects(int objectType, GOBJENUMPROC pObjectFunc, LPARAM lparam) const
    {
        assert(m_pData->hDC);
        return ::EnumObjects(m_pData->hDC, objectType, pObjectFunc, lparam);
    }

#endif

    // Returns a memory device context (DC) compatible with the specified device.
    inline HDC CDC::CreateCompatibleDC(HDC hSource)
    {
        assert(m_pData->hDC == NULL);
        HDC hDC = ::CreateCompatibleDC(hSource);

        if (hDC == NULL)
            throw CResourceException(_T("CreateCompatibleDC failed"));

        m_pData->hDC = hDC;
        m_pData->isManagedHDC = TRUE;
        AddToMap();

        return hDC;
    }


    // Returns a device context (DC) for a device using the specified name.
    inline HDC CDC::CreateDC(LPCTSTR pDriver, LPCTSTR pDevice, LPCTSTR pOutput, const DEVMODE* pInitData)
    {
        assert(m_pData->hDC == NULL);
        HDC hDC = ::CreateDC(pDriver, pDevice, pOutput, pInitData);

        if (hDC == NULL)
            throw CResourceException(_T("CreateDC failed"));

        m_pData->hDC = hDC;
        m_pData->isManagedHDC = TRUE;
        AddToMap();
        return hDC;
    }

#ifndef _WIN32_WCE

    // Returns an information context for the specified device. The information context
    // provides a fast way to get information about the device without creating a device context (DC).
    // However, GDI drawing functions cannot accept a handle to an information context.
    inline HDC CDC::CreateIC(LPCTSTR pDriver, LPCTSTR pDevice, LPCTSTR pOutput, const DEVMODE* pInitData)
    {
        assert(m_pData->hDC == NULL);
        HDC hDC = ::CreateIC(pDriver, pDevice, pOutput, pInitData);

        if (hDC == 0)
            throw CResourceException(_T("CreateIC failed"));

        m_pData->hDC = hDC;
        m_pData->isManagedHDC = TRUE;
        AddToMap();
        return hDC;
    }

#endif

    // Draws the specified bitmap to the specified DC using the mask colour provided as the transparent colour
    // Suitable for use with a Window DC or a memory DC
    inline void CDC::DrawBitmap(int x, int y, int cx, int cy, HBITMAP hBitmap, COLORREF mask) const
    {
        // Create the Image memory DC
        CMemDC dcImage(*this);
        dcImage.SetBkColor(mask);
        ::SelectObject(dcImage, hBitmap);

        // Create the Mask memory DC
        CMemDC dcMask(*this);
        dcMask.CreateBitmap(cx, cy, 1, 1, NULL);
        dcMask.BitBlt(0, 0, cx, cy, dcImage, 0, 0, SRCCOPY);

        // Mask the image to 'this' DC
        BitBlt(x, y, cx, cy, dcImage, 0, 0, SRCINVERT);
        BitBlt(x, y, cx, cy, dcMask, 0, 0, SRCAND);
        BitBlt(x, y, cx, cy, dcImage, 0, 0, SRCINVERT);
    }


    // An efficient color gradient filler compatible with all Windows operating systems
    inline void CDC::GradientFill(COLORREF color1, COLORREF color2, const RECT& rc, BOOL isVertical) const
    {
        int Width = rc.right - rc.left;
        int Height = rc.bottom - rc.top;

        int r1 = GetRValue(color1);
        int g1 = GetGValue(color1);
        int b1 = GetBValue(color1);

        int r2 = GetRValue(color2);
        int g2 = GetGValue(color2);
        int b2 = GetBValue(color2);

        COLORREF oldBkColor = GetBkColor();

        if (isVertical)
        {
            for(int i=0; i < Width; ++i)
            {
                int r = r1 + (i * (r2-r1) / Width);
                int g = g1 + (i * (g2-g1) / Width);
                int b = b1 + (i * (b2-b1) / Width);
                SetBkColor(RGB(r, g, b));
                CRect line( i + rc.left, rc.top, i + 1 + rc.left, rc.top+Height);
                ExtTextOut(0, 0, ETO_OPAQUE, line, NULL, 0, 0);
            }
        }
        else
        {
            for(int i=0; i < Height; ++i)
            {
                int r = r1 + (i * (r2-r1) / Height);
                int g = g1 + (i * (g2-g1) / Height);
                int b = b1 + (i * (b2-b1) / Height);
                SetBkColor(RGB(r, g, b));
                CRect line(rc.left, i + rc.top, rc.left+Width, i + 1 + rc.top);
                ExtTextOut(0, 0, ETO_OPAQUE, line, NULL, 0, 0);
            }
        }

        SetBkColor(oldBkColor);
    }


    inline void CDC::Release()
    {
        if (m_pData->count > 0)
        {
            if (InterlockedDecrement(&m_pData->count) == 0)
            {
                Destroy();
                delete m_pData;
                m_pData = 0;
            }
        }
    }


    inline BOOL CDC::RemoveFromMap()
    {
        BOOL Success = FALSE;

        if( &GetApp() )
        {
            // Allocate an iterator for our Data map
            std::map<HDC, CDC_Data*, CompareHDC>::iterator m;

            CWinApp& App = GetApp();
            CThreadLock mapLock(App.m_gdiLock);
            m = App.m_mapCDCData.find(m_pData->hDC);
            if (m != App.m_mapCDCData.end())
            {
                // Erase the CDC data entry from the map
                App.m_mapCDCData.erase(m);
                Success = TRUE;
            }

        }

        return Success;
    }


    inline BOOL CDC::RestoreDC(int savedDC) const
    {
        assert(m_pData->hDC);
        return ::RestoreDC(m_pData->hDC, savedDC);
    }


    inline int CDC::SaveDC() const
    {
        assert(m_pData->hDC);
        return ::SaveDC(m_pData->hDC);
    }


    // Select a GDI object into the device context.
    inline HGDIOBJ CDC::SelectObject(HGDIOBJ hObject) const
    {
        assert(m_pData);
        return ::SelectObject(m_pData->hDC, hObject);
    }


    // Select a bitmap into the device context.
    inline HBITMAP CDC::SelectObject(HBITMAP hBitmap) const
    {
        assert(m_pData);
        return (HBITMAP)::SelectObject(m_pData->hDC, hBitmap);
    }


    // Select a brush into the device context.
    inline HBRUSH CDC::SelectObject(HBRUSH hBrush) const
    {
        assert(m_pData);
        return reinterpret_cast<HBRUSH>(::SelectObject(m_pData->hDC, hBrush));
    }


    // Select a font into the device context.
    inline HFONT CDC::SelectObject(HFONT hFont) const
    {
        assert(m_pData);
        return reinterpret_cast<HFONT>(::SelectObject(m_pData->hDC, hFont));
    }


    // Select a pen into the device context.
    inline HPEN CDC::SelectObject(HPEN hPen) const
    {
        assert(m_pData);
        return reinterpret_cast<HPEN>(::SelectObject(m_pData->hDC, hPen));
    }


    // Select a region into the device context.
    inline int CDC::SelectObject(HRGN hRgn) const
    {
        assert(m_pData);
        INT_PTR object = reinterpret_cast<INT_PTR>(::SelectObject(m_pData->hDC, hRgn));
        return static_cast<int>(object);
    }


    // Fills a rectangle with a solid color
    inline void CDC::SolidFill(COLORREF color, const RECT& rc) const
    {
        COLORREF oldColor = SetBkColor(color);
        ExtTextOut(0, 0, ETO_OPAQUE, rc, NULL, 0, 0);
        SetBkColor(oldColor);
    }

    /////////////////////////
    // Bitmap functions

    // Creates a compatible bitmap and selects it into the device context.
    inline void CDC::CreateCompatibleBitmap(HDC hdc, int cx, int cy)
    {
        assert(m_pData->hDC);

        CBitmap bitmap;
        bitmap.CreateCompatibleBitmap(hdc, cx, cy);
        SelectObject(bitmap);
        m_pData->bitmap = bitmap;
    }


    // Creates a bitmap and selects it into the device context.
    inline void CDC::CreateBitmap(int cx, int cy, UINT planes, UINT bitsPerPixel, LPCVOID pColors)
    {
        assert(m_pData->hDC);

        CBitmap bitmap;
        bitmap.CreateBitmap(cx, cy, planes, bitsPerPixel, pColors);
        SelectObject(bitmap);
        m_pData->bitmap = bitmap;
    }

#ifndef _WIN32_WCE

    // Creates a bitmap and selects it into the device context.
    inline void CDC::CreateBitmapIndirect (const BITMAP& bitmap)
    {
        assert(m_pData->hDC);

        CBitmap newBitmap;
		newBitmap.CreateBitmapIndirect(bitmap);
        SelectObject(newBitmap);
        m_pData->bitmap = newBitmap;
    }


    // Creates a bitmap and selects it into the device context.
    // A CBitmapInfoPtr object can be used for the LPBITMAPINFO parameter.
    inline void CDC::CreateDIBitmap(HDC hdc, const BITMAPINFOHEADER& bmih, DWORD init, LPCVOID pInit,
                                        const LPBITMAPINFO pBMI,  UINT flags)
    {
        assert(m_pData->hDC);

        CBitmap newBitmap;
		newBitmap.CreateDIBitmap(hdc, &bmih, init, pInit, pBMI, flags);
        SelectObject(newBitmap);
        m_pData->bitmap = newBitmap;
    }

#endif

    // Creates a bitmap and selects it into the device context.
    // A CBitmapInfoPtr object can be used for the LPBITMAPINFO parameter.
    inline void CDC::CreateDIBSection(HDC hdc, const LPBITMAPINFO pBMI, UINT usage, LPVOID* pBits,
                                        HANDLE hSection, DWORD offset)
    {
        assert(m_pData->hDC);

        CBitmap newBitmap;
		newBitmap.CreateDIBSection(hdc, pBMI, usage, pBits, hSection, offset);
        SelectObject(newBitmap);
        m_pData->bitmap = newBitmap;
    }


    // Provides a convenient method of detaching a bitmap from a memory device context.
    // Returns the CBitmap detached from the DC.
    // Usage:  CBitmap MyBitmap = MyMemDC.DetachBitmap();
    inline CBitmap CDC::DetachBitmap()
    {
        // Create a stock bitmap to replace the current one.
        CBitmap bitmap;
        bitmap.CreateBitmap(1, 1, 1, 1, 0);

        CBitmap oldBitmap = SelectObject(bitmap);
        m_pData->bitmap = bitmap;

        return oldBitmap;
    }


    // Deletes or releases the device context and returns the CDC object to its
    // default state, ready for reuse.
    inline void CDC::Destroy()
    {
        if (m_pData->hDC != 0)
        {
            RemoveFromMap();

            if (m_pData->isManagedHDC)
            {
                // Return the DC back to its initial state
                ::RestoreDC(m_pData->hDC, m_pData->savedDCState);

                // We need to release a Window DC, and delete a memory DC
                if (m_pData->hWnd != 0)
                    ::ReleaseDC(m_pData->hWnd, m_pData->hDC);
                else
                    if (!::DeleteDC(m_pData->hDC))
                        ::ReleaseDC(NULL, m_pData->hDC);
            }

            m_pData->hDC = 0;
            m_pData->hWnd = 0;
            m_pData->isManagedHDC = FALSE;
        }
    }


    // Retrieves the BITMAP information for the current HBITMAP.
    inline BITMAP CDC::GetBitmapData() const
    {
        assert(m_pData->hDC);

        HBITMAP hbm = (HBITMAP)::GetCurrentObject(m_pData->hDC, OBJ_BITMAP);
        BITMAP bm;
        ZeroMemory(&bm, sizeof(bm));
        VERIFY(::GetObject(hbm, sizeof(bm), &bm) != 0);
        return bm;
    }


    // Retrieves the handle of the currently selected bitmap.
    inline HBITMAP CDC::GetCurrentBitmap() const
    {
        assert(m_pData->hDC);
        return static_cast<HBITMAP>(::GetCurrentObject(m_pData->hDC, OBJ_BITMAP));
    }


    // Loads a bitmap from the resource and selects it into the device context.
    // Returns TRUE if successful.
    inline BOOL CDC::LoadBitmap(UINT id)
    {
        return LoadBitmap(MAKEINTRESOURCE(id));
    }


    // Loads a bitmap from the resource and selects it into the device context.
    // Returns TRUE if successful.
    inline BOOL CDC::LoadBitmap(LPCTSTR pResName)
    {
        assert(m_pData->hDC);

        CBitmap bitmap;
        BOOL IsLoaded = bitmap.LoadBitmap(pResName);

        if (IsLoaded)
        {
            SelectObject(bitmap);
            m_pData->bitmap = bitmap;
        }

        return IsLoaded;
    }


    // Loads a bitmap from the resource and selects it into the device context.
    // The fuLoad parameter can be one of  LR_DEFAULTCOLOR, LR_CREATEDIBSECTION,
    // LR_LOADFROMFILE, LR_LOADTRANSPARENT, LR_MONOCHROME, LR_SHARED and LR_VGACOLOR.
    // Returns TRUE if successful
    inline BOOL CDC::LoadImage(UINT id, UINT flags)
    {
        return LoadImage(id, flags);
    }


    // Loads a bitmap from the resource and selects it into the device context.
    // The fuLoad parameter can be one of  LR_DEFAULTCOLOR, LR_CREATEDIBSECTION,
    // LR_LOADFROMFILE, LR_LOADTRANSPARENT, LR_MONOCHROME, LR_SHARED and LR_VGACOLOR.
    // Returns TRUE if successful
    inline BOOL CDC::LoadImage(LPCTSTR pResName, UINT flags)
    {
        assert(m_pData->hDC);

        CBitmap bitmap;
        BOOL IsLoaded = bitmap.LoadImage(pResName, flags);

        if (IsLoaded)
        {
            SelectObject(bitmap);
            m_pData->bitmap = bitmap;
        }

        return IsLoaded;
    }


    // Loads a predefined system bitmap and selects it into the device context.
    // Returns TRUE if successful.
    inline BOOL CDC::LoadOEMBitmap(UINT bitmapID) // for OBM_/OCR_/OIC_
    {
        assert(m_pData->hDC);

        CBitmap bitmap;
        BOOL IsLoaded = bitmap.LoadOEMBitmap(bitmapID);

        if (IsLoaded)
        {
            SelectObject(bitmap);
            m_pData->bitmap = bitmap;
        }

        return IsLoaded;
    }

#ifndef _WIN32_WCE

    // creates and selects a new bitmap using the bitmap data and colors specified by
    // the bitmap resource and the color mapping information.
    inline void CDC::CreateMappedBitmap(UINT bitmapID, UINT flags /*= 0*/, LPCOLORMAP pColorMap /*= NULL*/, int mapSize /*= 0*/)
    {
        assert(m_pData->hDC);

        CBitmap bitmap;
        bitmap.CreateMappedBitmap(bitmapID, static_cast<WORD>(flags), pColorMap, mapSize);
        SelectObject(bitmap);
        m_pData->bitmap = bitmap;
    }

#endif // !_WIN32_WCE

    //////////////////
    // Brush functions

    // Creates the brush with the specified pattern, and selects it into the device context.
    inline void CDC::CreatePatternBrush(HBITMAP hBitmap)
    {
        assert(m_pData->hDC);

        CBrush brush;
        brush.CreatePatternBrush(hBitmap);
        SelectObject(brush);
        m_pData->brush = brush;
    }


    // Creates the brush with the specified color, and selects it into the device context.
    inline void CDC::CreateSolidBrush(COLORREF rgb)
    {
        assert(m_pData->hDC);

        CBrush brush;
        brush.CreateSolidBrush(rgb);
        SelectObject(brush);
        m_pData->brush = brush;
    }


    // Retrieves the handle of the currently selected brush object
    inline HBRUSH CDC::GetCurrentBrush() const
    {
        assert(m_pData->hDC);
        return static_cast<HBRUSH>(::GetCurrentObject(m_pData->hDC, OBJ_BRUSH));
    }


    // Retrieves the current brush information
    inline LOGBRUSH CDC::GetLogBrush() const
    {
        assert(m_pData->hDC);

        HBRUSH hBrush = reinterpret_cast<HBRUSH>(::GetCurrentObject(m_pData->hDC, OBJ_BRUSH));
        LOGBRUSH logBrush;
        ZeroMemory(&logBrush, sizeof(logBrush));
        VERIFY(::GetObject(hBrush, sizeof(logBrush), &logBrush) != 0);
        return logBrush;
    }

#ifndef _WIN32_WCE

    // Creates the brush and selects it into the device context.
    inline void CDC::CreateBrushIndirect(const LOGBRUSH& logBrush)
    {
        assert(m_pData->hDC);

        CBrush brush;
        brush.CreateBrushIndirect(logBrush);
        SelectObject(brush);
        m_pData->brush = brush;
    }


    // Creates a brush with the specified hatch pattern and color, and selects it into the device context.
    inline void CDC::CreateHatchBrush(int style, COLORREF rgb)
    {
        assert(m_pData->hDC);

        CBrush brush;
        brush.CreateHatchBrush(style, rgb);
        SelectObject(brush);
        m_pData->brush = brush;
    }


    // Creates a logical from the specified device-independent bitmap (DIB), and selects it into the device context.
    inline void CDC::CreateDIBPatternBrush(HGLOBAL hDIBPacked, UINT colorSpec)
    {
        assert(m_pData->hDC);

        CBrush brush;
        brush.CreateDIBPatternBrush(hDIBPacked, colorSpec);
        SelectObject(brush);
        m_pData->brush = brush;
    }


    // Creates a logical from the specified device-independent bitmap (DIB), and selects it into the device context.
    inline void CDC::CreateDIBPatternBrushPt(LPCVOID pPackedDIB, UINT usage)
    {
        assert(m_pData->hDC);

        CBrush brush;
        brush.CreateDIBPatternBrushPt(pPackedDIB, usage);
        SelectObject(brush);
        m_pData->brush = brush;
    }

#endif

    /////////////////
    // Font functions

    // Creates a logical font and selects it into the device context.
    inline void CDC::CreateFontIndirect(const LOGFONT& lf)
    {
        assert(m_pData->hDC);

        CFont font;
        font.CreateFontIndirect(lf);
        SelectObject(font);
        m_pData->font = font;
    }


    // Retrieves the handle to the current font object.
    inline HFONT CDC::GetCurrentFont() const
    {
        assert(m_pData->hDC);
        return static_cast<HFONT>(::GetCurrentObject(m_pData->hDC, OBJ_FONT));
    }


    // Retrieves the current font information.
    inline LOGFONT CDC::GetLogFont() const
    {
        assert(m_pData->hDC);

        HFONT hFont = reinterpret_cast<HFONT>(::GetCurrentObject(m_pData->hDC, OBJ_FONT));
        LOGFONT logFont;
        ZeroMemory(&logFont, sizeof(logFont));
        VERIFY(::GetObject(hFont, sizeof(logFont), &logFont) != 0);
        return logFont;
    }

#ifndef _WIN32_WCE

    // Creates a logical font with the specified characteristics.
    inline void CDC::CreateFont (
                    int height,               // height of font
                    int width,                // average character width
                    int escapement,           // angle of escapement
                    int orientation,          // base-line orientation angle
                    int weight,               // font weight
                    DWORD italic,             // italic attribute option
                    DWORD underline,          // underline attribute option
                    DWORD strikeOut,          // strikeout attribute option
                    DWORD charSet,            // character set identifier
                    DWORD outputPrecision,    // output precision
                    DWORD clipPrecision,      // clipping precision
                    DWORD quality,            // output quality
                    DWORD pitchAndFamily,     // pitch and family
                    LPCTSTR pFaceName         // typeface name
                    )

    {
        assert(m_pData->hDC);

        CFont font;
        font.CreateFont(height, width, escapement, orientation, weight,
            italic, underline, strikeOut, charSet, outputPrecision,
			clipPrecision, quality, pitchAndFamily, pFaceName);
        
		SelectObject(font);
        m_pData->font = font;
    }

#endif

    ////////////////////
    // Palette functions

    // Creates and selects a palette.
    inline void CDC::CreatePalette(LPLOGPALETTE pLogPalette, BOOL forceBkgnd)
    {
        assert(m_pData->hDC);

        CPalette palette;
        palette.CreatePalette(pLogPalette);
        SelectPalette(palette, forceBkgnd);
        m_pData->palette = palette;
        RealizePalette();
    }


    // Retrieves the handle to the currently selected palette.
    inline HPALETTE CDC::GetCurrentPalette() const
    {
        assert(m_pData->hDC);
        return static_cast<HPALETTE>(::GetCurrentObject(m_pData->hDC, OBJ_PAL));
    }


    // Retrieves a color value identifying a color from the system palette that will be
    // displayed when the specified color value is used.
    inline COLORREF CDC::GetNearestColor(COLORREF color) const
    {
        assert(m_pData->hDC);
        return GetNearestColor(color);
    }


    // Use this to attach an existing palette.
    inline HPALETTE CDC::SelectPalette(const HPALETTE hPalette, BOOL forceBkgnd)
    {
        assert(m_pData->hDC);
        return static_cast<HPALETTE>(::SelectPalette(m_pData->hDC, hPalette, forceBkgnd));
    }


    // Use this to realize changes to the device context palette.
    inline void CDC::RealizePalette() const
    {
        assert(m_pData->hDC);
        ::RealizePalette(m_pData->hDC);
    }

#ifndef _WIN32_WCE

    // Creates and selects halftone palette
    inline void CDC::CreateHalftonePalette(BOOL forceBkgnd)
    {
        assert(m_pData->hDC);

        CPalette palette;
        palette.CreateHalftonePalette(*this);
        ::SelectPalette(m_pData->hDC, palette, forceBkgnd);
        m_pData->palette = palette;
        ::RealizePalette(m_pData->hDC);
    }


    // Retrieves the color adjustment values for the device context.
    inline BOOL CDC::GetColorAdjustment(LPCOLORADJUSTMENT pCA) const
    {
        assert(m_pData->hDC);
        return ::GetColorAdjustment(m_pData->hDC, pCA);
    }


    // Sets the color adjustment values for the device context.
    inline BOOL CDC::SetColorAdjustment(const COLORADJUSTMENT* pCA) const
    {
        assert(m_pData->hDC);
        return ::SetColorAdjustment(m_pData->hDC, pCA);
    }


    // Updates the client area of the specified device context by remapping the current
    // colors in the client area to the currently realized logical palette.
    inline BOOL CDC::UpdateColors() const
    {
        assert(m_pData->hDC);
        return ::UpdateColors(m_pData->hDC);
    }

#endif

    ////////////////
    // Pen functions

    // Creates the pen and selects it into the device context.
    inline void CDC::CreatePen (int style, int width, COLORREF color)
    {
        assert(m_pData->hDC);

        CPen pen;
        pen.CreatePen(style, width, color);
        SelectObject(pen);
        m_pData->pen = pen;
    }


    // Creates the pen and selects it into the device context.
    inline void CDC::CreatePenIndirect (const LOGPEN& logPen)
    {
        assert(m_pData->hDC);

        CPen pen;
        pen.CreatePenIndirect(logPen);
        SelectObject(pen);
        m_pData->pen = pen;
    }


    // Retrieves the handle to the currently selected pen.
    inline HPEN CDC::GetCurrentPen() const
    {
        assert(m_pData->hDC);
        return static_cast<HPEN>(::GetCurrentObject(m_pData->hDC, OBJ_PEN));
    }


    // Retrieves the current pen information as a LOGPEN.
    inline LOGPEN CDC::GetLogPen() const
    {
        assert(m_pData->hDC);

        HPEN hPen = reinterpret_cast<HPEN>(::GetCurrentObject(m_pData->hDC, OBJ_PEN));
        LOGPEN logPen;
        ZeroMemory(&logPen, sizeof(logPen));
        VERIFY(::GetObject(hPen, sizeof(logPen), &logPen) != 0);
        return logPen;
    }

    ////////////////////////////////////
    // Retrieve and Select Stock Objects

    // Retrieves a stock brush, pen, or font.
    // index values:  BLACK_BRUSH, DKGRAY_BRUSH, DC_BRUSH, HOLLOW_BRUSH, LTGRAY_BRUSH, NULL_BRUSH,
    //                WHITE_BRUSH, BLACK_PEN, DC_PEN, ANSI_FIXED_FONT, ANSI_VAR_FONT, DEVICE_DEFAULT_FONT,
    //                DEFAULT_GUI_FONT, OEM_FIXED_FONT, SYSTEM_FONT, or SYSTEM_FIXED_FONT.
    inline HGDIOBJ CDC::GetStockObject(int index) const
    {
        return ::GetStockObject(index);
    }


    // Selects a stock brush, pen, or font into the device context.
    // index values:  BLACK_BRUSH, DKGRAY_BRUSH, DC_BRUSH, HOLLOW_BRUSH, LTGRAY_BRUSH, NULL_BRUSH,
    //                WHITE_BRUSH, BLACK_PEN, DC_PEN, ANSI_FIXED_FONT, ANSI_VAR_FONT, DEVICE_DEFAULT_FONT,
    //                DEFAULT_GUI_FONT, OEM_FIXED_FONT, SYSTEM_FONT, or SYSTEM_FIXED_FONT.
    inline HGDIOBJ CDC::SelectStockObject(int index) const
    {
        assert(m_pData->hDC);
        HGDIOBJ hStockObject = ::GetStockObject(index);
        return SelectObject(hStockObject);
    }


    ///////////////////
    // Region functions

    // Creates a rectangular region from the rectangle co-ordinates.
    // The return value specifies the region's complexity: NULLREGION;SIMPLEREGION;COMPLEXREGION;ERROR.
    inline int CDC::CreateRectRgn(int left, int top, int right, int bottom)
    {
        assert(m_pData->hDC);

        CRgn rgn;
        rgn.CreateRectRgn(left, top, right, bottom);
        int Complexity = SelectClipRgn(rgn);
        m_pData->rgn = rgn;
        return Complexity;
    }


    // Creates a rectangular region from the rectangle co-ordinates.
    // The return value specifies the region's complexity: NULLREGION;SIMPLEREGION;COMPLEXREGION;ERROR.
    inline int CDC::CreateRectRgnIndirect(const RECT& rc)
    {
        assert(m_pData->hDC);

        CRgn rgn;
        rgn.CreateRectRgnIndirect(rc);
        int Complexity = SelectClipRgn(rgn);
        m_pData->rgn = rgn;
        return Complexity;
    }


    // Creates a region from the specified region data and transformation data.
    // The return value specifies the region's complexity: NULLREGION;SIMPLEREGION;COMPLEXREGION;ERROR.
    // Notes: GetRegionData can be used to get a region's data
    //        If the XFROM pointer is NULL, the identity transformation is used.
    inline int CDC::CreateFromData(const XFORM* pXform, DWORD count, const RGNDATA* pRgnData)
    {
        assert(m_pData->hDC);

        CRgn rgn;
        rgn.CreateFromData(pXform, count, pRgnData);
        int Complexity = SelectClipRgn(rgn);
        m_pData->rgn = rgn;
        return Complexity;
    }

#ifndef _WIN32_WCE

    // Creates the elliptical region from the bounding rectangle co-ordinates.
    // and selects it into the device context.
    // The return value specifies the region's complexity: NULLREGION;SIMPLEREGION;COMPLEXREGION;ERROR.
    inline int CDC::CreateEllipticRgn(int left, int top, int right, int bottom)
    {
        assert(m_pData->hDC);

        CRgn rgn;
        rgn.CreateEllipticRgn(left, top, right, bottom);
        int Complexity = SelectClipRgn(rgn);
        m_pData->rgn = rgn;
        return Complexity;
    }


    // Creates the elliptical region from the bounding rectangle co-ordinates.
    // and selects it into the device context.
    // The return value specifies the region's complexity: NULLREGION;SIMPLEREGION;COMPLEXREGION;ERROR.
    inline int CDC::CreateEllipticRgnIndirect(const RECT& rc)
    {
        assert(m_pData->hDC);

        CRgn rgn;
        rgn.CreateEllipticRgnIndirect(rc);
        int Complexity = SelectClipRgn(rgn);
        m_pData->rgn = rgn;
        return Complexity;
    }


    // Creates the polygon region from the array of points and selects it into
    // the device context. The polygon is presumed closed.
    // The return value specifies the region's complexity: NULLREGION;SIMPLEREGION;COMPLEXREGION;ERROR.
    inline int CDC::CreatePolygonRgn(LPPOINT pPointArray, int points, int polyFillMode)
    {
        assert(m_pData->hDC);

        CRgn rgn;
        rgn.CreatePolygonRgn(pPointArray, points, polyFillMode);
        int Complexity = SelectClipRgn(rgn);
        m_pData->rgn = rgn;
        return Complexity;
    }


    // Creates the polygon region from a series of polygons.The polygons can overlap.
    // The return value specifies the region's complexity: NULLREGION;SIMPLEREGION;COMPLEXREGION;ERROR.
    inline int CDC::CreatePolyPolygonRgn(LPPOINT pPointArray, LPINT pCount, int count, int polyFillMode)
    {
        assert(m_pData->hDC);

        CRgn rgn;
        rgn.CreatePolyPolygonRgn(pPointArray, pCount, count, polyFillMode);
        int Complexity = SelectClipRgn(rgn);
        m_pData->rgn = rgn;
        return Complexity;
    }

#endif

    ////////////////////////////////
    // Wrappers for WinAPI functions


    // Retrieves device-specific information for the specified device.
    inline int CDC::GetDeviceCaps (int index) const
    {
        assert(m_pData->hDC);
        return ::GetDeviceCaps(m_pData->hDC, index);
    }

    //////////////////
    // Brush functions

#if (_WIN32_WINNT >= 0x0500)

    // Retrieves the current brush color from the device context.
    inline COLORREF CDC::GetDCBrushColor() const
    {
        assert(m_pData->hDC);
        return ::GetDCBrushColor(m_pData->hDC);
    }


    // Sets the current brush color for the device context.
    inline COLORREF CDC::SetDCBrushColor(COLORREF color) const
    {
        assert(m_pData->hDC);
        return ::SetDCBrushColor(m_pData->hDC, color);
    }

#endif

    /////////////////
    // Font Functions


#ifndef _WIN32_WCE

    // Retrieves font metric data for a TrueType font.
    inline DWORD CDC::GetFontData(DWORD table, DWORD offset, LPVOID pBuffer, DWORD data) const
    {
        assert(m_pData->hDC);
        return ::GetFontData(m_pData->hDC, table, offset, pBuffer, data);
    }


    // Returns information about the currently selected font for the display context.
    inline DWORD CDC::GetFontLanguageInfo() const
    {
        assert(m_pData->hDC);
        return ::GetFontLanguageInfo(m_pData->hDC);
    }


    // Retrieves the outline or bitmap for a character in the TrueType font that is selected into the device context.
    inline DWORD CDC::GetGlyphOutline(UINT query, UINT format, LPGLYPHMETRICS pGM, DWORD bufSize,
                              LPVOID pBuffer, const MAT2* pMAT2) const
    {
        assert(m_pData->hDC);
        return ::GetGlyphOutline(m_pData->hDC, query, format, pGM, bufSize, pBuffer, pMAT2);
    }


    // retrieves the character-kerning pairs for the currently selected font for the device context.
    inline DWORD CDC::GetKerningPairs(DWORD numPairs, LPKERNINGPAIR pKrnPair) const
    {
        assert(m_pData->hDC);
        return ::GetKerningPairs(m_pData->hDC, numPairs, pKrnPair);
    }


    // Alters the algorithm the font mapper uses when it maps logical fonts to physical fonts.
    inline DWORD CDC::SetMapperFlags(DWORD flag) const
    {
        assert(m_pData->hDC);
        return ::SetMapperFlags(m_pData->hDC, flag);
    }


    ////////////////
    // Pen Functions


    // Retrieves the miter limit for the device context. The miter limit is used when
    // drawing geometric lines that have miter joins.
    inline BOOL CDC::GetMiterLimit(PFLOAT pLimit) const
    {
        assert(m_pData->hDC);
        return ::GetMiterLimit(m_pData->hDC, pLimit);
    }


    // Sets the limit for the length of miter joins for the device context.
    inline BOOL CDC::SetMiterLimit(FLOAT newLimit, PFLOAT pOldLimit) const

    {
        assert(m_pData->hDC);
        return ::SetMiterLimit(m_pData->hDC, newLimit, pOldLimit);
    }

#endif

    /////////////////////
    // Clipping functions


    // Creates a new clipping region that consists of the existing clipping region minus
    // the specified rectangle.
    inline int CDC::ExcludeClipRect(int left, int top, int right, int bottom) const
    {
        assert(m_pData->hDC);
        return ::ExcludeClipRect(m_pData->hDC, left, top, right, bottom);
    }


    // Creates a new clipping region that consists of the existing clipping region minus
    // the specified rectangle.
    inline int CDC::ExcludeClipRect(const RECT& rc) const

    {
        assert(m_pData->hDC);
        return ::ExcludeClipRect(m_pData->hDC, rc.left, rc.top, rc.right, rc.bottom);
    }


    // Retrieves the dimensions of the tightest bounding rectangle that can be drawn around
    // the current visible area on the device.
    inline int CDC::GetClipBox (RECT& rc) const
    {
        assert(m_pData->hDC);
        return ::GetClipBox(m_pData->hDC, &rc);
    }


    // Creates a new clipping region from the intersection of the current clipping region and
    // the specified rectangle.
    inline int CDC::IntersectClipRect(int left, int top, int right, int bottom) const
    {
        assert(m_pData->hDC);
        return ::IntersectClipRect(m_pData->hDC, left, top, right, bottom);
    }


    // Creates a new clipping region from the intersection of the current clipping region
    // and the specified rectangle.
    inline int CDC::IntersectClipRect(const RECT& rc) const
    {
        assert(m_pData->hDC);
        return ::IntersectClipRect(m_pData->hDC, rc.left, rc.top, rc.right, rc.bottom);
    }


    // Determines whether any part of the specified rectangle lies within the
    // clipping region of a device context.
    inline BOOL CDC::RectVisible(const RECT& rc) const
    {
        assert(m_pData->hDC);
        return ::RectVisible (m_pData->hDC, &rc);
    }


    // Selects a region as the current clipping region for the specified device context.
    // Note: Only a copy of the selected region is used.
    //       To remove a device-context's clipping region, specify a NULL region handle.
    inline int CDC::SelectClipRgn(HRGN hRgn) const
    {
        assert(m_pData->hDC);
        return ::SelectClipRgn(m_pData->hDC, hRgn);
    }

#ifndef _WIN32_WCE

    // Opens a path bracket in the device context.
    inline BOOL CDC::BeginPath() const
    {
        assert(m_pData->hDC);
        return ::BeginPath(m_pData->hDC);
    }


    // Closes a path bracket and selects the path defined by the bracket into the device context.
    inline BOOL CDC::EndPath() const
    {
        assert(m_pData->hDC);
        return ::EndPath(m_pData->hDC);
    }


    // Combines the specified region with the current clipping region using the specified mode.
    inline int CDC::ExtSelectClipRgn(HRGN hRgn, int mode) const
    {
        assert(m_pData->hDC);
        return ::ExtSelectClipRgn(m_pData->hDC, hRgn, mode);
    }


    // Transforms any curves in the path that is selected into the device context, turning each
    // curve into a sequence of lines.
    inline BOOL CDC::FlattenPath() const
    {
        assert(m_pData->hDC);
        return ::FlattenPath(m_pData->hDC);
    }


    // Retrieves the coordinates defining the endpoints of lines and the control points of curves found in the path
    // that is selected into the device context.
    // pPoints: An array of POINT structures that receives the line endpoints and curve control points, in logical coordinates.
    // pTypes: Pointer to an array of bytes that receives the vertex types (PT_MOVETO, PT_LINETO or PT_BEZIERTO).
    // count: The total number of POINT structures that can be stored in the array pointed to by pPoints.
    inline int CDC::GetPath(POINT* pPoints, BYTE* pTypes, int count) const
    {
        assert(m_pData->hDC);
        return ::GetPath(m_pData->hDC, pPoints, pTypes, count);
    }


    // Determines whether the specified point is within the clipping region of a device context.
    inline BOOL CDC::PtVisible(int x, int y) const
    {
        assert(m_pData->hDC);
        return ::PtVisible (m_pData->hDC, x, y);
    }


    // Moves the clipping region of a device context by the specified offsets.
    inline int CDC::OffsetClipRgn(int xOffset, int yOffset) const
    {
        assert(m_pData->hDC);
        return ::OffsetClipRgn (m_pData->hDC, xOffset, yOffset);
    }


    // Selects the current path as a clipping region for the device context, combining
    // the new region with any existing clipping region using the specified mode.
    inline BOOL CDC::SelectClipPath(int mode) const
    {
        assert(m_pData->hDC);
        return ::SelectClipPath(m_pData->hDC, mode);
    }


    // Redefines the current path as the area that would be painted if the path were
    // stroked using the pen currently selected into the device context.
    inline BOOL CDC::WidenPath() const
    {
        assert(m_pData->hDC);
        return ::WidenPath(m_pData->hDC);
    }

#endif

    ///////////////////////////////////
    // Point and Line Drawing Functions


    // Returns the current "MoveToEx" position.
    inline CPoint CDC::GetCurrentPosition() const
    {
        assert(m_pData->hDC);
        CPoint pt;
        ::MoveToEx(m_pData->hDC, 0, 0, &pt);
        ::MoveToEx(m_pData->hDC, pt.x, pt.y, NULL);
        return pt;
    }


    // Retrieves the red, green, blue (RGB) color value of the pixel at the specified coordinates.
    inline COLORREF CDC::GetPixel(int x, int y) const
    {
        assert(m_pData->hDC);
        return ::GetPixel(m_pData->hDC, x, y);
    }


    // Retrieves the red, green, blue (RGB) color value of the pixel at the specified coordinates.
    inline COLORREF CDC::GetPixel(POINT pt) const
    {
        assert(m_pData->hDC);
        return ::GetPixel(m_pData->hDC, pt.x, pt.y);
    }


    // Updates the current position to the specified point.
    inline CPoint CDC::MoveTo(int x, int y) const
    {
        assert(m_pData->hDC);
        return ::MoveToEx(m_pData->hDC, x, y, NULL);
    }


    // Updates the current position to the specified point
    inline CPoint CDC::MoveTo(POINT pt) const
    {
        assert(m_pData->hDC);
        return ::MoveToEx(m_pData->hDC, pt.x, pt.y, NULL);
    }


    // Draws a line from the current position up to, but not including, the specified point.
    inline BOOL CDC::LineTo(int x, int y) const
    {
        assert(m_pData->hDC);
        return ::LineTo(m_pData->hDC, x, y);
    }


    // Draws a line from the current position up to, but not including, the specified point.
    inline BOOL CDC::LineTo(POINT pt) const
    {
        assert(m_pData->hDC);
        return ::LineTo(m_pData->hDC, pt.x, pt.y);
    }


    // Sets the current foreground mix mode. GDI uses the foreground mix mode to
    // combine pens and interiors of filled objects with the colors already on the screen.
    inline int CDC::SetROP2(int drawMode) const
    {
        assert(m_pData->hDC);
        return ::SetROP2(m_pData->hDC, drawMode);
    }


    // Sets the pixel at the specified coordinates to the specified color.
    inline COLORREF CDC::SetPixel (int x, int y, COLORREF color) const
    {
        assert(m_pData->hDC);
        return ::SetPixel(m_pData->hDC, x, y, color);
    }

#ifndef _WIN32_WCE

    // Draws an elliptical arc.
    inline BOOL CDC::Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const
    {
        assert(m_pData->hDC);
        return ::Arc(m_pData->hDC, x1, y1, x2, y2, x3, y3, x4, y4);
    }


    // Draws an elliptical arc.
    inline BOOL CDC::Arc(const RECT& rc, POINT ptStart, POINT ptEnd) const
    {
        assert(m_pData->hDC);
        return ::Arc(m_pData->hDC, rc.left, rc.top, rc.right, rc.bottom,
            ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
    }


    // Draws an elliptical arc.
    inline BOOL CDC::ArcTo(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const
    {
        assert(m_pData->hDC);
        return ::ArcTo(m_pData->hDC, x1, y1, x2, y2, x3, y3, x4, y4);
    }


    // Draws an elliptical arc.
    inline BOOL CDC::ArcTo(const RECT& rc, POINT ptStart, POINT ptEnd) const
    {
        assert(m_pData->hDC);
        return ::ArcTo (m_pData->hDC, rc.left, rc.top, rc.right, rc.bottom,
            ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
    }


    // Draws a line segment and an arc.
    inline BOOL CDC::AngleArc(int x, int y, int radius, float startAngle, float sweepAngle) const
    {
        assert(m_pData->hDC);
        return ::AngleArc(m_pData->hDC, x, y, radius, startAngle, sweepAngle);
    }


    // Closes the figure by drawing a line from the current position to the first point of the figure.
    inline BOOL CDC::CloseFigure() const
    {
        assert(m_pData->hDC);
        return ::CloseFigure(m_pData->hDC);
    }


    // Retrieves the foreground mix mode of the specified device context. The mix mode specifies how
    // the pen or interior color and the color already on the screen are combined to yield a new color.
    inline int CDC::GetROP2() const
    {
        assert(m_pData->hDC);
        return ::GetROP2(m_pData->hDC);
    }


    // Retrieves the current arc direction (AD_COUNTERCLOCKWISE or AD_CLOCKWISE).
    inline int CDC::GetArcDirection() const
    {
        assert(m_pData->hDC);
        return ::GetArcDirection(m_pData->hDC);
    }


    // Sets the current arc direction (AD_COUNTERCLOCKWISE or AD_CLOCKWISE).
    inline int CDC::SetArcDirection(int arcDirection) const
    {
        assert(m_pData->hDC);
        return ::SetArcDirection(m_pData->hDC, arcDirection);
    }


    // Draws a set of line segments and Bezier curves.
    inline BOOL CDC::PolyDraw(const POINT* pPointArray, const BYTE* pTypes, int count) const
    {
        assert(m_pData->hDC);
        return ::PolyDraw(m_pData->hDC, pPointArray, pTypes, count);
    }


    // Draws a series of line segments by connecting the points in the specified array.
    inline BOOL CDC::Polyline(LPPOINT pPointArray, int count) const
    {
        assert(m_pData->hDC);
        return ::Polyline(m_pData->hDC, pPointArray, count);
    }


    // Draws multiple series of connected line segments.
    inline BOOL CDC::PolyPolyline(const POINT* pPointArray, const DWORD* pPolyPoints, int count) const
    {
        assert(m_pData->hDC);
        return ::PolyPolyline(m_pData->hDC, pPointArray, pPolyPoints, count);
    }


    // Draws one or more straight lines.
    inline BOOL CDC::PolylineTo(const POINT* pPointArray, int count) const
    {
        assert(m_pData->hDC);
        return ::PolylineTo(m_pData->hDC, pPointArray, count);
    }


    // Draws one or more Bezier curves.
    inline BOOL CDC::PolyBezier(const POINT* pPointArray, int count) const
    {
        assert(m_pData->hDC);
        return ::PolyBezier(m_pData->hDC, pPointArray, count);
    }


    // Draws one or more Bezier curves.
    inline BOOL CDC::PolyBezierTo(const POINT* pPointArray, int count) const
    {
        assert(m_pData->hDC);
        return ::PolyBezierTo(m_pData->hDC, pPointArray, count );
    }


    // Sets the pixel at the specified coordinates to the specified color.
    inline COLORREF CDC::SetPixel(POINT pt, COLORREF color) const
    {
        assert(m_pData->hDC);
        return ::SetPixel(m_pData->hDC, pt.x, pt.y, color);
    }


    // Sets the pixel at the specified coordinates to the closest approximation of the specified color.
    inline BOOL CDC::SetPixelV(int x, int y, COLORREF color) const
    {
        assert(m_pData->hDC);
        return ::SetPixelV(m_pData->hDC, x, y, color);
    }


    // Sets the pixel at the specified coordinates to the closest approximation of the specified color.
    inline BOOL CDC::SetPixelV(POINT pt, COLORREF color) const

    {
        assert(m_pData->hDC);
        return ::SetPixelV(m_pData->hDC, pt.x, pt.y, color);
    }

#endif

    //////////////////////////
    // Shape Drawing Functions


    // Draws a rectangle in the style used to indicate that the rectangle has the focus.
    inline void CDC::DrawFocusRect(const RECT& rc) const
    {
        assert(m_pData->hDC);
        ::DrawFocusRect(m_pData->hDC, &rc);
    }


    // Draws an ellipse. The center of the ellipse is the center of the specified bounding rectangle.
    inline BOOL CDC::Ellipse(int x1, int y1, int x2, int y2) const
    {
        assert(m_pData->hDC);
        return ::Ellipse(m_pData->hDC, x1, y1, x2, y2);
    }


    // Draws an ellipse. The center of the ellipse is the center of the specified bounding rectangle.
    inline BOOL CDC::Ellipse(const RECT& rc) const
    {
        assert(m_pData->hDC);
        return ::Ellipse(m_pData->hDC, rc.left, rc.top, rc.right, rc.bottom);
    }


    // Draws a polygon consisting of two or more vertices connected by straight lines.
    inline BOOL CDC::Polygon(LPPOINT pPointArray, int count) const
    {
        assert(m_pData->hDC);
        return ::Polygon(m_pData->hDC, pPointArray, count);
    }


    // Draws a rectangle. The rectangle is outlined by using the current pen and filled
    // by using the current brush.
    inline BOOL CDC::Rectangle(int x1, int y1, int x2, int y2) const
    {
        assert(m_pData->hDC);
        return ::Rectangle(m_pData->hDC, x1, y1, x2, y2);
    }


    // Draws a rectangle. The rectangle is outlined by using the current pen and filled
    //  by using the current brush.
    inline BOOL CDC::Rectangle(const RECT& rc) const
    {
        assert(m_pData->hDC);
        return ::Rectangle(m_pData->hDC, rc.left, rc.top, rc.right, rc.bottom);
    }


    // Draws a rectangle with rounded corners.
    inline BOOL CDC::RoundRect(int x1, int y1, int x2, int y2, int width, int height) const
    {
        assert(m_pData->hDC);
        return ::RoundRect(m_pData->hDC, x1, y1, x2, y2, width, height);
    }


    // Draws a rectangle with rounded corners.
    inline BOOL CDC::RoundRect(const RECT& rc, int width, int height) const
    {
        assert(m_pData->hDC);
        return ::RoundRect(m_pData->hDC, rc.left, rc.top, rc.right, rc.bottom, width, height );
    }

#ifndef _WIN32_WCE

    // Draws a chord (a region bounded by the intersection of an ellipse and a line segment, called a secant).
    inline BOOL CDC::Chord(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const
    {
        assert(m_pData->hDC);
        return ::Chord(m_pData->hDC, x1, y1, x2, y2, x3, y3, x4, y4);
    }


    // Draws a chord (a region bounded by the intersection of an ellipse and a line segment, called a secant).
    inline BOOL CDC::Chord(const RECT& rc, POINT start, POINT end) const
    {
        assert(m_pData->hDC);
        return ::Chord(m_pData->hDC, rc.left, rc.top, rc.right, rc.bottom,
            start.x, start.y, end.x, end.y);
    }


    // Draws a pie-shaped wedge bounded by the intersection of an ellipse and two radials.
    inline BOOL CDC::Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const
    {
        assert(m_pData->hDC);
        return ::Pie(m_pData->hDC, x1, y1, x2, y2, x3, y3, x4, y4);
    }


    // Draws a pie-shaped wedge bounded by the intersection of an ellipse and two radials.
    inline BOOL CDC::Pie(const RECT& rc, POINT start, POINT end) const
    {
        assert(m_pData->hDC);
        return ::Pie(m_pData->hDC, rc.left, rc.top, rc.right, rc.bottom,
            start.x, start.y, end.x, end.y);
    }


    // Draws a series of closed polygons.
    inline BOOL CDC::PolyPolygon(LPPOINT pPointArray, LPINT pPolyCounts, int count) const
    {
        assert(m_pData->hDC);
        return ::PolyPolygon(m_pData->hDC, pPointArray, pPolyCounts, count);
    }

#endif

    ////////////////////////////////
    // Fill and 3D Drawing functions


    // Fills a rectangle by using the specified brush.
    inline BOOL CDC::FillRect(const RECT& rc, HBRUSH hBrush) const
    {
        assert(m_pData->hDC);
        return (::FillRect(m_pData->hDC, &rc, hBrush) != 0);
    }


    // Inverts a rectangle in a window by performing a logical NOT operation on the color
    // values for each pixel in the rectangle's interior.
    inline BOOL CDC::InvertRect(const RECT& rc) const
    {
        assert(m_pData->hDC);
        return ::InvertRect( m_pData->hDC, &rc);
    }


    // draws an icon or cursor, performing the specified raster operations, and stretching
    // or compressing the icon or cursor as specified.
    inline BOOL CDC::DrawIconEx(int xLeft, int yTop, HICON hIcon, int cxWidth, int cyWidth, UINT index, HBRUSH hFlickerFreeDraw, UINT flags) const
    {
        assert(m_pData->hDC);
        return ::DrawIconEx(m_pData->hDC, xLeft, yTop, hIcon, cxWidth, cyWidth, index, hFlickerFreeDraw, flags);
    }


    // Draws one or more edges of rectangle.
    inline BOOL CDC::DrawEdge(const RECT& rc, UINT edge, UINT flags) const
    {
        assert(m_pData->hDC);
        return ::DrawEdge(m_pData->hDC, (LPRECT)&rc, edge, flags);
    }


    // Draws a frame control of the specified type and style.
    inline BOOL CDC::DrawFrameControl(const RECT& rc, UINT type, UINT state) const
    {
        assert(m_pData->hDC);
        return ::DrawFrameControl(m_pData->hDC, (LPRECT)&rc, type, state);
    }


    // Fills a region by using the specified brush.
    inline BOOL CDC::FillRgn(HRGN hRgn, HBRUSH hBrush) const
    {
        assert(m_pData->hDC);
        return ::FillRgn(m_pData->hDC, hRgn, hBrush);
    }

  #if (WINVER >= 0x0410)

    // Fills rectangle and triangle structures.
    inline BOOL CDC::GradientFill(PTRIVERTEX pVertex, ULONG vertex, PVOID pMesh, ULONG mesh, ULONG mode) const
    {
        assert(m_pData->hDC);
        return ::GradientFill(m_pData->hDC, pVertex, vertex, pMesh, mesh, mode);
    }

  #endif

#ifndef _WIN32_WCE

    // Draws an icon or cursor.
    inline BOOL CDC::DrawIcon(int x, int y, HICON hIcon) const
    {
        assert(m_pData->hDC);
        return ::DrawIcon(m_pData->hDC, x, y, hIcon);
    }


    // Draws an icon or cursor.
    inline BOOL CDC::DrawIcon(POINT pt, HICON hIcon) const
    {
        assert(m_pData->hDC);
        return ::DrawIcon(m_pData->hDC, pt.x, pt.y, hIcon);
    }


    // Draws a border around the specified rectangle by using the specified brush.
    inline BOOL CDC::FrameRect(const RECT& rc, HBRUSH hBrush) const
    {
        assert(m_pData->hDC);
        return (::FrameRect(m_pData->hDC, &rc, hBrush) != 0);
    }


    // Draws a border around the specified region by using the specified brush.
    inline BOOL CDC::FrameRgn(HRGN hRgn, HBRUSH hBrush, int width, int height) const
    {
        assert(m_pData->hDC);
        return (::FrameRgn(m_pData->hDC, hRgn, hBrush, width, height) != 0);
    }


    // Retrieves the current polygon fill mode.
    inline int CDC::GetPolyFillMode() const
    {
        assert(m_pData->hDC);
        return ::GetPolyFillMode(m_pData->hDC);
    }


    // Paints the specified region by using the brush currently selected into the device context.
    inline BOOL CDC::PaintRgn(HRGN hRgn) const
    {
        assert(m_pData->hDC);
        return (::PaintRgn(m_pData->hDC, hRgn) != 0);
    }


    // Sets the polygon fill mode for functions that fill polygons.
    inline int CDC::SetPolyFillMode(int polyFillMode) const
    {
        assert(m_pData->hDC);
        return ::SetPolyFillMode(m_pData->hDC, polyFillMode);
    }

#endif

    ///////////////////
    // Bitmap Functions

    // Copies the color data for a rectangle of pixels in a DIB to the specified destination rectangle.
    // A CBitmapInfoPtr object can be used for the LPBITMAPINFO parameter.
    inline int CDC::StretchDIBits(int xDest, int yDest, int destWidth, int destHeight, int xSrc, int ySrc, int srcWidth,
                   int srcHeight, LPCVOID pBits, const LPBITMAPINFO pBMI, UINT usage, DWORD rop) const
    {
        assert(m_pData->hDC);
        return ::StretchDIBits(m_pData->hDC, xDest, yDest, destWidth, destHeight, xSrc, ySrc, srcWidth, srcHeight, pBits, pBMI, usage, rop);
    }


    // Paints the specified rectangle using the brush that is currently selected into the device context.
    inline BOOL CDC::PatBlt(int x, int y, int nWidth, int nHeight, DWORD rop) const
    {
        assert(m_pData->hDC);
        return ::PatBlt(m_pData->hDC, x, y, nWidth, nHeight, rop);
    }


    // Performs a bit-block transfer of the color data corresponding to a rectangle of pixels
    // from the specified source device context into a destination device context.
    inline BOOL CDC::BitBlt(int x, int y, int width, int height, HDC hSrc, int xSrc, int ySrc, DWORD rop) const
    {
        assert(m_pData->hDC);
        return ::BitBlt(m_pData->hDC, x, y, width, height, hSrc, xSrc, ySrc, rop);
    }


    // Combines the color data for the source and destination bitmaps using the specified mask and raster operation.
    //  xDest     x-coord of destination upper-left corner
    //  yDest     y-coord of destination upper-left corner
    //  width     width of source and destination
    //  yeight    height of source and destination
    //  hSrc      pointer to source DC
    //  xSrc      x-coord of upper-left corner of source
    //  ySrc      y-coord of upper-left corner of source
    //  hMask     handle to monochrome bit mask
    //  xMask     horizontal offset into mask bitmap
    //  yMask     vertical offset into mask bitmap
    //  rop       raster operation code
    inline BOOL CDC::MaskBlt(int xDest, int yDest, int width, int height, HDC hSrc, int xSrc, int ySrc, HBITMAP hMask, int xMask, int yMask, DWORD rop) const
    {
        assert(m_pData->hDC);
        return ::MaskBlt(m_pData->hDC, xDest, yDest, width, height, hSrc, xSrc, ySrc, hMask, xMask, yMask, rop);
    }


    // Copies a bitmap from a source rectangle into a destination rectangle, stretching or compressing
    // the bitmap to fit the dimensions of the destination rectangle, if necessary.
    //  x            x-coord of destination upper-left corner
    //  y            y-coord of destination upper-left corner
    //  width        width of destination rectangle
    //  height       height of destination rectangle
    //  hSrc         handle to source DC
    //  xSrc         x-coord of source upper-left corner
    //  ySrc         y-coord of source upper-left corner
    //  srcWidth     width of source rectangle
    //  srcHeight    height of source rectangle
    //  rop          raster operation code
    inline BOOL CDC::StretchBlt(int x, int y, int nWidth, int height, HDC hSrc, int xSrc, int ySrc, int srcWidth, int srcHeight, DWORD rop) const
    {
        assert(m_pData->hDC);
        return ::StretchBlt(m_pData->hDC, x, y, nWidth, height, hSrc, xSrc, ySrc, srcWidth, srcHeight, rop);
    }

#ifndef _WIN32_WCE

    // Retrieves the bits of the specified compatible bitmap and copies them into a buffer as a DIB using the specified format.
    // A CBitmapInfoPtr object can be used for the LPBITMAPINFO parameter.
    inline int CDC::GetDIBits(HBITMAP hBitmap, UINT startScan, UINT scanLines, LPVOID pBits, LPBITMAPINFO pBMI, UINT usage) const
    {
        assert(m_pData->hDC);
        return ::GetDIBits(m_pData->hDC, hBitmap, startScan, scanLines, pBits, pBMI, usage);
    }


    // Sets the pixels in a compatible bitmap (DDB) using the color data found in the specified DIB.
    // A CBitmapInfoPtr object can be used for the LPBITMAPINFO parameter.
    inline int CDC::SetDIBits(HBITMAP hBitmap, UINT startScan, UINT scanLines, LPCVOID pBits, LPBITMAPINFO pBMI, UINT colorUse) const
    {
        assert(m_pData->hDC);
        return ::SetDIBits(m_pData->hDC, hBitmap, startScan, scanLines, pBits, pBMI, colorUse);
    }


    // Retrieves the current stretching mode.
    // Possible modes: BLACKONWHITE, COLORONCOLOR, HALFTONE, STRETCH_ANDSCANS, STRETCH_DELETESCANS,
    //                 STRETCH_HALFTONE, STRETCH_ORSCANS, WHITEONBLACK
    inline int CDC::GetStretchBltMode() const
    {
        assert(m_pData->hDC);
        return ::GetStretchBltMode(m_pData->hDC);
    }


    // Sets the stretching mode.
    // Possible modes: BLACKONWHITE, COLORONCOLOR, HALFTONE, STRETCH_ANDSCANS, STRETCH_DELETESCANS,
    //                 STRETCH_HALFTONE, STRETCH_ORSCANS, WHITEONBLACK
    inline int CDC::SetStretchBltMode(int stretchMode) const
    {
        assert(m_pData->hDC);
        return ::SetStretchBltMode(m_pData->hDC, stretchMode);
    }

#if (WINVER >= 0x0410)

    // Performs a bit-block transfer of the color data corresponding to a rectangle
    // of pixels from the specified source device context into a destination device context.
    //  x             x-coord of destination upper-left corner
    //  y             y-coord of destination upper-left corner
    //  width         width of destination rectangle
    //  height        height of destination rectangle
    //  hSrc          handle to source DC
    //  xSrc          x-coord of source upper-left corner
    //  ySrc          y-coord of source upper-left corner
    //  widthSrc      width of source rectangle
    //  heightSrc     height of source rectangle
    //  transparent   color to make transparent
    inline BOOL CDC::TransparentBlt(int x, int y, int width, int height, HDC hSrc, int xSrc, int ySrc,
                                     int widthSrc, int heightSrc, UINT transparent) const
    {
        assert(m_pData->hDC);
        return ::TransparentBlt(m_pData->hDC, x, y, width, height, hSrc, xSrc, ySrc, widthSrc, heightSrc, transparent);
    }

#endif

    // Fills an area of the display surface with the current brush.
    inline BOOL CDC::FloodFill(int x, int y, COLORREF color) const
    {
        assert(m_pData->hDC);
        return ::FloodFill(m_pData->hDC, x, y, color);
    }


    // Fills an area of the display surface with the current brush.
    // Fill type: FLOODFILLBORDER or FLOODFILLSURFACE
    inline BOOL CDC::ExtFloodFill(int x, int y, COLORREF color, UINT fillType) const
    {
        assert(m_pData->hDC);
        return ::ExtFloodFill(m_pData->hDC, x, y, color, fillType );
    }

#endif

#ifndef _WIN32_WCE

    ////////////////////////
    // co-ordinate functions

    // Converts device coordinates into logical coordinates.
    inline BOOL CDC::DPtoLP(LPPOINT pPointArray, int count) const
    {
        assert(m_pData->hDC);
        return ::DPtoLP(m_pData->hDC, pPointArray, count);
    }


    // Converts device coordinates into logical coordinates.
    inline BOOL CDC::DPtoLP(RECT& rc) const
    {
        assert(m_pData->hDC);
        return ::DPtoLP(m_pData->hDC, (LPPOINT)&rc, 2);
    }


    // Converts logical coordinates into device coordinates.
    inline BOOL CDC::LPtoDP(LPPOINT pPointArray, int count) const
    {
        assert(m_pData->hDC);
        return ::LPtoDP(m_pData->hDC, pPointArray, count);
    }


    // Converts logical coordinates into device coordinates.
    inline BOOL CDC::LPtoDP(RECT& rc) const
    {
        assert(m_pData->hDC);
        return ::LPtoDP(m_pData->hDC, (LPPOINT)&rc, 2);
    }

#endif

    ///////////////////
    // Layout Functions


    // Returns the layout of a device context (LAYOUT_RTL and LAYOUT_BITMAPORIENTATIONPRESERVED).
    inline DWORD CDC::GetLayout() const
    {
#if (WINVER >= 0x0500)
        return ::GetLayout(m_pData->hDC);
#else
        return 0;
#endif
    }


    // changes the layout of a device context (DC).
    // dwLayout values:  LAYOUT_RTL or LAYOUT_BITMAPORIENTATIONPRESERVED
    inline DWORD CDC::SetLayout(DWORD layout) const
    {
#if (WINVER >= 0x0500)
        // Sets the layout of a device context
        return ::SetLayout(m_pData->hDC, layout);
#else
        UNREFERENCED_PARAMETER(layout); // no-op
        return 0;
#endif
    }

    ////////////////////
    // Mapping Functions


#ifndef _WIN32_WCE

    // Retrieves the current mapping mode.
    // Possible modes: MM_ANISOTROPIC, MM_HIENGLISH, MM_HIMETRIC, MM_ISOTROPIC, MM_LOENGLISH, MM_LOMETRIC, MM_TEXT, and MM_TWIPS.
    inline int CDC::GetMapMode()  const
    {
        assert(m_pData->hDC);
        return ::GetMapMode(m_pData->hDC);
    }


    // Retrieves the x-coordinates and y-coordinates of the viewport origin for the device context.
    inline BOOL CDC::GetViewportOrgEx(LPPOINT pPoint)  const
    {
        assert(m_pData->hDC);
        return ::GetViewportOrgEx(m_pData->hDC, pPoint);
    }


    // Sets the mapping mode of the specified device context.
    inline int CDC::SetMapMode(int mapMode) const
    {
        assert(m_pData->hDC);
        return ::SetMapMode(m_pData->hDC, mapMode);
    }


    // Specifies which device point maps to the window origin (0,0).
    inline BOOL CDC::SetViewportOrgEx(int x, int y, LPPOINT pPoint /* = NULL */) const
    {
        assert(m_pData->hDC);
        return ::SetViewportOrgEx(m_pData->hDC, x, y, pPoint);
    }


    // Specifies which device point maps to the window origin (0,0).
    inline BOOL CDC::SetViewportOrgEx(POINT point, LPPOINT pPointRet /* = NULL */) const
    {
        assert(m_pData->hDC);
        return SetViewportOrgEx(point.x, point.y, pPointRet);
    }


    // Modifies the viewport origin for the device context using the specified horizontal and vertical offsets.
    inline BOOL CDC::OffsetViewportOrgEx(int width, int height, LPPOINT pPoint /* = NULL */) const
    {
        assert(m_pData->hDC);
        return ::OffsetViewportOrgEx(m_pData->hDC, width, height, pPoint);
    }


    // Retrieves the x-extent and y-extent of the current viewport for the device context.
    inline BOOL CDC::GetViewportExtEx(LPSIZE pSize)  const
    {
        assert(m_pData->hDC);
        return ::GetViewportExtEx(m_pData->hDC, pSize);
    }


    // Sets the horizontal and vertical extents of the viewport for the device context by using the specified values.
    inline BOOL CDC::SetViewportExtEx(int x, int y, LPSIZE pSize ) const
    {
        assert(m_pData->hDC);
        return ::SetViewportExtEx(m_pData->hDC, x, y, pSize);
    }


    // Sets the horizontal and vertical extents of the viewport for the device context by using the specified values.
    inline BOOL CDC::SetViewportExtEx(SIZE size, LPSIZE pSizeRet ) const
    {
        assert(m_pData->hDC);
        return SetViewportExtEx(size.cx, size.cy, pSizeRet);
    }


    // Modifies the viewport for the device context using the ratios formed by the specified multiplicands and divisors.
    inline BOOL CDC::ScaleViewportExtEx(int xNum, int xDenom, int yNum, int yDenom, LPSIZE pSize ) const
    {
        assert(m_pData->hDC);
        return ::ScaleViewportExtEx(m_pData->hDC, xNum, xDenom, yNum, yDenom, pSize);
    }


    // Retrieves the x-coordinates and y-coordinates of the window origin for the device context.
    inline BOOL CDC::GetWindowOrgEx(LPPOINT pPoint) const
    {
        assert(m_pData->hDC);
        return ::GetWindowOrgEx(m_pData->hDC, pPoint);
    }


    // Specifies which window point maps to the viewport origin (0,0).
    inline BOOL CDC::SetWindowOrgEx(int x, int y, LPPOINT pPoint ) const
    {
        assert(m_pData->hDC);
        return ::SetWindowOrgEx(m_pData->hDC, x, y, pPoint);
    }


    // Specifies which window point maps to the viewport origin (0,0).
    inline BOOL CDC::SetWindowOrgEx(POINT point, LPPOINT pPointRet ) const
    {
        assert(m_pData->hDC);
        return SetWindowOrgEx(point.x, point.y, pPointRet);
    }


    // Modifies the window origin for the device context using the specified horizontal and vertical offsets.
    inline BOOL CDC::OffsetWindowOrgEx(int nWidth, int nHeight, LPPOINT pPoint ) const
    {
        assert(m_pData->hDC);
        return ::OffsetWindowOrgEx(m_pData->hDC, nWidth, nHeight, pPoint);
    }


    // Retrieves the x-extent and y-extent of the window for the device context.
    inline BOOL CDC::GetWindowExtEx(LPSIZE pSize)  const
    {
        assert(m_pData->hDC);
        return ::GetWindowExtEx(m_pData->hDC, pSize);
    }


    // Sets the horizontal and vertical extents of the window for the device context by using the specified values.
    inline BOOL CDC::SetWindowExtEx(int x, int y, LPSIZE pSize ) const
    {
        assert(m_pData->hDC);
        return ::SetWindowExtEx(m_pData->hDC, x, y, pSize);
    }


    // Sets the horizontal and vertical extents of the window for the device context by using the specified values.
    inline BOOL CDC::SetWindowExtEx(SIZE size, LPSIZE pSizeRet) const
    {
        assert(m_pData->hDC);
        return SetWindowExtEx(size.cx, size.cy, pSizeRet);
    }


    // Modifies the window for the device context using the ratios formed by the specified multiplicands and divisors.
    inline BOOL CDC::ScaleWindowExtEx(int xNum, int xDenom, int yNum, int yDenom, LPSIZE pSize) const
    {
        assert(m_pData->hDC);
        return ::ScaleWindowExtEx(m_pData->hDC, xNum, xDenom, yNum, yDenom, pSize);
    }


    /////////////////////
    // MetaFile Functions


    // Displays the picture stored in the specified metafile.
    inline BOOL CDC::PlayMetaFile(HMETAFILE hMetaFile) const
    {
        assert(m_pData->hDC);
        return ::PlayMetaFile(m_pData->hDC, hMetaFile);
    }


    // Displays the picture stored in the specified enhanced-format metafile.
    inline BOOL CDC::PlayMetaFile(HENHMETAFILE hEnhMetaFile, const RECT& bounds) const
    {
        assert(m_pData->hDC);
        return ::PlayEnhMetaFile(m_pData->hDC, hEnhMetaFile, &bounds);
    }

#endif // _WIN32_WCE

    ////////////////////
    // Printer Functions


    // Starts a print job.
    inline int CDC::StartDoc(LPDOCINFO pDocInfo) const
    {
        assert(m_pData->hDC);
        return ::StartDoc(m_pData->hDC, pDocInfo);
    }


    // Ends a print job.
    inline int CDC::EndDoc() const
    {
        assert(m_pData->hDC);
        return ::EndDoc(m_pData->hDC);
    }


    // Prepares the printer driver to accept data.
    inline int CDC::StartPage() const
    {
        assert(m_pData->hDC);
        return ::StartPage(m_pData->hDC);
    }


    // Notifies the device that the application has finished writing to a page.
    inline int CDC::EndPage() const
    {
        assert(m_pData->hDC);
        return ::EndPage(m_pData->hDC);
    }


    // Stops the current print job and erases everything drawn since the last call to the StartDoc function.
    inline int CDC::AbortDoc() const
    {
        assert(m_pData->hDC);
        return ::AbortDoc(m_pData->hDC);
    }


    // Sets the application-defined abort function that allows a print job to be cancelled during spooling.
    inline int CDC::SetAbortProc(BOOL (CALLBACK* pfn)(HDC, int)) const
    {
        assert(m_pData->hDC);
        return ::SetAbortProc(m_pData->hDC, pfn);
    }


    /////////////////
    // Text Functions

    // Draws text using the currently selected font, background color, and text color.
    inline BOOL CDC::ExtTextOut(int x, int y, UINT options, const RECT& rc, LPCTSTR pString, int count /*= -1*/, LPINT pDxWidths /*=NULL*/) const
    {
        assert(m_pData->hDC);

        if (count == -1)
            count = lstrlen (pString);

        return ::ExtTextOut(m_pData->hDC, x, y, options, &rc, pString, count, pDxWidths);
    }


    // Draws formatted text in the specified rectangle.
    inline int CDC::DrawText(LPCTSTR pString, int count, const RECT& rc, UINT format) const
    {
        assert(m_pData->hDC);
        return ::DrawText(m_pData->hDC, pString, count, (LPRECT)&rc, format );
    }


    // Retrieves the text-alignment setting.
    // Values: TA_BASELINE, TA_BOTTOM, TA_TOP, TA_CENTER, TA_LEFT, TA_RIGHT, TA_RTLREADING, TA_NOUPDATECP, TA_UPDATECP
    inline UINT CDC::GetTextAlign() const
    {
        assert(m_pData->hDC);
        return ::GetTextAlign(m_pData->hDC);
    }


    // Sets the text-alignment setting.
    // Values: TA_BASELINE, TA_BOTTOM, TA_TOP, TA_CENTER, TA_LEFT, TA_RIGHT, TA_RTLREADING, TA_NOUPDATECP, TA_UPDATECP
    inline UINT CDC::SetTextAlign(UINT flags) const
    {
        assert(m_pData->hDC);
        return ::SetTextAlign(m_pData->hDC, flags);
    }


    // Retrieves the typeface name of the font that is selected into the device context.
    inline int CDC::GetTextFace(int count, LPTSTR pFacename) const
    {
        assert(m_pData->hDC);
        return ::GetTextFace(m_pData->hDC, count, pFacename);
    }


    // Fills the specified buffer with the metrics for the currently selected font.
    inline BOOL CDC::GetTextMetrics(TEXTMETRIC& metrics) const
    {
        assert(m_pData->hDC);
        return ::GetTextMetrics(m_pData->hDC, &metrics);
    }


    // Returns the current background color.
    inline COLORREF CDC::GetBkColor() const
    {
        assert(m_pData->hDC);
        return ::GetBkColor(m_pData->hDC);
    }


    // Sets the current background color to the specified color value.
    inline COLORREF CDC::SetBkColor(COLORREF color) const
    {
        assert(m_pData->hDC);
        return ::SetBkColor(m_pData->hDC, color);
    }


    // Retrieves the current text color.
    inline COLORREF CDC::GetTextColor() const
    {
        assert(m_pData->hDC);
        return ::GetTextColor(m_pData->hDC);
    }


    // Sets the current text color.
    inline COLORREF CDC::SetTextColor(COLORREF color) const
    {
        assert(m_pData->hDC);
        return ::SetTextColor(m_pData->hDC, color);
    }


    // returns the current background mix mode (OPAQUE or TRANSPARENT).
    inline int CDC::GetBkMode() const
    {
        assert(m_pData->hDC);
        return ::GetBkMode(m_pData->hDC);
    }


    // Sets the current background mix mode (OPAQUE or TRANSPARENT).
    inline int CDC::SetBkMode(int bkMode) const
    {
        assert(m_pData->hDC);
        return ::SetBkMode(m_pData->hDC, bkMode);
    }

#ifndef _WIN32_WCE

    // Draws formatted text in the specified rectangle with more formatting options.
    inline int CDC::DrawTextEx(LPTSTR pString, int count, const RECT& rc, UINT format, LPDRAWTEXTPARAMS pDTParams) const
    {
        assert(m_pData->hDC);
        return ::DrawTextEx(m_pData->hDC, pString, count, (LPRECT)&rc, format, pDTParams);
    }


    // Retrieves the widths, in logical units, of consecutive characters in a specified range from the
    // current TrueType font. This function succeeds only with TrueType fonts.
    inline BOOL CDC::GetCharABCWidths(UINT firstChar, UINT lastChar, LPABC pABC) const
    {
        assert(m_pData->hDC);
        return::GetCharABCWidths(m_pData->hDC, firstChar, lastChar, pABC);
    }


    // Retrieves information about a character string, such as character widths, caret positioning,
    // ordering within the string, and glyph rendering.
    inline DWORD CDC::GetCharacterPlacement(LPCTSTR pString, int count, int maxExtent, LPGCP_RESULTS pResults, DWORD flags) const
    {
        assert(m_pData->hDC);
        return ::GetCharacterPlacement(m_pData->hDC, pString, count, maxExtent, pResults, flags);
    }


    // Retrieves the fractional widths of consecutive characters in a specified range from the current font.
    inline BOOL CDC::GetCharWidth(UINT firstChar, UINT lastChar, float* pBuffer) const
    {
        assert(m_pData->hDC);
        return ::GetCharWidthFloat(m_pData->hDC, firstChar, lastChar, pBuffer);
    }


    // Computes the width and height of the specified string of text.
    inline CSize CDC::GetTextExtentPoint32(LPCTSTR pString, int count) const
    {
        assert(m_pData->hDC);
        assert(pString);
        assert(count <= lstrlen(pString));
        CSize sz;
        ::GetTextExtentPoint32(m_pData->hDC, pString, count, &sz);
        return sz;
    }


    // Computes the width and height of the specified string of text.
    inline CSize CDC::GetTextExtentPoint32(LPCTSTR pString) const
    {
        assert(pString);
        return GetTextExtentPoint32(pString, lstrlen(pString));
    }


    // Computes the width and height of a character string.
    inline CSize CDC::GetTabbedTextExtent(LPCTSTR pString, int count, int tabPositions, LPINT pTabStopPositions) const
    {
        assert(m_pData->hDC);
        DWORD dwSize = ::GetTabbedTextExtent(m_pData->hDC, pString, count, tabPositions, pTabStopPositions);
        CSize sz(dwSize);
        return sz;
    }


    // Draws gray text at the specified location.
    inline BOOL CDC::GrayString(HBRUSH hBrush, GRAYSTRINGPROC pOutputFunc, LPARAM pData, int count, int x, int y, int width, int height) const
    {
        assert(m_pData->hDC);
        return ::GrayString(m_pData->hDC, hBrush, pOutputFunc, pData, count, x, y, width, height);
    }


    // Specifies the amount of space the system should add to the break characters in a string of text.
    inline int CDC::SetTextJustification(int breakExtra, int breakCount) const
    {
        assert(m_pData->hDC);
        return ::SetTextJustification(m_pData->hDC, breakExtra, breakCount);
    }


    // Retrieves the current inter-character spacing for the device context.
    inline int CDC::GetTextCharacterExtra() const
    {
        assert(m_pData->hDC);
        return ::GetTextCharacterExtra(m_pData->hDC);
    }


    // Sets the inter-character spacing.
    inline int CDC::SetTextCharacterExtra(int charExtra) const
    {
        assert(m_pData->hDC);
        return ::SetTextCharacterExtra(m_pData->hDC, charExtra);
    }


    // Writes a character string at a specified location, expanding tabs to the values.
    // specified in an array of tab-stop positions
    inline CSize CDC::TabbedTextOut(int x, int y, LPCTSTR pString, int count, int tabPositions, LPINT pTabStopPositions, int tabOrigin) const
    {
        assert(m_pData->hDC);
        DWORD dwSize = ::TabbedTextOut(m_pData->hDC, x, y, pString, count, tabPositions, pTabStopPositions, tabOrigin);
        CSize sz(dwSize);
        return sz;
    }


    // Writes a character string at the specified location.
    inline BOOL CDC::TextOut(int x, int y, LPCTSTR pString, int count/* = -1*/) const
    {
        assert(m_pData->hDC);
        if (count == -1)
            count = lstrlen (pString);

        return ::TextOut(m_pData->hDC, x, y, pString, count);
    }

  #if (_WIN32_WINNT >= 0x0500) && !defined(__GNUC__)

    // Retrieves the widths, in logical units, of consecutive glyph indices in a specified range from the
    // current TrueType font. This function succeeds only with TrueType fonts.
    inline BOOL CDC::GetCharABCWidthsI(UINT giFirst, UINT cgi, LPWORD pGI, LPABC pABC) const
    {
        assert(m_pData->hDC);
        return ::GetCharABCWidthsI(m_pData->hDC, giFirst, cgi, pGI, pABC);
    }


    // Retrieves the widths, in logical coordinates, of consecutive glyph indices in a specified range from the current font.
    inline BOOL CDC::GetCharWidthI(UINT giFirst, UINT cgi, LPWORD pGI, int* pBuffer) const
    {
        assert(m_pData->hDC);
        return ::GetCharWidthI(m_pData->hDC, giFirst, cgi, pGI, pBuffer);
    }

  #endif

#endif


} // namespace Win32xx

#endif // _WIN32XX_GDI_H_

