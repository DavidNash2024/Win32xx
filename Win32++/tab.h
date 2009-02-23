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
	public:
		CTab();
		virtual ~CTab();
		virtual void AddTabPage(CWnd* pWnd, LPCTSTR szTitle, HICON hIcon);
		virtual void AddTabPage(CWnd* pWnd, LPCTSTR szTitle, UINT nID_Icon);
		virtual void SelectPage(int iPage);
		virtual void RemoveTabPage(int iPage);

		// Attributes
		HIMAGELIST GetImageList() const { return m_himlTab; }
		CWnd* GetView() const		{ return m_pView; }

		// Wrappers for Win32 Macros
		void AdjustRect(BOOL fLarger, RECT *prc);
		BOOL DeleteAllItems();
		BOOL DeleteItem(int iItem);
		int  GetCurFocus();
		int  GetCurSel();
		BOOL GetItem(int iItem, LPTCITEM pitem);
		int  GetItemCount();
		int  InsertItem(int iItem, const LPTCITEM pitem);
		void SetCurFocus(int iItem);
		int  SetCurSel(int iItem);
		BOOL SetItem(int iItem, LPTCITEM pitem);
		DWORD SetItemSize(int cx, int cy);
		int  SetMinTabWidth(int cx);
		void SetPadding(int cx, int cy);

	protected:
		virtual SIZE GetMaxTabTextSize();
		virtual void OnCreate();
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void SetTabSize();
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			
	private:
		void DrawTabs(CDC& dcMem);
		void DrawTabBorders(CDC& dcMem, CRect& rcTab);
		void Paint();
		void SetView(CWnd& Wnd);

		std::vector<TabPageInfo> m_vTabPageInfo;
		HIMAGELIST m_himlTab;
		CWnd* m_pView;
		
	};

	//////////////////////////////////////////////////////////
	// Definitions for the CTab class
	//
	inline CTab::CTab() : m_pView(NULL)
	{
		m_himlTab = ImageList_Create(16, 16, ILC_MASK|ILC_COLOR32, 0, 0);
		TabCtrl_SetImageList(m_hWnd, m_himlTab);
	}
		
	inline CTab::~CTab()
	{
		ImageList_Destroy(m_himlTab);
		std::vector<TabPageInfo>::iterator iter;
		for (iter = m_vTabPageInfo.begin(); iter != m_vTabPageInfo.end(); ++iter)
		{
			delete (*iter).pWnd;
		}
	}

	inline void CTab::AddTabPage(CWnd* pWnd, LPCTSTR szTitle, HICON hIcon = 0)
	{
		TabPageInfo tbi = {0};
		tbi.pWnd = pWnd;
		lstrcpyn(tbi.szTitle, szTitle, MAX_MENU_STRING);
		if (hIcon)
			tbi.iImage = ImageList_AddIcon(GetImageList(), hIcon);
		else
			tbi.iImage = -1;
		
		int iNewPage = (int)m_vTabPageInfo.size();
		m_vTabPageInfo.push_back(tbi);		

		if (m_hWnd)
		{
			TCITEM tie = {0};
			tie.mask = TCIF_TEXT | TCIF_IMAGE;
			tie.iImage = tbi.iImage;
			tie.pszText = m_vTabPageInfo[iNewPage].szTitle;
			InsertItem(iNewPage, &tie);

			SetTabSize();
		}

		SetView(*pWnd);
	}

	inline void CTab::AddTabPage(CWnd* pWnd, LPCTSTR szTitle, UINT nID_Icon)
	{
		HICON hIcon = LoadIcon(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(nID_Icon));
		AddTabPage(pWnd, szTitle, hIcon);
	}

	inline void CTab::DrawTabs(CDC& dcMem)
	{
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
	}

	inline void CTab::DrawTabBorders(CDC& dcMem, CRect& rcTab)
	{
		BOOL IsBottomTab = GetWindowLongPtr(GWL_STYLE) & TCS_BOTTOM;

		// Draw a lighter rectangle touching the tab buttons
		CRect rcItem;
		TabCtrl_GetItemRect(m_hWnd, 0, &rcItem);
		int left = rcItem.left +1;
		int right = rcTab.right;
		int top = rcTab.bottom;
		int bottom = top + 3;
		
		if (!IsBottomTab)
		{
			top = rcTab.top -3;
			bottom = rcTab.top;
		}
		dcMem.CreateSolidBrush(RGB(248,248,248));
		dcMem.CreatePen(PS_SOLID, 1, RGB(248,248,248));
		Rectangle(dcMem, left, top, right, bottom);

		// Draw a darker line below the rectangle
		dcMem.CreatePen(PS_SOLID, 1, RGB(160, 160, 160));
		if (IsBottomTab)
		{
			MoveToEx(dcMem, left-1, bottom, NULL);
			LineTo(dcMem, right, bottom);
		}
		else
		{
			MoveToEx(dcMem, left-1, top-1, NULL);
			LineTo(dcMem, right, top-1);
		}
		dcMem.CreatePen(PS_SOLID, 1, RGB(248,248,248));

		// Draw a lighter line over the darker line for the selected tab
		TabCtrl_GetItemRect(m_hWnd, TabCtrl_GetCurSel(m_hWnd), &rcItem);
		OffsetRect(&rcItem, 1, 1);

		if (IsBottomTab)
		{
			MoveToEx(dcMem, rcItem.left, bottom, NULL);
			LineTo(dcMem, rcItem.right, bottom);
		}
		else
		{
			MoveToEx(dcMem, rcItem.left, top-1, NULL);
			LineTo(dcMem, rcItem.right, top-1);
		}
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

	inline void CTab::OnCreate()
	{			
		for (int i = 0; i < (int)m_vTabPageInfo.size(); ++i)
		{
			// Add tabs for each view.
			TCITEM tie = {0};
			tie.mask = TCIF_TEXT | TCIF_IMAGE;
			tie.iImage = i;
			tie.pszText = m_vTabPageInfo[i].szTitle;
			TabCtrl_InsertItem(m_hWnd, i, &tie);
		}

		SelectPage(0);
	}

	inline LRESULT CTab::OnNotifyReflect(WPARAM /*wParam*/, LPARAM lParam)
	{
		switch (((LPNMHDR)lParam)->code)
		{
		case TCN_SELCHANGE:
			{
				// Display the newly selected tab page
				int iPage = GetCurSel();
				SelectPage(iPage);
			}
			break;
		}

		return 0L;
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

		// Draw the tab buttons on the memory DC:
		DrawTabs(dcMem);
		DrawTabBorders(dcMem, rcTab);

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
		// For Tabs on the bottom, add the TCS_BOTTOM style
		cs.style = WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | TCS_OWNERDRAWFIXED | TCS_FIXEDWIDTH;
		cs.lpszClass = WC_TABCONTROL;
	}

	inline void CTab::RemoveTabPage(int iPage)
	{
		if ((iPage < 0) || (iPage > GetItemCount()-1) || (GetItemCount() <= 1))
			return;

		// Remove the tab
		DeleteItem(iPage);		
	
		// Remove the TapPageInfo entry
		std::vector<TabPageInfo>::iterator iter = m_vTabPageInfo.begin() + iPage;
		int iImage = (*iter).iImage;
		if (iImage >= 0) 
			TabCtrl_RemoveImage(m_hWnd, iImage);
		m_vTabPageInfo.erase(iter);

		SetTabSize();
		SelectPage(0);
	}

	inline void CTab::SelectPage(int iPage)
	{
		if ((iPage >= 0) && (iPage < GetItemCount()))
		{
			if (GetView()->IsWindow()) GetView()->ShowWindow(SW_HIDE);
			SetCurSel(iPage);
			SetView(*(m_vTabPageInfo[iPage].pWnd));

			// Position the View over the tab control's display area
			CRect rc = GetClientRect();
			TabCtrl_AdjustRect(m_hWnd, FALSE, &rc);						
			GetView()->SetWindowPos(HWND_TOP, rc, SWP_SHOWWINDOW);

			GetView()->SetFocus();
		}
	}

	inline void CTab::SetTabSize()
	{
		if (GetItemCount() > 0)
		{
			CRect rc = GetClientRect();
			TabCtrl_AdjustRect(m_hWnd, FALSE, &rc);
			int nItemWidth = min(25 + GetMaxTabTextSize().cx, (rc.Width()-2)/GetItemCount());
			SendMessage(TCM_SETITEMSIZE, 0, MAKELPARAM(nItemWidth, 20));
		}
	}

	inline void CTab::SetView(CWnd& Wnd)  
	// Sets or changes the View window displayed within the tab page
	{
		// Assign the view window
		m_pView = &Wnd;

		if (m_hWnd)
		{
			if (!m_pView->IsWindow())
			{
				// The tab control is already created, so create the new view too
				GetView()->Create(m_hWnd);
			}
		}
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
				// Set the tab sizes
				SetTabSize();

				// Position the View over the tab control's display area
				CRect rc = GetClientRect();
				TabCtrl_AdjustRect(m_hWnd, FALSE, &rc);						
				GetView()->SetWindowPos(HWND_TOP, rc, SWP_SHOWWINDOW);
			}
			return 0;
		}

		// pass unhandled messages on for default processing
		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

	// Wrappers for Win32 Macros


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

	inline int CTab::InsertItem(int iItem, const LPTCITEM pitem)
	{
		return TabCtrl_InsertItem(m_hWnd, iItem, pitem);
	}
	
	inline void CTab::SetCurFocus(int iItem)
	{
		TabCtrl_SetCurFocus(m_hWnd, iItem);
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
}

#endif
