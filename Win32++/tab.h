// Win32++  Version 6.4.1
// Released: 12th February, 2009 by:
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


///////////////////////////////////////////////////////
// tab.h
//  Declaration of the CTab class

#ifndef TAB_H
#define TAB_H

#include "wincore.h"

namespace Win32xx
{
	class CTab : public CWnd
	{
		// Nested class. This is the Wnd for the window displayed over the client area
		// of the tab control.  The toolbar and view window are child windows of the 
		// viewpage window. Only the ViewPage of the parent CContainer is displayed. It's
		// contents are updated with the view window of the relevant container whenever
		// a different tab is selected.
		class CViewPage : public CWnd
		{
		public:
			CViewPage() : m_pView(NULL) {}
			virtual ~CViewPage() {}
			virtual CToolbar& GetToolbar() const {return (CToolbar&)m_Toolbar;}
			virtual CWnd* GetView() const	{return m_pView;}
			virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
			virtual void OnCreate();
			virtual void PreRegisterClass(WNDCLASS &wc);
			virtual void RecalcLayout();
			virtual void SetView(CWnd& wndView);
			virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		private:
			CToolbar m_Toolbar;
			CWnd* m_pView;
		};

	public:
		CTab();
		virtual ~CTab();
		virtual void AddToolbarButton(UINT nID);
		virtual CViewPage& GetViewPage() const	{ return (CViewPage&)m_ViewPage; }
		virtual CToolbar& GetToolbar() const	{ return GetViewPage().GetToolbar(); }	
		virtual void Paint();
		virtual void PreCreate(CREATESTRUCT& cs);

		// Attributes
		HIMAGELIST GetImageList() const { return m_himlTab; }
		HICON GetTabIcon() const		{ return m_hTabIcon; }
		LPCTSTR GetTabText() const		{ return m_tsTabText.c_str(); }
		std::vector<UINT>& GetToolbarData() const {return (std::vector <UINT> &)m_vToolbarData;}
		void SetTabIcon(HICON hTabIcon) { m_hTabIcon = hTabIcon; }
		void SetTabIcon(UINT nID_Icon);
		void SetTabText(LPCTSTR szText) { m_tsTabText = szText; }
			
	private:
		std::vector<UINT> m_vToolbarData;
		tString m_tsTabText;
		CViewPage m_ViewPage;
		HIMAGELIST m_himlTab;
		HICON m_hTabIcon;
		
	};


	inline CTab::CTab() : m_hTabIcon(0)
	{
		m_himlTab = ImageList_Create(16, 16, ILC_MASK|ILC_COLOR32, 0, 0);
		TabCtrl_SetImageList(m_hWnd, m_himlTab);
	}
		
	inline CTab::~CTab()
	{
		ImageList_Destroy(m_himlTab);
	}

	inline void CTab::AddToolbarButton(UINT nID)
	// Adds Resource IDs to toolbar buttons.
	// A resource ID of 0 is a separator
	{
		GetToolbarData().push_back(nID);
	}

