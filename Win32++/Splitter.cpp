// Win32++  Version 5.0.2 Beta
// Modified: 10th January, 2007 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//
//
// Copyright (c) 2005-2007  David Nash
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

//////////////////////////////////////////////
// Splitter.cpp
//  Definitions for the CSplitter class

#include "Splitter.h"
#include "Frame.h"
#include <windowsx.h>

namespace Win32xx
{

	CSplitter::CSplitter(int nBarpos, BOOL bVertical) : m_pPane0(NULL), m_pPane1(NULL),
					m_nBarPos(nBarpos), m_bCapture(false), m_bVertical(bVertical)
	{
		m_nWidth = 5;
		WORD HashPattern[] = {0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA};
		m_hbm = ::CreateBitmap (8, 8, 1, 1, HashPattern);
		m_hbrDithered = ::CreatePatternBrush (m_hbm);
		m_nOldBarPos = m_nBarPos;
		m_nBarpreMove = m_nBarPos;
	}

	void CSplitter::DrawBar(int Pos)
	{
		HDC hDC = ::GetDC(m_hWnd);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC, (HBRUSH)m_hbrDithered);

		RECT rc = {0};
		GetClientRect(m_hWnd, &rc);
		int cx = rc.right - rc.left;
		int cy = rc.bottom - rc.top;

		if (m_bVertical)
			::PatBlt (hDC, Pos - m_nWidth/2, 0, m_nWidth, cy, PATINVERT);
		else
			::PatBlt (hDC, 0, Pos - m_nWidth/2, cx, m_nWidth, PATINVERT);

		// Clean up
		::SelectObject(hDC, hOldBrush);
		::ReleaseDC(m_hWndParent, hDC);
	}

	int CSplitter::GetBarPos()
	{
		return m_nBarPos;
	}

	void CSplitter::OnCancelMode()
	{
		// Handle unexpected loss of capture (application losing focus)
		if (m_bCapture)
		{
			::ReleaseCapture();
			m_bCapture = false;

			DrawBar(m_nBarPos);
			m_nBarPos = m_nBarpreMove;
			m_nOldBarPos = m_nBarpreMove;
		}
	}

	void CSplitter::OnCreate()
	{
		try
		{
			// Create the splitter bar
			WNDCLASSEX wcx = {0};
			wcx.cbSize = sizeof(WNDCLASSEX);
			wcx.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);

			if (m_bVertical)
			{
				wcx.lpszClassName = TEXT("Win32++ V Splitter");
				wcx.hCursor = ::LoadCursor (NULL, IDC_SIZEWE);
				m_Bar.RegisterClassEx(wcx);
				m_Bar.CreateEx(0L, wcx.lpszClassName, NULL, WS_VISIBLE |  WS_CHILD, 0, 0, 0, 0, m_hWnd, NULL);
			}
			else
			{
				wcx.lpszClassName = TEXT("Win32++ H Splitter");
				wcx.hCursor = ::LoadCursor (NULL, IDC_SIZENS);
				m_Bar.RegisterClassEx(wcx);
				m_Bar.CreateEx(0L, wcx.lpszClassName, NULL, WS_VISIBLE |  WS_CHILD, 0, 0, 0, 0, m_hWnd, NULL);
			}

			// Create the two window panes
			if ((m_pPane0 == 0) || (m_pPane1 == 0))
				throw CWinException(TEXT("CSplitter::OnCreate ... m_pPane0 or m_pPane1 is NULL\n Use SetPanes to set m_pPane0 & m_pPane1"));

			m_pPane0->Create(m_hWnd);
			m_pPane1->Create(m_hWnd);
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CSplitter::OnCreate"));
		}
	}

	void CSplitter::OnLButtonDown()
	{
		::SetCapture(m_hWnd);
		m_bCapture = true;

		DrawBar(m_nBarPos);
		m_nBarpreMove = m_nBarPos;
	}

	void CSplitter::OnLButtonUp()
	{
		DrawBar(m_nBarPos);
		::ReleaseCapture();

		RecalcLayout();
		m_bCapture = false;
	}

	void CSplitter::OnMouseMove()
	{
		if (m_bCapture)
		{
			POINT pt = {0};
			static POINT Oldpt = {0};
			::GetCursorPos(&pt);
			::ScreenToClient (m_hWnd, &pt);

			if ((pt.x == Oldpt.x) && (pt.y == Oldpt.y))
				return;
			Oldpt.x = pt.x;
			Oldpt.y = pt.y;

			RECT rc = {0};
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

			DrawBar(m_nOldBarPos);
			DrawBar(m_nBarPos);

			m_nOldBarPos = m_nBarPos;
		}
	}

	void CSplitter::OnSize()
	{
		RecalcLayout();
	}

	void CSplitter::RecalcLayout()
	{
		RECT r = {0};
		::GetClientRect(m_hWnd, &r);

		int cx = r.right - r.left;
		int cy = r.bottom - r.top;
		int w2 = m_nWidth / 2;

		if (m_bVertical)
		{
			// Reposition our window panes and bar
			::SetWindowPos(m_pPane0->GetHwnd(), NULL, 0, 0, m_nBarPos - w2, cy, SWP_SHOWWINDOW );
			::SetWindowPos(m_pPane1->GetHwnd(), NULL, m_nBarPos - w2 + m_nWidth, 0, cx - m_nBarPos - m_nWidth + w2, cy, SWP_SHOWWINDOW );
			::SetWindowPos(m_Bar.GetHwnd(), NULL, m_nBarPos - w2, 0, m_nWidth, cy, SWP_SHOWWINDOW );
		}
		else
		{
			// Reposition our window panes and bar
			::SetWindowPos(m_pPane0->GetHwnd(), NULL, 0, 0, cx, m_nBarPos - w2, SWP_SHOWWINDOW );
			::SetWindowPos(m_pPane1->GetHwnd(), NULL, 0, m_nBarPos - w2 + m_nWidth, cx, cy - m_nBarPos - m_nWidth + w2, SWP_SHOWWINDOW );
			::SetWindowPos(m_Bar.GetHwnd(), NULL, 0, m_nBarPos - w2, cx, m_nWidth , SWP_SHOWWINDOW );
		}
		InvalidateRect(m_Bar.GetHwnd(), NULL, TRUE);
	}

	void CSplitter::SetBarPos(int nBarPos)
	{
		if (nBarPos < 0) return;

		RECT rClient = {0};
		::GetClientRect(m_hWnd, &rClient);
		int cx = rClient.right - rClient.left;
		int cy = rClient.bottom - rClient.top;
		if ((m_bVertical) && (nBarPos > cx)) return;
		if ((!m_bVertical) && (nBarPos >cy)) return;

		m_nBarPos = nBarPos;
		m_nOldBarPos = nBarPos;
		RecalcLayout();
	}

	void CSplitter::SetPanes(CWnd& Pane0, CWnd& Pane1)
	{
		m_pPane0 = &Pane0;
		m_pPane1 = &Pane1;
	}

	void CSplitter::SetWidth(int nWidth)
	{
		if (nWidth > 0)
			m_nWidth = nWidth;
		else
			::MessageBox(NULL, TEXT("Invalid Splitter bar width"), TEXT("Error"), MB_OK);
	}

	LRESULT CSplitter::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
				POINT pt;
				GetCursorPos(&pt);
				if (WindowFromPoint(pt) == m_Bar.GetHwnd())
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
		}

		// Use the frame default message handling for remaining messages
		return CWnd::WndProc(hwnd, uMsg, wParam, lParam);
	}
}  // namespace Win32xx

