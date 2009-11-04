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


///////////////////////////////////////////////////////
// tab.h
//  Declaration of the CTab and CMDITab classes

#ifndef _TAB_H_
#define _TAB_H_

#include "wincore.h"
#include "dialog.h"
#include "gdi.h"
#include "Default_Resource.h"

namespace Win32xx
{

	struct TabPageInfo
	{
		TCHAR szTitle[MAX_MENU_STRING];
		int iImage;
		int nID;
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
			virtual BOOL IsTab() const { return FALSE; }

		protected:
			virtual BOOL OnInitDialog();
			virtual void OnOK();
			virtual void OnCancel() { EndDialog(-2); }

		private:
			CSelectDialog(const CSelectDialog&);				// Disable copy construction
			CSelectDialog& operator = (const CSelectDialog&); // Disable assignment operator

			std::vector<tString> m_vItems;
			int IDC_LIST;

		};
	public:
		CTab();
		virtual ~CTab();
		virtual int  AddTabPage(TabPageInfo& tbi);
		virtual int  AddTabPage(CWnd* pWnd, LPCTSTR szTitle, HICON hIcon);
		virtual int  AddTabPage(CWnd* pWnd, LPCTSTR szTitle, UINT nID_Icon = 0);
		virtual CRect GetCloseRect();
		virtual CRect GetListRect();
		virtual BOOL GetTabsAtTop();
		virtual int  GetTabIndex(CWnd* pWnd);
		virtual TabPageInfo GetTabPageInfo(UINT nTab);
		virtual void RecalcLayout();
		virtual void RemoveTabPage(int iPage);
		virtual void SelectPage(int iPage);
		virtual void SetShowButtons(BOOL bShow);
		virtual void SetTabImage(UINT nTab, int iImage);
		virtual void SetTabsAtTop(BOOL bTop);
		virtual void SetTabText(UINT nTab, LPCTSTR szText);
		virtual void SwapTabs(UINT nTab1, UINT nTab2);

		// Attributes
		std::vector <TabPageInfo>& GetAllTabs() const { return (std::vector <TabPageInfo>&) m_vTabPageInfo; }
		HIMAGELIST GetImageList() const { return m_himlTab; }
		BOOL GetShowButtons() const { return m_bShowButtons; }
		int GetTabHeight() const { return m_nTabHeight; }
		CWnd* GetActiveView() const		{ return m_pView; }
		void SetTabHeight(int nTabHeight) { m_nTabHeight = nTabHeight; NotifyChanged();}

		// Wrappers for Win32 Macros
		void AdjustRect(BOOL fLarger, RECT *prc);
		int  GetCurFocus();
		int  GetCurSel();
		BOOL GetItem(int iItem, LPTCITEM pitem);
		int  GetItemCount();
		int  HitTest(TCHITTESTINFO& info);
		void SetCurFocus(int iItem);
		int  SetCurSel(int iItem);
		DWORD SetItemSize(int cx, int cy);
		int  SetMinTabWidth(int cx);
		void SetPadding(int cx, int cy);

	protected:
		virtual void	DrawCloseButton(CDC& DrawDC);
		virtual void	DrawListButton(CDC& DrawDC);
		virtual void	DrawTabs(CDC& dcMem);
		virtual void	DrawTabBorders(CDC& dcMem, CRect& rcTab);
		virtual SIZE    GetMaxTabSize();
		virtual void    OnCreate();
		virtual void    OnLButtonDown(WPARAM wParam, LPARAM lParam);
		virtual void    OnLButtonUp(WPARAM wParam, LPARAM lParam);
		virtual void    OnMouseLeave(WPARAM wParam, LPARAM lParam);
		virtual void    OnMouseMove(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNCHitTest(WPARAM wParam, LPARAM lParam);
		virtual void	NotifyChanged();
		virtual void	Paint();
		virtual void    PreCreate(CREATESTRUCT& cs);
		virtual BOOL	PreTranslateMessage(MSG* pMsg);
		virtual void    SetTabSize();
		virtual void	SetActiveView(CWnd& Wnd);
		virtual void	ShowListDialog();
		virtual void	ShowListMenu();
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		CTab(const CTab&);				// Disable copy construction
		CTab& operator = (const CTab&); // Disable assignment operator

		std::vector<TabPageInfo> m_vTabPageInfo;
		HIMAGELIST m_himlTab;
		CWnd* m_pView;
		BOOL m_bShowButtons;	// Show or hide the close and list button
		BOOL m_IsTracking;
		BOOL m_IsClosePressed;
		BOOL m_IsListPressed;
		BOOL m_IsListMenuActive;
		int m_nTabHeight;
	};

