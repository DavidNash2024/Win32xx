// Win32++   Version 7.8
// Release Date: 17th March 2015
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2015  David Nash
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

// Notes
//  1) Owner-drawn menus send the WM_MEASUREITEM and WM_DRAWITEM messages
//     to the window that owns the menu. To manage owner drawing for menus,
//     handle these two messages in the CWnd's WndProc function.
//
//  2) The CMenu pointer returned by FromHandle might be a temporary pointer. It
//     should be used immediately, not saved for later use.
//
//  3) The CMenu pointers returned by FromHandle or GetSubMenu do not need
//     to be deleted. They are automatically deleted by the Win32++.
//
//  4) CMenu pointers returned by GetSubMenu are deleted when the parent CMenu is
//     detached, destroyed or deconstructed.
//
//  5) The HMENU that is attached to a CMenu object (using the attach function) is 
//     automatically deleted when the CMenu object goes out of scope. Detach the
//     HMENU to stop it being deleted when CMenu's destructor is called.
//
//  6) Pass CMenu objects by reference or by pointer when passing them as function 
//     arguments.
//
//  7) In those functions that use a MENUITEMINFO structure, its cbSize member is 
//     automatically set to the correct value.

//  Program sample
//  --------------
//	void CView::CreatePopup()
//	{
// 		CPoint pt = GetCursorPos();
// 	
// 		// Create the menu
// 		CMenu Popup;
// 		Popup.CreatePopupMenu();
// 
// 		// Add some menu items
// 		Popup.AppendMenu(MF_STRING, 101, _T("Menu Item &1"));
// 		Popup.AppendMenu(MF_STRING, 102, _T("Menu Item &2"));
// 		Popup.AppendMenu(MF_STRING, 103, _T("Menu Item &3"));
// 		Popup.AppendMenu(MF_SEPARATOR);
// 		Popup.AppendMenu(MF_STRING, 104, _T("Menu Item &4"));
// 
// 		// Set menu item states
// 		Popup.CheckMenuRadioItem(101, 101, 101, MF_BYCOMMAND);
// 		Popup.CheckMenuItem(102, MF_BYCOMMAND | MF_CHECKED);
// 		Popup.EnableMenuItem(103, MF_BYCOMMAND | MF_GRAYED);
// 		Popup.SetDefaultItem(104);
//	
// 		// Display the popup menu
// 		Popup.TrackPopupMenu(0, pt.x, pt.y, this); 
//	}



#if !defined(_WIN32XX_MENU_H_) && !defined(_WIN32_WCE)
#define _WIN32XX_MENU_H_


#include "wincore.h"
#include "gdi.h"


namespace Win32xx
{

	// Forward declarations
	class CBitmap;

	class CMenu
	{
	
		struct DataMembers
		{
			std::vector<MenuPtr> vSubMenus;	// A vector of smart pointers to CMenu
			HMENU hMenu;
			BOOL IsTmpMenu;
			long Count;
		};			

	public:
		//Construction
		CMenu();
		CMenu(UINT nID);
		CMenu(const CMenu& rhs);
		CMenu& operator = (const CMenu& rhs);
		void operator = (const HMENU hImageList);
		~CMenu();

		//Initialization
		void Attach(HMENU hMenu);
		void CreateMenu();
		void CreatePopupMenu();
		void DestroyMenu();
		HMENU Detach();
		static CMenu* FromHandle(HMENU hMenu);
		HMENU GetHandle() const;
		BOOL LoadMenu(LPCTSTR lpszResourceName);
		BOOL LoadMenu(UINT uIDResource);
		BOOL LoadMenuIndirect(const void* lpMenuTemplate);

		//Menu Operations
		BOOL TrackPopupMenu(UINT uFlags, int x, int y, CWnd* pWnd, LPCRECT lpRect = 0);
		BOOL TrackPopupMenuEx(UINT uFlags, int x, int y, CWnd* pWnd, LPTPMPARAMS lptpm);

		//Menu Item Operations
		BOOL AppendMenu(UINT uFlags, UINT_PTR uIDNewItem = 0, LPCTSTR lpszNewItem = NULL);
		BOOL AppendMenu(UINT uFlags, UINT_PTR uIDNewItem, const CBitmap* pBmp);
		UINT CheckMenuItem(UINT uIDCheckItem, UINT uCheck);
		BOOL CheckMenuRadioItem(UINT uIDFirst, UINT uIDLast, UINT uIDItem, UINT uFlags);
		BOOL DeleteMenu(UINT uPosition, UINT uFlags);
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
		CMenu* GetSubMenu(int nPos);
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

