// Win32++  PreRelease Version 7.1
// Released: N/A
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2010  David Nash
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
// gdi.h
//  Declaration of the CDC class, and CBitmapInfoPtr class

// The CDC class provides a device context, along with the various associated
//  objects such as Bitmaps, Brushes, Bitmaps, Fonts and Pens. This class
//  handles the creation, selection, de-selection and deletion of these objects
//  automatically. It also automatically deletes or releases the device context
//  itself as appropriate. Any failure to create the new GDI object throws an
//  exception.
//
// Coding Exampe without CDC ...
//  HDC hDC = ::CreateCompatibleDC(SomeHDC);
//  HPEN hPen = :: CreatePen(PS_SOLID, 1, RGB(255,0,0);
//  HPEN hPenOld = (HPEN)::SelectObject(hDC, hPen);
//	::MoveToEx(hDC, 0, 0, NULL);
//  ::LineTo(hDC, 50, 50);
//  ::SelectObject(hDC, hPenOld);
//  ::DeleteObject(hPen);
//  ::DeleteDC(hDC);
//
// Coding Example with CDC ...
//  CDC DrawDC = ::CreateCompatibleDC(SomeHDC);
//  DrawDC.CreatePen(PS_SOLID, 1, RGB(255,0,0);
//	DrawDC.MoveTo(0, 0);
//  DrawDC.LineTo(50, 50);
//
// When the CDC object drops out of scope, it's destructor is called, cleaning up
//  the GDI objects and device context.
//
// Notes:
//  * A device context assigned to a CDC object will be released or deleted, unless
//     it is detached.
//  * A GDI object created by one of the CDC member functions, or one attached to the CDC
//     object will be deleted when the CDC object is destroyed, unless it is detached.
//  * A GDI object (with the exception of regions) can only be selected into one
//     device context at a time.
//  * Set the region's shape before selecting it into a DC.

// The CBitmapInfoPtr class is a convienient wrapper for the BITMAPINFO structure.
// The size of the BITMAPINFO structure is dependant on the type of HBITMAP, and its
// space needs to be allocated dynamically. CBitmapInfoPtr automatically allocates
// and deallocates the memory for the structure. A CBitmapInfoPtr object can be
// used anywhere in place of a LPBITMAPINFO. LPBITMAPINFO is used in functions like
// GetDIBits and SetDIBits.
//
// Coding example ...
//  CDC MemDC = CreateCompatibleDC(NULL);
//  CBitmapInfoPtr pbmi(hBitmap);
//  MemDC.GetDIBits(hBitmap, 0, pbmi->bmiHeader.biHeight, NULL, pbmi, DIB_RGB_COLORS);

#ifndef _GDI_H_
#define _GDI_H_

#include "winutils.h"

namespace Win32xx
{

	/////////////////////////////////////////////////////////////////
	// Declarations for some global functions in the Win32xx namespace
	//
#ifndef _WIN32_WCE
	void GrayScaleBitmap( HBITMAP hbmSource );
	void TintBitmap( HBITMAP hbmSource, int cRed, int cGreen, int cBlue );
	HIMAGELIST CreateDisabledImageList( HIMAGELIST himlNormal );
#endif


	///////////////////////////////////////////////
	// Declarations for the CBitmap class
	//
	class CBitmap
	{
	  public:
		CBitmap ();
		CBitmap(HBITMAP hBitmap) : m_hBitmap(hBitmap) {}
		void operator = (HBITMAP hBitmap)
		{
			assert (m_hBitmap == NULL);
			m_hBitmap = hBitmap;
		}
		operator HBITMAP() const;
		~CBitmap();
		void Attach (HBITMAP hBitmap)
		{
			if (m_hBitmap != NULL && m_hBitmap != hBitmap)
				::DeleteObject(m_hBitmap);
			m_hBitmap = hBitmap;
		}

		HBITMAP Detach()
		{
			HBITMAP hBitmap = m_hBitmap;
			m_hBitmap = NULL;
			return hBitmap;
		}

		// Create and load methods
		//HBITMAP LoadBitmap (LPCTSTR lpstr);
		void LoadOEMBitmap (UINT nIDBitmap);
		void CreateBitmap (int nWidth, int nHeight, UINT nPlanes, UINT nBitsPerPixel, const void* lpBits);
		void CreateCompatibleBitmap (HDC hDC, int nWidth, int nHeight);

		// Attributes
		BITMAP GetBitmap () const;
		void CreateDIBSection(HDC hDC, CONST BITMAPINFO* lpbmi, UINT uColorUse, VOID** ppvBits, HANDLE hSection, DWORD dwOffset);

#ifndef _WIN32_WCE
		int GetDIBits(HDC hDC, UINT uStartScan, UINT cScanLines,  LPVOID lpvBits, LPBITMAPINFO lpbmi, UINT uColorUse) const;
		int SetDIBits(HDC hDC, UINT uStartScan, UINT cScanLines, CONST VOID* lpvBits, CONST BITMAPINFO* lpbmi, UINT uColorUse);
		//HBITMAP LoadMappedBitmap (UINT nIDBitmap, UINT nFlags = 0, LPCOLORMAP lpColorMap = NULL, int nMapSize = 0);
		void CreateBitmapIndirect (LPBITMAP lpBitmap);
		void CreateDiscardableBitmap (HDC hDC, int nWidth, int nHeight);
		CSize GetBitmapDimensionEx() const;
		CSize SetBitmapDimensionEx(int nWidth, int nHeight);

		// DIB support
		HBITMAP CreateDIBitmap(HDC hDC, CONST BITMAPINFOHEADER* lpbmih, DWORD dwInit, CONST VOID* lpbInit, CONST BITMAPINFO* lpbmi, UINT uColorUse);
#endif // !_WIN32_WCE

	private:
		CBitmap(const CBitmap&); 
		CBitmap& operator= (const CBitmap&);
		HBITMAP m_hBitmap;									  // Data member
	};


	///////////////////////////////////////////////
	// Declarations for the CBrush class
	//
	class CBrush
	{
	  public:
		CBrush ();
		CBrush (HBRUSH hBrush) : m_hBrush(hBrush) {}
		CBrush (COLORREF crColor);
		void operator = (HBRUSH hBrush)
		{
			assert (m_hBrush == NULL);
			m_hBrush = hBrush;
		}
		operator HBRUSH() const { return m_hBrush; }
		~CBrush();
		void Attach (HBRUSH hBrush)
		{
			if (m_hBrush != NULL && m_hBrush != hBrush)
				::DeleteObject(m_hBrush);
			m_hBrush = hBrush;
		}

		HBRUSH Detach()
		{
			HBRUSH hBrush = m_hBrush;
			m_hBrush = NULL;
			return hBrush;
		}

		void CreateSolidBrush (COLORREF crColor);
		void CreatePatternBrush (HBITMAP hBitmap);
		void CreateDIBPatternBrushPt (LPCVOID lpPackedDIB, UINT nUsage);
		void CreateSysColorBrush (int nIndex);
		LOGBRUSH GetLogBrush () const;

#ifndef _WIN32_WCE
		void CreateHatchBrush (int nIndex, COLORREF crColor);
		void CreateBrushIndirect (LPLOGBRUSH lpLogBrush);
#endif // !defined(_WIN32_WCE)

	  private:
		CBrush(const CBrush&);
		const CBrush& operator= (const CBrush&);
		HBRUSH m_hBrush;
	};


	///////////////////////////////////////////////
	// Declarations for the CFont class
	//
	class CFont
	{
	public:
		CFont ();
		CFont (HFONT hFont) : m_hFont(hFont) {}
		void operator = (HFONT hFont)
		{
			assert (m_hFont == NULL);
			m_hFont = hFont;
		}
		operator HFONT() const { return m_hFont; }
		~CFont();
		void Attach (HFONT hFont)
		{
			if (m_hFont != NULL && m_hFont != hFont)
				::DeleteObject(m_hFont);
			m_hFont = hFont;
		}

		HFONT Detach()
		{
			HFONT hFont = m_hFont;
			m_hFont = NULL;
			return hFont;
		}

		// Create methods
		void CreateFontIndirect (const LOGFONT* lpLogFont);
		LOGFONT GetLogFont () const;


#ifndef _WIN32_WCE
//		void CreateFontIndirectEx (const ENUMLOGFONTEXDV* penumlfex);
		void CreateFont (int nHeight, int nWidth, int nEscapement,
				int nOrientation, int nWeight, DWORD dwItalic, DWORD dwUnderline,
				DWORD dwStrikeOut, DWORD dwCharSet, DWORD dwOutPrecision,
				DWORD dwClipPrecision, DWORD dwQuality, DWORD dwPitchAndFamily,
				LPCTSTR lpszFacename);
#endif // #ifndef _WIN32_WCE

	  private:
		CFont(const CFont&);
		CFont& operator= (const CFont&);
		HFONT m_hFont;
	};


	///////////////////////////////////////////////
	// Declarations for the CPalette class
	//
	class CPalette
	{
	  public:
		CPalette ();
		CPalette (HPALETTE hPalette) : m_hPalette(hPalette) {}
		~CPalette();
		void operator = (HPALETTE hPalette)
		{
			assert (m_hPalette == NULL);
			m_hPalette = hPalette;
		}
		operator HPALETTE() const;
		void Attach (HPALETTE hPalette)
		{
			if (m_hPalette != NULL && m_hPalette != hPalette)
				::DeleteObject(m_hPalette);
			m_hPalette = hPalette;
		}

		HPALETTE Detach()
		{
			HPALETTE hPalette = m_hPalette;
			m_hPalette = NULL;
			return hPalette;
		}

		// Create methods
		void CreatePalette(LPLOGPALETTE lpLogPalette);

#ifndef _WIN32_WCE
		void CreateHalftonePalette(HDC hDC);
#endif // !_WIN32_WCE

		// Attributes
		int GetEntryCount() const;
		UINT GetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) const;
		UINT SetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors);

		// Operations
#ifndef _WIN32_WCE
		BOOL ResizePalette(UINT nNumEntries);
		void AnimatePalette(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors);
