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
			std::vector<tString> m_vItems;
			int IDC_LIST;

		};
	public:
		CTab();
		virtual ~CTab();
		virtual int  AddTabPage(TabPageInfo& tbi);
		virtual int  AddTabPage(CWnd* pWnd, LPCTSTR szTitle, HICON hIcon);
		virtual int  AddTabPage(CWnd* pWnd, LPCTSTR szTitle, UINT nID_Icon);
		virtual BOOL GetTabsAtTop();
		virtual int  GetTabIndex(CWnd* pWnd);
		virtual TabPageInfo GetTabPageInfo(UINT nTab);
		virtual void SelectPage(int iPage);
		virtual void RecalcLayout();
		virtual void RemoveTabPage(int iPage);
		virtual void SetTabsAtTop(BOOL bTop);
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
		virtual SIZE    GetMaxTabSize();
		virtual void    OnCreate();
		virtual void    OnLButtonDown(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNCHitTest(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void    PreCreate(CREATESTRUCT& cs);
		virtual void    SetTabSize();
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
		virtual void ShowListMenu();

	protected:
		virtual HWND    Create(HWND hWndParent);
		virtual CWnd*   NewMDIChildFromID(int nID);
		virtual void    OnDestroy(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnEraseBkGnd(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual void    OnWindowPosChanged(WPARAM wParam, LPARAM lParam);
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		CTab m_Tab;
	};
	
}


#endif  // _TAB_H_
