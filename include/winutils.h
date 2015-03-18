// Win32++   Version 7.8
// Release Date: 17th March 2015
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2015  David Nash
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


///////////////////////////////////////////////////////
// winutils.h
//  Declaration of the following classes:
//  CPoint, CRect, and CSize

#ifndef _WIN32XX_WINUTILS_H_
#define _WIN32XX_WINUTILS_H_


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
		operator LPSIZE()					{ return this; }
		BOOL operator == (SIZE sz) const	{ return (cx == sz.cx && cy == sz.cy); }
		BOOL operator != (SIZE sz) const	{ return (cx != sz.cx || cy != sz.cy); }
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
		CRect operator + (LPCRECT prc) const;
		CRect operator - (LPCRECT prc) const;
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
		CPoint(LPARAM dwPos)				{ x = GET_X_LPARAM(dwPos); y = GET_Y_LPARAM(dwPos); }

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
		CRect operator + (LPCRECT prc) const;
		CRect operator - (LPCRECT prc) const;
	};


	/////////////////////////////////////////
	// Definition of the CRect class
	// This class can be used to replace the RECT structure.
	class CRect : public RECT
	{
	public:
		CRect()										{ left = top = right = bottom = 0; }
		CRect(int l, int t, int r, int b)			{ left = l; top = t; right = r; bottom = b; }
		CRect(const RECT& rc)						{ left = rc.left; top = rc.top; right = rc.right; bottom = rc.bottom; }
		CRect(POINT pt, SIZE sz)					{ right = (left = pt.x) + sz.cx; bottom = (top = pt.y) + sz.cy; }
		CRect(POINT topLeft, POINT bottomRight)		{ left = topLeft.x; top = topLeft.y; right = bottomRight.x; bottom = bottomRight.y; }

		BOOL CopyRect(LPCRECT prc)					{ return ::CopyRect(this, prc); }
		BOOL DeflateRect(int x, int y)				{ return ::InflateRect(this, -x, -y); }
		BOOL DeflateRect(SIZE size)					{ return ::InflateRect(this, -size.cx, -size.cy); }
		void DeflateRect(LPCRECT prc)				{ left += prc->left; top += prc->top; right -= prc->right; bottom -= prc->bottom; }
		void DeflateRect(int l, int t, int r, int b){ left += l; top += t; right -= r; bottom -= b; }
		BOOL EqualRect(LPRECT prc) const			{ return ::EqualRect(prc, this); }
		BOOL InflateRect(int dx, int dy)			{ return ::InflateRect(this, dx, dy); }
		BOOL InflateRect(SIZE sz)					{ return ::InflateRect(this, sz.cx, sz.cy); }
		void InflateRect(LPCRECT prc)				{ left -= prc->left; top -= prc->top; right += prc->right; bottom += prc->bottom; }
		void InflateRect(int l, int t, int r, int b){ left -= l; top -= t; right += r; bottom += b; }
		BOOL IntersectRect(LPCRECT prc1, LPCRECT prc2)	{ return ::IntersectRect(this, prc1, prc2); }
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
		BOOL SubtractRect(LPCRECT prc1, LPCRECT prc2)	{ return ::SubtractRect(this, prc1, prc2); }
		BOOL UnionRect(LPCRECT prc1, LPCRECT prc2)	{ return ::UnionRect(this, prc1, prc2); }

		// Reposition rectangle
		void MoveToX (int x)						{ right = Width() + x; left = x; }
		void MoveToY (int y)						{ bottom = Height() + y; top = y; }
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
		operator LPCRECT() const					{ return this; }
		BOOL operator == (const RECT& rc) const		{ return ::EqualRect(this, &rc); }
		BOOL operator != (const RECT& rc) const		{ return !::EqualRect(this, &rc); }
		void operator += (POINT pt)					{ ::OffsetRect(this, pt.x, pt.y); }
		void operator += (SIZE size)				{ ::OffsetRect(this, size.cx, size.cy); }
		void operator += (LPCRECT prc)				{ ::InflateRect(this, prc->right - prc->left, prc->bottom - prc->top); }
		void operator -= (LPCRECT prc)				{ ::InflateRect(this, prc->left - prc->right, prc->top - prc->bottom); }
		void operator -= (POINT pt)					{ ::OffsetRect(this, -pt.x, -pt.y); }
		void operator -= (SIZE sz)					{ ::OffsetRect(this, -sz.cx, -sz.cy); }
		void operator &= (const RECT& rc)			{ ::IntersectRect(this, this, &rc); }
		void operator |= (const RECT& rc)			{ ::UnionRect(this, this, &rc); }

		// Operators returning CRect
		CRect operator + (POINT pt) const			{ CRect rc(*this); ::OffsetRect(&rc, pt.x, pt.y); return rc; }
		CRect operator - (POINT pt) const			{ CRect rc(*this); ::OffsetRect(&rc, -pt.x, -pt.y); return rc; }
		CRect operator + (SIZE sz) const			{ CRect rc(*this); ::OffsetRect(&rc, sz.cx, sz.cy); return rc; }
		CRect operator - (SIZE sz) const			{ CRect rc(*this); ::OffsetRect(&rc, -sz.cx, -sz.cy); return rc; }
		CRect operator + (LPRECT prc) const			{ CRect rc1(*this); rc1.InflateRect(prc); return rc1; }
		CRect operator - (LPRECT prc) const			{ CRect rc1(*this); rc1.DeflateRect(prc); return rc1; }
		CRect operator & (const RECT& rc) const		{ CRect rc1; ::IntersectRect(&rc1, this, &rc); return rc1; }
		CRect operator | (const RECT& rc) const		{ CRect rc1; ::UnionRect(&rc1, this, &rc); return rc1; }
	};

	// CSize member function definitions
	inline CPoint CSize::operator + (POINT pt) const	{ return CPoint(pt) + *this; }
	inline CPoint CSize::operator - (POINT pt) const	{ return CPoint(pt) - *this; }
	inline CRect CSize::operator + (LPCRECT prc) const		{ return CRect(*prc) + *this; }
	inline CRect CSize::operator - (LPCRECT prc) const		{ return CRect(*prc) - *this; }

	// CPoint member function definitions
	inline CRect CPoint::operator + (LPCRECT prc) const		{ return CRect(*prc) + *this; }
	inline CRect CPoint::operator - (LPCRECT prc) const		{ return CRect(*prc) - *this; }


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
	typedef BSTR2W BSTR2T;
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
		CA2W(LPCSTR pStr, UINT codePage = CP_ACP) : m_pStr(pStr)
		{
			if (pStr)
			{
				// Resize the vector and assign null WCHAR to each element
				int length = MultiByteToWideChar(codePage, 0, pStr, -1, NULL, 0) + 1;
				m_vWideArray.assign(length, L'\0');

				// Fill our vector with the converted WCHAR array
				MultiByteToWideChar(codePage, 0, pStr, -1, &m_vWideArray[0], length);
			}
		}
		~CA2W() {}
		operator LPCWSTR() { return m_pStr? &m_vWideArray[0] : NULL; }
		operator LPOLESTR() { return m_pStr? (LPOLESTR)&m_vWideArray[0] : (LPOLESTR)NULL; }

	private:
		CA2W(const CA2W&);
		CA2W& operator= (const CA2W&);
		std::vector<wchar_t> m_vWideArray;
		LPCSTR m_pStr;
	};

	class CW2A
	{
	public:
		CW2A(LPCWSTR pWStr, UINT codePage = CP_ACP) : m_pWStr(pWStr)
		// Usage:
		//   CW2A ansiString(L"Some Text");
		//   CW2A utf8String(L"Some Text", CP_UTF8);
		//
		// or
		//   SetWindowTextA( W2A(L"Some Text") ); The ANSI version of SetWindowText
		{
			// Resize the vector and assign null char to each element
			int length = WideCharToMultiByte(codePage, 0, pWStr, -1, NULL, 0, NULL, NULL) + 1;
			m_vAnsiArray.assign(length, '\0');

			// Fill our vector with the converted char array
			WideCharToMultiByte(codePage, 0, pWStr, -1, &m_vAnsiArray[0], length, NULL,NULL);
		}

		~CW2A() {}
		operator LPCSTR() { return m_pWStr? &m_vAnsiArray[0] : NULL; }

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

	////////////////////////////////////////////////
	// Declarations of structures for themes
	//

	// Defines the theme colors for the MenuBar and popup menues.
	// The popup menu colors are replaced by the Aero theme if available (Vista and above)
	struct MenuTheme
	{
		BOOL UseThemes;			// TRUE if themes are used
		COLORREF clrHot1;		// Colour 1 for top menu. Color of selected menu item
		COLORREF clrHot2;		// Colour 2 for top menu. Color of checkbox
		COLORREF clrPressed1;	// Colour 1 for pressed top menu and side bar
		COLORREF clrPressed2;	// Colour 2 for pressed top menu and side bar
		COLORREF clrOutline;	// Colour for border outline
	};

	// Defines the theme colors and options for the ReBar
	struct ReBarTheme
	{
		BOOL UseThemes;			// TRUE if themes are used
		COLORREF clrBkgnd1;		// Colour 1 for rebar background
		COLORREF clrBkgnd2;		// Colour 2 for rebar background
		COLORREF clrBand1;		// Colour 1 for rebar band background. Use NULL if not required
		COLORREF clrBand2;		// Colour 2 for rebar band background. Use NULL if not required
		BOOL FlatStyle;			// Bands are rendered with flat rather than raised style
		BOOL BandsLeft;			// Position bands left on rearrange
		BOOL LockMenuBand;		// Lock MenuBar's band in dedicated top row, without gripper
		BOOL RoundBorders;		// Use rounded band borders
		BOOL ShortBands;        // Allows bands to be shorter than maximum available width
		BOOL UseLines;			// Displays horizontal lines between bands
	};

	// Defines the theme colors and options for the StatusBar
	struct StatusBarTheme
	{
		BOOL UseThemes;			// TRUE if themes are used
		COLORREF clrBkgnd1;		// Colour 1 for statusbar background
		COLORREF clrBkgnd2;		// Colour 2 for statusbar background
	};

	// Defines the theme colors for the ToolBar
	struct ToolBarTheme
	{
		BOOL UseThemes;			// TRUE if themes are used
		COLORREF clrHot1;		// Colour 1 for hot button
		COLORREF clrHot2;		// Colour 2 for hot button
		COLORREF clrPressed1;	// Colour 1 for pressed button
		COLORREF clrPressed2;	// Colour 2 for pressed button
		COLORREF clrOutline;	// Colour for border outline
	};

	////////////////////////////////////////
	// Global Functions
	//

	inline CWinApp* GetApp()
	// Returns a pointer to the CWinApp derived class
	{
		return CWinApp::SetnGetThis();
	}

	inline CPoint GetCursorPos()
	{
		CPoint pt;
		::GetCursorPos(&pt);
		return pt;
	}

	inline void TRACE(LPCSTR str)
	// TRACE sends a string to the debug/output pane, or an external debugger
	{
  #ifdef _DEBUG
		OutputDebugString(A2T(str));
  #else
		UNREFERENCED_PARAMETER(str); // no-op
  #endif
	}

	inline void TRACE(LPCWSTR str)
	// TRACE sends a string to the debug/output pane, or an external debugger
	{
  #ifdef _DEBUG
		OutputDebugString(W2T(str));
  #else
		UNREFERENCED_PARAMETER(str); // no-op
  #endif
	}


  #ifndef _WIN32_WCE		// for Win32/64 operating systems, not WinCE

	inline int GetWinVersion()
	{
		DWORD dwVersion = GetVersion();
		int Platform = (dwVersion < 0x80000000)? 2:1;
		int MajorVer = LOBYTE(LOWORD(dwVersion));
		int MinorVer = HIBYTE(LOWORD(dwVersion));

		int nVersion =  1000*Platform + 100*MajorVer + MinorVer;

		// Return values and window versions:
		//  1400     Windows 95
		//  1410     Windows 98
		//  1490     Windows ME
		//  2400     Windows NT
		//  2500     Windows 2000
		//  2501     Windows XP
		//  2502     Windows Server 2003
		//  2600     Windows Vista and Windows Server 2008
		//  2601     Windows 7

		return nVersion;
	}

	inline int GetComCtlVersion()
	{
		// Load the Common Controls DLL
		HMODULE hComCtl = ::LoadLibrary(_T("COMCTL32.DLL"));
		if (hComCtl == 0)
			return 0;

		int ComCtlVer = 400;

		if (::GetProcAddress(hComCtl, "InitCommonControlsEx"))
		{
			// InitCommonControlsEx is unique to 4.7 and later
			ComCtlVer = 470;

			if (::GetProcAddress(hComCtl, "DllGetVersion"))
			{
				typedef HRESULT CALLBACK DLLGETVERSION(DLLVERSIONINFO*);
				DLLGETVERSION* pfnDLLGetVersion = NULL;

				pfnDLLGetVersion = (DLLGETVERSION*)::GetProcAddress(hComCtl, "DllGetVersion");
				if(pfnDLLGetVersion)
				{
					DLLVERSIONINFO dvi;
					dvi.cbSize = sizeof dvi;
					if(NOERROR == pfnDLLGetVersion(&dvi))
					{
						DWORD dwVerMajor = dvi.dwMajorVersion;
						DWORD dwVerMinor = dvi.dwMinorVersion;
						ComCtlVer = 100 * dwVerMajor + dwVerMinor;
					}
				}
			}
			else if (::GetProcAddress(hComCtl, "InitializeFlatSB"))
				ComCtlVer = 471;	// InitializeFlatSB is unique to version 4.71
		}

		::FreeLibrary(hComCtl);

		// return values and DLL versions
		// 400  dll ver 4.00	Windows 95/Windows NT 4.0
		// 470  dll ver 4.70	Internet Explorer 3.x
		// 471  dll ver 4.71	Internet Explorer 4.0
		// 472  dll ver 4.72	Internet Explorer 4.01 and Windows 98
		// 580  dll ver 5.80	Internet Explorer 5
		// 581  dll ver 5.81	Windows 2000 and Windows ME
		// 582  dll ver 5.82	Windows XP or Vista without XP themes
		// 600  dll ver 6.00	Windows XP with XP themes
		// 610  dll ver 6.10	Windows Vista with XP themes
		// 616  dll ver 6.16    Windows Vista SP1 or Windows 7 with XP themes

		return ComCtlVer;
	}

	inline UINT GetSizeofMenuItemInfo()
	{
		// For Win95 and NT, cbSize needs to be 44
		if (1400 == (GetWinVersion()) || (2400 == GetWinVersion()))
			return CCSIZEOF_STRUCT(MENUITEMINFO, cch);

		return sizeof(MENUITEMINFO);
	}

	inline UINT GetSizeofNonClientMetrics()
	{
		// This function correctly determines the sizeof NONCLIENTMETRICS

  #if (WINVER >= 0x0600)
		// Is OS version less than Vista, adjust size to correct value
		if (GetWinVersion() < 2600)
			return CCSIZEOF_STRUCT(NONCLIENTMETRICS, lfMessageFont);
  #endif

		return sizeof(NONCLIENTMETRICS);
	}



	// A global function to report the state of the left mouse button
	inline BOOL IsLeftButtonDown()
	{
		SHORT state;
		if (GetSystemMetrics(SM_SWAPBUTTON))
			// Mouse buttons are swapped
			state = GetAsyncKeyState(VK_RBUTTON);
		else
			// Mouse buttons are not swapped
			state = GetAsyncKeyState(VK_LBUTTON);

		// returns true if the left mouse button is down
		return (state & 0x8000);
	}

	inline BOOL IsAeroThemed()
	{
		BOOL IsAeroThemed = FALSE;

		// Test if Windows version is XP or greater
		if (GetWinVersion() >= 2501)
		{
			HMODULE hMod = ::LoadLibrary(_T("uxtheme.dll"));

			if(hMod != 0)
			{
				// Declare pointers to IsCompositionActive function
				FARPROC pIsCompositionActive = ::GetProcAddress(hMod, "IsCompositionActive");

				if(pIsCompositionActive)
				{
					if(pIsCompositionActive())
					{
						IsAeroThemed = TRUE;
					}
				}
				::FreeLibrary(hMod);
			}
		}

		return IsAeroThemed;
	}

	inline BOOL IsXPThemed()
	{
		BOOL IsXPThemed = FALSE;

		// Test if Windows version is XP or greater
		if (GetWinVersion() >= 2501)
		{
			HMODULE hMod = ::LoadLibrary(_T("uxtheme.dll"));
			if(hMod != 0)
			{
				// Declare pointers to functions
				FARPROC pIsAppThemed   = ::GetProcAddress(hMod, "IsAppThemed");
				FARPROC pIsThemeActive = ::GetProcAddress(hMod, "IsThemeActive");

				if(pIsAppThemed && pIsThemeActive)
				{
					if(pIsAppThemed() && pIsThemeActive())
					{
						// Test if ComCtl32 dll used is version 6 or later
						IsXPThemed = (GetComCtlVersion() >= 600);
					}
				}
				::FreeLibrary(hMod);
			}
		}

		return IsXPThemed;
	}

  #endif // #ifndef _WIN32_WCE

	inline void LoadCommonControls()
	{
		// Load the Common Controls DLL
		HMODULE hComCtl = ::LoadLibrary(_T("COMCTL32.DLL"));
		if (hComCtl == 0)
			hComCtl = ::LoadLibrary(_T("COMMCTRL.DLL"));

		if (hComCtl)
		{
			// Declare a typedef for the InItCommonControlsEx function
			typedef BOOL WINAPI INIT_EX(INITCOMMONCONTROLSEX*);

  #ifdef _WIN32_WCE
			INIT_EX* pfnInitEx = (INIT_EX*)::GetProcAddress(hComCtl, _T("InitCommonControlsEx"));
  #else
			INIT_EX* pfnInitEx = (INIT_EX*)::GetProcAddress(hComCtl, "InitCommonControlsEx");
  #endif

			if (pfnInitEx)
			{
				// Load the full set of common controls
				INITCOMMONCONTROLSEX InitStruct;
				ZeroMemory(&InitStruct, sizeof(INITCOMMONCONTROLSEX));
				InitStruct.dwSize = sizeof(INITCOMMONCONTROLSEX);
				InitStruct.dwICC = ICC_WIN95_CLASSES|ICC_BAR_CLASSES|ICC_COOL_CLASSES|ICC_DATE_CLASSES;

  #ifndef _WIN32_WCE
				InitStruct.dwICC |= ICC_INTERNET_CLASSES|ICC_NATIVEFNTCTL_CLASS|ICC_PAGESCROLLER_CLASS|ICC_USEREX_CLASSES;
  #endif

 				// Call InitCommonControlsEx
				(*pfnInitEx)(&InitStruct);
			}
			else
			{
				// Declare a typedef for the InItCommonControls function
				typedef BOOL WINAPI INIT();

  #ifdef _WIN32_WCE
				INIT* pfnInit = (INIT*)::GetProcAddress(hComCtl, _T("InitCommonControls"));
  #else
				INIT* pfnInit = (INIT*)::GetProcAddress(hComCtl, "InitCommonControls");
  #endif

				(*pfnInit)();
			}

			::FreeLibrary(hComCtl);
		}
	}

  // Required for WinCE
  #ifndef lstrcpyn
	inline LPTSTR lstrcpyn(LPTSTR lpstrDest, LPCTSTR lpstrSrc, int nLength)
	{
		if(NULL == lpstrDest || NULL == lpstrSrc || nLength <= 0)
			return NULL;
		int nLen = MIN((int)lstrlen(lpstrSrc), nLength - 1);
		LPTSTR lpstrRet = (LPTSTR)memcpy(lpstrDest, lpstrSrc, nLen * sizeof(TCHAR));
		lpstrDest[nLen] = _T('\0');
		return lpstrRet;
	}
  #endif // !lstrcpyn

}


#endif	// _WIN32XX_WINUTILS_H_
