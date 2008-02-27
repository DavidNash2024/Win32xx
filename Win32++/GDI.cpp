// Win32++  Version 6.0
// Released: 4th March, 2008 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: http://users.bigpond.net.au/programming/
//
//
// Copyright (c) 2005-2008  David Nash
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

#include "WinCore.h"
#include "gdi.h"


namespace Win32xx
{

	CDC::CDC() : m_hDC(0), m_hBitmapOld(0), m_hBrushOld(0), m_hFontOld(0),
		m_hPenOld(0), m_IsCopy(FALSE)
	{
	}

	CDC::CDC(HDC hDC) : m_hDC(0), m_hBitmapOld(0), m_hBrushOld(0), m_hFontOld(0),
		m_hPenOld(0), m_IsCopy(FALSE)
	{
		// This constructor assigns an existing HDC to the CDC
		// The HDC WILL be released or deleted when the CDC object is destroyed
		if (!hDC) throw CWinException(_T("Can't attach a NULL hDC"));

		m_hDC = hDC;

		// Note: this constructor permits a call like this:
		// CDC MyCDC = SomeHDC;
		//  or
		// CDC MyCDC = ::CreateCompatibleDC(SomeHDC);
		//  or
		// CDC MyCDC = ::GetDC(SomeHWND);
	}

	CDC::CDC(const CDC& rhs)	// Copy constructor
	{
		m_hBitmapOld = rhs.m_hBitmapOld;
		m_hBrushOld  = rhs.m_hBrushOld;
		m_hDC		 = rhs.m_hDC;
		m_hFontOld	 = rhs.m_hFontOld;
		m_hPenOld    = rhs.m_hPenOld;

		// This CDC is a copy, so we won't need to delete GDI resources
		//  in the destructor
		m_IsCopy     = TRUE;
	}

	void CDC::operator = (const HDC hDC)
	{
		AttachDC(hDC);
	}

