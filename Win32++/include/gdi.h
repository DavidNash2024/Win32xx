// Win32++  Version 7.0 (Prerelease version)
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

#include "wincore.h"

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
	// Declarations for the CDC class
	//
	class CDC
	{
	public:
		CDC( );
		CDC( HDC hDC );
		CDC(const CDC& rhs);					// Copy constructor
		void operator = ( const HDC hDC );		// Cast the CDC object to a HDC
		operator HDC( ) const { return *m_DC; }
		virtual ~CDC( );
		void AttachDC( HDC hDC );
		HDC  DetachDC( );
		HDC GetHDC( ) const { return *m_DC; }

		// Create and Select Bitmaps
		void AttachBitmap( HBITMAP hBitmap );
		void CreateBitmap( int cx, int cy, UINT Planes, UINT BitsPerPixel, CONST VOID *pvColors );
		void CreateCompatibleBitmap( HDC hDC, int cx, int cy );
		void CreateDIBSection( HDC hdc, const BITMAPINFO& bmi, UINT iUsage, VOID **ppvBits,
										HANDLE hSection, DWORD dwOffset) ;
		HBITMAP DetachBitmap( );
#ifndef _WIN32_WCE
		void CreateBitmapIndirect( const BITMAP& bm );
		void CreateDIBitmap( HDC hdc, const BITMAPINFOHEADER& bmih, DWORD fdwInit, CONST VOID *lpbInit,
										BITMAPINFO& bmi, UINT fuUsage );
#endif		

		// Create and Select Brushes
		void AttachBrush( HBRUSH hBrush );
		void CreateDIBPatternBrushPt( const VOID *lpPackedDIB, UINT iUsage );
		void CreatePatternBrush( HBITMAP hbmp );
		void CreateSolidBrush( COLORREF rbg );
		HBRUSH DetachBrush( );
#ifndef _WIN32_WCE
		void CreateBrushIndirect( const LOGBRUSH& lb );
		void CreateDIBPatternBrush( HGLOBAL hglbDIBPacked, UINT fuColorSpec );
		void CreateHatchBrush( int fnStyle, COLORREF rgb );
#endif		

		// Create and Select Fonts
		void AttachFont( HFONT hFont );
		void CreateFontIndirect( const LOGFONT& lf );
		HFONT DetachFont( );
#ifndef _WIN32_WCE
		void CreateFont( int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight,
  							DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet,
  							DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality,
  							DWORD fdwPitchAndFamily, LPCTSTR lpszFace );
#endif		

		// Create and Select Pens
		void AttachPen( HPEN hPen );
		void CreatePen( int nStyle, int nWidth, COLORREF rgb );
		void CreatePenIndirect( const LOGPEN& lgpn );
		HPEN DetachPen( );

		// Create Select Regions
		void AttachClipRegion( HRGN hRegion );
		void CreateRectRgn( int left, int top, int right, int bottom );
		void CreateRectRgnIndirect( const RECT& rc );
		HRGN DetachClipRegion( );
		void ExtCreateRegion( const XFORM& Xform, DWORD nCount, const RGNDATA *pRgnData );
#ifndef _WIN32_WCE
		void CreateEllipticRgn( int left, int top, int right, int bottom );
		void CreateEllipticRgnIndirect( const RECT& rc );
		void CreatePolygonRgn( const POINT* ppt, int cPoints, int fnPolyFillMode );
		void CreatePolyPolygonRgn( const POINT* ppt, const int* pPolyCounts, int nCount, int fnPolyFillMode );
#endif

		// Wrappers for WinAPI functions

		// Initialization
		HDC CreateCompatibleDC( ) const;
		HDC CreateDC( LPCTSTR lpszDriver, LPCTSTR lpszDevice, LPCTSTR lpszOutput, const DEVMODE& dvmInit ) const;
		int GetDeviceCaps( int nIndex ) const;
#ifndef _WIN32_WCE
		HDC CreateIC( LPCTSTR lpszDriver, LPCTSTR lpszDevice, LPCTSTR lpszOutput, const DEVMODE& dvmInit ) const;
		CWnd* WindowFromDC( ) const;
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
		BOOL DPtoLP(LPPOINT lpPoints, int nCount )  const;
		BOOL DPtoLP(LPRECT lpRect)  const;
		BOOL LPtoDP(LPPOINT lpPoints, int nCount )  const;
		BOOL LPtoDP(LPRECT lpRect)  const;

		// Layout Functions
		DWORD GetLayout() const;
		DWORD SetLayout(DWORD dwLayout) const;

		// Mapping functions
		int GetMapMode()  const;
		int SetMapMode(int nMapMode) const;
		BOOL GetViewportOrgEx(LPPOINT lpPoint)  const;
		BOOL SetViewportOrgEx(int x, int y, LPPOINT lpPoint) const;
		BOOL SetViewportOrgEx(POINT point, LPPOINT lpPointRet ) const;
		BOOL OffsetViewportOrgEx(int nWidth, int nHeight, LPPOINT lpPoint ) const;
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
		CSize GetTextExtentPoint( LPCTSTR lpszString, int nCount ) const;
#ifndef _WIN32_WCE
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
		CDC& operator = ( const CDC& rhs );

		HDC* m_DC;
		HBITMAP* m_BitmapOld;
		HBRUSH* m_BrushOld;
		HFONT* m_FontOld;
		HPEN* m_PenOld;
		HRGN* m_RgnOld;
		long* m_Count;
	};


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
			UINT uQuadSize = (cClrBits == 24)? 0 : sizeof(RGBQUAD) * ((__int64)1 << cClrBits);
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
		AttachDC(hDC);
	}

	inline CDC::~CDC()
	{	
		if (InterlockedDecrement(m_Count) == 0)
		{
			if (*m_DC)
			{
				// Delete any GDI objects belonging to this CDC
				if (*m_PenOld)    ::DeleteObject(::SelectObject(*m_DC, *m_PenOld));
				if (*m_BrushOld)  ::DeleteObject(::SelectObject(*m_DC, *m_BrushOld));
				if (*m_BitmapOld) ::DeleteObject(::SelectObject(*m_DC, *m_BitmapOld));
				if (*m_FontOld)	  ::DeleteObject(::SelectObject(*m_DC, *m_FontOld));
				if (*m_RgnOld)    ::DeleteObject(*m_RgnOld);

				// We need to release a Window DC, and delete a memory DC
	#ifndef _WIN32_WCE
				HWND hwnd = ::WindowFromDC(*m_DC);
				if (hwnd) ::ReleaseDC(hwnd, *m_DC);
				else      ::DeleteDC(*m_DC);
	#else
				::DeleteDC(*m_DC);
	#endif
			}

			delete m_PenOld;
			delete m_BrushOld;
			delete m_BitmapOld;
			delete m_FontOld;
			delete m_RgnOld;
			delete m_Count;
		}
	}

	inline void CDC::AttachDC(HDC hDC)
	{
		assert (0 == *m_DC);
		assert(hDC);

		*m_DC = hDC;
	}

	inline HDC CDC::DetachDC()
	{
		assert(*m_DC);

		if (*m_PenOld)    ::DeleteObject(::SelectObject(*m_DC, *m_PenOld));
		if (*m_BrushOld)  ::DeleteObject(::SelectObject(*m_DC, *m_BrushOld));
		if (*m_BitmapOld) ::DeleteObject(::SelectObject(*m_DC, *m_BitmapOld));
		if (*m_FontOld)	  ::DeleteObject(::SelectObject(*m_DC, *m_FontOld));

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
		CDC dcImage = ::CreateCompatibleDC(*m_DC);
		dcImage.AttachBitmap(hbmImage);

		// Create the Mask memory DC
		HBITMAP hbmMask = ::CreateBitmap(cx, cy, 1, 1, NULL);
		CDC dcMask = ::CreateCompatibleDC(*m_DC);
		dcMask.AttachBitmap(hbmMask);
		::SetBkColor(dcImage, clrMask);
		::BitBlt(dcMask, 0, 0, cx, cy, dcImage, 0, 0, SRCCOPY);

		// Mask the image to the DC provided
		::BitBlt(*m_DC, x, y, cx, cy, dcImage, 0, 0, SRCINVERT);
		::BitBlt(*m_DC, x, y, cx, cy, dcMask, 0, 0, SRCAND);
		::BitBlt(*m_DC, x, y, cx, cy, dcImage, 0, 0, SRCINVERT);

		// Detach the bitmap before the dcImage is destroyed
		dcImage.DetachBitmap();
	}

	inline void CDC::GradientFill( COLORREF Color1, COLORREF Color2, const RECT& rc, BOOL bVertical )
	// A simple but efficient Gradient Filler compatible with all Windows operating systems
	{
		int Width = rc.right - rc.left;
		int Height = rc.bottom - rc.top;

		int r1 = GetRValue(Color1);
		int g1 = GetGValue(Color1);
		int b1 = GetBValue(Color1);

		int r2 = GetRValue(Color2);
		int g2 = GetGValue(Color2);
		int b2 = GetBValue(Color2);

		COLORREF OldBkColor = ::GetBkColor(*m_DC);

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

		// Delete any existing bitmap
		if (*m_BitmapOld) ::DeleteObject(::SelectObject(*m_DC, *m_BitmapOld));

		*m_BitmapOld = (HBITMAP)::SelectObject(*m_DC, hBitmap);
	}

	inline void CDC::CreateCompatibleBitmap(HDC hDC, int cx, int cy)
	{
		// Creates a compatible bitmap and selects it into the device context

		assert(*m_DC);

		if (*m_BitmapOld) ::DeleteObject(::SelectObject(*m_DC, *m_BitmapOld));

		HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, cx, cy);
		assert(hBitmap);

		*m_BitmapOld = (HBITMAP)::SelectObject(*m_DC, hBitmap);
	}

	inline void CDC::CreateBitmap(int cx, int cy, UINT Planes, UINT BitsPerPixel, CONST VOID *pvColors)
	{
		// Creates a bitmap and selects it into the device context

		assert(*m_DC);
		if (*m_BitmapOld) ::DeleteObject(::SelectObject(*m_DC, *m_BitmapOld));

		HBITMAP hBitmap = ::CreateBitmap(cx, cy, Planes, BitsPerPixel, pvColors);
		assert(hBitmap);

		*m_BitmapOld = (HBITMAP)::SelectObject(*m_DC, hBitmap);
	}

