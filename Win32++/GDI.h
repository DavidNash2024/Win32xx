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

////////////////////////////////////////////////////////
// GDI.h
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
//	::MoveToEx(DrawDC, 0, 0, NULL);
//  ::LineTo(DrawDC, 50, 50);
//
// When the CDC object drops out of scope, it's destructor is called, cleaning up
//  the GDI objects and device context.
//
// Notes:
//  * A device context assigned to a CDC object will be released or deleted, unless
//     it is detached.
//  * A GDI object created by one of the CDC member, or one attached to the CDC
//     object (except regions) will be deleted when the CDC object is destroyed,
//     unless it is detached.


#ifndef GDI_H
#define GDI_H

#include "WinCore.h"

namespace Win32xx
{

	////////////////////////////////////////////////
	// Declarations for some useful functions
	//
	void GradientFill(HDC hDC, COLORREF Color1, COLORREF Color2, LPRECT pRc, BOOL bVertical);
	void SolidFill(HDC hDC, COLORREF Color, LPRECT pRc);


	///////////////////////////////////////////////
	// Declarations for the CDC class
	//
	class CDC
	{
	public:
		CDC();
		CDC(HDC hDC);
		void operator = (const HDC hDC);
		virtual ~CDC();
		CDC(const CDC& rhs);	// Copy constructor

		virtual void AttachDC(HDC hDC);
		virtual HDC  DetachDC();

		// Create and Select Bitmaps
		virtual void AttachBitmap(HBITMAP hBitmap);
		virtual void CreateBitmap(int cx, int cy, UINT Planes, UINT BitsPerPixel, CONST VOID *pvColors);
		virtual void CreateCompatibleBitmap(HDC hDC, int cx, int cy);
		virtual void CreateDIBSection(HDC hdc, CONST BITMAPINFO *pbmi, UINT iUsage, VOID **ppvBits,
										HANDLE hSection, DWORD dwOffset);
		virtual HBITMAP DetachBitmap();

		// Create and Select Brushes
		virtual void AttachBrush(HBRUSH hBrush);
		virtual void CreateDIBPatternBrushPt(const VOID *lpPackedDIB, UINT iUsage);
		virtual void CreatePatternBrush(HBITMAP hbmp);
		virtual void CreateSolidBrush(COLORREF rbg);
		virtual HBRUSH DetachBrush();

		// Create and Select Fonts
		virtual void AttachFont(HFONT hFont);
		virtual void CreateFontIndirect(CONST LOGFONT* pLF);
		virtual HFONT DetachFont();

		// Create and Select Pens
		virtual void AttachPen(HPEN hPen);
		virtual void CreatePen(int nStyle, int nWidth, COLORREF rgb);
		virtual void CreatePenIndirect(const LOGPEN *lplgpn);
		virtual HPEN DetachPen();

		// Select Regions
		virtual void AttachRegion(HRGN hRegion);

		// Cast the CDC object to a HDC
		operator HDC() const {return m_hDC;}

#ifndef _WIN32_WCE
		// Create and Select Bitmaps
		virtual void CreateBitmapIndirect(CONST BITMAP *lpbm);
		virtual void CreateDIBitmap(HDC hdc, CONST BITMAPINFOHEADER *lpbmih, DWORD fdwInit, CONST VOID *lpbInit,
										CONST BITMAPINFO *lpbmi,  UINT fuUsage);
		// Create and Select Brushes
		virtual void CreateBrushIndirect(CONST LOGBRUSH *lplb);
		virtual void CreateDIBPatternBrush(HGLOBAL hglbDIBPacked, UINT fuColorSpec);
		virtual void CreateHatchBrush(int fnStyle, COLORREF rgb);

		// Create and Select Fonts
		virtual void CreateFont(int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight,
  							DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet,
  							DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality,
  							DWORD fdwPitchAndFamily, LPCTSTR lpszFace);
#endif

	private:
		CDC& operator = (const CDC&);	// Disable assignment operator

		HDC m_hDC;
		HBITMAP m_hBitmapOld;
		HBRUSH m_hBrushOld;
		HFONT m_hFontOld;
		HPEN m_hPenOld;
		BOOL m_IsCopy;
	};

} // namespace Win32xx

#endif // GDI_H

