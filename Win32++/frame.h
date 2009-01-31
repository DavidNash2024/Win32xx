// Win32++  Version 6.4
// Released: 4th February, 2009 by:
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


////////////////////////////////////////////////////////
// frame.h
//  Declaration of the CFrame and CMenubar classes

// The classes declared in this file support SDI (Single Document Interface)
// frames on Win32/Win64 operating systems (not Windows CE). For Windows CE,
// use wceframe.h instead. SDI frames are a simple frame which supports a
// single view window. Refer to mdi.h for frames that support several
// child windows.

// CFrame also includes each of the following classes as members:
// * CRebar for managing the frame's rebar control.
// * CMenubar for managing the menu inside the rebar.
// * CToolbar for managing the frame's toolbar.
// * CStatusbar for managing the frame's status bar.
// In each case these members are exposed by a GetXXX function, allowing
// them to be accessed or sent messages.

// CFrame is responsible for creating a window which includes a menu and
// toolbar (usualy hosted within a rebar), and a status bar, and managing
// the position of these windows and the "view" window. The "view" window
// is a seperate CWnd object assigned to the frame with the SetView function.

// When compiling an application with these classes, it will need to be linked
// with Comctl32.lib.

// To create a SDI frame application, inherit a CMainFrame class from CFrame.
// Use the Frame generic application as the starting point for your own frame
// applications.
// Refer to the Notepad and Scribble samples for examples on how to use these
// classes to create a frame application.


#ifndef FRAME_H
#define FRAME_H

#include "wincore.h"
#include "dialog.h"
#include "gdi.h"
#include "statusbar.h"
#include "toolbar.h"
#include "rebar.h"
#include <shlwapi.h>
#include "Default_Resource.h"

#ifndef RBN_MINMAX
  #define RBN_MINMAX (RBN_FIRST - 21)
#endif



namespace Win32xx
{

	////////////////////////////////////////////////
	// Declarations for structures for themes
	//
	struct ThemeMenu
	{
		BOOL UseThemes;			// TRUE if themes are used
		COLORREF clrHot1;		// Colour 1 for hot button
		COLORREF clrHot2;		// Colour 2 for hot button
		COLORREF clrPressed1;	// Colour 1 for pressed button
		COLORREF clrPressed2;	// Colour 2 for pressed button
		COLORREF clrOutline;	// Colour for border outline
	};


	// Forward declaration of CFrame. Its defined later.
	class CFrame;


	////////////////////////////////////
	// Declaration of the CMenubar class
	//
	class CMenubar : public CToolbar
	{
	public:
		CMenubar();
		virtual ~CMenubar();
		void MenuChar(WPARAM wParam, LPARAM lParam);
		void SysCommand(WPARAM wParam, LPARAM lParam);
		HMENU GetMenu() const {return m_hTopMenu;}
		void SetMenu(HMENU hMenu);
		ThemeMenu& GetMenubarTheme() {return m_ThemeMenu;}
		void SetMenubarTheme(ThemeMenu& Theme);

	protected:
	//Overridables
		virtual void OnCreate();
		virtual LRESULT OnCustomDraw(NMHDR* pNMHDR);
		virtual void OnKeyDown(WPARAM wParam, LPARAM lParam);
		virtual void OnLButtonDown(WPARAM wParam, LPARAM lParam);
		virtual void OnLButtonUp(WPARAM wParam, LPARAM lParam);
		virtual void OnMouseLeave();
		virtual void OnMouseMove(WPARAM wParam, LPARAM lParam);
		virtual BOOL OnMenuInput(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void OnWindowPosChanged();
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		void DoAltKey(WORD KeyCode);
		void DoPopupMenu();
		void DrawAllMDIButtons(CDC& DrawDC);
		void DrawMDIButton(CDC& DrawDC, int iButton, UINT uState);
		void ExitMenu();
		HWND GetActiveMDIChild();
		void GrabFocus();
		BOOL IsMDIChildMaxed() const;
		BOOL IsMDIFrame() const;
		void ReleaseFocus();
		void SetHotItem(int nHot);
		static LRESULT CALLBACK StaticMsgHook(int nCode, WPARAM wParam, LPARAM lParam);

		enum MDIButtonType
		{
			MDI_MIN = 0,
			MDI_RESTORE = 1,
			MDI_CLOSE = 2,
		};

		BOOL  m_bExitAfter;		// Exit after Popup menu ends
		BOOL  m_bKeyMode;		// keyboard navigation mode
		BOOL  m_bMenuActive;	// popup menu active
		BOOL  m_bSelPopup;		// a popup (cascade) menu is selected
		HMENU m_hPopupMenu;		// handle to the popup menu
		HMENU m_hSelMenu;		// handle to the casceded popup menu
		HMENU m_hTopMenu;		// handle to the top level menu
		HWND  m_hPrevFocus;		// handle to window which had focus
		CRect m_MDIRect[3];		// array of CRect for MDI buttons
		int   m_nHotItem;		// hot item
		int   m_nMDIButton;		// the MDI button (MDIButtonType) pressed
		CPoint m_OldMousePos;	// old Mouse position
		ThemeMenu m_ThemeMenu;	// Theme structure
		CFrame* m_pFrame;       // Pointer to the frame.


	};  // class CMenubar

	//////////////////////////////////
	// Declaration of the CFrame class
	//
	class CFrame : public CWnd
	{
	public:
		CFrame();
		virtual ~CFrame();

		// These are the functions you might wish to override
		virtual BOOL AddMenuIcon(int nID_MenuItem, HICON hIcon, int cx = 16, int cy = 16);
		virtual int  AddMenuIcons(const std::vector<UINT>& MenuData, COLORREF crMask, UINT ToolbarID, UINT ToolbarDisabledID);
		virtual void AddToolbarButton(UINT nID);
		virtual void AdjustFrameRect(RECT rcView) const;
		virtual int  GetMenuItemPos(HMENU hMenu, LPCTSTR szItem);
		virtual CRect GetViewRect() const;
		virtual void LoadRegistryMRUSettings(UINT nMaxMRU = 0);
		virtual void OnFrameDrawItem(WPARAM wParam, LPARAM lParam);
		virtual void OnFrameExitMenuLoop();
		virtual void OnFrameInitMenuPopup(WPARAM wParam, LPARAM lParam);
		virtual void OnFrameMeasureItem(WPARAM wParam, LPARAM lParam);
		virtual int  SetMenuIcons(const std::vector<UINT>& MenuData, COLORREF crMask, UINT ToolbarID, UINT ToolbarDisabledID);
		virtual void SetStatusIndicators();
		virtual void SetStatusText();
		virtual void SetTheme();
		virtual void SetToolbarImages(CToolbar& TB, int iNumButtons, COLORREF crMask, UINT ToolbarID, UINT ToolbarHotID, UINT ToolbarDisabledID);
		virtual void RecalcLayout();
		virtual void UpdateCheckMarks();

		// Virtual Attributes
		// If you need to modify the default behaviour of the menubar, rebar, 
		// statusbar or toolbar, inherrit from those classes, and override 
		// the following attribute functions.
		virtual CMenubar& GetMenubar() const		{return (CMenubar&)m_WndMenubar;}
		virtual CRebar& GetRebar() const			{return (CRebar&)m_WndRebar;}
		virtual CStatusbar& GetStatusbar() const	{return (CStatusbar&)m_WndStatusbar;}
		virtual CToolbar& GetToolbar() const		{return (CToolbar&)m_WndToolbar;}

		// These functions aren't intended to be overridden
		HMENU GetFrameMenu() const	{return m_hMenu;}
		const ThemeMenu& GetMenuTheme()	const {return m_ThemeMenu;}
		tString GetRegistryKeyName(){return m_tsKeyName;}
		CWnd* GetView() const		{return m_pWndView;}
		tString GetMRUEntry(int nIndex);
		void SetFrameMenu(INT ID_MENU);
		void SetMenuTheme(ThemeMenu& Theme);
		void LoadRegistrySettings(LPCTSTR szKeyName);
		void SetView(CWnd& wndView);

		BOOL IsMDIFrame() const			{return m_bIsMDIFrame;}
		BOOL IsMenubarUsed() const		{return (GetMenubar() != 0);}
		BOOL IsRebarSupported() const	{return (GetComCtlVersion() >= 470);}
		BOOL IsRebarUsed() const		{return (GetRebar() != 0);}

	protected:
		// These are the functions you might wish to override
		virtual void AddMenubarBand();
		virtual void AddMRUEntry(LPCTSTR szMRUEntry);
		virtual void AddToolbarBand(CToolbar& TB);
		virtual void DrawCheckmark(LPDRAWITEMSTRUCT pdis);
		virtual void DrawMenuIcon(LPDRAWITEMSTRUCT pdis, BOOL bDisabled);
		virtual void DrawMenuText(CDC& DrawDC, LPCTSTR ItemText, CRect& rc, COLORREF colorText);
		virtual void OnCreate();
		virtual void OnFrameClose();
		virtual BOOL OnFrameCommand(WPARAM wPAram, LPARAM lParam);
		virtual LRESULT OnFrameNotify(WPARAM wParam, LPARAM lParam);
		virtual void OnFrameSetFocus();
		virtual void OnFrameSysColorChange();
		virtual	void OnFrameTimer(WPARAM wParam);
		virtual void OnHelp();
		virtual void OnMenuSelect(WPARAM wParam, LPARAM lParam);
		virtual void OnViewStatusbar();
		virtual void OnViewToolbar();
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void PreRegisterClass(WNDCLASS &wc);
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		virtual void RemoveMRUEntry(LPCTSTR szMRUEntry);
		virtual void SaveRegistrySettings();
		virtual void UpdateMRUMenu();
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// Its unlikely you would need to override these functions
		virtual void SetMenubarBandSize();

		enum Constants
		{
			ID_STATUS_TIMER = 1,
			POST_TEXT_GAP   = 16,			// for owner draw menu item
		};

		struct ItemData
		// Each Dropdown menu item has this data
		{
			HMENU hMenu;
			UINT  nPos;
			UINT  fType;
			TCHAR Text[MAX_MENU_STRING];
			HMENU hSubMenu;
		};

		std::vector<UINT> m_ToolbarData;	// vector of resource IDs for toolbar buttons
		std::vector<ItemData*> m_vpItemData;// vector of ItemData pointers
		std::vector<UINT> m_MenuData;		// vector of menu icon resource IDs
		BOOL m_bIsMDIFrame;					// TRUE if this is a MDI frame
        BOOL m_bShowIndicatorStatus;		// set to TRUE to see indicators in status bar
		BOOL m_bShowMenuStatus;				// set to TRUE to see menu and toolbar updates in status bar
		BOOL m_bUseRebar;					// set to TRUE if Rebars are to be used
		BOOL m_bUseThemes;					// set to TRUE if themes are to be used
		BOOL m_bUpdateTheme;				// set to TRUE to run SetThemes when theme changes
		ThemeMenu m_ThemeMenu;				// Theme structure
		HIMAGELIST m_himlMenu;				// Imagelist of menu icons
		HIMAGELIST m_himlMenuDis;			// Imagelist of disabled menu icons

	private:
		void LoadCommonControls(INITCOMMONCONTROLSEX InitStruct);

		CDialog* m_pAboutDialog;			// Pointer to the about dialog object
		CMenubar m_WndMenubar;				// CMenubar object
		CRebar m_WndRebar;					// CRebar object
		CStatusbar m_WndStatusbar;			// CStatusbar object
		CToolbar m_WndToolbar;				// CToolbar object
		HMENU m_hMenu;						// handle to the frame menu
		CWnd* m_pWndView;					// pointer to the View CWnd object
		LPCTSTR m_OldStatus[3];				// Array of TCHAR pointers;
		std::vector<tString> m_vMRUEntries;	// Vector of tStrings for MRU entires
		tString m_tsKeyName;				// TCHAR std::string for Registry key name
		tString m_tsStatusText;				// TCHAR std::string for status text
		UINT m_nMaxMRU;						// maximum number of MRU entries
		CRect m_rcPosition;					// CRect of the starting window position
		HWND m_hOldFocus;					// The window which had focus prior to the app'a deactivation

	};  // class CFrame


	/////////////////////////////////////
	// Definitions for the CMenubar class
	//
	inline CMenubar::CMenubar()
	{
		m_bExitAfter	= FALSE;
		m_hTopMenu		= NULL;
		m_nHotItem		= -1;
		m_bSelPopup		= FALSE;
		m_hSelMenu		= NULL;
		m_bMenuActive	= FALSE;
		m_bKeyMode		= FALSE;
		m_hPrevFocus	= NULL;
		m_nMDIButton    = 0;
		m_hPopupMenu	= 0;

		ZeroMemory(&m_ThemeMenu, sizeof(ThemeMenu));
	}

	inline CMenubar::~CMenubar()
	{
	}

	inline void CMenubar::DoAltKey(WORD KeyCode)
	{
		//Handle key pressed with Alt held down
		UINT ID;
		if (::SendMessage(m_hWnd, TB_MAPACCELERATOR, KeyCode, (LPARAM) &ID))
		{
			GrabFocus();
			m_bKeyMode = TRUE;
			SetHotItem(ID);
			m_bMenuActive = TRUE;
			::PostMessage(m_hWnd, UWM_POPUPMENU, 0, 0);
		}
		else
			::MessageBeep(MB_OK);
	}

	inline void CMenubar::DoPopupMenu()
	{
		if (m_bKeyMode)
			// Simulate a down arrow key press
			::PostMessage(m_hWnd, WM_KEYDOWN, VK_DOWN, 0);

		m_bKeyMode = FALSE;
		m_bExitAfter = FALSE;
		::GetCursorPos(&m_OldMousePos);

		HWND hMaxMDIChild = NULL;
		if (IsMDIChildMaxed())
			hMaxMDIChild = GetActiveMDIChild();

		// Load the submenu
		int nMaxedOffset = IsMDIChildMaxed()? 1:0;
		m_hPopupMenu = ::GetSubMenu(m_hTopMenu, m_nHotItem - nMaxedOffset);
		if (IsMDIChildMaxed() && (0 == m_nHotItem) )
			m_hPopupMenu = ::GetSystemMenu(hMaxMDIChild, FALSE);

        // Retrieve the bounding rectangle for the toolbar button
		CRect rc = GetItemRect(m_nHotItem);

		// convert rectangle to desktop coordinates
		::MapWindowPoints(m_hWnd, HWND_DESKTOP, (LPPOINT)&rc, 2);

		// Position popup above toolbar if it won't fit below
		TPMPARAMS tpm;
		tpm.cbSize = sizeof(TPMPARAMS);
		tpm.rcExclude = rc;

		// Set the hot button
		::SendMessage(m_hWnd, TB_SETHOTITEM, m_nHotItem, 0);
		::SendMessage(m_hWnd, TB_PRESSBUTTON, m_nHotItem, MAKELONG(TRUE, 0));

		m_bSelPopup = FALSE;
		m_hSelMenu = NULL;
		m_bMenuActive = TRUE;

		// We hook mouse input to process mouse and keyboard input during
		//  the popup menu. Messages are sent to StaticMsgHook.

		// Remove any remaining hook first
		TLSData* pTLSData = (TLSData*)::TlsGetValue(GetApp()->GetTlsIndex());
		pTLSData->pMenubar = this;
		if (pTLSData->hMenuHook != NULL)
			::UnhookWindowsHookEx(pTLSData->hMenuHook);

		// Hook messages about to be processed by the shortcut menu
		pTLSData->hMenuHook = ::SetWindowsHookEx(WH_MSGFILTER, (HOOKPROC)StaticMsgHook, NULL, ::GetCurrentThreadId());

		// Display the shortcut menu
		UINT nID = ::TrackPopupMenuEx(m_hPopupMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
			rc.left, rc.bottom, m_hWnd, &tpm);

		// We get here once the TrackPopupMenuEx has ended
		m_bMenuActive = FALSE;

		// Remove the message hook
		::UnhookWindowsHookEx(pTLSData->hMenuHook);
		pTLSData->hMenuHook = NULL;

		// Process MDI Child system menu
		if (IsMDIChildMaxed())
		{
			if (::GetSystemMenu(hMaxMDIChild, FALSE) == m_hPopupMenu )
			{
				if (nID)
					::SendMessage(hMaxMDIChild, WM_SYSCOMMAND, nID, 0);
			}
		}

		// Resestablish Focus
		if (m_bKeyMode)
			GrabFocus();
	}

