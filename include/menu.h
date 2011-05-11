// Win32++   Pre-release Version 7.2
// Released: N/A
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2011  David Nash
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


////////////////////////////////////////////////////////
// menu.h
//  Declaration of the CMenu class


#ifndef _WIN32XX_MENU_H_
#define _WIN32XX_MENU_H_


#include "wincore.h"


namespace Win32xx
{

	// Forward declarations
	class CBitmap;

	class CMenu
	{
	public:
		//Construction
		CMenu() : m_hMenu(0) {}
		virtual ~CMenu();

		//Initialization
		void Attach(HMENU hMenu);
		void CreateMenu();
		void CreatePopupMenu();
	//	static void PASCAL DeleteTempMap();	???
		void DestroyMenu();
		HMENU Detach();
		static CMenu* FromHandle(HMENU hMenu);
		HMENU GetHmenu() const;
		BOOL LoadMenu(LPCTSTR lpszResourceName);
		BOOL LoadMenu(UINT uIDResource);
		BOOL LoadMenuIndirect(const void* lpMenuTemplate);

		//Menu Operations
		BOOL DeleteMenu(UINT uPosition, UINT uFlags);
		BOOL TrackPopupMenu(UINT uFlags, int x, int y, CWnd* pWnd, LPCRECT lpRect = 0);
		BOOL TrackPopupMenuEx(UINT uFlags, int x, int y, CWnd* pWnd, LPTPMPARAMS lptpm);

		//Menu Item Operations
		BOOL AppendMenu(UINT uFlags, UINT_PTR uIDNewItem = 0, LPCTSTR lpszNewItem = NULL);
		BOOL AppendMenu(UINT nFlags, UINT_PTR uIDNewItem, const CBitmap* pBmp);
		UINT CheckMenuItem(UINT uIDCheckItem, UINT uCheck);
		BOOL CheckMenuRadioItem(UINT uIDFirst, UINT uIDLast, UINT uIDItem, UINT uFlags);
		UINT EnableMenuItem(UINT uIDEnableItem, UINT uEnable);
		UINT GetDefaultItem(UINT gmdiFlags, BOOL fByPos = FALSE);
		DWORD GetMenuContextHelpId() const;

#if(WINVER >= 0x0500)	// Minimum OS required is Win2000
		BOOL GetMenuInfo(LPMENUINFO lpcmi) const;
		BOOL SetMenuInfo(LPCMENUINFO lpcmi);
#endif

		UINT GetMenuItemCount() const;
		UINT GetMenuItemID(int nPos) const;
		BOOL GetMenuItemInfo(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos = FALSE);
		UINT GetMenuState(UINT uID, UINT uFlags) const;
		int GetMenuString(UINT uIDItem, LPTSTR lpString, int nMaxCount, UINT uFlags) const;
		int GetMenuString(UINT uIDItem, CString& rString, UINT uFlags) const;
		CMenu* GetSubMenu(int nPos) const;
		BOOL InsertMenu(UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem = 0, LPCTSTR lpszNewItem = NULL);
		BOOL InsertMenu(UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, const CBitmap* pBmp);
		BOOL InsertMenuItem(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos = FALSE);
		BOOL ModifyMenu(UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem = 0, LPCTSTR lpszNewItem = NULL);
		BOOL ModifyMenu(UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, const CBitmap* pBmp);
		BOOL RemoveMenu(UINT uPosition, UINT uFlags);
		BOOL SetDefaultItem(UINT uItem, BOOL fByPos = FALSE);
		BOOL SetMenuContextHelpId(DWORD dwContextHelpId);
		BOOL SetMenuItemBitmaps(UINT uPosition, UINT uFlags, const CBitmap* pBmpUnchecked, const CBitmap* pBmpChecked);
		BOOL SetMenuItemInfo(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos = FALSE);

		//Overridables
		virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
		virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

		//Operators
		void operator = (const HMENU hMenu);
		BOOL operator != (const CMenu& menu) const;
		BOOL operator == (const CMenu& menu) const;
		operator HMENU () const;

	private:
		CMenu(const CMenu&);				// Disable copy construction
		CMenu& operator = (const CMenu&);	// Disable assignment operator
		void AddToMap();
		BOOL RemoveFromMap();
		HMENU m_hMenu;

	};

