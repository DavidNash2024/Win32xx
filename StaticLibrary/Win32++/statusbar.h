// Win32++  Version 6.61
// Released: 30th August, 2009 by:
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


#ifndef _STATUSBAR_H_
#define _STATUSBAR_H_

#include "wincore.h"

namespace Win32xx
{

	//////////////////////////////////////
	// Declaration of the CStatusbar class
	//
	class CStatusbar : public CWnd
	{
	public:
		CStatusbar();
		virtual ~CStatusbar() {}

	// Overridables
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual BOOL IsStatusbar() const { return TRUE; }

	// Attributes
		int GetParts();
		HICON GetPartIcon(int iPart);
		CRect GetPartRect(int iPart);
		tString GetPartText(int iPart) const;
		BOOL IsSimple();
		BOOL SetPartIcon(int iPart, HICON hIcon);
		void SetPartText(int iPart, LPCTSTR szText, UINT Style = 0) const;
		void SetPartWidth(int iPart, int iWidth) const;

	// Operations
		CStatusbar(const CStatusbar&);				// Disable copy construction
		CStatusbar& operator = (const CStatusbar&); // Disable assignment operator

		void CreateParts(int iParts, const int iPaneWidths[]) const;
		void SetSimple(BOOL fSimple = TRUE);
	};

}


#endif // #ifndef _STATUSBAR_H_
