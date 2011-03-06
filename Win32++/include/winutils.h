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
	// Forward declarations
	class CPoint;
	class CRect;

	/////////////////////////////////////////
	// Definition of the CSize class
	// This class can be used to replace the SIZE structure
	class CSize : public SIZE
	{
	public:
		CSize()								{ cx = 0; cy = 0; }
		CSize(int CX, int CY)				{ cx = CX; cy = CY; }
		CSize(SIZE sz)						{ cx = sz.cx; cy = sz.cy; }
		CSize(POINT pt)						{ cx = pt.x;  cy = pt.y; }
		CSize(DWORD dw)						{ cx = (short)LOWORD(dw); cy = (short)HIWORD(dw); }
		void SetSize(int CX, int CY)		{ cx = CX; cy = CY; }
		
		// Operators
		BOOL operator == (SIZE sz)			{ return (cx == sz.cx && cy == sz.cy); }
		BOOL operator != (SIZE sz)			{ return (cx != sz.cx || cy != sz.cy); }
		operator LPSIZE()					{ return this; }
		void operator += (SIZE sz)			{ cx += sz.cx; cy += sz.cy; }
		void operator -= (SIZE sz)			{ cx -= sz.cx; cy -= sz.cy; }

		// Operators returning CSize
		CSize operator - () const			{ return CSize (-cx, -cy); }
		CSize operator + (SIZE sz) const	{ return CSize (cx + sz.cx, cy + sz.cy); }
		CSize operator - (SIZE sz) const	{ return CSize (cx - sz.cx, cy - sz.cy); }

		// Operators returning CPoint
		CPoint operator + (POINT point) const;
		CPoint operator - (POINT point) const;

		// Operators returning CRect
		CRect operator + (RECT rc) const;
		CRect operator - (RECT rc) const;
	};


	/////////////////////////////////////////
	// Definition of the CPoint class
	// This class can be used to replace the POINT structure
	class CPoint : public POINT
	{
	public:
		CPoint()							{ x = 0; y = 0; }
		CPoint(int X, int Y)				{ x = X; y = Y; }
		CPoint(POINT pt)					{ x = pt.x ; y = pt.y; }
		CPoint(POINTS pts)					{ x = pts.x; y = pts.y; }
		CPoint(SIZE sz)						{ x = sz.cx; y = sz.cy; }
		CPoint(DWORD dw)					{ x = (short) LOWORD(dw); y = (short) HIWORD(dw); }
		
		void Offset(int dx, int dy)			{ x += dx; y += dy; }
		void Offset(POINT pt)				{ x += pt.x; y += pt.y; }
		void Offset(SIZE sz)				{ x += sz.cx; y += sz.cy; }
		void SetPoint(int X, int Y)			{ x = X; y = Y; }

		// Operators
		operator LPPOINT()					{ return this; }
		BOOL operator == (POINT pt) const	{ return ((x == pt.x) && (y == pt.y)); }
		BOOL operator != (POINT pt) const	{ return ((x != pt.x) || (y != pt.y)); }
		void operator += (SIZE sz)			{ x += sz.cx; y += sz.cy; }
		void operator -= (SIZE sz)			{ x -= sz.cx; y -= sz.cy; }
		void operator += (POINT pt)			{ x += pt.x; y += pt.y; }
		void operator -= (POINT pt)			{ x -= pt.x; y -= pt.y; }
		
		// Operators returning CPoint
		CPoint operator - () const			{ return CPoint(-x, -y); }
		CPoint operator + (SIZE sz) const	{ return CPoint(x + sz.cx, y + sz.cy); }
		CPoint operator - (SIZE sz) const	{ return CPoint(x - sz.cx, y - sz.cy); }
		CPoint operator + (POINT pt) const	{ return CPoint(x + pt.x, y + pt.y); }
		CPoint operator - (POINT pt) const	{ return CPoint(x - pt.x, y - pt.y); }
		
		// Operators returning CRect
		CRect operator + (RECT rc) const;
		CRect operator - (RECT rc) const;
	};

	CPoint GetCursorPos();


	/////////////////////////////////////////
	// Definition of the CRect class
	// This class can be used to replace the RECT structure.
	class CRect : public RECT
	{
	public:
		CRect()										{ left = top = right = bottom = 0; }
		CRect(int l, int t, int r, int b)			{ left = l; top = t; right = r; bottom = b; }
		CRect(RECT rc)								{ left = rc.left; top = rc.top; right = rc.right; bottom = rc.bottom; }
		CRect(POINT pt, SIZE sz)					{ right = (left = pt.x) + sz.cx; bottom = (top = pt.y) + sz.cy; }
		CRect(POINT topLeft, POINT bottomRight)		{ left = topLeft.x; top = topLeft.y; right = bottomRight.x; bottom = bottomRight.y; }

		BOOL CopyRect(RECT rc)						{ return ::CopyRect(this, &rc); }
		BOOL DeflateRect(int x, int y)				{ return ::InflateRect(this, -x, -y); }
		BOOL DeflateRect(SIZE size)					{ return ::InflateRect(this, -size.cx, -size.cy); }
		BOOL DeflateRect(RECT rc)					{ return ::InflateRect(this, rc.left - rc.right, rc.top - rc.bottom); }
		BOOL DeflateRect(int l, int t, int r, int b){ return ::InflateRect(this, l - r, t - b); }
		BOOL EqualRect(RECT rc) const				{ return ::EqualRect(&rc, this); }
		BOOL InflateRect(int dx, int dy)			{ return ::InflateRect(this, dx, dy); }
		BOOL InflateRect(SIZE sz)					{ return ::InflateRect(this, sz.cx, sz.cy); }
		BOOL InflateRect(RECT rc)					{ return ::InflateRect(this, rc.right - rc.left, rc.bottom - rc.top); }
		BOOL InflateRect(int l, int t, int r, int b){ return ::InflateRect(this, r - l, b - t); }
		BOOL IntersectRect(RECT rc1, RECT rc2)		{ return ::IntersectRect(this, &rc1, &rc2); }
		BOOL IsRectEmpty() const					{ return ::IsRectEmpty(this);}
		BOOL IsRectNull() const						{ return (left == 0 && right == 0 && top == 0 && bottom == 0); }	
		CRect MulDiv(int nMult, int nDiv) const		{ return CRect ((left * nMult) / nDiv, (top * nMult) / nDiv,
														(right * nMult) / nDiv, (bottom * nMult) / nDiv); }
		void NormalizeRect()						{ int nTemp; if (left > right) { nTemp = left; left = right; right = nTemp; }
														if (top > bottom) { nTemp = top; top = bottom; bottom = nTemp; } }
		BOOL OffsetRect(int dx, int dy)				{ return ::OffsetRect(this, dx, dy); }
		BOOL OffsetRect(POINT pt)					{ return ::OffsetRect(this, pt.x, pt.y); }
		BOOL OffsetRect(SIZE size)					{ return ::OffsetRect(this, size.cx, size.cy); }
		BOOL PtInRect(POINT pt) const				{ return ::PtInRect(this, pt); }
		BOOL SetRect(int l, int t, int r, int b)	{ return ::SetRect(this, l, t, r, b); }
		BOOL SetRect(POINT TopLeft, POINT BtmRight)	{ return ::SetRect(this, TopLeft.x, TopLeft.y, BtmRight.x, BtmRight.y); }
		BOOL SetRectEmpty()							{ return ::SetRectEmpty(this); }
		BOOL SubtractRect(RECT rc1, RECT rc2)		{ return ::SubtractRect(this, &rc1, &rc2); }
		BOOL UnionRect(RECT rc1, RECT rc2)			{ return ::UnionRect(this, &rc1, &rc2); }

		// Reposition rectangle
		void MoveToY (int y)						{ bottom = Height() + y; top = y; }
		void MoveToX (int x)						{ right = Width() + x; left = x; }
		void MoveToXY (int x, int y)				{ MoveToX(x); MoveToY(y); }
		void MoveToXY (POINT pt)					{ MoveToX (pt.x); MoveToY (pt.y); }

		// Attributes
		int Height() const							{ return bottom - top; }	
		int Width() const							{ return right - left; }
		CSize Size() const							{ return CSize(Width(), Height()); }
		CPoint CenterPoint() const					{ return CPoint((left + right) / 2, (top + bottom) / 2); }
		CPoint TopLeft() const						{ return CPoint(left, top); }
		CPoint BottomRight() const					{ return CPoint(right, bottom); }

		// operators
		operator LPRECT()							{ return this; }
		BOOL operator == (RECT rc)					{ return ::EqualRect(this, &rc); }
		BOOL operator != (RECT rc)					{ return !::EqualRect(this, &rc); }
		void operator += (POINT pt)					{ ::OffsetRect(this, pt.x, pt.y); }
		void operator += (SIZE size)				{ ::OffsetRect(this, size.cx, size.cy); }
		void operator += (RECT rc)					{ ::InflateRect(this, rc.right - rc.left, rc.bottom - rc.top); }
		void operator -= (RECT rc)					{ ::InflateRect(this, rc.left - rc.right, rc.top - rc.bottom); }
		void operator -= (POINT pt)					{ ::OffsetRect(this, -pt.x, -pt.y); }
		void operator -= (SIZE sz)					{ ::OffsetRect(this, -sz.cx, -sz.cy); }
		void operator &= (RECT rc)					{ ::IntersectRect(this, this, &rc); }
		void operator |= (RECT rc)					{ ::UnionRect(this, this, &rc); }

		// Operators returning CRect
		CRect operator + (POINT pt) const			{ CRect rc(*this); ::OffsetRect(&rc, pt.x, pt.y); return rc; }
		CRect operator - (POINT pt) const			{ CRect rc(*this); ::OffsetRect(&rc, -pt.x, -pt.y); return rc; }
		CRect operator + (SIZE sz) const			{ CRect rc(*this); ::OffsetRect(&rc, sz.cx, sz.cy); return rc; }
		CRect operator - (SIZE sz) const			{ CRect rc(*this); ::OffsetRect(&rc, -sz.cx, -sz.cy); return rc; }
		CRect operator + (RECT rc) const			{ CRect rc1(*this); rc1.InflateRect(rc); return rc1; }
		CRect operator - (RECT rc) const			{ CRect rc1(*this); rc1.DeflateRect(rc); return rc1; }
		CRect operator & (RECT rc) const			{ CRect rc1; ::IntersectRect(&rc1, this, &rc); return rc1; }
		CRect operator | (RECT rc) const			{ CRect rc1; ::UnionRect(&rc1, this, &rc); return rc1; }
	};

	// CPoint member function definitions
	inline CPoint CSize::operator + (POINT pt) const	{ return CPoint(pt) + *this; }
	inline CPoint CSize::operator - (POINT pt) const	{ return CPoint(pt) - *this; }
	inline CRect CSize::operator + (RECT rc) const		{ return CRect(rc) + *this; }
	inline CRect CSize::operator - (RECT rc) const		{ return CRect(rc) - *this; }

	// CSize member function definitions
	inline CRect CPoint::operator + (RECT rc) const		{ return CRect(rc) + *this; }
	inline CRect CPoint::operator - (RECT rc) const		{ return CRect(rc) - *this; }


	////////////////////////////////////////////////////////
	// Classes and functions (typedefs) for text conversions
	//
	//  This section defines the following text conversions:
	//  A2BSTR		ANSI  to BSTR
	//  A2OLE		ANSI  to OLE
	//	A2T			ANSI  to TCHAR
	//	A2W			ANSI  to WCHAR
	//  OLE2A		OLE   to ANSI
	//  OLE2T		OLE   to TCHAR
	//  OLE2W		OLE   to WCHAR
	//  T2A			TCHAR to ANSI
	//  T2BSTR		TCHAR to BSTR
	//  T2OLE       TCHAR to OLE
	//  T2W			TCHAR to WCHAR
	//  W2A			WCHAR to ANSI
	//  W2BSTR		WCHAR to BSTR
	//  W2OLE		WCHAR to OLE
	//  W2T			WCHAR to TCHAR

	// About different character and string types:
	// ------------------------------------------
	// char (or CHAR) character types are ANSI (8 bits).
	// wchar_t (or WCHAR) character types are Unicode (16 bits).
	// TCHAR characters are Unicode if the _UNICODE macro is defined, otherwise they are ANSI.
	// BSTR (Basic String) is a type of string used in Visual Basic and COM programming.
	// OLE is the same as WCHAR. It is used in Visual Basic and COM programming.


	// Forward declarations of our classes. They are defined later.
	class CA2A;
	class CA2W;
	class CW2A;
	class CW2W;
	class CA2BSTR;
	class CW2BSTR;

	// typedefs for the well known text conversions
	typedef CA2W A2W;
	typedef CW2A W2A;
	typedef CW2BSTR W2BSTR;
	typedef CA2BSTR A2BSTR;
	typedef CW2A BSTR2A;
	typedef CW2W BSTR2W;