	inline CMenu::~CMenu()
	{
		if (m_hMenu)
		{
			RemoveFromMap();
			::DestroyMenu(m_hMenu);
		}
	}

	inline void CMenu::AddToMap()
	// Store the HDC and CDC pointer in the HDC map
	{
		assert( GetApp() );
		assert(m_hMenu);
		GetApp()->m_csMapLock.Lock();

		assert(!GetApp()->GetCMenuFromMap(m_hMenu));

		GetApp()->m_mapHMENU.insert(std::make_pair(m_hMenu, this));
		GetApp()->m_csMapLock.Release();
	}

	inline BOOL CMenu::RemoveFromMap()
	{
		assert( GetApp() );
		BOOL Success = FALSE;

		// Allocate an iterator for our HDC map
		std::map<HMENU, CMenu*, CompareHMENU>::iterator m;

		CWinApp* pApp = GetApp();
		if (pApp)
		{
			// Erase the CDC pointer entry from the map
			pApp->m_csMapLock.Lock();
			for (m = pApp->m_mapHMENU.begin(); m != pApp->m_mapHMENU.end(); ++m)
			{
				if (this == m->second)
				{
					pApp->m_mapHMENU.erase(m);
					Success = TRUE;
					break;
				}
			}

			pApp->m_csMapLock.Release();
		}

		return Success;
	}

	inline void CMenu::Attach(HMENU hMenu)
	{
		assert(0 == m_hMenu);
		assert(IsMenu(hMenu));
		m_hMenu = hMenu;
		AddToMap();
	}

	inline void CMenu::CreateMenu()
	{
		assert(NULL == m_hMenu);
		m_hMenu = ::CreateMenu();
		AddToMap();
	}

	inline void CMenu::CreatePopupMenu()
	{
		assert(NULL == m_hMenu);
		m_hMenu = ::CreatePopupMenu();
		AddToMap();
	}

	//inline static void PASCAL CMenu::DeleteTempMap();	???

	inline void CMenu::DestroyMenu()
	{
		assert(IsMenu(m_hMenu));
		::DestroyMenu(m_hMenu);
		m_hMenu = 0;
		RemoveFromMap();
	}

	inline HMENU CMenu::Detach()
	{
		assert(IsMenu(m_hMenu));
		HMENU hMenu = m_hMenu;
		m_hMenu = 0;
		RemoveFromMap();
		return hMenu;
	}

	inline CMenu* CMenu::FromHandle(HMENU hMenu)
	// Returns the CMenu object associated with the menu handle (HMENU)
	{
		assert( GetApp() );
		CMenu* pMenu = GetApp()->GetCMenuFromMap(hMenu);
		return pMenu;
	}

	inline HMENU CMenu::GetHmenu() const
	{
		return m_hMenu;
	}

	inline BOOL CMenu::LoadMenu(LPCTSTR lpszResourceName)
	{
		assert(NULL == m_hMenu);
		assert(lpszResourceName);
		m_hMenu = ::LoadMenu(GetApp()->GetResourceHandle(), lpszResourceName);
		if (m_hMenu) AddToMap();
		return NULL != m_hMenu;
	}

	inline BOOL CMenu::LoadMenu(UINT uIDResource)
	{
		assert(NULL == m_hMenu);
		m_hMenu = ::LoadMenu(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(uIDResource));
		if (m_hMenu) AddToMap();
		return NULL != m_hMenu;
	}

	inline BOOL CMenu::LoadMenuIndirect(const void* lpMenuTemplate)
	{
		assert(NULL == m_hMenu);
		assert(lpMenuTemplate);
		m_hMenu = ::LoadMenuIndirect(lpMenuTemplate);
		if (m_hMenu) AddToMap();
		return NULL != m_hMenu;
	}

	inline BOOL CMenu::DeleteMenu(UINT uPosition, UINT uFlags)
	{
		assert(IsMenu(m_hMenu));
		return ::DeleteMenu(m_hMenu, uPosition, uFlags);
	}

	inline BOOL CMenu::TrackPopupMenu(UINT uFlags, int x, int y, CWnd* pWnd, LPCRECT lpRect /*= 0*/)
	{
		assert(IsMenu(m_hMenu));
		HWND hWnd = pWnd? pWnd->GetHwnd() : 0;
		return ::TrackPopupMenu(m_hMenu, uFlags, x, y, 0, hWnd, lpRect);
	}