#endif // !_WIN32_WCE

		UINT GetNearestPaletteIndex(COLORREF crColor) const;

	  private:
		CPalette(const CPalette&);
		CPalette& operator= (const CPalette&);
		HPALETTE m_hPalette;					// Data member
	};


	///////////////////////////////////////////////
	// Declarations for the CPen class
	//
	class CPen
	{
	public:
		CPen ();
		CPen (HPEN hPen) : m_hPen(hPen) {}
		CPen (int nPenStyle, int nWidth, COLORREF crColor);
		void operator = (HPEN hPen)
		{
			assert (m_hPen == NULL);
			m_hPen = hPen;
		}
		operator HPEN() const { return m_hPen; }
		~CPen ();
		void Attach (HPEN hPen)
		{
			if (m_hPen != NULL && m_hPen != hPen)
				::DeleteObject(m_hPen);
			m_hPen = hPen;
		}

		HPEN Detach()
		{
			HPEN hPen = m_hPen;
			m_hPen = NULL;
			return hPen;
		}

		void CreatePen (int nPenStyle, int nWidth, COLORREF crColor);
		void CreatePenIndirect (LPLOGPEN lpLogPen);
		LOGPEN GetLogPen () const;

#ifndef _WIN32_WCE
		void ExtCreatePen (int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush, int nStyleCount = 0, const DWORD* lpStyle = NULL);
		EXTLOGPEN GetExtLogPen () const;
#endif // !_WIN32_WCE

	  private:
		CPen(const CPen&);
		CPen& operator= (const CPen&);
		HPEN m_hPen;
	};


	///////////////////////////////////////////////
	// Declarations for the CRgn class
	//
	class CRgn
	{
	  public:
		CRgn();
		CRgn(HRGN hRgn) : m_hRgn(hRgn) {}
		~CRgn();
		void operator = (HRGN hRgn)
		{
			assert (m_hRgn == NULL);
			m_hRgn = hRgn;
		}
		operator HRGN() const;
		void Attach (HRGN hRgn)
		{
			if (m_hRgn != NULL && m_hRgn != hRgn)
				::DeleteObject(m_hRgn);
			m_hRgn = hRgn;
		}

		HRGN Detach()
		{
			HRGN hRgn = m_hRgn;
			m_hRgn = NULL;
			return hRgn;
		}

		// Create methods
		void CreateRectRgn (int x1, int y1, int x2, int y2);
		void CreateRectRgnIndirect (LPCRECT lpRect);

#ifndef _WIN32_WCE
		void CreateEllipticRgn (int x1, int y1, int x2, int y2);
		void CreateEllipticRgnIndirect (LPCRECT lpRect);
		void CreatePolygonRgn (LPPOINT lpPoints, int nCount, int nMode);
		void CreatePolyPolygonRgn (LPPOINT lpPoints, LPINT lpPolyCounts, int nCount, int nPolyFillMode);
		void CreateRoundRectRgn (int x1, int y1, int x2, int y2, int x3, int y3);
		void CreateFromPath (HDC hDC);
		void CreateFromData (const XFORM* lpXForm, int nCount, const RGNDATA* pRgnData);
#endif // !_WIN32_WCE

		// Operations
		void SetRectRgn (int x1, int y1, int x2, int y2);
		void SetRectRgn (LPCRECT lpRect);
		int CombineRgn (HRGN hRgnSrc1, HRGN hRgnSrc2, int nCombineMode);
		int CombineRgn (HRGN hRgnSrc, int nCombineMode);
		int CopyRgn (HRGN hRgnSrc);
		BOOL EqualRgn (HRGN hRgn) const;
		int OffsetRgn (int x, int y);
		int OffsetRgn (POINT point);
		int GetRgnBox (LPRECT lpRect) const;
		BOOL PtInRegion (int x, int y) const;
		BOOL PtInRegion (POINT point) const;
		BOOL RectInRegion (LPCRECT lpRect) const;
		int GetRegionData (LPRGNDATA lpRgnData, int nDataSize) const;

	  private:
		CRgn(const CRgn&);
		CRgn& operator = (const CRgn&);
		HRGN m_hRgn;							// Data member
	};


	///////////////////////////////////////////////
	// Declarations for the CDC class
	//
	class CDC
	{
	public:
		CDC( );									// Constructs a new CDC without assigning a HDC
		CDC( HDC hDC);							// Assigns a HDC to a new CDC
		CDC(const CDC& rhs);					// Constructs a new copy of the CDC
		void operator = ( const HDC hDC );		// Assigns a HDC to an existing CDC
		CDC& operator = ( const CDC& rhs );     // Assigns a CDC to an existing CDC
		operator HDC( ) const { return *m_DC; }	// Converts a CDC to a HDC
		virtual ~CDC( );
		void AttachDC( HDC hDC );
		HDC  DetachDC( );
		HDC GetHDC( ) const { return *m_DC; }
		void Release();

		// Create and Select Bitmaps
		void AttachBitmap( HBITMAP hBitmap );
		void CreateBitmap( int cx, int cy, UINT Planes, UINT BitsPerPixel, CONST VOID *pvColors );
		void CreateCompatibleBitmap( HDC hDC, int cx, int cy );
		void CreateDIBSection( HDC hdc, const BITMAPINFO& bmi, UINT iUsage, VOID **ppvBits,
										HANDLE hSection, DWORD dwOffset) ;
		HBITMAP DetachBitmap( );
		BITMAP GetBitmapInfo( );

#ifndef _WIN32_WCE
		void CreateBitmapIndirect( LPBITMAP pBitmap );
		void CreateDIBitmap( HDC hdc, const BITMAPINFOHEADER& bmih, DWORD fdwInit, CONST VOID *lpbInit,
										BITMAPINFO& bmi, UINT fuUsage );
#endif

		// Create and Select Brushes
		void AttachBrush( HBRUSH hBrush );
		void CreateDIBPatternBrushPt( LPCVOID lpPackedDIB, UINT iUsage );
		void CreatePatternBrush( HBITMAP hbmp );
		void CreateSolidBrush( COLORREF rbg );
		HBRUSH DetachBrush( );
		LOGBRUSH GetBrushInfo( );

#ifndef _WIN32_WCE
		void CreateBrushIndirect(  LPLOGBRUSH& pLogBrush );
		void CreateHatchBrush( int fnStyle, COLORREF rgb );
#endif

		// Create and Select Fonts
		void AttachFont( HFONT hFont );
		void CreateFontIndirect( const LOGFONT& lf );
		HFONT DetachFont( );
		LOGFONT GetFontInfo( );

#ifndef _WIN32_WCE
		void CreateFont( int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight,
  							DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet,
  							DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality,
  							DWORD fdwPitchAndFamily, LPCTSTR lpszFace );
#endif

		// Create and Select Pens
		void AttachPen( HPEN hPen );
		void CreatePen( int nStyle, int nWidth, COLORREF rgb );
		void CreatePenIndirect( LPLOGPEN pLogPen );
		HPEN DetachPen( );
		LOGPEN GetPenInfo( );

		// Create Select Regions
		void AttachClipRgn( HRGN hRegion );
		void CreateRectRgn( int left, int top, int right, int bottom );
		void CreateRectRgnIndirect( const RECT& rc );
		HRGN DetachClipRgn( );
		void CreateFromData( const XFORM* Xform, DWORD nCount, const RGNDATA *pRgnData );
#ifndef _WIN32_WCE
		void CreateEllipticRgn( int left, int top, int right, int bottom );
		void CreateEllipticRgnIndirect( const RECT& rc );
		void CreatePolygonRgn( LPPOINT ppt, int cPoints, int fnPolyFillMode );
		void CreatePolyPolygonRgn( LPPOINT ppt, LPINT pPolyCounts, int nCount, int fnPolyFillMode );
#endif

		// Wrappers for WinAPI functions

		// Initialization
		HDC CreateCompatibleDC( ) const;
		HDC CreateDC( LPCTSTR lpszDriver, LPCTSTR lpszDevice, LPCTSTR lpszOutput, const DEVMODE& dvmInit ) const;
		int GetDeviceCaps( int nIndex ) const;
#ifndef _WIN32_WCE
		HDC CreateIC( LPCTSTR lpszDriver, LPCTSTR lpszDevice, LPCTSTR lpszOutput, const DEVMODE& dvmInit ) const;
#endif

		// Point and Line Drawing Functions
		CPoint GetCurrentPosition( ) const;
		CPoint MoveTo( int x, int y ) const;
		CPoint MoveTo( POINT pt ) const;
		BOOL LineTo( int x, int y ) const;
		BOOL LineTo( POINT pt ) const;
		COLORREF GetPixel( int x, int y ) const;
		COLORREF GetPixel( POINT pt ) const;
		COLORREF SetPixel( int x, int y, COLORREF crColor ) const;
		COLORREF SetPixel( POINT pt, COLORREF crColor ) const;
#ifndef _WIN32_WCE
		BOOL Arc( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 ) const;
		BOOL Arc( RECT& rc, POINT ptStart, POINT ptEnd ) const;
		BOOL ArcTo( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 ) const;
		BOOL ArcTo( RECT& rc, POINT ptStart, POINT ptEnd ) const;
		BOOL AngleArc( int x, int y, int nRadius, float fStartAngle, float fSweepAngle ) const;
		int GetArcDirection( ) const;
		int SetArcDirection( int nArcDirection ) const;
		BOOL PolyDraw( const POINT* lpPoints, const BYTE* lpTypes, int nCount ) const;
		BOOL Polyline( LPPOINT lpPoints, int nCount ) const;
		BOOL PolyPolyline( const POINT* lpPoints, const DWORD* lpPolyPoints, int nCount ) const;
		BOOL PolylineTo( const POINT* lpPoints, int nCount ) const;
		BOOL PolyBezier( const POINT* lpPoints, int nCount ) const;
		BOOL PolyBezierTo( const POINT* lpPoints, int nCount ) const;
		BOOL SetPixelV( int x, int y, COLORREF crColor ) const;
		BOOL SetPixelV( POINT pt, COLORREF crColor ) const;
#endif

		// Shape Drawing Functions
		void DrawFocusRect( const RECT& rc ) const;
		BOOL Ellipse( int x1, int y1, int x2, int y2 ) const;
		BOOL Ellipse( const RECT& rc ) const;
		BOOL Polygon( LPPOINT lpPoints, int nCount ) const;
		BOOL Rectangle( int x1, int y1, int x2, int y2 ) const;
		BOOL Rectangle( const RECT& rc) const;
		BOOL RoundRect( int x1, int y1, int x2, int y2, int nWidth, int nHeight ) const;
		BOOL RoundRect( const RECT& rc, int nWidth, int nHeight ) const;
#ifndef _WIN32_WCE
		BOOL Chord( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 ) const;
		BOOL Chord( const RECT& rc, POINT ptStart, POINT ptEnd ) const;
		BOOL Pie( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 ) const;
		BOOL Pie( const RECT& rc, POINT ptStart, POINT ptEnd ) const;
		BOOL PolyPolygon( LPPOINT lpPoints, LPINT lpPolyCounts, int nCount ) const;
#endif

		// Fill and Image Drawing functions
		BOOL FillRect( const RECT& rc, HBRUSH hbr ) const;
		BOOL InvertRect( const RECT& rc ) const;
		BOOL DrawIconEx( int xLeft, int yTop, HICON hIcon, int cxWidth, int cyWidth, UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw, UINT diFlags ) const;
		BOOL DrawEdge( const RECT& rc, UINT nEdge, UINT nFlags ) const;
		BOOL DrawFrameControl( const RECT& rc, UINT nType, UINT nState ) const;
		BOOL FillRgn( HRGN hrgn, HBRUSH hbr ) const;
		virtual void GradientFill( COLORREF Color1, COLORREF Color2, const RECT& rc, BOOL bVertical );
		virtual void SolidFill( COLORREF Color, const RECT& rc );
#ifndef _WIN32_WCE
		BOOL DrawIcon( int x, int y, HICON hIcon ) const;
		BOOL DrawIcon( POINT point, HICON hIcon ) const;
		BOOL FrameRect( const RECT& rc, HBRUSH hbr ) const;
		BOOL PaintRgn( HRGN hrgn ) const;
#endif

		// Bitmap Functions
		virtual void DrawBitmap( int x, int y, int cx, int cy, HBITMAP hbmImage, COLORREF clrMask );
		int StretchDIBits( int XDest, int YDest, int nDestWidth, int nDestHeight, int XSrc, int YSrc, int nSrcWidth,
			           int nSrcHeight, CONST VOID *lpBits, BITMAPINFO& bi, UINT iUsage, DWORD dwRop ) const;
		BOOL PatBlt( int x, int y, int nWidth, int nHeight, DWORD dwRop ) const;
		BOOL BitBlt( int x, int y, int nWidth, int nHeight, HDC hSrcDC, int xSrc, int ySrc, DWORD dwRop ) const;
		BOOL StretchBlt( int x, int y, int nWidth, int nHeight, HDC hSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop ) const;
#ifndef _WIN32_WCE
		int GetDIBits( HBITMAP hbmp, UINT uStartScan, UINT cScanLines, LPVOID lpvBits, LPBITMAPINFO lpbi, UINT uUsage ) const;
		int SetDIBits( HBITMAP hbmp, UINT uStartScan, UINT cScanLines, CONST VOID *lpvBits, LPBITMAPINFO lpbi, UINT fuColorUse ) const;
		int GetStretchBltMode( ) const;
		int SetStretchBltMode( int iStretchMode ) const;
		BOOL FloodFill( int x, int y, COLORREF crColor ) const;
		BOOL ExtFloodFill( int x, int y, COLORREF crColor, UINT nFillType ) const;
#endif

        // Co-ordinate Functions
#ifndef _WIN32_WCE
		BOOL DPtoLP(LPPOINT lpPoints, int nCount )  const;
		BOOL DPtoLP(LPRECT lpRect)  const;
		BOOL LPtoDP(LPPOINT lpPoints, int nCount )  const;
		BOOL LPtoDP(LPRECT lpRect)  const;
#endif

		// Layout Functions
		DWORD GetLayout() const;
		DWORD SetLayout(DWORD dwLayout) const;

		// Mapping functions
#ifndef _WIN32_WCE
		int GetMapMode()  const;
		int SetMapMode(int nMapMode) const;
		BOOL GetViewportOrgEx(LPPOINT lpPoint)  const;
		BOOL SetViewportOrgEx(int x, int y, LPPOINT lpPoint = NULL) const;
		BOOL SetViewportOrgEx(POINT point, LPPOINT lpPointRet = NULL) const;
		BOOL OffsetViewportOrgEx(int nWidth, int nHeight, LPPOINT lpPoint = NULL ) const;
		BOOL GetViewportExtEx(LPSIZE lpSize)  const;
		BOOL SetViewportExtEx(int x, int y, LPSIZE lpSize ) const;
		BOOL SetViewportExtEx(SIZE size, LPSIZE lpSizeRet ) const;
		BOOL ScaleViewportExtEx(int xNum, int xDenom, int yNum, int yDenom, LPSIZE lpSize ) const;
		BOOL OffsetWindowOrg(int nWidth, int nHeight, LPPOINT lpPoint ) const;
		BOOL GetWindowExtEx(LPSIZE lpSize)  const;
		BOOL SetWindowExtEx(int x, int y, LPSIZE lpSize ) const;
		BOOL SetWindowExtEx(SIZE size, LPSIZE lpSizeRet) const;
		BOOL ScaleWindowExtEx(int xNum, int xDenom, int yNum, int yDenom, LPSIZE lpSize) const;
		BOOL GetWindowOrgEx(LPPOINT lpPoint)  const;
		BOOL SetWindowOrgEx(int x, int y, LPPOINT lpPoint ) const;
		BOOL SetWindowOrgEx(POINT point, LPPOINT lpPointRet ) const;
		BOOL OffsetWindowOrgEx(int nWidth, int nHeight, LPPOINT lpPoint ) const;
#endif

		// Printer Functions
		int StartDoc(LPDOCINFO lpDocInfo) const;
		int EndDoc() const;
		int StartPage() const;
		int EndPage() const;
		int AbortDoc() const;
		int SetAbortProc(BOOL (CALLBACK* lpfn)(HDC, int)) const;

		// Text Functions
		BOOL ExtTextOut( int x, int y, UINT nOptions, const RECT& rc, LPCTSTR lpszString, UINT nCount, LPINT lpDxWidths ) const;
		int DrawText( LPCTSTR lpszString, int nCount, const RECT& rc, UINT nFormat ) const;
		UINT GetTextAlign( ) const;
		UINT SetTextAlign( UINT nFlags ) const;
		int GetTextFace( int nCount, LPTSTR lpszFacename ) const;
		BOOL GetTextMetrics( TEXTMETRIC& Metrics ) const;
		COLORREF GetBkColor( ) const;
		COLORREF SetBkColor( COLORREF crColor ) const;
		COLORREF GetTextColor( ) const;
		COLORREF SetTextColor( COLORREF crColor ) const;
		int GetBkMode( ) const;
		int SetBkMode( int iBkMode ) const;
#ifndef _WIN32_WCE
		CSize GetTextExtentPoint( LPCTSTR lpszString, int nCount ) const;
		BOOL TextOut( int x, int y, LPCTSTR lpszString, int nCount ) const;
		CSize TabbedTextOut( int x, int y, LPCTSTR lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin ) const;
		int DrawTextEx( LPTSTR lpszString, int nCount, const RECT& rc, UINT nFormat, const DRAWTEXTPARAMS& DTParams ) const;
		CSize GetTabbedTextExtent( LPCTSTR lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions ) const;
		BOOL GrayString( HBRUSH hBrush, GRAYSTRINGPROC lpOutputFunc, LPARAM lpData, int nCount, int x, int y, int nWidth, int nHeight ) const;
		int SetTextJustification( int nBreakExtra, int nBreakCount ) const;
		int GetTextCharacterExtra( ) const;
		int SetTextCharacterExtra( int nCharExtra ) const;
		CSize GetTextExtentPoint32( LPCTSTR lpszString, int nCount ) const;
#endif

	private:
		CBitmap m_Bitmap;
		CBrush m_Brush;
		CFont m_Font;
		CPen m_Pen;
		CRgn m_Rgn;
		void RemoveCurrentBitmap() 
		{
			if (*m_PenOld)
			{
				HBITMAP hBitmap = (HBITMAP)::SelectObject(*m_DC, *m_BitmapOld);
				if (hBitmap == m_Bitmap)
				{
					// Only delete the bitmap objects we create
					m_Bitmap.Detach();
					DeleteObject(hBitmap);
				}
			}
		}
		void RemoveCurrentBrush() 
		{
			if (*m_BrushOld)
			{
				HBRUSH hBrush = (HBRUSH)::SelectObject(*m_DC, *m_BrushOld);
				if (hBrush == m_Brush)
				{
					// Only delete the brush objects we create
					m_Brush.Detach();
					DeleteObject(hBrush);
				}
			}
		}
		void RemoveCurrentFont() 
		{
			if (*m_FontOld)
			{
				HFONT hFont = (HFONT)::SelectObject(*m_DC, *m_FontOld);
				if (hFont == m_Font)
				{
					// Only delete the font objects we create
					m_Font.Detach();
					DeleteObject(hFont);
				}
			}
		}
		void RemoveCurrentPen() 
		{
			if (*m_PenOld)
			{
				HPEN hPen = (HPEN)::SelectObject(*m_DC, *m_PenOld);
				if (hPen == m_Pen)
				{
					// Only delete the pen objects we create
					m_Pen.Detach();
					DeleteObject(hPen);
				}
			}
		}
		void RemoveCurrentRgn() 
		{
			if (*m_RgnOld)
			{
				HRGN hRgn = (HRGN)::SelectObject(*m_DC, *m_RgnOld);
				if (hRgn == m_Rgn)
				{
					// Only delete the region objects we create
					m_Rgn.Detach();
					DeleteObject(hRgn);
				}
			}
		}

		HDC* m_DC;
		HBITMAP* m_BitmapOld;
		HBRUSH* m_BrushOld;
		HFONT* m_FontOld;
		HPEN* m_PenOld;
		HRGN* m_RgnOld;
		long* m_Count;
	};


	///////////////////////////////////////////////
	// Declarations for the CBitmapInfoPtr class
	// The CBitmapInfoPtr class is a convienient wrapper for the BITMAPINFO structure.
	class CBitmapInfoPtr
	{
	public:
		CBitmapInfoPtr(HBITMAP hbm)
		{
			BITMAP bmSource;
			::GetObject(hbm, sizeof(BITMAP), &bmSource);

			// Convert the color format to a count of bits.
			WORD cClrBits = (WORD)(bmSource.bmPlanes * bmSource.bmBitsPixel);
			if (cClrBits == 1) 	     cClrBits = 1;
			else if (cClrBits <= 4)  cClrBits = 4;
			else if (cClrBits <= 8)  cClrBits = 8;
			else if (cClrBits <= 16) cClrBits = 16;
			else if (cClrBits <= 24) cClrBits = 24;
			else                     cClrBits = 32;

			// Allocate memory for the BITMAPINFO structure.
			UINT uQuadSize = (cClrBits == 24)? 0 : sizeof(RGBQUAD) * (int)(1 << cClrBits);
			m_bmi.assign(sizeof(BITMAPINFOHEADER) + uQuadSize, 0);
			m_pbmiArray = (LPBITMAPINFO) &m_bmi.front();

			m_pbmiArray->bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
			m_pbmiArray->bmiHeader.biHeight		= bmSource.bmHeight;
			m_pbmiArray->bmiHeader.biWidth		= bmSource.bmWidth;
			m_pbmiArray->bmiHeader.biPlanes		= bmSource.bmPlanes;
			m_pbmiArray->bmiHeader.biBitCount	= bmSource.bmBitsPixel;
			m_pbmiArray->bmiHeader.biCompression = BI_RGB;
			if (cClrBits < 24)
				m_pbmiArray->bmiHeader.biClrUsed = (1<<cClrBits);
		}
		LPBITMAPINFO get() const { return m_pbmiArray; }
		operator LPBITMAPINFO() const { return m_pbmiArray; }
		LPBITMAPINFO operator->() const { return m_pbmiArray; }

	private:
		CBitmapInfoPtr(const CBitmapInfoPtr&);				// Disable copy construction
		CBitmapInfoPtr& operator = (const CBitmapInfoPtr&);	// Disable assignment operator
		LPBITMAPINFO m_pbmiArray;
		std::vector<byte> m_bmi;
	};

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{
	///////////////////////////////////////////////
	// Declarations for the CBitmap class
	//
	inline CBitmap::CBitmap () : m_hBitmap(NULL)
	{ }
	inline CBitmap::operator HBITMAP() const { return m_hBitmap; }
	inline CBitmap::~CBitmap()
	{
		BOOL bSucceeded = TRUE;
		if (m_hBitmap != NULL)
			bSucceeded = ::DeleteObject(m_hBitmap);

		assert(bSucceeded);
	}

	// Create and load methods
	//inline HBITMAP LoadBitmap (LPCTSTR lpstr)
	//{
	//	assert(m_hBitmap == NULL);
	//	m_hBitmap = ::LoadBitmap (GetApp()->GetResourceHandle(), lpstr);
	//	return m_hBitmap;
	//}

	inline void CBitmap::LoadOEMBitmap (UINT nIDBitmap) // for OBM_/OCR_/OIC_
	{
		assert(m_hBitmap == NULL);
		m_hBitmap = ::LoadBitmap (NULL, MAKEINTRESOURCE(nIDBitmap));
		assert (m_hBitmap);
	}

#ifndef _WIN32_WCE
		//inline HBITMAP CBitmap::LoadMappedBitmap (UINT nIDBitmap, UINT nFlags = 0, LPCOLORMAP lpColorMap = NULL, int nMapSize = 0)
		//{
		//	assert(m_hBitmap == NULL);
		//	m_hBitmap = ::CreateMappedBitmap (GetApp()->GetResourceHandle(), nIDBitmap, (WORD)nFlags, lpColorMap, nMapSize);
		//	return m_hBitmap;
		//}
#endif // !_WIN32_WCE

		inline void CBitmap::CreateBitmap (int nWidth, int nHeight, UINT nPlanes, UINT nBitsPerPixel, const void* lpBits)
		{
			assert(m_hBitmap == NULL);
			m_hBitmap = ::CreateBitmap (nWidth, nHeight, nPlanes, nBitsPerPixel, lpBits);
			assert(m_hBitmap);
		}

#ifndef _WIN32_WCE
		inline void CBitmap::CreateBitmapIndirect (LPBITMAP lpBitmap)
		{
			assert(m_hBitmap == NULL);
			m_hBitmap = ::CreateBitmapIndirect(lpBitmap);
			assert(m_hBitmap);
		}
#endif // !_WIN32_WCE

		inline void CBitmap::CreateCompatibleBitmap (HDC hDC, int nWidth, int nHeight)
		{
			assert(m_hBitmap == NULL);
			m_hBitmap = ::CreateCompatibleBitmap(hDC, nWidth, nHeight);
			assert(m_hBitmap);
		}

#ifndef _WIN32_WCE
		inline void CBitmap::CreateDiscardableBitmap (HDC hDC, int nWidth, int nHeight)
		{
			assert(m_hBitmap == NULL);
			m_hBitmap = ::CreateDiscardableBitmap(hDC, nWidth, nHeight);
			assert(m_hBitmap);
		}
#endif // !_WIN32_WCE

		// Attributes
		inline BITMAP CBitmap::GetBitmap () const
		{
			assert(m_hBitmap != NULL);
			BITMAP bmp = {0};
			::GetObject(m_hBitmap, sizeof(BITMAP), &bmp);
			return bmp;
		}

#ifndef _WIN32_WCE
		inline CSize CBitmap::GetBitmapDimensionEx() const
		{
			assert(m_hBitmap != NULL);
			CSize Size;
			::GetBitmapDimensionEx(m_hBitmap, &Size);
			return Size;
		}

		inline CSize CBitmap::SetBitmapDimensionEx(int nWidth, int nHeight)
		{
			assert(m_hBitmap != NULL);
			CSize Size;
			::SetBitmapDimensionEx(m_hBitmap, nWidth, nHeight, Size);
			return Size;
		}

		// DIB support
		inline HBITMAP CBitmap::CreateDIBitmap(HDC hDC, CONST BITMAPINFOHEADER* lpbmih, DWORD dwInit, CONST VOID* lpbInit, CONST BITMAPINFO* lpbmi, UINT uColorUse)
		{
			assert(m_hBitmap == NULL);
			m_hBitmap = ::CreateDIBitmap(hDC, lpbmih, dwInit, lpbInit, lpbmi, uColorUse);
			assert(m_hBitmap);
			return m_hBitmap;
		}
#endif // !_WIN32_WCE

		inline void CBitmap::CreateDIBSection(HDC hDC, CONST BITMAPINFO* lpbmi, UINT uColorUse, VOID** ppvBits, HANDLE hSection, DWORD dwOffset)
		{
			assert(m_hBitmap == NULL);
			m_hBitmap = ::CreateDIBSection(hDC, lpbmi, uColorUse, ppvBits, hSection, dwOffset);
			assert(m_hBitmap);
		}

#ifndef _WIN32_WCE
		inline int CBitmap::GetDIBits(HDC hDC, UINT uStartScan, UINT cScanLines,  LPVOID lpvBits, LPBITMAPINFO lpbmi, UINT uColorUse) const
		{
			assert(m_hBitmap != NULL);
			return ::GetDIBits(hDC, m_hBitmap, uStartScan, cScanLines,  lpvBits, lpbmi, uColorUse);
		}

		inline int CBitmap::SetDIBits(HDC hDC, UINT uStartScan, UINT cScanLines, CONST VOID* lpvBits, CONST BITMAPINFO* lpbmi, UINT uColorUse)
		{
			assert(m_hBitmap != NULL);
			return ::SetDIBits(hDC, m_hBitmap, uStartScan, cScanLines, lpvBits, lpbmi, uColorUse);
		}
#endif // !_WIN32_WCE


	///////////////////////////////////////////////
	// Definitions of the CBrush class
	//
	inline CBrush::CBrush () : m_hBrush(NULL)
	{ }

	inline CBrush::CBrush (COLORREF crColor)
	{
		m_hBrush = ::CreateSolidBrush (crColor);
		assert (m_hBrush);
	}

	inline CBrush::~CBrush()
	{
		BOOL bSucceeded = TRUE;
		if (m_hBrush != NULL)
			bSucceeded = ::DeleteObject(m_hBrush);

		assert(bSucceeded);
	}

	inline void CBrush::CreateSolidBrush (COLORREF crColor)
	{
		assert(m_hBrush == NULL);
		m_hBrush = ::CreateSolidBrush (crColor);
		assert (m_hBrush);
	}

#ifndef _WIN32_WCE
	inline void CBrush::CreateHatchBrush (int nIndex, COLORREF crColor)
	{
		assert(m_hBrush == NULL);
		m_hBrush = ::CreateHatchBrush (nIndex, crColor);
		assert (m_hBrush);
	}

	inline void CBrush::CreateBrushIndirect (LPLOGBRUSH lpLogBrush)
	{
		assert(m_hBrush == NULL);
		m_hBrush = ::CreateBrushIndirect (lpLogBrush);
		assert (m_hBrush);
	}
#endif // !defined(_WIN32_WCE)

	inline void CBrush::CreatePatternBrush (HBITMAP hBitmap)
	{
		assert(m_hBrush == NULL);
		m_hBrush = ::CreatePatternBrush (hBitmap);
		assert (m_hBrush);
	}

	inline void CBrush::CreateDIBPatternBrushPt (LPCVOID lpPackedDIB, UINT nUsage)
	{
		assert(m_hBrush == NULL);
		m_hBrush = ::CreateDIBPatternBrushPt (lpPackedDIB, nUsage);
		assert (m_hBrush);
	}

	inline void CBrush::CreateSysColorBrush (int nIndex)
	{
		assert(m_hBrush == NULL);
		m_hBrush = ::GetSysColorBrush (nIndex);
		assert (m_hBrush);
	}

	inline LOGBRUSH CBrush::GetLogBrush () const
	{
		assert(m_hBrush != NULL);
		LOGBRUSH LogBrush = {0};
		::GetObject (m_hBrush, sizeof(LOGBRUSH), &LogBrush);
		return LogBrush;
	}


	///////////////////////////////////////////////
	// Definitions of the CFont class
	//
	inline CFont::CFont () : m_hFont(NULL)
	{ }

	inline CFont::~CFont()
	{
		BOOL bSucceeded = TRUE;
		if (m_hFont != NULL)
			bSucceeded = ::DeleteObject(m_hFont);

		assert(bSucceeded);
	}

	// Create methods
	inline void CFont::CreateFontIndirect (const LOGFONT* lpLogFont)
	{
		assert(m_hFont == NULL);
		m_hFont = ::CreateFontIndirect(lpLogFont);
	}


#ifndef _WIN32_WCE
//	inline void CFont::CreateFontIndirectEx (const ENUMLOGFONTEXDV* penumlfex)
//	{
//		assert(m_hFont == NULL);
//		m_hFont = ::CreateFontIndirectEx (penumlfex);
//	}

	inline void CFont::CreateFont (int nHeight, int nWidth, int nEscapement,
			int nOrientation, int nWeight, DWORD dwItalic, DWORD dwUnderline,
			DWORD dwStrikeOut, DWORD dwCharSet, DWORD dwOutPrecision,
			DWORD dwClipPrecision, DWORD dwQuality, DWORD dwPitchAndFamily,
			LPCTSTR lpszFacename)
	{
		assert(m_hFont == NULL);
		m_hFont = ::CreateFont (nHeight, nWidth, nEscapement,
			nOrientation, nWeight, dwItalic, dwUnderline, dwStrikeOut,
			dwCharSet, dwOutPrecision, dwClipPrecision, dwQuality,
			dwPitchAndFamily, lpszFacename);
	}
#endif // #ifndef _WIN32_WCE

	inline LOGFONT CFont::GetLogFont () const
	{
		assert(m_hFont != NULL);
		LOGFONT LogFont = {0};
		::GetObject (m_hFont, sizeof(LOGFONT), &LogFont);
		return LogFont;
	}


	///////////////////////////////////////////////
	// Definitions of the CPalette class
	//
	inline CPalette::CPalette () : m_hPalette(NULL)
	{ }

	inline CPalette::~CPalette()
	{
		BOOL bSucceeded = TRUE;
		if (m_hPalette != NULL)
			bSucceeded = ::DeleteObject(m_hPalette);

		assert(bSucceeded);
	}

	inline CPalette::operator HPALETTE() const {return m_hPalette;}


	inline void CPalette::CreatePalette(LPLOGPALETTE lpLogPalette)
	{
		assert(m_hPalette == NULL);
		m_hPalette = ::CreatePalette (lpLogPalette);
	}

#ifndef _WIN32_WCE
	inline void CPalette::CreateHalftonePalette(HDC hDC)
	{
		assert(m_hPalette == NULL);
		assert(hDC != NULL);
		m_hPalette = ::CreateHalftonePalette(hDC);
	}
#endif // !_WIN32_WCE

	inline int CPalette::GetEntryCount() const
	{
		assert(m_hPalette != NULL);
		WORD nEntries = 0;
		::GetObject (m_hPalette, sizeof(WORD), &nEntries);
		return (int)nEntries;
	}

	inline UINT CPalette::GetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) const
	{
		assert(m_hPalette != NULL);
		return ::GetPaletteEntries (m_hPalette, nStartIndex, nNumEntries, lpPaletteColors);
	}

	inline UINT CPalette::SetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors)
	{
		assert(m_hPalette != NULL);
		return ::SetPaletteEntries (m_hPalette, nStartIndex, nNumEntries, lpPaletteColors);
	}