	////////////////////////////////////////
	// Declaration of the CTabbedMDI class
	class CTabbedMDI : public CWnd
	{
	public:
		CTabbedMDI();
		virtual ~CTabbedMDI();
		virtual CWnd* AddMDIChild(CWnd* pWnd, LPCTSTR szTabText, int nID = 0);
		virtual void  CloseActiveMDI();
		virtual void  CloseAllMDIChildren();
		virtual void  CloseMDIChild(int nTab);
		virtual CWnd* GetActiveMDIChild();
		virtual CWnd* GetMDIChild(int nTab) {return GetTab().GetTabPageInfo(nTab).pWnd;}
		virtual int   GetMDIChildCount();
		virtual int   GetMDIChildID(int nTab) {return GetTab().GetTabPageInfo(nTab).nID;}
		virtual LPCTSTR GetMDIChildTitle(int nTab) {return GetTab().GetTabPageInfo(nTab).szTitle;}
		virtual CTab& GetTab() const	{return (CTab&)m_Tab;}
		virtual BOOL IsTabbedMDI() const {return TRUE;}
		virtual BOOL LoadRegistrySettings(tString tsRegistryKeyName);
		virtual void RecalcLayout();
		virtual void SaveRegistrySettings(tString tsRegistryKeyName);
		virtual void SetActiveMDIChild(CWnd* pWnd);
		virtual void SetActiveMDITab(int nTab);

	protected:
		virtual HWND    Create(HWND hWndParent);
		virtual CWnd*   NewMDIChildFromID(int nID);
		virtual void    OnDestroy(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnEraseBkGnd(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual void    OnWindowPosChanged(WPARAM wParam, LPARAM lParam);
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		CTabbedMDI(const CTabbedMDI&);				// Disable copy construction
		CTabbedMDI& operator = (const CTabbedMDI&); // Disable assignment operator

		CTab m_Tab;
	};
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{

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
	inline CTab::CTab() : m_pView(NULL), m_bShowButtons(FALSE), m_IsTracking(FALSE), m_IsClosePressed(FALSE),
							m_IsListPressed(FALSE), m_IsListMenuActive(FALSE), m_nTabHeight(20)
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

	inline int CTab::AddTabPage(CWnd* pWnd, LPCTSTR szTitle, HICON hIcon)
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
			TabCtrl_InsertItem(m_hWnd, iNewPage, &tie);

			SetTabSize();
			SelectPage(iNewPage);
		}

		// Adjust the window styles if needed
		DWORD dwStyle = (DWORD)GetWindowLongPtr(GWL_STYLE);
		if (!(dwStyle & TCS_OWNERDRAWFIXED) || !(dwStyle & TCS_FIXEDWIDTH))
		{
			dwStyle |= TCS_OWNERDRAWFIXED | TCS_FIXEDWIDTH;
			SetWindowLongPtr(GWL_STYLE, dwStyle);
		}

		SetActiveView(*pWnd);
		NotifyChanged();
		return iNewPage;
	}

	inline int CTab::AddTabPage(TabPageInfo& tbi)
	{
		int iNewPage = AddTabPage(tbi.pWnd, tbi.szTitle);
		m_vTabPageInfo[iNewPage].nID = tbi.nID;
		return iNewPage;
	}

	inline int CTab::AddTabPage(CWnd* pWnd, LPCTSTR szTitle, UINT nID_Icon)
	{
		HICON hIcon = LoadIcon(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(nID_Icon));
		return AddTabPage(pWnd, szTitle, hIcon);
	}

	inline void CTab::DrawCloseButton(CDC& DrawDC)
	{
		// The close button isn't displayed on Win95
		if (GetWinVersion() == 1400)  return;

		if (!m_bShowButtons) return;

		// Determine the close button's drawing position relative to the window
		CRect rcClose = GetCloseRect();

		CPoint pt = GetCursorPos();
		MapWindowPoints(NULL, m_hWnd, &pt, 1);
		UINT uState = rcClose.PtInRect(pt)? m_IsClosePressed? 2: 1: 0;

		// Draw the outer highlight for the close button
		if (!IsRectEmpty(&rcClose))
		{
			switch (uState)
			{
			case 0:
				{
					DrawDC.CreatePen(PS_SOLID, 1, RGB(232, 228, 220));

					DrawDC.MoveTo(rcClose.left, rcClose.bottom);
					DrawDC.LineTo(rcClose.right, rcClose.bottom);
					DrawDC.LineTo(rcClose.right, rcClose.top);
					DrawDC.LineTo(rcClose.left, rcClose.top);
					DrawDC.LineTo(rcClose.left, rcClose.bottom);
					break;
				}

			case 1:
				{
					// Draw outline, white at top, black on bottom
					DrawDC.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					DrawDC.MoveTo(rcClose.left, rcClose.bottom);
					DrawDC.LineTo(rcClose.right, rcClose.bottom);
					DrawDC.LineTo(rcClose.right, rcClose.top);
					DrawDC.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
					DrawDC.LineTo(rcClose.left, rcClose.top);
					DrawDC.LineTo(rcClose.left, rcClose.bottom);
				}

				break;
			case 2:
				{
					// Draw outline, black on top, white on bottom
					DrawDC.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
					DrawDC.MoveTo(rcClose.left, rcClose.bottom);
					DrawDC.LineTo(rcClose.right, rcClose.bottom);
					DrawDC.LineTo(rcClose.right, rcClose.top);
					DrawDC.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					DrawDC.LineTo(rcClose.left, rcClose.top);
					DrawDC.LineTo(rcClose.left, rcClose.bottom);
				}
				break;
			}

			// Manually draw close button
			DrawDC.CreatePen(PS_SOLID, 1, RGB(64, 64, 64));

			DrawDC.MoveTo(rcClose.left + 3, rcClose.top +3);
			DrawDC.LineTo(rcClose.right - 3, rcClose.bottom -3);

			DrawDC.MoveTo(rcClose.left + 4, rcClose.top +3);
			DrawDC.LineTo(rcClose.right - 3, rcClose.bottom -4);

			DrawDC.MoveTo(rcClose.left + 3, rcClose.top +4);
			DrawDC.LineTo(rcClose.right - 4, rcClose.bottom -3);

			DrawDC.MoveTo(rcClose.right -4, rcClose.top +3);
			DrawDC.LineTo(rcClose.left + 2, rcClose.bottom -3);

			DrawDC.MoveTo(rcClose.right -4, rcClose.top +4);
			DrawDC.LineTo(rcClose.left + 3, rcClose.bottom -3);

			DrawDC.MoveTo(rcClose.right -5, rcClose.top +3);
			DrawDC.LineTo(rcClose.left + 2, rcClose.bottom -4);
		}
	}

