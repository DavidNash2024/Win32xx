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


#ifndef _FRAME_H_
#define _FRAME_H_

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
		virtual BOOL IsMenubar() const {return TRUE;}
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
		friend class CMenubar;

	public:
		CFrame();
		virtual ~CFrame();

		// These are the functions you might wish to override
		virtual BOOL AddMenuIcon(int nID_MenuItem, HICON hIcon, int cx = 16, int cy = 16);
		virtual size_t AddMenuIcons(const std::vector<UINT>& MenuData, COLORREF crMask, UINT ToolbarID, UINT ToolbarDisabledID);
		virtual void AddToolbarButton(UINT nID, BOOL bEnabled = TRUE, LPCTSTR szText = 0);
		virtual void AdjustFrameRect(RECT rcView) const;
		virtual int  GetMenuItemPos(HMENU hMenu, LPCTSTR szItem);
		virtual CRect GetViewRect() const;
		virtual void LoadRegistryMRUSettings(UINT nMaxMRU = 0);
		virtual size_t SetMenuIcons(const std::vector<UINT>& MenuData, COLORREF crMask, UINT ToolbarID, UINT ToolbarDisabledID);
		virtual void SetStatusIndicators();
		virtual void SetStatusText();
		virtual void SetTheme();
		virtual void SetupToolbar();
		virtual void SetToolbarImages(COLORREF crMask, UINT ToolbarID, UINT ToolbarHotID, UINT ToolbarDisabledID);
		virtual void RecalcLayout();

		// Virtual Attributes
		// If you need to modify the default behaviour of the menubar, rebar, 
		// statusbar or toolbar, inherrit from those classes, and override 
		// the following attribute functions.
		virtual CMenubar& GetMenubar() const		{return (CMenubar&)m_Menubar;}
		virtual CRebar& GetRebar() const			{return (CRebar&)m_Rebar;}
		virtual CStatusbar& GetStatusbar() const	{return (CStatusbar&)m_Statusbar;}
		virtual CToolbar& GetToolbar() const		{return (CToolbar&)m_Toolbar;}

		// These functions aren't intended to be overridden
		HMENU GetFrameMenu() const	{return m_hMenu;}
		const ThemeMenu& GetMenuTheme()	const {return m_ThemeMenu;}
		tString GetRegistryKeyName(){return m_tsKeyName;}
		CWnd* GetView() const		{return m_pView;}
		tString GetMRUEntry(size_t nIndex);
		void SetFrameMenu(INT ID_MENU);
		void SetMenuTheme(ThemeMenu& Theme);
		void LoadRegistrySettings(LPCTSTR szKeyName);
		void SetView(CWnd& wndView);

		BOOL IsFrame() const			{return TRUE;}
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
		virtual void OnActivate(WPARAM wParam, LPARAM lParam);
		virtual void OnClose();
		virtual LRESULT OnDrawItem(WPARAM wParam, LPARAM lParam);
		virtual void OnExitMenuLoop();
		virtual void OnInitMenuPopup(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnMeasureItem(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual void OnSetFocus();
		virtual void OnSysColorChange();
		virtual LRESULT OnSysCommand(WPARAM wParam, LPARAM lParam);
		virtual	void OnTimer(WPARAM wParam);
		virtual void OnHelp();
		virtual LRESULT OnMenuChar(WPARAM wParam, LPARAM lParam);
		virtual void OnMenuSelect(WPARAM wParam, LPARAM lParam);
		virtual void OnViewStatusbar();
		virtual void OnViewToolbar();
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void PreRegisterClass(WNDCLASS &wc);
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		virtual void RemoveMRUEntry(LPCTSTR szMRUEntry);
		virtual void SaveRegistrySettings();
		virtual void ShowToolbar();
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

        BOOL m_bShowIndicatorStatus;		// set to TRUE to see indicators in status bar
		BOOL m_bShowMenuStatus;				// set to TRUE to see menu and toolbar updates in status bar
		BOOL m_bUseRebar;					// set to TRUE if Rebars are to be used
		BOOL m_bUseThemes;					// set to TRUE if themes are to be used
		BOOL m_bUpdateTheme;				// set to TRUE to run SetThemes when theme changes
		BOOL m_bUseToolbar;					// set to TRUE if the toolbar is used
		ThemeMenu m_ThemeMenu;				// Theme structure
		HIMAGELIST m_himlMenu;				// Imagelist of menu icons
		HIMAGELIST m_himlMenuDis;			// Imagelist of disabled menu icons

	private:
		void LoadCommonControls();

		std::vector<ItemData*> m_vMenuItemData;// vector of ItemData pointers
		std::vector<UINT> m_vMenuIcons;		// vector of menu icon resource IDs
		CDialog* m_pAboutDialog;			// Pointer to the about dialog object
		CMenubar m_Menubar;					// CMenubar object
		CRebar m_Rebar;						// CRebar object
		CStatusbar m_Statusbar;				// CStatusbar object
		CToolbar m_Toolbar;					// CToolbar object
		HMENU m_hMenu;						// handle to the frame menu
		CWnd* m_pView;						// pointer to the View CWnd object
		LPCTSTR m_OldStatus[3];				// Array of TCHAR pointers;
		std::vector<tString> m_vMRUEntries;	// Vector of tStrings for MRU entires
		tString m_tsKeyName;				// TCHAR std::string for Registry key name
		tString m_tsStatusText;				// TCHAR std::string for status text
		size_t m_nMaxMRU;					// maximum number of MRU entries
		CRect m_rcPosition;					// CRect of the starting window position
		HWND m_hOldFocus;					// The window which had focus prior to the app'a deactivation
		int m_nOldID;

	};  // class CFrame

}


#endif // _FRAME_H_