#ifndef _WIN32_WCE
	inline void CDC::CreateBitmapIndirect(const BITMAP& bm)
	{
		// Creates a bitmap and selects it into the device context

		assert(*m_DC);
		if (*m_BitmapOld) ::DeleteObject(::SelectObject(*m_DC, *m_BitmapOld));

		HBITMAP hBitmap = ::CreateBitmapIndirect(&bm);
		assert(hBitmap);

		*m_BitmapOld = (HBITMAP)::SelectObject(*m_DC, hBitmap);
	}

	inline void CDC::CreateDIBitmap(HDC hdc, const BITMAPINFOHEADER& bmih, DWORD fdwInit, CONST VOID *lpbInit,
										BITMAPINFO& bmi,  UINT fuUsage)
	{
		// Creates a bitmap and selects it into the device context

		assert(*m_DC);
		if (*m_BitmapOld) ::DeleteObject(::SelectObject(*m_DC, *m_BitmapOld));

		HBITMAP hBitmap = ::CreateDIBitmap(hdc, &bmih, fdwInit, lpbInit, &bmi, fuUsage);
		assert(hBitmap);

		*m_BitmapOld = (HBITMAP)::SelectObject(*m_DC, hBitmap);
	}
#endif

	inline void CDC::CreateDIBSection(HDC hdc, const BITMAPINFO& bmi, UINT iUsage, VOID **ppvBits,
										HANDLE hSection, DWORD dwOffset)
	{
		// Creates a bitmap and selects it into the device context

		assert(*m_DC);
		if (*m_BitmapOld)::DeleteObject(::SelectObject(*m_DC, *m_BitmapOld));

		HBITMAP hBitmap = ::CreateDIBSection(hdc, &bmi, iUsage, ppvBits, hSection, dwOffset);
		assert(hBitmap);

		*m_BitmapOld = (HBITMAP)::SelectObject(*m_DC, hBitmap);
	}

	inline HBITMAP CDC::DetachBitmap()
	{
		// Use this to detach the bitmap from the HDC.
		// You are then responible for deleting the detached bitmap

		assert(*m_DC);
		assert(*m_BitmapOld);

		HBITMAP hBitmap = (HBITMAP)::SelectObject(*m_DC, *m_BitmapOld);
		*m_BitmapOld = NULL;
		return hBitmap;
	}

	// Brush functions
	inline void CDC::AttachBrush(HBRUSH hBrush)
	{
		// Use this to attach an existing brush.
		// The brush will be deleted for you, unless its detached

		assert(*m_DC);
		assert(hBrush);
		if (*m_BrushOld) ::DeleteObject(::SelectObject(*m_DC, *m_BrushOld));
		*m_BrushOld = (HBRUSH)::SelectObject(*m_DC, hBrush);
	}