		//Operators
		operator HMENU () const;

	private:
		void AddToMap();
		void Release();
		BOOL RemoveFromMap();
		DataMembers* m_pData;
	};

} // namespace Win32xx

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace Win32xx
{

	inline CMenu::CMenu()
	{
		m_pData = new DataMembers;
		m_pData->hMenu = 0;
		m_pData->Count = 1L;
		m_pData->IsTmpMenu = FALSE;	
	}

	inline CMenu::CMenu(UINT nID) 
	{
		m_pData = new DataMembers;
		m_pData->hMenu = 0;
		m_pData->Count = 1L;
		m_pData->IsTmpMenu = FALSE;

		HMENU menu = ::LoadMenu(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(nID));
		Attach(menu);
	}

	inline CMenu::CMenu(const CMenu& rhs)
	// Note: A copy of a CMenu is a clone of the original.
	//       Both objects manipulate the one HMENU.
	{
		m_pData = rhs.m_pData;
		InterlockedIncrement(&m_pData->Count);
	}

	inline CMenu& CMenu::operator = (const CMenu& rhs)
	// Note: A copy of a CImageList is a clone of the original.
	{
		if (this != &rhs)
		{
			InterlockedIncrement(&rhs.m_pData->Count);
			Release();
			m_pData = rhs.m_pData;
		}

		return *this;
	}

	inline void CMenu::operator = (const HMENU hMenu)
	{
		Attach(hMenu);
	}

	inline CMenu::~CMenu()
	{
		Release();
	}

	inline void CMenu::AddToMap()
	// Store the HMENU and CMenu pointer in the HMENU map
	{
		assert( GetApp() );
		assert(m_pData);
		assert(m_pData->hMenu);
		
		GetApp()->m_csMapLock.Lock();
		GetApp()->m_mapHMENU.insert(std::make_pair(m_pData->hMenu, this));
		GetApp()->m_csMapLock.Release();
	}

	inline CMenu* CMenu::FromHandle(HMENU hMenu)
	// Returns the CMenu object associated with the menu handle (HMENU).
	{
		assert( GetApp() );

		// Find an existing permanent CMenu from the map
		CMenu* pMenu = GetApp()->GetCMenuFromMap(hMenu);
		if ((0 != hMenu) && (0 == pMenu))
		{		
			// Find any existing temporary CMenu for the HMENU
			TLSData* pTLSData = GetApp()->SetTlsData();
			std::map<HMENU, MenuPtr, CompareHMENU>::iterator m;
			m = pTLSData->TmpMenus.find(hMenu);
	
			if (m != pTLSData->TmpMenus.end())
				pMenu = m->second.get();

			if (!pMenu)
			{
				pMenu = new CMenu;
				pMenu->m_pData->hMenu = hMenu;
				pMenu->m_pData->IsTmpMenu = TRUE;
				pTLSData->TmpMenus.insert(std::make_pair(hMenu, pMenu));

				::PostMessage(0, UWM_CLEANUPTEMPS, 0, 0);
			}
		}
		return pMenu;
	}

	inline void CMenu::Release()
	{
		assert(m_pData);

		if (InterlockedDecrement(&m_pData->Count) == 0)
		{
			if (m_pData->hMenu != NULL)
			{
				if (!m_pData->IsTmpMenu)
				{
					::DestroyMenu(m_pData->hMenu);
					RemoveFromMap();
				}
			}

			delete m_pData;
			m_pData = 0;
		}
	}

	inline BOOL CMenu::RemoveFromMap()
	{
		BOOL Success = FALSE;

		if (GetApp())
		{
			// Allocate an iterator for our HMENU map
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
		}

		return Success;
	}
	
	
	inline BOOL CMenu::AppendMenu(UINT uFlags, UINT_PTR uIDNewItem /*= 0*/, LPCTSTR lpszNewItem /*= NULL*/)
	// Appends a new item to the end of the specified menu bar, drop-down menu, submenu, or shortcut menu.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		return ::AppendMenu(m_pData->hMenu, uFlags, uIDNewItem, lpszNewItem);
	}

	inline BOOL CMenu::AppendMenu(UINT uFlags, UINT_PTR uIDNewItem, const CBitmap* pBmp)
	// Appends a new item to the end of the specified menu bar, drop-down menu, submenu, or shortcut menu.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		assert(pBmp);
		return ::AppendMenu(m_pData->hMenu, uFlags, uIDNewItem, (LPCTSTR)pBmp->GetHandle());
	}

	inline void CMenu::Attach(HMENU hMenu)
	// Attaches an existing menu to this CMenu
	{
		assert(m_pData);
		assert(IsMenu(hMenu));
		assert( 0 == m_pData->hMenu );

		CMenu* pMenu = GetApp()->GetCMenuFromMap(hMenu);
		if (pMenu)
		{
			delete m_pData;
			m_pData = pMenu->m_pData;
			InterlockedIncrement(&m_pData->Count);
		}
		else
		{
			m_pData->hMenu = hMenu;
			AddToMap();
		}

		m_pData->hMenu = hMenu;
		AddToMap();
	}
	
	inline UINT CMenu::CheckMenuItem(UINT uIDCheckItem, UINT uCheck)
	// Sets the state of the specified menu item's check-mark attribute to either selected or clear.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		return ::CheckMenuItem(m_pData->hMenu, uIDCheckItem, uCheck);
	}

	inline BOOL CMenu::CheckMenuRadioItem(UINT uIDFirst, UINT uIDLast, UINT uIDItem, UINT uFlags)
	// Checks a specified menu item and makes it a radio item. At the same time, the function clears 
	//  all other menu items in the associated group and clears the radio-item type flag for those items.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		return ::CheckMenuRadioItem(m_pData->hMenu, uIDFirst, uIDLast, uIDItem, uFlags);
	}	

	inline void CMenu::CreateMenu()
	// Creates an empty menu.
	{
		assert(m_pData);
		assert(NULL == m_pData->hMenu);
		m_pData->hMenu = ::CreateMenu();
		AddToMap();
	}

	inline void CMenu::CreatePopupMenu()
	// Creates a drop-down menu, submenu, or shortcut menu. The menu is initially empty.
	{
		assert(m_pData);
		assert(NULL == m_pData->hMenu);
		m_pData->hMenu = ::CreatePopupMenu();
		AddToMap();
	}
	
	inline BOOL CMenu::DeleteMenu(UINT uPosition, UINT uFlags)
	// Deletes an item from the specified menu.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		return ::DeleteMenu(m_pData->hMenu, uPosition, uFlags);
	}	

	inline void CMenu::DestroyMenu()
	// Destroys the menu and frees any memory that the menu occupies.
	{
		assert(m_pData);
		if (::IsMenu(m_pData->hMenu)) 
			::DestroyMenu(m_pData->hMenu);
		
		m_pData->hMenu = 0;
		m_pData->vSubMenus.clear();
		RemoveFromMap();
	}

	inline HMENU CMenu::Detach()
	// Detaches the HMENU from this CMenu. If the HMENU is not detached it will be 
	// destroyed when this CMenu is deconstructed.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		HMENU hMenu = m_pData->hMenu;
		
		if (m_pData->Count > 0)
		{
			if (InterlockedDecrement(&m_pData->Count) == 0)
			{
				RemoveFromMap();
				delete m_pData;
			}
		}		
		
		m_pData = new DataMembers;
		m_pData->hMenu = 0;
		m_pData->Count = 1L;
		m_pData->IsTmpMenu = FALSE;	

		return hMenu;
	}

	inline HMENU CMenu::GetHandle() const
	// Returns the HMENU assigned to this CMenu
	{
		assert(m_pData);
		return m_pData->hMenu;
	}

	inline UINT CMenu::EnableMenuItem(UINT uIDEnableItem, UINT uEnable)
	// Enables, disables, or grays the specified menu item.
	// The uEnable parameter must be a combination of either MF_BYCOMMAND or MF_BYPOSITION
	// and MF_ENABLED, MF_DISABLED, or MF_GRAYED.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		return ::EnableMenuItem(m_pData->hMenu, uIDEnableItem, uEnable);
	}
	
	inline UINT CMenu::GetDefaultItem(UINT gmdiFlags, BOOL fByPos /*= FALSE*/)
	// Determines the default menu item.
	// The gmdiFlags parameter specifies how the function searches for menu items. 
	// This parameter can be zero or more of the following values: GMDI_GOINTOPOPUPS; GMDI_USEDISABLED.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		return ::GetMenuDefaultItem(m_pData->hMenu, fByPos, gmdiFlags);
	}

	inline DWORD CMenu::GetMenuContextHelpId() const
	// Retrieves the Help context identifier associated with the menu.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		return ::GetMenuContextHelpId(m_pData->hMenu);
	}