	inline void CTab::DrawListButton(CDC& DrawDC)
	{
		// The list button isn't displayed on Win95
		if (GetWinVersion() == 1400)  return;

		if (!m_bShowButtons) return;

		// Determine the list button's drawing position relative to the window
		CRect rcList = GetListRect();

		CPoint pt = GetCursorPos();
		MapWindowPoints(NULL, m_hWnd, &pt, 1);
		UINT uState = rcList.PtInRect(pt)? 1: 0;
		if (m_IsListMenuActive) uState = 2; 

		// Draw the outer highlight for the list button
		if (!IsRectEmpty(&rcList))
		{
			switch (uState)
			{
			case 0:
				{
					DrawDC.CreatePen(PS_SOLID, 1, RGB(232, 228, 220));

					DrawDC.MoveTo(rcList.left, rcList.bottom);
					DrawDC.LineTo(rcList.right, rcList.bottom);
					DrawDC.LineTo(rcList.right, rcList.top);
					DrawDC.LineTo(rcList.left, rcList.top);
					DrawDC.LineTo(rcList.left, rcList.bottom);
					break;
				}

			case 1:
				{
					// Draw outline, white at top, black on bottom
					DrawDC.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					DrawDC.MoveTo(rcList.left, rcList.bottom);
					DrawDC.LineTo(rcList.right, rcList.bottom);
					DrawDC.LineTo(rcList.right, rcList.top);
					DrawDC.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
					DrawDC.LineTo(rcList.left, rcList.top);
					DrawDC.LineTo(rcList.left, rcList.bottom);
				}

				break;
			case 2:
				{
					// Draw outline, black on top, white on bottom
					DrawDC.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
					DrawDC.MoveTo(rcList.left, rcList.bottom);
					DrawDC.LineTo(rcList.right, rcList.bottom);
					DrawDC.LineTo(rcList.right, rcList.top);
					DrawDC.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					DrawDC.LineTo(rcList.left, rcList.top);
					DrawDC.LineTo(rcList.left, rcList.bottom);
				}
				break;
			}

			// Manually draw list button
			DrawDC.CreatePen(PS_SOLID, 1, RGB(64, 64, 64));

			for (int i = 0; i <= 4; i++)
			{
				int Length = 9 - 2*i;
				DrawDC.MoveTo(rcList.left + 4 + i, rcList.top +6 +i);
				DrawDC.LineTo(rcList.left + 4 + i + Length, rcList.top +6 +i);
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
			if (!rcItem.IsRectEmpty())
			{
				if (i == TabCtrl_GetCurSel(m_hWnd))
				{
					dcMem.CreateSolidBrush(RGB(248,248,248));
					dcMem.SetBkColor(RGB(248,248,248));
				}
				else
				{
					dcMem.CreateSolidBrush(RGB(200,200,200));
					dcMem.SetBkColor(RGB(200,200,200));
				}

				dcMem.CreatePen(PS_SOLID, 1, RGB(160, 160, 160));
				dcMem.RoundRect(rcItem.left+1, rcItem.top, rcItem.right+2, rcItem.bottom, 6, 6);

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
					info.cbSize = GetSizeofNonClientMetrics();					
					SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
					dcMem.CreateFontIndirect(info.lfStatusFont);				

					// Calculate the size of the text
					CRect rcText = rcItem;

					int iImageSize = 20;
					int iPadding = 4;
					if (tcItem.iImage >= 0)
						rcText.left += iImageSize;

					rcText.left += iPadding;
					dcMem.DrawText(szText, -1, rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
				}
			}
		}   
	}

	inline void CTab::DrawTabBorders(CDC& dcMem, CRect& rcTab)
	{
		BOOL IsBottomTab = (BOOL)GetWindowLongPtr(GWL_STYLE) & TCS_BOTTOM;

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
		if (!rcItem.IsRectEmpty())
		{
			dcMem.Rectangle(left, top, right, bottom);

			// Draw a darker line below the rectangle
			dcMem.CreatePen(PS_SOLID, 1, RGB(160, 160, 160));
			if (IsBottomTab)
			{
				dcMem.MoveTo(left-1, bottom);
				dcMem.LineTo(right, bottom);
			}
			else
			{
				dcMem.MoveTo(left-1, top-1);
				dcMem.LineTo(right, top-1);
			}

			// Draw a lighter line over the darker line for the selected tab
			dcMem.CreatePen(PS_SOLID, 1, RGB(248,248,248));
			TabCtrl_GetItemRect(m_hWnd, TabCtrl_GetCurSel(m_hWnd), &rcItem);
			OffsetRect(&rcItem, 1, 1);

			if (IsBottomTab)
			{
				dcMem.MoveTo(rcItem.left, bottom);
				dcMem.LineTo(rcItem.right, bottom);
			}
			else
			{
				dcMem.MoveTo(rcItem.left, top-1);
				dcMem.LineTo(rcItem.right, top-1);
			}
		}
	}

	inline CRect CTab::GetCloseRect()
	{
		CRect rcClose;
		if (GetShowButtons())
		{
			rcClose= GetClientRect();
			int Gap = 2;
			int cx = GetSystemMetrics(SM_CXSMICON);
			int cy = GetSystemMetrics(SM_CYSMICON);
			rcClose.right -= Gap;
			rcClose.left = rcClose.right - cx;

			if (GetTabsAtTop())
				rcClose.top = Gap;
			else
				rcClose.top = MAX(Gap, rcClose.bottom - m_nTabHeight);

			rcClose.bottom = rcClose.top + cy;	
		}
		return rcClose;
	}

	inline CRect CTab::GetListRect()
	{
		CRect rcList;
		if (GetShowButtons())
		{
			rcList = GetCloseRect();		
			rcList.OffsetRect(-18, 0);
		}
		return rcList;
	}

	inline SIZE CTab::GetMaxTabSize()
	{
		CSize Size;

		for (int i = 0; i < TabCtrl_GetItemCount(m_hWnd); i++)
		{
			CDC dc = GetDC();
			NONCLIENTMETRICS info = {0};
			info.cbSize = GetSizeofNonClientMetrics();
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
			dc.CreateFontIndirect(info.lfStatusFont);
			TCHAR szTitle[32];
			TCITEM tcItem = {0};
			tcItem.mask = TCIF_TEXT |TCIF_IMAGE;
			tcItem.cchTextMax = 32;
			tcItem.pszText = szTitle;
			TabCtrl_GetItem(m_hWnd, i, &tcItem);
			CSize TempSize = dc.GetTextExtentPoint32(szTitle, lstrlen(szTitle));

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

	inline BOOL CTab::GetTabsAtTop()
	// Returns TRUE if the contol's tabs are placed at the top
	{
		DWORD dwStyle = (DWORD)GetWindowLongPtr(GWL_STYLE);
		return (!(dwStyle & TCS_BOTTOM));
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

	inline void CTab::OnLButtonDown(WPARAM /*wParam*/, LPARAM lParam)
	{
		CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		if (GetCloseRect().PtInRect(pt))
		{
			m_IsClosePressed = TRUE;
			SetCapture();
			CDC dc = GetDC();
			DrawCloseButton(dc);
		}
		else
			m_IsClosePressed = FALSE;

		if (GetListRect().PtInRect(pt))
		{
			ShowListMenu();
		}

		TCHITTESTINFO tchti = {0};
		tchti.pt = pt;
		int nPage = HitTest(tchti);
		if (nPage >= 0)
			SelectPage(nPage);
	}

	inline void CTab::OnLButtonUp(WPARAM /*wParam*/, LPARAM lParam)
	{
		ReleaseCapture();
		CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		if (m_IsClosePressed && GetCloseRect().PtInRect(pt))
			RemoveTabPage(GetCurSel());
		
		m_IsClosePressed = FALSE;
	}

	inline void CTab::OnMouseLeave(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		CDC dc = GetDC();
		DrawCloseButton(dc);
		DrawListButton(dc);

		m_IsTracking = FALSE; 
	}

	inline void CTab::OnMouseMove(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		if (!m_IsListMenuActive && m_IsListPressed)
		{
			m_IsListPressed = FALSE;
		}

		if (!IsLeftButtonDown())
		{
			m_IsClosePressed = FALSE;
			m_IsListPressed = FALSE;
		}	
		
		if (!m_IsTracking)
		{
			TRACKMOUSEEVENT TrackMouseEventStruct = {0};
			TrackMouseEventStruct.cbSize = sizeof(TrackMouseEventStruct);
			TrackMouseEventStruct.dwFlags = TME_LEAVE;
			TrackMouseEventStruct.hwndTrack = m_hWnd;
			_TrackMouseEvent(&TrackMouseEventStruct);
			m_IsTracking = TRUE;
		}
		
		CDC dc = GetDC(); 
		DrawCloseButton(dc);
		DrawListButton(dc);
	}

	inline LRESULT CTab::OnNCHitTest(WPARAM wParam, LPARAM lParam)
	{
		// Ensure we have an arrow cursor when the tab has no view window
		if (0 == GetAllTabs().size())
			SetCursor(LoadCursor(NULL, IDC_ARROW));

		// Cause WM_LBUTTONUP and WM_LBUTTONDOWN messages to be sent for buttons
		CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		MapWindowPoints(NULL, m_hWnd, &pt, 1);
		if (GetCloseRect().PtInRect(pt)) return HTCLIENT;
		if (GetListRect().PtInRect(pt))  return HTCLIENT;

		return CWnd::WndProcDefault(WM_NCHITTEST, wParam, lParam);
	}

	inline void CTab::Paint()
	{
		// Microsoft's drawing for a tab control is rubbish, so we do our own.
		// We use double buffering and regions to eliminate flicker

		// Create the memory DC and bitmap
		CDC dcMem = ::CreateCompatibleDC(NULL);
		int xAdjust = 0;

#if WINVER >= 0x0500
		if (GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_LAYOUTRTL)
		dcMem.SetLayout(LAYOUT_RTL);
		xAdjust = -2;	// Fudge factor for WS_EX_LAYOUTRTL exstyle
#endif

		CRect rcClient = GetClientRect();
		CDC dcView = GetDC();
		dcMem.CreateCompatibleBitmap(dcView, rcClient.Width(), rcClient.Height());

		if (0 == GetItemCount())
		{
			// No tabs, so simply display a grey background and exit
			COLORREF rgbDialog = GetSysColor(COLOR_BTNFACE);
			dcView.SolidFill(rgbDialog, rcClient);
			return;
		}

		// Create a clipping region. Its the overall tab window's region,
		//  less the region belonging to the individual tab view's client area
		HRGN hrgnSrc1 = ::CreateRectRgn(rcClient.left + xAdjust, rcClient.top, rcClient.right, rcClient.bottom);
		CRect rcTab = GetClientRect();
		TabCtrl_AdjustRect(m_hWnd, FALSE, &rcTab);
		if (rcTab.Height() < 0) 
			rcTab.top = rcTab.bottom;
		if (rcTab.Width() < 0) 
			rcTab.left = rcTab.right;

		HRGN hrgnSrc2 = ::CreateRectRgn(rcTab.left, rcTab.top, rcTab.right + xAdjust, rcTab.bottom);
		HRGN hrgnClip = ::CreateRectRgn(0, 0, 0, 0);
		::CombineRgn(hrgnClip, hrgnSrc1, hrgnSrc2, RGN_DIFF);

		// Use the region in the memory DC to paint the grey background
		dcMem.AttachClipRegion(hrgnClip);
		HWND hWndParent = GetParent();
		CDC dcParent = ::GetDC(hWndParent);
		HBRUSH hBrush = (HBRUSH)::SendMessage(hWndParent, WM_CTLCOLORDLG, (WPARAM)dcParent.GetHDC(), (LPARAM)hWndParent);
		dcMem.AttachBrush(hBrush);
		dcMem.PaintRgn(hrgnClip);

		// Draw the tab buttons on the memory DC:
		DrawTabs(dcMem);

		// Draw buttons and tab borders
		DrawCloseButton(dcMem);
		DrawListButton(dcMem);
		DrawTabBorders(dcMem, rcTab);

		// Now copy our from our memory DC to the window DC
		dcMem.DetachClipRegion();
		dcView.AttachClipRegion(hrgnClip);
		dcView.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), dcMem, 0, 0, SRCCOPY);
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

	inline BOOL CTab::PreTranslateMessage(MSG* pMsg)
	{
		// allow the tab control to translate keyboard input
		if ((pMsg->message >= WM_KEYFIRST) && (pMsg->message <= WM_KEYLAST))
		{
			if (IsDialogMessage(m_hWnd, pMsg))
				return TRUE;
		}

		return CWnd::PreTranslateMessage(pMsg);
	}

	inline void CTab::RecalcLayout()
	{
		if (GetActiveView())
		{
			// Set the tab sizes
			SetTabSize();

			// Position the View over the tab control's display area
			CRect rc = GetClientRect();
			TabCtrl_AdjustRect(m_hWnd, FALSE, &rc);
			GetActiveView()->SetWindowPos(HWND_TOP, rc, SWP_SHOWWINDOW);

		}
	}

	inline void CTab::RemoveTabPage(int iPage)
	{
		if ((iPage < 0) || (iPage > GetItemCount()-1))
			return;

		// Remove the tab
		TabCtrl_DeleteItem(m_hWnd, iPage);

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
			m_pView = GetAllTabs()[0].pWnd;
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
			if (GetActiveView() && (GetActiveView()->IsWindow()))
				GetActiveView()->ShowWindow(SW_HIDE);
			SetCurSel(iPage);
			SetActiveView(*(m_vTabPageInfo[iPage].pWnd));

			// Position the View over the tab control's display area
			CRect rc = GetClientRect();
			TabCtrl_AdjustRect(m_hWnd, FALSE, &rc);
			GetActiveView()->SetWindowPos(HWND_TOP, rc, SWP_SHOWWINDOW);

			GetActiveView()->SetFocus();
		}
	}

	inline void CTab::SetShowButtons(BOOL bShow)
	{ 
		m_bShowButtons = bShow;
		RecalcLayout();
	}

	inline void CTab::SetTabImage(UINT nTab, int iImage)
	{
		// Allows the image to be changed on an existing tab
		if (nTab < GetAllTabs().size())
		{
			TCITEM Item = {0};
			Item.mask = TCIF_TEXT;
			Item.iImage = iImage;

			if (TabCtrl_SetItem(m_hWnd, nTab, &Item))
				m_vTabPageInfo[nTab].iImage = iImage;
		}
	}

	inline void CTab::SetTabsAtTop(BOOL bTop)
	// Positions the tabs at the top or botttom of the control
	{
		DWORD dwStyle = (DWORD)GetWindowLongPtr(GWL_STYLE);

		if (bTop)
			dwStyle &= ~TCS_BOTTOM;
		else
			dwStyle |= TCS_BOTTOM;

		SetWindowLongPtr(GWL_STYLE, dwStyle);
		RecalcLayout();
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
			nItemWidth = MAX(nItemWidth, 0);
			SendMessage(TCM_SETITEMSIZE, 0L, MAKELPARAM(nItemWidth, m_nTabHeight));
			NotifyChanged();
		}
	}

	inline void CTab::SetTabText(UINT nTab, LPCTSTR szText)
	{
		// Allows the text to be changed on an existing tab
		if (nTab < GetAllTabs().size())
		{
			TCITEM Item = {0};
			TCHAR* Text = new TCHAR[MAX_MENU_STRING+1];
			if (Text == NULL) throw std::bad_alloc();
			lstrcpyn(Text, szText, MAX_MENU_STRING);
			Item.mask = TCIF_TEXT;
			Item.pszText = Text;

			if (TabCtrl_SetItem(m_hWnd, nTab, &Item))
				lstrcpyn(m_vTabPageInfo[nTab].szTitle, Text, MAX_MENU_STRING);
				
			delete [] Text;
		}
	}

	inline void CTab::SetActiveView(CWnd& Wnd)
	// Sets or changes the View window displayed within the tab page
	{
		// Assign the view window
		m_pView = &Wnd;

		if (m_hWnd)
		{
			if (!m_pView->IsWindow())
			{
				// The tab control is already created, so create the new view too
				GetActiveView()->Create(m_hWnd);
			}
		}
	}

	inline void CTab::ShowListMenu()
	// Displays the list of windows in a popup menu
	{
		if (!m_IsListPressed)
		{
			m_IsListPressed = TRUE;
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

			CPoint pt(GetListRect().left, GetListRect().top + GetTabHeight());
			MapWindowPoints(m_hWnd, NULL, &pt, 1);

			// Choosing the frame's hwnd for the menu's messages will automatically theme the popup menu
			HWND MenuHwnd = GetAncestor();
			int iPage = 0;
			m_IsListMenuActive = TRUE;
			iPage = TrackPopupMenuEx(hMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, pt.x, pt.y, MenuHwnd, NULL) - IDW_FIRSTCHILD;
			if ((iPage >= 0) && (iPage < 9)) SelectPage(iPage);
			if (iPage == 9) ShowListDialog();
			m_IsListMenuActive = FALSE;

			::DestroyMenu(hMenu);
		}

		CDC dc = GetDC();
		DrawListButton(dc);
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

		int iSelected = (int)MyDialog.DoModal();
		if (iSelected >= 0) SelectPage(iSelected);
	}

	inline void CTab::SwapTabs(UINT nTab1, UINT nTab2)
	{
		if ((nTab1 < GetAllTabs().size()) && (nTab2 < GetAllTabs().size()) && (nTab1 != nTab2))
		{
			int iPage = GetCurSel();
			TabPageInfo T1 = GetTabPageInfo(nTab1);
			TabPageInfo T2 = GetTabPageInfo(nTab2);

			SetTabImage(nTab1, T2.iImage);
			SetTabImage(nTab2, T1.iImage);
			SetTabText(nTab1, T2.szTitle);
			SetTabText(nTab2, T1.szTitle);
			m_vTabPageInfo[nTab1] = T2;
			m_vTabPageInfo[nTab2] = T1;
			SelectPage(iPage);		
		}
	}

	inline LRESULT CTab::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch(uMsg)
		{
		case WM_PAINT:
			{
				// Remove all pending paint requests
				PAINTSTRUCT ps;
				::BeginPaint(m_hWnd, &ps);
				::EndPaint(m_hWnd, &ps);

				// Now call our local Paint
				Paint();
			}
			return 0;
		case WM_ERASEBKGND:
			return 0;

		case WM_KILLFOCUS:
			m_IsClosePressed = FALSE;
			break;
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONDOWN:
			// Do default processing first
			CallWindowProc(GetPrevWindowProc(), uMsg, wParam, lParam);
			
			// Now do our processing
			OnLButtonDown(wParam, lParam);
			return 0L;
		case WM_LBUTTONUP:
			OnLButtonUp(wParam, lParam);
			break;
		case WM_MOUSEMOVE:
			OnMouseMove(wParam, lParam);
			break;
		case WM_MOUSELEAVE:
			OnMouseLeave(wParam, lParam);
			break;
		case WM_NCHITTEST:
			return OnNCHitTest(wParam, lParam);
		
		case WM_WINDOWPOSCHANGED:
			RecalcLayout();
			break;
		}

		// pass unhandled messages on for default processing
		return CWnd::WndProcDefault(uMsg, wParam, lParam);
	}