#ifndef _WIN32_WCE
	inline void CPalette::AnimatePalette(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors)
	{
		assert(m_hPalette != NULL);
		::AnimatePalette (m_hPalette, nStartIndex, nNumEntries, lpPaletteColors);
	}

	inline BOOL CPalette::ResizePalette(UINT nNumEntries)
	{
		assert(m_hPalette != NULL);
		return ::ResizePalette (m_hPalette, nNumEntries);
	}
#endif // !_WIN32_WCE

	inline UINT CPalette::GetNearestPaletteIndex(COLORREF crColor) const
	{
		assert(m_hPalette != NULL);
		return ::GetNearestPaletteIndex (m_hPalette, crColor);
	}


	///////////////////////////////////////////////
	// Declarations for the CPen class
	//
	inline CPen::CPen() : m_hPen(NULL) {}
	inline CPen::CPen(int nPenStyle, int nWidth, COLORREF crColor)
	{
		m_hPen = ::CreatePen (nPenStyle, nWidth, crColor);
	}

	inline CPen::~CPen ()
	{
		BOOL bSucceeded = TRUE;
		if (m_hPen != NULL)
			bSucceeded = ::DeleteObject(m_hPen);

		assert(bSucceeded);
	}

	inline void CPen::CreatePen (int nPenStyle, int nWidth, COLORREF crColor)
	{
		assert(m_hPen == NULL);
		m_hPen = ::CreatePen(nPenStyle, nWidth, crColor);
		assert(m_hPen);
	}

	inline void CPen::CreatePenIndirect (LPLOGPEN lpLogPen)
	{
		assert(m_hPen == NULL);
		m_hPen = ::CreatePenIndirect(lpLogPen);
		assert(m_hPen);
	}

	inline LOGPEN CPen::GetLogPen () const
	{
		assert(m_hPen != NULL);

		LOGPEN LogPen = {0};
		::GetObject(m_hPen, sizeof(LOGPEN), &LogPen);
		return LogPen;
	}