#if(WINVER >= 0x0500)
// minimum OS required : Win2000

	inline BOOL CMenu::GetMenuInfo(LPMENUINFO lpcmi) const
	// Retrieves the menu information.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		return ::GetMenuInfo(m_pData->hMenu, lpcmi);
	}

	inline BOOL CMenu::SetMenuInfo(LPCMENUINFO lpcmi)
	// Sets the menu information from the specified MENUINFO structure.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		return ::SetMenuInfo(m_pData->hMenu, lpcmi);
	}

#endif

	inline UINT CMenu::GetMenuItemCount() const
	// Retrieves the number of menu items.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		return ::GetMenuItemCount(m_pData->hMenu);
	}

	inline UINT CMenu::GetMenuItemID(int nPos) const
	// Retrieves the menu item identifier of a menu item located at the specified position
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		return ::GetMenuItemID(m_pData->hMenu, nPos);
	}

	inline BOOL CMenu::GetMenuItemInfo(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos /*= FALSE*/)
	// retrieves information about the specified menu item.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		assert(lpMenuItemInfo);
		lpMenuItemInfo->cbSize = GetSizeofMenuItemInfo();
		return ::GetMenuItemInfo(m_pData->hMenu, uItem, fByPos, lpMenuItemInfo);
	}

	inline UINT CMenu::GetMenuState(UINT uID, UINT uFlags) const
	// Retrieves the menu flags associated with the specified menu item.
	// Possible values for uFlags are: MF_BYCOMMAND (default) or MF_BYPOSITION.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		return ::GetMenuState(m_pData->hMenu, uID, uFlags);
	}

	inline int CMenu::GetMenuString(UINT uIDItem, LPTSTR lpString, int nMaxCount, UINT uFlags) const
	// Copies the text string of the specified menu item into the specified buffer.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		assert(lpString);
		return ::GetMenuString(m_pData->hMenu, uIDItem, lpString, nMaxCount, uFlags);
	}

	inline int CMenu::GetMenuString(UINT uIDItem, CString& rString, UINT uFlags) const
	// Copies the text string of the specified menu item into the specified buffer.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		int n = ::GetMenuString(m_pData->hMenu, uIDItem, rString.GetBuffer(MAX_MENU_STRING), MAX_MENU_STRING, uFlags);
		rString.ReleaseBuffer();
		return n;
	}

	inline CMenu* CMenu::GetSubMenu(int nPos)
	// Retrieves the CMenu object of a pop-up menu.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		CMenu* pMenu = new CMenu;
		pMenu->m_pData->hMenu = ::GetSubMenu(m_pData->hMenu, nPos);
		pMenu->m_pData->IsTmpMenu = TRUE;
		m_pData->vSubMenus.push_back(pMenu);
		return pMenu;
	}

	inline BOOL CMenu::InsertMenu(UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem /*= 0*/, LPCTSTR lpszNewItem /*= NULL*/)
	// Inserts a new menu item into a menu, moving other items down the menu.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		return ::InsertMenu(m_pData->hMenu, uPosition, uFlags, uIDNewItem, lpszNewItem);
	}

	inline BOOL CMenu::InsertMenu(UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, const CBitmap* pBmp)
	// Inserts a new menu item into a menu, moving other items down the menu.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		return ::InsertMenu(m_pData->hMenu, uPosition, uFlags, uIDNewItem, (LPCTSTR)pBmp->GetHandle());
	}

	inline BOOL CMenu::InsertMenuItem(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos /*= FALSE*/)
	// Inserts a new menu item at the specified position in a menu.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		assert(lpMenuItemInfo);
		lpMenuItemInfo->cbSize = GetSizeofMenuItemInfo();
		return ::InsertMenuItem(m_pData->hMenu, uItem, fByPos, lpMenuItemInfo);
	}
	
	inline BOOL CMenu::LoadMenu(LPCTSTR lpszResourceName)
	// Loads the menu from the specified windows resource.
	{
		assert(m_pData);
		assert(NULL == m_pData->hMenu);
		assert(lpszResourceName);
		m_pData->hMenu = ::LoadMenu(GetApp()->GetResourceHandle(), lpszResourceName);
		if (m_pData->hMenu != 0) AddToMap();
		return NULL != m_pData->hMenu;
	}

	inline BOOL CMenu::LoadMenu(UINT uIDResource)
	// Loads the menu from the specified windows resource.
	{
		assert(m_pData);
		assert(NULL == m_pData->hMenu);
		m_pData->hMenu = ::LoadMenu(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(uIDResource));
		if (m_pData->hMenu != 0) AddToMap();
		return NULL != m_pData->hMenu;
	}

	inline BOOL CMenu::LoadMenuIndirect(const void* lpMenuTemplate)
	// Loads the specified menu template and assigns it to this CMenu.
	{
		assert(m_pData);
		assert(NULL == m_pData->hMenu);
		assert(lpMenuTemplate);
		m_pData->hMenu = ::LoadMenuIndirect(lpMenuTemplate);
		if (m_pData->hMenu) AddToMap();
		return NULL != m_pData->hMenu;
	}	

	inline BOOL CMenu::ModifyMenu(UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem /*= 0*/, LPCTSTR lpszNewItem /*= NULL*/)
	// Changes an existing menu item. This function is used to specify the content, appearance, and behavior of the menu item.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		return ::ModifyMenu(m_pData->hMenu, uPosition, uFlags, uIDNewItem, lpszNewItem);
	}

	inline BOOL CMenu::ModifyMenu(UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, const CBitmap* pBmp)
	// Changes an existing menu item. This function is used to specify the content, appearance, and behavior of the menu item.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		assert(pBmp);
		return ::ModifyMenu(m_pData->hMenu, uPosition, uFlags, uIDNewItem, (LPCTSTR)pBmp->GetHandle());
	}

	inline BOOL CMenu::RemoveMenu(UINT uPosition, UINT uFlags)
	// Deletes a menu item or detaches a submenu from the menu.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		return ::RemoveMenu(m_pData->hMenu, uPosition, uFlags);
	}

	inline BOOL CMenu::SetDefaultItem(UINT uItem, BOOL fByPos /*= FALSE*/)
	//  sets the default menu item for the menu.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		return ::SetMenuDefaultItem(m_pData->hMenu, uItem, fByPos);
	}

	inline BOOL CMenu::SetMenuContextHelpId(DWORD dwContextHelpId)
	// Associates a Help context identifier with the menu.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		return ::SetMenuContextHelpId(m_pData->hMenu, dwContextHelpId);
	}

	inline BOOL CMenu::SetMenuItemBitmaps(UINT uPosition, UINT uFlags, const CBitmap* pBmpUnchecked, const CBitmap* pBmpChecked)
	// Associates the specified bitmap with a menu item.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		return ::SetMenuItemBitmaps(m_pData->hMenu, uPosition, uFlags, *pBmpUnchecked, *pBmpChecked);
	}

	inline BOOL CMenu::SetMenuItemInfo(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos /*= FALSE*/)
	// Changes information about a menu item.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		assert(lpMenuItemInfo);
		lpMenuItemInfo->cbSize = GetSizeofMenuItemInfo();
		return ::SetMenuItemInfo(m_pData->hMenu, uItem, fByPos, lpMenuItemInfo);
	}
		
	inline BOOL CMenu::TrackPopupMenu(UINT uFlags, int x, int y, CWnd* pWnd, LPCRECT lpRect /*= 0*/)
	// Displays a shortcut menu at the specified location and tracks the selection of items on the menu.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		HWND hWnd = pWnd? pWnd->GetHwnd() : 0;
		return ::TrackPopupMenu(m_pData->hMenu, uFlags, x, y, 0, hWnd, lpRect);
	}

	inline BOOL CMenu::TrackPopupMenuEx(UINT uFlags, int x, int y, CWnd* pWnd, LPTPMPARAMS lptpm)
	// Displays a shortcut menu at the specified location and tracks the selection of items on the shortcut menu.
	{
		assert(m_pData);
		assert(IsMenu(m_pData->hMenu));
		HWND hWnd = pWnd? pWnd->GetHwnd() : 0;
		return ::TrackPopupMenuEx(m_pData->hMenu, uFlags, x, y, hWnd, lptpm);
	}

	inline CMenu::operator HMENU () const
	// Retrieves the menu's handle.
	{
		assert(m_pData);
		return m_pData->hMenu;
	}
	

}	// namespace Win32xx

#endif	// _WIN32XX_MENU_H_

