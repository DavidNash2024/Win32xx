// Win32++  Version 6.1
// Released: 7th June, 2008 by:
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

#ifndef REBAR_H
#define REBAR_H


namespace Win32xx
{

	struct ThemeRebar
	{
		BOOL UseThemes;			// TRUE if themes are used
		COLORREF clrBkgnd1;		// Color 1 for rebar background
		COLORREF clrBkgnd2;		// Color 2 for rebar background
		COLORREF clrBand1;		// Color 1 for rebar band background. Use NULL if not required
		COLORREF clrBand2;		// Color 2 for rebar band background. Use NULL if not required
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

		// These are the functions you might wish to override
		virtual void MoveBandsLeft();
		virtual void ShowGripper(int nBand, BOOL fShow) const;
		virtual BOOL ShowBand(int nBand, BOOL fShow) const;
		virtual void ResizeBand(const int nBand, SIZE sz) const;

		// These functions aren't intended to be overridden
		void DeleteBand(const int nBand) const ;
		int  GetBand(const HWND hWnd) const;
		RECT GetBandBorders(int nBand) const;
		int  GetBandCount() const;
		void GetBandInfo(const int nBand, LPREBARBANDINFO prbbi) const;
		RECT GetBandRect(int i) const;
		void GetBarInfo(LPREBARINFO prbi) const;
		int  GetRowHeight(int nRow) const;
		void InsertBand(const int nBand, LPREBARBANDINFO prbbi) const;
		BOOL IsBandVisible(int nBand) const;
		void SetBandColor(const int nBand, const COLORREF clrFore, const COLORREF clrBack) const;
		void SetBandBitmap(const int nBand, const HBITMAP hBackground) const;
		void SetBandInfo(const int nBand, LPREBARBANDINFO prbbi) const;
		void SetBarInfo(LPREBARINFO prbi) const;
		ThemeRebar& GetRebarTheme() {return m_Theme;}
		void SetRebarTheme(ThemeRebar& Theme);
		HWND GetMenubar() {return m_hMenubar;}
		void SetMenubar(HWND hMenubar) {m_hMenubar = hMenubar;}

	protected:
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		virtual BOOL OnEraseBkgnd(HDC hDC);

		ThemeRebar m_Theme;
		BOOL m_bIsDragging;
		HWND m_hMenubar;
	};

	///////////////////////////////////
	// Definitions for the CRebar class
	//
	inline CRebar::CRebar() : m_bIsDragging(FALSE), m_hMenubar(0)
	{
		ZeroMemory(&m_Theme, sizeof(ThemeRebar));
	}

	inline CRebar::~CRebar()
	{
	}

	inline void CRebar::DeleteBand(int nBand) const
	{
		if (!::SendMessage(m_hWnd, RB_DELETEBAND, nBand, 0))
			throw CWinException(_T("Failed to delete rebar band"));
	}

	inline int CRebar::GetBand(HWND hWnd) const
	// Returns the zero based band number for this window handle
	{
		int nResult = -1;
		if (NULL == hWnd) return nResult;

		for (int nBand = 0; nBand < GetBandCount(); ++nBand)
		{
			REBARBANDINFO rbbi = {0};
			rbbi.fMask = RBBIM_CHILD;
			GetBandInfo(nBand, &rbbi);
			if (rbbi.hwndChild == hWnd)
				nResult = nBand;
		}
		return nResult;
	}

	inline RECT CRebar::GetBandBorders(int nBand) const
	{
		RECT rc = {0};
		::SendMessage(m_hWnd, RB_GETBANDBORDERS, nBand, (LPARAM)&rc);
		return rc;
	}

	inline int CRebar::GetBandCount() const
	{
		return (int)::SendMessage(m_hWnd, RB_GETBANDCOUNT, 0, 0);
	}

	inline void CRebar::GetBandInfo(int nBand, LPREBARBANDINFO prbbi) const
	{
		// REBARBANDINFO describes individual BAND characteristics
		if (nBand < 0)
			throw CWinException(_T("Invalid Rebar band number"));

		prbbi->cbSize = sizeof(REBARBANDINFO);
		if(!::SendMessage(m_hWnd, RB_GETBANDINFO, nBand, (LPARAM)prbbi))
			throw CWinException(_T("Failed to get rebar band info"));
	}

	inline RECT CRebar::GetBandRect(int i) const
	{
		RECT rc = {0};
		::SendMessage(m_hWnd, RB_GETRECT, i, (LPARAM)&rc);
		return rc;
	}

