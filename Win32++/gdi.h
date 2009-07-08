// Win32++  Version 6.6 alpha
// Released: ?? July, 2009 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2009  David Nash
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
//  Declaration of the CDC class

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


#ifndef _GDI_H_
#define _GDI_H_

#include "wincore.h"

namespace Win32xx
{

	/////////////////////////////////////////////////////////////////
	// Declarations for some global functions in the Win32xx namespace
	//
	
	void DrawBitmap(HDC hDC, int x, int y, int cx, int cy, HBITMAP hbmImage, COLORREF clrMask);
	void GradientFill(HDC hDC, COLORREF Color1, COLORREF Color2, LPRECT pRc, BOOL bVertical);
	void SolidFill(HDC hDC, COLORREF Color, LPRECT pRc);
	void TintBitmap(HBITMAP hbmSource, int cRed, int cGreen, int cBlue);
	HIMAGELIST CreateDisabledImageList(HIMAGELIST himlNormal);

	
	///////////////////////////////////////////////
	// Declarations for the CDC class
	//
	class CDC
	{
	public:
		CDC( );
		CDC( HDC hDC );
		void operator = ( const HDC hDC );
		virtual ~CDC( );
		HDC GetDC( ) const { return m_hDC; }

		virtual void AttachDC( HDC hDC );
		virtual HDC  DetachDC( );

		// Create and Select Bitmaps
		virtual void AttachBitmap( HBITMAP hBitmap );
		virtual void CreateBitmap( int cx, int cy, UINT Planes, UINT BitsPerPixel, CONST VOID *pvColors );
		virtual void CreateCompatibleBitmap( HDC hDC, int cx, int cy );
		virtual void CreateDIBSection( HDC hdc, CONST BITMAPINFO *pbmi, UINT iUsage, VOID **ppvBits,
										HANDLE hSection, DWORD dwOffset) ;
		virtual HBITMAP DetachBitmap( );

		// Create and Select Brushes
		virtual void AttachBrush( HBRUSH hBrush );
		virtual void CreateDIBPatternBrushPt( const VOID *lpPackedDIB, UINT iUsage );
		virtual void CreatePatternBrush( HBITMAP hbmp );
		virtual void CreateSolidBrush( COLORREF rbg );
		virtual HBRUSH DetachBrush( );

		// Create and Select Fonts
		virtual void AttachFont( HFONT hFont );
		virtual void CreateFontIndirect( CONST LOGFONT* pLF );
		virtual HFONT DetachFont( );

		// Create and Select Pens
		virtual void AttachPen( HPEN hPen );
		virtual void CreatePen( int nStyle, int nWidth, COLORREF rgb );
		virtual void CreatePenIndirect( const LOGPEN *lplgpn );
		virtual HPEN DetachPen( );

		// Create Select Regions
		virtual void AttachClipRegion( HRGN hRegion );
		virtual void CreateEllipticRgn( int left, int top, int right, int bottom );
		virtual void CreateEllipticRgnIndirect( const RECT* prc );
		virtual void CreatePolygonRgn( const POINT* ppt, int cPoints, int fnPolyFillMode );
		virtual void CreatePolyPolygonRgn( const POINT* ppt, const int* pPolyCounts, int nCount, int fnPolyFillMode );
		virtual void CreateRectRgn( int left, int top, int right, int bottom );
		virtual void CreateRectRgnIndirect( const RECT* prc );
		virtual HRGN DetachClipRegion( );
		virtual void ExtCreateRegion( const XFORM *pXform, DWORD nCount, const RGNDATA *pRgnData );

		// Cast the CDC object to a HDC
		operator HDC( ) const { return m_hDC; }

#ifndef _WIN32_WCE
		// Create and Select Bitmaps
		virtual void CreateBitmapIndirect( CONST BITMAP *lpbm );
		virtual void CreateDIBitmap( HDC hdc, CONST BITMAPINFOHEADER *lpbmih, DWORD fdwInit, CONST VOID *lpbInit,
										CONST BITMAPINFO *lpbmi,  UINT fuUsage );
		// Create and Select Brushes
		virtual void CreateBrushIndirect( CONST LOGBRUSH *lplb );
		virtual void CreateDIBPatternBrush( HGLOBAL hglbDIBPacked, UINT fuColorSpec );
		virtual void CreateHatchBrush( int fnStyle, COLORREF rgb );

		// Create and Select Fonts
		virtual void CreateFont( int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight,
  							DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet,
  							DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality,
  							DWORD fdwPitchAndFamily, LPCTSTR lpszFace );
#endif


		// Wrappers for WinAPI functions
		
		// Point and Line Drawing Functions
		CPoint GetCurrentPosition( ) const;
		CPoint MoveTo( int x, int y );
		CPoint MoveTo( POINT pt );
		BOOL LineTo( int x, int y );
		BOOL LineTo( POINT pt );
		BOOL Arc( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 );
		BOOL Arc( LPCRECT lpRect, POINT ptStart, POINT ptEnd );
		BOOL ArcTo( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 );
		BOOL ArcTo( LPCRECT lpRect, POINT ptStart, POINT ptEnd );
		BOOL AngleArc( int x, int y, int nRadius, float fStartAngle, float fSweepAngle );
		int GetArcDirection( ) const;
		int SetArcDirection( int nArcDirection );
		BOOL PolyDraw( const POINT* lpPoints, const BYTE* lpTypes, int nCount );
		BOOL Polyline( LPPOINT lpPoints, int nCount );
		BOOL PolyPolyline( const POINT* lpPoints, const DWORD* lpPolyPoints, int nCount );
		BOOL PolylineTo( const POINT* lpPoints, int nCount );
		BOOL PolyBezier( const POINT* lpPoints, int nCount );
		BOOL PolyBezierTo( const POINT* lpPoints, int nCount );
		COLORREF GetPixel( int x, int y ) const;
		COLORREF GetPixel( POINT pt ) const;
		COLORREF SetPixel( int x, int y, COLORREF crColor );
		COLORREF SetPixel( POINT pt, COLORREF crColor );
		BOOL SetPixelV( int x, int y, COLORREF crColor );
		BOOL SetPixelV( POINT pt, COLORREF crColor );

		// Shape Drawing Functions
		BOOL Chord( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 );
		BOOL Chord( LPCRECT lpRect, POINT ptStart, POINT ptEnd );
		void DrawFocusRect( LPCRECT lpRect );
		BOOL Ellipse( int x1, int y1, int x2, int y2 );
		BOOL Ellipse( LPCRECT lpRect );
		BOOL Pie( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 );
		BOOL Pie( LPCRECT lpRect, POINT ptStart, POINT ptEnd );
		BOOL Polygon( LPPOINT lpPoints, int nCount );
		BOOL PolyPolygon( LPPOINT lpPoints, LPINT lpPolyCounts, int nCount );
		BOOL Rectangle( int x1, int y1, int x2, int y2 );
		BOOL Rectangle( LPCRECT lpRect );