	inline BOOL CMenu::TrackPopupMenuEx(UINT uFlags, int x, int y, CWnd* pWnd, LPTPMPARAMS lptpm)
	{
		assert(IsMenu(m_hMenu));
		HWND hWnd = pWnd? pWnd->GetHwnd() : 0;
		return ::TrackPopupMenuEx(m_hMenu, uFlags, x, y, hWnd, lptpm);
	}


	inline BOOL CMenu::AppendMenu(UINT uFlags, UINT_PTR uIDNewItem /*= 0*/, LPCTSTR lpszNewItem /*= NULL*/)
	{
		assert(IsMenu(m_hMenu));
		return ::AppendMenu(m_hMenu, uFlags, uIDNewItem, lpszNewItem);
	}

	inline BOOL CMenu::AppendMenu(UINT uFlags, UINT_PTR uIDNewItem, const CBitmap* pBmp)
	{
		assert(IsMenu(m_hMenu));
		assert(pBmp);
		return ::AppendMenu(m_hMenu, uFlags, uIDNewItem, (LPCTSTR)pBmp->GetBitmap());
	}

	inline UINT CMenu::CheckMenuItem(UINT uIDCheckItem, UINT uCheck)
	{
		assert(IsMenu(m_hMenu));
		return ::CheckMenuItem(m_hMenu, uIDCheckItem, uCheck);
	}

	inline BOOL CMenu::CheckMenuRadioItem(UINT uIDFirst, UINT uIDLast, UINT uIDItem, UINT uFlags)
	{
		assert(IsMenu(m_hMenu));
		return ::CheckMenuRadioItem(m_hMenu, uIDFirst, uIDLast, uIDItem, uFlags);
	}

	inline UINT CMenu::EnableMenuItem(UINT uIDEnableItem, UINT uEnable)
	{
		assert(IsMenu(m_hMenu));
		return ::EnableMenuItem(m_hMenu, uIDEnableItem, uEnable);
	}

	inline UINT CMenu::GetDefaultItem(UINT gmdiFlags, BOOL fByPos /*= FALSE*/)
	{
		assert(IsMenu(m_hMenu));
		return ::GetMenuDefaultItem(m_hMenu, fByPos, gmdiFlags);
	}

	inline DWORD CMenu::GetMenuContextHelpId() const
	{
		assert(IsMenu(m_hMenu));
		return ::GetMenuContextHelpId(m_hMenu);
	}

#if(WINVER >= 0x0500)
// minimum OS required : Win2000

	inline BOOL CMenu::GetMenuInfo(LPMENUINFO lpcmi) const
	{
		assert(IsMenu(m_hMenu));
		return ::GetMenuInfo(m_hMenu, lpcmi);
	}

	inline BOOL CMenu::SetMenuInfo(LPCMENUINFO lpcmi)
	{
		assert(IsMenu(m_hMenu));
		return ::SetMenuInfo(m_hMenu, lpcmi);
	}

#endif

	inline UINT CMenu::GetMenuItemCount() const
	{
		assert(IsMenu(m_hMenu));
		return ::GetMenuItemCount(m_hMenu);
	}

	inline UINT CMenu::GetMenuItemID(int nPos) const
	{
		assert(IsMenu(m_hMenu));
		return ::GetMenuItemID(m_hMenu, nPos);
	}

	inline BOOL CMenu::GetMenuItemInfo(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos /*= FALSE*/)
	{
		assert(IsMenu(m_hMenu));
		assert(lpMenuItemInfo);
		return ::GetMenuItemInfo(m_hMenu, uItem, fByPos, lpMenuItemInfo);
	}

	inline UINT CMenu::GetMenuState(UINT uID, UINT uFlags) const
	{
		assert(IsMenu(m_hMenu));
		return ::GetMenuState(m_hMenu, uID, uFlags);
	}

	inline int CMenu::GetMenuString(UINT uIDItem, LPTSTR lpString, int nMaxCount, UINT uFlags) const
	{
		assert(IsMenu(m_hMenu));
		assert(lpString);
		return ::GetMenuString(m_hMenu, uIDItem, lpString, nMaxCount, uFlags);
	}

