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

#ifndef _WINUTILS_H_
#define _WINUTILS_H_


// define useful macros from WindowsX.h
#ifndef GET_X_LPARAM
  #define GET_X_LPARAM(lp)  ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
  #define GET_Y_LPARAM(lp)  ((int)(short)HIWORD(lp))
#endif

// Define our own MIN and MAX macros
// this avoids inconsistencies with Dev-C++ and other compilers, and
// avoids conflicts between typical min/max macros and std::min/std::max
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))


namespace Win32xx
{

	/////////////////////////////////////////
	// Definition of the CSize class
	// This class can be used to replace the SIZE structure
	class CSize : public SIZE
	{
	public:
		CSize()						{ cx = 0; cy = 0; }
		CSize(int CX, int CY)		{ cx = CX; cy = CY; }
		CSize(SIZE sz)				{ cx = sz.cx; cy = sz.cy; }
		CSize(POINT pt)				{ cx = pt.x;  cy = pt.y; }
		CSize(DWORD dw)				{ cx = (short)LOWORD(dw); cy = (short)HIWORD(dw); }
		void SetSize(int CX, int CY){ cx = CX; cy = CY; }
		BOOL operator == (SIZE sz)	{ return (cx == sz.cx && cy == sz.cy); }
		BOOL operator != (SIZE sz)	{ return (cx != sz.cx || cy != sz.cy); }
		operator LPSIZE()			{ return this; }
	};

	/////////////////////////////////////////
	// Definition of the CPoint class
	// This class can be used to replace the POINT structure
	class CPoint : public POINT
	{
	public:
		CPoint()					{ x = 0; y = 0; }
		CPoint(int X, int Y)		{ x = X; y = Y; }
		CPoint(SIZE sz)				{ x = sz.cx; y = sz.cy; }
		CPoint(POINT pt)			{ x = pt.x ; y = pt.y; }
		CPoint(DWORD dw)			{ x = (short)LOWORD(dw); y = (short)HIWORD(dw); }
		void Offset(int dx, int dy)	{ x += dx; y += dy; }
		void SetPoint(int X, int Y)	{ x = X; y = Y; }
		BOOL operator == (POINT pt)	{ return ((x == pt.x) && (y == pt.y)); }
		BOOL operator != (POINT pt)	{ return ((x != pt.x) || (y != pt.y)); }
		operator LPPOINT()			{ return this; }
	};

	CPoint GetCursorPos();


	/////////////////////////////////////////
	// Definition of the CRect class
	// This class can be used to replace the RECT structure.
	class CRect : public RECT
	{
	public:
		CRect()
		{ left = top = right = bottom = 0; }

		CRect(int Left, int Top, int Right, int Bottom)
		{ left = Left; top = Top; right = Right; bottom = Bottom; }

		CRect(RECT rc)
		{ left = rc.left; top = rc.top; right = rc.right; bottom = rc.bottom; }

		CRect(POINT pt, SIZE sz)
		{ right = (left = pt.x) + sz.cx; bottom = (top = pt.y) + sz.cy; }

		CRect(POINT topLeft, POINT bottomRight)
		{ left = topLeft.x; top = topLeft.y; right = bottomRight.x; bottom = bottomRight.y; }

		operator LPRECT()
		{ return this; }

		operator LPCRECT() const
		{ return this; }

		BOOL operator == (const RECT& rc)
		{ return ::EqualRect(this, &rc); }

		BOOL operator != (const RECT& rc)
		{ return !::EqualRect(this, &rc); }

		void  operator=(const RECT& srcRect)
		{ ::CopyRect(this, &srcRect); }

		void CopyRect(const RECT& rc)
		{ ::CopyRect(this, &rc); }

		BOOL EqualRect(const RECT& rc) const
		{ return ::EqualRect(&rc, this); }

		int Height() const
		{ return bottom - top; }
		
		BOOL InflateRect(int dx, int dy)
		{ return ::InflateRect(this, dx, dy); }

		BOOL IntersectRect(const RECT& rc1, const RECT& rc2)
		{ return ::IntersectRect(this, &rc1, &rc2); }

		BOOL IsRectEmpty() const
		{ return ::IsRectEmpty(this);}

		BOOL OffsetRect(int dx, int dy)
		{ return ::OffsetRect(this, dx, dy); }

		BOOL PtInRect(POINT pt) const
		{ return ::PtInRect(this, pt); }

		BOOL SetRect(int left, int top, int right, int bottom)
		{ return ::SetRect(this, left, top, right, bottom); }

		BOOL SetRectEmpty()
		{ return ::SetRectEmpty(this); }

		BOOL SubtractRect(const RECT& rc1, const RECT& rc2)
		{ return ::SubtractRect(this, &rc1, &rc2); }

		BOOL UnionRect(const RECT& rc1, const RECT& rc2)
		{ return ::UnionRect(this, &rc1, &rc2); }
		
		int Width() const
		{ return right - left; }
	};

}

#endif
