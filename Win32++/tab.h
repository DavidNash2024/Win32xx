// Win32++  Version 6.5 beta
// Released: ??th May, 2009 by:
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
//  Declaration of the CTab and CMDITab classes

#ifndef _TAB_H_
#define _TAB_H_

#include "wincore.h"
#include "dialog.h"
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
	protected:
		// Declaration of the CSelectDialog class, a nested class of CTab
		// It creates the dialog to choose which tab to activate
		class CSelectDialog : public CDialog
		{
		public:
			CSelectDialog(LPCDLGTEMPLATE lpTemplate, HWND hWndParent = NULL);
			virtual ~CSelectDialog() {}
			virtual void AddItem(LPCTSTR szString);

		protected:
			virtual BOOL OnInitDialog();
			virtual void OnOK(); 
			virtual void OnCancel() { EndDialog(-2); }

		private:
			std::vector<tString> m_vItems;
			int IDC_LIST;

		};
	public:
		CTab();
		virtual ~CTab();
		virtual void AddTabPage(CWnd* pWnd, LPCTSTR szTitle, HICON hIcon);
		virtual void AddTabPage(CWnd* pWnd, LPCTSTR szTitle, UINT nID_Icon);
		virtual int  GetTabIndex(CWnd* pWnd);
		virtual TabPageInfo GetTabPageInfo(UINT nTab);
		virtual void SelectPage(int iPage);
		virtual void RecalcLayout();
		virtual void RemoveTabPage(int iPage);
		virtual void ShowListMenu();
		virtual void ShowListDialog();

		// Attributes
		HIMAGELIST GetImageList() const { return m_himlTab; }
		BOOL GetShowButtons() const { return m_bShowButtons; }
		int GetTabHeight() const { return m_nTabHeight; }
		CWnd* GetView() const		{ return m_pView; }
		void SetShowButtons(BOOL bShow)	{ m_bShowButtons = bShow; }
		void SetTabHeight(int nTabHeight) { m_nTabHeight = nTabHeight; NotifyChanged();}

		// Wrappers for Win32 Macros
		void AdjustRect(BOOL fLarger, RECT *prc);
		BOOL DeleteAllItems();
		BOOL DeleteItem(int iItem);
		std::vector <TabPageInfo>& GetAllTabs() const { return (std::vector <TabPageInfo>&) m_vTabPageInfo; }
		CRect GetCloseRect() { return m_rcClose; }
		int  GetCurFocus();
		int  GetCurSel();
		BOOL GetItem(int iItem, LPTCITEM pitem);
		int  GetItemCount();
		CRect GetListRect() { return m_rcList; }
		int  InsertItem(int iItem, const LPTCITEM pitem);
		void SetCurFocus(int iItem);
		int  SetCurSel(int iItem);
		BOOL SetItem(int iItem, LPTCITEM pitem);
		DWORD SetItemSize(int cx, int cy);
		int  SetMinTabWidth(int cx);
		void SetPadding(int cx, int cy);

	protected:
		virtual SIZE GetMaxTabSize();
		virtual void OnCreate();
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void SetTabSize();
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			
	private:
		void DrawCloseButton(CDC& DrawDC, UINT uState);
		void DrawListButton(CDC& DrawDC, UINT uState);
		void DrawTabs(CDC& dcMem);
		void DrawTabBorders(CDC& dcMem, CRect& rcTab);
		void Paint();
		void NotifyChanged();
		void SetView(CWnd& Wnd);

		std::vector<TabPageInfo> m_vTabPageInfo;
		CRect m_rcList;
		CRect m_rcClose;
		HIMAGELIST m_himlTab;
		CWnd* m_pView;
		BOOL m_bShowButtons;	// Show or hide the close and list button
		int m_nTabHeight;
		
	};
	
	////////////////////////////////////////
	// Declaration of the CTabbedMDI class
	class CTabbedMDI : public CWnd
	{
	public:
		CTabbedMDI(); 
		virtual ~CTabbedMDI() {}
		virtual CWnd* AddMDIChild(CWnd* pWnd, LPCTSTR szTabText);
		virtual void CloseActiveMDI();
		virtual CWnd* GetActiveMDIChild();
		virtual void RecalcLayout();
		virtual void SetActiveMDIChild(CWnd* pWnd);
		virtual void SetActiveTab(int iTab);
		virtual void ShowListMenu();
	  
	protected:
		virtual HWND Create(HWND hWndParent);
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	  
	private:
		CTab m_Tab;
	};

	/////////////////////////////////////////////////////////////
	// Definitions for the CSelectDialog class nested within CTab
	//
	inline CTab::CSelectDialog::CSelectDialog(LPCDLGTEMPLATE lpTemplate, HWND hWndParent) : 
					CDialog(lpTemplate, hWndParent), IDC_LIST(121)
	{
	}

	inline BOOL CTab::CSelectDialog::OnInitDialog()
	{
		for (UINT u = 0; u < m_vItems.size(); ++u)
		{
			SendDlgItemMessage(IDC_LIST, LB_ADDSTRING, 0, (LPARAM) m_vItems[u].c_str());
		}

		return true;
	}

	inline void CTab::CSelectDialog::AddItem(LPCTSTR szString)
	{
		m_vItems.push_back(szString);
	}

	inline void CTab::CSelectDialog::OnOK()
	{
		int iSelect = (int)SendDlgItemMessage(IDC_LIST, LB_GETCURSEL, 0, 0);			
		if (iSelect != LB_ERR) EndDialog(iSelect);

		EndDialog(-2);
	}


	//////////////////////////////////////////////////////////
	// Definitions for the CTab class
	//
	inline CTab::CTab() : m_pView(NULL), m_bShowButtons(FALSE), m_nTabHeight(20)
	{
		m_himlTab = ImageList_Create(16, 16, ILC_MASK|ILC_COLOR32, 0, 0);
		TabCtrl_SetImageList(m_hWnd, m_himlTab);
	}
		
	inline CTab::~CTab()
	{
		if (IsWindow()) TRACE("Tab Window destroyed in destructor\n !");
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
			SelectPage(iNewPage);
		}

		SetView(*pWnd);
		NotifyChanged();
	}

	inline void CTab::AddTabPage(CWnd* pWnd, LPCTSTR szTitle, UINT nID_Icon)
	{
		HICON hIcon = LoadIcon(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(nID_Icon));
		AddTabPage(pWnd, szTitle, hIcon);
	}

	inline void CTab::DrawCloseButton(CDC& DrawDC, UINT uState)
	{
		// The close button isn't displayed on Win95 
		if (GetWinVersion() == 1400)  return;
		
		// Determine the close button's drawing position relative to the window
		CRect rcClose = m_rcClose;

		// Draw the outer highlight for the close button
		if (!IsRectEmpty(&rcClose))
		{
			switch (uState)
			{
			case 0:
				{
					DrawDC.CreatePen(PS_SOLID, 1, RGB(232, 228, 220));
					
					::MoveToEx(DrawDC, rcClose.left, rcClose.bottom, NULL);
					::LineTo(DrawDC, rcClose.right, rcClose.bottom);
					::LineTo(DrawDC, rcClose.right, rcClose.top);
					::LineTo(DrawDC, rcClose.left, rcClose.top);
					::LineTo(DrawDC, rcClose.left, rcClose.bottom);
					break;
				}

			case 1:
				{
					// Draw outline, white at top, black on bottom
					DrawDC.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					::MoveToEx(DrawDC, rcClose.left, rcClose.bottom, NULL);
					::LineTo(DrawDC, rcClose.right, rcClose.bottom);
					::LineTo(DrawDC, rcClose.right, rcClose.top);
					DrawDC.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
					::LineTo(DrawDC, rcClose.left, rcClose.top);
					::LineTo(DrawDC, rcClose.left, rcClose.bottom);
				}

				break;
			case 2:
				{
					// Draw outline, black on top, white on bottom
					DrawDC.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
					::MoveToEx(DrawDC, rcClose.left, rcClose.bottom, NULL);
					::LineTo(DrawDC, rcClose.right, rcClose.bottom);
					::LineTo(DrawDC, rcClose.right, rcClose.top);
					DrawDC.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					::LineTo(DrawDC, rcClose.left, rcClose.top);
					::LineTo(DrawDC, rcClose.left, rcClose.bottom);
				}
				break;
			}

			// Manually draw close button
			DrawDC.CreatePen(PS_SOLID, 1, RGB(64, 64, 64));

			::MoveToEx(DrawDC, rcClose.left + 3, rcClose.top +3, NULL);
			::LineTo(DrawDC, rcClose.right - 3, rcClose.bottom -3);

			::MoveToEx(DrawDC, rcClose.left + 4, rcClose.top +3, NULL);
			::LineTo(DrawDC, rcClose.right - 3, rcClose.bottom -4);

			::MoveToEx(DrawDC, rcClose.left + 3, rcClose.top +4, NULL);
			::LineTo(DrawDC, rcClose.right - 4, rcClose.bottom -3);

			::MoveToEx(DrawDC, rcClose.right -4, rcClose.top +3, NULL);
			::LineTo(DrawDC, rcClose.left + 2, rcClose.bottom -3);

			::MoveToEx(DrawDC, rcClose.right -4, rcClose.top +4, NULL);
			::LineTo(DrawDC, rcClose.left + 3, rcClose.bottom -3);

			::MoveToEx(DrawDC, rcClose.right -5, rcClose.top +3, NULL);
			::LineTo(DrawDC, rcClose.left + 2, rcClose.bottom -4);
		}
	}

	inline void CTab::DrawListButton(CDC& DrawDC, UINT uState)
	{
		// The list button isn't displayed on Win95 
		if (GetWinVersion() == 1400)  return;
		
		// Determine the list button's drawing position relative to the window
		CRect rcList = m_rcList;

		// Draw the outer highlight for the list button
		if (!IsRectEmpty(&rcList))
		{
			switch (uState)
			{
			case 0:
				{
					DrawDC.CreatePen(PS_SOLID, 1, RGB(232, 228, 220));
					
					::MoveToEx(DrawDC, rcList.left, rcList.bottom, NULL);
					::LineTo(DrawDC, rcList.right, rcList.bottom);
					::LineTo(DrawDC, rcList.right, rcList.top);
					::LineTo(DrawDC, rcList.left, rcList.top);
					::LineTo(DrawDC, rcList.left, rcList.bottom);
					break;
				}

			case 1:
				{
					// Draw outline, white at top, black on bottom
					DrawDC.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					::MoveToEx(DrawDC, rcList.left, rcList.bottom, NULL);
					::LineTo(DrawDC, rcList.right, rcList.bottom);
					::LineTo(DrawDC, rcList.right, rcList.top);
					DrawDC.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
					::LineTo(DrawDC, rcList.left, rcList.top);
					::LineTo(DrawDC, rcList.left, rcList.bottom);
				}

				break;
			case 2:
				{
					// Draw outline, black on top, white on bottom
					DrawDC.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
					::MoveToEx(DrawDC, rcList.left, rcList.bottom, NULL);
					::LineTo(DrawDC, rcList.right, rcList.bottom);
					::LineTo(DrawDC, rcList.right, rcList.top);
					DrawDC.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					::LineTo(DrawDC, rcList.left, rcList.top);
					::LineTo(DrawDC, rcList.left, rcList.bottom);
				}
				break;
			}

			// Manually draw list button
			DrawDC.CreatePen(PS_SOLID, 1, RGB(64, 64, 64));

			for (int i = 0; i <= 4; i++)
			{
				int Length = 9 - 2*i;
				::MoveToEx(DrawDC, rcList.left + 3 + i, rcList.top +5 +i, NULL);
				::LineTo(DrawDC, rcList.left + 3 + i + Length, rcList.top +5 +i);
			}
		}
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
				int iImageSize = 20;
				int iPadding = 4;
				if (tcItem.iImage >= 0)
					rcText.left += iImageSize;
				
				rcText.left += iPadding;
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
			bottom = MAX(rcTab.top, m_nTabHeight +4);
			top = bottom -3;
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
		
		// Draw a lighter line over the darker line for the selected tab
		dcMem.CreatePen(PS_SOLID, 1, RGB(248,248,248));
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

	inline SIZE CTab::GetMaxTabSize()
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
			tcItem.mask = TCIF_TEXT |TCIF_IMAGE;
			tcItem.cchTextMax = 32;
			tcItem.pszText = szTitle;
			TabCtrl_GetItem(m_hWnd, i, &tcItem);
			GetTextExtentPoint32(dc, szTitle, lstrlen(szTitle), &TempSize);

			int iImageSize = 0;
			int iPadding = 6;
			if (tcItem.iImage >= 0)
				iImageSize = 20;
			TempSize.cx += iImageSize + iPadding;

			if (TempSize.cx > Size.cx)
				Size = TempSize;
		}

		return Size;
	}

	inline int CTab::GetTabIndex(CWnd* pWnd)
	{
		for (int i = 0; i < (int)m_vTabPageInfo.size(); ++i)
		{
			if (m_vTabPageInfo[i].pWnd == pWnd)
				return i;
		}

		return -1;
	}

	inline TabPageInfo CTab::GetTabPageInfo(UINT nTab)
	{
		TabPageInfo tbi = {0};
		if (nTab < m_vTabPageInfo.size())
			tbi = m_vTabPageInfo[nTab];
		
		return tbi; 
	}

	inline void CTab::NotifyChanged()
	{
		NMHDR nmhdr = {0};
		nmhdr.hwndFrom = m_hWnd;
		nmhdr.code = UWM_TAB_CHANGED;
		::SendMessage(GetParent(), WM_NOTIFY, 0L, (LPARAM)&nmhdr);
	}

	inline void CTab::OnCreate()
	{			
		for (int i = 0; i < (int)m_vTabPageInfo.size(); ++i)
		{
			// Add tabs for each view.
			TCITEM tie = {0};
			tie.mask = TCIF_TEXT | TCIF_IMAGE;
			tie.iImage = m_vTabPageInfo[i].iImage;
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
			
		if (0 == GetItemCount())
		{
			// No tabs, so simply display a grey background and exit
			SolidFill(dcView, RGB(232, 228, 220), &rcClient);
			return;
		}

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
		dcMem.CreateSolidBrush(RGB(232, 228, 220));
		::PaintRgn(dcMem, hrgnClip);

		// Draw the tab buttons on the memory DC:
		DrawTabs(dcMem);
		
		// Draw the close button if required
		if (m_bShowButtons)
		{
			m_rcClose = GetClientRect();
			m_rcClose.left = m_rcClose.right - 16;
			m_rcClose.top = 2;
			m_rcClose.bottom = m_rcClose.top + 16;

			m_rcList = m_rcClose;
			m_rcList.OffsetRect(-20, 0);

		//	HRGN hrgnClip3 = ::CreateRectRgn(m_rcClose.left, m_rcClose.top, m_rcClose.right, m_rcClose.bottom);
		//	CombineRgn(hrgnClip, hrgnClip, hrgnClip3, RGN_OR);
		//	::DeleteObject(hrgnClip3);
			DrawCloseButton(dcMem, 0);
			DrawListButton(dcMem, 0);
		}  

		DrawTabBorders(dcMem, rcTab);

		// Now copy our from our memory DC to the window DC
		dcMem.DetachClipRegion();
		dcView.AttachClipRegion(hrgnClip);
		BitBlt(dcView, 0, 0, rcClient.Width(), rcClient.Height(), dcMem, 0, 0, SRCCOPY);
		dcView.DetachClipRegion();

		// Cleanup
		::DeleteObject(hrgnSrc1);
		::DeleteObject(hrgnSrc2);
		::DeleteObject(hrgnClip);
	}

	inline void CTab::PreCreate(CREATESTRUCT &cs)
	{
		// For Tabs on the bottom, add the TCS_BOTTOM style
		cs.style = WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | TCS_OWNERDRAWFIXED | TCS_FIXEDWIDTH;
		cs.lpszClass = WC_TABCONTROL;
	}

	inline void CTab::RecalcLayout()
	{
		if (GetView())
			{
				// Set the tab sizes
				SetTabSize();

				// Position the View over the tab control's display area
				CRect rc = GetClientRect();
				TabCtrl_AdjustRect(m_hWnd, FALSE, &rc);						
				GetView()->SetWindowPos(HWND_TOP, rc, SWP_SHOWWINDOW);
			}
	}

	inline void CTab::RemoveTabPage(int iPage)
	{
		if ((iPage < 0) || (iPage > GetItemCount()-1))
			return;

		// Remove the tab
		DeleteItem(iPage);		
	
		// Remove the TapPageInfo entry
		std::vector<TabPageInfo>::iterator iter = m_vTabPageInfo.begin() + iPage;
		int iImage = (*iter).iImage;
		if (iImage >= 0) 
			TabCtrl_RemoveImage(m_hWnd, iImage);

		(*iter).pWnd->Destroy();
		delete (*iter).pWnd;
		m_vTabPageInfo.erase(iter);

		if (m_vTabPageInfo.size() > 0)
		{
			SetTabSize();
			SelectPage(0);
		}
		else
			m_pView = NULL;

		NotifyChanged();
	}

	inline void CTab::SelectPage(int iPage)
	{
		if ((iPage >= 0) && (iPage < GetItemCount()))
		{
			if (GetView() && (GetView()->IsWindow())) 
				GetView()->ShowWindow(SW_HIDE);
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
			
			int xGap = 2;
			if (m_bShowButtons) xGap += 30;

			int nItemWidth = MIN( GetMaxTabSize().cx, (rc.Width() - xGap)/GetItemCount() );
			SendMessage(TCM_SETITEMSIZE, 0L, MAKELPARAM(nItemWidth, m_nTabHeight));
			NotifyChanged();
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

	inline void CTab::ShowListMenu()
	// Displays the list of windows in a popup menu
	{	
		HMENU hMenu = CreatePopupMenu();
		
		// Add the menu items
		for(UINT u = 0; u < MIN(GetAllTabs().size(), 9); ++u)
		{
			TCHAR szMenuString[MAX_MENU_STRING+1];
			TCHAR szTitle[MAX_MENU_STRING];
			lstrcpyn(szTitle, GetAllTabs()[u].szTitle, MAX_MENU_STRING -4);
			wsprintf(szMenuString, _T("&%d %s"), u+1, szTitle);
			AppendMenu(hMenu, MF_STRING, IDW_FIRSTCHILD +u, szMenuString);
		}
		if (GetAllTabs().size() >= 10)
			AppendMenu(hMenu, MF_STRING, IDW_FIRSTCHILD +9, _T("More Windows"));

		// Add a checkmark to the menu
		int iSelected = GetCurSel();
		if (iSelected < 9) 
			CheckMenuItem(hMenu, iSelected, MF_BYPOSITION|MF_CHECKED);

		CPoint pt(GetListRect().left, GetTabHeight());
		MapWindowPoints(m_hWnd, NULL, &pt, 1);

		// Choosing the frame's hwnd for the menu's messages will automatically theme the popup menu
		HWND MenuHwnd = GetAncestor();

		int iPage = TrackPopupMenuEx(hMenu, TPM_LEFTALIGN|TPM_TOPALIGN|TPM_RETURNCMD, pt.x, pt.y, MenuHwnd, NULL) - IDW_FIRSTCHILD;
		if ((iPage >= 0) && (iPage < 10)) SelectPage(iPage);
		if (iPage == 10) ShowListDialog();

		::DestroyMenu(hMenu);
	}

	inline void CTab::ShowListDialog()
	{
		// Definition of a dialog template which displays a List Box
		unsigned char dlg_Template[] = 
		{
			0x01,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0xc8,0x00,0xc8,0x90,0x03,
			0x00,0x00,0x00,0x00,0x00,0xdc,0x00,0x8e,0x00,0x00,0x00,0x00,0x00,0x53,0x00,0x65,
			0x00,0x6c,0x00,0x65,0x00,0x63,0x00,0x74,0x00,0x20,0x00,0x57,0x00,0x69,0x00,0x6e,
			0x00,0x64,0x00,0x6f,0x00,0x77,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x01,0x4d,
			0x00,0x53,0x00,0x20,0x00,0x53,0x00,0x68,0x00,0x65,0x00,0x6c,0x00,0x6c,0x00,0x20,
			0x00,0x44,0x00,0x6c,0x00,0x67,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x01,0x00,0x01,0x50,0x40,0x00,0x7a,0x00,0x25,0x00,0x0f,0x00,0x01,
			0x00,0x00,0x00,0xff,0xff,0x80,0x00,0x4f,0x00,0x4b,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x50,0x7a,0x00,0x7a,0x00,0x25,
			0x00,0x0f,0x00,0x02,0x00,0x00,0x00,0xff,0xff,0x80,0x00,0x43,0x00,0x61,0x00,0x6e,
			0x00,0x63,0x00,0x65,0x00,0x6c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			0x00,0x02,0x00,0x01,0x01,0x21,0x50,0x06,0x00,0x06,0x00,0xcf,0x00,0x6d,0x00,0x79,
			0x00,0x00,0x00,0xff,0xff,0x83,0x00,0x00,0x00,0x00,0x00
		};

		// Display the modal dialog. The dialog is defined in the dialog template rather
		// than in the resource script (rc) file.
		CSelectDialog MyDialog((LPCDLGTEMPLATE) dlg_Template);
		for(UINT u = 0; u < GetAllTabs().size(); ++u)
		{
			MyDialog.AddItem(GetAllTabs()[u].szTitle);
		}
		
		int iSelected = MyDialog.DoModal();	
		if (iSelected >= 0) SelectPage(iSelected);
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
		case WM_NCHITTEST:
			{
				// Ensure we have an arrow cursor when the tab has no view window
				if (0 == GetAllTabs().size())
					SetCursor(LoadCursor(NULL, IDC_ARROW));
				
				// Cause WM_LBUTTONUP and WM_LBUTTONDOWN messages to be sent for buttons
				CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				MapWindowPoints(NULL, m_hWnd, &pt, 1);				
				if (GetCloseRect().PtInRect(pt)) return HTCLIENT;
				if (GetListRect().PtInRect(pt))  return HTCLIENT;
			}
			break;
		case WM_LBUTTONDOWN:
			{
				CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				
				if (GetCloseRect().PtInRect(pt))
				{
					RemoveTabPage(GetCurSel());
				}

				if (GetListRect().PtInRect(pt))
				{
					ShowListMenu();
				}
			}
			break;
		case WM_LBUTTONUP:
			break;
		case WM_WINDOWPOSCHANGED:
			RecalcLayout();
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
	
	////////////////////////////////////////
	// Definitions for the CTabbedMDI class
	inline CTabbedMDI::CTabbedMDI()
	{
		m_Tab.SetShowButtons(TRUE);
	}

	inline CWnd* CTabbedMDI::AddMDIChild(CWnd* pWnd, LPCTSTR szTabText)
	{
		if (NULL == pWnd)
			throw CWinException(_T("Cannot add Null MDI Child"));
		
		// Fake a WM_MOUSEACTIVATE to propogate focus change to dockables
		::SendMessage(GetParent(), WM_MOUSEACTIVATE, (WPARAM)GetAncestor(), MAKELPARAM(HTCLIENT,WM_LBUTTONDOWN));

		m_Tab.AddTabPage(pWnd, szTabText);
		if (!m_Tab.IsWindow())
		{
			m_Tab.Create(m_hWnd);
		}	

		RecalcLayout();
		return pWnd;
	}

	inline void CTabbedMDI::CloseActiveMDI()
	{
		int nTab = m_Tab.GetCurSel();
		m_Tab.RemoveTabPage(nTab);
		RecalcLayout();
	}

	inline HWND CTabbedMDI::Create(HWND hWndParent /* = NULL*/)
	{
		CLIENTCREATESTRUCT clientcreate ;
		clientcreate.hWindowMenu  = m_hWnd;
		clientcreate.idFirstChild = IDW_FIRSTCHILD ;
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | MDIS_ALLCHILDSTYLES;

		// Create the MDICLIENT view window
		if (!CreateEx(0, _T("MDICLIENT"), _T(""),
			dwStyle, 0, 0, 0, 0, hWndParent, NULL, (PSTR) &clientcreate))
				throw CWinException(_T("CMDIClient::Create ... CreateEx failed"));

		return m_hWnd;
	}

	inline CWnd* CTabbedMDI::GetActiveMDIChild()
	{
		int nTab = m_Tab.GetCurSel();
		TabPageInfo tbi = m_Tab.GetTabPageInfo(nTab);
		return tbi.pWnd;
	}

	inline LRESULT CTabbedMDI::OnNotify(WPARAM /*wParam*/, LPARAM lParam)
	{
		LPNMHDR pnmhdr = (LPNMHDR)lParam;
		if (pnmhdr->code == UWM_TAB_CHANGED)
			RecalcLayout();

		return 0L;
	}

	inline void CTabbedMDI::RecalcLayout()
	{
		if (m_Tab.GetItemCount() >0)
		{
			CRect rc = GetClientRect();
			m_Tab.SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
		}
		else
		{
			m_Tab.ShowWindow(SW_HIDE);
		}
	}

	inline void CTabbedMDI::SetActiveMDIChild(CWnd* pWnd)
	{
		int iPage = m_Tab.GetTabIndex(pWnd);
		if (iPage >= 0)
			m_Tab.SelectPage(iPage);
	}

	inline void CTabbedMDI::SetActiveTab(int iTab)
	{
		m_Tab.SelectPage(iTab);
	}

	inline void CTabbedMDI::ShowListMenu()
	{
		// Displays the list of windows in a popup menu
		HMENU hMenu = CreatePopupMenu();
		
		for(UINT u = 0; u < m_Tab.GetAllTabs().size(); ++u)
		{
			AppendMenu(hMenu, MF_STRING, u+1, m_Tab.GetAllTabs()[u].szTitle);
		}

		CPoint pt(m_Tab.GetListRect().left, m_Tab.GetTabHeight());
		MapWindowPoints(m_hWnd, NULL, &pt, 1);

		int iPage = TrackPopupMenuEx(hMenu, TPM_LEFTALIGN|TPM_TOPALIGN|TPM_RETURNCMD, pt.x, pt.y, m_hWnd, NULL) - 1;
		if (iPage >= 0) m_Tab.SelectPage(iPage);

		::DestroyMenu(hMenu);
	}

	inline LRESULT CTabbedMDI::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch(uMsg)
		{
		case WM_ERASEBKGND:
			if (m_Tab.GetItemCount() >0)
				return 0;
			break;
		case WM_WINDOWPOSCHANGED:
			RecalcLayout();
			break;
		case WM_LBUTTONDOWN:
			{
				CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			}
			break;
		case WM_NCHITTEST:
			{
				CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				::MapWindowPoints(NULL, m_hWnd, &pt, 1);
			}
			break;
		}

		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

} // namespace Win32xx

#endif  // _TAB_H_