	inline void CRebar::GetBarInfo(LPREBARINFO prbi) const
	{
		// REBARINFO describes overall rebar control characteristics
		prbi->cbSize = sizeof(REBARINFO);
		if (!::SendMessage(m_hWnd, RB_GETBARINFO, 0, (LPARAM)prbi))
			throw CWinException(_T("Failed to get rebar info"));
	}

	inline int CRebar::GetRowHeight(int nRow) const
	{
		return (int)::SendMessage(m_hWnd, RB_GETROWHEIGHT, nRow, 0);
	}

	inline void CRebar::InsertBand(int nBand, LPREBARBANDINFO prbbi) const
	{
		if ( !::SendMessage(m_hWnd, RB_INSERTBAND, nBand, (LPARAM)(LPREBARBANDINFO)prbbi))
			throw CWinException(_T("Failed to insert rebar band"));
	}

	inline BOOL CRebar::IsBandVisible(int nBand) const
	{
		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof(REBARBANDINFO);
		rbbi.fMask = RBBIM_STYLE;
		GetBandInfo(nBand, &rbbi);

		return !(rbbi.fStyle & RBBS_HIDDEN);
	}

	inline BOOL CRebar::OnEraseBkgnd(HDC hDC)
	{
		if (!m_Theme.UseThemes)
			return FALSE;

		RECT rc;
		GetClientRect(m_hWnd, &rc);
		int BarWidth = rc.right - rc.left;
		int BarHeight = rc.bottom - rc.top;

		// Create and set up our memory DC
		CDC MemDC = ::CreateCompatibleDC(hDC);
		MemDC.CreateCompatibleBitmap(hDC, BarWidth, BarHeight);

		// Draw to Rebar background to the memory DC
		rc.right = 600;
		GradientFill(MemDC, m_Theme.clrBkgnd1, m_Theme.clrBkgnd2, &rc, TRUE);
		if (BarWidth >= 600)
		{
			rc.left = 600;
			rc.right = BarWidth;
			SolidFill(MemDC, m_Theme.clrBkgnd2, &rc);
		}

		if (m_Theme.clrBand1 || m_Theme.clrBand2)
		{
			// Draw the individual band backgrounds
			for (int nBand = 0 ; nBand < GetBandCount(); ++nBand)
			{
				if (IsBandVisible(nBand))
				{
					if (nBand != GetBand(m_hMenubar))
					{
						// Determine the size of this band
						RECT rcBand = GetBandRect(nBand);

						// Determine the size of the child window
						REBARBANDINFO rbbi = {0};
						rbbi.cbSize = sizeof(REBARBANDINFO);
						rbbi.fMask = RBBIM_CHILD ;
						GetBandInfo(nBand, &rbbi);
						RECT rcChild;
						::GetWindowRect(rbbi.hwndChild, &rcChild);
						int ChildWidth = rcChild.right - rcChild.left;

						// Determine our drawing rectangle
						RECT rcDraw = {0};
						CopyRect(&rcDraw, &rcBand);
						rcDraw.bottom = rcDraw.top + (rcBand.bottom - rcBand.top)/2;
						int xPad = IsXPThemed()? 2: 0;
						rcDraw.left -= xPad;

						// Fill the Source CDC with the band's background
						CDC SourceDC = ::CreateCompatibleDC(hDC);
						SourceDC.CreateCompatibleBitmap(hDC, BarWidth, BarHeight);
						RECT rcBorder = GetBandBorders(nBand);
						rcDraw.right = rcBand.left + ChildWidth + rcBorder.left;
						SolidFill(SourceDC, m_Theme.clrBand1, &rcDraw);
						rcDraw.top = rcDraw.bottom;
						rcDraw.bottom = rcBand.bottom;
						GradientFill(SourceDC, m_Theme.clrBand1, m_Theme.clrBand2, &rcDraw, FALSE);

						// Set Curve amount for rounded edges
						int Curve = m_Theme.RoundBorders? 12 : 0;

						// Create our mask for rounded edges using RoundRect
						CDC MaskDC = ::CreateCompatibleDC(hDC);
						MaskDC.CreateCompatibleBitmap(hDC, BarWidth, BarHeight);

						rcDraw.top = rcBand.top;
						if (!m_Theme.FlatStyle)
							::InflateRect(&rcDraw, 1, 1);

						int left = rcDraw.left;
						int right = rcDraw.right;
						int top = rcDraw.top;
						int bottom = rcDraw.bottom;
						int cx = rcDraw.right - rcBand.left + xPad;
						int cy = rcDraw.bottom - rcBand.top;

						if (m_Theme.FlatStyle)
						{
							SolidFill(MaskDC, RGB(0,0,0), &rcDraw);
							::BitBlt(MaskDC, left, top, cx, cy, MaskDC, left, top, PATINVERT);
							::RoundRect(MaskDC, left, top, right, bottom, Curve, Curve);
						}
						else
						{
							SolidFill(MaskDC, RGB(0,0,0), &rcDraw);
							::RoundRect(MaskDC, left, top, right, bottom, Curve, Curve);
							::BitBlt(MaskDC, left, top, cx, cy, MaskDC, left, top, PATINVERT);
						}

						// Copy Source DC to Memory DC using the RoundRect mask
						::BitBlt(MemDC, left, top, cx, cy, SourceDC, left, top, SRCINVERT);
						::BitBlt(MemDC, left, top, cx, cy, MaskDC,   left, top, SRCAND);
						::BitBlt(MemDC, left, top, cx, cy, SourceDC, left, top, SRCINVERT);

						// Extra drawing to prevent jagged edge while moving bands
						if (m_bIsDragging)
						{
							CDC RebarDC = ::GetDC(m_hWnd);
							::BitBlt(RebarDC, rcDraw.right - ChildWidth, rcDraw.top, ChildWidth, cy, MemDC, rcDraw.right - ChildWidth, rcDraw.top, SRCCOPY);
						}
					}
				}
			}
		}

		if (m_Theme.UseLines)
		{
			// Draw lines between bands
			for (int j = 0; j < GetBandCount()-1; ++j)
			{
				rc = GetBandRect(j);
				rc.left = max(0, rc.left - 4);
				rc.bottom +=2;
				::DrawEdge(MemDC, &rc, EDGE_ETCHED, BF_BOTTOM | BF_ADJUST);
			}
		}

		// Copy the Memory DC to the window's DC
		::BitBlt(hDC, 0, 0, BarWidth, BarHeight, MemDC, 0, 0, SRCCOPY);

		return TRUE;
	}

