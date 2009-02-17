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
#include "Default_Resource.h"

namespace Win32xx
{

	struct TabPageInfo
	{
		TCHAR szTitle[MAX_MENU_STRING];
		int iImage;
		CWnd* pWnd;
	};

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
		virtual SIZE GetMaxTabTextSize();
		virtual CViewPage& GetViewPage() const	{ return (CViewPage&)m_ViewPage; }
		virtual CToolbar& GetToolbar() const	{ return GetViewPage().GetToolbar(); }	
		virtual void OnCreate();
		virtual void Paint();
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void SetTabSize();
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// Attributes
		HIMAGELIST GetImageList() const { return m_himlTab; }
		HICON GetTabIcon() const		{ return m_hTabIcon; }
		LPCTSTR GetTabText() const		{ return m_tsTabText.c_str(); }
		std::vector<UINT>& GetToolbarData() const {return (std::vector <UINT> &)m_vToolbarData;}
		CWnd* GetView() const			{ return GetViewPage().GetView(); }
		void SetTabIcon(HICON hTabIcon) { m_hTabIcon = hTabIcon; }
		void SetTabIcon(UINT nID_Icon);
		void SetTabText(LPCTSTR szText) { m_tsTabText = szText; }
		void SetView(CWnd& Wnd) 		{ GetViewPage().SetView(Wnd); }

		// Wrappers for Win32 Macros
		void AdjustRect(BOOL fLarger, RECT *prc);
		BOOL DeleteAllItems();
		BOOL DeleteItem(int iItem);
		int  GetCurFocus();
		int  GetCurSel();
		BOOL GetItem(int iItem, LPTCITEM pitem);
		int  GetItemCount();
		int  InsertItem(int iItem, const LPTCITEM pitem);
		int  SetCurFocus(int iItem);
		int  SetCurSel(int iItem);
		BOOL SetItem(int iItem, LPTCITEM pitem);
		DWORD SetItemSize(int cx, int cy);
		int  SetMinTabWidth(int cx);
		void SetPadding(int cx, int cy);
			