		// Fill and 3D Drawing functions
		BOOL FillRect( LPCRECT lpRect, HBRUSH hbr );
		BOOL FrameRect( LPCRECT lpRect, HBRUSH hbr );
		BOOL InvertRect( LPCRECT lpRect );
		BOOL DrawIcon( int x, int y, HICON hIcon );
		BOOL DrawIcon( POINT point, HICON hIcon );
		BOOL DrawEdge( LPRECT lpRect, UINT nEdge, UINT nFlags );
		BOOL DrawFrameControl( LPRECT lpRect, UINT nType, UINT nState );

		// Bitmap Functions
		int GetDIBits( HBITMAP hbmp, UINT uStartScan, UINT cScanLines, LPVOID lpvBits, LPBITMAPINFO lpbi, UINT uUsage );
		int StretchDIBits( int XDest, int YDest, int nDestWidth, int nDestHeight, int XSrc, int YSrc, int nSrcWidth, 
			           int nSrcHeight, CONST VOID *lpBits, CONST BITMAPINFO *lpBitsInfo, UINT iUsage, DWORD dwRop );
		int GetStretchBltMode( );
		int SetStretchBltMode( int iStretchMode );
		BOOL PatBlt( int x, int y, int nWidth, int nHeight, DWORD dwRop );
		BOOL BitBlt( int x, int y, int nWidth, int nHeight, HDC hSrcDC, int xSrc, int ySrc, DWORD dwRop );
		BOOL StretchBlt( int x, int y, int nWidth, int nHeight, HDC hSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop );
		BOOL FloodFill( int x, int y, COLORREF crColor );
		BOOL ExtFloodFill( int x, int y, COLORREF crColor, UINT nFillType );

		// Text Functions
		BOOL TextOut( int x, int y, LPCTSTR lpszString, int nCount );
		BOOL ExtTextOut( int x, int y, UINT nOptions, LPCRECT lpRect, LPCTSTR lpszString, UINT nCount, LPINT lpDxWidths );
		CSize TabbedTextOut( int x, int y, LPCTSTR lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin );	
		int DrawText( LPCTSTR lpszString, int nCount, LPRECT lpRect, UINT nFormat );		
		CSize GetTextExtentPoint32( LPCTSTR lpszString, int nCount ) const;			
		CSize GetTabbedTextExtent( LPCTSTR lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions ) const;			
		BOOL GrayString( HBRUSH hBrush, GRAYSTRINGPROC lpOutputFunc, LPARAM lpData, int nCount, int x, int y, int nWidth, int nHeight );
		UINT GetTextAlign( ) const;
		UINT SetTextAlign( UINT nFlags );
		int GetTextFace( int nCount, LPTSTR lpszFacename ) const;
		BOOL GetTextMetrics( LPTEXTMETRIC lpMetrics ) const;
		int SetTextJustification( int nBreakExtra, int nBreakCount );
		int GetTextCharacterExtra( ) const;
		int SetTextCharacterExtra( int nCharExtra );


	private:
		CDC(const CDC&);				// Disable copy construction
		CDC& operator = (const CDC&);	// Disable assignment operator
		HDC m_hDC;
		HBITMAP m_hBitmapOld;
		HBRUSH m_hBrushOld;
		HFONT m_hFontOld;
		HPEN m_hPenOld;
		HRGN m_hRgnOld;
	};
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{

	///////////////////////////////////////////////
	// Definitions of the CDC class
	//
	inline CDC::CDC() : m_hDC(0), m_hBitmapOld(0), m_hBrushOld(0), m_hFontOld(0), 
		            m_hPenOld(0), m_hRgnOld(0)
	{
	}

	inline CDC::CDC(HDC hDC) : m_hDC(0), m_hBitmapOld(0), m_hBrushOld(0), m_hFontOld(0), 
		            m_hPenOld(0), m_hRgnOld(0)
	{
		// This constructor assigns an existing HDC to the CDC
		// The HDC WILL be released or deleted when the CDC object is destroyed
		if (!hDC) throw CWinException(_T("Can't assign a NULL hDC"));

		m_hDC = hDC;

		// Note: this constructor permits a call like this:
		// CDC MyCDC = SomeHDC;
		//  or
		// CDC MyCDC = ::CreateCompatibleDC(SomeHDC);
		//  or
		// CDC MyCDC = ::GetDC(SomeHWND);
	}

	inline void CDC::operator = (const HDC hDC)
	{
		AttachDC(hDC);
	}

	inline CDC::~CDC()
	{
		if (m_hDC)
		{
			// Delete any GDI objects belonging to this CDC
			if (m_hPenOld)    ::DeleteObject(::SelectObject(m_hDC, m_hPenOld));
			if (m_hBrushOld)  ::DeleteObject(::SelectObject(m_hDC, m_hBrushOld));
			if (m_hBitmapOld) ::DeleteObject(::SelectObject(m_hDC, m_hBitmapOld));
			if (m_hFontOld)	  ::DeleteObject(::SelectObject(m_hDC, m_hFontOld));
			if (m_hRgnOld)    ::DeleteObject(m_hRgnOld);

			// We need to release a Window DC, and delete a memory DC
	#ifndef _WIN32_WCE
			HWND hwnd = ::WindowFromDC(m_hDC);
			if (hwnd) ::ReleaseDC(hwnd, m_hDC);
			else      ::DeleteDC(m_hDC);
	#else
			::DeleteDC(m_hDC);
	#endif
			
		}
	}

	inline void CDC::AttachDC(HDC hDC)
	{
		if (m_hDC) throw CWinException(_T("Device Context ALREADY assigned"));
		if (!hDC) throw CWinException(_T("Can't attach a NULL hDC"));

		m_hDC = hDC;
	}


	inline HDC CDC::DetachDC()
	{
		if (!m_hDC) throw CWinException(_T("No HDC assigned to this CDC"));
		if (m_hPenOld)    ::DeleteObject(::SelectObject(m_hDC, m_hPenOld));
		if (m_hBrushOld)  ::DeleteObject(::SelectObject(m_hDC, m_hBrushOld));
		if (m_hBitmapOld) ::DeleteObject(::SelectObject(m_hDC, m_hBitmapOld));
		if (m_hFontOld)	  ::DeleteObject(::SelectObject(m_hDC, m_hFontOld));

		HDC hDC = m_hDC;

		m_hDC = NULL;
		return hDC;
	}

	// Bitmap functions
	inline void CDC::AttachBitmap(HBITMAP hBitmap)
	{
		// Use this to attach an existing bitmap.
		// The bitmap will be deleted for you, unless its detached

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (!hBitmap) throw CWinException(_T("Can't attach a NULL HBITMAP"));

		// Delete any existing bitmap
		if (m_hBitmapOld) ::DeleteObject(::SelectObject(m_hDC, m_hBitmapOld));

		m_hBitmapOld = (HBITMAP)::SelectObject(m_hDC, hBitmap);
	}

	inline void CDC::CreateCompatibleBitmap(HDC hDC, int cx, int cy)
	{
		// Creates a compatible bitmap and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBitmapOld) ::DeleteObject(::SelectObject(m_hDC, m_hBitmapOld));

		HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, cx, cy);
		if (!hBitmap) throw CWinException(_T("CreateCompatibleBitmap failed"));

		m_hBitmapOld = (HBITMAP)::SelectObject(m_hDC, hBitmap);
	}