	inline int CMenu::GetMenuString(UINT uIDItem, CString& rString, UINT uFlags) const
	{
		assert(IsMenu(m_hMenu));
		return ::GetMenuString(m_hMenu, uIDItem, (LPTSTR)rString.c_str(), rString.GetLength(), uFlags);
	}

//	inline HMENU CMenu::GetSubMenu(int nPos) const
//	{
//		assert(IsMenu(m_hMenu));
//		return ::GetSubMenu(m_hMenu);
//	}

	inline BOOL CMenu::InsertMenu(UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem /*= 0*/, LPCTSTR lpszNewItem /*= NULL*/)
	{
		assert(IsMenu(m_hMenu));
		return ::InsertMenu(m_hMenu, uPosition, uFlags, uIDNewItem, lpszNewItem);
	}

	inline BOOL CMenu::InsertMenu(UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, const CBitmap* pBmp)
	{
		assert(IsMenu(m_hMenu));
		return ::InsertMenu(m_hMenu, uPosition, uFlags, uIDNewItem, (LPCTSTR)pBmp->GetBitmap());
	}

	inline BOOL CMenu::InsertMenuItem(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos /*= FALSE*/)
	{
		assert(IsMenu(m_hMenu));
		assert(lpMenuItemInfo);
		return ::InsertMenuItem(m_hMenu, uItem, fByPos, lpMenuItemInfo);
	}

	inline BOOL CMenu::ModifyMenu(UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem /*= 0*/, LPCTSTR lpszNewItem /*= NULL*/)
	{
		assert(IsMenu(m_hMenu));
		return ::ModifyMenu(m_hMenu, uPosition, uFlags, uIDNewItem, lpszNewItem);
	}

	inline BOOL CMenu::ModifyMenu(UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, const CBitmap* pBmp)
	{
		assert(IsMenu(m_hMenu));
		assert(pBmp);
		return ::ModifyMenu(m_hMenu, uPosition, uFlags, uIDNewItem, (LPCTSTR)pBmp->GetBitmap());
	}

	inline BOOL CMenu::RemoveMenu(UINT uPosition, UINT uFlags)
	{
		assert(IsMenu(m_hMenu));
		return ::RemoveMenu(m_hMenu, uPosition, uFlags);
	}

	inline BOOL CMenu::SetDefaultItem(UINT uItem, BOOL fByPos /*= FALSE*/)
	{
		assert(IsMenu(m_hMenu));
		return ::SetMenuDefaultItem(m_hMenu, uItem, fByPos);
	}

	inline BOOL CMenu::SetMenuContextHelpId(DWORD dwContextHelpId)
	{
		assert(IsMenu(m_hMenu));
		return ::SetMenuContextHelpId(m_hMenu, dwContextHelpId);
	}

	inline BOOL CMenu::SetMenuItemBitmaps(UINT uPosition, UINT uFlags, const CBitmap* pBmpUnchecked, const CBitmap* pBmpChecked)
	{
		assert(IsMenu(m_hMenu));
		assert(pBmpUnchecked);
		assert(pBmpChecked);
		return ::SetMenuItemBitmaps(m_hMenu, uPosition, uFlags, *pBmpUnchecked, *pBmpChecked);
	}

	inline BOOL CMenu::SetMenuItemInfo(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos /*= FALSE*/)
	{
		assert(IsMenu(m_hMenu));
		return ::SetMenuItemInfo(m_hMenu, uItem, fByPos, lpMenuItemInfo);
	}

	inline void CMenu::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		UNREFERENCED_PARAMETER(lpDrawItemStruct);
	}

	inline void CMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
	{
		UNREFERENCED_PARAMETER(lpMeasureItemStruct);
	}

	inline void CMenu::operator = (const HMENU hMenu)
	{
		Attach(hMenu);
	}

	inline BOOL CMenu::operator != (const CMenu& menu) const
	{
		return menu.m_hMenu != m_hMenu;
	}

	inline BOOL CMenu::operator == (const CMenu& menu) const
	{
		return menu.m_hMenu == m_hMenu;
	}

	inline CMenu::operator HMENU () const
	{
		return m_hMenu;
	}


}	// namespace Win32xx

#endif	// _WIN32XX_MENU_H_