#ifndef _WIN32_WCE
	inline void CPen::ExtCreatePen (int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush, int nStyleCount /* = 0*/, const DWORD* lpStyle /*= NULL*/)
	{
		assert(m_hPen == NULL);
		m_hPen = ::ExtCreatePen(nPenStyle, nWidth, pLogBrush, nStyleCount, lpStyle);
		assert(m_hPen);
	}
	inline EXTLOGPEN CPen::GetExtLogPen () const
	{
		assert(m_hPen != NULL);

		EXTLOGPEN ExLogPen = {0};
		::GetObject(m_hPen, sizeof(EXTLOGPEN), &ExLogPen);
		return ExLogPen;
	}
#endif // !_WIN32_WCE


	///////////////////////////////////////////////
	// Definitions of the CRgn class
	//
	inline CRgn::CRgn () : m_hRgn(NULL)	// Constructor
	{ }

	inline CRgn::~CRgn()									// Destructor
	{
		BOOL bSucceeded = TRUE;
		if (m_hRgn != NULL)
			bSucceeded = ::DeleteObject(m_hRgn);

		assert(bSucceeded);
	}

	inline CRgn::operator HRGN() const {return m_hRgn;}

	inline void CRgn::CreateRectRgn (int x1, int y1, int x2, int y2)
	{
		assert(m_hRgn == NULL);
		m_hRgn = ::CreateRectRgn(x1, y1, x2, y2);
		assert (m_hRgn);
	}

	inline void CRgn::CreateRectRgnIndirect (LPCRECT lpRect)
	{
		assert(m_hRgn == NULL);
		m_hRgn = ::CreateRectRgnIndirect(lpRect);
		assert (m_hRgn);
	}

#ifndef _WIN32_WCE
	inline void CRgn::CreateEllipticRgn (int x1, int y1, int x2, int y2)
	{
		assert(m_hRgn == NULL);
		m_hRgn = ::CreateEllipticRgn (x1, y1, x2, y2);
		assert (m_hRgn);
	}

	inline void CRgn::CreateEllipticRgnIndirect (LPCRECT lpRect)
	{
		assert(m_hRgn == NULL);
		m_hRgn = ::CreateEllipticRgnIndirect (lpRect);
		assert (m_hRgn);
	}

	inline void CRgn::CreatePolygonRgn (LPPOINT lpPoints, int nCount, int nMode)
	{
		assert(m_hRgn == NULL);
		m_hRgn = ::CreatePolygonRgn (lpPoints, nCount, nMode);
		assert (m_hRgn);
	}

	inline void CRgn::CreatePolyPolygonRgn (LPPOINT lpPoints, LPINT lpPolyCounts, int nCount, int nPolyFillMode)
	{
		assert(m_hRgn == NULL);
		m_hRgn = ::CreatePolyPolygonRgn (lpPoints, lpPolyCounts, nCount, nPolyFillMode);
		assert (m_hRgn);
	}

	inline void CRgn::CreateRoundRectRgn (int x1, int y1, int x2, int y2, int x3, int y3)
	{
		assert(m_hRgn == NULL);
		m_hRgn = ::CreateRoundRectRgn (x1, y1, x2, y2, x3, y3);
		assert (m_hRgn);
	}

	inline void CRgn::CreateFromPath (HDC hDC)
	{
		assert(m_hRgn == NULL);
		assert(hDC != NULL);
		m_hRgn = ::PathToRegion(hDC);
		assert(m_hRgn);
	}

	inline void CRgn::CreateFromData (const XFORM* lpXForm, int nCount, const RGNDATA* pRgnData)
	{
		assert(m_hRgn == NULL);
		m_hRgn = ::ExtCreateRegion (lpXForm, nCount, pRgnData);
		assert(m_hRgn);
	}
#endif // !_WIN32_WCE

	inline void CRgn::SetRectRgn (int x1, int y1, int x2, int y2)
	{
		assert(m_hRgn != NULL);
		::SetRectRgn (m_hRgn, x1, y1, x2, y2);
	}

	inline void CRgn::SetRectRgn (LPCRECT lpRect)
	{
		assert(m_hRgn != NULL);
		::SetRectRgn (m_hRgn, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
	}

	inline int CRgn::CombineRgn (HRGN hRgnSrc1, HRGN hRgnSrc2, int nCombineMode)
	{
		assert(m_hRgn != NULL);
		return ::CombineRgn (m_hRgn, hRgnSrc1, hRgnSrc2, nCombineMode);
	}

	inline int CRgn::CombineRgn (HRGN hRgnSrc, int nCombineMode)
	{
		assert(m_hRgn != NULL);
		return ::CombineRgn (m_hRgn, m_hRgn, hRgnSrc, nCombineMode);
	}

	inline int CRgn::CopyRgn (HRGN hRgnSrc)
	{
		assert(m_hRgn != NULL);
		return ::CombineRgn (m_hRgn, hRgnSrc, NULL, RGN_COPY);
	}

	inline BOOL CRgn::EqualRgn (HRGN hRgn) const
	{
		assert(m_hRgn != NULL);
		return ::EqualRgn (m_hRgn, hRgn);
	}

	inline int CRgn::OffsetRgn (int x, int y)
	{
		assert(m_hRgn != NULL);
		return ::OffsetRgn (m_hRgn, x, y);
	}

	inline int CRgn::OffsetRgn (POINT point)
	{
		assert(m_hRgn != NULL);
		return ::OffsetRgn (m_hRgn, point.x, point.y);
	}

	inline int CRgn::GetRgnBox (LPRECT lpRect) const
	{
		assert(m_hRgn != NULL);
		return ::GetRgnBox (m_hRgn, lpRect);
	}

	inline BOOL CRgn::PtInRegion (int x, int y) const
	{
		assert(m_hRgn != NULL);
		return ::PtInRegion (m_hRgn, x, y);
	}

	inline BOOL CRgn::PtInRegion (POINT point) const
	{
		assert(m_hRgn != NULL);
		return ::PtInRegion (m_hRgn, point.x, point.y);
	}

	inline BOOL CRgn::RectInRegion (LPCRECT lpRect) const
	{
		assert(m_hRgn != NULL);
		return ::RectInRegion (m_hRgn, lpRect);
	}

	inline int CRgn::GetRegionData (LPRGNDATA lpRgnData, int nDataSize) const
	{
		assert(m_hRgn != NULL);
		return (int)::GetRegionData (m_hRgn, nDataSize, lpRgnData);
	}


	///////////////////////////////////////////////
	// Definitions of the CDC class
	//
	inline CDC::CDC() : m_DC(0), m_BitmapOld(0), m_BrushOld(0), m_FontOld(0),
		                 m_PenOld(0), m_RgnOld(0), m_Count(0)
	{
		try
		{
			m_DC = new HDC;
			m_BitmapOld = new HBITMAP;
			m_BrushOld = new HBRUSH;
			m_FontOld = new HFONT;
			m_PenOld = new HPEN;
			m_RgnOld = new HRGN;
			m_Count = new long;

			*m_DC = 0;
			*m_BitmapOld = 0;
			*m_BrushOld = 0;
			*m_FontOld = 0;
			*m_PenOld = 0;
			*m_RgnOld = 0;
			*m_Count = 1;
		}
		catch (const std::bad_alloc&)
		{
			if (m_DC)			delete m_DC;
			if (m_BitmapOld)	delete m_BitmapOld;
			if (m_BrushOld)		delete m_BrushOld;
			if (m_FontOld)		delete m_FontOld;
			if (m_PenOld)		delete m_PenOld;
			if (m_RgnOld)		delete m_RgnOld;
			if (m_Count)		delete m_Count;

			throw;
		}
	}

	inline CDC::CDC(HDC hDC) : m_DC(0), m_BitmapOld(0), m_BrushOld(0), m_FontOld(0),
		                        m_PenOld(0), m_RgnOld(0), m_Count(0)
	{
		// This constructor assigns an existing HDC to the CDC
		// The HDC WILL be released or deleted when the CDC object is destroyed

		// Note: this constructor permits a call like this:
		// CDC MyCDC = SomeHDC;
		//  or
		// CDC MyCDC = ::CreateCompatibleDC(SomeHDC);
		//  or
		// CDC MyCDC = ::GetDC(SomeHWND);

		try
		{
			assert(hDC);

			m_DC = new HDC;
			m_BitmapOld = new HBITMAP;
			m_BrushOld = new HBRUSH;
			m_FontOld = new HFONT;
			m_PenOld = new HPEN;
			m_RgnOld = new HRGN;
			m_Count = new long;

			*m_DC = hDC;
			*m_BitmapOld = 0;
			*m_BrushOld = 0;
			*m_FontOld = 0;
			*m_PenOld = 0;
			*m_RgnOld = 0;
			*m_Count = 1;
		}
		catch (const std::bad_alloc&)
		{
			if (m_DC)			delete m_DC;
			if (m_BitmapOld)	delete m_BitmapOld;
			if (m_BrushOld)		delete m_BrushOld;
			if (m_FontOld)		delete m_FontOld;
			if (m_PenOld)		delete m_PenOld;
			if (m_RgnOld)		delete m_RgnOld;
			if (m_Count)		delete m_Count;

			throw;
		}
	}

	inline CDC::CDC(const CDC& rhs)	// Copy constructor
	{
		// The copy constructor is called when a temporary copy of the CDC needs to be created.
		// This can happen when a CDC is passed by value in a function call. Each CDC copy manages
		// the same Device Context and GDI objects.

		m_BitmapOld = rhs.m_BitmapOld;
		m_BrushOld  = rhs.m_BrushOld;
		m_DC		= rhs.m_DC;
		m_FontOld	= rhs.m_FontOld;
		m_PenOld    = rhs.m_PenOld;
		m_RgnOld    = rhs.m_RgnOld;
		m_Count		= rhs.m_Count;

		InterlockedIncrement(m_Count);
	}

	inline void CDC::operator = (const HDC hDC)
	{
		// Note: this assignment operater permits a call like this:
		// CDC MyCDC;
		// MyCDC = SomeHDC;

		AttachDC(hDC);
	}

	inline CDC& CDC::operator = ( const CDC& rhs )
	{
		// Note: A copy of a CDC is a clone of the original.
		//       Both objects manipulate the one HDC
		if (this != &rhs)
		{
			Release();

			m_BitmapOld = rhs.m_BitmapOld;
			m_BrushOld  = rhs.m_BrushOld;
			m_DC		= rhs.m_DC;
			m_FontOld	= rhs.m_FontOld;
			m_PenOld    = rhs.m_PenOld;
			m_RgnOld    = rhs.m_RgnOld;
			m_Count		= rhs.m_Count;

			InterlockedIncrement(m_Count);
		}

		return *this;
	}

	inline CDC::~CDC()
	{
		Release();
	}

	inline void CDC::AttachDC(HDC hDC)
	{
		assert(m_DC);
		assert(0 == *m_DC);
		assert(hDC);

		*m_DC = hDC;
	}

	inline HDC CDC::DetachDC()
	{
		assert(m_DC);
		assert(*m_DC);

		if (*m_PenOld)    ::SelectObject(*m_DC, *m_PenOld);
		if (*m_BrushOld)  ::SelectObject(*m_DC, *m_BrushOld);
		if (*m_BitmapOld) ::SelectObject(*m_DC, *m_BitmapOld);
		if (*m_FontOld)	  ::SelectObject(*m_DC, *m_FontOld);

		*m_PenOld = 0;
		*m_BrushOld = 0;
		*m_BitmapOld = 0;
		*m_FontOld = 0;

		HDC hDC = *m_DC;

		*m_DC = 0;
		return hDC;
	}

	inline void CDC::DrawBitmap( int x, int y, int cx, int cy, HBITMAP hbmImage, COLORREF clrMask )
	// Draws the specified bitmap to the specified DC using the mask colour provided as the transparent colour
	// Suitable for use with a Window DC or a memory DC
	{
		// Create the Image memory DC
		CDC dcImage = CreateCompatibleDC();
		dcImage.SetBkColor(clrMask);
		dcImage.AttachBitmap(hbmImage);

		// Create the Mask memory DC
		CDC dcMask = CreateCompatibleDC();
        dcMask.CreateBitmap(cx, cy, 1, 1, NULL);
		dcMask.BitBlt(0, 0, cx, cy, dcImage, 0, 0, SRCCOPY);

		// Mask the image to 'this' DC
		BitBlt(x, y, cx, cy, dcImage, 0, 0, SRCINVERT);
		BitBlt(x, y, cx, cy, dcMask, 0, 0, SRCAND);
		BitBlt(x, y, cx, cy, dcImage, 0, 0, SRCINVERT);

		// Detach the bitmap before the dcImage is destroyed
	//	dcImage.DetachBitmap();
	}

	inline void CDC::GradientFill( COLORREF Color1, COLORREF Color2, const RECT& rc, BOOL bVertical )
	// An efficient color gradient filler compatible with all Windows operating systems
	{
		int Width = rc.right - rc.left;
		int Height = rc.bottom - rc.top;

		int r1 = GetRValue(Color1);
		int g1 = GetGValue(Color1);
		int b1 = GetBValue(Color1);

		int r2 = GetRValue(Color2);
		int g2 = GetGValue(Color2);
		int b2 = GetBValue(Color2);

		COLORREF OldBkColor = GetBkColor();

		if (bVertical)
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
				CRect line( rc.left, i + rc.top, rc.left+Width, i + 1 + rc.top);
				ExtTextOut(0, 0, ETO_OPAQUE, line, NULL, 0, 0);
			}
		}

		SetBkColor(OldBkColor);
	}

	inline void CDC::Release()
	{
		if (m_Count)
		{
		if (InterlockedDecrement(m_Count) == 0)
			{
				if (*m_DC)
				{
					// Delete any GDI objects belonging to this CDC
					if (*m_PenOld)		::SelectObject(*m_DC, *m_PenOld);
					if (*m_BrushOld)	::SelectObject(*m_DC, *m_BrushOld);
					if (*m_BitmapOld)	::SelectObject(*m_DC, *m_BitmapOld);
					if (*m_FontOld)		::SelectObject(*m_DC, *m_FontOld);
					if (*m_RgnOld)		::SelectClipRgn(*m_DC, NULL);

					// We need to release a Window DC, and delete a memory DC
		#ifndef _WIN32_WCE
					HWND hwnd = ::WindowFromDC(*m_DC);
					if (hwnd) ::ReleaseDC(hwnd, *m_DC);
					else      ::DeleteDC(*m_DC);
		#else
					::DeleteDC(*m_DC);
		#endif
				}

				delete m_DC;
				delete m_PenOld;
				delete m_BrushOld;
				delete m_BitmapOld;
				delete m_FontOld;
				delete m_RgnOld;
				delete m_Count;

				m_DC = 0;
				m_PenOld = 0;
				m_BrushOld = 0;
				m_BitmapOld = 0;
				m_FontOld = 0;
				m_RgnOld  = 0;
				m_Count = 0;
			}
		}
	}

	inline void CDC::SolidFill( COLORREF Color, const RECT& rc )
	// Fills a rectangle with a solid color
	{
		COLORREF OldColor = SetBkColor(Color);
		ExtTextOut(0, 0, ETO_OPAQUE, rc, NULL, 0, 0);
		SetBkColor(OldColor);
	}

	// Bitmap functions
	inline void CDC::AttachBitmap(HBITMAP hBitmap)
	{
		// Use this to attach an existing bitmap.
		// The bitmap will be deleted for you, unless its detached

		assert(*m_DC);
		assert(hBitmap);

		RemoveCurrentBitmap();
		*m_BitmapOld = (HBITMAP)::SelectObject(*m_DC, hBitmap);
	}

	inline void CDC::CreateCompatibleBitmap(HDC hDC, int cx, int cy)
	{
		// Creates a compatible bitmap and selects it into the device context

		assert(*m_DC);
		RemoveCurrentBitmap();
		
		m_Bitmap.CreateCompatibleBitmap(hDC, cx, cy);
		*m_BitmapOld = (HBITMAP)::SelectObject(*m_DC, m_Bitmap);
	}

	inline void CDC::CreateBitmap(int cx, int cy, UINT Planes, UINT BitsPerPixel, CONST VOID *pvColors)
	{
		// Creates a bitmap and selects it into the device context

		assert(*m_DC);
		RemoveCurrentBitmap();

		m_Bitmap.CreateBitmap(cx, cy, Planes, BitsPerPixel, pvColors);
		*m_BitmapOld = (HBITMAP)::SelectObject(*m_DC, m_Bitmap);
	}