	// Wrappers for Win32 Macros
	inline void CTab::AdjustRect(BOOL fLarger, RECT *prc)
	{
		TabCtrl_AdjustRect(m_hWnd, fLarger, prc);
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

	inline int CTab::HitTest(TCHITTESTINFO& info)
	{
		return TabCtrl_HitTest(m_hWnd, &info);
	}

	inline void CTab::SetCurFocus(int iItem)
	{
		TabCtrl_SetCurFocus(m_hWnd, iItem);
	}

	inline int CTab::SetCurSel(int iItem)
	{
		return TabCtrl_SetCurSel(m_hWnd, iItem);
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
		GetTab().SetShowButtons(TRUE);
	}

	inline CTabbedMDI::~CTabbedMDI()
	{
	}

	inline CWnd* CTabbedMDI::AddMDIChild(CWnd* pWnd, LPCTSTR szTabText, int nID /*= 0*/)
	{
		if (NULL == pWnd)
			throw CWinException(_T("Cannot add Null MDI Child"));

		// Fake a WM_MOUSEACTIVATE to propogate focus change to dockers
		::SendMessage(GetParent(), WM_MOUSEACTIVATE, (WPARAM)GetAncestor(), MAKELPARAM(HTCLIENT,WM_LBUTTONDOWN));

		TabPageInfo tpi = {0};
		tpi.nID = nID;
		tpi.pWnd = pWnd;
		lstrcpyn(tpi.szTitle, szTabText, 79);
		tpi.szTitle[79] = '\0';

		if (!GetTab().IsWindow())
		{
			GetTab().Create(m_hWnd);
			CRect rc = GetClientRect();
			GetTab().SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
		}

		GetTab().AddTabPage(tpi);	
		return pWnd;
	}

	inline void CTabbedMDI::CloseActiveMDI()
	{
		int nTab = GetTab().GetCurSel();
		GetTab().RemoveTabPage(nTab);
		RecalcLayout();
	}

	inline void CTabbedMDI::CloseAllMDIChildren()
	{
		while (GetMDIChildCount() > 0)
		{
			GetTab().RemoveTabPage(0);
		}
	}

	inline void CTabbedMDI::CloseMDIChild(int nTab)
	{
		GetTab().RemoveTabPage(nTab);
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
		int nTab = GetTab().GetCurSel();
		TabPageInfo tbi = GetTab().GetTabPageInfo(nTab);
		return tbi.pWnd;
	}

	inline int CTabbedMDI::GetMDIChildCount()
	{ 
		return (int) GetTab().GetAllTabs().size(); 
	}

	inline BOOL CTabbedMDI::LoadRegistrySettings(tString tsRegistryKeyName)
	{
		BOOL bResult = FALSE;

		if (0 != tsRegistryKeyName.size())
		{
			tString tsKey = _T("Software\\") + tsRegistryKeyName + _T("\\MDI Children");
			HKEY hKey = 0;
			RegOpenKeyEx(HKEY_CURRENT_USER, tsKey.c_str(), 0, KEY_READ, &hKey);
			if (hKey)
			{
				DWORD dwType = REG_BINARY;
				DWORD BufferSize = sizeof(TabPageInfo);
				TabPageInfo tbi = {0};
				int i = 0;
				TCHAR szNumber[16];
				tString tsSubKey = _T("MDI Child ");
				tsSubKey += _itot(i, szNumber, 10);

				// Fill the DockList vector from the registry
				while (0 == RegQueryValueEx(hKey, tsSubKey.c_str(), NULL, &dwType, (LPBYTE)&tbi, &BufferSize))
				{
					CWnd* pWnd = NewMDIChildFromID(tbi.nID);
					if (pWnd)
					{
						AddMDIChild(pWnd, tbi.szTitle, tbi.nID);
						i++;
						tsSubKey = _T("MDI Child ");
						tsSubKey += _itot(i, szNumber, 10);
						bResult = TRUE;
					}
					else
					{
						TRACE(_T("Failed to get TabbedMDI info from registry"));
						bResult = FALSE;
						break;
					}
				}

				RegCloseKey(hKey);
				SetActiveMDITab(0);
			}
		}

		if (!bResult) CloseAllMDIChildren();
		return bResult;
	}

	inline CWnd* CTabbedMDI::NewMDIChildFromID(int /*nID*/)
	{
		// Override this function to create new MDI children from IDs as shown below
		CWnd* pView = NULL;
	/*	switch(nID)
		{
		case ID_SIMPLE:
			pView = new CViewSimple;
			break;
		case ID_RECT:
			pView = new CViewRect;
			break;
		default:
			TRACE(_T("Unknown MDI child ID\n"));
			break;
		} */

		return pView;
	}

	inline void CTabbedMDI::OnDestroy(WPARAM /*wParam*/, LPARAM /*lParam*/ )
	{
		CloseAllMDIChildren();
	}

	inline LRESULT CTabbedMDI::OnEraseBkGnd(WPARAM wParam, LPARAM lParam)
	{
		if (GetTab().GetItemCount() >0)
			return 0;
	
		return CWnd::WndProcDefault(WM_ERASEBKGND, wParam, lParam);
	}

	inline LRESULT CTabbedMDI::OnNotify(WPARAM /*wParam*/, LPARAM lParam)
	{
		LPNMHDR pnmhdr = (LPNMHDR)lParam;
		if (pnmhdr->code == UWM_TAB_CHANGED)
			RecalcLayout();

		return 0L;
	}

	inline void CTabbedMDI::OnWindowPosChanged(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		RecalcLayout();
	}

	inline void CTabbedMDI::RecalcLayout()
	{
		if (GetTab().GetItemCount() >0)
		{
			CRect rc = GetClientRect();
			GetTab().SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
		}
		else
		{
			GetTab().ShowWindow(SW_HIDE);
		}
	}

	inline void CTabbedMDI::SaveRegistrySettings(tString tsRegistryKeyName)
	{
		if (0 != tsRegistryKeyName.size())
		{
			tString tsKeyName = _T("Software\\") + tsRegistryKeyName;
			HKEY hKey = NULL;
			HKEY hKeyMDIChild = NULL;
			if (RegCreateKeyEx(HKEY_CURRENT_USER, tsKeyName.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL))
				throw (CWinException(_T("RegCreateKeyEx Failed")));

			RegDeleteKey(hKey, _T("MDI Children"));
			if (RegCreateKeyEx(hKey, _T("MDI Children"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKeyMDIChild, NULL))
				throw (CWinException(_T("RegCreateKeyEx Failed")));

			for (int i = 0; i < GetMDIChildCount(); ++i)
			{
				TCHAR szNumber[16];
				tString tsSubKey = _T("MDI Child ");
				tsSubKey += _itot(i, szNumber, 10);
				TabPageInfo pdi = GetTab().GetTabPageInfo(i);
				RegSetValueEx(hKeyMDIChild, tsSubKey.c_str(), 0, REG_BINARY, (LPBYTE)&pdi, sizeof(TabPageInfo));
			}

			RegCloseKey(hKeyMDIChild);
			RegCloseKey(hKey);
		}
	}

	inline void CTabbedMDI::SetActiveMDIChild(CWnd* pWnd)
	{
		int iPage = GetTab().GetTabIndex(pWnd);
		if (iPage >= 0)
			GetTab().SelectPage(iPage);
	}

	inline void CTabbedMDI::SetActiveMDITab(int iTab)
	{
		GetTab().SelectPage(iTab);
	}

	inline LRESULT CTabbedMDI::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch(uMsg)
		{
		case WM_DESTROY:
			OnDestroy(wParam, lParam);
			break;
		case WM_ERASEBKGND:
			return OnEraseBkGnd(wParam, lParam);
		
		case WM_WINDOWPOSCHANGED:
			OnWindowPosChanged(wParam, lParam);
			break;
		} 

		return CWnd::WndProcDefault(uMsg, wParam, lParam);
	}

} // namespace Win32xx

#endif  // _TAB_H_
