// Win32++  Version 6.7a
// Released: 8th November, 2009 by:
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
		CDC(const CDC& rhs);				// Copy constructor
		void operator = ( const HDC hDC );
		virtual ~CDC( );
		HDC GetHDC( ) const { return m_hDC; }

		virtual void AttachDC( HDC hDC );
		virtual HDC  DetachDC( );
		virtual void DrawBitmap( int x, int y, int cx, int cy, HBITMAP hbmImage, COLORREF clrMask );
		virtual void GradientFill( COLORREF Color1, COLORREF Color2, const RECT& rc, BOOL bVertical );
		virtual void SolidFill( COLORREF Color, const RECT& rc );

		// Create and Select Bitmaps
		virtual void AttachBitmap( HBITMAP hBitmap );
		virtual void CreateBitmap( int cx, int cy, UINT Planes, UINT BitsPerPixel, CONST VOID *pvColors );
		virtual void CreateCompatibleBitmap( HDC hDC, int cx, int cy );
		virtual void CreateDIBSection( HDC hdc, const BITMAPINFO& bmi, UINT iUsage, VOID **ppvBits,
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
		virtual void CreateFontIndirect( const LOGFONT& lf );
		virtual HFONT DetachFont( );

		// Create and Select Pens
		virtual void AttachPen( HPEN hPen );
		virtual void CreatePen( int nStyle, int nWidth, COLORREF rgb );
		virtual void CreatePenIndirect( const LOGPEN& lgpn );
		virtual HPEN DetachPen( );

		// Create Select Regions
		virtual void AttachClipRegion( HRGN hRegion );

		virtual void CreateRectRgn( int left, int top, int right, int bottom );
		virtual void CreateRectRgnIndirect( const RECT& rc );
		virtual HRGN DetachClipRegion( );
		virtual void ExtCreateRegion( const XFORM& Xform, DWORD nCount, const RGNDATA *pRgnData );
#ifndef _WIN32_WCE
		virtual void CreateEllipticRgn( int left, int top, int right, int bottom );
		virtual void CreateEllipticRgnIndirect( const RECT& rc );
		virtual void CreatePolygonRgn( const POINT* ppt, int cPoints, int fnPolyFillMode );
		virtual void CreatePolyPolygonRgn( const POINT* ppt, const int* pPolyCounts, int nCount, int fnPolyFillMode );
#endif
		// Cast the CDC object to a HDC
		operator HDC( ) const { return m_hDC; }

#ifndef _WIN32_WCE
		// Create and Select Bitmaps
		virtual void CreateBitmapIndirect( const BITMAP& bm );
		virtual void CreateDIBitmap( HDC hdc, const BITMAPINFOHEADER& bmih, DWORD fdwInit, CONST VOID *lpbInit,
										BITMAPINFO& bmi, UINT fuUsage );
		// Create and Select Brushes
		virtual void CreateBrushIndirect( const LOGBRUSH& lb );
		virtual void CreateDIBPatternBrush( HGLOBAL hglbDIBPacked, UINT fuColorSpec );
		virtual void CreateHatchBrush( int fnStyle, COLORREF rgb );

		// Create and Select Fonts
		virtual void CreateFont( int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight,
  							DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet,
  							DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality,
  							DWORD fdwPitchAndFamily, LPCTSTR lpszFace );
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
#ifndef _WIN32_WCE
		BOOL DrawIcon( int x, int y, HICON hIcon ) const;
		BOOL DrawIcon( POINT point, HICON hIcon ) const;
		BOOL FrameRect( const RECT& rc, HBRUSH hbr ) const;
		BOOL PaintRgn( HRGN hrgn ) const;
#endif

		// Bitmap Functions
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
		CDC& operator = (const CDC&);	// Disable assignment operator
		HDC m_hDC;
		HBITMAP m_hBitmapOld;
		HBRUSH m_hBrushOld;
		HFONT m_hFontOld;
		HPEN m_hPenOld;
		HRGN m_hRgnOld;
		BOOL m_IsCopy;
		CDC* m_pCopiedFrom;
		UINT m_nCopies;
	};


	class CBitmapInfoPtr
	{
	public:
		CBitmapInfoPtr(HBITMAP hbm) : m_pbmi(0)
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
			UINT uQuadSize = (cClrBits == 24)? 0 : sizeof(RGBQUAD) * ((size_t)1<< cClrBits);
			m_pbmi = (LPBITMAPINFO)new byte[sizeof(BITMAPINFOHEADER) + uQuadSize];
			if (NULL == m_pbmi) throw std::bad_alloc();
			ZeroMemory(m_pbmi, sizeof(BITMAPINFOHEADER) + uQuadSize);

			m_pbmi->bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
			m_pbmi->bmiHeader.biHeight		= bmSource.bmHeight;
			m_pbmi->bmiHeader.biWidth		= bmSource.bmWidth;
			m_pbmi->bmiHeader.biPlanes		= bmSource.bmPlanes;
			m_pbmi->bmiHeader.biBitCount	= bmSource.bmBitsPixel;
			m_pbmi->bmiHeader.biCompression = BI_RGB;
			if (cClrBits < 24)
				m_pbmi->bmiHeader.biClrUsed = (1<<cClrBits);
		}
		~CBitmapInfoPtr() 	{ delete[] m_pbmi; }
		operator LPBITMAPINFO() const { return m_pbmi; }
		LPBITMAPINFO operator->() const { return m_pbmi; }

	private:
		CBitmapInfoPtr(const CBitmapInfoPtr&);				// Disable copy construction
		CBitmapInfoPtr& operator = (const CBitmapInfoPtr&);	// Disable assignment operator
		LPBITMAPINFO m_pbmi;
	};

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{

	///////////////////////////////////////////////
	// Definitions of the CDC class
	//
	inline CDC::CDC() : m_hDC(0), m_hBitmapOld(0), m_hBrushOld(0), m_hFontOld(0), m_hPenOld(0),
					m_hRgnOld(0), m_IsCopy(FALSE), m_pCopiedFrom(0), m_nCopies(0)
	{
	}

	inline CDC::CDC(HDC hDC) : m_hDC(0), m_hBitmapOld(0), m_hBrushOld(0), m_hFontOld(0), m_hPenOld(0),
						m_hRgnOld(0), m_IsCopy(FALSE), m_pCopiedFrom(0), m_nCopies(0)
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

	inline CDC::CDC(const CDC& rhs)	// Copy constructor
	{
		// The copy constructor is called when a temporary copy of the CDC needs to be created.
		// Since we have two (or more) CDC objects looking after the same HDC, we need to
		//  take account of this in the destructor.

		// Note: Were it not for the peculiarities of Dev-C++, the copy constructor would
		//  simply have been disabled.
		m_hBitmapOld = rhs.m_hBitmapOld;
		m_hBrushOld  = rhs.m_hBrushOld;
		m_hDC		 = rhs.m_hDC;
		m_hFontOld	 = rhs.m_hFontOld;
		m_hPenOld    = rhs.m_hPenOld;
		m_hRgnOld    = rhs.m_hRgnOld;
		m_nCopies    = 0;

		// This CDC is a copy, so we won't need to delete GDI resources in the destructor
		m_IsCopy  = TRUE;
		m_pCopiedFrom = (CDC*)&rhs;
		m_pCopiedFrom->m_nCopies++;
	}

	inline void CDC::operator = (const HDC hDC)
	{
		AttachDC(hDC);
	}

	inline CDC::~CDC()
	{
		if (m_hDC)
		{
			if (m_IsCopy)
			{
				// This CDC is just a temporary clone, created by the copy constructor
                // so pass members back to the original
				m_pCopiedFrom->m_hPenOld	= m_hPenOld;
				m_pCopiedFrom->m_hBrushOld	= m_hBrushOld;
				m_pCopiedFrom->m_hBitmapOld	= m_hBitmapOld;
				m_pCopiedFrom->m_hFontOld	= m_hFontOld;
				m_pCopiedFrom->m_hRgnOld    = m_hRgnOld;
				m_pCopiedFrom->m_hDC		= m_hDC;
				m_pCopiedFrom->m_nCopies--;
			}
			else
			{
				// Assert that all CDC copies have been destroyed before destroying primary.
				// An assert here indicates a bug in user code! (somehow destroying the primary CDC before its copies)
				assert(m_nCopies == 0);

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

	inline void CDC::DrawBitmap( int x, int y, int cx, int cy, HBITMAP hbmImage, COLORREF clrMask )
	// Draws the specified bitmap to the specified DC using the mask colour provided as the transparent colour
	// Suitable for use with a Window DC or a memory DC
	{
		// Create the Image memory DC
		CDC dcImage = ::CreateCompatibleDC(m_hDC);
		dcImage.AttachBitmap(hbmImage);

		// Create the Mask memory DC
		HBITMAP hbmMask = ::CreateBitmap(cx, cy, 1, 1, NULL);
		CDC dcMask = ::CreateCompatibleDC(m_hDC);
		dcMask.AttachBitmap(hbmMask);
		::SetBkColor(dcImage, clrMask);
		::BitBlt(dcMask, 0, 0, cx, cy, dcImage, 0, 0, SRCCOPY);

		// Mask the image to the DC provided
		::BitBlt(m_hDC, x, y, cx, cy, dcImage, 0, 0, SRCINVERT);
		::BitBlt(m_hDC, x, y, cx, cy, dcMask, 0, 0, SRCAND);
		::BitBlt(m_hDC, x, y, cx, cy, dcImage, 0, 0, SRCINVERT);

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

		COLORREF OldBkColor = ::GetBkColor(m_hDC);

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
	inline void CDC::CreateBitmapIndirect(const BITMAP& bm)
	{
		// Creates a bitmap and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBitmapOld) ::DeleteObject(::SelectObject(m_hDC, m_hBitmapOld));

		HBITMAP hBitmap = ::CreateBitmapIndirect(&bm);
		if (!hBitmap) throw CWinException(_T("CreateBitmap failed"));

		m_hBitmapOld = (HBITMAP)::SelectObject(m_hDC, hBitmap);
	}

	inline void CDC::CreateDIBitmap(HDC hdc, const BITMAPINFOHEADER& bmih, DWORD fdwInit, CONST VOID *lpbInit,
										BITMAPINFO& bmi,  UINT fuUsage)
	{
		// Creates a bitmap and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBitmapOld) ::DeleteObject(::SelectObject(m_hDC, m_hBitmapOld));

		HBITMAP hBitmap = ::CreateDIBitmap(hdc, &bmih, fdwInit, lpbInit, &bmi, fuUsage);
		if (!hBitmap) throw CWinException(_T("CreateDIBitmap failed"));

		m_hBitmapOld = (HBITMAP)::SelectObject(m_hDC, hBitmap);
	}
#endif

	inline void CDC::CreateDIBSection(HDC hdc, const BITMAPINFO& bmi, UINT iUsage, VOID **ppvBits,
										HANDLE hSection, DWORD dwOffset)
	{
		// Creates a bitmap and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBitmapOld)::DeleteObject(::SelectObject(m_hDC, m_hBitmapOld));

		HBITMAP hBitmap = ::CreateDIBSection(hdc, &bmi, iUsage, ppvBits, hSection, dwOffset);
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
	inline void CDC::CreateBrushIndirect( const LOGBRUSH& lb)
	{
		// Creates the brush and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBrushOld) ::DeleteObject(::SelectObject(m_hDC, m_hBrushOld));

		HBRUSH hBrush = ::CreateBrushIndirect(&lb);
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

	inline void CDC::CreateFontIndirect( const LOGFONT& lf)
	{
		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hFontOld) ::DeleteObject(::SelectObject(m_hDC, m_hFontOld));

		HFONT hFont = ::CreateFontIndirect(&lf);
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

	inline void CDC::CreatePenIndirect( const LOGPEN& lgpn)
	{
		// Creates the pen and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hPenOld) ::DeleteObject(::SelectObject(m_hDC, m_hPenOld));

		HPEN hPen = ::CreatePenIndirect(&lgpn);
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

	inline void CDC::CreateRectRgnIndirect( const RECT& rc)
	{
		// Creates a rectangular region from the rectangle co-ordinates.
		// The region will be deleted for you, unless its detached
		// Note: The shape of a region cannot be changed while it is attached to a DC

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hRgnOld) ::DeleteObject(m_hRgnOld);

		HRGN hRgn = ::CreateRectRgnIndirect(&rc);
		if (!hRgn) throw CWinException(_T("CreateRectRgnIndirect failed"));

		::SelectClipRgn(m_hDC, hRgn);
		m_hRgnOld = hRgn;
	}

	inline void CDC::ExtCreateRegion( const XFORM& Xform, DWORD nCount, const RGNDATA *pRgnData)
	{
		// Creates a region from the specified region data and tranformation data.
		// The region will be deleted for you, unless its detached
		// Notes: The shape of a region cannot be changed while it is attached to a DC
		//        GetRegionData can be used to get a region's data
		//        If the XFROM pointer is NULL, the identity transformation is used.

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hRgnOld) ::DeleteObject(m_hRgnOld);

		HRGN hRgn = ::ExtCreateRegion(&Xform, nCount, pRgnData);
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

#ifndef _WIN32_WCE
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

	inline void CDC::CreateEllipticRgnIndirect( const RECT& rc)
	{
		// Creates the ellyiptical region from the bounding rectangle co-ordinates
		// and selects it into the device context
		// The region will be deleted for you, unless its detached
		// Note: The shape of a region cannot be changed while it is attached to a DC

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hRgnOld) ::DeleteObject(m_hRgnOld);

		HRGN hRgn = ::CreateEllipticRgnIndirect(&rc);
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
#endif


	// Wrappers for WinAPI functions

	// Initialization
	inline HDC CDC::CreateCompatibleDC( ) const
	{
		return ::CreateCompatibleDC( m_hDC );
	}
	inline HDC CDC::CreateDC( LPCTSTR lpszDriver, LPCTSTR lpszDevice, LPCTSTR lpszOutput, const DEVMODE& dvmInit ) const
	{
		return ::CreateDC( lpszDriver, lpszDevice, lpszOutput, &dvmInit );
	}
	inline int CDC::GetDeviceCaps( int nIndex ) const
	{
		return ::GetDeviceCaps(m_hDC, nIndex);
	}
#ifndef _WIN32_WCE
	inline CWnd* CDC::WindowFromDC( ) const
	{
		return CWnd::FromHandle( ::WindowFromDC( m_hDC ) );
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
		::MoveToEx( m_hDC, 0, 0, &pt );
		::MoveToEx( m_hDC, pt.x, pt.y, NULL);
		return pt;
	}
	inline CPoint CDC::MoveTo( int x, int y ) const
	{
		// Updates the current position to the specified point
		return ::MoveToEx( m_hDC, x, y, NULL );
	}
	inline CPoint CDC::MoveTo( POINT pt ) const
	{
		// Updates the current position to the specified point
		return ::MoveToEx( m_hDC, pt.x, pt.y, NULL );
	}
	inline BOOL CDC::LineTo( int x, int y ) const
	{
		// Draws a line from the current position up to, but not including, the specified point
		return ::LineTo( m_hDC, x, y );
	}
	inline BOOL CDC::LineTo( POINT pt ) const
	{
		// Draws a line from the current position up to, but not including, the specified point
		return ::LineTo( m_hDC, pt.x, pt.y );
	}

#ifndef _WIN32_WCE
	inline BOOL CDC::Arc( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 ) const
	{
		return ::Arc( m_hDC, x1, y1, x2, y2, x3, y3, x4, y4 );
	}
	inline BOOL CDC::Arc( RECT& rc, POINT ptStart, POINT ptEnd ) const
	{
		// Draws an elliptical arc
		return ::Arc( m_hDC, rc.left, rc.top, rc.right, rc.bottom,
			ptStart.x, ptStart.y, ptEnd.x, ptEnd.y );
	}
	inline BOOL CDC::ArcTo( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 ) const
	{
		// Draws an elliptical arc
		return ::ArcTo( m_hDC, x1, y1, x2, y2, x3, y3, x4, y4) ;
	}
	inline BOOL CDC::ArcTo( RECT& rc, POINT ptStart, POINT ptEnd ) const
	{
		// Draws an elliptical arc
		return ::ArcTo(  m_hDC, rc.left, rc.top, rc.right, rc.bottom,
			ptStart.x, ptStart.y, ptEnd.x, ptEnd.y );
	}
	inline BOOL CDC::AngleArc( int x, int y, int nRadius, float fStartAngle, float fSweepAngle ) const
	{
		// Draws a line segment and an arc
		return ::AngleArc( m_hDC, x, y, nRadius, fStartAngle, fSweepAngle);
	}
	inline int CDC::GetArcDirection( ) const
	{
		// Retrieves the current arc direction ( AD_COUNTERCLOCKWISE or AD_CLOCKWISE )
		return ::GetArcDirection( m_hDC );
	}
	inline int CDC::SetArcDirection( int nArcDirection ) const
	{
		// Sets the current arc direction ( AD_COUNTERCLOCKWISE or AD_CLOCKWISE )
		return ::SetArcDirection( m_hDC, nArcDirection );
	}
	inline BOOL CDC::PolyDraw( const POINT* lpPoints, const BYTE* lpTypes, int nCount ) const
	{
		// Draws a set of line segments and Bzier curves
		return ::PolyDraw( m_hDC, lpPoints, lpTypes, nCount );
	}
	inline BOOL CDC::Polyline( LPPOINT lpPoints, int nCount ) const
	{
		// Draws a series of line segments by connecting the points in the specified array
		return ::Polyline( m_hDC, lpPoints, nCount );
	}
	inline BOOL CDC::PolyPolyline( const POINT* lpPoints, const DWORD* lpPolyPoints, int nCount ) const
	{
		// Draws multiple series of connected line segments
		return ::PolyPolyline( m_hDC, lpPoints, lpPolyPoints, nCount );
	}
	inline BOOL CDC::PolylineTo( const POINT* lpPoints, int nCount ) const
	{
		// Draws one or more straight lines
		return ::PolylineTo( m_hDC, lpPoints, nCount );
	}
	inline BOOL CDC::PolyBezier( const POINT* lpPoints, int nCount ) const
	{
		// Draws one or more Bzier curves
		return ::PolyBezier( m_hDC, lpPoints, nCount );
	}
	inline BOOL CDC::PolyBezierTo( const POINT* lpPoints, int nCount ) const
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
	inline COLORREF CDC::SetPixel( int x, int y, COLORREF crColor ) const
	{
		// Sets the pixel at the specified coordinates to the specified color
		return ::SetPixel( m_hDC, x, y, crColor );
	}
	inline COLORREF CDC::SetPixel( POINT pt, COLORREF crColor ) const
	{
		// Sets the pixel at the specified coordinates to the specified color
		return ::SetPixel( m_hDC, pt.x, pt.y, crColor );
	}
	inline BOOL CDC::SetPixelV( int x, int y, COLORREF crColor ) const
	{
		// Sets the pixel at the specified coordinates to the closest approximation of the specified color
		return ::SetPixelV( m_hDC, x, y, crColor );
	}
	inline BOOL CDC::SetPixelV( POINT pt, COLORREF crColor ) const
	{
		// Sets the pixel at the specified coordinates to the closest approximation of the specified color
		return ::SetPixelV( m_hDC, pt.x, pt.y, crColor );
	}
#endif

	// Shape Drawing Functions
	inline void CDC::DrawFocusRect( const RECT& rc ) const
	{
		// draws a rectangle in the style used to indicate that the rectangle has the focus
		::DrawFocusRect( m_hDC, &rc );
	}
	inline BOOL CDC::Ellipse( int x1, int y1, int x2, int y2 ) const
	{
		// Draws an ellipse. The center of the ellipse is the center of the specified bounding rectangle.
		return ::Ellipse( m_hDC, x1, y1, x2, y2 );
	}
	inline BOOL CDC::Ellipse( const RECT& rc ) const
	{
		// Draws an ellipse. The center of the ellipse is the center of the specified bounding rectangle.
		return ::Ellipse( m_hDC, rc.left, rc.top, rc.right, rc.bottom );
	}
	inline BOOL CDC::Polygon( LPPOINT lpPoints, int nCount ) const
	{
		// Draws a polygon consisting of two or more vertices connected by straight lines
		return ::Polygon( m_hDC, lpPoints, nCount);
	}
	inline BOOL CDC::Rectangle( int x1, int y1, int x2, int y2 ) const
	{
		// Draws a rectangle. The rectangle is outlined by using the current pen and filled by using the current brush.
		return ::Rectangle( m_hDC, x1, y1, x2, y2 );
	}
	inline BOOL CDC::Rectangle( const RECT& rc) const
	{
		// Draws a rectangle. The rectangle is outlined by using the current pen and filled by using the current brush.
		return ::Rectangle( m_hDC, rc.left, rc.top, rc.right, rc.bottom );
	}
	inline BOOL CDC::RoundRect( int x1, int y1, int x2, int y2, int nWidth, int nHeight ) const
	{
		// Draws a rectangle with rounded corners
		return ::RoundRect( m_hDC, x1, y1, x2, y2, nWidth, nHeight );
	}
	inline BOOL CDC::RoundRect( const RECT& rc, int nWidth, int nHeight ) const
	{
		// Draws a rectangle with rounded corners
		return ::RoundRect(m_hDC, rc.left, rc.top, rc.right, rc.bottom, nWidth, nHeight );
	}

#ifndef _WIN32_WCE
	inline BOOL CDC::Chord( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 ) const
	{
		// Draws a chord (a region bounded by the intersection of an ellipse and a line segment, called a secant)
		return ::Chord( m_hDC, x1, y1, x2, y2, x3, y3, x4, y4 );
	}
	inline BOOL CDC::Chord( const RECT& rc, POINT ptStart, POINT ptEnd ) const
	{
		// Draws a chord (a region bounded by the intersection of an ellipse and a line segment, called a secant)
		return ::Chord( m_hDC, rc.left, rc.top, rc.right, rc.bottom,
			ptStart.x, ptStart.y, ptEnd.x, ptEnd.y );
	}
	inline BOOL CDC::Pie( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 ) const
	{
		// Draws a pie-shaped wedge bounded by the intersection of an ellipse and two radials.
		return ::Pie( m_hDC, x1, y1, x2, y2, x3, y3, x4, y4 );
	}
	inline BOOL CDC::Pie( const RECT& rc, POINT ptStart, POINT ptEnd ) const
	{
		// Draws a pie-shaped wedge bounded by the intersection of an ellipse and two radials.
		return ::Pie( m_hDC, rc.left, rc.top, rc.right, rc.bottom,
			ptStart.x, ptStart.y, ptEnd.x, ptEnd.y );
	}
	inline BOOL CDC::PolyPolygon( LPPOINT lpPoints, LPINT lpPolyCounts, int nCount ) const
	{
		// Draws a series of closed polygons
		return ::PolyPolygon( m_hDC, lpPoints, lpPolyCounts, nCount );
	}
#endif

	// Fill and 3D Drawing functions
	inline BOOL CDC::FillRect( const RECT& rc, HBRUSH hbr ) const
	{
		// Fills a rectangle by using the specified brush
		return (BOOL)::FillRect( m_hDC, &rc, hbr );
	}
	inline BOOL CDC::InvertRect( const RECT& rc ) const
	{
		// Inverts a rectangle in a window by performing a logical NOT operation on the color values for each pixel in the rectangle's interior
		return ::InvertRect( m_hDC, &rc );
	}
	inline BOOL CDC::DrawIconEx( int xLeft, int yTop, HICON hIcon, int cxWidth, int cyWidth, UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw, UINT diFlags ) const
	{
		// draws an icon or cursor, performing the specified raster operations, and stretching or compressing the icon or cursor as specified.
		return ::DrawIconEx( m_hDC, xLeft, yTop, hIcon, cxWidth, cyWidth, istepIfAniCur, hbrFlickerFreeDraw, diFlags );
	}
	inline BOOL CDC::DrawEdge( const RECT& rc, UINT nEdge, UINT nFlags ) const
	{
		// Draws one or more edges of rectangle
		return ::DrawEdge( m_hDC, (LPRECT)&rc, nEdge, nFlags );
	}
	inline BOOL CDC::DrawFrameControl( const RECT& rc, UINT nType, UINT nState ) const
	{
		// Draws a frame control of the specified type and style
		return ::DrawFrameControl( m_hDC, (LPRECT)&rc, nType, nState );
	}
	inline BOOL CDC::FillRgn( HRGN hrgn, HBRUSH hbr ) const
	{
		// Fills a region by using the specified brush
		return ::FillRgn( m_hDC, hrgn, hbr );
	}

#ifndef _WIN32_WCE
	inline BOOL CDC::DrawIcon( int x, int y, HICON hIcon ) const
	{
		// Draws an icon or cursor
		return ::DrawIcon( m_hDC, x, y, hIcon );
	}
	inline BOOL CDC::DrawIcon( POINT pt, HICON hIcon ) const
	{
		// Draws an icon or cursor
		return ::DrawIcon( m_hDC, pt.x, pt.y, hIcon );
	}
	inline BOOL CDC::FrameRect( const RECT& rc, HBRUSH hbr ) const
	{
		// Draws a border around the specified rectangle by using the specified brush
		return (BOOL)::FrameRect( m_hDC, &rc, hbr );
	}
	inline BOOL CDC::PaintRgn( HRGN hrgn ) const
	{
		// Paints the specified region by using the brush currently selected into the device context
		return ::PaintRgn( m_hDC, hrgn);
	}
#endif

	// Bitmap Functions
	inline int CDC::StretchDIBits( int XDest, int YDest, int nDestWidth, int nDestHeight, int XSrc, int YSrc, int nSrcWidth,
		           int nSrcHeight, CONST VOID *lpBits, BITMAPINFO& bi, UINT iUsage, DWORD dwRop ) const
	{
		// Copies the color data for a rectangle of pixels in a DIB to the specified destination rectangle
		return ::StretchDIBits( m_hDC, XDest, YDest, nDestWidth, nDestHeight, XSrc, YSrc, nSrcWidth, nSrcHeight, lpBits, &bi, iUsage, dwRop );
	}

	inline BOOL CDC::PatBlt( int x, int y, int nWidth, int nHeight, DWORD dwRop ) const
	{
		// Paints the specified rectangle using the brush that is currently selected into the device context
		return ::PatBlt( m_hDC, x, y, nWidth, nHeight, dwRop );
	}
	inline BOOL CDC::BitBlt( int x, int y, int nWidth, int nHeight, HDC hSrcDC, int xSrc, int ySrc, DWORD dwRop ) const
	{
		// Performs a bit-block transfer of the color data corresponding to a rectangle of pixels from the specified source device context into a destination device context
		return ::BitBlt( m_hDC, x, y, nWidth, nHeight, hSrcDC, xSrc, ySrc, dwRop );
	}
	inline BOOL CDC::StretchBlt( int x, int y, int nWidth, int nHeight, HDC hSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop ) const
	{
		// Copies a bitmap from a source rectangle into a destination rectangle, stretching or compressing the bitmap to fit the dimensions of the destination rectangle, if necessary
		return ::StretchBlt( m_hDC, x, y, nWidth, nHeight, hSrcDC, xSrc, ySrc, nSrcWidth, nSrcHeight, dwRop );
	}

#ifndef _WIN32_WCE
	inline int CDC::GetDIBits( HBITMAP hbmp, UINT uStartScan, UINT cScanLines, LPVOID lpvBits, LPBITMAPINFO lpbi, UINT uUsage ) const
	{
		// Retrieves the bits of the specified compatible bitmap and copies them into a buffer as a DIB using the specified format
		return ::GetDIBits( m_hDC, hbmp, uStartScan, cScanLines, lpvBits, lpbi, uUsage );
	}
	inline int CDC::SetDIBits( HBITMAP hbmp, UINT uStartScan, UINT cScanLines, CONST VOID *lpvBits, LPBITMAPINFO lpbi, UINT fuColorUse ) const
	{
		// Sets the pixels in a compatible bitmap (DDB) using the color data found in the specified DIB
		return ::SetDIBits( m_hDC, hbmp, uStartScan, cScanLines, lpvBits, lpbi, fuColorUse );
	}
	inline int CDC::GetStretchBltMode( ) const
	{
		// Retrieves the current stretching mode
		// Possible modes: BLACKONWHITE, COLORONCOLOR, HALFTONE, STRETCH_ANDSCANS, STRETCH_DELETESCANS, STRETCH_HALFTONE, STRETCH_ORSCANS, WHITEONBLACK
		return ::GetStretchBltMode( m_hDC );
	}
	inline int CDC::SetStretchBltMode( int iStretchMode ) const
	{
		// Sets the stretching mode
		// Possible modes: BLACKONWHITE, COLORONCOLOR, HALFTONE, STRETCH_ANDSCANS, STRETCH_DELETESCANS, STRETCH_HALFTONE, STRETCH_ORSCANS, WHITEONBLACK
		return ::SetStretchBltMode( m_hDC, iStretchMode );
	}
	inline BOOL CDC::FloodFill( int x, int y, COLORREF crColor ) const
	{
		// Fills an area of the display surface with the current brush
		return ::FloodFill(m_hDC, x, y, crColor );
	}
	inline BOOL CDC::ExtFloodFill( int x, int y, COLORREF crColor, UINT nFillType ) const
	{
		// Fills an area of the display surface with the current brush
		// Fill type: FLOODFILLBORDER or FLOODFILLSURFACE
		return ::ExtFloodFill(m_hDC, x, y, crColor, nFillType );
	}
#endif

	// co-ordingate functions
	inline BOOL CDC::DPtoLP(LPPOINT lpPoints, int nCount )  const
	{
		if (m_hDC != NULL)
			return ::DPtoLP(m_hDC, lpPoints, nCount);
		return FALSE;
	}
	inline BOOL CDC::DPtoLP(LPRECT lpRect)  const
	{
		if (m_hDC != NULL)
			return ::DPtoLP(m_hDC, (LPPOINT)lpRect, 2);
		return FALSE;
	}
	
	inline BOOL CDC::LPtoDP(LPPOINT lpPoints, int nCount )  const
	{
		if (m_hDC != NULL)
			return ::LPtoDP(m_hDC, lpPoints, nCount);
		return FALSE;
	}
	inline BOOL CDC::LPtoDP(LPRECT lpRect)  const
	{
		if (m_hDC != NULL)
			return ::LPtoDP(m_hDC, (LPPOINT)lpRect, 2);
		return FALSE;
	}

	// Layout Functions
	inline DWORD CDC::GetLayout() const
	{
		// Returns the layout of a device context (LAYOUT_RTL and LAYOUT_BITMAPORIENTATIONPRESERVED)
#if defined(WINVER) && (WINVER >= 0x0500)
		return ::GetLayout(m_hDC);
#else
		return 0;
#endif
	}
	inline DWORD CDC::SetLayout(DWORD dwLayout) const
	{
#if defined(WINVER) && (WINVER >= 0x0500)
		// Sets the layout of a device context
		return ::SetLayout(m_hDC, dwLayout);
#else
		UNREFERENCED_PARAMETER(dwLayout); // no-op
		return 0;
#endif
	}

	// Mapping Functions
	inline int CDC::GetMapMode()  const
	{
		if (m_hDC != NULL)
			return ::GetMapMode(m_hDC);
		return 0;
	}
	inline BOOL CDC::GetViewportOrgEx(LPPOINT lpPoint)  const
	{
		if (m_hDC != NULL)
			return ::GetViewportOrgEx(m_hDC, lpPoint);
		return 0;
	}
	inline int CDC::SetMapMode(int nMapMode) const
	{
		if (m_hDC != NULL)
			return ::SetMapMode(m_hDC, nMapMode);
		return 0;
	}	
	inline BOOL CDC::SetViewportOrgEx(int x, int y, LPPOINT lpPoint) const
	{
		if (m_hDC != NULL)
			return ::SetViewportOrgEx(m_hDC, x, y, lpPoint);
		return FALSE;
	}
	inline BOOL CDC::SetViewportOrgEx(POINT point, LPPOINT lpPointRet ) const
	{
		if (m_hDC != NULL)
			return SetViewportOrgEx(point.x, point.y, lpPointRet);
		return FALSE;
	}	
	inline BOOL CDC::OffsetViewportOrgEx(int nWidth, int nHeight, LPPOINT lpPoint ) const
	{
		if (m_hDC != NULL)
			return ::OffsetViewportOrgEx(m_hDC, nWidth, nHeight, lpPoint);
		return FALSE;
	}
	inline BOOL CDC::GetViewportExtEx(LPSIZE lpSize)  const
	{
		if (m_hDC != NULL)
			return ::GetViewportExtEx(m_hDC, lpSize);
		return FALSE;
	}
	inline BOOL CDC::SetViewportExtEx(int x, int y, LPSIZE lpSize ) const
	{
		if (m_hDC != NULL)
			return ::SetViewportExtEx(m_hDC, x, y, lpSize);
		return FALSE;
	}	
	inline BOOL CDC::SetViewportExtEx(SIZE size, LPSIZE lpSizeRet ) const
	{
		if (m_hDC != NULL)
			return SetViewportExtEx(size.cx, size.cy, lpSizeRet);
		return FALSE;
	}
	inline BOOL CDC::ScaleViewportExtEx(int xNum, int xDenom, int yNum, int yDenom, LPSIZE lpSize ) const
	{
		if (m_hDC != NULL)
			return ::ScaleViewportExtEx(m_hDC, xNum, xDenom, yNum, yDenom, lpSize);
		return FALSE;
	}	
	inline BOOL CDC::GetWindowOrgEx(LPPOINT lpPoint)  const
	{
		if (m_hDC != NULL)
			return ::GetWindowOrgEx(m_hDC, lpPoint);
		return FALSE;
	}	
	inline BOOL CDC::SetWindowOrgEx(int x, int y, LPPOINT lpPoint ) const
	{
		if (m_hDC != NULL)
			return ::SetWindowOrgEx(m_hDC, x, y, lpPoint);
		return FALSE;
	}
	inline BOOL CDC::SetWindowOrgEx(POINT point, LPPOINT lpPointRet ) const
	{
		if (m_hDC != NULL)
			return SetWindowOrgEx(point.x, point.y, lpPointRet);
		return FALSE;
	}
	inline BOOL CDC::OffsetWindowOrgEx(int nWidth, int nHeight, LPPOINT lpPoint ) const
	{
		if (m_hDC != NULL)
			return ::OffsetWindowOrgEx(m_hDC, nWidth, nHeight, lpPoint);
		return FALSE;
	}
	inline BOOL CDC::GetWindowExtEx(LPSIZE lpSize)  const
	{
		if (m_hDC != NULL)
			return ::GetWindowExtEx(m_hDC, lpSize);
		return FALSE;
	}	
	inline BOOL CDC::SetWindowExtEx(int x, int y, LPSIZE lpSize ) const
	{
		if (m_hDC != NULL)
			return ::SetWindowExtEx(m_hDC, x, y, lpSize);
		return FALSE;
	}
	inline BOOL CDC::SetWindowExtEx(SIZE size, LPSIZE lpSizeRet) const
	{
		if (m_hDC != NULL)
			return SetWindowExtEx(size.cx, size.cy, lpSizeRet);
		return FALSE;
	}
	inline BOOL CDC::ScaleWindowExtEx(int xNum, int xDenom, int yNum, int yDenom, LPSIZE lpSize) const
	{
		if (m_hDC != NULL)
			return ::ScaleWindowExtEx(m_hDC, xNum, xDenom, yNum, yDenom, lpSize);
		return FALSE;
	}

	// Printer Functions
	inline int CDC::StartDoc(LPDOCINFO lpDocInfo) const
	{
		return ::StartDoc(m_hDC, lpDocInfo);
	}
	inline int CDC::EndDoc() const
	{
		return ::EndDoc(m_hDC);
	}
	inline int CDC::StartPage() const
	{
		return ::StartPage(m_hDC);
	}
	inline int CDC::EndPage() const
	{
		return ::EndPage(m_hDC);
	}
	inline int CDC::AbortDoc() const
	{
		return ::AbortDoc(m_hDC);
	}
	inline int CDC::SetAbortProc(BOOL (CALLBACK* lpfn)(HDC, int)) const
	{
		return ::SetAbortProc(m_hDC, lpfn);
	}

	// Text Functions
	inline BOOL CDC::ExtTextOut( int x, int y, UINT nOptions, const RECT& rc, LPCTSTR lpszString, UINT nCount, LPINT lpDxWidths ) const
	{
		// Draws text using the currently selected font, background color, and text color
		return ::ExtTextOut(m_hDC, x, y, nOptions, &rc, lpszString, nCount, lpDxWidths );
	}

	inline int CDC::DrawText( LPCTSTR lpszString, int nCount, const RECT& rc, UINT nFormat ) const
	{
		// Draws formatted text in the specified rectangle
		return ::DrawText(m_hDC, lpszString, nCount, (LPRECT)&rc, nFormat );
	}
	inline UINT CDC::GetTextAlign( ) const
	{
		// Retrieves the text-alignment setting
		// Values: TA_BASELINE, TA_BOTTOM, TA_TOP, TA_CENTER, TA_LEFT, TA_RIGHT, TA_RTLREADING, TA_NOUPDATECP, TA_UPDATECP
		return ::GetTextAlign( m_hDC );
	}
	inline UINT CDC::SetTextAlign( UINT nFlags ) const
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
	inline BOOL CDC::GetTextMetrics( TEXTMETRIC& Metrics ) const
	{
		// Fills the specified buffer with the metrics for the currently selected font
		return ::GetTextMetrics( m_hDC, &Metrics );
	}
	inline COLORREF CDC::GetBkColor( ) const
	{
		// Returns the current background color
		return ::GetBkColor( m_hDC );
	}
	inline COLORREF CDC::SetBkColor( COLORREF crColor ) const
	{
		// Sets the current background color to the specified color value
		return ::SetBkColor(m_hDC, crColor );
	}
	inline COLORREF CDC::GetTextColor( ) const
	{
		// Retrieves the current text color
		return ::GetTextColor( m_hDC);
	}
	inline COLORREF CDC::SetTextColor( COLORREF crColor ) const
	{
		// Sets the current text color
		return ::SetTextColor( m_hDC, crColor );
	}
	inline int CDC::GetBkMode( ) const
	{
		// returns the current background mix mode (OPAQUE or TRANSPARENT)
		return ::GetBkMode( m_hDC );
	}
	inline int CDC::SetBkMode( int iBkMode ) const
	{
		// Sets the current background mix mode (OPAQUE or TRANSPARENT)
		return ::SetBkMode( m_hDC, iBkMode);
	}
	inline CSize CDC::GetTextExtentPoint( LPCTSTR lpszString, int nCount ) const
	{
		// Computes the width and height of the specified string of text
		CSize sz;
		::GetTextExtentPoint(m_hDC, lpszString, nCount, &sz );
		return sz;
	}
#ifndef _WIN32_WCE
	inline BOOL CDC::TextOut( int x, int y, LPCTSTR lpszString, int nCount ) const
	{
		// Writes a character string at the specified location
		return ::TextOut( m_hDC, x, y, lpszString, nCount );
	}
	inline int CDC::DrawTextEx( LPTSTR lpszString, int nCount, const RECT& rc, UINT nFormat, const DRAWTEXTPARAMS& DTParams ) const
	{
		// Draws formatted text in the specified rectangle with more formatting options
		return ::DrawTextEx(m_hDC, lpszString, nCount, (LPRECT)&rc, nFormat, (LPDRAWTEXTPARAMS)&DTParams );
	}
	inline CSize CDC::TabbedTextOut( int x, int y, LPCTSTR lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin ) const
	{
		// Writes a character string at a specified location, expanding tabs to the values specified in an array of tab-stop positions
		DWORD dwSize = ::TabbedTextOut(m_hDC, x, y, lpszString, nCount, nTabPositions, lpnTabStopPositions, nTabOrigin );
		CSize sz(dwSize);
		return sz;
	}
	inline CSize CDC::GetTabbedTextExtent( LPCTSTR lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions ) const
	{
		// Computes the width and height of a character string
		DWORD dwSize = ::GetTabbedTextExtent(m_hDC, lpszString, nCount, nTabPositions, lpnTabStopPositions );
		CSize sz(dwSize);
		return sz;
	}
	inline BOOL CDC::GrayString( HBRUSH hBrush, GRAYSTRINGPROC lpOutputFunc, LPARAM lpData, int nCount, int x, int y, int nWidth, int nHeight ) const
	{
		// Draws gray text at the specified location
		return ::GrayString(m_hDC, hBrush, lpOutputFunc, lpData, nCount, x, y, nWidth, nHeight );
	}
	inline int CDC::SetTextJustification( int nBreakExtra, int nBreakCount  ) const
	{
		// Specifies the amount of space the system should add to the break characters in a string of text
		return ::SetTextJustification( m_hDC, nBreakExtra, nBreakCount  );
	}
	inline int CDC::GetTextCharacterExtra( ) const
	{
		// Retrieves the current intercharacter spacing for the device context
		return ::GetTextCharacterExtra( m_hDC );
	}
	inline int CDC::SetTextCharacterExtra( int nCharExtra ) const
	{
		// Sets the intercharacter spacing
		return ::SetTextCharacterExtra( m_hDC, nCharExtra );
	}
	inline CSize CDC::GetTextExtentPoint32( LPCTSTR lpszString, int nCount ) const
	{
		// Computes the width and height of the specified string of text
		CSize sz;
		::GetTextExtentPoint32(m_hDC, lpszString, nCount, &sz );
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
		byte* lpvBits = new byte[pbmi->bmiHeader.biSizeImage];
		if (NULL == lpvBits) throw std::bad_alloc();
		MemDC.GetDIBits(hbmSource, 0, pbmi->bmiHeader.biHeight, lpvBits, pbmi, DIB_RGB_COLORS);
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
				xOffset += pbmi->bmiHeader.biBitCount >> 3;
			}

			// Increment vertical offset
			yOffset += nWidthBytes;
		}

		// Save the modified colour back into our source DDB
		MemDC.SetDIBits(hbmSource, 0, pbmi->bmiHeader.biHeight, lpvBits, pbmi, DIB_RGB_COLORS);

		// Cleanup
		delete []lpvBits;
	}

	inline void GrayScaleBitmap( HBITMAP hbmSource )
	{
		// Create our LPBITMAPINFO object
		CBitmapInfoPtr pbmi(hbmSource);

		// Create the reference DC for GetDIBits to use
		CDC MemDC = CreateCompatibleDC(NULL);

		// Use GetDIBits to create a DIB from our DDB, and extract the colour data
		MemDC.GetDIBits(hbmSource, 0, pbmi->bmiHeader.biHeight, NULL, pbmi, DIB_RGB_COLORS);
		byte* lpvBits = new byte[pbmi->bmiHeader.biSizeImage];
		if (NULL == lpvBits) throw std::bad_alloc();
		MemDC.GetDIBits(hbmSource, 0, pbmi->bmiHeader.biHeight, lpvBits, pbmi, DIB_RGB_COLORS);
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

				BYTE byGray = (BYTE) ((lpvBits[Index] + lpvBits[Index+1]*6 + lpvBits[Index+2] *3)/10);
				lpvBits[Index]   = byGray;
				lpvBits[Index+1] = byGray;
				lpvBits[Index+2] = byGray;

				// Increment the horizontal offset
				xOffset += pbmi->bmiHeader.biBitCount >> 3;
			}

			// Increment vertical offset
			yOffset += nWidthBytes;
		}

		// Save the modified colour back into our source DDB
		MemDC.SetDIBits(hbmSource, 0, pbmi->bmiHeader.biHeight, lpvBits, pbmi, DIB_RGB_COLORS);

		// Cleanup
		delete []lpvBits;
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