#ifndef _WIN32_WCE
	inline void CDC::CreateBitmapIndirect(LPBITMAP pBitmap)
	{
		// Creates a bitmap and selects it into the device context

		assert(*m_DC);
		RemoveCurrentBitmap();

		m_Bitmap.CreateBitmapIndirect(pBitmap);
		*m_BitmapOld = (HBITMAP)::SelectObject(*m_DC, m_Bitmap);
	}

	inline void CDC::CreateDIBitmap(HDC hdc, const BITMAPINFOHEADER& bmih, DWORD fdwInit, CONST VOID *lpbInit,
										BITMAPINFO& bmi,  UINT fuUsage)
	{
		// Creates a bitmap and selects it into the device context

		assert(*m_DC);
		RemoveCurrentBitmap();

		m_Bitmap.CreateDIBitmap(hdc, &bmih, fdwInit, lpbInit, &bmi, fuUsage);
		*m_BitmapOld = (HBITMAP)::SelectObject(*m_DC, m_Bitmap);
	}
#endif

	inline void CDC::CreateDIBSection(HDC hdc, const BITMAPINFO& bmi, UINT iUsage, VOID **ppvBits,
										HANDLE hSection, DWORD dwOffset)
	{
		// Creates a bitmap and selects it into the device context

		assert(*m_DC);
		RemoveCurrentBitmap();

		m_Bitmap.CreateDIBSection(hdc, &bmi, iUsage, ppvBits, hSection, dwOffset);
		*m_BitmapOld = (HBITMAP)::SelectObject(*m_DC, m_Bitmap);
	}

	inline HBITMAP CDC::DetachBitmap()
	{
		// Use this to detach the bitmap from the HDC.

		assert(*m_DC);
		assert(*m_BitmapOld);

		HBITMAP hBitmap = (HBITMAP)::SelectObject(*m_DC, *m_BitmapOld);
		if (hBitmap == m_Bitmap)
			m_Bitmap.Detach();
		
		*m_BitmapOld = NULL;
		return hBitmap;
	}

	inline BITMAP CDC::GetBitmapInfo()
	{
		// Retrieves the BITMAP for the current HBITMAP

		assert(*m_DC);

		HBITMAP hbm = (HBITMAP)::GetCurrentObject(*m_DC, OBJ_BITMAP);
		BITMAP bm = {0};
		::GetObject(hbm, sizeof(bm), &bm);
		return bm;
	}


	// Brush functions
	inline void CDC::AttachBrush(HBRUSH hBrush)
	{
		// Use this to attach an existing brush.
		// The brush will be deleted for you, unless its detached

		assert(*m_DC);
		assert(hBrush);
		RemoveCurrentBrush();
		*m_BrushOld = (HBRUSH)::SelectObject(*m_DC, hBrush);
	}

#ifndef _WIN32_WCE
	inline void CDC::CreateBrushIndirect( LPLOGBRUSH& pLogBrush)
	{
		// Creates the brush and selects it into the device context

		assert(*m_DC);
		RemoveCurrentBrush();

		m_Brush.CreateBrushIndirect(pLogBrush);
		*m_BrushOld = (HBRUSH)::SelectObject(*m_DC, m_Brush);
	}

	inline void CDC::CreateHatchBrush(int fnStyle, COLORREF rgb)
	{
		// Creates the brush and selects it into the device context

		assert(*m_DC);
		RemoveCurrentBrush();

		m_Brush.CreateHatchBrush(fnStyle, rgb);
		*m_BrushOld = (HBRUSH)::SelectObject(*m_DC, m_Brush);
	}
#endif

	inline void CDC::CreateDIBPatternBrushPt(LPCVOID lpPackedDIB, UINT iUsage)
	{
		// Creates the brush and selects it into the device context

		assert(*m_DC);
		RemoveCurrentBrush();

		m_Brush.CreateDIBPatternBrushPt(lpPackedDIB, iUsage);
		*m_BrushOld = (HBRUSH)::SelectObject(*m_DC, m_Brush);
	}

	inline void CDC::CreatePatternBrush(HBITMAP hbmp)
	{
		// Creates the brush and selects it into the device context

		assert(*m_DC);
		RemoveCurrentBrush();

		m_Brush.CreatePatternBrush(hbmp);
		*m_BrushOld = (HBRUSH)::SelectObject(*m_DC, m_Brush);
	}

	inline void CDC::CreateSolidBrush(COLORREF rgb)
	{
		// Creates the brush and selects it into the device context

		assert(*m_DC);
		RemoveCurrentBrush();

		m_Brush.CreateSolidBrush(rgb);
		*m_BrushOld = (HBRUSH)::SelectObject(*m_DC, m_Brush);
	}

	inline HBRUSH CDC::DetachBrush()
	{
		// Use this to detach the brush from the HDC.

		assert(*m_DC);
		assert(*m_BrushOld);

		HBRUSH hBrush = (HBRUSH)::SelectObject(*m_DC, *m_BrushOld);
		if (hBrush == m_Brush)
			m_Brush.Detach();

		*m_BrushOld = NULL;
		return hBrush;
	}

	inline LOGBRUSH CDC::GetBrushInfo( )
	{
		// Retrieves the current brush information

		assert(*m_DC);

		HBRUSH hBrush = (HBRUSH)::GetCurrentObject(*m_DC, OBJ_BRUSH);
		LOGBRUSH lBrush = {0};
		::GetObject(hBrush, sizeof(lBrush), &lBrush);
		return lBrush;
	}


	// Font functions
	inline void CDC::AttachFont(HFONT hFont)
	{
		// Use this to attach an existing font.
		// The font will be deleted for you, unless its detached

		assert(*m_DC);
		assert(hFont);
		
		RemoveCurrentFont();
		*m_FontOld = (HFONT)::SelectObject(*m_DC, hFont);
	}

#ifndef _WIN32_WCE
	inline void CDC::CreateFont(
					int nHeight,               // height of font
  					int nWidth,                // average character width
  					int nEscapement,           // angle of escapement
  					int nOrientation,          // base-line orientation angle
  					int fnWeight,              // font weight
  					DWORD fdwItalic,           // italic attribute option
  					DWORD fdwUnderline,        // underline attribute option
  					DWORD fdwStrikeOut,        // strikeout attribute option
  					DWORD fdwCharSet,          // character set identifier
  					DWORD fdwOutputPrecision,  // output precision
  					DWORD fdwClipPrecision,    // clipping precision
  					DWORD fdwQuality,          // output quality
  					DWORD fdwPitchAndFamily,   // pitch and family
  					LPCTSTR lpszFace           // typeface name
 					)

	{
		// Creates a logical font with the specified characteristics.

		assert(*m_DC);
		RemoveCurrentFont();

		m_Font.CreateFont(nHeight, nWidth, nEscapement, nOrientation, fnWeight,
								fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet,
								fdwOutputPrecision, fdwClipPrecision, fdwQuality,
								fdwPitchAndFamily, lpszFace);

		*m_FontOld = (HFONT)::SelectObject(*m_DC, m_Font);
	}