	inline void CDC::CreateBitmap(int cx, int cy, UINT Planes, UINT BitsPerPixel, CONST VOID *pvColors)
	{
		// Creates a bitmap and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBitmapOld) ::DeleteObject(::SelectObject(m_hDC, m_hBitmapOld));

		HBITMAP hBitmap = ::CreateBitmap(cx, cy, Planes, BitsPerPixel, pvColors);
		if (!hBitmap) throw CWinException(_T("CreateBitmap failed"));

		m_hBitmapOld = (HBITMAP)::SelectObject(m_hDC, hBitmap);
	}

#ifndef _WIN32_WCE
	inline void CDC::CreateBitmapIndirect(CONST BITMAP *lpbm)
	{
		// Creates a bitmap and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBitmapOld) ::DeleteObject(::SelectObject(m_hDC, m_hBitmapOld));

		HBITMAP hBitmap = ::CreateBitmapIndirect(lpbm);
		if (!hBitmap) throw CWinException(_T("CreateBitmap failed"));

		m_hBitmapOld = (HBITMAP)::SelectObject(m_hDC, hBitmap);
	}

	inline void CDC::CreateDIBitmap(HDC hdc, CONST BITMAPINFOHEADER *lpbmih, DWORD fdwInit, CONST VOID *lpbInit,
										CONST BITMAPINFO *lpbmi,  UINT fuUsage)
	{
		// Creates a bitmap and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBitmapOld) ::DeleteObject(::SelectObject(m_hDC, m_hBitmapOld));

		HBITMAP hBitmap = ::CreateDIBitmap(hdc, lpbmih, fdwInit, lpbInit, lpbmi, fuUsage);
		if (!hBitmap) throw CWinException(_T("CreateDIBitmap failed"));

		m_hBitmapOld = (HBITMAP)::SelectObject(m_hDC, hBitmap);
	}
#endif

	inline void CDC::CreateDIBSection(HDC hdc, CONST BITMAPINFO *pbmi, UINT iUsage, VOID **ppvBits,
										HANDLE hSection, DWORD dwOffset)
	{
		// Creates a bitmap and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBitmapOld)::DeleteObject(::SelectObject(m_hDC, m_hBitmapOld));

		HBITMAP hBitmap = ::CreateDIBSection(hdc, pbmi, iUsage, ppvBits, hSection, dwOffset);
		if (!hBitmap) throw CWinException(_T("CreateDIBSection failed"));

		m_hBitmapOld = (HBITMAP)::SelectObject(m_hDC, hBitmap);
	}

	inline HBITMAP CDC::DetachBitmap()
	{
		// Use this to detach the bitmap from the HDC.
		// You are then responible for deleting the detached bitmap

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (!m_hBitmapOld) throw CWinException(_T("No Bitmap to detach"));

		HBITMAP hBitmap = (HBITMAP)::SelectObject(m_hDC, m_hBitmapOld);
		m_hBitmapOld = NULL;
		return hBitmap;
	}

	// Brush functions
	inline void CDC::AttachBrush(HBRUSH hBrush)
	{
		// Use this to attach an existing brush.
		// The brush will be deleted for you, unless its detached

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (!hBrush) throw CWinException(_T("Can't attach a NULL HBRUSH"));
		if (m_hBrushOld) ::DeleteObject(::SelectObject(m_hDC, m_hBrushOld));
		m_hBrushOld = (HBRUSH)::SelectObject(m_hDC, hBrush);
	}

#ifndef _WIN32_WCE
	inline void CDC::CreateBrushIndirect(CONST LOGBRUSH *lplb)
	{
		// Creates the brush and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBrushOld) ::DeleteObject(::SelectObject(m_hDC, m_hBrushOld));

		HBRUSH hBrush = ::CreateBrushIndirect(lplb);
		if (!hBrush) throw CWinException(_T("CreateBrusIndirect failed"));

		m_hBrushOld = (HBRUSH)::SelectObject(m_hDC, hBrush);
	}

	inline void CDC::CreateDIBPatternBrush(HGLOBAL hglbDIBPacked, UINT fuColorSpec)
	{
		// Creates the brush and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBrushOld) ::DeleteObject(::SelectObject(m_hDC, m_hBrushOld));

		HBRUSH hBrush = ::CreateDIBPatternBrush(hglbDIBPacked, fuColorSpec);
		if (!hBrush) throw CWinException(_T("CreateDIBPatternBrush failed"));

		m_hBrushOld = (HBRUSH)::SelectObject(m_hDC, hBrush);
	}

	inline void CDC::CreateHatchBrush(int fnStyle, COLORREF rgb)
	{
		// Creates the brush and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBrushOld) ::DeleteObject(::SelectObject(m_hDC, m_hBrushOld));

		HBRUSH hBrush = ::CreateHatchBrush(fnStyle, rgb);
		if (!hBrush) throw CWinException(_T("CreateHatchBrush failed"));

		m_hBrushOld = (HBRUSH)::SelectObject(m_hDC, hBrush);
	}
#endif

	inline void CDC::CreateDIBPatternBrushPt(CONST VOID *lpPackedDIB, UINT iUsage)
	{
		// Creates the brush and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBrushOld) ::DeleteObject(::SelectObject(m_hDC, m_hBrushOld));

		HBRUSH hBrush = ::CreateDIBPatternBrushPt(lpPackedDIB, iUsage);
		if (!hBrush) throw CWinException(_T("CreateDIBPatternPrushPt failed"));

		m_hBrushOld = (HBRUSH)::SelectObject(m_hDC, hBrush);
	}

	inline void CDC::CreatePatternBrush(HBITMAP hbmp)
	{
		// Creates the brush and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBrushOld) ::DeleteObject(::SelectObject(m_hDC, m_hBrushOld));

		HBRUSH hBrush = ::CreatePatternBrush(hbmp);
		if (!hBrush) throw CWinException(_T("CreatePatternBrush failed"));

		m_hBrushOld = (HBRUSH)::SelectObject(m_hDC, hBrush);
	}