	inline void CTab::Paint()
	{	
		// Microsoft's drawing for a tab control is rubbish, so we do our own.
		// We use double buffering and regions to eliminate flicker

		// Create the memory DC and bitmap
		CDC dcMem = ::CreateCompatibleDC(NULL);
		CRect rcClient = GetClientRect();
		CDC dcView = GetDC();
		dcMem.CreateCompatibleBitmap(dcView, rcClient.Width(), rcClient.Height());

		// Create a clipping region. Its the overall tab window's region,
		//  less the region belonging to the individual tab view's client area
		HRGN hrgnSrc1 = ::CreateRectRgn(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
		CRect rcTab = GetClientRect();
		TabCtrl_AdjustRect(m_hWnd, FALSE, &rcTab);
		if (rcTab.Height() < 0) rcTab.top = rcTab.bottom;
		if (rcTab.Width() < 0) rcTab.left = rcTab.right;
		
		HRGN hrgnSrc2 = ::CreateRectRgn(rcTab.left, rcTab.top, rcTab.right, rcTab.bottom);
		HRGN hrgnClip = ::CreateRectRgn(0, 0, 0, 0);
		::CombineRgn(hrgnClip, hrgnSrc1, hrgnSrc2, RGN_DIFF); 

		// Use the region in the memory DC to paint the grey background
		dcMem.AttachClipRegion(hrgnClip);
		dcMem.CreateSolidBrush(RGB(230, 230, 230));
		::PaintRgn(dcMem, hrgnClip);

		// Draw the tab buttons:
		for (int i = 0; i < TabCtrl_GetItemCount(m_hWnd); ++i)
		{
			CRect rcItem;
			TabCtrl_GetItemRect(m_hWnd, i, &rcItem);
			if (i == TabCtrl_GetCurSel(m_hWnd))
			{
				dcMem.CreateSolidBrush(RGB(248,248,248));
				SetBkColor(dcMem, RGB(248,248,248));
			}
			else
			{
				dcMem.CreateSolidBrush(RGB(200,200,200));
				SetBkColor(dcMem, RGB(200,200,200));
			}
			dcMem.CreatePen(PS_SOLID, 1, RGB(160, 160, 160));
			RoundRect(dcMem, rcItem.left+1, rcItem.top, rcItem.right+2, rcItem.bottom, 6, 6);

			if (rcItem.Width() >= 24)
			{
				TCHAR szText[30];
				TCITEM tcItem = {0};
				tcItem.mask = TCIF_TEXT | TCIF_IMAGE;
				tcItem.cchTextMax = 30;
				tcItem.pszText = szText;
				TabCtrl_GetItem(m_hWnd, i, &tcItem);

				// Draw the icon
				ImageList_Draw(m_himlTab, tcItem.iImage, dcMem, rcItem.left+5, rcItem.top+2, ILD_NORMAL);

				// Draw the text
				NONCLIENTMETRICS info = {0};
				info.cbSize = sizeof(info);
				SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
				dcMem.CreateFontIndirect(&info.lfStatusFont);
				CRect rcText = rcItem;
				rcText.left += 24;
				::DrawText(dcMem, szText, -1, &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
			}
		}  

		// Draw a lighter rectangle touching the tab buttons
		CRect rcItem;
		TabCtrl_GetItemRect(m_hWnd, 0, &rcItem);
		int left = rcItem.left +1;
		int right = rcTab.right -1;
		int top = rcTab.bottom;
		int bottom = top + 3;
		dcMem.CreateSolidBrush(RGB(248,248,248));
		dcMem.CreatePen(PS_SOLID, 1, RGB(248,248,248));
		Rectangle(dcMem, left, top, right, bottom);

		// Draw a darker line below the rectangle
		dcMem.CreatePen(PS_SOLID, 1, RGB(160, 160, 160));
		MoveToEx(dcMem, left-1, bottom, NULL);
		LineTo(dcMem, right, bottom);
		dcMem.CreatePen(PS_SOLID, 1, RGB(248,248,248));

		// Draw a lighter line below the rectangle for the selected tab
		TabCtrl_GetItemRect(m_hWnd, TabCtrl_GetCurSel(m_hWnd), &rcItem);
		OffsetRect(&rcItem, 1, 1);
		MoveToEx(dcMem, rcItem.right, rcItem.top, NULL);
		LineTo(dcMem, rcItem.left, rcItem.top);

		// Now copy our from our memory DC to the window DC
		dcMem.DetachClipRegion();
		dcView.AttachClipRegion(hrgnClip);
		BitBlt(dcView, 0, 0, rcClient.Width(), rcClient.Height(), dcMem, 0, 0, SRCCOPY);

		// Cleanup
		::DeleteObject(hrgnSrc1);
		::DeleteObject(hrgnSrc2);
		// hrgnClip is attached to dcView, so it will be deleted automatically
	}

	inline void CTab::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | TCS_OWNERDRAWFIXED | TCS_FIXEDWIDTH | TCS_BOTTOM ;
		cs.lpszClass = WC_TABCONTROL;
	}

	inline void CTab::SetTabIcon(UINT nID_Icon)
	{
		HICON hIcon = LoadIcon(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(nID_Icon));
		SetTabIcon(hIcon);
	}

}

#endif