#endif

	inline void CDC::CreateFontIndirect( const LOGFONT& lf)
	{
		// Creates a logical font that has the specified characteristics

		assert(*m_DC);
		RemoveCurrentFont();

		m_Font.CreateFontIndirect(&lf);
		*m_FontOld = (HFONT)::SelectObject(*m_DC, m_Font);
	}

	inline HFONT CDC::DetachFont()
	{
		// Use this to detach the font from the HDC.

		assert(*m_DC);
		assert(*m_FontOld);

		HFONT hFont = (HFONT)::SelectObject(*m_DC, *m_FontOld);
		if (m_Font == hFont)
			m_Font.Detach();

		*m_FontOld = NULL;
		return hFont;
	}

	inline LOGFONT CDC::GetFontInfo( )
	{
		// Retrieves the current font information

		assert(*m_DC);

		HFONT hFont = (HFONT)::GetCurrentObject(*m_DC, OBJ_FONT);
		LOGFONT lFont = {0};
		::GetObject(hFont, sizeof(lFont), &lFont);
		return lFont;
	}

	// Pen functions
	inline void CDC::AttachPen(HPEN hPen)
	{
		// Use this to attach an existing pen.
		// The pen will be deleted for you, unless its detached

		assert(*m_DC);
		assert(hPen);
		RemoveCurrentPen();
			
		*m_PenOld = (HPEN)::SelectObject(*m_DC, hPen);
	}

	inline void CDC::CreatePen(int nStyle, int nWidth, COLORREF rgb)
	{
		// Creates the pen and selects it into the device context

		assert(*m_DC);
		RemoveCurrentPen();

		m_Pen.CreatePen(nStyle, nWidth, rgb);
		*m_PenOld = (HPEN)::SelectObject(*m_DC, m_Pen);
	}

	inline void CDC::CreatePenIndirect( LPLOGPEN pLogPen)
	{
		// Creates the pen and selects it into the device context

		assert(*m_DC);
		RemoveCurrentPen();

		m_Pen.CreatePenIndirect(pLogPen);
		*m_PenOld = (HPEN)::SelectObject(*m_DC, m_Pen);
	}

	inline HPEN CDC::DetachPen()
	{
		// Use this to detach the pen from the HDC.

		assert(*m_DC);
		assert(*m_PenOld);

		HPEN hPen = (HPEN)::SelectObject(*m_DC, *m_PenOld);
		if (hPen == m_Pen)
			m_Pen.Detach();

		*m_PenOld = NULL;
		return hPen;
	}

	inline LOGPEN CDC::GetPenInfo( )
	{
		// Retrieves the current pen information as a LOGPEN

		assert(*m_DC);

		HPEN hPen = (HPEN)::GetCurrentObject(*m_DC, OBJ_PEN);
		LOGPEN lPen = {0};
		::GetObject(hPen, sizeof(lPen), &lPen);
		return lPen;
	}

	// Region functions
	inline void CDC::AttachClipRgn(HRGN hRegion)
	{
		// Use this to attach an existing region.
		// The region will be deleted for you, unless its detached
		// Note: The shape of a region cannot be changed while it is attached to a DC

		assert(*m_DC);
		assert(hRegion);
		RemoveCurrentRgn();

		::SelectClipRgn(*m_DC, hRegion);
		*m_RgnOld = hRegion;
	}
	inline void CDC::CreateRectRgn(int left, int top, int right, int bottom)
	{
		// Creates a rectangular region from the rectangle co-ordinates.
		// The region will be deleted for you, unless its detached
		// Note: The shape of a region cannot be changed while it is attached to a DC

		assert(*m_DC);
		RemoveCurrentRgn();

		m_Rgn.CreateRectRgn(left, top, right, bottom);
		::SelectClipRgn(*m_DC, m_Rgn);
		*m_RgnOld = m_Rgn;
	}

	inline void CDC::CreateRectRgnIndirect( const RECT& rc)
	{
		// Creates a rectangular region from the rectangle co-ordinates.
		// The region will be deleted for you, unless its detached
		// Note: The shape of a region cannot be changed while it is attached to a DC

		assert(*m_DC);
		RemoveCurrentRgn();

		m_Rgn.CreateRectRgnIndirect(&rc);
		::SelectClipRgn(*m_DC, m_Rgn);
		*m_RgnOld = m_Rgn;
	}

	inline void CDC::CreateFromData( const XFORM* Xform, DWORD nCount, const RGNDATA *pRgnData)
	{
		// Creates a region from the specified region data and tranformation data.
		// The region will be deleted for you, unless its detached
		// Notes: The shape of a region cannot be changed while it is attached to a DC
		//        GetRegionData can be used to get a region's data
		//        If the XFROM pointer is NULL, the identity transformation is used.

		assert(*m_DC);
		RemoveCurrentRgn();

		m_Rgn.CreateFromData(Xform, nCount, pRgnData);
		::SelectClipRgn(*m_DC, m_Rgn);
		*m_RgnOld = m_Rgn;
	}

	inline HRGN CDC::DetachClipRgn()
	{
		// Use this to detach the region from the HDC.

		assert(*m_DC);
		assert(*m_RgnOld);

		::SelectClipRgn(*m_DC, NULL);
		HRGN hRgn = *m_RgnOld;
		if (hRgn == m_Rgn)
			m_Rgn.Detach();

		*m_RgnOld = NULL;
		return hRgn;
	}

#ifndef _WIN32_WCE
		inline void CDC::CreateEllipticRgn(int left, int top, int right, int bottom)
	{
		// Creates the ellyiptical region from the bounding rectangle co-ordinates
		// and selects it into the device context
		// The region will be deleted for you, unless its detached
		// Note: The shape of a region cannot be changed while it is attached to a DC

		assert(*m_DC);
		RemoveCurrentRgn();

		m_Rgn.CreateEllipticRgn(left, top, right, bottom);
		::SelectClipRgn(*m_DC, m_Rgn);
		*m_RgnOld = m_Rgn;
	}

	inline void CDC::CreateEllipticRgnIndirect( const RECT& rc)
	{
		// Creates the ellyiptical region from the bounding rectangle co-ordinates
		// and selects it into the device context
		// The region will be deleted for you, unless its detached
		// Note: The shape of a region cannot be changed while it is attached to a DC

		assert(*m_DC);
		RemoveCurrentRgn();

		m_Rgn.CreateEllipticRgnIndirect(&rc);
		::SelectClipRgn(*m_DC, m_Rgn);
		*m_RgnOld = m_Rgn;
	}

	inline void CDC::CreatePolygonRgn(LPPOINT ppt, int cPoints, int fnPolyFillMode)
	{
		// Creates the polygon region from the array of points and selects it into
		// the device context. The polygon is presumed closed
		// The region will be deleted for you, unless its detached
		// Note: The shape of a region cannot be changed while it is attached to a DC

		assert(*m_DC);
		RemoveCurrentRgn();

		m_Rgn.CreatePolygonRgn(ppt, cPoints, fnPolyFillMode);
		::SelectClipRgn(*m_DC, m_Rgn);
		*m_RgnOld = m_Rgn;
	}

	inline void CDC::CreatePolyPolygonRgn(LPPOINT ppt, LPINT pPolyCounts, int nCount, int fnPolyFillMode)
	{
		// Creates the polygon region from a series of polygons.The polygons can overlap.
		// The region will be deleted for you, unless its detached
		// Note: The shape of a region cannot be changed while it is attached to a DC

		assert(*m_DC);
		RemoveCurrentRgn();

		m_Rgn.CreatePolyPolygonRgn(ppt, pPolyCounts, nCount, fnPolyFillMode);
		::SelectClipRgn(*m_DC, m_Rgn);
		*m_RgnOld = m_Rgn;
	}
#endif


	// Wrappers for WinAPI functions

	// Initialization
	inline HDC CDC::CreateCompatibleDC( ) const
	{
		return ::CreateCompatibleDC( *m_DC );
	}
	inline HDC CDC::CreateDC( LPCTSTR lpszDriver, LPCTSTR lpszDevice, LPCTSTR lpszOutput, const DEVMODE& dvmInit ) const
	{
		return ::CreateDC( lpszDriver, lpszDevice, lpszOutput, &dvmInit );
	}
	inline int CDC::GetDeviceCaps( int nIndex ) const
	{
		return ::GetDeviceCaps(*m_DC, nIndex);
	}

	// Point and Line Drawing Functions
	inline CPoint CDC::GetCurrentPosition( ) const
	{
		//  returns the current "MoveToEx" position
		CPoint pt;
		::MoveToEx( *m_DC, 0, 0, &pt );
		::MoveToEx( *m_DC, pt.x, pt.y, NULL);
		return pt;
	}
	inline CPoint CDC::MoveTo( int x, int y ) const
	{
		// Updates the current position to the specified point
		return ::MoveToEx( *m_DC, x, y, NULL );
	}
	inline CPoint CDC::MoveTo( POINT pt ) const
	{
		// Updates the current position to the specified point
		return ::MoveToEx( *m_DC, pt.x, pt.y, NULL );
	}
	inline BOOL CDC::LineTo( int x, int y ) const
	{
		// Draws a line from the current position up to, but not including, the specified point
		return ::LineTo( *m_DC, x, y );
	}
	inline BOOL CDC::LineTo( POINT pt ) const
	{
		// Draws a line from the current position up to, but not including, the specified point
		return ::LineTo( *m_DC, pt.x, pt.y );
	}

#ifndef _WIN32_WCE
	inline BOOL CDC::Arc( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 ) const
	{
		return ::Arc( *m_DC, x1, y1, x2, y2, x3, y3, x4, y4 );
	}
	inline BOOL CDC::Arc( RECT& rc, POINT ptStart, POINT ptEnd ) const
	{
		// Draws an elliptical arc
		return ::Arc( *m_DC, rc.left, rc.top, rc.right, rc.bottom,
			ptStart.x, ptStart.y, ptEnd.x, ptEnd.y );
	}
	inline BOOL CDC::ArcTo( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 ) const
	{
		// Draws an elliptical arc
		return ::ArcTo( *m_DC, x1, y1, x2, y2, x3, y3, x4, y4) ;
	}
	inline BOOL CDC::ArcTo( RECT& rc, POINT ptStart, POINT ptEnd ) const
	{
		// Draws an elliptical arc
		return ::ArcTo(  *m_DC, rc.left, rc.top, rc.right, rc.bottom,
			ptStart.x, ptStart.y, ptEnd.x, ptEnd.y );
	}
	inline BOOL CDC::AngleArc( int x, int y, int nRadius, float fStartAngle, float fSweepAngle ) const
	{
		// Draws a line segment and an arc
		return ::AngleArc( *m_DC, x, y, nRadius, fStartAngle, fSweepAngle);
	}
	inline int CDC::GetArcDirection( ) const
	{
		// Retrieves the current arc direction ( AD_COUNTERCLOCKWISE or AD_CLOCKWISE )
		return ::GetArcDirection( *m_DC );
	}
	inline int CDC::SetArcDirection( int nArcDirection ) const
	{
		// Sets the current arc direction ( AD_COUNTERCLOCKWISE or AD_CLOCKWISE )
		return ::SetArcDirection( *m_DC, nArcDirection );
	}
	inline BOOL CDC::PolyDraw( const POINT* lpPoints, const BYTE* lpTypes, int nCount ) const
	{
		// Draws a set of line segments and Bzier curves
		return ::PolyDraw( *m_DC, lpPoints, lpTypes, nCount );
	}
	inline BOOL CDC::Polyline( LPPOINT lpPoints, int nCount ) const
	{
		// Draws a series of line segments by connecting the points in the specified array
		return ::Polyline( *m_DC, lpPoints, nCount );
	}
	inline BOOL CDC::PolyPolyline( const POINT* lpPoints, const DWORD* lpPolyPoints, int nCount ) const
	{
		// Draws multiple series of connected line segments
		return ::PolyPolyline( *m_DC, lpPoints, lpPolyPoints, nCount );
	}
	inline BOOL CDC::PolylineTo( const POINT* lpPoints, int nCount ) const
	{
		// Draws one or more straight lines
		return ::PolylineTo( *m_DC, lpPoints, nCount );
	}
	inline BOOL CDC::PolyBezier( const POINT* lpPoints, int nCount ) const
	{
		// Draws one or more Bzier curves
		return ::PolyBezier( *m_DC, lpPoints, nCount );
	}
	inline BOOL CDC::PolyBezierTo( const POINT* lpPoints, int nCount ) const
	{
		// Draws one or more Bzier curves
		return ::PolyBezierTo(*m_DC, lpPoints, nCount );
	}
	inline COLORREF CDC::GetPixel( int x, int y ) const
	{
		// Retrieves the red, green, blue (RGB) color value of the pixel at the specified coordinates
		return ::GetPixel( *m_DC, x, y );
	}
	inline COLORREF CDC::GetPixel( POINT pt ) const
	{
		// Retrieves the red, green, blue (RGB) color value of the pixel at the specified coordinates
		return ::GetPixel( *m_DC, pt.x, pt.y );
	}
	inline COLORREF CDC::SetPixel( int x, int y, COLORREF crColor ) const
	{
		// Sets the pixel at the specified coordinates to the specified color
		return ::SetPixel( *m_DC, x, y, crColor );
	}
	inline COLORREF CDC::SetPixel( POINT pt, COLORREF crColor ) const
	{
		// Sets the pixel at the specified coordinates to the specified color
		return ::SetPixel( *m_DC, pt.x, pt.y, crColor );
	}
	inline BOOL CDC::SetPixelV( int x, int y, COLORREF crColor ) const
	{
		// Sets the pixel at the specified coordinates to the closest approximation of the specified color
		return ::SetPixelV( *m_DC, x, y, crColor );
	}
	inline BOOL CDC::SetPixelV( POINT pt, COLORREF crColor ) const
	{
		// Sets the pixel at the specified coordinates to the closest approximation of the specified color
		return ::SetPixelV( *m_DC, pt.x, pt.y, crColor );
	}
#endif

	// Shape Drawing Functions
	inline void CDC::DrawFocusRect( const RECT& rc ) const
	{
		// draws a rectangle in the style used to indicate that the rectangle has the focus
		::DrawFocusRect( *m_DC, &rc );
	}
	inline BOOL CDC::Ellipse( int x1, int y1, int x2, int y2 ) const
	{
		// Draws an ellipse. The center of the ellipse is the center of the specified bounding rectangle.
		return ::Ellipse( *m_DC, x1, y1, x2, y2 );
	}
	inline BOOL CDC::Ellipse( const RECT& rc ) const
	{
		// Draws an ellipse. The center of the ellipse is the center of the specified bounding rectangle.
		return ::Ellipse( *m_DC, rc.left, rc.top, rc.right, rc.bottom );
	}
	inline BOOL CDC::Polygon( LPPOINT lpPoints, int nCount ) const
	{
		// Draws a polygon consisting of two or more vertices connected by straight lines
		return ::Polygon( *m_DC, lpPoints, nCount);
	}
	inline BOOL CDC::Rectangle( int x1, int y1, int x2, int y2 ) const
	{
		// Draws a rectangle. The rectangle is outlined by using the current pen and filled by using the current brush.
		return ::Rectangle( *m_DC, x1, y1, x2, y2 );
	}
	inline BOOL CDC::Rectangle( const RECT& rc) const
	{
		// Draws a rectangle. The rectangle is outlined by using the current pen and filled by using the current brush.
		return ::Rectangle( *m_DC, rc.left, rc.top, rc.right, rc.bottom );
	}
	inline BOOL CDC::RoundRect( int x1, int y1, int x2, int y2, int nWidth, int nHeight ) const
	{
		// Draws a rectangle with rounded corners
		return ::RoundRect( *m_DC, x1, y1, x2, y2, nWidth, nHeight );
	}
	inline BOOL CDC::RoundRect( const RECT& rc, int nWidth, int nHeight ) const
	{
		// Draws a rectangle with rounded corners
		return ::RoundRect(*m_DC, rc.left, rc.top, rc.right, rc.bottom, nWidth, nHeight );
	}

