// Win32++  Version 6.7
// Released: 6th November, 2009 by:
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


//////////////////////////////////////////////////////
// splitter.h

// The CSplitter class positions 3 windows over the parent window.
// The CWnd classes for these windows are m_Bar, m_pPane0 and m_pPane1.
// Use SetPanes to set the CWnd objects for m_pPane0 and m_Pane1.
//
// Refer to the Splitter sample for a demonstration of how to use CSplitter.


#ifndef _SPLITTER_H_
#define _SPLITTER_H_

#include "frame.h"

namespace Win32xx
{
	// Declaration of the CSplitter class
	class CSplitter : public CWnd
	{
	public:
		CSplitter();
		virtual ~CSplitter();
		virtual	int GetBarPos();
		virtual void SetBarPos(int nBarPos);
		virtual void SetBarColor(COLORREF color);
		virtual void SetImmediate(bool bImmediate);
		virtual void SetVertical(bool bVertical);
		virtual void SetWidth(int nWidth);
		virtual void RecalcLayout();

	protected:
		virtual void DrawBar(int Pos);
		virtual void OnCancelMode();
		virtual void OnCreate();
		virtual void OnLButtonDown();
		virtual void OnLButtonUp();
		virtual void OnMouseMove();
		virtual void OnSize();
		virtual void OnSysColorChange();
		virtual void SetPanes(CWnd& Pane0, CWnd& Pane1);
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

		CWnd m_Bar;			// Splitter bar
		CWnd* m_pPane0;		// Left or top CWnd object
		CWnd* m_pPane1;		// Right or botton CWnd Object

	private:
		CSplitter(const CSplitter&);				// Disable copy construction
		CSplitter& operator = (const CSplitter&); // Disable assignment operator