#ifdef _UNICODE
	typedef CA2W A2T;
	typedef CW2A T2A;
	typedef CW2W T2W;
	typedef CW2W W2T;
	typedef CW2BSTR T2BSTR;
#else
	typedef CA2A A2T;
	typedef CA2A T2A;
	typedef CA2W T2W;
	typedef CW2A W2T;
	typedef CA2BSTR T2BSTR;
	typedef BSTR2A BSTR2T;
#endif

	typedef A2W  A2OLE;
	typedef T2W  T2OLE;
	typedef CW2W W2OLE;
	typedef W2A  OLE2A;
	typedef W2T  OLE2T;
	typedef CW2W OLE2W;

	class CA2W
	{
	public:
		CA2W(LPCSTR pStr) : m_pStr(pStr)
		{
			if (pStr)
			{
				// Resize the vector and assign null WCHAR to each element
				int length = (int)strlen(pStr)+1;
				m_vWideArray.assign(length, L'\0');

				// Fill our vector with the converted WCHAR array
				MultiByteToWideChar(CP_ACP, 0, pStr, -1, &m_vWideArray.front(), length);
			}
		}
		~CA2W() {}
		operator LPCWSTR() { return m_pStr? &m_vWideArray.front() : NULL; }
		operator LPOLESTR() { return m_pStr? (LPOLESTR)&m_vWideArray.front() : (LPOLESTR)NULL; }
		operator LPBSTR() { return m_pStr? (LPBSTR)&m_vWideArray.front() : (LPBSTR)NULL; }

	private:
		CA2W(const CA2W&);
		CA2W& operator= (const CA2W&);
		std::vector<wchar_t> m_vWideArray;
		LPCSTR m_pStr;
	};

	class CW2A
	{
	public:
		CW2A(LPCWSTR pWStr) : m_pWStr(pWStr)
		{
			// Resize the vector and assign null char to each element
			int length = (int)wcslen(pWStr)+1;
			m_vAnsiArray.assign(length, '\0');

			// Fill our vector with the converted char array
			WideCharToMultiByte(CP_ACP, 0, pWStr, -1, &m_vAnsiArray.front(), length, NULL,NULL);
		}

		~CW2A() {}
		operator LPCSTR() { return m_pWStr? &m_vAnsiArray.front() : NULL; }

	private:
		CW2A(const CW2A&);
		CW2A& operator= (const CW2A&);
		std::vector<char> m_vAnsiArray;
		LPCWSTR m_pWStr;
	};

	class CW2W
	{
	public:
		CW2W(LPCWSTR pWStr) : m_pWStr(pWStr) {}
		operator LPCWSTR() { return (LPWSTR)m_pWStr; }
		operator LPOLESTR() { return (LPOLESTR)m_pWStr; }

	private:
		CW2W(const CW2W&);
		CW2W& operator= (const CW2W&);

		LPCWSTR m_pWStr;
	};

	class CA2A
	{
	public:
		CA2A(LPCSTR pStr) : m_pStr(pStr) {}
		operator LPCSTR() { return (LPSTR)m_pStr; }

	private:
		CA2A(const CA2A&);
		CA2A& operator= (const CA2A&);

		LPCSTR m_pStr;
	};

	class CW2BSTR
	{
	public:
		CW2BSTR(LPCWSTR pWStr) { m_bstrString = ::SysAllocString(pWStr); }
		~CW2BSTR() { ::SysFreeString(m_bstrString); }
		operator BSTR() { return m_bstrString;}

	private:
		CW2BSTR(const CW2BSTR&);
		CW2BSTR& operator= (const CW2BSTR&);
		BSTR m_bstrString;
	};

	class CA2BSTR
	{
	public:
		CA2BSTR(LPCSTR pStr) { m_bstrString = ::SysAllocString(A2W(pStr)); }
		~CA2BSTR() { ::SysFreeString(m_bstrString); }
		operator BSTR() { return m_bstrString;}

	private:
		CA2BSTR(const CA2BSTR&);
		CA2BSTR& operator= (const CA2BSTR&);
		BSTR m_bstrString;
	};
	
}


#endif