	inline void CMenubar::DrawAllMDIButtons(CDC& DrawDC)
	{
		if (!IsMDIFrame())
			return;

		if (IsMDIChildMaxed())
		{
			int cx = GetSystemMetrics(SM_CXSMICON);
			int cy = GetSystemMetrics(SM_CYSMICON);
			CRect rc = GetClientRect();
			int gap = 4;
			rc.right -= gap;

			// Assign values to each element of the CRect array
			for (int i = 0 ; i < 3 ; ++i)
			{
				int left = rc.right - (i+1)*cx - gap*(i+1);
				int top = rc.bottom/2 - cy/2;
				int right = rc.right - i*cx - gap*(i+1);
				int bottom = rc.bottom/2 + cy/2;
				::SetRect(&m_MDIRect[2 - i], left, top, right, bottom);
			}

			// Hide the MDI button if it won't fit
			for (int k = 0 ; k <= 2 ; ++k)
			{

				if (m_MDIRect[k].left < GetMaxSize().cx)
				{
					::SetRectEmpty(&m_MDIRect[k]);
				}
			}

			DrawMDIButton(DrawDC, MDI_MIN, 0);
			DrawMDIButton(DrawDC, MDI_RESTORE, 0);
			DrawMDIButton(DrawDC, MDI_CLOSE, 0);
		}
	}

	inline void CMenubar::DrawMDIButton(CDC& DrawDC, int iButton, UINT uState)
	{
		if (!IsRectEmpty(&m_MDIRect[iButton]))
		{
			switch (uState)
			{
			case 0:
				{
					// Draw a grey outline
					DrawDC.CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNFACE));
					::MoveToEx(DrawDC, m_MDIRect[iButton].left, m_MDIRect[iButton].bottom, NULL);
					::LineTo(DrawDC, m_MDIRect[iButton].right, m_MDIRect[iButton].bottom);
					::LineTo(DrawDC, m_MDIRect[iButton].right, m_MDIRect[iButton].top);
					::LineTo(DrawDC, m_MDIRect[iButton].left, m_MDIRect[iButton].top);
					::LineTo(DrawDC, m_MDIRect[iButton].left, m_MDIRect[iButton].bottom);
				}
				break;
			case 1:
				{
					// Draw outline, white at top, black on bottom
					DrawDC.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					::MoveToEx(DrawDC, m_MDIRect[iButton].left, m_MDIRect[iButton].bottom, NULL);
					::LineTo(DrawDC, m_MDIRect[iButton].right, m_MDIRect[iButton].bottom);
					::LineTo(DrawDC, m_MDIRect[iButton].right, m_MDIRect[iButton].top);
					DrawDC.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
					::LineTo(DrawDC, m_MDIRect[iButton].left, m_MDIRect[iButton].top);
					::LineTo(DrawDC, m_MDIRect[iButton].left, m_MDIRect[iButton].bottom);
				}

				break;
			case 2:
				{
					// Draw outline, black on top, white on bottom
					DrawDC.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
					::MoveToEx(DrawDC, m_MDIRect[iButton].left, m_MDIRect[iButton].bottom, NULL);
					::LineTo(DrawDC, m_MDIRect[iButton].right, m_MDIRect[iButton].bottom);
					::LineTo(DrawDC, m_MDIRect[iButton].right, m_MDIRect[iButton].top);
					DrawDC.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					::LineTo(DrawDC, m_MDIRect[iButton].left, m_MDIRect[iButton].top);
					::LineTo(DrawDC, m_MDIRect[iButton].left, m_MDIRect[iButton].bottom);
				}
				break;
			}

			DrawDC.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