#ifndef _WIN32_WCE
	inline void CDC::CreateBrushIndirect( const LOGBRUSH& lb)
	{
		// Creates the brush and selects it into the device context

		assert(*m_DC);
		if (*m_BrushOld) ::DeleteObject(::SelectObject(*m_DC, *m_BrushOld));

		HBRUSH hBrush = ::CreateBrushIndirect(&lb);
		assert(hBrush);

		*m_BrushOld = (HBRUSH)::SelectObject(*m_DC, hBrush);
	}

	inline void CDC::CreateDIBPatternBrush(HGLOBAL hglbDIBPacked, UINT fuColorSpec)
	{
		// Creates the brush and selects it into the device context

		assert(*m_DC);
		if (*m_BrushOld) ::DeleteObject(::SelectObject(*m_DC, *m_BrushOld));

		HBRUSH hBrush = ::CreateDIBPatternBrush(hglbDIBPacked, fuColorSpec);
		assert(hBrush);

		*m_BrushOld = (HBRUSH)::SelectObject(*m_DC, hBrush);
	}

	inline void CDC::CreateHatchBrush(int fnStyle, COLORREF rgb)
	{
		// Creates the brush and selects it into the device context

		assert(*m_DC);
		if (*m_BrushOld) ::DeleteObject(::SelectObject(*m_DC, *m_BrushOld));

		HBRUSH hBrush = ::CreateHatchBrush(fnStyle, rgb);
		assert(hBrush);

		*m_BrushOld = (HBRUSH)::SelectObject(*m_DC, hBrush);
	}