	private:
		std::vector<UINT> m_vToolbarData;
		tString m_tsTabText;
		CViewPage m_ViewPage;
		HIMAGELIST m_himlTab;
		HICON m_hTabIcon;
		
	};

	//////////////////////////////////////////////////////////
	// Definitions for the CTab class
	//
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

	inline void CTab::AdjustRect(BOOL fLarger, RECT *prc)
	{
		TabCtrl_AdjustRect(m_hWnd, fLarger, prc);
	}

	inline BOOL CTab::DeleteAllItems()
	{
		return TabCtrl_DeleteAllItems(m_hWnd);
	}

	inline BOOL CTab::DeleteItem(int iItem)
	{
		return TabCtrl_DeleteItem(m_hWnd, iItem);
	}

	inline int CTab::GetCurFocus()
	{
		return TabCtrl_GetCurFocus(m_hWnd);
	}

	inline int CTab::GetCurSel()
	{
		return TabCtrl_GetCurSel(m_hWnd);
	}

	inline BOOL CTab::GetItem(int iItem, LPTCITEM pitem)
	{
		return TabCtrl_GetItem(m_hWnd, iItem, pitem);
	}

	inline int CTab::GetItemCount()
	{
		return TabCtrl_GetItemCount(m_hWnd);
	}

	inline SIZE CTab::GetMaxTabTextSize()
	{
		CSize Size;

		for (int i = 0; i < TabCtrl_GetItemCount(m_hWnd); i++)
		{
			CSize TempSize;
			CDC dc = GetDC();
			NONCLIENTMETRICS info = {0};
			info.cbSize = sizeof(info);
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
			dc.CreateFontIndirect(&info.lfStatusFont);
			TCHAR szTitle[32];
			TCITEM tcItem = {0};
			tcItem.mask = TCIF_TEXT;
			tcItem.cchTextMax = 32;
			tcItem.pszText = szTitle;
			TabCtrl_GetItem(m_hWnd, i, &tcItem);
			GetTextExtentPoint32(dc, szTitle, lstrlen(szTitle), &TempSize);
			if (TempSize.cx > Size.cx)
				Size = TempSize;
		}

		return Size;
	}

	inline int CTab::InsertItem(int iItem, const LPTCITEM pitem)
	{
		return TabCtrl_InsertItem(m_hWnd, iItem, pitem);
	}

	inline void CTab::OnCreate()
	{		
		if (NULL == GetView())
			throw CWinException(_T("CTab::OnCreate... View window not assigned!\nUse SetView to set the View Window"));

		// Create the page window
		GetViewPage().Create(m_hWnd);

		// Create the toolbar
		if (GetToolbarData().size() > 0)
		{
			GetToolbar().Create(GetViewPage().GetHwnd());
			DWORD style = (DWORD)GetToolbar().GetWindowLongPtr(GWL_STYLE);
			style |= CCS_NODIVIDER ;//| CCS_NORESIZE;
			GetToolbar().SetWindowLongPtr(GWL_STYLE, style);
			int iButtons = GetToolbar().SetButtons(GetToolbarData());

			// Set the toolbar images
			// A mask of 192,192,192 is compatible with AddBitmap (for Win95)
			GetToolbar().SetImages(iButtons, RGB(192,192,192), IDW_MAIN, 0, 0);
			GetToolbar().SendMessage(TB_AUTOSIZE, 0, 0);
		}

		TCITEM tie = {0};
		tie.mask = TCIF_TEXT | TCIF_IMAGE;
		tie.iImage = 0;
		tie.pszText = (LPTSTR)m_tsTabText.c_str();
		TabCtrl_InsertItem(m_hWnd, 0, &tie);
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

	inline int CTab::SetCurFocus(int iItem)
	{
		return TabCtrl_SetCurFocus(m_hWnd, iItem);
	}
	
	inline int CTab::SetCurSel(int iItem)
	{
		return TabCtrl_SetCurSel(m_hWnd, iItem);
	}

	inline BOOL CTab::SetItem(int iItem, LPTCITEM pitem)
	{
		return TabCtrl_SetItem(m_hWnd, iItem, pitem);
	}

	inline DWORD CTab::SetItemSize(int cx, int cy)
	{
		return TabCtrl_SetItemSize(m_hWnd, cx, cy);
	}

	inline int CTab::SetMinTabWidth(int cx)
	{
		return TabCtrl_SetMinTabWidth(m_hWnd, cx);
	}

	inline void CTab::SetPadding(int cx, int cy)
	{
		TabCtrl_SetPadding(m_hWnd, cx, cy);
	}

	inline void CTab::SetTabIcon(UINT nID_Icon)
	{
		HICON hIcon = LoadIcon(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(nID_Icon));
		SetTabIcon(hIcon);
	}

	inline void CTab::SetTabSize()
	{
		CRect rc = GetClientRect();
		TabCtrl_AdjustRect(m_hWnd, FALSE, &rc);
		int nItemWidth = min(25 + GetMaxTabTextSize().cx, (rc.Width()-2)/TabCtrl_GetItemCount(m_hWnd));
		SendMessage(TCM_SETITEMSIZE, 0, MAKELPARAM(nItemWidth, 20));
	}

	inline LRESULT CTab::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch(uMsg)
		{
		case WM_PAINT:
			{
				// Remove all pending paint requests
				PAINTSTRUCT ps;
				::BeginPaint(hWnd, &ps);
				::EndPaint(hWnd, &ps);

				// Now call our local Paint
				Paint();
			}
			return 0;
		case WM_ERASEBKGND:
			return 0;
		case WM_SIZE:
			{
		//		if ((int)m_vTabPageInfo.size() > m_iCurrentPage)
		//		{				
					// Set the tab sizes
					SetTabSize();

					// Position the View over the tab control's display area
					CRect rc = GetClientRect();
					TabCtrl_AdjustRect(m_hWnd, FALSE, &rc);						
		//			m_vTabPageInfo[m_iCurrentPage].pContainer->GetViewPage().SetWindowPos(HWND_TOP, rc, SWP_SHOWWINDOW);
					GetViewPage().SetWindowPos(HWND_TOP, rc, SWP_SHOWWINDOW);
		//		} 			
			}
		//	break;
			return 0;
		}

		// pass unhandled messages on for default processing
		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

	///////////////////////////////////////////
	// Declaration of the nested CViewPage class
	inline BOOL CTab::CViewPage::OnCommand(WPARAM wParam, LPARAM lParam)
	{
		return (BOOL)::SendMessage(GetParent(), WM_COMMAND, wParam, lParam);
	}

	inline void CTab::CViewPage::OnCreate()
	{
		m_pView->Create(m_hWnd);
	}

	inline void CTab::CViewPage::PreRegisterClass(WNDCLASS &wc)
	{
		wc.lpszClassName = _T("Win32++ TabPage");
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	}

	inline void CTab::CViewPage::RecalcLayout()
	{
		GetToolbar().SendMessage(TB_AUTOSIZE, 0, 0);
		CRect rc = GetClientRect();
		CRect rcToolbar = m_Toolbar.GetClientRect();
		rc.top += rcToolbar.Height();
		GetView()->SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
	}

	inline void CTab::CViewPage::SetView(CWnd& wndView)
	// Sets or changes the View window displayed within the frame
	{
		// Assign the view window
		m_pView = &wndView;

		if (m_hWnd)
		{
			// The container is already created, so create and position the new view too
			GetView()->Create(m_hWnd);
			RecalcLayout();
		}
	}

	inline LRESULT CTab::CViewPage::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_SIZE:
			RecalcLayout();
			break;
		case WM_NOTIFY:
			switch (((LPNMHDR)lParam)->code)
			{
			// Send the focus change notifications to the grandparent
			case NM_KILLFOCUS:
			case NM_SETFOCUS:
			case UWM_FRAMELOSTFOCUS:
				::SendMessage(::GetParent(GetParent()), WM_NOTIFY, wParam, lParam);
				break;
			}

			break;
		}

		// pass unhandled messages on for default processing
		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

}

#endif