#ifndef _WIN32_WCE
	inline BOOL CDC::Chord( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 ) const
	{
		// Draws a chord (a region bounded by the intersection of an ellipse and a line segment, called a secant)
		return ::Chord( *m_DC, x1, y1, x2, y2, x3, y3, x4, y4 );
	}
	inline BOOL CDC::Chord( const RECT& rc, POINT ptStart, POINT ptEnd ) const
	{
		// Draws a chord (a region bounded by the intersection of an ellipse and a line segment, called a secant)
		return ::Chord( *m_DC, rc.left, rc.top, rc.right, rc.bottom,
			ptStart.x, ptStart.y, ptEnd.x, ptEnd.y );
	}
	inline BOOL CDC::Pie( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 ) const
	{
		// Draws a pie-shaped wedge bounded by the intersection of an ellipse and two radials.
		return ::Pie( *m_DC, x1, y1, x2, y2, x3, y3, x4, y4 );
	}
	inline BOOL CDC::Pie( const RECT& rc, POINT ptStart, POINT ptEnd ) const
	{
		// Draws a pie-shaped wedge bounded by the intersection of an ellipse and two radials.
		return ::Pie( *m_DC, rc.left, rc.top, rc.right, rc.bottom,
			ptStart.x, ptStart.y, ptEnd.x, ptEnd.y );
	}
	inline BOOL CDC::PolyPolygon( LPPOINT lpPoints, LPINT lpPolyCounts, int nCount ) const
	{
		// Draws a series of closed polygons
		return ::PolyPolygon( *m_DC, lpPoints, lpPolyCounts, nCount );
	}
#endif

	// Fill and 3D Drawing functions
	inline BOOL CDC::FillRect( const RECT& rc, HBRUSH hbr ) const
	{
		// Fills a rectangle by using the specified brush
		return (BOOL)::FillRect( *m_DC, &rc, hbr );
	}
	inline BOOL CDC::InvertRect( const RECT& rc ) const
	{
		// Inverts a rectangle in a window by performing a logical NOT operation on the color values for each pixel in the rectangle's interior
		return ::InvertRect( *m_DC, &rc );
	}
	inline BOOL CDC::DrawIconEx( int xLeft, int yTop, HICON hIcon, int cxWidth, int cyWidth, UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw, UINT diFlags ) const
	{
		// draws an icon or cursor, performing the specified raster operations, and stretching or compressing the icon or cursor as specified.
		return ::DrawIconEx( *m_DC, xLeft, yTop, hIcon, cxWidth, cyWidth, istepIfAniCur, hbrFlickerFreeDraw, diFlags );
	}
	inline BOOL CDC::DrawEdge( const RECT& rc, UINT nEdge, UINT nFlags ) const
	{
		// Draws one or more edges of rectangle
		return ::DrawEdge( *m_DC, (LPRECT)&rc, nEdge, nFlags );
	}
	inline BOOL CDC::DrawFrameControl( const RECT& rc, UINT nType, UINT nState ) const
	{
		// Draws a frame control of the specified type and style
		return ::DrawFrameControl( *m_DC, (LPRECT)&rc, nType, nState );
	}
	inline BOOL CDC::FillRgn( HRGN hrgn, HBRUSH hbr ) const
	{
		// Fills a region by using the specified brush
		return ::FillRgn( *m_DC, hrgn, hbr );
	}

#ifndef _WIN32_WCE
	inline BOOL CDC::DrawIcon( int x, int y, HICON hIcon ) const
	{
		// Draws an icon or cursor
		return ::DrawIcon( *m_DC, x, y, hIcon );
	}
	inline BOOL CDC::DrawIcon( POINT pt, HICON hIcon ) const
	{
		// Draws an icon or cursor
		return ::DrawIcon( *m_DC, pt.x, pt.y, hIcon );
	}
	inline BOOL CDC::FrameRect( const RECT& rc, HBRUSH hbr ) const
	{
		// Draws a border around the specified rectangle by using the specified brush
		return (BOOL)::FrameRect( *m_DC, &rc, hbr );
	}
	inline BOOL CDC::PaintRgn( HRGN hrgn ) const
	{
		// Paints the specified region by using the brush currently selected into the device context
		return ::PaintRgn( *m_DC, hrgn);
	}
#endif

	// Bitmap Functions
	inline int CDC::StretchDIBits( int XDest, int YDest, int nDestWidth, int nDestHeight, int XSrc, int YSrc, int nSrcWidth,
		           int nSrcHeight, CONST VOID *lpBits, BITMAPINFO& bi, UINT iUsage, DWORD dwRop ) const
	{
		// Copies the color data for a rectangle of pixels in a DIB to the specified destination rectangle
		return ::StretchDIBits( *m_DC, XDest, YDest, nDestWidth, nDestHeight, XSrc, YSrc, nSrcWidth, nSrcHeight, lpBits, &bi, iUsage, dwRop );
	}

	inline BOOL CDC::PatBlt( int x, int y, int nWidth, int nHeight, DWORD dwRop ) const
	{
		// Paints the specified rectangle using the brush that is currently selected into the device context
		return ::PatBlt( *m_DC, x, y, nWidth, nHeight, dwRop );
	}
	inline BOOL CDC::BitBlt( int x, int y, int nWidth, int nHeight, HDC hSrcDC, int xSrc, int ySrc, DWORD dwRop ) const
	{
		// Performs a bit-block transfer of the color data corresponding to a rectangle of pixels from the specified source device context into a destination device context
		return ::BitBlt( *m_DC, x, y, nWidth, nHeight, hSrcDC, xSrc, ySrc, dwRop );
	}
	inline BOOL CDC::StretchBlt( int x, int y, int nWidth, int nHeight, HDC hSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop ) const
	{
		// Copies a bitmap from a source rectangle into a destination rectangle, stretching or compressing the bitmap to fit the dimensions of the destination rectangle, if necessary
		return ::StretchBlt( *m_DC, x, y, nWidth, nHeight, hSrcDC, xSrc, ySrc, nSrcWidth, nSrcHeight, dwRop );
	}

#ifndef _WIN32_WCE
	inline int CDC::GetDIBits( HBITMAP hbmp, UINT uStartScan, UINT cScanLines, LPVOID lpvBits, LPBITMAPINFO lpbi, UINT uUsage ) const
	{
		// Retrieves the bits of the specified compatible bitmap and copies them into a buffer as a DIB using the specified format
		return ::GetDIBits( *m_DC, hbmp, uStartScan, cScanLines, lpvBits, lpbi, uUsage );
	}
	inline int CDC::SetDIBits( HBITMAP hbmp, UINT uStartScan, UINT cScanLines, CONST VOID *lpvBits, LPBITMAPINFO lpbi, UINT fuColorUse ) const
	{
		// Sets the pixels in a compatible bitmap (DDB) using the color data found in the specified DIB
		return ::SetDIBits( *m_DC, hbmp, uStartScan, cScanLines, lpvBits, lpbi, fuColorUse );
	}
	inline int CDC::GetStretchBltMode( ) const
	{
		// Retrieves the current stretching mode
		// Possible modes: BLACKONWHITE, COLORONCOLOR, HALFTONE, STRETCH_ANDSCANS, STRETCH_DELETESCANS, STRETCH_HALFTONE, STRETCH_ORSCANS, WHITEONBLACK
		return ::GetStretchBltMode( *m_DC );
	}
	inline int CDC::SetStretchBltMode( int iStretchMode ) const
	{
		// Sets the stretching mode
		// Possible modes: BLACKONWHITE, COLORONCOLOR, HALFTONE, STRETCH_ANDSCANS, STRETCH_DELETESCANS, STRETCH_HALFTONE, STRETCH_ORSCANS, WHITEONBLACK
		return ::SetStretchBltMode( *m_DC, iStretchMode );
	}
	inline BOOL CDC::FloodFill( int x, int y, COLORREF crColor ) const
	{
		// Fills an area of the display surface with the current brush
		return ::FloodFill(*m_DC, x, y, crColor );
	}
	inline BOOL CDC::ExtFloodFill( int x, int y, COLORREF crColor, UINT nFillType ) const
	{
		// Fills an area of the display surface with the current brush
		// Fill type: FLOODFILLBORDER or FLOODFILLSURFACE
		return ::ExtFloodFill(*m_DC, x, y, crColor, nFillType );
	}

	// co-ordinate functions
	inline BOOL CDC::DPtoLP(LPPOINT lpPoints, int nCount )  const
	{
		if (*m_DC != NULL)
			return ::DPtoLP(*m_DC, lpPoints, nCount);
		return FALSE;
	}
	inline BOOL CDC::DPtoLP(LPRECT lpRect)  const
	{
		if (*m_DC != NULL)
			return ::DPtoLP(*m_DC, (LPPOINT)lpRect, 2);
		return FALSE;
	}

	inline BOOL CDC::LPtoDP(LPPOINT lpPoints, int nCount )  const
	{
		if (*m_DC != NULL)
			return ::LPtoDP(*m_DC, lpPoints, nCount);
		return FALSE;
	}
	inline BOOL CDC::LPtoDP(LPRECT lpRect)  const
	{
		if (*m_DC != NULL)
			return ::LPtoDP(*m_DC, (LPPOINT)lpRect, 2);
		return FALSE;
	}

#endif

	// Layout Functions
	inline DWORD CDC::GetLayout() const
	{
		// Returns the layout of a device context (LAYOUT_RTL and LAYOUT_BITMAPORIENTATIONPRESERVED)
#if defined(WINVER) && defined (GetLayout) && (WINVER >= 0x0500)
		return ::GetLayout(*m_DC);
#else
		return 0;
#endif
	}
	inline DWORD CDC::SetLayout(DWORD dwLayout) const
	{
#if defined(WINVER) && defined (SetLayout) && (WINVER >= 0x0500)
		// Sets the layout of a device context
		return ::SetLayout(*m_DC, dwLayout);
#else
		UNREFERENCED_PARAMETER(dwLayout); // no-op
		return 0;
#endif
	}

	// Mapping Functions
#ifndef _WIN32_WCE
	inline int CDC::GetMapMode()  const
	{
		if (*m_DC != NULL)
			return ::GetMapMode(*m_DC);
		return 0;
	}
	inline BOOL CDC::GetViewportOrgEx(LPPOINT lpPoint)  const
	{
		if (*m_DC != NULL)
			return ::GetViewportOrgEx(*m_DC, lpPoint);
		return 0;
	}
	inline int CDC::SetMapMode(int nMapMode) const
	{
		if (*m_DC != NULL)
			return ::SetMapMode(*m_DC, nMapMode);
		return 0;
	}
	inline BOOL CDC::SetViewportOrgEx(int x, int y, LPPOINT lpPoint /* = NULL */) const
	{
		if (*m_DC != NULL)
			return ::SetViewportOrgEx(*m_DC, x, y, lpPoint);
		return FALSE;
	}
	inline BOOL CDC::SetViewportOrgEx(POINT point, LPPOINT lpPointRet /* = NULL */) const
	{
		if (*m_DC != NULL)
			return SetViewportOrgEx(point.x, point.y, lpPointRet);
		return FALSE;
	}
	inline BOOL CDC::OffsetViewportOrgEx(int nWidth, int nHeight, LPPOINT lpPoint /* = NULL */) const
	{
		if (*m_DC != NULL)
			return ::OffsetViewportOrgEx(*m_DC, nWidth, nHeight, lpPoint);
		return FALSE;
	}
	inline BOOL CDC::GetViewportExtEx(LPSIZE lpSize)  const
	{
		if (*m_DC != NULL)
			return ::GetViewportExtEx(*m_DC, lpSize);
		return FALSE;
	}
	inline BOOL CDC::SetViewportExtEx(int x, int y, LPSIZE lpSize ) const
	{
		if (*m_DC != NULL)
			return ::SetViewportExtEx(*m_DC, x, y, lpSize);
		return FALSE;
	}
	inline BOOL CDC::SetViewportExtEx(SIZE size, LPSIZE lpSizeRet ) const
	{
		if (*m_DC != NULL)
			return SetViewportExtEx(size.cx, size.cy, lpSizeRet);
		return FALSE;
	}
	inline BOOL CDC::ScaleViewportExtEx(int xNum, int xDenom, int yNum, int yDenom, LPSIZE lpSize ) const
	{
		if (*m_DC != NULL)
			return ::ScaleViewportExtEx(*m_DC, xNum, xDenom, yNum, yDenom, lpSize);
		return FALSE;
	}
	inline BOOL CDC::GetWindowOrgEx(LPPOINT lpPoint)  const
	{
		if (*m_DC != NULL)
			return ::GetWindowOrgEx(*m_DC, lpPoint);
		return FALSE;
	}
	inline BOOL CDC::SetWindowOrgEx(int x, int y, LPPOINT lpPoint ) const
	{
		if (*m_DC != NULL)
			return ::SetWindowOrgEx(*m_DC, x, y, lpPoint);
		return FALSE;
	}
	inline BOOL CDC::SetWindowOrgEx(POINT point, LPPOINT lpPointRet ) const
	{
		if (*m_DC != NULL)
			return SetWindowOrgEx(point.x, point.y, lpPointRet);
		return FALSE;
	}
	inline BOOL CDC::OffsetWindowOrgEx(int nWidth, int nHeight, LPPOINT lpPoint ) const
	{
		if (*m_DC != NULL)
			return ::OffsetWindowOrgEx(*m_DC, nWidth, nHeight, lpPoint);
		return FALSE;
	}
	inline BOOL CDC::GetWindowExtEx(LPSIZE lpSize)  const
	{
		if (*m_DC != NULL)
			return ::GetWindowExtEx(*m_DC, lpSize);
		return FALSE;
	}
	inline BOOL CDC::SetWindowExtEx(int x, int y, LPSIZE lpSize ) const
	{
		if (*m_DC != NULL)
			return ::SetWindowExtEx(*m_DC, x, y, lpSize);
		return FALSE;
	}
	inline BOOL CDC::SetWindowExtEx(SIZE size, LPSIZE lpSizeRet) const
	{
		if (*m_DC != NULL)
			return SetWindowExtEx(size.cx, size.cy, lpSizeRet);
		return FALSE;
	}
	inline BOOL CDC::ScaleWindowExtEx(int xNum, int xDenom, int yNum, int yDenom, LPSIZE lpSize) const
	{
		if (*m_DC != NULL)
			return ::ScaleWindowExtEx(*m_DC, xNum, xDenom, yNum, yDenom, lpSize);
		return FALSE;
	}