#endif

	inline void CDC::CreateDIBPatternBrushPt(CONST VOID *lpPackedDIB, UINT iUsage)
	{
		// Creates the brush and selects it into the device context

		assert(*m_DC);
		if (*m_BrushOld) ::DeleteObject(::SelectObject(*m_DC, *m_BrushOld));

		HBRUSH hBrush = ::CreateDIBPatternBrushPt(lpPackedDIB, iUsage);
		assert(hBrush);

		*m_BrushOld = (HBRUSH)::SelectObject(*m_DC, hBrush);
	}

	inline void CDC::CreatePatternBrush(HBITMAP hbmp)
	{
		// Creates the brush and selects it into the device context

		assert(*m_DC);
		if (*m_BrushOld) ::DeleteObject(::SelectObject(*m_DC, *m_BrushOld));

		HBRUSH hBrush = ::CreatePatternBrush(hbmp);
		assert(hBrush);

		*m_BrushOld = (HBRUSH)::SelectObject(*m_DC, hBrush);
	}

	inline void CDC::CreateSolidBrush(COLORREF rgb)
	{
		// Creates the brush and selects it into the device context

		assert(*m_DC);
		if (*m_BrushOld) ::DeleteObject(::SelectObject(*m_DC, *m_BrushOld));

		HBRUSH hBrush = ::CreateSolidBrush(rgb);
		assert(hBrush);

		*m_BrushOld = (HBRUSH)::SelectObject(*m_DC, hBrush);
	}

	inline HBRUSH CDC::DetachBrush()
	{
		// Use this to detach the brush from the HDC.
		// You are then responible for deleting the detached brush

		assert(*m_DC);
		assert(*m_BrushOld);

		HBRUSH hBrush = (HBRUSH)::SelectObject(*m_DC, *m_BrushOld);
		*m_BrushOld = NULL;
		return hBrush;
	}

	// Font functions
	inline void CDC::AttachFont(HFONT hFont)
	{
		// Use this to attach an existing font.
		// The font will be deleted for you, unless its detached

		assert(*m_DC);
		assert(hFont);
		if (*m_FontOld) ::DeleteObject(::SelectObject(*m_DC, *m_FontOld));
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
		assert(*m_DC);
		if (*m_FontOld) ::DeleteObject(::SelectObject(*m_DC, *m_FontOld));

		HFONT hFont = ::CreateFont(nHeight, nWidth, nEscapement, nOrientation, fnWeight,
								fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet,
								fdwOutputPrecision, fdwClipPrecision, fdwQuality,
								fdwPitchAndFamily, lpszFace);

		assert(hFont);

		*m_FontOld = (HFONT)::SelectObject(*m_DC, hFont);
	}