		HBRUSH m_hbrDithered; // Brush for the splitter bar during dragging
		HBRUSH m_hbrBackground; // Brush for the splitter bar background
		HBITMAP	m_hbm;		// Used to create m_hbrDithered
		int m_nWidth;		// Width of the splitter bar
		int m_nBarPos;		// X or Y position of the splitter bar
		int m_nOldBarPos;	// Previous bar position, updated during move
		int m_nBarpreMove;	// Previous bar position, not updated during move
		bool m_bCapture;	// Mouse capture flag
		bool m_bVertical;   // Vertical or horizontal splitter bar
		bool m_bImmediate;	// Reposition immediately, or draw hashed reposition bar
	};

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{

	inline CSplitter::CSplitter() : m_pPane0(NULL), m_pPane1(NULL), m_nBarPos(10), m_bCapture(false),
		                       m_bVertical(true), m_bImmediate(false)
	{
		m_nWidth = 5;
		WORD HashPattern[] = {0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA};
		m_hbm = ::CreateBitmap (8, 8, 1, 1, HashPattern);
		m_hbrDithered = ::CreatePatternBrush (m_hbm);
		m_hbrBackground = NULL;
		m_nOldBarPos = m_nBarPos;
		m_nBarpreMove = m_nBarPos;
	}

	inline CSplitter::~CSplitter()
	{
		::DeleteObject(m_hbrDithered);
		::DeleteObject(m_hbm);
		::DeleteObject(m_hbrBackground);
	}

	inline void CSplitter::DrawBar(int Pos)
	{
		// draws a hashed bar while the splitter bar is being dragged
		if (m_bCapture)
		{
			CDC BarDC = ::GetDC(m_hWnd);
			BarDC.AttachBrush(m_hbrDithered);

			CRect rc = GetClientRect();
			int cx = rc.right - rc.left;
			int cy = rc.bottom - rc.top;

			if (m_bVertical)
				::PatBlt (BarDC, Pos - m_nWidth/2, 0, m_nWidth, cy, PATINVERT);
			else
				::PatBlt (BarDC, 0, Pos - m_nWidth/2, cx, m_nWidth, PATINVERT);

			BarDC.DetachBrush();
		}
	}

	inline int CSplitter::GetBarPos()
	{
		return m_nBarPos;
	}

	inline void CSplitter::OnCancelMode()
	{
		// Handle unexpected loss of capture (application losing focus)
		if (m_bCapture)
		{
			::ReleaseCapture();
			m_bCapture = false;

			if (!m_bImmediate)
				DrawBar(m_nBarPos);

			m_nBarPos = m_nBarpreMove;
			m_nOldBarPos = m_nBarpreMove;
		}
	}

	inline void CSplitter::OnCreate()
	{
		// Set the color of the splitter bar
		CFrame* pFrame = (CFrame*)FromHandle(GetAncestor());
		CRebar& RB = pFrame->GetRebar();
		if (RB.GetRebarTheme().UseThemes)
			SetBarColor(RB.GetRebarTheme().clrBkgnd2);
		else
			SetBarColor(GetSysColor(COLOR_BTNFACE));

		// Create the splitter bar
		WNDCLASS wc = {0};

		// Create the splitter bar
		if (m_bVertical)
		{
			wc.lpszClassName = _T("Win32++ V Splitter");
			wc.hCursor = ::LoadCursor (NULL, IDC_SIZEWE);
			wc.hbrBackground = m_hbrBackground;
			m_Bar.RegisterClass(wc);
			m_Bar.CreateEx(0L, wc.lpszClassName, NULL, WS_VISIBLE |  WS_CHILD, 0, 0, 0, 0, m_hWnd, NULL);
		}
		else
		{
			wc.lpszClassName = _T("Win32++ H Splitter");
			wc.hCursor = ::LoadCursor (NULL, IDC_SIZENS);
			wc.hbrBackground = m_hbrBackground;
			m_Bar.RegisterClass(wc);
			m_Bar.CreateEx(0L, wc.lpszClassName, NULL, WS_VISIBLE |  WS_CHILD, 0, 0, 0, 0, m_hWnd, NULL);
		}

		// Create the two window panes
		if ((m_pPane0 == 0) || (m_pPane1 == 0))
			throw CWinException(_T("CSplitter::OnCreate ... m_pPane0 or m_pPane1 is NULL\n Use SetPanes to set m_pPane0 & m_pPane1"));

		m_pPane0->Create(m_hWnd);
		m_pPane1->Create(m_hWnd);
	}

	inline void CSplitter::OnLButtonDown()
	{
		::SetCapture(m_hWnd);
		m_bCapture = true;

		if (!m_bImmediate)
			DrawBar(m_nBarPos);

		m_nBarpreMove = m_nBarPos;
	}

	inline void CSplitter::OnLButtonUp()
	{
		if (!m_bImmediate)
			DrawBar(m_nBarPos);

		::ReleaseCapture();

		RecalcLayout();
		m_bCapture = false;
	}

	inline void CSplitter::OnMouseMove()
	{
		if (m_bCapture)
		{
			CPoint pt;
			static CPoint Oldpt;
			::GetCursorPos(&pt);
			::ScreenToClient (m_hWnd, &pt);

			if ((pt.x == Oldpt.x) && (pt.y == Oldpt.y))
				return;
			Oldpt.x = pt.x;
			Oldpt.y = pt.y;

			CRect rc;
			::GetClientRect(m_hWnd, &rc);
			int cx = rc.right - rc.left;
			int cy = rc.bottom - rc.top;

			if (m_bVertical)
			{
				m_nBarPos = pt.x;
				if (m_nBarPos < m_nWidth/2)
					m_nBarPos = m_nWidth/2;

				if (m_nBarPos > (cx - m_nWidth/2))
					m_nBarPos = cx - m_nWidth/2;
			}
			else
			{
				m_nBarPos = pt.y;
				if (m_nBarPos < (m_nWidth/2))
					m_nBarPos = m_nWidth/2;

				if (m_nBarPos > (cy - m_nWidth/2))
					m_nBarPos = cy - m_nWidth/2;
			}

			if (m_bImmediate)
				RecalcLayout();
			else
			{
				DrawBar(m_nOldBarPos);
				DrawBar(m_nBarPos);
			}

			m_nOldBarPos = m_nBarPos;
		}
	}

	inline void CSplitter::OnSize()
	{
		RecalcLayout();
	}

	inline void CSplitter::OnSysColorChange()
	{
		// Set the color of the splitter bar
		CFrame* pFrame = (CFrame*)FromHandle(GetAncestor());
		CRebar& RB = pFrame->GetRebar();
		if (RB.GetRebarTheme().UseThemes)
			SetBarColor(RB.GetRebarTheme().clrBkgnd2);
	}

	inline void CSplitter::RecalcLayout()
	{
		CRect r;
		::GetClientRect(m_hWnd, &r);

		int cx = r.right - r.left;
		int cy = r.bottom - r.top;
		int w2 = m_nWidth / 2;

		if (m_bVertical)
		{
			// Reposition our window panes and bar
			::SetWindowPos(m_pPane0->GetHwnd(), NULL, 0, 0, m_nBarPos - w2, cy, SWP_SHOWWINDOW);
			::SetWindowPos(m_pPane1->GetHwnd(), NULL, m_nBarPos - w2 + m_nWidth, 0, cx - m_nBarPos - m_nWidth + w2, cy, SWP_SHOWWINDOW );
			::SetWindowPos(m_Bar, NULL, m_nBarPos - w2, 0, m_nWidth, cy, SWP_SHOWWINDOW );
		}
		else
		{
			// Reposition our window panes and bar
			::SetWindowPos(m_pPane0->GetHwnd(), NULL, 0, 0, cx, m_nBarPos - w2, SWP_SHOWWINDOW );
			::SetWindowPos(m_pPane1->GetHwnd(), NULL, 0, m_nBarPos - w2 + m_nWidth, cx, cy - m_nBarPos - m_nWidth + w2, SWP_SHOWWINDOW );
			::SetWindowPos(m_Bar, NULL, 0, m_nBarPos - w2, cx, m_nWidth , SWP_SHOWWINDOW );
		}
		InvalidateRect(NULL, TRUE);
	}

	inline void CSplitter::SetBarPos(int nBarPos)
	{
		if (nBarPos < 0) return;

		CRect rClient;
		if (::GetClientRect(m_hWnd, &rClient))
		{
			int cx = rClient.right - rClient.left;
			int cy = rClient.bottom - rClient.top;
			if ((m_bVertical) && (nBarPos > cx)) return;
			if ((!m_bVertical) && (nBarPos >cy)) return;
		}

		m_nBarPos = nBarPos;
		m_nOldBarPos = nBarPos;

		if (m_hWnd)	RecalcLayout();
	}

	inline void CSplitter::SetBarColor(COLORREF color)
	{
		// Useful colors:
		// GetSysColor(COLOR_BTNFACE)	// Default Grey
		// RGB(196, 215, 250)			// Default Blue

		if (m_hbrBackground)
			::DeleteObject(m_hbrBackground);

		m_hbrBackground = ::CreateSolidBrush(color);

		if (0 != m_Bar.GetHwnd())
		{
			m_Bar.SetClassLongPtr(GCLP_HBRBACKGROUND, (LONG_PTR)m_hbrBackground);
		}
	}

	inline void CSplitter::SetPanes(CWnd& Pane0, CWnd& Pane1)
	{
		m_pPane0 = &Pane0;
		m_pPane1 = &Pane1;
	}

	inline void CSplitter::SetWidth(int nWidth)
	{
		// Sets the width of the splitter bar in pixels (default = 5)
		if (nWidth > 0)
			m_nWidth = nWidth;
		else
			DebugErrMsg(_T("Invalid Splitter bar width"));

		if (m_hWnd)
			RecalcLayout();
	}

	inline void CSplitter::SetImmediate(bool bImmediate)
	{
		// Set to true to resize the panes while the splitter bar is moving
		// Set to false to resize the panse after the splitter bar has moved
		m_bImmediate = bImmediate;
		if (m_hWnd)
			RecalcLayout();
	}

	inline void CSplitter::SetVertical(bool bVertical)
	{
		// Set to true for a vertical splitter bar, and false for a horizintal one
		m_bVertical = bVertical;
		if (m_hWnd)
			RecalcLayout();
	}

	inline LRESULT CSplitter::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		// Child windows are positioned over this window, so we don't see any
		//  WM_LBUTTONDOWN, WM_LBUTTONUP, WM_MOUSEMOVE messages unless the mouse
		//  input is captured. We detect left mouse button down with WM_PARENTNOTIFY.
		{
		case WM_ERASEBKGND:
			return 0L;
		case WM_PARENTNOTIFY:
			// Check for left mouse button down in child window
			if (wParam == WM_LBUTTONDOWN)
			{
				CPoint pt = GetCursorPos();
				if (WindowFromPoint(pt) == m_Bar)
					OnLButtonDown();
			}
			break;
		case WM_LBUTTONUP:
			OnLButtonUp();
			return 0L;
		case WM_MOUSEMOVE:
			OnMouseMove();
			return 0L;
		case WM_CANCELMODE:
			OnCancelMode();
			return 0L;
		case WM_SIZE:
			OnSize();
			break;
		case WM_SYSCOLORCHANGE:
			OnSysColorChange();
			::PostMessage(m_pPane0->GetHwnd(), WM_SYSCOLORCHANGE, 0, 0);
			::PostMessage(m_pPane1->GetHwnd(), WM_SYSCOLORCHANGE, 0, 0);
			break;
		}

		// Use the frame default message handling for remaining messages
		return CWnd::WndProcDefault(uMsg, wParam, lParam);
	}

} // namespace Win32xx

#endif // _SPLITTER_H_