	CDC::~CDC()
	{
		// We don't delete the CDC's resources if the CDC is just a copy
		if ((m_hDC) && !m_IsCopy)
		{
			// Delete any GDI objects belonging to this CDC
			if (m_hPenOld)    ::DeleteObject(::SelectObject(m_hDC, m_hPenOld));
			if (m_hBrushOld)  ::DeleteObject(::SelectObject(m_hDC, m_hBrushOld));
			if (m_hBitmapOld) ::DeleteObject(::SelectObject(m_hDC, m_hBitmapOld));
			if (m_hFontOld)	  ::DeleteObject(::SelectObject(m_hDC, m_hFontOld));

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

	void CDC::AttachDC(HDC hDC)
	{
		if (m_hDC) throw CWinException(_T("Device Context ALREADY assigned"));
		if (!hDC) throw CWinException(_T("Can't attach a NULL hDC"));

		m_hDC = hDC;
	}


	HDC CDC::DetachDC()
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
	void CDC::AttachBitmap(HBITMAP hBitmap)
	{
		// Use this to attach an existing bitmap.
		// The bitmap will be deleted for you, unless its detached

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (!hBitmap) throw CWinException(_T("Can't attach a NULL HBITMAP"));

		// Delete any existing bitmap
		if (m_hBitmapOld) ::DeleteObject(::SelectObject(m_hDC, m_hBitmapOld));

		m_hBitmapOld = (HBITMAP)::SelectObject(m_hDC, hBitmap);
	}

	void CDC::CreateCompatibleBitmap(HDC hDC, int cx, int cy)
	{
		// Creates a compatible bitmap and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBitmapOld) ::DeleteObject(::SelectObject(m_hDC, m_hBitmapOld));

		HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, cx, cy);
		if (!hBitmap) throw CWinException(_T("CreateCompatibleBitmap failed"));

		m_hBitmapOld = (HBITMAP)::SelectObject(m_hDC, hBitmap);
	}

	void CDC::CreateBitmap(int cx, int cy, UINT Planes, UINT BitsPerPixel, CONST VOID *pvColors)
	{
		// Creates a bitmap and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBitmapOld) ::DeleteObject(::SelectObject(m_hDC, m_hBitmapOld));

		HBITMAP hBitmap = ::CreateBitmap(cx, cy, Planes, BitsPerPixel, pvColors);
		if (!hBitmap) throw CWinException(_T("CreateBitmap failed"));

		m_hBitmapOld = (HBITMAP)::SelectObject(m_hDC, hBitmap);
	}

#ifndef _WIN32_WCE
	void CDC::CreateBitmapIndirect(CONST BITMAP *lpbm)
	{
		// Creates a bitmap and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBitmapOld) ::DeleteObject(::SelectObject(m_hDC, m_hBitmapOld));

		HBITMAP hBitmap = ::CreateBitmapIndirect(lpbm);
		if (!hBitmap) throw CWinException(_T("CreateBitmap failed"));

		m_hBitmapOld = (HBITMAP)::SelectObject(m_hDC, hBitmap);
	}

	void CDC::CreateDIBitmap(HDC hdc, CONST BITMAPINFOHEADER *lpbmih, DWORD fdwInit, CONST VOID *lpbInit,
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

	void CDC::CreateDIBSection(HDC hdc, CONST BITMAPINFO *pbmi, UINT iUsage, VOID **ppvBits,
										HANDLE hSection, DWORD dwOffset)
	{
		// Creates a bitmap and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBitmapOld)::DeleteObject(::SelectObject(m_hDC, m_hBitmapOld));

		HBITMAP hBitmap = ::CreateDIBSection(hdc, pbmi, iUsage, ppvBits, hSection, dwOffset);
		if (!hBitmap) throw CWinException(_T("CreateDIBSection failed"));

		m_hBitmapOld = (HBITMAP)::SelectObject(m_hDC, hBitmap);
	}

	HBITMAP CDC::DetachBitmap()
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
	void CDC::AttachBrush(HBRUSH hBrush)
	{
		// Use this to attach an existing brush.
		// The brush will be deleted for you, unless its detached

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (!hBrush) throw CWinException(_T("Can't attach a NULL HBRUSH"));
		if (m_hBrushOld) ::DeleteObject(::SelectObject(m_hDC, m_hBrushOld));
		m_hBrushOld = (HBRUSH)::SelectObject(m_hDC, hBrush);
	}

#ifndef _WIN32_WCE
	void CDC::CreateBrushIndirect(CONST LOGBRUSH *lplb)
	{
		// Creates the brush and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBrushOld) ::DeleteObject(::SelectObject(m_hDC, m_hBrushOld));

		HBRUSH hBrush = ::CreateBrushIndirect(lplb);
		if (!hBrush) throw CWinException(_T("CreateBrusIndirect failed"));

		m_hBrushOld = (HBRUSH)::SelectObject(m_hDC, hBrush);
	}

	void CDC::CreateDIBPatternBrush(HGLOBAL hglbDIBPacked, UINT fuColorSpec)
	{
		// Creates the brush and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBrushOld) ::DeleteObject(::SelectObject(m_hDC, m_hBrushOld));

		HBRUSH hBrush = ::CreateDIBPatternBrush(hglbDIBPacked, fuColorSpec);
		if (!hBrush) throw CWinException(_T("CreateDIBPatternBrush failed"));

		m_hBrushOld = (HBRUSH)::SelectObject(m_hDC, hBrush);
	}

	void CDC::CreateHatchBrush(int fnStyle, COLORREF rgb)
	{
		// Creates the brush and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBrushOld) ::DeleteObject(::SelectObject(m_hDC, m_hBrushOld));

		HBRUSH hBrush = ::CreateHatchBrush(fnStyle, rgb);
		if (!hBrush) throw CWinException(_T("CreateHatchBrush failed"));

		m_hBrushOld = (HBRUSH)::SelectObject(m_hDC, hBrush);
	}
#endif

	void CDC::CreateDIBPatternBrushPt(CONST VOID *lpPackedDIB, UINT iUsage)
	{
		// Creates the brush and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBrushOld) ::DeleteObject(::SelectObject(m_hDC, m_hBrushOld));

		HBRUSH hBrush = ::CreateDIBPatternBrushPt(lpPackedDIB, iUsage);
		if (!hBrush) throw CWinException(_T("CreateDIBPatternPrushPt failed"));

		m_hBrushOld = (HBRUSH)::SelectObject(m_hDC, hBrush);
	}

	void CDC::CreatePatternBrush(HBITMAP hbmp)
	{
		// Creates the brush and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBrushOld) ::DeleteObject(::SelectObject(m_hDC, m_hBrushOld));

		HBRUSH hBrush = ::CreatePatternBrush(hbmp);
		if (!hBrush) throw CWinException(_T("CreatePatternBrush failed"));

		m_hBrushOld = (HBRUSH)::SelectObject(m_hDC, hBrush);
	}

	void CDC::CreateSolidBrush(COLORREF rgb)
	{
		// Creates the brush and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hBrushOld) ::DeleteObject(::SelectObject(m_hDC, m_hBrushOld));

		HBRUSH hBrush = ::CreateSolidBrush(rgb);
		if (!hBrush) throw CWinException(_T("CreateSolidBrush failed"));

		m_hBrushOld = (HBRUSH)::SelectObject(m_hDC, hBrush);
	}

	HBRUSH CDC::DetachBrush()
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
	void CDC::AttachFont(HFONT hFont)
	{
		// Use this to attach an existing font.
		// The font will be deleted for you, unless its detached

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (!hFont) throw CWinException(_T("Can't attach a NULL HFONT"));
		if (m_hFontOld) ::DeleteObject(::SelectObject(m_hDC, m_hFontOld));
		m_hFontOld = (HFONT)::SelectObject(m_hDC, hFont);
	}

#ifndef _WIN32_WCE
	void CDC::CreateFont(
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

	void CDC::CreateFontIndirect(CONST LOGFONT* pLF)
	{
		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hFontOld) ::DeleteObject(::SelectObject(m_hDC, m_hFontOld));

		HFONT hFont = ::CreateFontIndirect(pLF);
		if (!hFont) throw CWinException(_T("CreateFontIndirect failed"));

		m_hFontOld = (HFONT)::SelectObject(m_hDC, hFont);
	}

	HFONT CDC::DetachFont()
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
	void CDC::AttachPen(HPEN hPen)
	{
		// Use this to attach an existing pen.
		// The pen will be deleted for you, unless its detached

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (!hPen) throw CWinException(_T("Can't attach a NULL HPEN"));
		if (m_hPenOld) ::DeleteObject(::SelectObject(m_hDC, m_hPenOld));
		m_hPenOld = (HPEN)::SelectObject(m_hDC, hPen);
	}

	void CDC::CreatePen(int nStyle, int nWidth, COLORREF rgb)
	{
		// Creates the pen and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hPenOld) ::DeleteObject(::SelectObject(m_hDC, m_hPenOld));

		HPEN hPen = ::CreatePen(nStyle, nWidth, rgb);
		if (!hPen) throw CWinException(_T("CreatePen failed"));

		m_hPenOld = (HPEN)::SelectObject(m_hDC, hPen);
	}

	void CDC::CreatePenIndirect(const LOGPEN *lplgpn)
	{
		// Creates the pen and selects it into the device context

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (m_hPenOld) ::DeleteObject(::SelectObject(m_hDC, m_hPenOld));

		HPEN hPen = ::CreatePenIndirect(lplgpn);
		if (!hPen) throw CWinException(_T("CreatePenIndirect failed"));

		m_hPenOld = (HPEN)::SelectObject(m_hDC, hPen);
	}

	HPEN CDC::DetachPen()
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
	void CDC::AttachRegion(HRGN hRegion)
	{
		// Use this to attach an existing region.
		// You are responible for deleting the region

		if (!m_hDC) throw CWinException(_T("Device Context not assigned"));
		if (!hRegion) throw CWinException(_T("Can't attach a NULL HRGN"));

		::SelectObject(m_hDC, hRegion);
	}

} // namespace Win32xx

/////////////////////////////////////////////////////////////////
// Definitions for some global functions in the Win32xx namespace
//

void Win32xx::GradientFill(HDC hDC, COLORREF Color1, COLORREF Color2, LPRECT pRc, BOOL bVertical)
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
		for(int i=0; i < Width; i++)
		{
			int r = r1 + (i * (r2-r1) / Width);
			int g = g1 + (i * (g2-g1) / Width);
			int b = b1 + (i * (b2-b1) / Width);
			::SetBkColor(hDC, RGB(r, g, b));
			RECT line;

			::SetRect(&line, i + pRc->left, pRc->top, i + 1 + pRc->left, pRc->top+Height);
			::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &line, NULL, 0, NULL);
		}
	}
	else
	{
		for(int i=0; i < Height; i++)
		{
			int r = r1 + (i * (r2-r1) / Height);
			int g = g1 + (i * (g2-g1) / Height);
			int b = b1 + (i * (b2-b1) / Height);
			::SetBkColor(hDC, RGB(r, g, b));
			RECT line;

			::SetRect(&line, pRc->left, i + pRc->top, pRc->left+Width, i + 1 +pRc->top);
			::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &line, NULL, 0, NULL);
		}
	}

	::SetBkColor(hDC, OldBkColor);
}

void Win32xx::SolidFill(HDC hDC, COLORREF Color, LPRECT pRc)
// Fills a rectangle with a solid color
{
	COLORREF OldColor = ::SetBkColor(hDC, Color);
	::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, pRc, NULL, 0, NULL);
	::SetBkColor(hDC, OldColor);
}