	inline void CDC::CreateSolidBrush(COLORREF rgb)
	{
		// Creates the brush and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBrushOld) ::DeleteObject(::SelectObject(m_hDC, m_hBrushOld));

		HBRUSH hBrush = ::CreateSolidBrush(rgb);
		if (!hBrush) throw CWinException(_T("CreateSolidBrush failed"));

		m_hBrushOld = (HBRUSH)::SelectObject(m_hDC, hBrush);
	}

	inline HBRUSH CDC::DetachBrush()
	{
		// Use this to detach the brush from the HDC.
		// You are then responible for deleting the detached brush

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (!m_hBrushOld) throw CWinException(_T("No Brush to detach"));

		HBRUSH hBrush = (HBRUSH)::SelectObject(m_hDC, m_hBrushOld);
		m_hBrushOld = NULL;
		return hBrush;
	}

	// Font functions
	inline void CDC::AttachFont(HFONT hFont)
	{
		// Use this to attach an existing font.
		// The font will be deleted for you, unless its detached

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (!hFont) throw CWinException(_T("Can't attach a NULL HFONT"));
		if (m_hFontOld) ::DeleteObject(::SelectObject(m_hDC, m_hFontOld));
		m_hFontOld = (HFONT)::SelectObject(m_hDC, hFont);
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
		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hFontOld) ::DeleteObject(::SelectObject(m_hDC, m_hFontOld));

		HFONT hFont = ::CreateFont(nHeight, nWidth, nEscapement, nOrientation, fnWeight,
								fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet,
								fdwOutputPrecision, fdwClipPrecision, fdwQuality,
								fdwPitchAndFamily, lpszFace);

		if (!hFont) throw CWinException(_T("CreateFont failed"));

		m_hFontOld = (HFONT)::SelectObject(m_hDC, hFont);
	}
