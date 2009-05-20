// Win32++  Version 6.5
// Released: 22nd May, 2009 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: http://users.bigpond.net.au/programming/
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


#ifndef _REBAR_H_
#define _REBAR_H_

#include "wincore.h"
#include "gdi.h"


namespace Win32xx
{

	struct ThemeRebar
	{
		BOOL UseThemes;			// TRUE if themes are used
		COLORREF clrBkgnd1;		// Colour 1 for rebar background
		COLORREF clrBkgnd2;		// Colour 2 for rebar background
		COLORREF clrBand1;		// Colour 1 for rebar band background. Use NULL if not required
		COLORREF clrBand2;		// Colour 2 for rebar band background. Use NULL if not required
		BOOL FlatStyle;			// Bands are rendered with flat rather than raised style
		BOOL KeepBandsLeft;		// TRUE if we always keep bands left
		BOOL LockMenuBand;		// Lock Menubar's band up top, without gripper
		BOOL RoundBorders;		// Use rounded band borders
		BOOL ShortBands;        // Allows bands to be shorter than maximum available width
		BOOL UseLines;			// Displays horizontal lines between bands
	};

	////////////////////////////////////
	// Declaration of the CRebar class
	//
	class CRebar : public CWnd
	{
	public:
		CRebar();
		virtual ~CRebar();
		virtual BOOL IsRebar() const {return TRUE;}

	// Attributes
		int  GetBand(const HWND hWnd) const;
		CRect GetBandBorders(int nBand) const;
		int  GetBandCount() const;
		void GetBandInfo(const int nBand, REBARBANDINFO& rbbi) const;
		CRect GetBandRect(int i) const;
		UINT GetBarHeight() const;
		void GetBarInfo(REBARINFO& rbi) const;
		HWND GetMenubar() {return m_hMenubar;}
		ThemeRebar& GetRebarTheme() {return m_Theme;}
		UINT GetRowCount() const;
		int  GetRowHeight(int nRow) const;
		HWND GetToolTips() const;
		void SetBandColor(const int nBand, const COLORREF clrFore, const COLORREF clrBack) const;
		void SetBandBitmap(const int nBand, const HBITMAP hBackground) const;
		void SetBandInfo(const int nBand, REBARBANDINFO& rbbi) const;
		void SetBarInfo(REBARINFO& rbi) const;
		void SetMenubar(HWND hMenubar) {m_hMenubar = hMenubar;}
		void SetRebarTheme(ThemeRebar& Theme);

	// Operations
		void DeleteBand(const int nBand) const;
		int  HitTest(RBHITTESTINFO& rbht);
		HWND HitTest(POINT pt);
		int  IDToIndex(UINT uBandID) const;
		void InsertBand(const int nBand, REBARBANDINFO& rbbi) const;
		BOOL IsBandVisible(int nBand) const;
		void MaximizeBand(UINT uBand, BOOL fIdeal = FALSE);
		void MinimizeBand(UINT uBand);
		BOOL MoveBand(UINT uFrom, UINT uTo);
		void MoveBandsLeft();
		void ResizeBand(const int nBand, const CSize& sz) const;
		void ShowGripper(int nBand, BOOL fShow) const;
		BOOL ShowBand(int nBand, BOOL fShow) const;

	protected:
	//Overridables
		virtual BOOL OnEraseBkgnd(HDC hDC);
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		ThemeRebar m_Theme;
		BOOL m_bIsDragging;
		HWND m_hMenubar;
		LPARAM m_Orig_lParam;
	};

}


#endif // #ifndef _REBAR_H_
