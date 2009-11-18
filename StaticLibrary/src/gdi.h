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


#endif // _GDI_H_