#endif

	// Printer Functions
	inline int CDC::StartDoc(LPDOCINFO lpDocInfo) const
	{
		return ::StartDoc(*m_DC, lpDocInfo);
	}
	inline int CDC::EndDoc() const
	{
		return ::EndDoc(*m_DC);
	}
	inline int CDC::StartPage() const
	{
		return ::StartPage(*m_DC);
	}
	inline int CDC::EndPage() const
	{
		return ::EndPage(*m_DC);
	}
	inline int CDC::AbortDoc() const
	{
		return ::AbortDoc(*m_DC);
	}
	inline int CDC::SetAbortProc(BOOL (CALLBACK* lpfn)(HDC, int)) const
	{
		return ::SetAbortProc(*m_DC, lpfn);
	}

	// Text Functions
	inline BOOL CDC::ExtTextOut( int x, int y, UINT nOptions, const RECT& rc, LPCTSTR lpszString, UINT nCount, LPINT lpDxWidths ) const
	{
		// Draws text using the currently selected font, background color, and text color
		return ::ExtTextOut(*m_DC, x, y, nOptions, &rc, lpszString, nCount, lpDxWidths );
	}

	inline int CDC::DrawText( LPCTSTR lpszString, int nCount, const RECT& rc, UINT nFormat ) const
	{
		// Draws formatted text in the specified rectangle
		return ::DrawText(*m_DC, lpszString, nCount, (LPRECT)&rc, nFormat );
	}
	inline UINT CDC::GetTextAlign( ) const
	{
		// Retrieves the text-alignment setting
		// Values: TA_BASELINE, TA_BOTTOM, TA_TOP, TA_CENTER, TA_LEFT, TA_RIGHT, TA_RTLREADING, TA_NOUPDATECP, TA_UPDATECP
		return ::GetTextAlign( *m_DC );
	}
	inline UINT CDC::SetTextAlign( UINT nFlags ) const
	{
		// Sets the text-alignment setting
		// Values: TA_BASELINE, TA_BOTTOM, TA_TOP, TA_CENTER, TA_LEFT, TA_RIGHT, TA_RTLREADING, TA_NOUPDATECP, TA_UPDATECP
		return ::SetTextAlign( *m_DC, nFlags );
	}
	inline int CDC::GetTextFace( int nCount, LPTSTR lpszFacename ) const
	{
		// Retrieves the typeface name of the font that is selected into the device context
		return ::GetTextFace( *m_DC, nCount, lpszFacename );
	}
	inline BOOL CDC::GetTextMetrics( TEXTMETRIC& Metrics ) const
	{
		// Fills the specified buffer with the metrics for the currently selected font
		return ::GetTextMetrics( *m_DC, &Metrics );
	}
	inline COLORREF CDC::GetBkColor( ) const
	{
		// Returns the current background color
		return ::GetBkColor( *m_DC );
	}
	inline COLORREF CDC::SetBkColor( COLORREF crColor ) const
	{
		// Sets the current background color to the specified color value
		return ::SetBkColor(*m_DC, crColor );
	}
	inline COLORREF CDC::GetTextColor( ) const
	{
		// Retrieves the current text color
		return ::GetTextColor( *m_DC);
	}
	inline COLORREF CDC::SetTextColor( COLORREF crColor ) const
	{
		// Sets the current text color
		return ::SetTextColor( *m_DC, crColor );
	}
	inline int CDC::GetBkMode( ) const
	{
		// returns the current background mix mode (OPAQUE or TRANSPARENT)
		return ::GetBkMode( *m_DC );
	}
	inline int CDC::SetBkMode( int iBkMode ) const
	{
		// Sets the current background mix mode (OPAQUE or TRANSPARENT)
		return ::SetBkMode( *m_DC, iBkMode);
	}
#ifndef _WIN32_WCE
	inline CSize CDC::GetTextExtentPoint( LPCTSTR lpszString, int nCount ) const
	{
		// Computes the width and height of the specified string of text
		CSize sz;
		::GetTextExtentPoint(*m_DC, lpszString, nCount, &sz );
		return sz;
	}
	inline BOOL CDC::TextOut( int x, int y, LPCTSTR lpszString, int nCount ) const
	{
		// Writes a character string at the specified location
		return ::TextOut( *m_DC, x, y, lpszString, nCount );
	}
	inline int CDC::DrawTextEx( LPTSTR lpszString, int nCount, const RECT& rc, UINT nFormat, const DRAWTEXTPARAMS& DTParams ) const
	{
		// Draws formatted text in the specified rectangle with more formatting options
		return ::DrawTextEx(*m_DC, lpszString, nCount, (LPRECT)&rc, nFormat, (LPDRAWTEXTPARAMS)&DTParams );
	}
	inline CSize CDC::TabbedTextOut( int x, int y, LPCTSTR lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin ) const
	{
		// Writes a character string at a specified location, expanding tabs to the values specified in an array of tab-stop positions
		DWORD dwSize = ::TabbedTextOut(*m_DC, x, y, lpszString, nCount, nTabPositions, lpnTabStopPositions, nTabOrigin );
		CSize sz(dwSize);
		return sz;
	}
	inline CSize CDC::GetTabbedTextExtent( LPCTSTR lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions ) const
	{
		// Computes the width and height of a character string
		DWORD dwSize = ::GetTabbedTextExtent(*m_DC, lpszString, nCount, nTabPositions, lpnTabStopPositions );
		CSize sz(dwSize);
		return sz;
	}
	inline BOOL CDC::GrayString( HBRUSH hBrush, GRAYSTRINGPROC lpOutputFunc, LPARAM lpData, int nCount, int x, int y, int nWidth, int nHeight ) const
	{
		// Draws gray text at the specified location
		return ::GrayString(*m_DC, hBrush, lpOutputFunc, lpData, nCount, x, y, nWidth, nHeight );
	}
	inline int CDC::SetTextJustification( int nBreakExtra, int nBreakCount  ) const
	{
		// Specifies the amount of space the system should add to the break characters in a string of text
		return ::SetTextJustification( *m_DC, nBreakExtra, nBreakCount  );
	}
	inline int CDC::GetTextCharacterExtra( ) const
	{
		// Retrieves the current intercharacter spacing for the device context
		return ::GetTextCharacterExtra( *m_DC );
	}
	inline int CDC::SetTextCharacterExtra( int nCharExtra ) const
	{
		// Sets the intercharacter spacing
		return ::SetTextCharacterExtra( *m_DC, nCharExtra );
	}
	inline CSize CDC::GetTextExtentPoint32( LPCTSTR lpszString, int nCount ) const
	{
		// Computes the width and height of the specified string of text
		CSize sz;
		::GetTextExtentPoint32(*m_DC, lpszString, nCount, &sz );
		return sz;
	}
#endif



	/////////////////////////////////////////////////////////////////
	// Definitions for some global functions in the Win32xx namespace
	//

#ifndef _WIN32_WCE
	inline void TintBitmap( HBITMAP hbmSource, int cRed, int cGreen, int cBlue )
	// Modifies the colour of the supplied Device Dependant Bitmap, by the colour
	// correction values specified. The correction values can range from -255 to +255.
	// This function gains its speed by accessing the bitmap colour information
	// directly, rather than using GetPixel/SetPixel.
	{
		// Create our LPBITMAPINFO object
		CBitmapInfoPtr pbmi(hbmSource);
		pbmi->bmiHeader.biBitCount = 24;

		// Create the reference DC for GetDIBits to use
		CDC MemDC = CreateCompatibleDC(NULL);

		// Use GetDIBits to create a DIB from our DDB, and extract the colour data
		MemDC.GetDIBits(hbmSource, 0, pbmi->bmiHeader.biHeight, NULL, pbmi, DIB_RGB_COLORS);
		std::vector<byte> vBits(pbmi->bmiHeader.biSizeImage, 0);
		byte* pByteArray = &vBits.front();

		MemDC.GetDIBits(hbmSource, 0, pbmi->bmiHeader.biHeight, pByteArray, pbmi, DIB_RGB_COLORS);
		UINT nWidthBytes = pbmi->bmiHeader.biSizeImage/pbmi->bmiHeader.biHeight;

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
		for (int Row=0; Row < pbmi->bmiHeader.biHeight; Row++)
		{
			xOffset = 0;

			for (int Column=0; Column < pbmi->bmiHeader.biWidth; Column++)
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
				xOffset += pbmi->bmiHeader.biBitCount >> 3;
			}

			// Increment vertical offset
			yOffset += nWidthBytes;
		}

		// Save the modified colour back into our source DDB
		MemDC.SetDIBits(hbmSource, 0, pbmi->bmiHeader.biHeight, pByteArray, pbmi, DIB_RGB_COLORS);
	}

	inline void GrayScaleBitmap( HBITMAP hbmSource )
	{
		// Create our LPBITMAPINFO object
		CBitmapInfoPtr pbmi(hbmSource);

		// Create the reference DC for GetDIBits to use
		CDC MemDC = CreateCompatibleDC(NULL);

		// Use GetDIBits to create a DIB from our DDB, and extract the colour data
		MemDC.GetDIBits(hbmSource, 0, pbmi->bmiHeader.biHeight, NULL, pbmi, DIB_RGB_COLORS);
		std::vector<byte> vBits(pbmi->bmiHeader.biSizeImage, 0);
		byte* pByteArray = &vBits.front();

		MemDC.GetDIBits(hbmSource, 0, pbmi->bmiHeader.biHeight, pByteArray, pbmi, DIB_RGB_COLORS);
		UINT nWidthBytes = pbmi->bmiHeader.biSizeImage/pbmi->bmiHeader.biHeight;

		int yOffset = 0;
		int xOffset;
		int Index;

		for (int Row=0; Row < pbmi->bmiHeader.biHeight; Row++)
		{
			xOffset = 0;

			for (int Column=0; Column < pbmi->bmiHeader.biWidth; Column++)
			{
				// Calculate Index
				Index = yOffset + xOffset;

				BYTE byGray = (BYTE) ((pByteArray[Index] + pByteArray[Index+1]*6 + pByteArray[Index+2] *3)/10);
				pByteArray[Index]   = byGray;
				pByteArray[Index+1] = byGray;
				pByteArray[Index+2] = byGray;

				// Increment the horizontal offset
				xOffset += pbmi->bmiHeader.biBitCount >> 3;
			}

			// Increment vertical offset
			yOffset += nWidthBytes;
		}

		// Save the modified colour back into our source DDB
		MemDC.SetDIBits(hbmSource, 0, pbmi->bmiHeader.biHeight, pByteArray, pbmi, DIB_RGB_COLORS);
	}

	inline HIMAGELIST CreateDisabledImageList( HIMAGELIST himlNormal )
	// Returns a greyed image list, created from hImageList
	{
		int cx, cy;
		int nCount = ImageList_GetImageCount(himlNormal);
		if (0 == nCount)
			return NULL;

		ImageList_GetIconSize(himlNormal, &cx, &cy);

		// Create the disabled ImageList
		HIMAGELIST himlDisabled = ImageList_Create(cx, cy, ILC_COLOR24 | ILC_MASK, nCount, 0);

		// Process each image in the ImageList
		for (int i = 0 ; i < nCount; ++i)
		{
			CDC DesktopDC = ::GetDC(NULL);
			CDC MemDC = ::CreateCompatibleDC(NULL);
			MemDC.CreateCompatibleBitmap(DesktopDC, cx, cx);
			CRect rc;
			rc.SetRect(0, 0, cx, cx);

			// Set the mask color to grey for the new ImageList
			COLORREF crMask = RGB(200, 199, 200);
			if ( GetDeviceCaps(DesktopDC, BITSPIXEL) < 24)
			{
				HPALETTE hPal = (HPALETTE)GetCurrentObject(DesktopDC, OBJ_PAL);
				UINT Index = GetNearestPaletteIndex(hPal, crMask);
				if (Index != CLR_INVALID) crMask = PALETTEINDEX(Index);
			}

			MemDC.SolidFill(crMask, rc);

			// Draw the image on the memory DC
			ImageList_SetBkColor(himlNormal, crMask);
			ImageList_Draw(himlNormal, i, MemDC, 0, 0, ILD_NORMAL);

			// Convert colored pixels to gray
			for (int x = 0 ; x < cx; ++x)
			{
				for (int y = 0; y < cy; ++y)
				{
					COLORREF clr = ::GetPixel(MemDC, x, y);

					if (clr != crMask)
					{
						BYTE byGray = (BYTE) (95 + (GetRValue(clr) *3 + GetGValue(clr)*6 + GetBValue(clr))/20);
						MemDC.SetPixel(x, y, RGB(byGray, byGray, byGray));
					}

				}
			}

			// Detach the bitmap so we can use it.
			HBITMAP hbm = MemDC.DetachBitmap();
			ImageList_AddMasked(himlDisabled, hbm, crMask);
			::DeleteObject(hbm);
		}

		return himlDisabled;
	}
#endif


} // namespace Win32xx

#endif // _GDI_H_