			switch (iButton)
			{
			case MDI_MIN:
				// Manually Draw Minimise button
				::MoveToEx(DrawDC, m_MDIRect[0].left + 4, m_MDIRect[0].bottom -4, NULL);
				::LineTo(DrawDC, m_MDIRect[0].right - 4, m_MDIRect[0].bottom - 4);

				::MoveToEx(DrawDC, m_MDIRect[0].left + 4, m_MDIRect[0].bottom -5, NULL);
				::LineTo(DrawDC, m_MDIRect[0].right - 4, m_MDIRect[0].bottom - 5);
				break;
			case MDI_RESTORE:
				// Manually Draw Restore Button
				::MoveToEx(DrawDC, m_MDIRect[1].left + 3, m_MDIRect[1].top + 7, NULL);
				::LineTo(DrawDC, m_MDIRect[1].left + 3, m_MDIRect[1].bottom -4);
				::LineTo(DrawDC, m_MDIRect[1].right - 6, m_MDIRect[1].bottom -4);
				::LineTo(DrawDC, m_MDIRect[1].right - 6, m_MDIRect[1].top + 7);
				::LineTo(DrawDC, m_MDIRect[1].left + 3, m_MDIRect[1].top + 7);

				::MoveToEx(DrawDC, m_MDIRect[1].left + 3, m_MDIRect[1].top + 8, NULL);
				::LineTo(DrawDC, m_MDIRect[1].right - 6, m_MDIRect[1].top + 8);

				::MoveToEx(DrawDC, m_MDIRect[1].left + 5, m_MDIRect[1].top + 7, NULL);
				::LineTo(DrawDC, m_MDIRect[1].left + 5, m_MDIRect[1].top + 4);
				::LineTo(DrawDC, m_MDIRect[1].right - 4, m_MDIRect[1].top + 4);
				::LineTo(DrawDC, m_MDIRect[1].right - 4, m_MDIRect[1].bottom -6);
				::LineTo(DrawDC, m_MDIRect[1].right - 6, m_MDIRect[1].bottom -6);

				::MoveToEx(DrawDC, m_MDIRect[1].left + 5, m_MDIRect[1].top + 5, NULL);
				::LineTo(DrawDC, m_MDIRect[1].right - 4, m_MDIRect[1].top + 5);
				break;
			case MDI_CLOSE:
				// Manually Draw Close Button
				::MoveToEx(DrawDC, m_MDIRect[2].left + 4, m_MDIRect[2].top +5, NULL);
				::LineTo(DrawDC, m_MDIRect[2].right - 4, m_MDIRect[2].bottom -3);

				::MoveToEx(DrawDC, m_MDIRect[2].left + 5, m_MDIRect[2].top +5, NULL);
				::LineTo(DrawDC, m_MDIRect[2].right - 4, m_MDIRect[2].bottom -4);

				::MoveToEx(DrawDC, m_MDIRect[2].left + 4, m_MDIRect[2].top +6, NULL);
				::LineTo(DrawDC, m_MDIRect[2].right - 5, m_MDIRect[2].bottom -3);

				::MoveToEx(DrawDC, m_MDIRect[2].right -5, m_MDIRect[2].top +5, NULL);
				::LineTo(DrawDC, m_MDIRect[2].left + 3, m_MDIRect[2].bottom -3);

				::MoveToEx(DrawDC, m_MDIRect[2].right -5, m_MDIRect[2].top +6, NULL);
				::LineTo(DrawDC, m_MDIRect[2].left + 4, m_MDIRect[2].bottom -3);

				::MoveToEx(DrawDC, m_MDIRect[2].right -6, m_MDIRect[2].top +5, NULL);
				::LineTo(DrawDC, m_MDIRect[2].left + 3, m_MDIRect[2].bottom -4);
				break;
			}
		}
	}

	inline void CMenubar::ExitMenu()
	{
		ReleaseFocus();
		m_bKeyMode = FALSE;
		m_bMenuActive = FALSE;
		::SendMessage(m_hWnd, TB_PRESSBUTTON, m_nHotItem, (LPARAM) MAKELONG (FALSE, 0));
		SetHotItem(-1);

		CPoint pt;
		::GetCursorPos(&pt);
		::ScreenToClient(m_hWnd, &pt);

		// Update mouse mouse position for hot tracking
		::SendMessage(m_hWnd, WM_MOUSEMOVE, 0, MAKELONG(pt.x, pt.y));
	}

	inline HWND CMenubar::GetActiveMDIChild()
	{
		HWND hwndMDIChild = NULL;
		if (IsMDIFrame())
		{
			hwndMDIChild = (HWND)::SendMessage(m_pFrame->GetView()->GetHwnd(), WM_MDIGETACTIVE, 0, 0);
		}

		return hwndMDIChild;
	}

	inline void CMenubar::GrabFocus()
	{
		if (::GetFocus() != m_hWnd)
			m_hPrevFocus = ::SetFocus(m_hWnd);
		::SetCapture(m_hWnd);
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}

	inline BOOL CMenubar::IsMDIChildMaxed() const
	{
		BOOL bMaxed = FALSE;

		if (IsMDIFrame())
		{
			::SendMessage(m_pFrame->GetView()->GetHwnd(), WM_MDIGETACTIVE, 0, (LPARAM)&bMaxed);
		}

		return bMaxed;
	}

	inline BOOL CMenubar::IsMDIFrame() const
	{
		return m_pFrame->IsMDIFrame();
	}

	inline void CMenubar::MenuChar(WPARAM wParam, LPARAM /* lParam */)
	{
		if (!m_bMenuActive)
			DoAltKey(LOWORD(wParam));
	}

	inline void CMenubar::OnCreate()
	{
		// We must send this message before sending the TB_ADDBITMAP or TB_ADDBUTTONS message
		::SendMessage(m_hWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

		m_pFrame = (CFrame*)FromHandle(GetAncestor());
	}

	inline LRESULT CMenubar::OnCustomDraw(NMHDR* pNMHDR)
	// CustomDraw is used to render the Menubar's toolbar buttons
	{
		LPNMTBCUSTOMDRAW lpNMCustomDraw = (LPNMTBCUSTOMDRAW)pNMHDR;

		switch (lpNMCustomDraw->nmcd.dwDrawStage)
		{
		// Begin paint cycle
		case CDDS_PREPAINT:
			// Send NM_CUSTOMDRAW item draw, and post-paint notification messages.
			return CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT ;

		// An item is about to be drawn
		case CDDS_ITEMPREPAINT:
			{
				CDC DrawDC = lpNMCustomDraw->nmcd.hdc;
				CRect rcRect = lpNMCustomDraw->nmcd.rc;
				int nState = lpNMCustomDraw->nmcd.uItemState;
				DWORD dwItem = (DWORD)lpNMCustomDraw->nmcd.dwItemSpec;

				// Leave a 2 pixel gap above the drawn rectangle
				rcRect.top = 2;

				if (IsMDIChildMaxed() && (0 == dwItem))
				// Draw over MDI Max button
				{
					HICON hIcon = (HICON)::SendMessage(GetActiveMDIChild(), WM_GETICON, ICON_SMALL, 0);
					if (NULL == hIcon)
						hIcon = ::LoadIcon(NULL, IDI_APPLICATION);

					int cx = ::GetSystemMetrics (SM_CXSMICON);
					int cy = ::GetSystemMetrics (SM_CYSMICON);
					int y = 1 + (rcRect.bottom - rcRect.top - cy)/2;
					int x = 0;
					::DrawIconEx(DrawDC, x, y, hIcon, cx, cy, 0, NULL, DI_NORMAL);

					// Detach the DC so it doesn't get destroyed
					DrawDC.DetachDC();
					return CDRF_SKIPDEFAULT;  // No further drawing
				}

				else if (nState & (CDIS_HOT | CDIS_SELECTED))
				{
					if (m_ThemeMenu.UseThemes)
					{
						if ((nState & CDIS_SELECTED) || (GetButtonState(dwItem) & TBSTATE_PRESSED))
						{
							GradientFill(DrawDC, m_ThemeMenu.clrPressed1, m_ThemeMenu.clrPressed2, &rcRect, FALSE);
						}
						else if (nState & CDIS_HOT)
						{
							GradientFill(DrawDC, m_ThemeMenu.clrHot1, m_ThemeMenu.clrHot2, &rcRect, FALSE);
						}

						// Draw border
						DrawDC.CreatePen(PS_SOLID, 1, m_ThemeMenu.clrOutline);
						::MoveToEx(DrawDC, rcRect.left, rcRect.bottom -1, NULL);
						::LineTo(DrawDC, rcRect.left, rcRect.top);
						::LineTo(DrawDC, rcRect.right-1, rcRect.top);
						::LineTo(DrawDC, rcRect.right-1, rcRect.bottom);
						if (!(nState & CDIS_SELECTED))
						{
							::MoveToEx(DrawDC, rcRect.right-1, rcRect.bottom-1, NULL);
							::LineTo(DrawDC, rcRect.left, rcRect.bottom-1);
						}
					}
					else
					{
						// Draw highlight rectangle
						DrawDC.CreatePen(PS_SOLID, 1, m_ThemeMenu.clrOutline);
						HBRUSH hbHighlight = ::GetSysColorBrush(COLOR_HIGHLIGHT);
						::FillRect(DrawDC, &rcRect, hbHighlight);
					}

					TCHAR str[80] = _T("");
					int nLength = (int)::SendMessage(m_hWnd, TB_GETBUTTONTEXT, lpNMCustomDraw->nmcd.dwItemSpec, (LPARAM) NULL);
					if ((nLength > 0) && (nLength < 80))
						::SendMessage(m_hWnd, TB_GETBUTTONTEXT, lpNMCustomDraw->nmcd.dwItemSpec, (LPARAM)str);

					// Draw highlight text
					DrawDC.AttachFont((HFONT)::SendMessage(m_hWnd, WM_GETFONT, 0, 0));
					if (!m_ThemeMenu.UseThemes)
						::SetTextColor(DrawDC, ::GetSysColor(COLOR_HIGHLIGHTTEXT));

					rcRect.bottom += 1;
					int iMode = ::SetBkMode(DrawDC, TRANSPARENT);
					::DrawText(DrawDC, str, lstrlen(str), &rcRect, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

					::SetBkMode(DrawDC, iMode);
					DrawDC.DetachFont();
					DrawDC.DetachDC();
					return CDRF_SKIPDEFAULT;  // No further drawing
				}
				// Detach the DC so it doesn't get destroyed
				DrawDC.DetachDC();
			}
			return CDRF_DODEFAULT ;   // Do default drawing

		// Painting cycle has completed
		case CDDS_POSTPAINT:
			// Draw MDI Minimise, Restore and Close buttons
			{
				CDC DrawDC = lpNMCustomDraw->nmcd.hdc;
				DrawAllMDIButtons(DrawDC);
				// Detach the DC so it doesn't get destroyed
				DrawDC.DetachDC();
			}
			break;
		}
		return 0L;
	}

	inline void CMenubar::OnKeyDown(WPARAM wParam, LPARAM /*lParam*/)
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			ExitMenu();
			break;

		case VK_SPACE:
			ExitMenu();
			// Bring up the system menu
			::PostMessage(GetAncestor(), WM_SYSCOMMAND, SC_KEYMENU, VK_SPACE);
			break;

		// Handle VK_DOWN,VK_UP and VK_RETURN together
		case VK_DOWN:
		case VK_UP:
		case VK_RETURN:
			// Always use PostMessage for USER_POPUPMENU (not SendMessage)
			::PostMessage(m_hWnd, UWM_POPUPMENU, 0, 0);
			break;

		case VK_LEFT:
			// Move left to next topmenu item
			(m_nHotItem > 0)? SetHotItem(m_nHotItem -1) : SetHotItem(GetButtonCount()-1);
			break;

		case VK_RIGHT:
			// Move right to next topmenu item
			(m_nHotItem < GetButtonCount() -1)? SetHotItem(m_nHotItem +1) : SetHotItem(0);
			break;

		default:
			// Handle Accelerator keys with Alt toggled down
			if (m_bKeyMode)
			{
				UINT ID;
				if (::SendMessage(m_hWnd, TB_MAPACCELERATOR, wParam, (LPARAM) &ID))
				{
					m_nHotItem = ID;
					::PostMessage(m_hWnd, UWM_POPUPMENU, 0, 0);
				}
				else
					::MessageBeep(MB_OK);
			}
			break;
		} // switch (wParam)
	}

	inline void CMenubar::OnLButtonDown(WPARAM /*wParam*/, LPARAM lParam)
	{
		GrabFocus();
		m_nMDIButton = 0;
		CPoint pt;

		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		if (IsMDIFrame())
		{
			if (IsMDIChildMaxed())
			{
				CDC MenubarDC = ::GetDC(m_hWnd);
				m_nMDIButton = -1;

				if (PtInRect(&m_MDIRect[0], pt)) m_nMDIButton = 0;
				if (PtInRect(&m_MDIRect[1], pt)) m_nMDIButton = 1;
				if (PtInRect(&m_MDIRect[2], pt)) m_nMDIButton = 2;

				if (m_nMDIButton >= 0)
				{
					DrawMDIButton(MenubarDC, MDI_MIN,     (0 == m_nMDIButton)? 2 : 0);
					DrawMDIButton(MenubarDC, MDI_RESTORE, (1 == m_nMDIButton)? 2 : 0);
					DrawMDIButton(MenubarDC, MDI_CLOSE,   (2 == m_nMDIButton)? 2 : 0);
				}

				// Bring up the MDI Child window's system menu when the icon is pressed
				if (0 == HitTest())
				{
					m_nHotItem = 0;
					::PostMessage(m_hWnd, UWM_POPUPMENU, 0, 0);
				}
			}
		}
	}

	inline void CMenubar::OnLButtonUp(WPARAM /*wParam*/, LPARAM lParam)
	{
		CPoint pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		if (IsMDIFrame())
		{
			HWND MDIClient = m_pFrame->GetView()->GetHwnd();
			HWND MDIChild = GetActiveMDIChild();

			if (IsMDIChildMaxed())
			{
				CPoint pt;
				::GetCursorPos(&pt);
				::ScreenToClient(m_hWnd, &pt);

				// Process the MDI button action when the left mouse button is up
				if (PtInRect(&m_MDIRect[0], pt))
				{
					if (MDI_MIN == m_nMDIButton)
						::ShowWindow(MDIChild, SW_MINIMIZE);
				}

				if (PtInRect(&m_MDIRect[1], pt))
				{
					if (MDI_RESTORE == m_nMDIButton)
					::PostMessage(MDIClient, WM_MDIRESTORE, (WPARAM)MDIChild, 0);
				}

				if (PtInRect(&m_MDIRect[2], pt))
				{
					if (MDI_CLOSE == m_nMDIButton)
						::PostMessage(MDIChild, WM_CLOSE, 0, 0);
				}
			}
		}
		m_nMDIButton = 0;
		ExitMenu();
	}

	inline BOOL CMenubar::OnMenuInput(UINT uMsg, WPARAM wParam, LPARAM lParam)
	// When a popup menu is active, StaticMsgHook directs all menu messages here
	{
		switch(uMsg)
		{
		case WM_KEYDOWN:
			m_bExitAfter = FALSE;
			{
				switch (wParam)
				{
				case VK_ESCAPE:
					// Use default processing if inside a Sub Menu
					if ((m_hSelMenu) &&(m_hSelMenu != m_hPopupMenu))
						return FALSE;

					m_bMenuActive = FALSE;
					m_bKeyMode = TRUE;
					::SendMessage(m_hWnd, WM_CANCELMODE, 0, 0);
					::SendMessage(m_hWnd, TB_PRESSBUTTON, m_nHotItem, MAKELONG(FALSE, 0));
					::SendMessage(m_hWnd, TB_SETHOTITEM, m_nHotItem, 0);
					break;

				case VK_LEFT:
					// Use default processing if inside a Sub Menu
				    if ((m_hSelMenu) &&(m_hSelMenu != m_hPopupMenu))
						return FALSE;

					::SendMessage(m_hWnd, TB_PRESSBUTTON, m_nHotItem, MAKELONG(FALSE, 0));

					// Move left to next topmenu item
					(m_nHotItem > 0)? --m_nHotItem : m_nHotItem = GetButtonCount()-1;
					::SendMessage(m_hWnd, WM_CANCELMODE, 0, 0);

					// Always use PostMessage for USER_POPUPMENU (not SendMessage)
					::PostMessage(m_hWnd, UWM_POPUPMENU, 0, 0);
					::PostMessage(m_hWnd, WM_KEYDOWN, VK_DOWN, 0);
					break;

				case VK_RIGHT:
					// Use default processing to open Sub Menu
					if (m_bSelPopup)
						return FALSE;

					::SendMessage(m_hWnd, TB_PRESSBUTTON, m_nHotItem, MAKELONG(FALSE, 0));

					// Move right to next topmenu item
					(m_nHotItem < GetButtonCount()-1)? ++m_nHotItem : m_nHotItem = 0;
					::SendMessage(m_hWnd, WM_CANCELMODE, 0, 0);

					// Always use PostMessage for USER_POPUPMENU (not SendMessage)
					::PostMessage(m_hWnd, UWM_POPUPMENU, 0, 0);
					::PostMessage(m_hWnd, WM_KEYDOWN, VK_DOWN, 0);
					break;

				case VK_RETURN:
					m_bExitAfter = TRUE;
					break;

				} // switch (wParam)

			} // case WM_KEYDOWN

			return FALSE;

		case WM_CHAR:
			m_bExitAfter = TRUE;
			return FALSE;

		case WM_LBUTTONDOWN:
			{
				m_bExitAfter = TRUE;
				if (HitTest() >= 0)
				{
					// Cancel popup when we hit a button a second time
					::SendMessage(m_hWnd, WM_CANCELMODE, 0, 0);
					return TRUE;
				}
			}
			return FALSE;

		case WM_LBUTTONDBLCLK:
			// Perform default action for DblClick on MDI Maxed icon
			if (IsMDIChildMaxed() && (0 == HitTest()))
			{
				HWND MDIChild = GetActiveMDIChild();
				HMENU hChildMenu = ::GetSystemMenu(MDIChild, FALSE);

				UINT nID = ::GetMenuDefaultItem(hChildMenu, FALSE, 0);
				if (nID)
					::PostMessage(MDIChild, WM_SYSCOMMAND, nID, 0);
			}

			m_bExitAfter = TRUE;
			return FALSE;

		case WM_MENUSELECT:
			{
				// store info about selected item
				m_hSelMenu = (HMENU)lParam;
				m_bSelPopup = ((HIWORD(wParam) & MF_POPUP) != 0);

				// Reflect message back to the frame window
				::SendMessage(GetAncestor(), WM_MENUSELECT, wParam, lParam);
			}
			return TRUE;

		case WM_MOUSEMOVE:
			{
				CPoint pt;
				pt.x = GET_X_LPARAM(lParam);
				pt.y = GET_Y_LPARAM(lParam);

				// Skip if mouse hasn't moved
				if ((pt.x == m_OldMousePos.x) && (pt.y == m_OldMousePos.y))
					return FALSE;

				m_OldMousePos.x = pt.x;
				m_OldMousePos.y = pt.y;
				::ScreenToClient(m_hWnd, &pt);

				// Reflect messages back to the Menubar for hot tracking
				::SendMessage(m_hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(pt.x, pt.y));
			}
			break;

		}
		return FALSE;
	}

	inline void CMenubar::OnMouseLeave()
	{
		if (IsMDIFrame())
		{
			if (IsMDIChildMaxed())
			{
				CDC MenubarDC = ::GetDC(m_hWnd);

				DrawMDIButton(MenubarDC, MDI_MIN,     0);
				DrawMDIButton(MenubarDC, MDI_RESTORE, 0);
				DrawMDIButton(MenubarDC, MDI_CLOSE,   0);
			}
		}
	}

	inline void CMenubar::OnMouseMove(WPARAM wParam, LPARAM lParam)
	{
		CPoint pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		if (IsMDIFrame())
		{
			if (IsMDIChildMaxed())
			{
				CDC MenubarDC = ::GetDC(m_hWnd);
				int MDIButton = -1;
				if (PtInRect(&m_MDIRect[0], pt)) MDIButton = 0;
				if (PtInRect(&m_MDIRect[1], pt)) MDIButton = 1;
				if (PtInRect(&m_MDIRect[2], pt)) MDIButton = 2;

				if (MK_LBUTTON == wParam)  // mouse moved with left mouse button is held down
				{
					// toggle the MDI button image pressed/unpressed as required
					if (MDIButton >= 0)
					{
						DrawMDIButton(MenubarDC, MDI_MIN,     ((0 == MDIButton) && (0 == m_nMDIButton))? 2 : 0);
						DrawMDIButton(MenubarDC, MDI_RESTORE, ((1 == MDIButton) && (1 == m_nMDIButton))? 2 : 0);
						DrawMDIButton(MenubarDC, MDI_CLOSE,   ((2 == MDIButton) && (2 == m_nMDIButton))? 2 : 0);
					}
					else
					{
						DrawMDIButton(MenubarDC, MDI_MIN,     0);
						DrawMDIButton(MenubarDC, MDI_RESTORE, 0);
						DrawMDIButton(MenubarDC, MDI_CLOSE,   0);
					}
				}
				else	// mouse moved without left mouse button held down
				{
					if (MDIButton >= 0)
					{
						DrawMDIButton(MenubarDC, MDI_MIN,     (0 == MDIButton)? 1 : 0);
						DrawMDIButton(MenubarDC, MDI_RESTORE, (1 == MDIButton)? 1 : 0);
						DrawMDIButton(MenubarDC, MDI_CLOSE,   (2 == MDIButton)? 1 : 0);
					}
					else
					{
						DrawMDIButton(MenubarDC, MDI_MIN,     0);
						DrawMDIButton(MenubarDC, MDI_RESTORE, 0);
						DrawMDIButton(MenubarDC, MDI_CLOSE,   0);
					}
				}
			}
		}
	}

	inline LRESULT CMenubar::OnNotifyReflect(WPARAM /* wParam */, LPARAM lParam)
	{
		switch (((LPNMHDR)lParam)->code)
		{
		case NM_CUSTOMDRAW:
			{
				return OnCustomDraw((LPNMHDR) lParam);
			}

		case TBN_DROPDOWN:
			// Always use PostMessage for USER_POPUPMENU (not SendMessage)
			::PostMessage(m_hWnd, UWM_POPUPMENU, 0, 0);
			break;

		case TBN_HOTITEMCHANGE:
			// This is the notification that a hot item change is about to occur
			// This is used to bring up a new popup menu when required
			{
				DWORD flag = ((LPNMTBHOTITEM)lParam)->dwFlags;
				if ((flag & HICF_MOUSE) && !(flag & HICF_LEAVING))
				{
					int nButton = HitTest();
					if ((m_bMenuActive) && (nButton != m_nHotItem))
					{
						::SendMessage(m_hWnd, TB_PRESSBUTTON, m_nHotItem, MAKELONG(FALSE, 0));
						m_nHotItem = nButton;
						::SendMessage(m_hWnd, WM_CANCELMODE, 0, 0);

						//Always use PostMessage for USER_POPUPMENU (not SendMessage)
						::PostMessage(m_hWnd, UWM_POPUPMENU, 0, 0);
					}
					m_nHotItem = nButton;
				}

				// Handle escape from popup menu
				if ((flag & HICF_LEAVING) && m_bKeyMode)
				{
					m_nHotItem = ((LPNMTBHOTITEM)lParam)->idOld;
					::PostMessage(m_hWnd, TB_SETHOTITEM, m_nHotItem, 0);
				}

				break;
			} //case TBN_HOTITEMCHANGE:

		} // switch(((LPNMHDR)lParam)->code)
		return 0L;
	} // CMenubar::OnNotify(...)

	inline void CMenubar::OnWindowPosChanged()
	{
		::InvalidateRect(m_hWnd, &m_MDIRect[0], TRUE);
		::InvalidateRect(m_hWnd, &m_MDIRect[1], TRUE);
		::InvalidateRect(m_hWnd, &m_MDIRect[2], TRUE);
		{
			CDC MenubarDC = ::GetDC(m_hWnd);
			DrawAllMDIButtons(MenubarDC);
		}
	}

	inline void CMenubar::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | CCS_NODIVIDER | CCS_NORESIZE;
		cs.lpszClass = TOOLBARCLASSNAME;
	}

	inline void CMenubar::ReleaseFocus()
	{
		if (m_hPrevFocus)
			::SetFocus(m_hPrevFocus);

		m_hPrevFocus = NULL;
		::ReleaseCapture();
	}

	inline void CMenubar::SetHotItem(int nHot)
	{
		m_nHotItem = nHot;
		::SendMessage(m_hWnd, TB_SETHOTITEM, m_nHotItem, 0);
	}

	inline void CMenubar::SetMenu(HMENU hMenu)
	{
		if (!IsWindow()) return;

		m_hTopMenu = hMenu;
		int nMaxedOffset = (IsMDIChildMaxed()? 1:0);

		// Remove any existing buttons
		while (::SendMessage(m_hWnd, TB_BUTTONCOUNT,  0, 0) > 0)
		{
			if(!::SendMessage(m_hWnd, TB_DELETEBUTTON, 0, 0))
				break;
		}

		// Set the Bitmap size to zero
		::SendMessage(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(0, 0));

		if (IsMDIChildMaxed())
		{
			// Create an extra button for the MDI child system menu
			// Later we will custom draw the window icon over this button
			TBBUTTON tbb = {0};
			tbb.fsState = TBSTATE_ENABLED;
			tbb.fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE ;
			tbb.iString = (INT_PTR)_T(" ");
			if(!::SendMessage(m_hWnd, TB_ADDBUTTONS, 1, (WPARAM)&tbb))
				throw CWinException(_T("Menubar::SetMenu  TB_ADDBUTTONS failed"));

			SetButtonText(0, _T("    "));
		}

		for (int i = 0 ; i < ::GetMenuItemCount(hMenu); ++i)
		{
			// Assign the Toolbar Button struct
			TBBUTTON tbb = {0};
			tbb.idCommand = i  + nMaxedOffset;	// Each button needs a unique ID
			tbb.fsState = TBSTATE_ENABLED;
			tbb.fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE | TBSTYLE_DROPDOWN;
			tbb.iString = (INT_PTR)_T(" ");
			if (!::SendMessage(m_hWnd, TB_ADDBUTTONS, 1, (WPARAM)&tbb))
				throw CWinException(_T("Menubar::SetMenu  TB_ADDBUTTONS failed"));

			// Add the menu title to the string table
			TCHAR szMenuName[MAX_MENU_STRING +1] = _T("");

			if (0 == ::GetMenuString(hMenu, i, szMenuName, MAX_MENU_STRING, MF_BYPOSITION) )
				throw CWinException(TEXT("Menubar::SetMenu  GetMenuString failed"));

			SetButtonText(i  + nMaxedOffset, szMenuName);
		}

	}

	inline void CMenubar::SetMenubarTheme(ThemeMenu& Theme)
	{
		m_ThemeMenu.UseThemes   = Theme.UseThemes;
		m_ThemeMenu.clrHot1     = Theme.clrHot1;
		m_ThemeMenu.clrHot2     = Theme.clrHot2;
		m_ThemeMenu.clrPressed1 = Theme.clrPressed1;
		m_ThemeMenu.clrPressed2 = Theme.clrPressed2;
		m_ThemeMenu.clrOutline  = Theme.clrOutline;

		::InvalidateRect(m_hWnd, NULL, TRUE);
	}

	inline LRESULT CALLBACK CMenubar::StaticMsgHook(int nCode, WPARAM wParam, LPARAM lParam)
	{
		MSG* pMsg = (MSG*)lParam;
		TLSData* pTLSData = (TLSData*)TlsGetValue(GetApp()->GetTlsIndex());
		CMenubar* pMenubar = (CMenubar*)pTLSData->pMenubar;

		if (pMenubar && (MSGF_MENU == nCode))
		{
			// process menu message
			if (pMenubar->OnMenuInput(pMsg->message, pMsg->wParam, pMsg->lParam))
			{
				return TRUE;
			}
		}

		return CallNextHookEx(pTLSData->hMenuHook, nCode, wParam, lParam);
	}

	inline void CMenubar::SysCommand(WPARAM wParam, LPARAM lParam)
	{
		if (SC_KEYMENU == wParam)
		{
			if (0 == lParam)
			{
				// Alt/F10 key toggled
				GrabFocus();
				m_bKeyMode = TRUE;
				int nMaxedOffset = (IsMDIChildMaxed()? 1:0);
				SetHotItem(nMaxedOffset);
			}
			else
				// Handle key pressed with Alt held down
				DoAltKey((WORD)lParam);
		}
	}

	inline LRESULT CMenubar::WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CHAR:
			return 0L;  // Discard these messages
		case WM_DRAWITEM:
			m_pFrame->OnFrameDrawItem(wParam, lParam);
			return TRUE; // handled
		case WM_EXITMENULOOP:
			if (m_bExitAfter)
				ExitMenu();
			m_pFrame->OnFrameExitMenuLoop();
			break;
		case WM_INITMENUPOPUP:
			m_pFrame->OnFrameInitMenuPopup(wParam, lParam);
			break;
		case WM_KEYDOWN:
			OnKeyDown(wParam, lParam);
			return 0L;	// Discard these messages
		case WM_KILLFOCUS:
			ExitMenu();
			return 0L;
		case WM_LBUTTONDBLCLK:
			// Convert double left click to single left click
			::mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			return 0L;	// Discard these messages
		case WM_LBUTTONDOWN:
			// Do default processing first
			CallPrevWindowProc(hWnd, uMsg, wParam, lParam);

			OnLButtonDown(wParam, lParam);
			return 0L;
		case WM_LBUTTONUP:
			OnLButtonUp(wParam, lParam);
			break;
		case WM_MEASUREITEM:
			m_pFrame->OnFrameMeasureItem(wParam, lParam);
			return TRUE; // handled
		case WM_MOUSELEAVE:
			OnMouseLeave();
			break;
		case WM_MOUSEMOVE:
			OnMouseMove(wParam, lParam);
			break;
		case UWM_POPUPMENU:
			DoPopupMenu();
			return 0L;
		case WM_SYSKEYDOWN:
			if ((VK_MENU == wParam) || (VK_F10 == wParam))
				return 0L;
			break;
		case WM_SYSKEYUP:
			if ((VK_MENU == wParam) || (VK_F10 == wParam))
			{
				ExitMenu();
				return 0L;
			}
			break;
		case WM_WINDOWPOSCHANGED:
			OnWindowPosChanged();
			break;
		case WM_WINDOWPOSCHANGING:
			// Bypass CToolbar::WndProcDefault for this message
			return CWnd::WndProcDefault(hWnd, uMsg, wParam, lParam);

		} // switch (uMsg)

		return CToolbar::WndProcDefault(hWnd, uMsg, wParam, lParam);
	} // LRESULT CMenubar::WndProc(...)



	///////////////////////////////////
	// Definitions for the CFrame class
	//
	inline CFrame::CFrame() :  m_bIsMDIFrame(FALSE), m_bShowIndicatorStatus(TRUE), m_bShowMenuStatus(TRUE),
		                m_bUseRebar(FALSE), m_bUseThemes(TRUE), m_bUpdateTheme(FALSE), m_himlMenu(NULL),
		                m_himlMenuDis(NULL), m_pAboutDialog(NULL), m_hMenu(NULL), m_pWndView(NULL),
		                m_tsStatusText(_T("Ready")), m_nMaxMRU(0), m_hOldFocus(0)
	{

		ZeroMemory(&m_ThemeMenu, sizeof(m_ThemeMenu));

		INITCOMMONCONTROLSEX InitStruct;
		InitStruct.dwSize = sizeof(INITCOMMONCONTROLSEX);
		InitStruct.dwICC = ICC_WIN95_CLASSES| ICC_COOL_CLASSES;

		// Do either InitCommonControls or InitCommonControlsEx
		LoadCommonControls(InitStruct);

		// By default, we use the rebar if we can
		if (GetComCtlVersion() >= 470)
			m_bUseRebar = TRUE;

		for (int i = 0 ; i < 3 ; ++i)
			m_OldStatus[i] = _T('\0');

		// Place this code in CMainFrame's constructor
/*
		// Set the Resource IDs for the toolbar buttons
		AddToolbarButton( IDM_FILE_NEW   );
		AddToolbarButton( IDM_FILE_OPEN  );
		AddToolbarButton( IDM_FILE_SAVE  );
		AddToolbarButton( 0 );				// Separator
		AddToolbarButton( IDM_EDIT_CUT   );
		AddToolbarButton( IDM_EDIT_COPY  );
		AddToolbarButton( IDM_EDIT_PASTE );
		AddToolbarButton( 0 );				// Separator
		AddToolbarButton( IDM_FILE_PRINT );
		AddToolbarButton( 0 );				// Separator
		AddToolbarButton( IDM_HELP_ABOUT );
*/
	}

	inline CFrame::~CFrame()
	{
		for (UINT nItem = 0; nItem < m_vpItemData.size(); ++nItem)
		{
			// These are normally deleted in OnFrameExitMenuLoop
			delete m_vpItemData[nItem];
		}

		if (m_hMenu) ::DestroyMenu(m_hMenu);
		if (m_himlMenu) ImageList_Destroy(m_himlMenu);
		if (m_himlMenuDis) ImageList_Destroy(m_himlMenuDis);
	}

	inline BOOL CFrame::AddMenuIcon(int nID_MenuItem, HICON hIcon, int cx /*= 16*/, int cy /*= 16*/)
	{
		// Get ImageList image size
		int cxOld;
		int cyOld;
		ImageList_GetIconSize(m_himlMenu, &cxOld, &cyOld );

		// Create a new ImageList if required
		if ((cx != cxOld) || (cy != cyOld) || (NULL == m_himlMenu))
		{
			if (m_himlMenu) ImageList_Destroy(m_himlMenu);
			m_himlMenu = ImageList_Create(cx, cy, ILC_COLOR32 | ILC_MASK, 1, 0);
			m_MenuData.clear();
		}

        if (ImageList_AddIcon(m_himlMenu, hIcon) != -1)
		{
			m_MenuData.push_back(nID_MenuItem);

			// Recreate the Disabled imagelist
			if (m_himlMenuDis) ImageList_Destroy(m_himlMenuDis);
			m_himlMenuDis = NULL;
			m_himlMenuDis = CreateDisabledImageList(m_himlMenu);

			return TRUE;
		}

		return FALSE;
	}

	inline int CFrame::AddMenuIcons(const std::vector<UINT>& MenuData, COLORREF crMask, UINT ToolbarID, UINT ToolbarDisabledID)
	// Adds the icons from a bitmap resouce to an internal ImageList for use with popup menu items.
	// Note:  If existing are a different size to the new ones, the old ones will be removed!
	//        The ToolbarDisabledID is ignored unless ToolbarID and ToolbarDisabledID bitmaps are the same size.
	{
		// Count the MenuData entries excluding seperators
		int iImages = 0;
		for (unsigned int i = 0 ; i < MenuData.size(); ++i)
		{
			if (MenuData[i] != 0)	// Don't count seperators
			{
				++iImages;
			}
		}

		// Load the button images from Resouce ID
		HBITMAP hbm = LoadBitmap(MAKEINTRESOURCE(ToolbarID));

		if ((0 == iImages) || (NULL == hbm))
			return (int)m_MenuData.size();	// No valid images, so nothing to do!

		BITMAP bm = {0};
		::GetObject(hbm, sizeof(BITMAP), &bm);
		int iImageWidth  = bm.bmWidth / iImages;
		int iImageHeight = bm.bmHeight;

		// Create the ImageList if required
		if (NULL == m_himlMenu)
		{
			m_himlMenu = ImageList_Create(iImageWidth, iImageHeight, ILC_COLOR32 | ILC_MASK, iImages, 0);
			m_MenuData.clear();
		}
		else
		{
			int Oldcx;
			int Oldcy;

			ImageList_GetIconSize(m_himlMenu, &Oldcx, &Oldcy);
			if ((iImageWidth != Oldcx) || (iImageHeight != Oldcy))
			{
				// The new icons are a different size to the old ones in m_himlToolbar,
				//  so we throw the old ones away.
				ImageList_Destroy(m_himlMenu);
				m_himlMenu = ImageList_Create(iImageWidth, iImageHeight, ILC_COLOR32 | ILC_MASK, iImages, 0);
				ImageList_Destroy(m_himlMenuDis);
				m_himlMenuDis = NULL;
				m_MenuData.clear();
				TRACE(_T("WARNING: Discarded old menu icons\n"));
			}
		}

		// Add the resource IDs to the m_MenuData vector
		for (unsigned int j = 0 ; j < MenuData.size(); ++j)
		{
			if (MenuData[j] != 0)
			{
				m_MenuData.push_back(MenuData[j]);
			}
		}

		// Add the images to the ImageList
		ImageList_AddMasked(m_himlMenu, hbm, crMask);
		::DeleteObject(hbm);

		// Create the Disabled imagelist
		if (ToolbarDisabledID)
		{
			if (0 != m_himlMenuDis)
				m_himlMenuDis = ImageList_Create(iImageWidth, iImageHeight, ILC_COLOR32 | ILC_MASK, iImages, 0);

			HBITMAP hbmDis = LoadBitmap(MAKEINTRESOURCE(ToolbarDisabledID));

			BITMAP bmDis = {0};
			::GetObject(hbmDis, sizeof(BITMAP), &bmDis);
			int iImageWidthDis  = bmDis.bmWidth / iImages;
			int iImageHeightDis = bmDis.bmHeight;

			// Normal and Disabled icons must be the same size
			if ((iImageWidthDis == iImageWidth) && (iImageHeightDis == iImageHeight))
			{
				ImageList_AddMasked(m_himlMenu, hbmDis, crMask);
			}
			else
			{
				ImageList_Destroy(m_himlMenuDis);
				m_himlMenuDis = CreateDisabledImageList(m_himlMenu);
				TRACE(_T("WARNING:  Using default disabled menu icons\n"));
			}
			::DeleteObject(hbmDis);
		}
		else
		{
			if (m_himlMenuDis) ImageList_Destroy(m_himlMenuDis);
			m_himlMenuDis = CreateDisabledImageList(m_himlMenu);
		}

		// return the number of menu icons
		return (int)m_MenuData.size();
	}

	inline void CFrame::AddMenubarBand()
	{
		// Adds a Menubar to the rebar control
		REBARBANDINFO rbbi = {0};
		CSize sz = GetMenubar().GetMaxSize();

		// Calculate the Menubar height from the menu font
		CSize csMenubar;
		NONCLIENTMETRICS nm = {0};
		nm.cbSize = sizeof (NONCLIENTMETRICS);
		SystemParametersInfo (SPI_GETNONCLIENTMETRICS, 0, &nm, 0);
		LOGFONT lf = nm.lfMenuFont;
		CDC dcFrame = GetDC();
		dcFrame.CreateFontIndirect(&lf);
		::GetTextExtentPoint32(dcFrame, _T("\tSomeText"), lstrlen(_T("\tSomeText")), &csMenubar);
		int Menubar_Height = csMenubar.cy + 8;

		rbbi.cbSize     = sizeof(REBARBANDINFO);
		rbbi.fMask      = RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_CHILD | RBBIM_SIZE;
		rbbi.cxMinChild = sz.cx;
		rbbi.cx         = sz.cx;
		rbbi.cyMinChild = Menubar_Height;
		rbbi.cyMaxChild = Menubar_Height;
		rbbi.fStyle     = RBBS_BREAK | RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS ;
		rbbi.hwndChild  = GetMenubar();

		GetRebar().InsertBand(-1, rbbi);
		SetMenubarBandSize();
		GetRebar().SetMenubar(GetMenubar());
	}

	inline void CFrame::AddMRUEntry(LPCTSTR szMRUEntry)
	{
		// Erase duplicate entries from vector
		for (int i = (int)m_vMRUEntries.size() -1; i >= 0; --i)
		{
			if (m_vMRUEntries[i] == szMRUEntry)
				m_vMRUEntries.erase(m_vMRUEntries.begin() + i);
		}

		// Insert the entry at the beginning of the vector
		m_vMRUEntries.insert(m_vMRUEntries.begin(), szMRUEntry);

		// Delete excessive MRU entries
		if (m_vMRUEntries.size() > m_nMaxMRU)
			m_vMRUEntries.erase(m_vMRUEntries.begin() + m_nMaxMRU, m_vMRUEntries.end());

		UpdateMRUMenu();
	}

	inline void CFrame::AddToolbarBand(CToolbar& TB)
	{
		// Adds a Toolbar to the rebar control

		// Create the Toolbar Window
		TB.Create(GetRebar());

		// Fill the REBARBAND structure
		REBARBANDINFO rbbi = {0};
		CSize sz = TB.GetMaxSize();

		rbbi.cbSize     = sizeof(REBARBANDINFO);
		rbbi.fMask      = RBBIM_CHILDSIZE | RBBIM_STYLE |  RBBIM_CHILD | RBBIM_SIZE;
		rbbi.cyMinChild = sz.cy;
		rbbi.cyMaxChild = sz.cy;
		rbbi.cx         = sz.cx +2;
		rbbi.cxMinChild = sz.cx +2;

		rbbi.fStyle     = RBBS_BREAK | RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS;
		rbbi.hwndChild  = TB;

		GetRebar().InsertBand(-1, rbbi);
	}

	inline void CFrame::AddToolbarButton(UINT nID)
	// Adds Resource IDs to toolbar buttons.
	// A resource ID of 0 is a separator
	{
		m_ToolbarData.push_back(nID);
	}

	inline void CFrame::AdjustFrameRect(RECT rcView) const
	// Adjust the size of the frame to accommodate the View window's dimensions
	{
		// Adjust for the view styles
		CRect rc = rcView;
		DWORD dwStyle = (DWORD)GetView()->GetWindowLongPtr(GWL_STYLE);
		DWORD dwExStyle = (DWORD)GetView()->GetWindowLongPtr(GWL_EXSTYLE);
		AdjustWindowRectEx(&rc, dwStyle, FALSE, dwExStyle);
		
		// Calculate the new frame height
		CRect rcFrameBefore = GetWindowRect();
		CRect rcViewBefore = GetViewRect();
		int Height = rc.Height() + rcFrameBefore.Height() - rcViewBefore.Height();

		// Adjust for the frame styles
		dwStyle = (DWORD)GetWindowLongPtr(GWL_STYLE);
		dwExStyle = (DWORD)GetWindowLongPtr(GWL_EXSTYLE);
		AdjustWindowRectEx(&rc, dwStyle, FALSE, dwExStyle);

		// Calculate final rect size, and reposition frame 
		SetWindowPos(NULL, 0, 0, rc.Width(), Height, SWP_NOMOVE);
	}

	inline void CFrame::DrawCheckmark(LPDRAWITEMSTRUCT pdis)
	// Draws the checkmark or radiocheck transparently
	{
		CDC DrawDC = pdis->hDC;
		CRect rc = pdis->rcItem;
		UINT fType = ((ItemData*)pdis->itemData)->fType;

		// Draw the checkmark's background rectangle first
		if (m_ThemeMenu.UseThemes)
		{
			int Iconx = 0, Icony = 0;
			ImageList_GetIconSize(m_himlMenu, &Iconx, &Icony);
			int offset = -1 + (rc.bottom - rc.top - Icony)/2;
			int height = rc.bottom - rc.top;
			CRect rcBk;
			rcBk.SetRect(rc.left, rc.top, rc.left + height, rc.bottom);
			rcBk.InflateRect( -offset, -offset );
			DrawDC.CreateSolidBrush(m_ThemeMenu.clrHot2);
			DrawDC.CreatePen(PS_SOLID, 1, m_ThemeMenu.clrOutline);

			// Draw the checkmark's background rectangle
			::Rectangle(DrawDC, rcBk.left, rcBk.top, rcBk.right, rcBk.bottom);
		}

		CDC MemDC = ::CreateCompatibleDC(pdis->hDC);
		int cxCheck = ::GetSystemMetrics(SM_CXMENUCHECK);
		int cyCheck = ::GetSystemMetrics(SM_CYMENUCHECK);
		MemDC.CreateBitmap(cxCheck, cyCheck, 1, 1, NULL);
		CRect rCheck( 0, 0, cxCheck, cyCheck);

		// Copy the check mark bitmap to hdcMem
		if (MFT_RADIOCHECK == fType)
			::DrawFrameControl(MemDC, &rCheck, DFC_MENU, DFCS_MENUBULLET);
		else
			::DrawFrameControl(MemDC, &rCheck, DFC_MENU, DFCS_MENUCHECK);

		int offset = (rc.bottom - rc.top - ::GetSystemMetrics(SM_CXMENUCHECK))/2;
		if (m_ThemeMenu.UseThemes)
			rc.left += 2;

		// Draw a white or black check mark as required
		// Unfortunately MaskBlt isn't supported on Win95, 98 or ME, so we do it the hard way
		CDC MaskDC = ::CreateCompatibleDC(pdis->hDC);
		MaskDC.CreateCompatibleBitmap(pdis->hDC, cxCheck, cyCheck);

		::BitBlt(MaskDC, 0, 0, cxCheck, cyCheck, MaskDC, 0, 0, WHITENESS);
		if ((pdis->itemState & ODS_SELECTED) && (!m_ThemeMenu.UseThemes))
		{
			// Draw a white checkmark
			::BitBlt(MemDC, 0, 0, cxCheck, cyCheck, MemDC, 0, 0, DSTINVERT);
			::BitBlt(MaskDC, 0, 0, cxCheck, cyCheck, MemDC, 0, 0, SRCAND);
			::BitBlt(DrawDC, rc.left + offset, rc.top + offset, cxCheck, cyCheck, MaskDC, 0, 0, SRCPAINT);
		}
		else
		{
			// Draw a black checkmark
			int BullitOffset = ((MFT_RADIOCHECK == fType) && m_ThemeMenu.UseThemes)? 1 : 0;
			::BitBlt(MaskDC, -BullitOffset, BullitOffset, cxCheck, cyCheck, MemDC, 0, 0, SRCAND);
			::BitBlt(DrawDC, rc.left + offset, rc.top + offset, cxCheck, cyCheck, MaskDC, 0, 0, SRCAND);
		}
		// Detach the DC so it doesn't get destroyed
		DrawDC.DetachDC();
	}

	inline void CFrame::DrawMenuIcon(LPDRAWITEMSTRUCT pdis, BOOL bDisabled)
	{
		if (!m_himlMenu)
			return;
		// Get icon size
		int Iconx;
		int Icony;
		ImageList_GetIconSize(m_himlMenu, &Iconx, &Icony);

		// get the drawing rectangle
		CDC DrawDC = pdis->hDC;
		CRect rc = pdis->rcItem;
		int offset = (rc.bottom - rc.top - Icony)/2;
		int height = rc.bottom - rc.top;
		rc.SetRect(rc.left, rc.top, rc.left + height, rc.bottom);
		rc.InflateRect( -offset, -offset);

		// get the icon's location in the imagelist
		int iImage = -1;
		for (UINT i = 0 ; i < m_MenuData.size(); ++i)
		{
			if (pdis->itemID == m_MenuData[i])
				iImage = i;
		}

		// draw the image
		if (iImage >= 0 )
		{
			if ((bDisabled) && (m_himlMenuDis))
				ImageList_Draw(m_himlMenuDis, iImage, DrawDC, rc.left, rc.top, ILD_TRANSPARENT);
			else
				ImageList_Draw(m_himlMenu, iImage, DrawDC, rc.left, rc.top, ILD_TRANSPARENT);
		}
		// Detach the DC so it doesn't get destroyed
		DrawDC.DetachDC();
	}

	inline void CFrame::DrawMenuText(CDC& DrawDC, LPCTSTR ItemText, CRect& rc, COLORREF colorText)
	{
		// find the position of tab character
		int nTab = -1;
		for(int i = 0; i < lstrlen(ItemText); ++i)
		{
			if(_T('\t') == ItemText[i])
			{
				nTab = i;
				break;
			}
		}

		// Draw the item text
		::SetTextColor(DrawDC, colorText);
		::DrawText(DrawDC, ItemText, nTab, &rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

		// Draw text after tab, right aligned
		if(nTab != -1)
			::DrawText(DrawDC, &ItemText[nTab + 1], -1, &rc, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
	}

	inline int CFrame::GetMenuItemPos(HMENU hMenu, LPCTSTR szItem)
	// Returns the position of the menu item, given it's name
	{
		int nMenuItemCount = GetMenuItemCount(hMenu);
		MENUITEMINFO mii = {0};

		// For Win95 and NT, cbSize needs to be 44
		if (1400 == (GetWinVersion()) || (2400 == GetWinVersion()))
			mii.cbSize = 44;
		else
			mii.cbSize = sizeof(MENUITEMINFO);

		for (int nItem = 0 ; nItem < nMenuItemCount; ++nItem)
		{
			TCHAR szStr[MAX_MENU_STRING +1] = _T("");
			TCHAR szStripped[MAX_MENU_STRING +1] = _T("");

			mii.fMask      = MIIM_TYPE;
			mii.fType      = MFT_STRING;
			mii.dwTypeData = szStr;
			mii.cch        = MAX_MENU_STRING;

			// Fill the contents of szStr from the menu item
			if (::GetMenuItemInfo(hMenu, nItem, TRUE, &mii) && (lstrlen(szStr) <= MAX_MENU_STRING))
			{
				// Strip out any & characters
				int j = 0;
				for (int i = 0; i < lstrlen(szStr); ++i)
				{
					if (szStr[i] != _T('&'))
						szStripped[j++] = szStr[i];
				}
				szStripped[j] = _T('\0');	// Append null tchar

				// Compare the strings
				if (0 == lstrcmp(szStripped, szItem))
					return nItem;
			}
		}

		return -1;
	}

	inline tString CFrame::GetMRUEntry(int nIndex)
	{
		tString tsPathName;
		if (nIndex < (int)m_vMRUEntries.size())
		{
			tsPathName = m_vMRUEntries[nIndex];

			// Now put the selected entry at Index 0
			AddMRUEntry(tsPathName.c_str());
		}
		return tsPathName;
	}

	inline CRect CFrame::GetViewRect() const
	{
		// Get the frame's client area
		CRect rcFrame = GetClientRect();

		// Get the statusbar's window area
		CRect rcStatus;
		if (GetStatusbar().IsVisible())
			rcStatus = GetStatusbar().GetWindowRect();

		// Get the top rebar or toolbar's window area
		CRect rcTop;
		if (IsRebarSupported() && m_bUseRebar)
			rcTop = GetRebar().GetWindowRect();
		else
			if (GetToolbar().IsVisible())
				rcTop = GetToolbar().GetWindowRect();

		// Return client size less the rebar and status windows
		int top = rcFrame.top + rcTop.Height();
		int left = rcFrame.left;
		int right = rcFrame.right;
		int bottom = rcFrame.Height() - (rcStatus.Height());
		if ((bottom <= top) ||( right <= left))
			top = left = right = bottom = 0;

		CRect rcView(left, top, right, bottom);
		return rcView;
	} // CRect CFrame::GetViewRect()

	inline void CFrame::LoadCommonControls(INITCOMMONCONTROLSEX InitStruct)
	{
		HMODULE hComCtl;

		try
		{
			// Load the Common Controls DLL
			hComCtl = ::LoadLibrary(_T("COMCTL32.DLL"));
			if (!hComCtl)
				throw CWinException(_T("CFrame::LoadCommonControls ... Failed to load COMCTL32.DLL"));

			if (GetComCtlVersion() > 470)
			{
				// Declare a pointer to the InItCommonControlsEx function
				typedef BOOL WINAPI INIT_EX(INITCOMMONCONTROLSEX*);
				INIT_EX* pfnInit = (INIT_EX*)::GetProcAddress(hComCtl, "InitCommonControlsEx");

				// Call InitCommonControlsEx
				if(!((*pfnInit)(&InitStruct)))
					throw CWinException(_T("CFrame::LoadCommonControls ... InitCommonControlsEx failed"));
			}
			else
			{
				::InitCommonControls();
			}

			::FreeLibrary(hComCtl);
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
			if (hComCtl)
				::FreeLibrary(hComCtl);
		}
	}

	inline void CFrame::LoadRegistryMRUSettings(UINT nMaxMRU /*= 0*/)
	{
		try
		{
			if (m_tsKeyName.empty())
				throw CWinException(_T("KeyName must be set before calling LoadRegistryMRUSettings"));

			// Load the MRU from the registry
			m_nMaxMRU = min(nMaxMRU, 16);
			tString tsKey = _T("Software\\") + m_tsKeyName + _T("\\Recent Files");
			HKEY hKey = NULL;

			if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, tsKey.c_str(), 0, KEY_READ, &hKey))
			{
				for (UINT i = 0; i < m_nMaxMRU; ++i)
				{
					DWORD dwType = REG_SZ;
					DWORD dwBufferSize = 0;
					TCHAR szSubKey[10] = _T("");
					wsprintf(szSubKey, _T("File %d\0"), i+1);

					RegQueryValueEx(hKey, szSubKey, NULL, &dwType, NULL, &dwBufferSize);
					TCHAR* szPathName = new TCHAR[dwBufferSize];
					if (NULL == szPathName) throw std::bad_alloc();

					// load the entry from the registry
					if (ERROR_SUCCESS == RegQueryValueEx(hKey, szSubKey, NULL, &dwType, (LPBYTE)szPathName, &dwBufferSize))
					{
						if (lstrlen(szPathName))
							m_vMRUEntries.push_back(szPathName);
					}

					delete []szPathName;
				}
			}
		}

		catch(const CWinException& e)
		{
			e.MessageBox();
		}

		catch(std::bad_alloc)
		{
			::MessageBox(NULL, _T("Memory allocation error in LoadRegistryMRUSettings"), _T("Error"), MB_ICONEXCLAMATION | MB_OK);
		}
	}

	inline void CFrame::LoadRegistrySettings(LPCTSTR szKeyName)
	{
		m_tsKeyName = szKeyName;

		tString tsKey = _T("Software\\") + m_tsKeyName + _T("\\Position");
		HKEY hKey = 0;
		RegOpenKeyEx(HKEY_CURRENT_USER, tsKey.c_str(), 0, KEY_READ, &hKey);
		if (hKey)
		{
			DWORD dwType = REG_BINARY;
			DWORD BufferSize = sizeof(DWORD);
			DWORD dwTop, dwLeft, dwWidth, dwHeight;
			RegQueryValueEx(hKey, _T("Top"), NULL, &dwType, (LPBYTE)&dwTop, &BufferSize);
			RegQueryValueEx(hKey, _T("Left"), NULL, &dwType, (LPBYTE)&dwLeft, &BufferSize);
			RegQueryValueEx(hKey, _T("Width"), NULL, &dwType, (LPBYTE)&dwWidth, &BufferSize);
			RegQueryValueEx(hKey, _T("Height"), NULL, &dwType, (LPBYTE)&dwHeight, &BufferSize);

			// Get current desktop size to ensure reasonable a window position
			CRect rcDesktop;
			SystemParametersInfo(SPI_GETWORKAREA, 0, &rcDesktop, 0);
			m_rcPosition.top = min(dwTop, (UINT)rcDesktop.bottom - 30);
			m_rcPosition.left = min(dwLeft, (UINT)rcDesktop.right - 90);
			m_rcPosition.bottom = m_rcPosition.top + dwHeight;
			m_rcPosition.right = m_rcPosition.left + dwWidth;

			RegCloseKey(hKey);
		}
	}

	inline void CFrame::OnCreate()
	{
		// This is called when the frame window is being created.
		// Override this in CMainFrame if you wish to modify what happens here

		// Set the icon
		SetIconLarge(IDW_MAIN);
		SetIconSmall(IDW_MAIN);

		// Set the menu
		SetFrameMenu(IDW_MAIN);

		if (IsRebarSupported() && m_bUseRebar)
		{
			// Create the rebar
			GetRebar().Create(m_hWnd);

			// Create the menu inside rebar
			GetMenubar().Create(GetRebar());
			GetMenubar().SetMenu(m_hMenu);
			AddMenubarBand();

			// Create the toolbar inside rebar
			AddToolbarBand(GetToolbar());
		}
		else
		{
			// Create the toolbar without a rebar
			GetToolbar().Create(m_hWnd);
		}

		UpdateMRUMenu();

		// Set the toolbar data
		int iButtons = GetToolbar().SetButtons(m_ToolbarData);

		// Set the toolbar images
		// A mask of 192,192,192 is compatible with AddBitmap (for Win95)
		SetToolbarImages(GetToolbar(), iButtons, RGB(192,192,192), IDW_MAIN, 0, 0);

		// Set the icons for popup menu items
		SetMenuIcons(m_ToolbarData, RGB(192, 192, 192), IDW_MAIN, 0);

		if (!IsMenubarUsed())
			::SetMenu(m_hWnd, m_hMenu);

		if (m_bUseThemes)
			SetTheme();

		// Create the status bar
		GetStatusbar().Create(m_hWnd);

		// Create the view window
		if (NULL == m_pWndView)
			throw CWinException(_T("CFrame::OnCreate ... View window is not assigned!\nUse SetView to set the View Window"));
		m_pWndView->Create(m_hWnd);

		// Reposition the child windows
		RecalcLayout();

		// Start timer for Status updates
		if (m_bShowIndicatorStatus || m_bShowMenuStatus)
			::SetTimer(m_hWnd, ID_STATUS_TIMER, 200, NULL);
	}

	inline void CFrame::OnFrameDrawItem(WPARAM /*wParam*/, LPARAM lParam)
	// OwnerDraw is used to render the popup menu items
	{
		LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT) lParam;

		CRect rc = pdis->rcItem;
		ItemData* pmd = (ItemData*)pdis->itemData;
		CDC DrawDC = pdis->hDC;

		int Iconx = 0;
		int Icony = 0;
		ImageList_GetIconSize(m_himlMenu, &Iconx, &Icony);
		int BarWidth = m_ThemeMenu.UseThemes? Iconx + 6 : 0;

		// Draw the side bar
		if (m_ThemeMenu.UseThemes)
		{
			CRect rcBar = rc;
			rcBar.right = BarWidth;
			GradientFill(DrawDC, m_ThemeMenu.clrPressed1, m_ThemeMenu.clrPressed2, &rcBar, TRUE);
		}

		if (pmd->fType & MFT_SEPARATOR)
		{
			// draw separator
			CRect rcSep = rc;
			rcSep.left = BarWidth;
			if (m_ThemeMenu.UseThemes)
				SolidFill(DrawDC, RGB(255,255,255), &rcSep);
			else
				SolidFill(DrawDC, GetSysColor(COLOR_MENU), &rcSep);
			rcSep.top += (rc.bottom - rc.top)/2;
			rcSep.left = BarWidth + 2;
			::DrawEdge(DrawDC, &rcSep,  EDGE_ETCHED, BF_TOP);
		}
		else
		{
			// draw menu item
			BOOL bDisabled = pdis->itemState & ODS_GRAYED;
			BOOL bSelected = pdis->itemState & ODS_SELECTED;
			BOOL bChecked  = pdis->itemState & ODS_CHECKED;
			CRect rcDraw = rc;

			if ((bSelected) && (!bDisabled))
			{
				// draw selected item background
				if (m_ThemeMenu.UseThemes)
				{
					DrawDC.CreateSolidBrush(m_ThemeMenu.clrHot1);
					DrawDC.CreatePen(PS_SOLID, 1, m_ThemeMenu.clrOutline);
					Rectangle(DrawDC, rcDraw.left, rcDraw.top, rcDraw.right, rcDraw.bottom);
				}
				else
					SolidFill(DrawDC, GetSysColor(COLOR_HIGHLIGHT), &rcDraw);
			}
			else
			{
				// draw non-selected item background
				rcDraw.left = BarWidth;
				if (m_ThemeMenu.UseThemes)
					SolidFill(DrawDC, RGB(255,255,255), &rcDraw);
				else
					SolidFill(DrawDC, GetSysColor(COLOR_MENU), &rcDraw);
			}  

			if (bChecked)
				DrawCheckmark(pdis);
			else
				DrawMenuIcon(pdis, bDisabled);

			// Calculate the text rect size
			rc.left  = rc.bottom - rc.top + 2;
			if (_tcschr(pmd->Text, _T('\t')))
				rc.right -= POST_TEXT_GAP;	// Add POST_TEXT_GAP if the text includes a tab

			// Draw the text
			int iMode = ::SetBkMode(DrawDC, TRANSPARENT);
			COLORREF colorText;
			if (m_ThemeMenu.UseThemes)
			{
				rc.left += 8;
				colorText = GetSysColor(bDisabled ?  COLOR_GRAYTEXT : COLOR_MENUTEXT);
			}
			else
				colorText = GetSysColor(bDisabled ?  COLOR_GRAYTEXT : bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_MENUTEXT);

			DrawMenuText(DrawDC, pmd->Text, rc, colorText);
			::SetBkMode(DrawDC, iMode);
		}
		// Detach the DC so it doesn't get destroyed
		DrawDC.DetachDC();
	}

	inline void CFrame::OnFrameExitMenuLoop()
	{
		for (UINT nItem = 0; nItem < m_vpItemData.size(); ++nItem)
		{
			// Undo OwnerDraw and put the text back
			MENUITEMINFO mii = {0};

			// For Win95 and NT, cbSize needs to be 44
			if ((1400 == GetWinVersion()) || (2400 == GetWinVersion()))
				mii.cbSize = 44;
			else
				mii.cbSize = sizeof(MENUITEMINFO);

			mii.fMask = MIIM_TYPE | MIIM_DATA;
			mii.fType = m_vpItemData[nItem]->fType;
			mii.dwTypeData = m_vpItemData[nItem]->Text;
			mii.cch = lstrlen(m_vpItemData[nItem]->Text);
			mii.dwItemData = 0;
			::SetMenuItemInfo(m_vpItemData[nItem]->hMenu, m_vpItemData[nItem]->nPos, TRUE, &mii);

			// Delete the ItemData object, then erase the vector item
			delete m_vpItemData[nItem];
		}
		m_vpItemData.clear();
	}

	inline void CFrame::OnFrameClose()
	{
		// Called in response to a WM_CLOSE message for the frame.
		// It's called called OnFrameClose to avoid being accidently overriden
		// by a user's possible OnClose function.
		SaveRegistrySettings();

		GetMenubar().Destroy();
		GetToolbar().Destroy();
		GetRebar().Destroy();
		GetStatusbar().Destroy();
		GetView()->Destroy();
	}

	inline BOOL CFrame::OnFrameCommand(WPARAM wParam, LPARAM /*lParam*/)
	{
		// Handle the View Statusbar and Toolbar menu items
		switch (LOWORD(wParam))
		{
		case IDW_VIEW_STATUSBAR:
			OnViewStatusbar();
			return TRUE;
		case IDW_VIEW_TOOLBAR:
			OnViewToolbar();
			return TRUE;
		} // switch cmd

		return FALSE;
	}

	inline void CFrame::OnHelp()
	{
		if (NULL == m_pAboutDialog)
		{
			// Store the window handle that currently has keyboard focus
			HWND hPrevFocus = ::GetFocus();
			if (hPrevFocus == GetMenubar())
				hPrevFocus = m_hWnd;

			m_pAboutDialog = new CDialog(IDW_ABOUT, m_hWnd);
			// Some MS compilers (including VS2003 under some circumstances) return NULL instead of throwing
			//  an exception when new fails. We make sure an exception gets thrown!
			if (NULL == m_pAboutDialog)
				throw std::bad_alloc();

			m_pAboutDialog->DoModal();

			// Clean up
			delete m_pAboutDialog;
			m_pAboutDialog = NULL;
			::SetFocus(hPrevFocus);
		}
	}

	inline void CFrame::OnFrameInitMenuPopup(WPARAM wParam, LPARAM lParam)
	{
		// The system menu shouldn't be owner drawn
		if (HIWORD(lParam)) return;

		HMENU hMenu = (HMENU)wParam;

		for (int i = 0; i < ::GetMenuItemCount(hMenu) ; ++i)
		{
			MENUITEMINFO mii = {0};

			// For Win95 and NT, cbSize needs to be 44
			if ((1400 == GetWinVersion()) || (2400 == GetWinVersion()))
				mii.cbSize = 44;
			else
				mii.cbSize = sizeof(MENUITEMINFO);

			TCHAR szMenuItem[MAX_MENU_STRING] = _T("");

			// Use old fashioned MIIM_TYPE instead of MIIM_FTYPE for MS VC6 compatibility
			mii.fMask  = MIIM_TYPE | MIIM_DATA | MIIM_SUBMENU;
			mii.dwTypeData = szMenuItem;
			mii.cch = MAX_MENU_STRING -1;

			// Specify owner-draw for the menu item type
			if (::GetMenuItemInfo(hMenu, i, TRUE, &mii))
			{
				if (0 == mii.dwItemData)
				{
					ItemData* pItem = new ItemData;		// deleted in OnFrameExitMenuLoop

					// Some MS compilers (including VS2003 under some circumstances) return NULL instead of throwing
					//  an exception when new fails. We make sure an exception gets thrown!
					if (NULL == pItem)
						throw std::bad_alloc();

					ZeroMemory(pItem, sizeof(ItemData));
					pItem->hMenu = hMenu;
					pItem->nPos = i;
					pItem->fType = mii.fType;
					pItem->hSubMenu = mii.hSubMenu;
					mii.fType |= MFT_OWNERDRAW;
					lstrcpyn(pItem->Text, szMenuItem, MAX_MENU_STRING);
					mii.dwItemData = (DWORD_PTR)pItem;

					m_vpItemData.push_back(pItem);			// Store pItem in m_vpItemData
					::SetMenuItemInfo(hMenu, i, TRUE, &mii);// Store pItem in mii
				}
			}
		}
	}

	inline void CFrame::OnFrameMeasureItem(WPARAM /*wParam*/, LPARAM lParam)
	// Called before the Popup menu is displayed, so that the MEASUREITEMSTRUCT
	//  values can be assigned with the menu item's dimensions.
	{
		LPMEASUREITEMSTRUCT pmis = (LPMEASUREITEMSTRUCT) lParam;

		ItemData* pmd = (ItemData *) pmis->itemData;

		if (pmd->fType & MFT_SEPARATOR)
		{
			pmis->itemHeight = 7;
			pmis->itemWidth  = 0;
		}

		else
		{
			CDC DesktopDC = ::GetDC(NULL);

			// Get the font used in menu items
			NONCLIENTMETRICS nm = {0};
			nm.cbSize = sizeof(nm);
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(nm), &nm, 0);
			// Default menu items are bold, so take this into account
			if ((INT)::GetMenuDefaultItem(pmd->hMenu, TRUE, GMDI_USEDISABLED) != -1)
				nm.lfMenuFont.lfWeight = FW_BOLD;

			DesktopDC.CreateFontIndirect(&nm.lfMenuFont);

			// Calculate the size of the text
			CSize size;
			GetTextExtentPoint32(DesktopDC, pmd->Text, lstrlen(pmd->Text), &size);

			// Calculate the size of the icon
			int Iconx = 0;
			int Icony = 0;
			ImageList_GetIconSize(m_himlMenu, &Iconx, &Icony);

			pmis->itemHeight = 2+max(max(size.cy, GetSystemMetrics(SM_CYMENU)-2), Icony+2);
			pmis->itemWidth = size.cx + max(::GetSystemMetrics(SM_CXMENUSIZE), Iconx+2);

			// Allow extra width if the text includes a tab
			if (_tcschr(pmd->Text, _T('\t')))
				pmis->itemWidth += POST_TEXT_GAP;

			// Allow extra width if the menu item has a sub menu
			if (pmd->hSubMenu)
				pmis->itemWidth += 10;

			// Allow extra width for themed menu
			if (m_ThemeMenu.UseThemes)
				pmis->itemWidth += 8;
		}
	}

	inline void CFrame::OnMenuSelect(WPARAM wParam, LPARAM lParam)
	{
		// Set the Statusbar text when we hover over a menu
		// Only popup submenus have status strings
		if (m_bShowMenuStatus)
		{
			int nID = LOWORD (wParam);
			HMENU hMenu = (HMENU) lParam;

			if ((hMenu != ::GetMenu(m_hWnd)) && (nID != 0) && !(HIWORD(wParam) & MF_POPUP))
				m_tsStatusText = LoadString(nID);
			else
				m_tsStatusText = _T("Ready");

			SetStatusText();
		}
	}

	inline LRESULT CFrame::OnFrameNotify(WPARAM /*wParam*/, LPARAM lParam)
	{

		switch (((LPNMHDR)lParam)->code)
		{
		case UWM_UNDOCKED:
			m_hOldFocus = 0;
			break;
		case RBN_HEIGHTCHANGE:
			RecalcLayout();
			::InvalidateRect(m_hWnd, NULL, TRUE);
			break;
		case RBN_LAYOUTCHANGED:
			if (GetRebar().GetRebarTheme().UseThemes && GetRebar().GetRebarTheme().KeepBandsLeft)
				GetRebar().MoveBandsLeft();
			break;
		case RBN_MINMAX:
			if (GetRebar().GetRebarTheme().UseThemes && GetRebar().GetRebarTheme().ShortBands)
				return 1L;	// Supress maximise or minimise rebar band
			break;

		// Display tooltips for the toolbar
		case TTN_GETDISPINFO:
			{
				int iIndex =  GetToolbar().HitTest();
				LPNMTTDISPINFO lpDispInfo = (LPNMTTDISPINFO)lParam;
				if (iIndex >= 0)
				{
					int nID = GetToolbar().GetCommandID(iIndex);
					if (nID > 0) lpDispInfo->lpszText = (LPTSTR)LoadString(nID);
				}
			}
			break;
		} // switch LPNMHDR

		return 0L;

	} // CFrame::OnFrameNotify(...)

	inline void CFrame::OnFrameSetFocus()
	{
		SetStatusText();
	}

	inline void CFrame::OnFrameSysColorChange()
	{
		// Honor theme color changes
		for (int nBand = 0; nBand <= GetRebar().GetBandCount(); ++nBand)
		{
			GetRebar().SetBandColor(nBand, GetSysColor(COLOR_BTNTEXT), GetSysColor(COLOR_BTNFACE));
		}

		// Update the status bar font and text
		NONCLIENTMETRICS nm = {0};
		nm.cbSize = sizeof (NONCLIENTMETRICS);
		SystemParametersInfo (SPI_GETNONCLIENTMETRICS, 0, &nm, 0);
		LOGFONT lf = nm.lfStatusFont;
		HFONT hFontOld = (HFONT)GetStatusbar().SendMessage(WM_GETFONT, 0, 0);
		HFONT hFont = ::CreateFontIndirect(&lf);
		GetStatusbar().SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);
		::DeleteObject(hFontOld);
		SetStatusText();

		// Update the menubar font and band size
		if(IsMenubarUsed())
		{
			// Update the font
			NONCLIENTMETRICS nm = {0};
			nm.cbSize = sizeof (NONCLIENTMETRICS);
			SystemParametersInfo (SPI_GETNONCLIENTMETRICS, 0, &nm, 0);
			LOGFONT lf = nm.lfMenuFont;
			HFONT hFontOld = (HFONT)GetMenubar().SendMessage(WM_GETFONT, 0, 0);
			HFONT hFont = ::CreateFontIndirect(&lf);
			GetMenubar().SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);
			::DeleteObject(hFontOld);

			// Update the band size
			CSize csMenubar;
			CDC dcFrame = GetDC();
			dcFrame.CreateFontIndirect(&lf);
			::GetTextExtentPoint32(dcFrame, _T("\tSomeText"), lstrlen(_T("\tSomeText")), &csMenubar);
			int Menubar_Height = csMenubar.cy + 8;
			int nBand = GetRebar().GetBand(GetMenubar().GetHwnd());
			REBARBANDINFO rbbi = {0};
			rbbi.cbSize = sizeof(REBARBANDINFO);
			rbbi.fMask = RBBIM_CHILDSIZE;
			GetRebar().GetBandInfo(nBand, rbbi);
			rbbi.cyMinChild = Menubar_Height;
			rbbi.cyMaxChild = Menubar_Height;
			GetRebar().SetBandInfo(nBand, rbbi);
		}

		if ((m_bUpdateTheme) && (m_bUseThemes)) SetTheme();

		// Reposition and redraw everything
		RecalcLayout();
		::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
	}

	inline void CFrame::OnFrameTimer(WPARAM wParam)
	{
		if (ID_STATUS_TIMER == wParam)
		{
			if (m_bShowMenuStatus)
			{
				static int nOldID = -1;
				CToolbar& tb = GetToolbar();
				CPoint pt;
				::GetCursorPos(&pt);

				// Is the mouse hovering over the toolbar?
				if (WindowFromPoint(pt) == tb)
				{
					// Which toolbar button is the mouse cursor hovering over?
					int nButton = tb.HitTest();
					if (nButton >= 0)
					{
						int nID = GetToolbar().GetCommandID(nButton);
						// Only update the statusbar if things have changed
						if (nID != nOldID)
						{
							if (nID != 0)
								m_tsStatusText = LoadString(nID);
							else
								m_tsStatusText = _T("Ready");

							SetStatusText();
						}
						nOldID = nID;
					}
				}
				else
				{
					if (nOldID != -1)
					{
						m_tsStatusText = _T("Ready");
						SetStatusText();
					}
					nOldID = -1;
				}
			}

			if (m_bShowIndicatorStatus)
				SetStatusIndicators();
		}
	}

	inline void CFrame::OnViewStatusbar()
	{
		if (::IsWindowVisible(GetStatusbar()))
			::ShowWindow(GetStatusbar(), SW_HIDE);
		else
			::ShowWindow(GetStatusbar(), SW_SHOW);

		UpdateCheckMarks();

		// Reposition the Windows
		RecalcLayout();
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}

	inline void CFrame::OnViewToolbar()
	{
		if (::IsWindowVisible(GetToolbar()))
		{
			if (IsMenubarUsed())
				::SendMessage(GetRebar(), RB_SHOWBAND, GetRebar().GetBand(GetToolbar()), FALSE);
			else
				::ShowWindow(GetToolbar(), SW_HIDE);
		}
		else
		{
			if (IsMenubarUsed())
				::SendMessage(GetRebar(), RB_SHOWBAND, GetRebar().GetBand(GetToolbar()), TRUE);
			else
				::ShowWindow(GetToolbar(), SW_SHOW);
		}

		UpdateCheckMarks();

		// Reposition the Windows
		RecalcLayout();
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}

  	inline void CFrame::PreCreate(CREATESTRUCT& cs)
	{
		// Set the caption from the string resource
		cs.lpszName = LoadString(IDW_MAIN);

		// Set the frame window styles
		cs.style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

		// Set the original window position
		cs.x  = m_rcPosition.left;
		cs.y  = m_rcPosition.top;
		cs.cx = m_rcPosition.Width();
		cs.cy = m_rcPosition.Height();
	}

	inline void CFrame::PreRegisterClass(WNDCLASS &wc)
	{
		// Set the Window Class
		wc.lpszClassName =  _T("Win32++ Frame");
	}

	inline BOOL CFrame::PreTranslateMessage(MSG* pMsg)
	{
		HACCEL hAccelTable = ::LoadAccelerators(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_MAIN));
		if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
		{
			if (TranslateAccelerator(m_hWnd, hAccelTable, pMsg))
				return TRUE;
		}
		return CWnd::PreTranslateMessage(pMsg);
	}

	inline void CFrame::RecalcLayout()
	{
		if ((!m_pWndView) || (!m_pWndView->GetHwnd()))
			return;

		// Resize the status bar
		if (GetStatusbar())
		{
			GetStatusbar().SendMessage(WM_SIZE, 0, 0);
			GetStatusbar().Invalidate();
		}

		// Reposition the text
		SetStatusText();

		// Resize the rebar or toolbar
		if (IsRebarUsed())
		{
			GetRebar().SendMessage(WM_SIZE, 0, 0);
			GetRebar().Invalidate();
		}
		else
			GetToolbar().SendMessage(TB_AUTOSIZE, 0, 0);

		// Resize the View window
		CRect rClient = GetViewRect();

		if ((rClient.bottom - rClient.top) >= 0)
		{
			int x  = rClient.left;
			int y  = rClient.top;
			int cx = rClient.Width();
			int cy = rClient.Height();

			m_pWndView->SetWindowPos( NULL, x, y, cx, cy, SWP_SHOWWINDOW );
		}

		if (GetRebar().GetRebarTheme().UseThemes && GetRebar().GetRebarTheme().KeepBandsLeft)
			GetRebar().MoveBandsLeft();

		if (IsMenubarUsed())
			SetMenubarBandSize();

		::SendMessage(m_hWnd, UWM_REARRANGED, 0, 0);
	}

	inline void CFrame::RemoveMRUEntry(LPCTSTR szMRUEntry)
	{
		for (int i = (int)m_vMRUEntries.size() -1; i >= 0; --i)
		{
			if (m_vMRUEntries[i] == szMRUEntry)
				m_vMRUEntries.erase(m_vMRUEntries.begin() + i);
		}

		UpdateMRUMenu();
	}

	inline void CFrame::SaveRegistrySettings()
	{
		// Store the window position in the registry
		if (!m_tsKeyName.empty())
		{
			WINDOWPLACEMENT Wndpl = {0};
			Wndpl.length = sizeof(WINDOWPLACEMENT);
			if (GetWindowPlacement(Wndpl))
			{
				// Get the Frame's window position
				CRect rc = Wndpl.rcNormalPosition;
				tString tsKeyName = _T("Software\\") + m_tsKeyName + _T("\\Position");;
				HKEY hKey = NULL;
				DWORD dwTop = max(rc.top, 0);
				DWORD dwLeft = max(rc.left, 0);
				DWORD dwWidth = max(rc.Width(), 100);
				DWORD dwHeight = max(rc.Height(), 50);

				if (RegCreateKeyEx(HKEY_CURRENT_USER, tsKeyName.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL))
					throw (CWinException(_T("RegCreateKeyEx Failed")));

				if (RegSetValueEx(hKey, _T("Top"), 0, REG_DWORD, (LPBYTE)&dwTop, sizeof(DWORD)))
					throw (CWinException(_T("RegSetValueEx Failed")));

				if (RegSetValueEx(hKey, _T("Left"), 0, REG_DWORD, (LPBYTE)&dwLeft, sizeof(DWORD)))
					throw (CWinException(_T("RegSetValueEx Failed")));

				if (RegSetValueEx(hKey, _T("Width"), 0, REG_DWORD, (LPBYTE)&dwWidth, sizeof(DWORD)))
					throw (CWinException(_T("RegSetValueEx Failed")));

				if (RegSetValueEx(hKey, _T("Height"), 0, REG_DWORD, (LPBYTE)&dwHeight, sizeof(DWORD)))
					throw (CWinException(_T("RegSetValueEx Failed")));

				RegCloseKey(hKey);
			}

			// Store the MRU entries in the registry
			if (m_nMaxMRU > 0)
			{
				tString tsKeyName = _T("Software\\") + m_tsKeyName + _T("\\Recent Files");
				HKEY hKey = NULL;

				if (RegCreateKeyEx(HKEY_CURRENT_USER, tsKeyName.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL))
					throw (CWinException(_T("RegCreateKeyEx Failed")));

				for (UINT i = 0; i < m_nMaxMRU; ++i)
				{
					TCHAR szSubKey[10];
					wsprintf(szSubKey, _T("File %d\0"), i+1);
					tString tsPathName;
					if (i < m_vMRUEntries.size())
						tsPathName = m_vMRUEntries[i];

					if (RegSetValueEx(hKey, szSubKey, 0, REG_SZ, (LPBYTE)tsPathName.c_str(), (1 + lstrlen(tsPathName.c_str()))*sizeof(TCHAR)))
						throw (CWinException(_T("RegSetValueEx Failed")));
				}

				RegCloseKey(hKey);
			}
		}
	}

	inline void CFrame::SetFrameMenu(INT ID_MENU)
	{
		if (m_hMenu)
			::DestroyMenu(m_hMenu);

		m_hMenu = ::LoadMenu(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(ID_MENU));

		if(!m_hMenu)
			TRACE(_T("Load Menu failed\n"));
 	}

	inline int CFrame::SetMenuIcons(const std::vector<UINT>& MenuData, COLORREF crMask, UINT ToolbarID, UINT ToolbarDisabledID)
	{
		// Remove any existing menu icons
		if (m_himlMenu) ImageList_Destroy(m_himlMenu);
		if (m_himlMenuDis) ImageList_Destroy(m_himlMenuDis);
		m_himlMenu = NULL;
		m_himlMenuDis = NULL;
		m_MenuData.clear();

		// Exit if no ToolbarID is specified
		if (ToolbarID == 0) return 0;

		// Add the menu icons from the bitmap IDs
		return AddMenuIcons(MenuData, crMask, ToolbarID, ToolbarDisabledID);
	}

	inline void CFrame::SetMenuTheme(ThemeMenu& Theme)
	{
		m_ThemeMenu.UseThemes   = Theme.UseThemes;
		m_ThemeMenu.clrHot1     = Theme.clrHot1;
		m_ThemeMenu.clrHot2     = Theme.clrHot2;
		m_ThemeMenu.clrPressed1 = Theme.clrPressed1;
		m_ThemeMenu.clrPressed2 = Theme.clrPressed2;
		m_ThemeMenu.clrOutline  = Theme.clrOutline;

		::InvalidateRect(m_hWnd, NULL, TRUE);
	}

	inline void CFrame::SetMenubarBandSize()
	{
		// Sets the minimum width of the Menubar band to the width of the rebar
		// This prevents other bands from moving to this Menubar's row.

		CRect rcClient = GetClientRect();
		CRebar& RB = GetRebar();
		int nBand = RB.GetBand(GetMenubar());
		CRect rcBorder = RB.GetBandBorders(nBand);
		int Width = rcClient.Width() - rcBorder.Width() - 2;

		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof(REBARBANDINFO);
		rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_SIZE;
		RB.GetBandInfo(nBand, rbbi);
		if (GetRebar().GetRebarTheme().UseThemes)
		{
			rbbi.cxMinChild = Width;
			rbbi.cx         = Width;
		}

		RB.SetBandInfo(nBand, rbbi); 
	}

	inline void CFrame::SetStatusIndicators()
	{
		if (::IsWindow(GetStatusbar()))
		{
			LPCTSTR Status1 = (::GetKeyState(VK_CAPITAL) & 0x0001)? _T("\tCAP") : _T("");
			LPCTSTR Status2 = (::GetKeyState(VK_NUMLOCK) & 0x0001)? _T("\tNUM") : _T("");
			LPCTSTR Status3 = (::GetKeyState(VK_SCROLL)  & 0x0001)? _T("\tSCRL"): _T("");

			// Only update indictors if the text has changed
			if (Status1 != m_OldStatus[0]) 	GetStatusbar().SetPartText(1, (Status1));
			if (Status2 != m_OldStatus[1])  GetStatusbar().SetPartText(2, (Status2));
			if (Status3 != m_OldStatus[2])  GetStatusbar().SetPartText(3, (Status3));

			m_OldStatus[0] = Status1;
			m_OldStatus[1] = Status2;
			m_OldStatus[2] = Status3;
		}
	}

	inline void CFrame::SetStatusText()
	{
		if (::IsWindow(GetStatusbar()))
		{
			// Calculate the width of the text indicators
			CDC dcStatus = GetStatusbar().GetDC();
			CSize csCAP, csNUM, csSCRL;
			::GetTextExtentPoint32(dcStatus, _T("\tCAP"), lstrlen(_T("\tCAP")), &csCAP);
			::GetTextExtentPoint32(dcStatus, _T("\tNUM"), lstrlen(_T("\tNUM")), &csNUM);
			::GetTextExtentPoint32(dcStatus, _T("\tSCRL"), lstrlen(_T("\tSCRL")), &csSCRL);

			// Get the coordinates of the parent window's client area.
			CRect rcClient = GetClientRect();

			int width = max(300, rcClient.right);

			if (m_bShowIndicatorStatus)
			{
				// Create 4 panes
				GetStatusbar().SetPartWidth(0, width - (csCAP.cx+csNUM.cx+csSCRL.cx+20));
				GetStatusbar().SetPartWidth(1, csCAP.cx);
				GetStatusbar().SetPartWidth(2, csNUM.cx);
				GetStatusbar().SetPartWidth(3, csSCRL.cx);

				SetStatusIndicators();
			}

			// Place text in the 1st pane
			GetStatusbar().SetPartText(0, m_tsStatusText.c_str());
		}
	}

	inline void CFrame::SetTheme()
	{
		// Note: To modify theme colors, override this function in CMainframe,
		//        and make any modifications there.

		// Avoid themes if using less than 16 bit colors
		CDC DesktopDC = ::GetDC(NULL);
		if (::GetDeviceCaps(DesktopDC, BITSPIXEL) < 16)
			return;

		// Set a flag redo SetTheme when the theme changes
		m_bUpdateTheme = TRUE;

		// Detect the XP theme name
		WCHAR Name[30] = L"";
		HMODULE hMod = ::LoadLibrary(_T("uxtheme.dll"));
		if(hMod)
		{
			typedef HRESULT (__stdcall *PFNGETCURRENTTHEMENAME)(LPWSTR pszThemeFileName, int cchMaxNameChars,
				LPWSTR pszColorBuff, int cchMaxColorChars, LPWSTR pszSizeBuff, int cchMaxSizeChars);

			PFNGETCURRENTTHEMENAME pfn = (PFNGETCURRENTTHEMENAME)GetProcAddress(hMod, "GetCurrentThemeName");

			(*pfn)(0, 0, Name, 30, 0, 0);

			::FreeLibrary(hMod);
		}

		enum Themetype{ Grey, Blue, Silver, Olive };

		int Theme = Grey;
		if (0 == wcscmp(L"NormalColor", Name))	Theme = Blue;
		if (0 == wcscmp(L"Metallic", Name))		Theme = Silver;
		if (0 == wcscmp(L"HomeStead", Name))	Theme = Olive;

		BOOL T = TRUE;
		BOOL F = FALSE;

		switch (Theme)
		{
		case Grey:	// A color scheme suitable for 16 bit colors. Suitable for Windows older than XP.
			{
				ThemeToolbar tt = {T, RGB(182, 189, 210), RGB(182, 189, 210), RGB(133, 146, 181), RGB(133, 146, 181), RGB(10, 36, 106)};
				ThemeRebar tr = {T, RGB(212, 208, 200), RGB(212, 208, 200), RGB(230, 226, 222), RGB(220, 218, 208), F, T, T, T, T, F};
				ThemeMenu tm = {T, RGB(182, 189, 210), RGB( 182, 189, 210), RGB(200, 196, 190), RGB(200, 196, 190), RGB(100, 100, 100)};

				GetToolbar().SetToolbarTheme(tt);
				SetMenuTheme(tm); // Sets the theme for popup menus
				if (IsRebarUsed())
				{
					GetRebar().SetRebarTheme(tr);
					GetMenubar().SetMenubarTheme(tm); // Sets the theme for Menubar buttons
				}
			}
			break;
		case Blue:
			{
				// Used for XP default (blue) color scheme
				ThemeToolbar tt = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 140, 40), RGB(255, 180, 80), RGB(192, 128, 255)};
				ThemeRebar tr = {T, RGB(150,190,245), RGB(196,215,250), RGB(220,230,250), RGB( 70,130,220), F, T, T, T, T, F};
				ThemeMenu tm = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(220,230,250), RGB(150,190,245), RGB(128, 128, 200)};

				GetToolbar().SetToolbarTheme(tt);
				SetMenuTheme(tm); // Sets the theme for popup menus
				if (IsRebarUsed())
				{
					GetRebar().SetRebarTheme(tr);
					GetMenubar().SetMenubarTheme(tm); // Sets the theme for Menubar buttons
				}
			}
			break;

		case Silver:
			{
				// Used for XP Silver color scheme
				ThemeToolbar tt = {T, RGB(192, 210, 238), RGB(192, 210, 238), RGB(152, 181, 226), RGB(152, 181, 226), RGB(49, 106, 197)};
				ThemeRebar tr = {T, RGB(225, 220, 240), RGB(240, 240, 245), RGB(245, 240, 255), RGB(160, 155, 180), F, T, T, T, T, F};
				ThemeMenu tm = {T, RGB(196, 215, 250), RGB( 120, 180, 220), RGB(240, 240, 245), RGB(170, 165, 185), RGB(128, 128, 150)};

				GetToolbar().SetToolbarTheme(tt);
				SetMenuTheme(tm); // Sets the theme for popup menus
				if (IsRebarUsed())
				{
					GetRebar().SetRebarTheme(tr);
					GetMenubar().SetMenubarTheme(tm); // Sets the theme for Menubar buttons
				}
			}
			break;

		case Olive:
			{
				// Used for XP Olive color scheme
				ThemeRebar tr = {T, RGB(215, 216, 182), RGB(242, 242, 230), RGB(249, 255, 227), RGB(178, 191, 145), F, T, T, T, T, F};
				ThemeToolbar tt = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 140, 40), RGB(255, 180, 80), RGB(200, 128, 128)};
				ThemeMenu tm = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(249, 255, 227), RGB(178, 191, 145), RGB(128, 128, 128)};

				GetToolbar().SetToolbarTheme(tt);
				SetMenuTheme(tm); // Sets the theme for popup menus
				if (IsRebarUsed())
				{
					GetRebar().SetRebarTheme(tr);
					GetMenubar().SetMenubarTheme(tm); // Sets the theme for Menubar buttons
				}
			}
			break;
		}

		RecalcLayout();
	}

	inline void CFrame::SetToolbarImages(CToolbar& TB, int iNumButtons, COLORREF crMask, UINT ToolbarID, UINT ToolbarHotID, UINT ToolbarDisabledID)
	{
		TB.SetImages(iNumButtons, crMask, ToolbarID, ToolbarHotID, ToolbarDisabledID);

		// Adjust the rebar band size
		if (m_bUseRebar)
				GetRebar().ResizeBand(GetRebar().GetBand(TB), TB.GetMaxSize());
	}

	inline void CFrame::SetView(CWnd& wndView)
	// Sets or changes the View window displayed within the frame
	{
		// Destroy the existing view window (if any)
		if (m_pWndView) m_pWndView->Destroy();
		
		// Assign the view window
		m_pWndView = &wndView;
		
		if (m_hWnd)
		{
			// The frame is already created, so create and position the new view too
			m_pWndView->Create(m_hWnd);
			RecalcLayout();
		}
	}

	inline void CFrame::UpdateCheckMarks()
	{
		if (::IsWindowVisible(GetToolbar()))
		// Show toolbar check mark
		{
			::CheckMenuItem (m_hMenu, IDW_VIEW_TOOLBAR, MF_CHECKED);
			if (IsMenubarUsed())
				::CheckMenuItem(GetMenubar().GetMenu(), IDW_VIEW_TOOLBAR, MF_CHECKED);
		}
		else
		// Hide toolbar check mark
		{
			::CheckMenuItem (m_hMenu, IDW_VIEW_TOOLBAR, MF_UNCHECKED);
			if (IsMenubarUsed())
				::CheckMenuItem(GetMenubar().GetMenu(), IDW_VIEW_TOOLBAR, MF_UNCHECKED);
		}

		if (::IsWindowVisible(GetStatusbar()))
		// Show status bar check mark
		{
			::CheckMenuItem (m_hMenu, IDW_VIEW_STATUSBAR, MF_CHECKED);
			if (IsMenubarUsed())
				::CheckMenuItem(GetMenubar().GetMenu(), IDW_VIEW_STATUSBAR, MF_CHECKED);
		}
		else
		// Hide status bar check mark
		{
			::CheckMenuItem (m_hMenu, IDW_VIEW_STATUSBAR, MF_UNCHECKED);
			if (IsMenubarUsed())
				::CheckMenuItem(GetMenubar().GetMenu(), IDW_VIEW_STATUSBAR, MF_UNCHECKED);
		}
	}

	inline void CFrame::UpdateMRUMenu()
	{
		if (0 >= m_nMaxMRU) return;

		// Get the handle to the Menu entry titled "File"
		int nFileItem = GetMenuItemPos(GetFrameMenu(), _T("File"));
		HMENU hFileMenu = ::GetSubMenu (GetFrameMenu(), nFileItem);

		// Remove all but the first MRU Menu entry
		for (UINT u = IDW_FILE_MRU_FILE2; u <= IDW_FILE_MRU_FILE1 +16; ++u)
		{
			DeleteMenu(hFileMenu, u, MF_BYCOMMAND);
		}

		int nMaxMRUIndex =  (int)m_vMRUEntries.size()-1;
		nMaxMRUIndex = min(nMaxMRUIndex, (int)m_nMaxMRU);

		// Set the text for the MRU Menu
		tString tsMRUArray[16];
		if (nMaxMRUIndex >= 0)
		{
			for (int n = 0; n <= nMaxMRUIndex; ++n)
			{
				tsMRUArray[n] = m_vMRUEntries[n];
				if (tsMRUArray[n].length() > MAX_MENU_STRING - 10)
				{
					// Truncate the string if its too long
					tsMRUArray[n].erase(0, tsMRUArray[n].length() - MAX_MENU_STRING +10);
					tsMRUArray[n] = _T("... ") + tsMRUArray[n];
				}

				// Prefix the string with its number
				TCHAR tVal[5];
				wsprintf(tVal, _T("%d "), n+1);
				tsMRUArray[n] = tVal + tsMRUArray[n];
			}
		}
		else
		{
			tsMRUArray[0] = _T("Recent Files");
			nMaxMRUIndex = 0;
		}

		// Set MRU menu items
		MENUITEMINFO mii = {0};

		// For Win95 and NT, cbSize needs to be 44
		if (1400 == (GetWinVersion()) || (2400 == GetWinVersion()))
			mii.cbSize = 44;
		else
			mii.cbSize = sizeof(MENUITEMINFO);

		for (int index = nMaxMRUIndex; index >= 0; --index)
		{
			mii.fMask = MIIM_TYPE | MIIM_ID | MIIM_STATE;
			mii.fState = (0 == m_vMRUEntries.size())? MFS_GRAYED : 0;
			mii.fType = MFT_STRING;
			mii.wID = IDW_FILE_MRU_FILE1 + index;
			mii.dwTypeData = (LPTSTR)tsMRUArray[index].c_str();

			BOOL bResult;
			if (index == nMaxMRUIndex)
				// Replace the last MRU entry first
				bResult = SetMenuItemInfo(hFileMenu, IDW_FILE_MRU_FILE1, FALSE, &mii);
			else
				// Insert the other MRU entries next
				bResult = InsertMenuItem(hFileMenu, IDW_FILE_MRU_FILE1 + index + 1, FALSE, &mii);

			if (!bResult)
			{
				TRACE(_T("Failed to set MRU menu item\n"));
				break;
			}
		}

		DrawMenuBar();
	}

	inline LRESULT CFrame::WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_ACTIVATE:
			{
				// Do default processing first
				DefWindowProc(uMsg, wParam, lParam);
				
				if (LOWORD(wParam) == WA_INACTIVE)
				{
					// Save the hwnd of the window which currently has focus
					// (this must be CFrame window itself or a child window
					if (!IsIconic()) m_hOldFocus = GetFocus();

					// Send a notification to the view window
					int idCtrl = ::GetDlgCtrlID(m_hOldFocus);
					NMHDR nhdr={0};
					nhdr.hwndFrom = m_hOldFocus;
					nhdr.idFrom = idCtrl;
					nhdr.code = UWM_FRAMELOSTFOCUS;
					GetView()->SendMessage(WM_NOTIFY, (WPARAM)idCtrl, (LPARAM)&nhdr);
				}
				else
				{					
					// Now set the focus to the appropriate child window
					if (m_hOldFocus) ::SetFocus(m_hOldFocus);
					
					// Send a notification to the view window
					int idCtrl = ::GetDlgCtrlID(m_hOldFocus);
					NMHDR nhdr={0};
					nhdr.hwndFrom = m_hOldFocus;
					nhdr.idFrom = idCtrl;
					nhdr.code = NM_SETFOCUS;
					GetView()->SendMessage(WM_NOTIFY, (WPARAM)idCtrl, (LPARAM)&nhdr);
				} 
			}
			return 0L;

		case WM_CLOSE:
			OnFrameClose();
			break;
		case WM_DESTROY:
			::SetMenu(m_hWnd, NULL);
			::KillTimer(m_hWnd, ID_STATUS_TIMER);
			::PostQuitMessage(0);	// Terminates the application
			return 0L;
		case WM_ERASEBKGND:
			return 0L;
		case WM_HELP:
			OnHelp();
			return 0L;
		case WM_MENUCHAR:
			if ((IsMenubarUsed()) && (LOWORD(wParam)!= VK_SPACE))
			{
				// Activate Menubar for key pressed with Alt key held down
				GetMenubar().MenuChar(wParam, lParam);
				return -1L;
			}
			break;
		case WM_MENUSELECT:
			OnMenuSelect(wParam, lParam);
			return 0L;
		case WM_SETFOCUS:
			OnFrameSetFocus();
			break;
		case WM_SIZE:
			RecalcLayout();
			return 0L;
		case WM_SYSCOLORCHANGE:
			// Changing themes trigger this
			OnFrameSysColorChange();

			// Forward the message to the view window
			::PostMessage(m_pWndView->GetHwnd(), WM_SYSCOLORCHANGE, 0, 0);
			return 0L;
		case WM_SYSCOMMAND:
			if ((SC_KEYMENU == wParam) && (VK_SPACE != lParam) && IsMenubarUsed())
			{
				GetMenubar().SysCommand(wParam, lParam);
				return 0L;
			}
			
			if (SC_MINIMIZE == wParam)
				m_hOldFocus = GetFocus();
			break;
		case WM_TIMER:
			OnFrameTimer(wParam);
			return 0L;
		case WM_DRAWITEM:
			// Owner draw menu items
			{
				LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT) lParam;
				if (pdis->CtlType == ODT_MENU)
				{
					OnFrameDrawItem(wParam, lParam);
					return TRUE; // handled
				}
			}
			break;
		case WM_INITMENUPOPUP:
			OnFrameInitMenuPopup(wParam, lParam);
			break;
		case WM_MEASUREITEM:
			{
				LPMEASUREITEMSTRUCT pmis = (LPMEASUREITEMSTRUCT) lParam;
				if (pmis->CtlType == ODT_MENU)
				{
					OnFrameMeasureItem(wParam, lParam);
					return TRUE; // handled
				}
			}
		case WM_EXITMENULOOP:
			OnFrameExitMenuLoop();
			break;
		} // switch uMsg

		return CWnd::WndProcDefault(hWnd, uMsg, wParam, lParam);
	} // LRESULT CFrame::WndProcDefault(...)


} // namespace Win32xx

#endif // FRAME_H