	inline void CRebar::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
                         CCS_NODIVIDER | RBS_VARHEIGHT | RBS_BANDBORDERS ;

		cs.cy = 100;
		cs.lpszClass = REBARCLASSNAME;
	}

	inline void CRebar::MoveBandsLeft()
	{
		int OldrcTop = -1;
		for (int nBand = GetBandCount() -1; nBand >= 0; --nBand)
		{
			RECT rc = GetBandRect(nBand);
			if (rc.top != OldrcTop)
			{
				// Maximize the last band on each row
				if (IsBandVisible(nBand))
					::SendMessage(GetHwnd(), RB_MAXIMIZEBAND, nBand, 0);

				OldrcTop = rc.top;
			}
		}
	}

	inline void CRebar::ResizeBand(int nBand, SIZE sz) const
	{
		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof(rbbi);
		rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_SIZE;

		GetBandInfo(nBand, &rbbi);
		rbbi.cx         = sz.cx + 2;
		rbbi.cxMinChild = sz.cx + 2;
		rbbi.cyMinChild = sz.cy;
		rbbi.cyMaxChild = sz.cy;
		SetBandInfo(nBand, &rbbi );
	}

	inline void CRebar::SetBandBitmap(int nBand, HBITMAP hBackground) const
	{
		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof(REBARBANDINFO);
		rbbi.fMask  = RBBIM_STYLE;
		GetBandInfo(nBand, &rbbi);
		rbbi.fMask  |= RBBIM_BACKGROUND;
		rbbi.hbmBack = hBackground;
		::SendMessage(m_hWnd, RB_SETBANDINFO, nBand, (LPARAM)&rbbi);
	}

	inline void CRebar::SetBandColor(int nBand, COLORREF clrFore, COLORREF clrBack) const
	{
		// Won't work with XP themes enabled
		// Won't work if a bitmap has been set
		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof(REBARBANDINFO);
		rbbi.fMask = RBBIM_COLORS;
		rbbi.clrFore = clrFore;
		rbbi.clrBack = clrBack;
		::SendMessage(m_hWnd, RB_SETBANDINFO, nBand, (LPARAM)&rbbi);
	}

	inline void CRebar::SetBandInfo(int nBand, LPREBARBANDINFO prbbi) const
	{
		if (nBand < 0)
			throw CWinException(_T("Invalid Rebar band number"));

		// REBARBANDINFO describes individual BAND characteristics
		prbbi->cbSize = sizeof(REBARBANDINFO);
		if(!::SendMessage(m_hWnd, RB_SETBANDINFO, nBand, (LPARAM)prbbi))
			throw CWinException(_T("Failed to set rebar band info"));
	}

	inline void CRebar::SetBarInfo(LPREBARINFO prbi) const
	{
		// REBARINFO associates an image list with the rebar
		// A band will also need to set RBBIM_IMAGE
		prbi->cbSize = sizeof(REBARINFO);
		if (!::SendMessage(m_hWnd, RB_SETBARINFO, 0, (LPARAM)prbi))
			throw CWinException(_T("Failed to set rebar info"));
	}

	inline void CRebar::SetRebarTheme(ThemeRebar& Theme)
	{
		m_Theme.UseThemes    = Theme.UseThemes;
		m_Theme.clrBkgnd1    = Theme.clrBkgnd1;
		m_Theme.clrBkgnd2    = Theme.clrBkgnd2;
		m_Theme.clrBand1     = Theme.clrBand1;
		m_Theme.clrBand2     = Theme.clrBand2;
		m_Theme.KeepBandsLeft= Theme.KeepBandsLeft;
		m_Theme.LockMenuBand = Theme.LockMenuBand;
		m_Theme.ShortBands   = Theme.ShortBands;
		m_Theme.UseLines     = Theme.UseLines;
		m_Theme.FlatStyle    = Theme.FlatStyle;
		m_Theme.RoundBorders = Theme.RoundBorders;

		if (m_Theme.LockMenuBand)
			ShowGripper(GetBand(m_hMenubar), FALSE);
		else
			ShowGripper(GetBand(m_hMenubar), TRUE);

		::InvalidateRect(m_hWnd, NULL, TRUE);
	}

	inline BOOL CRebar::ShowBand(int nBand, BOOL fShow) const
	// Show or hide a band
	{
		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof(REBARBANDINFO);
		rbbi.fMask = RBBIM_STYLE;
		GetBandInfo(nBand, &rbbi);
		if (fShow)
		{
			rbbi.fStyle &=  ~RBBS_HIDDEN;
			SetBandInfo(nBand, &rbbi);
		}
		else
		{
			rbbi.fStyle |= RBBS_HIDDEN;
			SetBandInfo(nBand, &rbbi);
		}
		return fShow;
	}

	inline void CRebar::ShowGripper(int nBand, BOOL fShow) const
	{
		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof(REBARBANDINFO);
		rbbi.fMask = RBBIM_STYLE;
		GetBandInfo(nBand, &rbbi);
		if (fShow)
		{
			rbbi.fStyle |= RBBS_GRIPPERALWAYS;
			rbbi.fStyle &= ~RBBS_FIXEDSIZE;
		}
		else
		{
			rbbi.fStyle &= ~RBBS_GRIPPERALWAYS;
			rbbi.fStyle |= RBBS_FIXEDSIZE;
		}
		SetBandInfo(nBand, &rbbi);
	}


	inline LRESULT CRebar::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		static LPARAM Orig_lParam;
		switch (uMsg)
		{
		case WM_MOUSEMOVE:
			if (m_Theme.UseThemes && m_Theme.LockMenuBand)
			{
				// We want to lock the first row in place, but allow other bands to move!
				// Use move messages to limit the resizing of bands
				int y = GET_Y_LPARAM(lParam);

				if (y <= GetRowHeight(0))
					return 0L;	// throw this message away
			}
			break;
		case WM_LBUTTONDOWN:
			Orig_lParam = lParam;	// Store the x,y position
			m_bIsDragging = TRUE;
			break;
		case WM_LBUTTONUP:
			if (m_Theme.UseThemes && m_Theme.LockMenuBand)
			{
				// Use move messages to limit the resizing of bands
				int y = GET_Y_LPARAM(lParam);

				if (y <= GetRowHeight(0))
				{
					// Use x,y from WM_LBUTTONDOWN for WM_LBUTTONUP position
					lParam = Orig_lParam;
				}
			}
			m_bIsDragging = FALSE;
			break;
		case WM_ERASEBKGND:
			if (OnEraseBkgnd((HDC)wParam))
				return TRUE;
			break;
		}

		// pass unhandled messages on for default processing
		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

} // namespace Win32xx

#endif // #ifndef REBAR_H
