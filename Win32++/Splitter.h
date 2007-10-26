// Latest verion available at:
// http://sourceforge.net/projects/win32-framework


// Win32++  Version 5.6
// Released: 15th November, 2007 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: http://users.bigpond.net.au/programming/
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


//////////////////////////////////////////////////////
// Splitter.h


#ifndef SPLITTER_H
#define SPLITTER_H

#include "WinCore.h"

namespace Win32xx
{

// The CSplitter class positions 3 windows over the parent window.
// The CWnd classes for these windows are m_Bar, m_pPane0 and m_pPane1.
// Use SetPanes to set the CWnd objects for m_pPane0 and m_Pane1.


	// Declaration of the CSplitter class
	class CSplitter : public CWnd
	{
	public:
		CSplitter(int nBarpos, BOOL bVertical);
		virtual ~CSplitter();
		virtual	int GetBarPos();
		virtual void SetBarPos(int nBarPos);
		virtual void SetBarColor(COLORREF color);
		virtual void SetWidth(int nWidth);

	protected:
		virtual void DrawBar(int Pos);
		virtual void OnCancelMode();
		virtual void OnCreate();
		virtual void OnLButtonDown();
		virtual void OnLButtonUp();
		virtual void OnMouseMove();
		virtual void OnSize();
		virtual void RecalcLayout();
		virtual void SetPanes(CWnd& Pane0, CWnd& Pane1);
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		CWnd m_Bar;			// Splitter bar
		CWnd* m_pPane0;		// Left or top CWnd object
		CWnd* m_pPane1;		// Right or botton CWnd Object

	private:

		HBRUSH m_hbrDithered; // Brush for the splitter bar
		HBITMAP	m_hbm;		// Used to create m_hbrDithered
		int m_nWidth;		// Width of the splitter bar
		int m_nBarPos;		// X or Y position of the splitter bar
		int m_nOldBarPos;	// Previous bar position, updated during move
		int m_nBarpreMove;	// Previous bar position, not updated during move
		BOOL m_bCapture;	// Mouse capture flag
		BOOL m_bVertical;   // Vertical or horizontal splitter bar
	};

} // namespace Win32xx

#endif // SPLITTER_H