#endif

	inline void CDC::CreateFontIndirect( const LOGFONT& lf)
	{
		assert(*m_DC);
		if (*m_FontOld) ::DeleteObject(::SelectObject(*m_DC, *m_FontOld));

		HFONT hFont = ::CreateFontIndirect(&lf);
		assert(hFont);

		*m_FontOld = (HFONT)::SelectObject(*m_DC, hFont);
	}

	inline HFONT CDC::DetachFont()
	{
		// Use this to detach the font from the HDC.
		// You are then responible for deleting the detached font

		assert(*m_DC);
		assert(*m_FontOld);

		HFONT hFont = (HFONT)::SelectObject(*m_DC, *m_FontOld);
		*m_FontOld = NULL;
		return hFont;
	}

	// Pen functions
	inline void CDC::AttachPen(HPEN hPen)
	{
		// Use this to attach an existing pen.
		// The pen will be deleted for you, unless its detached

		assert(*m_DC);
		assert(hPen);
		if (*m_PenOld) ::DeleteObject(::SelectObject(*m_DC, *m_PenOld));
		*m_PenOld = (HPEN)::SelectObject(*m_DC, hPen);
	}

	inline void CDC::CreatePen(int nStyle, int nWidth, COLORREF rgb)
	{
		// Creates the pen and selects it into the device context

		assert(*m_DC);
		if (*m_PenOld) ::DeleteObject(::SelectObject(*m_DC, *m_PenOld));

		HPEN hPen = ::CreatePen(nStyle, nWidth, rgb);
		assert(hPen);

		*m_PenOld = (HPEN)::SelectObject(*m_DC, hPen);
	}

	inline void CDC::CreatePenIndirect( const LOGPEN& lgpn)
	{
		// Creates the pen and selects it into the device context

		assert(*m_DC);
		if (*m_PenOld) ::DeleteObject(::SelectObject(*m_DC, *m_PenOld));

		HPEN hPen = ::CreatePenIndirect(&lgpn);
		assert(hPen);

		*m_PenOld = (HPEN)::SelectObject(*m_DC, hPen);
	}

	inline HPEN CDC::DetachPen()
	{
		// Use this to detach the pen from the HDC.
		// You are then responible for deleting the detached pen

		assert(*m_DC);
		assert(*m_PenOld);

		HPEN hPen = (HPEN)::SelectObject(*m_DC, *m_PenOld);
		*m_PenOld = NULL;
		return hPen;
	}

	// Region functions
	inline void CDC::AttachClipRegion(HRGN hRegion)
	{
		// Use this to attach an existing region.
		// The region will be deleted for you, unless its detached
		// Note: The shape of a region cannot be changed while it is attached to a DC

		assert(*m_DC);
		assert(hRegion);

		if (*m_RgnOld) ::DeleteObject(*m_RgnOld);

		::SelectClipRgn(*m_DC, hRegion);
		*m_RgnOld = hRegion;
	}
	inline void CDC::CreateRectRgn(int left, int top, int right, int bottom)
	{
		// Creates a rectangular region from the rectangle co-ordinates.
		// The region will be deleted for you, unless its detached
		// Note: The shape of a region cannot be changed while it is attached to a DC

		assert(*m_DC);
		if (*m_RgnOld) ::DeleteObject(*m_RgnOld);

		HRGN hRgn = ::CreateRectRgn(left, top, right, bottom);
		assert(hRgn);

		::SelectClipRgn(*m_DC, hRgn);
		*m_RgnOld = hRgn;
	}

	inline void CDC::CreateRectRgnIndirect( const RECT& rc)
	{
		// Creates a rectangular region from the rectangle co-ordinates.
		// The region will be deleted for you, unless its detached
		// Note: The shape of a region cannot be changed while it is attached to a DC

		assert(*m_DC);
		if (*m_RgnOld) ::DeleteObject(*m_RgnOld);

		HRGN hRgn = ::CreateRectRgnIndirect(&rc);
		assert(hRgn);

		::SelectClipRgn(*m_DC, hRgn);
		*m_RgnOld = hRgn;
	}

	inline void CDC::ExtCreateRegion( const XFORM& Xform, DWORD nCount, const RGNDATA *pRgnData)
	{
		// Creates a region from the specified region data and tranformation data.
		// The region will be deleted for you, unless its detached
		// Notes: The shape of a region cannot be changed while it is attached to a DC
		//        GetRegionData can be used to get a region's data
		//        If the XFROM pointer is NULL, the identity transformation is used.

		assert(*m_DC);
		if (*m_RgnOld) ::DeleteObject(*m_RgnOld);

		HRGN hRgn = ::ExtCreateRegion(&Xform, nCount, pRgnData);
		assert(hRgn);

		::SelectClipRgn(*m_DC, hRgn);
		*m_RgnOld = hRgn;
	}

	inline HRGN CDC::DetachClipRegion()
	{
		// Use this to detach the region from the HDC.
		// You are then responible for deleting the detached region

		assert(*m_DC);
		assert(*m_RgnOld);

		::SelectClipRgn(*m_DC, NULL);
		HRGN hRgn = *m_RgnOld;
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
		if (*m_RgnOld) ::DeleteObject(*m_RgnOld);

		HRGN hRgn = ::CreateEllipticRgn(left, top, right, bottom);
		assert(hRgn);

		::SelectClipRgn(*m_DC, hRgn);
		*m_RgnOld = hRgn;
	}

	inline void CDC::CreateEllipticRgnIndirect( const RECT& rc)
	{
		// Creates the ellyiptical region from the bounding rectangle co-ordinates
		// and selects it into the device context
		// The region will be deleted for you, unless its detached
		// Note: The shape of a region cannot be changed while it is attached to a DC

		assert(*m_DC);
		if (*m_RgnOld) ::DeleteObject(*m_RgnOld);

		HRGN hRgn = ::CreateEllipticRgnIndirect(&rc);
		assert(hRgn);

		::SelectClipRgn(*m_DC, hRgn);
		*m_RgnOld = hRgn;
	}

	inline void CDC::CreatePolygonRgn(const POINT* ppt, int cPoints, int fnPolyFillMode)
	{
		// Creates the polygon region from the array of points and selects it into
		// the device context. The polygon is presumed closed
		// The region will be deleted for you, unless its detached
		// Note: The shape of a region cannot be changed while it is attached to a DC

		assert(*m_DC);
		if (*m_RgnOld) ::DeleteObject(*m_RgnOld);

		HRGN hRgn = ::CreatePolygonRgn(ppt, cPoints, fnPolyFillMode);
		assert(hRgn);

		::SelectClipRgn(*m_DC, hRgn);
		*m_RgnOld = hRgn;
	}

	inline void CDC::CreatePolyPolygonRgn(const POINT* ppt, const int* pPolyCounts, int nCount, int fnPolyFillMode)
	{
		// Creates the polygon region from a series of polygons.The polygons can overlap.
		// The region will be deleted for you, unless its detached
		// Note: The shape of a region cannot be changed while it is attached to a DC

		assert(*m_DC);
		if (*m_RgnOld) ::DeleteObject(*m_RgnOld);

		HRGN hRgn = ::CreatePolyPolygonRgn(ppt, pPolyCounts, nCount, fnPolyFillMode);
		assert(hRgn);

		::SelectClipRgn(*m_DC, hRgn);
		*m_RgnOld = hRgn;
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
#ifndef _WIN32_WCE
	inline CWnd* CDC::WindowFromDC( ) const
	{
		return CWnd::FromHandle( ::WindowFromDC( *m_DC ) );
	}
	inline HDC CDC::CreateIC( LPCTSTR lpszDriver, LPCTSTR lpszDevice, LPCTSTR lpszOutput, const DEVMODE& dvmInit ) const
	{
		return ::CreateIC( lpszDriver, lpszDevice, lpszOutput, &dvmInit );
	}
#endif

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
#endif

	// co-ordingate functions
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
	inline BOOL CDC::SetViewportOrgEx(int x, int y, LPPOINT lpPoint) const
	{
		if (*m_DC != NULL)
			return ::SetViewportOrgEx(*m_DC, x, y, lpPoint);
		return FALSE;
	}
	inline BOOL CDC::SetViewportOrgEx(POINT point, LPPOINT lpPointRet ) const
	{
		if (*m_DC != NULL)
			return SetViewportOrgEx(point.x, point.y, lpPointRet);
		return FALSE;
	}
	inline BOOL CDC::OffsetViewportOrgEx(int nWidth, int nHeight, LPPOINT lpPoint ) const
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
	inline CSize CDC::GetTextExtentPoint( LPCTSTR lpszString, int nCount ) const
	{
		// Computes the width and height of the specified string of text
		CSize sz;
		::GetTextExtentPoint(*m_DC, lpszString, nCount, &sz );
		return sz;
	}
#ifndef _WIN32_WCE
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