#endif

	inline void CDC::CreateFontIndirect(CONST LOGFONT* pLF)
	{
		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hFontOld) ::DeleteObject(::SelectObject(m_hDC, m_hFontOld));

		HFONT hFont = ::CreateFontIndirect(pLF);
		if (!hFont) throw CWinException(_T("CreateFontIndirect failed"));

		m_hFontOld = (HFONT)::SelectObject(m_hDC, hFont);
	}

	inline HFONT CDC::DetachFont()
	{
		// Use this to detach the font from the HDC.
		// You are then responible for deleting the detached font

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (!m_hFontOld) throw CWinException(_T("No Font to detach"));

		HFONT hFont = (HFONT)::SelectObject(m_hDC, m_hFontOld);
		m_hFontOld = NULL;
		return hFont;
	}

	// Pen functions
	inline void CDC::AttachPen(HPEN hPen)
	{
		// Use this to attach an existing pen.
		// The pen will be deleted for you, unless its detached

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (!hPen) throw CWinException(_T("Can't attach a NULL HPEN"));
		if (m_hPenOld) ::DeleteObject(::SelectObject(m_hDC, m_hPenOld));
		m_hPenOld = (HPEN)::SelectObject(m_hDC, hPen);
	}

	inline void CDC::CreatePen(int nStyle, int nWidth, COLORREF rgb)
	{
		// Creates the pen and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hPenOld) ::DeleteObject(::SelectObject(m_hDC, m_hPenOld));

		HPEN hPen = ::CreatePen(nStyle, nWidth, rgb);
		if (!hPen) throw CWinException(_T("CreatePen failed"));

		m_hPenOld = (HPEN)::SelectObject(m_hDC, hPen);
	}

	inline void CDC::CreatePenIndirect(const LOGPEN *lplgpn)
	{
		// Creates the pen and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hPenOld) ::DeleteObject(::SelectObject(m_hDC, m_hPenOld));

		HPEN hPen = ::CreatePenIndirect(lplgpn);
		if (!hPen) throw CWinException(_T("CreatePenIndirect failed"));

		m_hPenOld = (HPEN)::SelectObject(m_hDC, hPen);
	}

	inline HPEN CDC::DetachPen()
	{
		// Use this to detach the pen from the HDC.
		// You are then responible for deleting the detached pen

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (!m_hPenOld) throw CWinException(_T("No Pen to detach"));

		HPEN hPen = (HPEN)::SelectObject(m_hDC, m_hPenOld);
		m_hPenOld = NULL;
		return hPen;
	}

	// Region functions
	inline void CDC::AttachClipRegion(HRGN hRegion)
	{		
		// Use this to attach an existing region.
		// The region will be deleted for you, unless its detached
		// Note: The shape of a region cannot be changed while it is attached to a DC

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (!hRegion) throw CWinException(_T("Can't attach a NULL HRGN"));
		if (m_hRgnOld) ::DeleteObject(m_hRgnOld);

		::SelectClipRgn(m_hDC, hRegion);
		m_hRgnOld = hRegion;
	}

	inline void CDC::CreateEllipticRgn(int left, int top, int right, int bottom)
	{
		// Creates the ellyiptical region from the bounding rectangle co-ordinates
		// and selects it into the device context
		// The region will be deleted for you, unless its detached
		// Note: The shape of a region cannot be changed while it is attached to a DC

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hRgnOld) ::DeleteObject(m_hRgnOld);

		HRGN hRgn = ::CreateEllipticRgn(left, top, right, bottom);
		if (!hRgn) throw CWinException(_T("CreateEllipticRgn failed"));

		::SelectClipRgn(m_hDC, hRgn);
		m_hRgnOld = hRgn;
	}

	inline void CDC::CreateEllipticRgnIndirect(const RECT* prc)
	{
		// Creates the ellyiptical region from the bounding rectangle co-ordinates
		// and selects it into the device context
		// The region will be deleted for you, unless its detached
		// Note: The shape of a region cannot be changed while it is attached to a DC

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hRgnOld) ::DeleteObject(m_hRgnOld);

		HRGN hRgn = ::CreateEllipticRgnIndirect(prc);
		if (!hRgn) throw CWinException(_T("CreateEllipticRgnIndirect failed"));

		::SelectClipRgn(m_hDC, hRgn);
		m_hRgnOld = hRgn;
	}

	inline void CDC::CreatePolygonRgn(const POINT* ppt, int cPoints, int fnPolyFillMode)
	{
		// Creates the polygon region from the array of points and selects it into 
		// the device context. The polygon is presumed closed
		// The region will be deleted for you, unless its detached
		// Note: The shape of a region cannot be changed while it is attached to a DC

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hRgnOld) ::DeleteObject(m_hRgnOld);

		HRGN hRgn = ::CreatePolygonRgn(ppt, cPoints, fnPolyFillMode);
		if (!hRgn) throw CWinException(_T("CreatePolygonRgn failed"));

		::SelectClipRgn(m_hDC, hRgn);
		m_hRgnOld = hRgn;
	}

	inline void CDC::CreatePolyPolygonRgn(const POINT* ppt, const int* pPolyCounts, int nCount, int fnPolyFillMode)
	{
		// Creates the polygon region from a series of polygons.The polygons can overlap.
		// The region will be deleted for you, unless its detached
		// Note: The shape of a region cannot be changed while it is attached to a DC

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hRgnOld) ::DeleteObject(m_hRgnOld);

		HRGN hRgn = ::CreatePolyPolygonRgn(ppt, pPolyCounts, nCount, fnPolyFillMode);
		if (!hRgn) throw CWinException(_T("CreatePolyPolygonRgn failed"));

		::SelectClipRgn(m_hDC, hRgn);
		m_hRgnOld = hRgn;
	}
		
	inline void CDC::CreateRectRgn(int left, int top, int right, int bottom)
	{
		// Creates a rectangular region from the rectangle co-ordinates.
		// The region will be deleted for you, unless its detached
		// Note: The shape of a region cannot be changed while it is attached to a DC

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hRgnOld) ::DeleteObject(m_hRgnOld);

		HRGN hRgn = ::CreateRectRgn(left, top, right, bottom);
		if (!hRgn) throw CWinException(_T("CreateRectRgn failed"));

		::SelectClipRgn(m_hDC, hRgn);
		m_hRgnOld = hRgn;
	}

	inline void CDC::CreateRectRgnIndirect(const RECT* prc)
	{
		// Creates a rectangular region from the rectangle co-ordinates.
		// The region will be deleted for you, unless its detached
		// Note: The shape of a region cannot be changed while it is attached to a DC

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hRgnOld) ::DeleteObject(m_hRgnOld);

		HRGN hRgn = ::CreateRectRgnIndirect(prc);
		if (!hRgn) throw CWinException(_T("CreateRectRgnIndirect failed"));

		::SelectClipRgn(m_hDC, hRgn);
		m_hRgnOld = hRgn;
	}

	inline void CDC::ExtCreateRegion(const XFORM *pXform, DWORD nCount, const RGNDATA *pRgnData)
	{
		// Creates a region from the specified region data and tranformation data.
		// The region will be deleted for you, unless its detached
		// Notes: The shape of a region cannot be changed while it is attached to a DC
		//        GetRegionData can be used to get a region's data
		//        If the XFROM pointer is NULL, the identity transformation is used. 

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hRgnOld) ::DeleteObject(m_hRgnOld);

		HRGN hRgn = ::ExtCreateRegion(pXform, nCount, pRgnData);
		if (!hRgn) throw CWinException(_T("ExtCreateRegion failed"));

		::SelectClipRgn(m_hDC, hRgn);
		m_hRgnOld = hRgn;
	}



	inline HRGN CDC::DetachClipRegion()
	{
		// Use this to detach the region from the HDC.
		// You are then responible for deleting the detached region

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (!m_hRgnOld) throw CWinException(_T("No Region to detach"));

		::SelectClipRgn(m_hDC, NULL);
		HRGN hRgn = m_hRgnOld;
		m_hRgnOld = NULL;
		return hRgn;
	}



	/////////////////////////////////////////////////////////////////
	// Definitions for some global functions in the Win32xx namespace
	//

	inline void DrawBitmap(HDC hDC, int x, int y, int cx, int cy, HBITMAP hbmImage, COLORREF clrMask)
	// Draws the specified bitmap to the specified DC using the mask colour provided as the transparent colour
	// Suitable for use with a Window DC or a memory DC
	{
		// Create the Image memory DC
		CDC dcImage = CreateCompatibleDC(hDC);
		dcImage.AttachBitmap(hbmImage);
			
		// Create the Mask memory DC
		HBITMAP hbmMask = CreateBitmap(cx, cy, 1, 1, NULL);
		CDC dcMask = CreateCompatibleDC(hDC);
		dcMask.AttachBitmap(hbmMask);
		SetBkColor(dcImage, clrMask);
		BitBlt(dcMask, 0, 0, cx, cy, dcImage, 0, 0, SRCCOPY);

		// Mask the image to the DC provided
		BitBlt(hDC, x, y, cx, cy, dcImage, 0, 0, SRCINVERT);
		BitBlt(hDC, x, y, cx, cy, dcMask, 0, 0, SRCAND);
		BitBlt(hDC, x, y, cx, cy, dcImage, 0, 0, SRCINVERT);

		// Detach the bitmap before the dcImage is destroyed
		dcImage.DetachBitmap();
	}

	inline void GradientFill(HDC hDC, COLORREF Color1, COLORREF Color2, LPRECT pRc, BOOL bVertical)
	// A simple but efficient Gradient Filler compatible with all Windows operating systems
	{
		int Width = pRc->right - pRc->left;
		int Height = pRc->bottom - pRc->top;

		int r1 = GetRValue(Color1);
		int g1 = GetGValue(Color1);
		int b1 = GetBValue(Color1);

		int r2 = GetRValue(Color2);
		int g2 = GetGValue(Color2);
		int b2 = GetBValue(Color2);

		COLORREF OldBkColor = ::GetBkColor(hDC);

		if (bVertical)
		{
			for(int i=0; i < Width; ++i)
			{
				int r = r1 + (i * (r2-r1) / Width);
				int g = g1 + (i * (g2-g1) / Width);
				int b = b1 + (i * (b2-b1) / Width);
				::SetBkColor(hDC, RGB(r, g, b));
				CRect line( i + pRc->left, pRc->top, i + 1 + pRc->left, pRc->top+Height);
				::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &line, NULL, 0, NULL);
			}
		}
		else
		{
			for(int i=0; i < Height; ++i)
			{
				int r = r1 + (i * (r2-r1) / Height);
				int g = g1 + (i * (g2-g1) / Height);
				int b = b1 + (i * (b2-b1) / Height);
				::SetBkColor(hDC, RGB(r, g, b));
				CRect line( pRc->left, i + pRc->top, pRc->left+Width, i + 1 +pRc->top);
				::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &line, NULL, 0, NULL);
			}
		}

		::SetBkColor(hDC, OldBkColor);
	}

	inline void SolidFill(HDC hDC, COLORREF Color, LPRECT pRc)
	// Fills a rectangle with a solid color
	{
		COLORREF OldColor = ::SetBkColor(hDC, Color);
		::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, pRc, NULL, 0, NULL);
		::SetBkColor(hDC, OldColor);
	}

	inline void TintBitmap(HBITMAP hbmSource, int cRed, int cGreen, int cBlue)
	// Modifies the colour of the supplied Device Dependant Bitmap, by the colour 
	// correction values specified. The correction values can range from -255 to +255.
	// This function gains its speed by accessing the bitmap colour information
	// directly, rather than using GetPixel/SetPixel.
	{
		// Fill the BITMAP structure with the bitmap information
		BITMAP bmSource;
		::GetObject(hbmSource, sizeof(BITMAP), &bmSource);
		
		// Specify the BITMAPHEADER for the DIB that GetDIBits creates
		BITMAPINFOHEADER bi = {0};
		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biHeight = bmSource.bmHeight;
		bi.biWidth = bmSource.bmWidth;
		bi.biPlanes = 1;
		bi.biBitCount =  24;
		bi.biCompression = BI_RGB;
		int bpPixel = bi.biBitCount >> 3;

		// Create the reference DC for GetDIBits to use
		CDC MemDC = CreateCompatibleDC(NULL);
		
		// Use GetDIBits to create a DIB from our DDB, and extract the colour data
		GetDIBits(MemDC, hbmSource, 0, bi.biHeight, NULL, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
		byte* lpvBits = new byte[bi.biSizeImage];
		if (NULL == lpvBits) throw std::bad_alloc();
		GetDIBits(MemDC, hbmSource, 0, bi.biHeight, lpvBits, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
		UINT nWidthBytes = bi.biSizeImage/bi.biHeight;

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
		for (int Row=0; Row<bmSource.bmHeight; Row++)
		{
			xOffset = 0;

			for (int Column=0; Column<bmSource.bmWidth; Column++)
			{
				// Calculate Index
				Index = yOffset + xOffset;

				// Adjust the colour values
				if (cBlue > 0)
					lpvBits[Index]   = (BYTE)(cBlue + (((lpvBits[Index] *b1)) >>8));
				else if (cBlue < 0)
					lpvBits[Index]   = (BYTE)((lpvBits[Index] *b2) >>8);
				
				if (cGreen > 0)
					lpvBits[Index+1] = (BYTE)(cGreen + (((lpvBits[Index+1] *g1)) >>8));
				else if (cGreen < 0)
					lpvBits[Index+1] = (BYTE)((lpvBits[Index+1] *g2) >>8);
				
				if (cRed > 0)
					lpvBits[Index+2] = (BYTE)(cRed + (((lpvBits[Index+2] *r1)) >>8));
				else if (cRed < 0)
					lpvBits[Index+2] = (BYTE)((lpvBits[Index+2] *r2) >>8);

				// Increment the horizontal offset
				xOffset += bpPixel; 
			}

			// Increment vertical offset
			yOffset += nWidthBytes;
		}  

		// Save the modified colour back into our source DDB
		SetDIBits(MemDC, hbmSource, 0, bi.biHeight, lpvBits, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

		// Cleanup
		delete []lpvBits;
	}
	
	inline HIMAGELIST CreateDisabledImageList(HIMAGELIST himlNormal)
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
 			
			SolidFill(MemDC, crMask, &rc);

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
						::SetPixel(MemDC, x, y, RGB(byGray, byGray, byGray));
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

	// Wrappers for WinAPI functions
	// Point and Line Drawing Functions
	inline CPoint CDC::GetCurrentPosition( ) const
	{
		//  returns the current "MoveToEx" position
		CPoint pt;
		::MoveToEx( m_hDC, 0, 0, &pt );
		::MoveToEx( m_hDC, pt.x, pt.y, NULL);
		return pt;
	}
	inline CPoint CDC::MoveTo( int x, int y )
	{
		// Updates the current position to the specified point
		return ::MoveToEx( m_hDC, x, y, NULL ); 
	}
	inline CPoint CDC::MoveTo(POINT pt )
	{
		// Updates the current position to the specified point
		return ::MoveToEx( m_hDC, pt.x, pt.y, NULL );
	}
	inline BOOL CDC::LineTo( int x, int y )
	{
		// Draws a line from the current position up to, but not including, the specified point
		return ::LineTo( m_hDC, x, y );
	}
	inline BOOL CDC::LineTo( POINT pt )
	{
		// Draws a line from the current position up to, but not including, the specified point
		return ::LineTo( m_hDC, pt.x, pt.y );
	}
	inline BOOL CDC::Arc( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 )
	{
		return ::Arc( m_hDC, x1, y1, x2, y2, x3, y3, x4, y4 );
	}
	inline BOOL CDC::Arc( LPCRECT lpRect, POINT ptStart, POINT ptEnd )
	{
		// Draws an elliptical arc
		return ::Arc( m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom, 
			ptStart.x, ptStart.y, ptEnd.x, ptEnd.y );
	}
	inline BOOL CDC::ArcTo( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 )
	{
		// Draws an elliptical arc
		return ::ArcTo( m_hDC, x1, y1, x2, y2, x3, y3, x4, y4) ;
	}
	inline BOOL CDC::ArcTo( LPCRECT lpRect, POINT ptStart, POINT ptEnd )
	{
		// Draws an elliptical arc
		return ::ArcTo(  m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom,
			ptStart.x, ptStart.y, ptEnd.x, ptEnd.y );
	}
	inline BOOL CDC::AngleArc( int x, int y, int nRadius, float fStartAngle, float fSweepAngle )
	{
		// Draws a line segment and an arc
		return ::AngleArc( m_hDC, x, y, nRadius, fStartAngle, fSweepAngle); 
	}
	inline int CDC::GetArcDirection( ) const
	{
		// Retrieves the current arc direction ( AD_COUNTERCLOCKWISE or AD_CLOCKWISE )
		return ::GetArcDirection( m_hDC );
	}
	inline int CDC::SetArcDirection( int nArcDirection )
	{
		// Sets the current arc direction ( AD_COUNTERCLOCKWISE or AD_CLOCKWISE )
		return ::SetArcDirection( m_hDC, nArcDirection );
	}
	inline BOOL CDC::PolyDraw( const POINT* lpPoints, const BYTE* lpTypes, int nCount )
	{
		// Draws a set of line segments and Bzier curves
		return ::PolyDraw( m_hDC, lpPoints, lpTypes, nCount );
	}
	inline BOOL CDC::Polyline( LPPOINT lpPoints, int nCount )
	{
		// Draws a series of line segments by connecting the points in the specified array
		return ::Polyline( m_hDC, lpPoints, nCount );
	}
	inline BOOL CDC::PolyPolyline( const POINT* lpPoints, const DWORD* lpPolyPoints, int nCount )
	{
		// Draws multiple series of connected line segments
		return ::PolyPolyline( m_hDC, lpPoints, lpPolyPoints, nCount );
	}
	inline BOOL CDC::PolylineTo( const POINT* lpPoints, int nCount )
	{
		// Draws one or more straight lines
		return ::PolylineTo( m_hDC, lpPoints, nCount );
	}
	inline BOOL CDC::PolyBezier( const POINT* lpPoints, int nCount )
	{
		// Draws one or more Bzier curves
		return ::PolyBezier( m_hDC, lpPoints, nCount );
	}
	inline BOOL CDC::PolyBezierTo( const POINT* lpPoints, int nCount )
	{
		// Draws one or more Bzier curves
		return ::PolyBezierTo(m_hDC, lpPoints, nCount );
	}
	inline COLORREF CDC::GetPixel( int x, int y ) const
	{
		// Retrieves the red, green, blue (RGB) color value of the pixel at the specified coordinates
		return ::GetPixel( m_hDC, x, y );
	}
	inline COLORREF CDC::GetPixel( POINT pt ) const
	{
		// Retrieves the red, green, blue (RGB) color value of the pixel at the specified coordinates
		return ::GetPixel( m_hDC, pt.x, pt.y );
	}
	inline COLORREF CDC::SetPixel( int x, int y, COLORREF crColor )
	{
		// Sets the pixel at the specified coordinates to the specified color
		return ::SetPixel( m_hDC, x, y, crColor );
	}
	inline COLORREF CDC::SetPixel( POINT pt, COLORREF crColor )
	{
		// Sets the pixel at the specified coordinates to the specified color
		return ::SetPixel( m_hDC, pt.x, pt.y, crColor );
	}
	inline BOOL CDC::SetPixelV( int x, int y, COLORREF crColor )
	{
		// Sets the pixel at the specified coordinates to the closest approximation of the specified color
		return ::SetPixelV( m_hDC, x, y, crColor );
	}
	inline BOOL CDC::SetPixelV( POINT pt, COLORREF crColor )
	{
		// Sets the pixel at the specified coordinates to the closest approximation of the specified color
		return ::SetPixelV( m_hDC, pt.x, pt.y, crColor );
	}

	// Shape Drawing Functions
	inline BOOL CDC::Chord( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 )
	{
		// Draws a chord (a region bounded by the intersection of an ellipse and a line segment, called a secant)
		return ::Chord( m_hDC, x1, y1, x2, y2, x3, y3, x4, y4 );
	}
	inline BOOL CDC::Chord( LPCRECT lpRect, POINT ptStart, POINT ptEnd )
	{
		// Draws a chord (a region bounded by the intersection of an ellipse and a line segment, called a secant)
		return ::Chord( m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom,
			ptStart.x, ptStart.y, ptEnd.x, ptEnd.y );
	}
	inline void CDC::DrawFocusRect( LPCRECT lpRect )
	{
		// draws a rectangle in the style used to indicate that the rectangle has the focus
		::DrawFocusRect( m_hDC, lpRect );
	}
	inline BOOL CDC::Ellipse( int x1, int y1, int x2, int y2 )
	{
		// Draws an ellipse. The center of the ellipse is the center of the specified bounding rectangle.
		return ::Ellipse( m_hDC, x1, y1, x2, y2 );
	}
	inline BOOL CDC::Ellipse( LPCRECT lpRect )
	{
		// Draws an ellipse. The center of the ellipse is the center of the specified bounding rectangle.
		return ::Ellipse( m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom );
	}
	inline BOOL CDC::Pie( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 )
	{
		// Draws a pie-shaped wedge bounded by the intersection of an ellipse and two radials.
		return ::Pie( m_hDC, x1, y1, x2, y2, x3, y3, x4, y4 );
	}
	inline BOOL CDC::Pie( LPCRECT lpRect, POINT ptStart, POINT ptEnd )
	{
		// Draws a pie-shaped wedge bounded by the intersection of an ellipse and two radials.
		return ::Pie( m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom,
			ptStart.x, ptStart.y, ptEnd.x, ptEnd.y );
	}
	inline BOOL CDC::Polygon( LPPOINT lpPoints, int nCount )
	{
		// Draws a polygon consisting of two or more vertices connected by straight lines
		return ::Polygon( m_hDC, lpPoints, nCount);
	}
	inline BOOL CDC::PolyPolygon( LPPOINT lpPoints, LPINT lpPolyCounts, int nCount )
	{
		// Draws a series of closed polygons
		return ::PolyPolygon( m_hDC, lpPoints, lpPolyCounts, nCount );
	}
	inline BOOL CDC::Rectangle( int x1, int y1, int x2, int y2 )
	{
		// Draws a rectangle. The rectangle is outlined by using the current pen and filled by using the current brush.
		return ::Rectangle( m_hDC, x1, y1, x2, y2 );
	}
	inline BOOL CDC::Rectangle( LPCRECT lpRect )
	{
		// Draws a rectangle. The rectangle is outlined by using the current pen and filled by using the current brush.
		return ::Rectangle( m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom );
	}

	// Fill and 3D Drawing functions
	inline BOOL CDC::FillRect( LPCRECT lpRect, HBRUSH hbr )
	{
		// Fills a rectangle by using the specified brush
		return (BOOL)::FillRect( m_hDC, lpRect, hbr );
	}
	inline BOOL CDC::FrameRect( LPCRECT lpRect, HBRUSH hbr )
	{
		// Draws a border around the specified rectangle by using the specified brush
		return (BOOL)::FrameRect( m_hDC, lpRect, hbr );
	}
	inline BOOL CDC::InvertRect( LPCRECT lpRect )
	{
		// Inverts a rectangle in a window by performing a logical NOT operation on the color values for each pixel in the rectangle's interior
		return ::InvertRect( m_hDC, lpRect );
	}
	inline BOOL CDC::DrawIcon( int x, int y, HICON hIcon )
	{
		// Draws an icon or cursor
		return ::DrawIcon( m_hDC, x, y, hIcon );
	}
	inline BOOL CDC::DrawIcon( POINT pt, HICON hIcon )
	{
		// Draws an icon or cursor
		return ::DrawIcon( m_hDC, pt.x, pt.y, hIcon );
	}
	inline BOOL CDC::DrawEdge( LPRECT lpRect, UINT nEdge, UINT nFlags )
	{
		// Draws one or more edges of rectangle
		return ::DrawEdge( m_hDC, lpRect, nEdge, nFlags );
	}
	inline BOOL CDC::DrawFrameControl( LPRECT lpRect, UINT nType, UINT nState )
	{
		// Draws a frame control of the specified type and style
		return ::DrawFrameControl( m_hDC, lpRect, nType, nState );
	}

	// Bitmap Functions
	inline int CDC::GetDIBits( HBITMAP hbmp, UINT uStartScan, UINT cScanLines, LPVOID lpvBits, LPBITMAPINFO lpbi, UINT uUsage )
	{
		// Retrieves the bits of the specified compatible bitmap and copies them into a buffer as a DIB using the specified format
		return ::GetDIBits( m_hDC, hbmp, uStartScan, cScanLines, lpvBits, lpbi, uUsage );
	}
	
	inline int CDC::StretchDIBits( int XDest, int YDest, int nDestWidth, int nDestHeight, int XSrc, int YSrc, int nSrcWidth, 
		           int nSrcHeight, CONST VOID *lpBits, CONST BITMAPINFO *lpBitsInfo, UINT iUsage, DWORD dwRop )
	{
		// Copies the color data for a rectangle of pixels in a DIB to the specified destination rectangle
		return ::StretchDIBits( m_hDC, XDest, YDest, nDestWidth, nDestHeight, XSrc, YSrc, nSrcWidth, nSrcHeight, lpBits, lpBitsInfo, iUsage, dwRop );		           
	}
	inline int CDC::GetStretchBltMode( )
	{
		// Retrieves the current stretching mode 
		// Possible modes: BLACKONWHITE, COLORONCOLOR, HALFTONE, STRETCH_ANDSCANS, STRETCH_DELETESCANS, STRETCH_HALFTONE, STRETCH_ORSCANS, WHITEONBLACK
		return ::GetStretchBltMode( m_hDC );
	}
	inline int CDC::SetStretchBltMode( int iStretchMode )
	{
		// Sets the stretching mode 
		// Possible modes: BLACKONWHITE, COLORONCOLOR, HALFTONE, STRETCH_ANDSCANS, STRETCH_DELETESCANS, STRETCH_HALFTONE, STRETCH_ORSCANS, WHITEONBLACK
		return ::SetStretchBltMode( m_hDC, iStretchMode );
	}
	inline BOOL CDC::PatBlt( int x, int y, int nWidth, int nHeight, DWORD dwRop )
	{
		// Paints the specified rectangle using the brush that is currently selected into the device context
		return ::PatBlt( m_hDC, x, y, nWidth, nHeight, dwRop );
	}
	inline BOOL CDC::BitBlt( int x, int y, int nWidth, int nHeight, HDC hSrcDC, int xSrc, int ySrc, DWORD dwRop )
	{
		// Performs a bit-block transfer of the color data corresponding to a rectangle of pixels from the specified source device context into a destination device context
		return ::BitBlt( m_hDC, x, y, nWidth, nHeight, hSrcDC, xSrc, ySrc, dwRop );
	}
	inline BOOL CDC::StretchBlt( int x, int y, int nWidth, int nHeight, HDC hSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop )
	{
		// Copies a bitmap from a source rectangle into a destination rectangle, stretching or compressing the bitmap to fit the dimensions of the destination rectangle, if necessary
		return ::StretchBlt( m_hDC, x, y, nWidth, nHeight, hSrcDC, xSrc, ySrc, nSrcWidth, nSrcHeight, dwRop );
	}
	inline BOOL CDC::FloodFill( int x, int y, COLORREF crColor )
	{
		// Fills an area of the display surface with the current brush
		return ::FloodFill(m_hDC, x, y, crColor );
	}
	inline BOOL CDC::ExtFloodFill( int x, int y, COLORREF crColor, UINT nFillType )
	{
		// Fills an area of the display surface with the current brush
		// Fill type: FLOODFILLBORDER or FLOODFILLSURFACE
		return ::ExtFloodFill(m_hDC, x, y, crColor, nFillType );
	}

	// Text Functions
	inline BOOL CDC::TextOut( int x, int y, LPCTSTR lpszString, int nCount )
	{
		// Writes a character string at the specified location
		return ::TextOut( m_hDC, x, y, lpszString, nCount );
	}
	inline BOOL CDC::ExtTextOut( int x, int y, UINT nOptions, LPCRECT lpRect, LPCTSTR lpszString, UINT nCount, LPINT lpDxWidths )
	{
		// Draws text using the currently selected font, background color, and text color
		return ::ExtTextOut(m_hDC, x, y, nOptions, lpRect, lpszString, nCount, lpDxWidths );
	}
	inline CSize CDC::TabbedTextOut( int x, int y, LPCTSTR lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin )
	{
		// Writes a character string at a specified location, expanding tabs to the values specified in an array of tab-stop positions
		DWORD dwSize = ::TabbedTextOut(m_hDC, x, y, lpszString, nCount, nTabPositions, lpnTabStopPositions, nTabOrigin );
		CSize sz(dwSize);
		return sz;
	}
	inline int CDC::DrawText( LPCTSTR lpszString, int nCount, LPRECT lpRect, UINT nFormat )
	{
		// Draws formatted text in the specified rectangle
		return ::DrawText(m_hDC, lpszString, nCount, lpRect, nFormat );
	}
	inline CSize CDC::GetTextExtentPoint32( LPCTSTR lpszString, int nCount ) const
	{
		// Computes the width and height of the specified string of text
		CSize sz;
		::GetTextExtentPoint32(m_hDC, lpszString, nCount, &sz );
		return sz;
	}
	inline CSize CDC::GetTabbedTextExtent( LPCTSTR lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions ) const
	{
		// Computes the width and height of a character string
		DWORD dwSize = ::GetTabbedTextExtent(m_hDC, lpszString, nCount, nTabPositions, lpnTabStopPositions );
		CSize sz(dwSize);
		return sz;
	}
	inline BOOL CDC::GrayString( HBRUSH hBrush, GRAYSTRINGPROC lpOutputFunc, LPARAM lpData, int nCount, int x, int y, int nWidth, int nHeight )
	{
		// Draws gray text at the specified location
		return ::GrayString(m_hDC, hBrush, lpOutputFunc, lpData, nCount, x, y, nWidth, nHeight );
	}
	inline UINT CDC::GetTextAlign( ) const
	{
		// Retrieves the text-alignment setting
		// Values: TA_BASELINE, TA_BOTTOM, TA_TOP, TA_CENTER, TA_LEFT, TA_RIGHT, TA_RTLREADING, TA_NOUPDATECP, TA_UPDATECP
		return ::GetTextAlign( m_hDC );
	}
	inline UINT CDC::SetTextAlign( UINT nFlags )
	{
		// Sets the text-alignment setting
		// Values: TA_BASELINE, TA_BOTTOM, TA_TOP, TA_CENTER, TA_LEFT, TA_RIGHT, TA_RTLREADING, TA_NOUPDATECP, TA_UPDATECP
		return ::SetTextAlign( m_hDC, nFlags );
	}
	inline int CDC::GetTextFace( int nCount, LPTSTR lpszFacename ) const
	{
		// Retrieves the typeface name of the font that is selected into the device context
		return ::GetTextFace( m_hDC, nCount, lpszFacename );
	}
	inline BOOL CDC::GetTextMetrics( LPTEXTMETRIC lpMetrics ) const
	{
		// Fills the specified buffer with the metrics for the currently selected font
		return ::GetTextMetrics( m_hDC, lpMetrics );
	}
	inline int CDC::SetTextJustification( int nBreakExtra, int nBreakCount  )
	{
		// Specifies the amount of space the system should add to the break characters in a string of text
		return ::SetTextJustification( m_hDC, nBreakExtra, nBreakCount  );
	}
	inline int CDC::GetTextCharacterExtra( ) const
	{
		// Retrieves the current intercharacter spacing for the device context
		return ::GetTextCharacterExtra( m_hDC );
	}
	inline int CDC::SetTextCharacterExtra( int nCharExtra )
	{
		// Sets the intercharacter spacing
		return ::SetTextCharacterExtra( m_hDC, nCharExtra );
	}

} // namespace Win32xx

#endif // _GDI_H_

