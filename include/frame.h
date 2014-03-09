// Win32++   Pre-release Version 7.4
// Released: Not officially released
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2014  David Nash
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
//  Declaration of the CFrame and CMenuBar classes

// The classes declared in this file support SDI (Single Document Interface)
// frames on Win32/Win64 operating systems (not Windows CE). For Windows CE,
// use wceframe.h instead. SDI frames are a simple frame which supports a
// single view window. Refer to mdi.h for frames that support several
// child windows.

// CFrame also includes each of the following classes as members:
// * CReBar for managing the frame's rebar control.
// * CMenuBar for managing the menu inside the rebar.
// * CToolBar for managing the frame's toolbar.
// * CStatusBar for managing the frame's status bar.
// In each case these members are exposed by a GetXXX function, allowing
// them to be accessed or sent messages.

// CFrame is responsible for creating a "frame" window. This window has a
// menu and and several child windows, including a toolbar (usualy hosted
// within a rebar), a status bar, and a view positioned over the frame
// window's non-client area. The "view" window is a seperate CWnd object
// assigned to the frame with the SetView function.

// When compiling an application with these classes, it will need to be linked
// with Comctl32.lib.

// To create a SDI frame application, inherit a CMainFrame class from CFrame.
// Use the Frame sample application as the starting point for your own frame
// applications.
// Refer to the Notepad and Scribble samples for examples on how to use these
// classes to create a frame application.


#ifndef _WIN32XX_FRAME_H_
#define _WIN32XX_FRAME_H_

#include "wincore.h"
#include "dialog.h"
#include "gdi.h"
#include "statusbar.h"
#include "toolbar.h"
#include "rebar.h"
#include "default_resource.h"

#ifndef RBN_MINMAX
  #define RBN_MINMAX (RBN_FIRST - 21)
#endif
#ifndef TMT_BORDERSIZE
  #define TMT_BORDERSIZE	2403
#endif
#ifndef TMT_CONTENTMARGINS
  #define TMT_CONTENTMARGINS	3602
#endif
#ifndef VSCLASS_MENU
  #define VSCLASS_MENU	L"MENU"
#endif
#ifndef ODS_NOACCEL
  #define ODS_NOACCEL 0x0100
#endif
#ifndef ODS_HOTLIGHT
  #define ODS_HOTLIGHT 0x0040
#endif
#ifndef ODS_INACTIVE
  #define ODS_INACTIVE 0x0080
#endif
#ifndef DT_HIDEPREFIX
  #define DT_HIDEPREFIX 0x00100000
#endif



namespace Win32xx
{

	////////////////////////////////////////////////
	// Declarations of structures for themes
	//
	
	// define some structs and enums from uxtheme.h and vssym32.h
	typedef struct _MARGINS
	{
		int cxLeftWidth;      // width of left border that retains its size
		int cxRightWidth;     // width of right border that retains its size
		int cyTopHeight;      // height of top border that retains its size
		int cyBottomHeight;   // height of bottom border that retains its size
	} MARGINS, *PMARGINS;

	class CMargins : public MARGINS
	{
	public:
		CMargins(int cxLeft, int cxRight, int cyTop, int cyBottom)
		{
			cxLeftWidth    = cxLeft;	cxRightWidth   = cxRight;
			cyTopHeight    = cyTop;		cyBottomHeight = cyBottom;
		}
		CMargins()
		{
			cxLeftWidth    = 0;			cxRightWidth   = 0;
			cyTopHeight    = 0;			cyBottomHeight = 0;
		}
		int Width() const		{ return cxLeftWidth + cxRightWidth; }
		int Height() const		{ return cyTopHeight + cyBottomHeight; }
		void SetMargins(int cxLeft, int cxRight, int cyTop, int cyBottom)
		{
			cxLeftWidth    = cxLeft;	cxRightWidth   = cxRight;
			cyTopHeight    = cyTop;		cyBottomHeight = cyBottom;
		}
	};

	enum THEMESIZE
	{
		TS_MIN,             // minimum size
		TS_TRUE,            // size without stretching
		TS_DRAW             // size that theme mgr will use to draw part
	};

	enum POPUPCHECKSTATES
	{
		MC_CHECKMARKNORMAL = 1,
		MC_CHECKMARKDISABLED = 2,
		MC_BULLETNORMAL = 3,
		MC_BULLETDISABLED = 4,
	};

	enum POPUPCHECKBACKGROUNDSTATES
	{
		MCB_DISABLED = 1,
		MCB_NORMAL = 2,
		MCB_BITMAP = 3,
	};

	enum POPUPITEMSTATES
	{
		MPI_NORMAL = 1,
		MPI_HOT = 2,
		MPI_DISABLED = 3,
		MPI_DISABLEDHOT = 4,
	};

	enum POPUPSUBMENUSTATES
	{
		MSM_NORMAL = 1,
		MSM_DISABLED = 2,
	};

	enum MENUPARTS
	{
		MENU_MENUITEM_TMSCHEMA = 1,
		MENU_MENUDROPDOWN_TMSCHEMA = 2,
		MENU_MENUBARITEM_TMSCHEMA = 3,
		MENU_MENUBARDROPDOWN_TMSCHEMA = 4,
		MENU_CHEVRON_TMSCHEMA = 5,
		MENU_SEPARATOR_TMSCHEMA = 6,
		MENU_BARBACKGROUND = 7,
		MENU_BARITEM = 8,
		MENU_POPUPBACKGROUND = 9,
		MENU_POPUPBORDERS = 10,
		MENU_POPUPCHECK = 11,
		MENU_POPUPCHECKBACKGROUND = 12,
		MENU_POPUPGUTTER = 13,
		MENU_POPUPITEM = 14,
		MENU_POPUPSEPARATOR = 15,
		MENU_POPUPSUBMENU = 16,
		MENU_SYSTEMCLOSE = 17,
		MENU_SYSTEMMAXIMIZE = 18,
		MENU_SYSTEMMINIMIZE = 19,
		MENU_SYSTEMRESTORE = 20,
	};


	// Forward declaration of CFrame. Its defined later.
	class CFrame;

	struct MenuItemData
	// Each Dropdown menu item has this data
	{
		HMENU hMenu;
		MENUITEMINFO mii;
		UINT  nPos;
		std::vector<TCHAR> vItemText;

		MenuItemData() : hMenu(0), nPos(0) { vItemText.assign(MAX_MENU_STRING, _T('\0')); }
		LPTSTR GetItemText() {return &vItemText[0];}
	};


	////////////////////////////////////
	// Declaration of the CMenuBar class
	//
	class CMenuBar : public CToolBar
	{
		friend class CFrame;

	public:
		CMenuBar();
		virtual ~CMenuBar();
		virtual void SetMenu(HMENU hMenu);

		HMENU GetMenu() const {return m_hTopMenu;}

	protected:
	//Overridables
		virtual void OnAttach();
		virtual LRESULT OnDrawItem(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnExitMenuLoop(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnInitMenuPopup(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnKillFocus(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnMeasureItem(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnMenuChar(WPARAM wParam, LPARAM lParam);
		virtual BOOL	OnMenuInput(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnSysCommand(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnSysKeyDown(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnSysKeyUp(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnTBNDropDown(LPNMTOOLBAR pNMTB);
		virtual LRESULT OnTBNHotItemChange(LPNMTBHOTITEM pNMHI);
		virtual LRESULT OnWindowPosChanged(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnWindowPosChanging(WPARAM wParam, LPARAM lParam);
		virtual void	PreCreate(CREATESTRUCT &cs);
		virtual void	PreRegisterClass(WNDCLASS &wc);
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		CMenuBar(const CMenuBar&);				// Disable copy construction
		CMenuBar& operator = (const CMenuBar&); // Disable assignment operator
		void DoAltKey(WORD KeyCode);
		LRESULT OnPopupMenu();
		void DrawAllMDIButtons(CDC* pDrawDC);
		void DrawMDIButton(CDC* pDrawDC, int iButton, UINT uState);
		void ExitMenu();
		CWnd* GetActiveMDIChild();
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
		HMENU m_hSelMenu;		// handle to the cascaded popup menu
		HMENU m_hTopMenu;		// handle to the top level menu
		HWND  m_hPrevFocus;		// handle to window which had focus
		CRect m_MDIRect[3];		// array of CRect for MDI buttons
		int   m_nHotItem;		// hot item
		int   m_nMDIButton;		// the MDI button (MDIButtonType) pressed
		CPoint m_OldMousePos;	// old Mouse position
		CFrame* m_pFrame;       // Pointer to the frame

	};  // class CMenuBar


	/////////////////////////////////////
	// Declaration of the CMenuMetrics class
	//  This class is used by CFrame to retrieve the size of the components
	//  used to perform owner-drawing of menu items.  Windows Visual Styles
	//  are used to render menu items for systems running Aero (Vista and above).
	//  Win32++ custom themes are used to render menu items for systems without Aero.
	class CMenuMetrics
	{
	public:
		CMenuMetrics(CFrame* pFrame);
		~CMenuMetrics();

		CRect GetCheckBackgroundRect(CRect rcItem);
		CRect GetCheckRect(CRect rcItem);
		CRect GetGutterRect(CRect rcItem);
		CRect GetSelectionRect(CRect rcItem);
		CRect GetSeperatorRect(CRect rcItem);
		CRect GetTextRect(CRect rcItem);
		CSize GetTextSize(MenuItemData* pmd);
		void  Initialize();
		int   ToItemStateId(UINT uItemState);
		int   ToCheckBackgroundStateId(int iStateId);
		int   ToCheckStateId(UINT fType, int iStateId);

		// Wrappers for Windows API functions
		HRESULT CloseThemeData();
		HRESULT DrawThemeBackground(HDC hdc, int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect);
		HRESULT DrawThemeText(HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect);
		HRESULT GetThemePartSize(HDC hdc, int iPartId, int iStateId, LPCRECT prc, THEMESIZE eSize, SIZE* psz);
		HRESULT GetThemeInt(int iPartId, int iStateId, int iPropId, int* piVal);
		HRESULT GetThemeMargins(HDC hdc, int iPartId, int iStateId, int iPropId, LPRECT prc, MARGINS* pMargins);
		HRESULT GetThemeTextExtent(HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, LPCRECT pBoundingRect, LPRECT pExtentRect);
		BOOL    IsThemeBackgroundPartiallyTransparent(int iPartId, int iStateId);
		HANDLE  OpenThemeData(HWND hwnd, LPCWSTR pszClassList);

		HANDLE  m_hTheme;				// Theme handle
		CFrame* m_pFrame;				// Pointer to the frame window
		HMODULE m_hmodUXTheme;			// Module handle to the UXTheme dll

		CMargins m_marCheck;			// Check margins
		CMargins m_marCheckBackground;	// Check background margins
		CMargins m_marItem;				// Item margins
		CMargins m_marText;				// Text margins

		CSize   m_sizeCheck;			// Check size metric
		CSize   m_sizeSeparator;		// Separator size metric

	private:
		typedef HRESULT WINAPI CLOSETHEMEDATA(HANDLE);
		typedef HRESULT WINAPI DRAWTHEMEBACKGROUND(HANDLE, HDC, int, int, const RECT*, const RECT*);
		typedef HRESULT WINAPI DRAWTHEMETEXT(HANDLE, HDC, int, int, LPCWSTR, int, DWORD, DWORD, LPCRECT);
		typedef HRESULT WINAPI GETTHEMEPARTSIZE(HANDLE, HDC, int, int, LPCRECT, THEMESIZE, SIZE*);
		typedef HRESULT WINAPI GETTHEMEINT(HANDLE, int, int, int, int*);
		typedef HRESULT WINAPI GETTHEMEMARGINS(HANDLE, HDC, int, int, int, LPRECT, MARGINS*);
		typedef HRESULT WINAPI GETTHEMETEXTEXTENT(HANDLE, HDC, int, int, LPCWSTR, int, DWORD, LPCRECT, LPCRECT);
		typedef HRESULT WINAPI ISTHEMEBGPARTTRANSPARENT(HANDLE, int, int);
		typedef HANDLE  WINAPI OPENTHEMEDATA(HWND, LPCWSTR);

		// Pointers to functions defined in uxTheme.dll
		CLOSETHEMEDATA*			  m_pfnCloseThemeData;
		DRAWTHEMEBACKGROUND*	  m_pfnDrawThemeBackground;
		DRAWTHEMETEXT*			  m_pfnDrawThemeText;
		GETTHEMEPARTSIZE*		  m_pfnGetThemePartSize;
		GETTHEMEINT*			  m_pfnGetThemeInt;
		GETTHEMEMARGINS*		  m_pfnGetThemeMargins;
		GETTHEMETEXTEXTENT*		  m_pfnGetThemeTextExtent;
		ISTHEMEBGPARTTRANSPARENT* m_pfnIsThemeBGPartTransparent;
		OPENTHEMEDATA*			  m_pfnOpenThemeData;
	};


	//////////////////////////////////
	// Declaration of the CFrame class
	//
	class CFrame : public CWnd
	{
		friend class CMenuBar;
		typedef Shared_Ptr<MenuItemData> ItemDataPtr;

	public:
		CFrame();
		virtual ~CFrame();

		// Override these functions as required
		virtual void AdjustFrameRect(RECT rcView);
		virtual CString GetThemeName() const;
		virtual CRect GetViewRect() const;
		virtual BOOL IsMDIFrame() const { return FALSE; }
		virtual void SetStatusIndicators();
		virtual void SetStatusText();
		virtual void RecalcLayout();

		// Virtual Attributes
		// If you need to modify the default behaviour of the MenuBar, ReBar,
		// StatusBar or ToolBar, inherit from those classes, and override
		// the following attribute functions.
		virtual CMenuBar* GetMenuBar()	const		{ return (CMenuBar*)&m_MenuBar; }
		virtual CReBar* GetReBar() const			{ return (CReBar*)&m_ReBar; }
		virtual CStatusBar* GetStatusBar() const	{ return (CStatusBar*)&m_StatusBar; }
		virtual CToolBar* GetToolBar() const		{ return (CToolBar*)&m_ToolBar; }

		// Non-virtual Attributes
		// These functions aren't virtual, and shouldn't be overridden
		MenuTheme* GetMenuBarTheme() const			{ return (MenuTheme*)&m_MenuBarTheme; }
		ReBarTheme* GetReBarTheme()	const			{ return (ReBarTheme*)&m_ReBarTheme; }
		ToolBarTheme* GetToolBarTheme()	const		{ return (ToolBarTheme*)&m_ToolBarTheme; }
		void SetReBarTheme(ReBarTheme* pRBT); 
		void SetToolBarTheme(ToolBarTheme* pTBT);
		HACCEL GetFrameAccel() const				{ return m_hAccel; }
		CMenu* GetFrameMenu() const					{ return (CMenu*)&m_Menu; }
		std::vector<CString> GetMRUEntries() const	{ return m_vMRUEntries; }
		CString GetRegistryKeyName() const			{ return m_strKeyName; }
		CWnd* GetView() const						{ return m_pView; }
		CString GetMRUEntry(UINT nIndex);
		void SetFrameMenu(INT ID_MENU);
		void SetFrameMenu(HMENU hMenu);
		void SetMenuTheme(MenuTheme* pMBT);
		void SetView(CWnd& wndView);
		BOOL IsMenuBarUsed() const					{ return (m_MenuBar.IsWindow()); }
		BOOL IsReBarSupported() const				{ return (GetComCtlVersion() > 470); }
		BOOL IsReBarUsed() const					{ return (m_ReBar.IsWindow()); }

	protected:
		// Override these functions as required
		virtual BOOL AddMenuIcon(int nID_MenuItem, HICON hIcon);
		virtual UINT AddMenuIcons(const std::vector<UINT>& MenuData, COLORREF crMask, UINT ToolBarID, UINT ToolBarDisabledID);
		virtual void AddMenuBarBand();
		virtual void AddMRUEntry(LPCTSTR szMRUEntry);
		virtual void AddToolBarBand(CToolBar* pTB, DWORD dwStyle, UINT nID);
		virtual void AddToolBarButton(UINT nID, BOOL bEnabled = TRUE, LPCTSTR szText = 0, int iImage = -1);
		virtual void CreateToolBar();
		virtual LRESULT CustomDrawMenuBar(NMHDR* pNMHDR);
		virtual LRESULT CustomDrawToolBar(NMHDR* pNMHDR);
		virtual void DrawMenuItem(LPDRAWITEMSTRUCT pdis);
		virtual void DrawMenuItemBkgnd(LPDRAWITEMSTRUCT pdis);
		virtual void DrawMenuItemCheckmark(LPDRAWITEMSTRUCT pdis);
		virtual void DrawMenuItemIcon(LPDRAWITEMSTRUCT pdis);
		virtual void DrawMenuItemText(LPDRAWITEMSTRUCT pdis);
		virtual int  GetMenuItemPos(CMenu* pMenu, LPCTSTR szItem);
		virtual BOOL LoadRegistrySettings(LPCTSTR szKeyName);
		virtual BOOL LoadRegistryMRUSettings(UINT nMaxMRU = 0);
		virtual void MeasureMenuItem(MEASUREITEMSTRUCT *pmis);
		virtual LRESULT OnActivate(WPARAM wParam, LPARAM lParam);
		virtual void OnClose();
		virtual int  OnCreate(LPCREATESTRUCT pcs);
		virtual LRESULT OnCustomDraw(LPNMHDR pNMHDR);
		virtual void OnDestroy();
		virtual LRESULT OnDrawItem(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnExitMenuLoop(WPARAM wParam, LPARAM lParam);
		virtual LRESULT	OnHelp();
		virtual LRESULT OnInitMenuPopup(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnMeasureItem(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnMenuChar(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnMenuSelect(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnSetFocus(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnSize(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnSysColorChange(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnSysCommand(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnRBNHeightChange(LPNMHDR pNMHDR);
		virtual LRESULT OnRBNLayoutChanged(LPNMHDR pNMHDR);
		virtual LRESULT OnRBNMinMax(LPNMHDR pNMHDR);
		virtual LRESULT OnHotItemChange(LPNMTBHOTITEM pTBHotItem);
		virtual LRESULT OnTBNDropDown(LPNMTOOLBAR pNMTB);
		virtual LRESULT OnTTNGetDispInfo(LPNMTTDISPINFO pNMTDI);
		virtual	LRESULT OnTimer(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnUndocked();
		virtual void OnViewStatusBar();
		virtual void OnViewToolBar();
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void PreRegisterClass(WNDCLASS &wc);
		virtual void RemoveMRUEntry(LPCTSTR szMRUEntry);
		virtual BOOL SaveRegistrySettings();
		virtual void SetMenuBarBandSize();
		virtual UINT SetMenuIcons(const std::vector<UINT>& MenuData, COLORREF crMask, UINT ToolBarID, UINT ToolBarDisabledID);
		virtual void SetTBImageList(CToolBar* pToolBar, CImageList* pImageList, UINT nID, COLORREF crMask);
		virtual void SetTBImageListDis(CToolBar* pToolBar, CImageList* pImageList, UINT nID, COLORREF crMask);
		virtual void SetTBImageListHot(CToolBar* pToolBar, CImageList* pImageList, UINT nID, COLORREF crMask);
		virtual void SetupToolBar();
		virtual void SetTheme();
		virtual void SetToolBarImages(COLORREF crMask, UINT ToolBarID, UINT ToolBarHotID, UINT ToolBarDisabledID);
		virtual void ShowMenu(BOOL bShow);
		virtual void ShowStatusBar(BOOL bShow);
		virtual void ShowToolBar(BOOL bShow);
		virtual void UpdateMRUMenu();
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

		enum Constants
		{
			ID_STATUS_TIMER = 1,
		};

		Shared_Ptr<CMenuMetrics> m_pMenuMetrics;  // Smart pointer for CMenuMetrics
		CString m_strStatusText;			// CString for status text
        BOOL m_bShowIndicatorStatus;		// set to TRUE to see indicators in status bar
		BOOL m_bShowMenuStatus;				// set to TRUE to see menu and toolbar updates in status bar
		BOOL m_bUseReBar;					// set to TRUE if ReBars are to be used
		BOOL m_bUseThemes;					// set to TRUE if themes are to be used
		BOOL m_bUseToolBar;					// set to TRUE if the toolbar is used
		BOOL m_bShowStatusBar;				// A flag to indicate if the StatusBar should be displayed
		BOOL m_bShowToolBar;				// A flag to indicate if the ToolBar should be displayed
		CImageList m_imlMenu;				// Imagelist of menu icons
		CImageList m_imlMenuDis;			// Imagelist of disabled menu icons

	private:
		CFrame(const CFrame&);				// Disable copy construction
		CFrame& operator = (const CFrame&); // Disable assignment operator
		CSize GetTBImageSize(CBitmap* pbm);

		std::vector<ItemDataPtr> m_vMenuItemData;	// vector of MenuItemData pointers
		std::vector<CString> m_vMRUEntries;	// Vector of CStrings for MRU entries
		std::vector<UINT> m_vMenuIcons;		// vector of menu icon resource IDs
		std::vector<UINT> m_vToolBarData;	// vector of resource IDs for ToolBar buttons
		CDialog m_AboutDialog;				// Help about dialog
		CMenuBar m_MenuBar;					// CMenuBar object
		CReBar m_ReBar;						// CReBar object
		CStatusBar m_StatusBar;				// CStatusBar object
		CToolBar m_ToolBar;					// CToolBar object
		CMenu m_Menu;						// handle to the frame menu
		CFont m_fntMenuBar;					// MenuBar font
		CFont m_fntStatusBar;				// StatusBar font
		HACCEL m_hAccel;					// handle to the frame's accelerator table
		CWnd* m_pView;						// pointer to the View CWnd object
		CString m_OldStatus[3];				// Array of CString holding old status;
		CString m_strKeyName;				// CString for Registry key name
		CString m_strTooltip;				// CString for tool tips
		CString m_XPThemeName;				// CString for Windows Theme Name
		UINT m_nMaxMRU;						// maximum number of MRU entries
		CRect m_rcPosition;					// CRect of the starting window position
		HWND m_hOldFocus;					// The window which had focus prior to the app's deactivation
		int m_nOldID;						// The previous ToolBar ID displayed in the statusbar
		BOOL m_bDrawArrowBkgrnd;			// True if a separate arrow background is to be drawn on toolbar
		MenuTheme m_MenuBarTheme;			// struct of theme info for the popup Menu and MenuBar
		ReBarTheme m_ReBarTheme;			// struct of theme info for the ReBar
		ToolBarTheme m_ToolBarTheme;		// struct of theme info for the ToolBar
		CImageList m_ToolBarImages;			// Image list for the ToolBar buttons
		CImageList m_ToolBarDisabledImages;	// Image list for the Disabled ToolBar buttons
		CImageList m_ToolBarHotImages;		// Image list for the Hot ToolBar buttons
		
	};  // class CFrame

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{

	/////////////////////////////////////
	// Definitions for the CMenuBar class
	//
	inline CMenuBar::CMenuBar()
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
		m_pFrame		= 0;
	}

	inline CMenuBar::~CMenuBar()
	{
	}

	inline void CMenuBar::DoAltKey(WORD KeyCode)
	{
		//Handle key pressed with Alt held down
		UINT ID;
		if (SendMessage(TB_MAPACCELERATOR, KeyCode, (LPARAM) &ID))
		{
			GrabFocus();
			m_bKeyMode = TRUE;
			SetHotItem(ID);
			m_bMenuActive = TRUE;
			PostMessage(UWM_POPUPMENU, 0L, 0L);
		}
		else
			::MessageBeep(MB_OK);
	}

	inline LRESULT CMenuBar::OnPopupMenu()
	{
		if (m_bKeyMode)
			// Simulate a down arrow key press
			PostMessage(WM_KEYDOWN, VK_DOWN, 0L);

		m_bKeyMode = FALSE;
		m_bExitAfter = FALSE;
		m_OldMousePos = GetCursorPos();

		CWnd* pMaxMDIChild = NULL;
		if (IsMDIChildMaxed())
			pMaxMDIChild = GetActiveMDIChild();

		// Load the submenu
		int nMaxedOffset = IsMDIChildMaxed()? 1:0;
		m_hPopupMenu = ::GetSubMenu(m_hTopMenu, m_nHotItem - nMaxedOffset);
		if (IsMDIChildMaxed() && (0 == m_nHotItem) )
		m_hPopupMenu = ::GetSystemMenu(*pMaxMDIChild, FALSE);

        // Retrieve the bounding rectangle for the toolbar button
		CRect rc = GetItemRect(m_nHotItem);

		// convert rectangle to desktop coordinates
		ClientToScreen(rc);

		// Position popup above toolbar if it won't fit below
		TPMPARAMS tpm;
		tpm.cbSize = sizeof(TPMPARAMS);
		tpm.rcExclude = rc;

		// Set the hot button
		SendMessage(TB_SETHOTITEM, m_nHotItem, 0L);
		SendMessage(TB_PRESSBUTTON, m_nHotItem, MAKELONG(TRUE, 0));

		m_bSelPopup = FALSE;
		m_hSelMenu = NULL;
		m_bMenuActive = TRUE;

		// We hook mouse input to process mouse and keyboard input during
		//  the popup menu. Messages are sent to StaticMsgHook.

		// Remove any remaining hook first
		TLSData* pTLSData = (TLSData*)::TlsGetValue(GetApp()->GetTlsIndex());
		pTLSData->pMenuBar = this;
		if (pTLSData->hHook != NULL)
			::UnhookWindowsHookEx(pTLSData->hHook);

		// Hook messages about to be processed by the shortcut menu
		pTLSData->hHook = ::SetWindowsHookEx(WH_MSGFILTER, (HOOKPROC)StaticMsgHook, NULL, ::GetCurrentThreadId());

		// Display the shortcut menu
		BOOL bRightToLeft = FALSE;

#if defined(WINVER) && defined (WS_EX_LAYOUTRTL) && (WINVER >= 0x0500)
		bRightToLeft = ((GetAncestor()->GetWindowLongPtr(GWL_EXSTYLE)) & WS_EX_LAYOUTRTL);
#endif

		int xPos = bRightToLeft? rc.right : rc.left;
		UINT nID = ::TrackPopupMenuEx(m_hPopupMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
			xPos, rc.bottom, m_hWnd, &tpm);

		// We get here once the TrackPopupMenuEx has ended
		m_bMenuActive = FALSE;

		// Remove the message hook
		::UnhookWindowsHookEx(pTLSData->hHook);
		pTLSData->hHook = NULL;

		// Process MDI Child system menu
		if (IsMDIChildMaxed())
		{
			if (::GetSystemMenu(*pMaxMDIChild, FALSE) == m_hPopupMenu )
			{
				if (nID)
					pMaxMDIChild->SendMessage(WM_SYSCOMMAND, nID, 0L);
			}
		}

		// Re-establish Focus
		if (m_bKeyMode)
			GrabFocus();

		return 0L;
	}

	inline void CMenuBar::DrawAllMDIButtons(CDC* pDrawDC)
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

			DrawMDIButton(pDrawDC, MDI_MIN, 0);
			DrawMDIButton(pDrawDC, MDI_RESTORE, 0);
			DrawMDIButton(pDrawDC, MDI_CLOSE, 0);
		}
	}

	inline void CMenuBar::DrawMDIButton(CDC* pDrawDC, int iButton, UINT uState)
	{
		if (!IsRectEmpty(&m_MDIRect[iButton]))
		{
			switch (uState)
			{
			case 0:
				{
					// Draw a grey outline
					pDrawDC->CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNFACE));
					pDrawDC->MoveTo(m_MDIRect[iButton].left, m_MDIRect[iButton].bottom);
					pDrawDC->LineTo(m_MDIRect[iButton].right, m_MDIRect[iButton].bottom);
					pDrawDC->LineTo(m_MDIRect[iButton].right, m_MDIRect[iButton].top);
					pDrawDC->LineTo(m_MDIRect[iButton].left, m_MDIRect[iButton].top);
					pDrawDC->LineTo(m_MDIRect[iButton].left, m_MDIRect[iButton].bottom);
				}
				break;
			case 1:
				{
					// Draw outline, white at top, black on bottom
					pDrawDC->CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					pDrawDC->MoveTo(m_MDIRect[iButton].left, m_MDIRect[iButton].bottom);
					pDrawDC->LineTo(m_MDIRect[iButton].right, m_MDIRect[iButton].bottom);
					pDrawDC->LineTo(m_MDIRect[iButton].right, m_MDIRect[iButton].top);
					pDrawDC->CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
					pDrawDC->LineTo(m_MDIRect[iButton].left, m_MDIRect[iButton].top);
					pDrawDC->LineTo(m_MDIRect[iButton].left, m_MDIRect[iButton].bottom);
				}

				break;
			case 2:
				{
					// Draw outline, black on top, white on bottom
					pDrawDC->CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
					pDrawDC->MoveTo(m_MDIRect[iButton].left, m_MDIRect[iButton].bottom);
					pDrawDC->LineTo(m_MDIRect[iButton].right, m_MDIRect[iButton].bottom);
					pDrawDC->LineTo(m_MDIRect[iButton].right, m_MDIRect[iButton].top);
					pDrawDC->CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					pDrawDC->LineTo(m_MDIRect[iButton].left, m_MDIRect[iButton].top);
					pDrawDC->LineTo(m_MDIRect[iButton].left, m_MDIRect[iButton].bottom);
				}
				break;
			}

			pDrawDC->CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

			switch (iButton)
			{
			case MDI_MIN:
				// Manually Draw Minimise button
				pDrawDC->MoveTo(m_MDIRect[0].left + 4, m_MDIRect[0].bottom -4);
				pDrawDC->LineTo(m_MDIRect[0].right - 4, m_MDIRect[0].bottom - 4);

				pDrawDC->MoveTo(m_MDIRect[0].left + 4, m_MDIRect[0].bottom -5);
				pDrawDC->LineTo(m_MDIRect[0].right - 4, m_MDIRect[0].bottom - 5);
				break;
			case MDI_RESTORE:
				// Manually Draw Restore Button
				pDrawDC->MoveTo(m_MDIRect[1].left + 3, m_MDIRect[1].top + 7);
				pDrawDC->LineTo(m_MDIRect[1].left + 3, m_MDIRect[1].bottom -4);
				pDrawDC->LineTo(m_MDIRect[1].right - 6, m_MDIRect[1].bottom -4);
				pDrawDC->LineTo(m_MDIRect[1].right - 6, m_MDIRect[1].top + 7);
				pDrawDC->LineTo(m_MDIRect[1].left + 3, m_MDIRect[1].top + 7);

				pDrawDC->MoveTo(m_MDIRect[1].left + 3, m_MDIRect[1].top + 8);
				pDrawDC->LineTo(m_MDIRect[1].right - 6, m_MDIRect[1].top + 8);

				pDrawDC->MoveTo(m_MDIRect[1].left + 5, m_MDIRect[1].top + 7);
				pDrawDC->LineTo(m_MDIRect[1].left + 5, m_MDIRect[1].top + 4);
				pDrawDC->LineTo(m_MDIRect[1].right - 4, m_MDIRect[1].top + 4);
				pDrawDC->LineTo(m_MDIRect[1].right - 4, m_MDIRect[1].bottom -6);
				pDrawDC->LineTo(m_MDIRect[1].right - 6, m_MDIRect[1].bottom -6);

				pDrawDC->MoveTo(m_MDIRect[1].left + 5, m_MDIRect[1].top + 5);
				pDrawDC->LineTo(m_MDIRect[1].right - 4, m_MDIRect[1].top + 5);
				break;
			case MDI_CLOSE:
				// Manually Draw Close Button
				pDrawDC->MoveTo(m_MDIRect[2].left + 4, m_MDIRect[2].top +5);
				pDrawDC->LineTo(m_MDIRect[2].right - 4, m_MDIRect[2].bottom -3);

				pDrawDC->MoveTo(m_MDIRect[2].left + 5, m_MDIRect[2].top +5);
				pDrawDC->LineTo(m_MDIRect[2].right - 4, m_MDIRect[2].bottom -4);

				pDrawDC->MoveTo(m_MDIRect[2].left + 4, m_MDIRect[2].top +6);
				pDrawDC->LineTo(m_MDIRect[2].right - 5, m_MDIRect[2].bottom -3);

				pDrawDC->MoveTo(m_MDIRect[2].right -5, m_MDIRect[2].top +5);
				pDrawDC->LineTo(m_MDIRect[2].left + 3, m_MDIRect[2].bottom -3);

				pDrawDC->MoveTo(m_MDIRect[2].right -5, m_MDIRect[2].top +6);
				pDrawDC->LineTo(m_MDIRect[2].left + 4, m_MDIRect[2].bottom -3);

				pDrawDC->MoveTo(m_MDIRect[2].right -6, m_MDIRect[2].top +5);
				pDrawDC->LineTo(m_MDIRect[2].left + 3, m_MDIRect[2].bottom -4);
				break;
			}
		}
	}

	inline void CMenuBar::ExitMenu()
	{
		ReleaseFocus();
		m_bKeyMode = FALSE;
		m_bMenuActive = FALSE;
		SendMessage(TB_PRESSBUTTON, m_nHotItem, (LPARAM) MAKELONG (FALSE, 0));
		SetHotItem(-1);

		CPoint pt = GetCursorPos();
		ScreenToClient(pt);

		// Update mouse mouse position for hot tracking
		SendMessage(WM_MOUSEMOVE, 0L, MAKELONG(pt.x, pt.y));
	}

	inline CWnd* CMenuBar::GetActiveMDIChild()
	{
		CWnd* pMDIChild = NULL;
		if (IsMDIFrame())
		{
			pMDIChild = FromHandle((HWND)m_pFrame->GetView()->SendMessage(WM_MDIGETACTIVE, 0L, 0L));
		}

		return pMDIChild;
	}

	inline void CMenuBar::GrabFocus()
	{
		if (::GetFocus() != m_hWnd)
			m_hPrevFocus = ::SetFocus(m_hWnd);
		::SetCapture(m_hWnd);
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}

	inline BOOL CMenuBar::IsMDIChildMaxed() const
	{
		BOOL bMaxed = FALSE;

		if (IsMDIFrame() && m_pFrame->GetView()->IsWindow())
		{
			m_pFrame->GetView()->SendMessage(WM_MDIGETACTIVE, 0L, (LPARAM)&bMaxed);
		}

		return bMaxed;
	}

	inline BOOL CMenuBar::IsMDIFrame() const
	{
		return (m_pFrame && m_pFrame->IsMDIFrame());
	}

	inline LRESULT CMenuBar::OnMenuChar(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);

		if (!m_bMenuActive)
			DoAltKey(LOWORD(wParam));

		return FinalWindowProc(WM_MENUCHAR, wParam, lParam);
	}

	inline void CMenuBar::OnAttach()
	{
		// We must send this message before sending the TB_ADDBITMAP or TB_ADDBUTTONS message
		SendMessage(TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0L);

		m_pFrame = (CFrame*)GetAncestor();
		assert(m_pFrame);
	}

	inline LRESULT CMenuBar::OnDrawItem(WPARAM wParam, LPARAM lParam)
	{
		m_pFrame->OnDrawItem(wParam, lParam);
		return TRUE; // handled
	}

	inline LRESULT CMenuBar::OnExitMenuLoop(WPARAM wParam, LPARAM lParam)
	{
		if (m_bExitAfter)
			ExitMenu();
		m_pFrame->OnExitMenuLoop(wParam, lParam);
		
		return 0L;
	}

	inline LRESULT CMenuBar::OnInitMenuPopup(WPARAM wParam, LPARAM lParam)
	{
		m_pFrame->OnInitMenuPopup(wParam, lParam);
		return 0L;
	}

	inline LRESULT CMenuBar::OnKeyDown(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);

		switch (wParam)
		{
		case VK_ESCAPE:
			ExitMenu();
			break;

		case VK_SPACE:
			ExitMenu();
			// Bring up the system menu
			GetAncestor()->PostMessage(WM_SYSCOMMAND, SC_KEYMENU, VK_SPACE);
			break;

		// Handle VK_DOWN,VK_UP and VK_RETURN together
		case VK_DOWN:
		case VK_UP:
		case VK_RETURN:
			// Always use PostMessage for USER_POPUPMENU (not SendMessage)
			PostMessage(UWM_POPUPMENU, 0L, 0L);
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
				if (SendMessage(TB_MAPACCELERATOR, wParam, (LPARAM) &ID))
				{
					m_nHotItem = ID;
					PostMessage(UWM_POPUPMENU, 0L, 0L);
				}
				else
					::MessageBeep(MB_OK);
			}
			break;
		} // switch (wParam)

		return 0L;	// Discard these messages
	}

	inline LRESULT CMenuBar::OnKillFocus(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);
		UNREFERENCED_PARAMETER(lParam);

		ExitMenu();
		return 0;
	}

	inline LRESULT CMenuBar::OnLButtonDown(WPARAM wParam, LPARAM lParam)
	{
		// Do default processing first
		CallWindowProc(GetPrevWindowProc(), WM_LBUTTONDOWN, wParam, lParam);

		GrabFocus();
		m_nMDIButton = 0;
		CPoint pt;

		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		if (IsMDIFrame())
		{
			if (IsMDIChildMaxed())
			{
				CClientDC MenuBarDC(this);
				m_nMDIButton = -1;

				if (m_MDIRect[0].PtInRect(pt)) m_nMDIButton = 0;
				if (m_MDIRect[1].PtInRect(pt)) m_nMDIButton = 1;
				if (m_MDIRect[2].PtInRect(pt)) m_nMDIButton = 2;

				if (m_nMDIButton >= 0)
				{
					DrawMDIButton(&MenuBarDC, MDI_MIN,     (0 == m_nMDIButton)? 2 : 0);
					DrawMDIButton(&MenuBarDC, MDI_RESTORE, (1 == m_nMDIButton)? 2 : 0);
					DrawMDIButton(&MenuBarDC, MDI_CLOSE,   (2 == m_nMDIButton)? 2 : 0);
				}

				// Bring up the MDI Child window's system menu when the icon is pressed
				if (0 == HitTest())
				{
					m_nHotItem = 0;
					PostMessage(UWM_POPUPMENU, 0L, 0L);
				}
			}
		}

		return 0L;
	}

	inline LRESULT CMenuBar::OnLButtonUp(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);
		CPoint pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		if (IsMDIFrame())
		{
			CWnd* pMDIClient = m_pFrame->GetView();
			CWnd* pMDIChild = GetActiveMDIChild();
			assert(pMDIClient);
			assert(pMDIChild);

			if (IsMDIChildMaxed())
			{
				CPoint pt = GetCursorPos();
				ScreenToClient(pt);

				// Process the MDI button action when the left mouse button is up
				if (m_MDIRect[0].PtInRect(pt))
				{
					if (MDI_MIN == m_nMDIButton)
						pMDIChild->ShowWindow(SW_MINIMIZE);
				}

				if (m_MDIRect[1].PtInRect(pt))
				{
					if (MDI_RESTORE == m_nMDIButton)
						pMDIClient->PostMessage(WM_MDIRESTORE, (WPARAM)pMDIChild->GetHwnd(), 0L);
				}

				if (m_MDIRect[2].PtInRect(pt))
				{
					if (MDI_CLOSE == m_nMDIButton)
						pMDIChild->PostMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
				}
			}
		}
		m_nMDIButton = 0;
		ExitMenu();

		return 0L;
	}

	inline LRESULT CMenuBar::OnMeasureItem(WPARAM wParam, LPARAM lParam)
	{
		m_pFrame->OnMeasureItem(wParam, lParam);
		return TRUE; // handled
	}

	inline BOOL CMenuBar::OnMenuInput(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
					SendMessage(WM_CANCELMODE, 0L, 0L);
					SendMessage(TB_PRESSBUTTON, m_nHotItem, MAKELONG(FALSE, 0));
					SendMessage(TB_SETHOTITEM, m_nHotItem, 0L);
					ExitMenu();
					break;

				case VK_LEFT:
					// Use default processing if inside a Sub Menu
				    if ((m_hSelMenu) &&(m_hSelMenu != m_hPopupMenu))
						return FALSE;

					SendMessage(TB_PRESSBUTTON, m_nHotItem, MAKELONG(FALSE, 0));

					// Move left to next topmenu item
					(m_nHotItem > 0)? --m_nHotItem : m_nHotItem = GetButtonCount()-1;
					SendMessage(WM_CANCELMODE, 0L, 0L);

					// Always use PostMessage for USER_POPUPMENU (not SendMessage)
					PostMessage(UWM_POPUPMENU, 0L, 0L);
					PostMessage(WM_KEYDOWN, VK_DOWN, 0L);
					break;

				case VK_RIGHT:
					// Use default processing to open Sub Menu
					if (m_bSelPopup)
						return FALSE;

					SendMessage(TB_PRESSBUTTON, m_nHotItem, MAKELONG(FALSE, 0));

					// Move right to next topmenu item
					(m_nHotItem < GetButtonCount()-1)? ++m_nHotItem : m_nHotItem = 0;
					SendMessage(WM_CANCELMODE, 0L, 0L);

					// Always use PostMessage for USER_POPUPMENU (not SendMessage)
					PostMessage(UWM_POPUPMENU, 0L, 0L);
					PostMessage(WM_KEYDOWN, VK_DOWN, 0L);
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
					SendMessage(WM_CANCELMODE, 0L, 0L);
					return TRUE;
				}
			}
			return FALSE;

		case WM_LBUTTONDBLCLK:
			// Perform default action for DblClick on MDI Maxed icon
			if (IsMDIChildMaxed() && (0 == HitTest()))
			{
				CWnd* pMDIChild = GetActiveMDIChild();
				assert(pMDIChild);
				CMenu* pChildMenu = pMDIChild->GetSystemMenu(FALSE);

				UINT nID = pChildMenu->GetDefaultItem(FALSE, 0);
				if (nID)
					pMDIChild->PostMessage(WM_SYSCOMMAND, nID, 0L);
			}

			m_bExitAfter = TRUE;
			return FALSE;

		case WM_MENUSELECT:
			{
				// store info about selected item
				m_hSelMenu = (HMENU)lParam;
				m_bSelPopup = ((HIWORD(wParam) & MF_POPUP) != 0);

				// Reflect message back to the frame window
				GetAncestor()->SendMessage(WM_MENUSELECT, wParam, lParam);
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
				ScreenToClient(pt);

				// Reflect messages back to the MenuBar for hot tracking
				SendMessage(WM_MOUSEMOVE, 0L, MAKELPARAM(pt.x, pt.y));
			}
			break;

		}
		return FALSE;
	}

	inline LRESULT CMenuBar::OnMouseLeave(WPARAM wParam, LPARAM lParam)
	{
		if (IsMDIFrame())
		{
			if (IsMDIChildMaxed())
			{
				CClientDC MenuBarDC(this);

				DrawMDIButton(&MenuBarDC, MDI_MIN,     0);
				DrawMDIButton(&MenuBarDC, MDI_RESTORE, 0);
				DrawMDIButton(&MenuBarDC, MDI_CLOSE,   0);
			}
		}

		return FinalWindowProc(WM_MOUSELEAVE, wParam, lParam);
	}

	inline LRESULT CMenuBar::OnMouseMove(WPARAM wParam, LPARAM lParam)
	{
		CPoint pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		if (IsMDIFrame())
		{
			if (IsMDIChildMaxed())
			{
				CClientDC MenuBarDC(this);
				int MDIButton = -1;
				if (m_MDIRect[0].PtInRect(pt)) MDIButton = 0;
				if (m_MDIRect[1].PtInRect(pt)) MDIButton = 1;
				if (m_MDIRect[2].PtInRect(pt)) MDIButton = 2;

				if (MK_LBUTTON == wParam)  // mouse moved with left mouse button is held down
				{
					// toggle the MDI button image pressed/unpressed as required
					if (MDIButton >= 0)
					{
						DrawMDIButton(&MenuBarDC, MDI_MIN,     ((0 == MDIButton) && (0 == m_nMDIButton))? 2 : 0);
						DrawMDIButton(&MenuBarDC, MDI_RESTORE, ((1 == MDIButton) && (1 == m_nMDIButton))? 2 : 0);
						DrawMDIButton(&MenuBarDC, MDI_CLOSE,   ((2 == MDIButton) && (2 == m_nMDIButton))? 2 : 0);
					}
					else
					{
						DrawMDIButton(&MenuBarDC, MDI_MIN,     0);
						DrawMDIButton(&MenuBarDC, MDI_RESTORE, 0);
						DrawMDIButton(&MenuBarDC, MDI_CLOSE,   0);
					}
				}
				else	// mouse moved without left mouse button held down
				{
					if (MDIButton >= 0)
					{
						DrawMDIButton(&MenuBarDC, MDI_MIN,     (0 == MDIButton)? 1 : 0);
						DrawMDIButton(&MenuBarDC, MDI_RESTORE, (1 == MDIButton)? 1 : 0);
						DrawMDIButton(&MenuBarDC, MDI_CLOSE,   (2 == MDIButton)? 1 : 0);
					}
					else
					{
						DrawMDIButton(&MenuBarDC, MDI_MIN,     0);
						DrawMDIButton(&MenuBarDC, MDI_RESTORE, 0);
						DrawMDIButton(&MenuBarDC, MDI_CLOSE,   0);
					}
				}
			}
		}

		return FinalWindowProc(WM_MOUSEMOVE, wParam, lParam);
	}

	inline LRESULT CMenuBar::OnNotifyReflect(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);

		switch (((LPNMHDR)lParam)->code)
		{
		case TBN_DROPDOWN:		return OnTBNDropDown((LPNMTOOLBAR) lParam);
		case TBN_HOTITEMCHANGE:	return OnTBNHotItemChange((LPNMTBHOTITEM) lParam);
		}

		return 0L;
	}

	inline LRESULT CMenuBar::OnSysCommand(WPARAM wParam, LPARAM lParam)
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

		return FinalWindowProc(WM_SYSCOMMAND, wParam, lParam);
	}

	inline LRESULT CMenuBar::OnSysKeyDown(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);

		if ((VK_MENU == wParam) || (VK_F10 == wParam))
			return 0L;

		return FinalWindowProc(WM_SYSKEYDOWN, wParam, lParam);
	}

	inline LRESULT CMenuBar::OnSysKeyUp(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);

		if ((VK_MENU == wParam) || (VK_F10 == wParam))
		{
			ExitMenu();
			return 0L;
		}
		
		return FinalWindowProc(WM_SYSKEYUP, wParam, lParam);
	}

	inline LRESULT CMenuBar::OnTBNDropDown(LPNMTOOLBAR pNMTB)
	{
		UNREFERENCED_PARAMETER(pNMTB);

		// Always use PostMessage for USER_POPUPMENU (not SendMessage)
		PostMessage(UWM_POPUPMENU, 0L, 0L);

		return 0L;
	}

	inline LRESULT CMenuBar::OnTBNHotItemChange(LPNMTBHOTITEM pNMHI)
	{
		// This is the notification that a hot item change is about to occur
		// This is used to bring up a new popup menu when required
		
		CPoint pt = GetCursorPos();
		if (this == WindowFromPoint(pt))	// MenuBar window must be on top
		{
			DWORD flag = pNMHI->dwFlags;
			if ((flag & HICF_MOUSE) && !(flag & HICF_LEAVING))
			{
				int nButton = HitTest();
				if ((m_bMenuActive) && (nButton != m_nHotItem))
				{
					SendMessage(TB_PRESSBUTTON, m_nHotItem, MAKELONG(FALSE, 0));
					m_nHotItem = nButton;
					SendMessage(WM_CANCELMODE, 0L, 0L);

					//Always use PostMessage for USER_POPUPMENU (not SendMessage)
					PostMessage(UWM_POPUPMENU, 0L, 0L);
				}
				m_nHotItem = nButton;
			}

			// Handle escape from popup menu
			if ((flag & HICF_LEAVING) && m_bKeyMode)
			{
				m_nHotItem = pNMHI->idOld;
				PostMessage(TB_SETHOTITEM, m_nHotItem, 0L);
			}
		}

		return 0L;
	}
	
	inline LRESULT CMenuBar::OnWindowPosChanged(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);
		UNREFERENCED_PARAMETER(lParam);

		InvalidateRect(&m_MDIRect[0], TRUE);
		InvalidateRect(&m_MDIRect[1], TRUE);
		InvalidateRect(&m_MDIRect[2], TRUE);
		{
			CClientDC MenuBarDC(this);
			DrawAllMDIButtons(&MenuBarDC);
		}

		return FinalWindowProc(WM_WINDOWPOSCHANGED, wParam, lParam);
	}

	inline LRESULT CMenuBar::OnWindowPosChanging(WPARAM wParam, LPARAM lParam)
	{
		// Bypass CToolBar::WndProcDefault for this message
		return FinalWindowProc(WM_WINDOWPOSCHANGED, wParam, lParam);
	}

	inline void CMenuBar::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TBSTYLE_TOOLTIPS | TBSTYLE_LIST | TBSTYLE_FLAT | CCS_NODIVIDER | CCS_NORESIZE;
	}

	inline void CMenuBar::PreRegisterClass(WNDCLASS &wc)
	{
		// Set the Window Class
		wc.lpszClassName =  TOOLBARCLASSNAME;
	}

	inline void CMenuBar::ReleaseFocus()
	{
		if (m_hPrevFocus)
			::SetFocus(m_hPrevFocus);

		m_hPrevFocus = NULL;
		::ReleaseCapture();
	}

	inline void CMenuBar::SetHotItem(int nHot)
	{
		m_nHotItem = nHot;
		SendMessage(TB_SETHOTITEM, m_nHotItem, 0L);
	}

	inline void CMenuBar::SetMenu(HMENU hMenu)
	{
		assert(::IsWindow(m_hWnd));

		m_hTopMenu = hMenu;
		int nMaxedOffset = (IsMDIChildMaxed()? 1:0);

		// Remove any existing buttons
		while (SendMessage(TB_BUTTONCOUNT,  0L, 0L) > 0)
		{
			if(!SendMessage(TB_DELETEBUTTON, 0L, 0L))
				break;
		}

		// Set the Bitmap size to zero
		SendMessage(TB_SETBITMAPSIZE, 0L, MAKELPARAM(0, 0));

		if (IsMDIChildMaxed())
		{
			// Create an extra button for the MDI child system menu
			// Later we will custom draw the window icon over this button
			TBBUTTON tbb = {0};
			tbb.fsState = TBSTATE_ENABLED;
			tbb.fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE ;
			tbb.iString = (INT_PTR)_T(" ");
			SendMessage(TB_ADDBUTTONS, 1, (WPARAM)&tbb);
			SetButtonText(0, _T("    "));
		}

		for (int i = 0 ; i < ::GetMenuItemCount(hMenu); ++i)
		{
			// Assign the ToolBar Button struct
			TBBUTTON tbb = {0};
			tbb.idCommand = i  + nMaxedOffset;	// Each button needs a unique ID
			tbb.fsState = TBSTATE_ENABLED;
			tbb.fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE | TBSTYLE_DROPDOWN;
			tbb.iString = (INT_PTR)_T(" ");
			SendMessage(TB_ADDBUTTONS, 1, (WPARAM)&tbb);

			// Add the menu title to the string table
			std::vector<TCHAR> vMenuName( MAX_MENU_STRING+1, _T('\0') );
			TCHAR* szMenuName = &vMenuName[0];
			GetMenuString(hMenu, i, szMenuName, MAX_MENU_STRING, MF_BYPOSITION);
			SetButtonText(i  + nMaxedOffset, szMenuName);
		}
	}

	inline LRESULT CALLBACK CMenuBar::StaticMsgHook(int nCode, WPARAM wParam, LPARAM lParam)
	{
		assert(GetApp());
		MSG* pMsg = (MSG*)lParam;
		TLSData* pTLSData = (TLSData*)TlsGetValue(GetApp()->GetTlsIndex());
		assert(pTLSData);
		CMenuBar* pMenuBar = (CMenuBar*)pTLSData->pMenuBar;

		if (pMenuBar && (MSGF_MENU == nCode))
		{
			// process menu message
			if (pMenuBar->OnMenuInput(pMsg->message, pMsg->wParam, pMsg->lParam))
			{
				return TRUE;
			}
		}

		return CallNextHookEx(pTLSData->hHook, nCode, wParam, lParam);
	}

	inline LRESULT CMenuBar::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CHAR:				return 0L;  // Discard these messages
		case WM_DRAWITEM:			return OnDrawItem(wParam, lParam);
		case WM_EXITMENULOOP:		return OnExitMenuLoop(wParam, lParam);
		case WM_INITMENUPOPUP:		return OnInitMenuPopup(wParam, lParam);
		case WM_KEYDOWN:			return OnKeyDown(wParam, lParam);
		case WM_KILLFOCUS:			return OnKillFocus(wParam, lParam);
		case WM_LBUTTONDOWN:		return OnLButtonDown(wParam, lParam);
		case WM_LBUTTONUP:			return OnLButtonUp(wParam, lParam);
		case WM_MEASUREITEM:		return OnMeasureItem(wParam, lParam);
		case WM_MOUSELEAVE:			return OnMouseLeave(wParam, lParam);
		case WM_MOUSEMOVE:			return OnMouseMove(wParam, lParam);
		case WM_SYSKEYDOWN:			return OnSysKeyDown(wParam, lParam);
		case WM_SYSKEYUP:			return OnSysKeyUp(wParam, lParam);
		case WM_WINDOWPOSCHANGED:	return OnWindowPosChanged(wParam, lParam);
		case WM_WINDOWPOSCHANGING:	return OnWindowPosChanging(wParam, lParam);

		// Messages defined by Win32++
		case UWM_POPUPMENU:			return OnPopupMenu();

		} // switch (uMsg)

		return CToolBar::WndProcDefault(uMsg, wParam, lParam);
	} // LRESULT CMenuBar::WndProcDefault(...)


	inline CMenuMetrics::CMenuMetrics(CFrame* pFrame) : m_hTheme(0), m_hmodUXTheme(0), m_pfnCloseThemeData(0), m_pfnDrawThemeBackground(0),
												 m_pfnDrawThemeText(0), m_pfnGetThemePartSize(0), m_pfnGetThemeInt(0), m_pfnGetThemeMargins(0),
												 m_pfnGetThemeTextExtent(0), m_pfnIsThemeBGPartTransparent(0), m_pfnOpenThemeData(0)
	{
		assert(pFrame);
		m_pFrame = pFrame;

		Initialize();
	}

	inline void CMenuMetrics::Initialize()
	{
		assert(m_pFrame);

		if (!m_hmodUXTheme)
			m_hmodUXTheme = ::LoadLibrary(_T("UXTHEME.DLL"));

		if (m_hmodUXTheme)
		{
			m_pfnCloseThemeData			  = (CLOSETHEMEDATA*)::GetProcAddress(m_hmodUXTheme, "CloseThemeData");
			m_pfnDrawThemeBackground	  = (DRAWTHEMEBACKGROUND*)::GetProcAddress(m_hmodUXTheme, "DrawThemeBackground");
			m_pfnDrawThemeText			  = (DRAWTHEMETEXT*)::GetProcAddress(m_hmodUXTheme, "DrawThemeText");
			m_pfnGetThemePartSize		  = (GETTHEMEPARTSIZE*)::GetProcAddress(m_hmodUXTheme, "GetThemePartSize");
			m_pfnGetThemeInt			  = (GETTHEMEINT*)::GetProcAddress(m_hmodUXTheme, "GetThemeInt");
			m_pfnGetThemeMargins		  = (GETTHEMEMARGINS*)::GetProcAddress(m_hmodUXTheme, "GetThemeMargins");
			m_pfnGetThemeTextExtent		  = (GETTHEMETEXTEXTENT*)::GetProcAddress(m_hmodUXTheme, "GetThemeTextExtent");
			m_pfnIsThemeBGPartTransparent = (ISTHEMEBGPARTTRANSPARENT*)::GetProcAddress(m_hmodUXTheme, "IsThemeBackgroundPartiallyTransparent");
			m_pfnOpenThemeData			  = (OPENTHEMEDATA*)::GetProcAddress(m_hmodUXTheme, "OpenThemeData");
		}

		if (m_hTheme)
		{
			CloseThemeData();
			m_hTheme = 0;
		}

		m_hTheme = OpenThemeData(*m_pFrame, VSCLASS_MENU);

		if (m_hTheme)
		{
			int iBorderSize = 0;	// Border space between item text and accelerator
			int iBgBorderSize = 0;	// Border space between item text and gutter
			GetThemePartSize(NULL, MENU_POPUPCHECK, 0, NULL, TS_TRUE, &m_sizeCheck);
			GetThemePartSize(NULL, MENU_POPUPSEPARATOR, 0, NULL, TS_TRUE, &m_sizeSeparator);
			GetThemeInt(MENU_POPUPITEM, 0, TMT_BORDERSIZE, &iBorderSize);
			GetThemeInt(MENU_POPUPBACKGROUND, 0, TMT_BORDERSIZE, &iBgBorderSize);
			GetThemeMargins(NULL, MENU_POPUPCHECK, 0, TMT_CONTENTMARGINS, NULL, &m_marCheck);
			GetThemeMargins(NULL, MENU_POPUPCHECKBACKGROUND, 0, TMT_CONTENTMARGINS, NULL, &m_marCheckBackground);
			GetThemeMargins(NULL, MENU_POPUPITEM, 0, TMT_CONTENTMARGINS, NULL, &m_marItem);

			// Popup text margins
			m_marText = m_marItem;
			m_marText.cxRightWidth = iBorderSize;
			m_marText.cxLeftWidth = iBgBorderSize;
		}
		else
		{
			m_sizeCheck.SetSize(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON));
			m_sizeSeparator.SetSize(1, 7);
			m_marCheck.SetMargins(4, 4, 2, 2);
			m_marCheckBackground.SetMargins(0, 0, 0, 0);
			m_marItem.SetMargins(0, 0, 0, 0);
			m_marText.SetMargins(8, 16, 0, 0);
		}
	}

	inline CMenuMetrics::~CMenuMetrics()
	{
		if (m_hTheme)
			CloseThemeData();

		if (m_hmodUXTheme)
			::FreeLibrary(m_hmodUXTheme);
	}

	inline HRESULT CMenuMetrics::CloseThemeData()
	{
		if (m_pfnCloseThemeData)
			return (*m_pfnCloseThemeData)(m_hTheme);

		return E_NOTIMPL;
	}

	inline HRESULT CMenuMetrics::DrawThemeBackground(HDC hdc, int iPartId, int iStateId, const RECT *pRect, const RECT *pClipRect)
	{
		if (m_pfnDrawThemeBackground)
			return (*m_pfnDrawThemeBackground)(m_hTheme, hdc, iPartId, iStateId, pRect, pClipRect);

		return E_NOTIMPL;
	}

	inline HRESULT CMenuMetrics::DrawThemeText(HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect)
	{
		if (m_pfnDrawThemeText)
			return (*m_pfnDrawThemeText)(m_hTheme, hdc, iPartId, iStateId, pszText, iCharCount, dwTextFlags, dwTextFlags2, pRect);

		return E_NOTIMPL;
	}

	inline HRESULT CMenuMetrics::GetThemePartSize(HDC hdc, int iPartId, int iStateId, LPCRECT prc, THEMESIZE eSize, SIZE* psz)
	{
		if (m_pfnGetThemePartSize)
			return (*m_pfnGetThemePartSize)(m_hTheme, hdc, iPartId, iStateId, prc, eSize, psz);

		return E_NOTIMPL;
	}

	inline HRESULT CMenuMetrics::GetThemeInt(int iPartId, int iStateId, int iPropId, int* piVal)
	{
		if (m_pfnGetThemeInt)
			return (*m_pfnGetThemeInt)(m_hTheme, iPartId, iStateId, iPropId, piVal);

		return E_NOTIMPL;
	}

	inline HRESULT CMenuMetrics::GetThemeMargins(HDC hdc, int iPartId, int iStateId, int iPropId, LPRECT prc, MARGINS* pMargins)
	{
		if (m_pfnGetThemeMargins)
			return (*m_pfnGetThemeMargins)(m_hTheme, hdc, iPartId, iStateId, iPropId, prc, pMargins);

		return E_NOTIMPL;
	}

	inline HRESULT CMenuMetrics::GetThemeTextExtent(HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, LPCRECT pBoundingRect, LPRECT pExtentRect)
	{
		if (m_pfnGetThemeTextExtent)
			return (*m_pfnGetThemeTextExtent)(m_hTheme, hdc, iPartId, iStateId, pszText, iCharCount, dwTextFlags, pBoundingRect, pExtentRect);

		return E_NOTIMPL;
	}

	inline BOOL CMenuMetrics::IsThemeBackgroundPartiallyTransparent(int iPartId, int iStateId)
	{
		if (m_pfnIsThemeBGPartTransparent)
			return (*m_pfnIsThemeBGPartTransparent)(m_hTheme, iPartId, iStateId);

		return E_NOTIMPL;
	}

	inline HANDLE CMenuMetrics::OpenThemeData(HWND hwnd, LPCWSTR pszClassList)
	{
		if (m_pfnOpenThemeData)
			return (*m_pfnOpenThemeData)(hwnd, pszClassList);

		return NULL;
	}

	inline CRect CMenuMetrics::GetCheckBackgroundRect(CRect rcItem)
	{
		int x = rcItem.left + m_marCheckBackground.cxLeftWidth;
		int y = rcItem.top  + m_marCheckBackground.cyTopHeight;
		int cx = m_sizeCheck.cx + m_marCheck.Width();
		int cy = m_sizeCheck.cy + m_marCheck.Height();

		return CRect(x, y, x + cx, y + cy);
	}

	inline CRect CMenuMetrics::GetGutterRect(CRect rcItem)
	{
		int x = rcItem.left;
		int y = rcItem.top;
		int cx = m_marItem.cxLeftWidth + m_marCheckBackground.Width() + m_marCheck.Width() + m_sizeCheck.cx;
		int cy = rcItem.Height();

		return CRect(x, y, x + cx, y + cy);
	}

	inline CRect CMenuMetrics::GetCheckRect(CRect rcItem)
	{
		int x = rcItem.left + m_marCheckBackground.cxLeftWidth + m_marCheck.cxLeftWidth;
		int y = rcItem.top  + m_marCheckBackground.cyTopHeight + m_marCheck.cyTopHeight;

		return CRect(x, y, x + m_sizeCheck.cx, y + m_sizeCheck.cy);
	}

	inline CRect CMenuMetrics::GetSelectionRect(CRect rcItem)
	{
		int x = rcItem.left + m_marItem.cxLeftWidth;
		int y = rcItem.top;

		return CRect(x, y, rcItem.right - m_marItem.cxRightWidth, y + rcItem.Height());
	}

	inline CRect CMenuMetrics::GetSeperatorRect(CRect rcItem)
	{
		int left = GetGutterRect(rcItem).right;
		int top  = rcItem.top;
		int right = rcItem.right - m_marItem.cxRightWidth;
		int bottom = rcItem.top + m_sizeSeparator.cy;

		return CRect(left, top, right, bottom);
	}

	inline CSize CMenuMetrics::GetTextSize(MenuItemData* pmd)
	{
		CSize sizeText;
		assert(m_pFrame);
		CClientDC DesktopDC(NULL);
		LPCTSTR szItemText = pmd->GetItemText();

		if (IsAeroThemed())
		{
			CRect rcText;
			GetThemeTextExtent(DesktopDC, MENU_POPUPITEM, 0, T2W(szItemText), lstrlen(szItemText),
						   DT_LEFT | DT_SINGLELINE, NULL, &rcText);

			sizeText.SetSize(rcText.right + m_marText.Width(), rcText.bottom + m_marText.Height());
		}
		else
		{
			// Get the font used in menu items
			NONCLIENTMETRICS nm = {0};
			nm.cbSize = GetSizeofNonClientMetrics();
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(nm), &nm, 0);

			// Default menu items are bold, so take this into account
			if ((int)::GetMenuDefaultItem(pmd->hMenu, TRUE, GMDI_USEDISABLED) != -1)
				nm.lfMenuFont.lfWeight = FW_BOLD;

			// Calculate the size of the text
			DesktopDC.CreateFontIndirect(&nm.lfMenuFont);
			sizeText = DesktopDC.GetTextExtentPoint32(szItemText, lstrlen(szItemText));
		//	sizeText.cx += m_marText.Width();
			sizeText.cx += m_marText.cxRightWidth;
			sizeText.cy += m_marText.Height();
		}

		if (_tcschr(szItemText, _T('\t')))
			sizeText.cx += 8;	// Add POST_TEXT_GAP if the text includes a tab

		return sizeText;
	}

	inline CRect CMenuMetrics::GetTextRect(CRect rcItem)
	{
		int left    = GetGutterRect(rcItem).Width() + m_marText.cxLeftWidth;
		int top     = rcItem.top + m_marText.cyTopHeight;
		int right   = rcItem.right - m_marItem.cxRightWidth - m_marText.cxRightWidth;
		int bottom	= rcItem.bottom - m_marText.cyBottomHeight;

		return CRect(left, top, right, bottom);
	}

	inline int CMenuMetrics::ToItemStateId(UINT uItemState)
	{
		const bool      fDisabled   = ((uItemState & (ODS_INACTIVE | ODS_DISABLED)) != 0);
		const bool      fHot        = ((uItemState & (ODS_HOTLIGHT | ODS_SELECTED)) != 0);
		POPUPITEMSTATES iState;

		if (fDisabled)
			iState = (fHot ? MPI_DISABLEDHOT : MPI_DISABLED);
		else if (fHot)
			iState = MPI_HOT;
		else
			iState= MPI_NORMAL;

		return iState;
	}

	inline int CMenuMetrics::ToCheckBackgroundStateId(int iStateId)
	{
		POPUPCHECKBACKGROUNDSTATES iStateIdCheckBackground;

		// Determine the check background state.
		if (iStateId == MPI_DISABLED || iStateId == MPI_DISABLEDHOT)
			iStateIdCheckBackground = MCB_DISABLED;
		else
			iStateIdCheckBackground = MCB_NORMAL;

		return iStateIdCheckBackground;
	}

	inline int CMenuMetrics::ToCheckStateId(UINT fType, int iStateId)
	{
		POPUPCHECKSTATES iStateIdCheck;

		if (fType & MFT_RADIOCHECK)
		{
			if (iStateId == MPI_DISABLED || iStateId == MPI_DISABLEDHOT)
				iStateIdCheck = MC_BULLETDISABLED;
			else
				iStateIdCheck = MC_BULLETNORMAL;
		}
		else
		{
			if (iStateId == MPI_DISABLED || iStateId == MPI_DISABLEDHOT)
				iStateIdCheck = MC_CHECKMARKDISABLED;
			else
				iStateIdCheck = MC_CHECKMARKNORMAL;
		}

		return iStateIdCheck;
	}


	///////////////////////////////////
	// Definitions for the CFrame class
	//
	inline CFrame::CFrame() : m_pMenuMetrics(0), m_bShowIndicatorStatus(TRUE), m_bShowMenuStatus(TRUE), m_bUseThemes(TRUE), 
		                      m_bUseToolBar(TRUE), m_bShowStatusBar(TRUE), m_bShowToolBar(TRUE), m_AboutDialog(IDW_ABOUT),
						      m_pView(NULL), m_nMaxMRU(0), m_hOldFocus(0), m_nOldID(-1), m_bDrawArrowBkgrnd(FALSE)
	{
		ZeroMemory(&m_MenuBarTheme, sizeof(m_MenuBarTheme));
		ZeroMemory(&m_ReBarTheme, sizeof(m_ReBarTheme));
		ZeroMemory(&m_ToolBarTheme, sizeof(m_ToolBarTheme));

		m_strStatusText = LoadString(IDW_READY);

		// Do either InitCommonControls or InitCommonControlsEx
		LoadCommonControls();

		// By default, we use the rebar if we can
		m_bUseReBar = (GetComCtlVersion() > 470)? TRUE : FALSE;

		// Set the fonts
		NONCLIENTMETRICS nm = {0};
		nm.cbSize = GetSizeofNonClientMetrics();
		SystemParametersInfo (SPI_GETNONCLIENTMETRICS, 0, &nm, 0);
		m_fntMenuBar.CreateFontIndirect(&nm.lfMenuFont);
		m_fntStatusBar.CreateFontIndirect(&nm.lfStatusFont);
	}

	inline CFrame::~CFrame()
	{
	}

	inline BOOL CFrame::AddMenuIcon(int nID_MenuItem, HICON hIcon)
	{
		// Create a new ImageList if required
		if (NULL == m_imlMenu.GetHandle())
		{
			m_imlMenu.Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 0);
			m_vMenuIcons.clear();
		}

		if (m_imlMenu.Add(hIcon) != -1)
		{
			m_vMenuIcons.push_back(nID_MenuItem);

			// Recreate the Disabled imagelist
			m_imlMenuDis.DeleteImageList();
			m_imlMenuDis.Attach( CreateDisabledImageList(m_imlMenu.GetHandle() ) );

			return TRUE;
		}

		return FALSE;
	}

	inline UINT CFrame::AddMenuIcons(const std::vector<UINT>& MenuData, COLORREF crMask, UINT ToolBarID, UINT ToolBarDisabledID)
	// Adds the icons from a bitmap resource to an internal ImageList for use with popup menu items.
	// Note:  If existing are a different size to the new ones, the old ones will be removed!
	//        The ToolBarDisabledID is ignored unless ToolBarID and ToolBarDisabledID bitmaps are the same size.
	{
		// Count the MenuData entries excluding separators
		int iImages = 0;
		for (UINT i = 0 ; i < MenuData.size(); ++i)
		{
			if (MenuData[i] != 0)	// Don't count separators
			{
				++iImages;
			}
		}

		// Load the button images from Resource ID
		CBitmap Bitmap(ToolBarID);

		if ((0 == iImages) || (!Bitmap))
			return (UINT)m_vMenuIcons.size();	// No valid images, so nothing to do!

		BITMAP bm = Bitmap.GetBitmapData();
		int iImageHeight = bm.bmHeight;
		int iImageWidth  = MAX(bm.bmHeight, 16);		

		// Create the ImageList if required
		if (NULL == m_imlMenu.GetHandle() )
		{
			m_imlMenu.Create(iImageWidth, iImageHeight, ILC_COLOR32 | ILC_MASK, iImages, 0);
			m_vMenuIcons.clear();
		}
		else
		{
			int Oldcx;
			int Oldcy;

			m_imlMenu.GetIconSize(&Oldcx, &Oldcy);
			if (iImageHeight != Oldcy)
			{
				TRACE("Unable to add icons. The new icons are a different size to the old ones\n");
				return (UINT)m_vMenuIcons.size();
			}
		}

		// Add the resource IDs to the m_vMenuIcons vector
		for (UINT j = 0 ; j < MenuData.size(); ++j)
		{
			if (MenuData[j] != 0)
			{
				m_vMenuIcons.push_back(MenuData[j]);
			}
		}

		// Add the images to the ImageList
		m_imlMenu.Add(&Bitmap, crMask);

		// Create the Disabled imagelist
		if (ToolBarDisabledID)
		{
			m_imlMenuDis.DeleteImageList();
			m_imlMenuDis.Create(iImageWidth, iImageHeight, ILC_COLOR32 | ILC_MASK, iImages, 0);

			CBitmap BitmapDisabled(ToolBarDisabledID);
			BITMAP bmDis = BitmapDisabled.GetBitmapData();

			int iImageWidthDis  = bmDis.bmWidth / iImages;
			int iImageHeightDis = bmDis.bmHeight;

			// Normal and Disabled icons must be the same size
			if ((iImageWidthDis == iImageWidth) && (iImageHeightDis == iImageHeight))
			{
				m_imlMenuDis.Add(&BitmapDisabled, crMask);
			}
			else
			{
				m_imlMenuDis.Attach( CreateDisabledImageList(m_imlMenu.GetHandle() ) );
			}
		}
		else
		{
			m_imlMenuDis.DeleteImageList();
			m_imlMenuDis.Attach( CreateDisabledImageList(m_imlMenu.GetHandle() ) );
		}

		// return the number of menu icons
		return (UINT)m_vMenuIcons.size();
	}

	inline void CFrame::AddMenuBarBand()
	{
		// Adds a MenuBar to the rebar control
		REBARBANDINFO rbbi = {0};
		CSize sz = GetMenuBar()->GetMaxSize();

		// Calculate the MenuBar height from the menu font
		CSize csMenuBar;
		CClientDC dcMenuBar(GetMenuBar());
		dcMenuBar.SelectObject(GetMenuBar()->GetFont());
		csMenuBar = dcMenuBar.GetTextExtentPoint32(_T("\tSomeText"), lstrlen(_T("\tSomeText")));
		int MenuBar_Height = csMenuBar.cy + 6;

		rbbi.fMask      = RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_CHILD | RBBIM_SIZE | RBBIM_ID;
		rbbi.cxMinChild = sz.cx;
		rbbi.cx         = sz.cx;
		rbbi.cyMinChild = MenuBar_Height;
		rbbi.cyMaxChild = MenuBar_Height;
		rbbi.fStyle     = RBBS_BREAK | RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS ;
		rbbi.hwndChild  = GetMenuBar()->GetHwnd();
		rbbi.wID        = IDW_MENUBAR;

		// Note: rbbi.cbSize is set inside the InsertBand function
		GetReBar()->InsertBand(-1, rbbi);
		SetMenuBarBandSize();
		GetReBar()->SetMenuBar(GetMenuBar()->GetHwnd());

		if (m_ReBarTheme.LockMenuBand)
			GetReBar()->ShowGripper(GetReBar()->GetBand(GetMenuBar()->GetHwnd()), FALSE);
	}

	inline void CFrame::AddMRUEntry(LPCTSTR szMRUEntry)
	{
		// Erase possible duplicate entries from vector
		RemoveMRUEntry(szMRUEntry);

		// Insert the entry at the beginning of the vector
		m_vMRUEntries.insert(m_vMRUEntries.begin(), szMRUEntry);

		// Delete excessive MRU entries
		if (m_vMRUEntries.size() > m_nMaxMRU)
			m_vMRUEntries.erase(m_vMRUEntries.begin() + m_nMaxMRU, m_vMRUEntries.end());

		UpdateMRUMenu();
	}

	inline void CFrame::AddToolBarBand(CToolBar* pTB, DWORD dwStyle, UINT nID)
	{
		// Adds a ToolBar to the rebar control

		// Create the ToolBar Window
		pTB->Create(GetReBar());

		// Fill the REBARBAND structure
		REBARBANDINFO rbbi = {0};
		CSize sz = pTB->GetMaxSize();

		rbbi.fMask      = RBBIM_CHILDSIZE | RBBIM_STYLE |  RBBIM_CHILD | RBBIM_SIZE | RBBIM_ID;
		rbbi.cyMinChild = sz.cy;
		rbbi.cyMaxChild = sz.cy;
		rbbi.cx         = sz.cx +2;
		rbbi.cxMinChild = sz.cx +2;

		rbbi.fStyle     = dwStyle;
		rbbi.hwndChild  = pTB->GetHwnd();
		rbbi.wID        = nID;

		// Note: rbbi.cbSize is set inside the InsertBand function
		GetReBar()->InsertBand(-1, rbbi);
	}

	inline void CFrame::AddToolBarButton(UINT nID, BOOL bEnabled /* = TRUE*/, LPCTSTR szText /* = 0 */, int iImage /* = -1 */)
	// Adds Resource IDs to toolbar buttons.
	// A resource ID of 0 is a separator
	{
		m_vToolBarData.push_back(nID);

		GetToolBar()->AddButton(nID, bEnabled, iImage);

		if(0 != szText)
			GetToolBar()->SetButtonText(nID, szText);

		if (!IsWindow()) TRACE("Warning ... Resource IDs for toolbars should be added in SetupToolBar\n");
	}

	inline void CFrame::AdjustFrameRect(RECT rcView)
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

	inline void CFrame::CreateToolBar()
	{
		if (IsReBarSupported() && m_bUseReBar)
			AddToolBarBand(GetToolBar(), RBBS_BREAK, IDW_TOOLBAR);	// Create the toolbar inside rebar
		else
			GetToolBar()->Create(this);	// Create the toolbar without a rebar

		SetupToolBar();

		if (IsReBarSupported() && m_bUseReBar)
		{
			if (m_ReBarTheme.UseThemes && m_ReBarTheme.LockMenuBand)
			{
				// Hide gripper for single toolbar
				if (GetReBar()->GetBandCount() <= 2)
					GetReBar()->ShowGripper(GetReBar()->GetBand(GetToolBar()->GetHwnd()), FALSE);
			}
		}

		if (m_vToolBarData.size() > 0)
		{
			// Set the toolbar images (if not already set in SetupToolBar)
			// A mask of 192,192,192 is compatible with AddBitmap (for Win95)
			if (!GetToolBar()->SendMessage(TB_GETIMAGELIST,  0L, 0L))
				SetToolBarImages(RGB(192,192,192), IDW_MAIN, 0, 0);

			// Add the icons for popup menu
			AddMenuIcons(m_vToolBarData, RGB(192, 192, 192), IDW_MAIN, 0);
		}
		else
		{
			TRACE("Warning ... No resource IDs assigned to the toolbar\n");
		}
	}

	inline LRESULT CFrame::CustomDrawMenuBar(NMHDR* pNMHDR)
	// CustomDraw is used to render the MenuBar's toolbar buttons
	{
		if (m_MenuBarTheme.UseThemes)
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
					CDC* pDrawDC = FromHandle(lpNMCustomDraw->nmcd.hdc);
					CRect rcRect = lpNMCustomDraw->nmcd.rc;
					int nState = lpNMCustomDraw->nmcd.uItemState;
					DWORD dwItem = (DWORD)lpNMCustomDraw->nmcd.dwItemSpec;

					// Leave a pixel gap above and below the drawn rectangle
					if (IsAeroThemed())
						rcRect.InflateRect(0, -2);
					else
						rcRect.InflateRect(0, -1);

					if (GetMenuBar()->IsMDIChildMaxed() && (0 == dwItem))
					// Draw over MDI Max button
					{
						HICON hIcon = (HICON)GetMenuBar()->GetActiveMDIChild()->SendMessage(WM_GETICON, ICON_SMALL, 0L);
						if (NULL == hIcon)
							hIcon = ::LoadIcon(NULL, IDI_APPLICATION);

						int cx = ::GetSystemMetrics (SM_CXSMICON);
						int cy = ::GetSystemMetrics (SM_CYSMICON);
						int y = 1 + (GetMenuBar()->GetWindowRect().Height() - cy)/2;
						int x = (rcRect.Width() - cx)/2;
						pDrawDC->DrawIconEx(x, y, hIcon, cx, cy, 0, NULL, DI_NORMAL);

						return CDRF_SKIPDEFAULT;  // No further drawing
					}

					else if (nState & (CDIS_HOT | CDIS_SELECTED))
					{
						if ((nState & CDIS_SELECTED) || (GetMenuBar()->GetButtonState(dwItem) & TBSTATE_PRESSED))
						{
							pDrawDC->GradientFill(m_MenuBarTheme.clrPressed1, m_MenuBarTheme.clrPressed2, rcRect, FALSE);
						}
						else if (nState & CDIS_HOT)
						{
							pDrawDC->GradientFill(m_MenuBarTheme.clrHot1, m_MenuBarTheme.clrHot2, rcRect, FALSE);
						}

						// Draw border
						pDrawDC->CreatePen(PS_SOLID, 1, m_MenuBarTheme.clrOutline);
						pDrawDC->MoveTo(rcRect.left, rcRect.bottom);
						pDrawDC->LineTo(rcRect.left, rcRect.top);
						pDrawDC->LineTo(rcRect.right-1, rcRect.top);
						pDrawDC->LineTo(rcRect.right-1, rcRect.bottom);
						pDrawDC->MoveTo(rcRect.right-1, rcRect.bottom);
						pDrawDC->LineTo(rcRect.left, rcRect.bottom);

						CString str;
						int nLength = (int)GetMenuBar()->SendMessage(TB_GETBUTTONTEXT, lpNMCustomDraw->nmcd.dwItemSpec, 0L);
						if (nLength > 0) 
						{
							GetMenuBar()->SendMessage(TB_GETBUTTONTEXT, lpNMCustomDraw->nmcd.dwItemSpec, (LPARAM)str.GetBuffer(nLength));
							str.ReleaseBuffer();
						}

						// Draw highlight text
						CFont* pFont = GetMenuBar()->GetFont();
						pDrawDC->SelectObject(pFont);

						rcRect.bottom += 1;
						int iMode = pDrawDC->SetBkMode(TRANSPARENT);
						pDrawDC->DrawText(str, lstrlen(str), rcRect, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

						pDrawDC->SetBkMode(iMode);
						return CDRF_SKIPDEFAULT;  // No further drawing
					}
				}
				return CDRF_DODEFAULT ;   // Do default drawing

			// Painting cycle has completed
			case CDDS_POSTPAINT:
				// Draw MDI Minimise, Restore and Close buttons
				{
					CDC* pDrawDC = FromHandle(lpNMCustomDraw->nmcd.hdc);
					GetMenuBar()->DrawAllMDIButtons(pDrawDC);
				}
				break;
			}
		}
		return 0L;
	}

	inline LRESULT CFrame::CustomDrawToolBar(NMHDR* pNMHDR)
	// With CustomDraw we manually control the drawing of each toolbar button
	{
		if (m_ToolBarTheme.UseThemes)
		{
			LPNMTBCUSTOMDRAW lpNMCustomDraw = (LPNMTBCUSTOMDRAW)pNMHDR;
			CToolBar* pTB = (CToolBar*)FromHandle(pNMHDR->hwndFrom);

			switch (lpNMCustomDraw->nmcd.dwDrawStage)
			{
			// Begin paint cycle
			case CDDS_PREPAINT:
				// Send NM_CUSTOMDRAW item draw, and post-paint notification messages.
				return CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT ;

			// An item is about to be drawn
			case CDDS_ITEMPREPAINT:
				{
					CDC* pDrawDC = FromHandle(lpNMCustomDraw->nmcd.hdc);
					CRect rcRect = lpNMCustomDraw->nmcd.rc;
					int nState = lpNMCustomDraw->nmcd.uItemState;
					DWORD dwItem = (DWORD)lpNMCustomDraw->nmcd.dwItemSpec;
					DWORD dwTBStyle = (DWORD)pTB->SendMessage(TB_GETSTYLE, 0L, 0L);
					int nStyle = pTB->GetButtonStyle(dwItem);

					int nButton = (int)pTB->SendMessage(TB_COMMANDTOINDEX, (WPARAM) dwItem, 0L);
					TBBUTTON tbb = {0};
					pTB->SendMessage(TB_GETBUTTON, nButton, (LPARAM)&tbb);
					int iImage = (int)tbb.iBitmap;

					// Calculate text size
					CString str;
					CSize TextSize;
					if (pTB->HasText())	// Does any button have text?
					{
						pDrawDC->SelectObject(pTB->GetFont());
						LRESULT lr = pTB->SendMessage(TB_GETBUTTONTEXT, dwItem, (LPARAM)str.GetBuffer(MAX_MENU_STRING));
						str.ReleaseBuffer();
						if (lr> 0)
						{
							TextSize = pDrawDC->GetTextExtentPoint32(str, lstrlen(str));
						}
					}


					// Draw outline rectangle
					if (nState & (CDIS_HOT | CDIS_SELECTED | CDIS_CHECKED))
					{
						pDrawDC->CreatePen(PS_SOLID, 1, m_ToolBarTheme.clrOutline);
						pDrawDC->MoveTo(rcRect.left, rcRect.top);
						pDrawDC->LineTo(rcRect.left, rcRect.bottom-1);
						pDrawDC->LineTo(rcRect.right-1, rcRect.bottom-1);
						pDrawDC->LineTo(rcRect.right-1, rcRect.top);
						pDrawDC->LineTo(rcRect.left, rcRect.top);
					}

					// Draw filled gradient background
					rcRect.InflateRect(-1, -1);
					if ((nState & (CDIS_SELECTED|CDIS_CHECKED)) || (pTB->GetButtonState(dwItem) & TBSTATE_PRESSED))
					{
						pDrawDC->GradientFill(m_ToolBarTheme.clrPressed1, m_ToolBarTheme.clrPressed2, rcRect, FALSE);
					}
					else if (nState & CDIS_HOT)
					{
						pDrawDC->GradientFill(m_ToolBarTheme.clrHot1, m_ToolBarTheme.clrHot2, rcRect, FALSE);
					}

					// Get the appropriate image list depending on the button state
					CImageList* pimlToolBar;
					if (nState & CDIS_DISABLED)
					{
						pimlToolBar = pTB->GetDisabledImageList();
					}
					else if (nState & (CDIS_HOT | CDIS_SELECTED | CDIS_CHECKED))
					{
						pimlToolBar = pTB->GetHotImageList();
						if (0 == pimlToolBar)
							pimlToolBar = pTB->GetImageList();
					}
					else
					{
						pimlToolBar = pTB->GetImageList();
					}

					BOOL IsWin95 = (1400 == (GetWinVersion()) || (2400 == GetWinVersion()));

					// Calculate image position
					int cxImage = 0;
					int cyImage = 0;
					pimlToolBar->GetIconSize(&cxImage, &cyImage);

					int yImage = (rcRect.bottom - rcRect.top - cyImage - TextSize.cy +2)/2;
					int xImage = (rcRect.right + rcRect.left - cxImage)/2 + ((nState & (CDIS_SELECTED|CDIS_CHECKED))? 1:0);
					if (dwTBStyle & TBSTYLE_LIST)
					{
						xImage = rcRect.left + (IsXPThemed()?2:4) + ((nState & CDIS_SELECTED)? 1:0);
						yImage = (rcRect.bottom -rcRect.top - cyImage +2)/2 + ((nState & (CDIS_SELECTED|CDIS_CHECKED))? 1:0);
					}

					// Handle the TBSTYLE_DROPDOWN and BTNS_WHOLEDROPDOWN styles
					if ((nStyle & TBSTYLE_DROPDOWN) || ((nStyle & 0x0080) && (!IsWin95)))
					{
						// Calculate the dropdown arrow position
						int xAPos = (nStyle & TBSTYLE_DROPDOWN)? rcRect.right -6 : (rcRect.right + rcRect.left + cxImage + 4)/2;
						int yAPos = (nStyle & TBSTYLE_DROPDOWN)? (rcRect.bottom - rcRect.top +1)/2 : (cyImage)/2;
						if (dwTBStyle & TBSTYLE_LIST)
						{
							xAPos = (nStyle & TBSTYLE_DROPDOWN)?rcRect.right -6:rcRect.right -5;
							yAPos =	(rcRect.bottom - rcRect.top +1)/2 + ((nStyle & TBSTYLE_DROPDOWN)?0:1);
						}

						xImage -= (nStyle & TBSTYLE_DROPDOWN)?((dwTBStyle & TBSTYLE_LIST)? (IsXPThemed()?-4:0):6):((dwTBStyle & TBSTYLE_LIST)? 0:4);

						// Draw separate background for dropdown arrow
						if ((m_bDrawArrowBkgrnd) && (nState & CDIS_HOT))
						{
							CRect rcArrowBkgnd = rcRect;
							rcArrowBkgnd.left = rcArrowBkgnd.right - 13;
							pDrawDC->GradientFill(m_ToolBarTheme.clrPressed1, m_ToolBarTheme.clrPressed2, rcArrowBkgnd, FALSE);
						}

						m_bDrawArrowBkgrnd = FALSE;

						// Manually draw the dropdown arrow
						pDrawDC->CreatePen(PS_SOLID, 1, RGB(0,0,0));
						for (int i = 2; i >= 0; --i)
						{
							pDrawDC->MoveTo(xAPos -i-1, yAPos - i+1);
							pDrawDC->LineTo(xAPos +i,   yAPos - i+1);
						}

						// Draw line between icon and dropdown arrow
						if ((nStyle & TBSTYLE_DROPDOWN) && ((nState & CDIS_SELECTED) || nState & CDIS_HOT))
						{
							pDrawDC->CreatePen(PS_SOLID, 1, m_ToolBarTheme.clrOutline);
							pDrawDC->MoveTo(rcRect.right - 13, rcRect.top);
							pDrawDC->LineTo(rcRect.right - 13, rcRect.bottom);
						}
					}

					// Draw the button image
					if (xImage > 0)
					{
						pimlToolBar->Draw(pDrawDC, iImage, CPoint(xImage, yImage), ILD_TRANSPARENT);
					}

					//Draw Text
					if (lstrlen(str) > 0)
					{
						int iWidth = rcRect.right - rcRect.left - ((nStyle & TBSTYLE_DROPDOWN)?13:0);
						CRect rcText(0, 0, MIN(TextSize.cx, iWidth), TextSize.cy);

						int xOffset = (rcRect.right + rcRect.left - rcText.right + rcText.left - ((nStyle & TBSTYLE_DROPDOWN)? 11 : 1))/2;
						int yOffset = yImage + cyImage +1;

						if (dwTBStyle & TBSTYLE_LIST)
						{
							xOffset = rcRect.left + cxImage + ((nStyle & TBSTYLE_DROPDOWN)?(IsXPThemed()?10:6): 6) + ((nState & CDIS_SELECTED)? 1:0);
							yOffset = (2+rcRect.bottom - rcRect.top - rcText.bottom + rcText.top)/2 + ((nState & CDIS_SELECTED)? 1:0);
							rcText.right = MIN(rcText.right,  rcRect.right - xOffset);
						}

						OffsetRect(&rcText, xOffset, yOffset);

						int iMode = pDrawDC->SetBkMode(TRANSPARENT);
						pDrawDC->SelectObject(pTB->GetFont());

						if (nState & (CDIS_DISABLED))
						{
							// Draw text twice for embossed look
							rcText.OffsetRect(1, 1);
							pDrawDC->SetTextColor(RGB(255,255,255));
							pDrawDC->DrawText(str, lstrlen(str), rcText, DT_LEFT);
							rcText.OffsetRect(-1, -1);
							pDrawDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
							pDrawDC->DrawText(str, lstrlen(str), rcText, DT_LEFT);
						}
						else
						{
							pDrawDC->SetTextColor(GetSysColor(COLOR_BTNTEXT));
							pDrawDC->DrawText(str, lstrlen(str), rcText, DT_LEFT | DT_END_ELLIPSIS);
						}
						pDrawDC->SetBkMode(iMode);

					}
				}
				return CDRF_SKIPDEFAULT;  // No further drawing
			}
		}
		return 0L;
	}

	inline void CFrame::DrawMenuItem(LPDRAWITEMSTRUCT pdis)
	{
		MenuItemData* pmid = (MenuItemData*)pdis->itemData;
		int iStateId = m_pMenuMetrics->ToItemStateId(pdis->itemState);
		MenuTheme* pMBT = GetMenuBarTheme();
		CDC* pDrawDC = FromHandle(pdis->hDC);

		if (IsAeroThemed() && m_pMenuMetrics->IsThemeBackgroundPartiallyTransparent(MENU_POPUPITEM, iStateId))
		{
			m_pMenuMetrics->DrawThemeBackground(pdis->hDC, MENU_POPUPBACKGROUND, 0, &pdis->rcItem, NULL);
		}

		// Draw the gutter
		CRect rcGutter = m_pMenuMetrics->GetGutterRect(pdis->rcItem);
		if (IsAeroThemed())
			m_pMenuMetrics->DrawThemeBackground(pdis->hDC, MENU_POPUPGUTTER, 0, &rcGutter, NULL);
		else
			pDrawDC->GradientFill(pMBT->clrPressed1, pMBT->clrPressed2, rcGutter, TRUE);

		if (pmid->mii.fType & MFT_SEPARATOR)
		{
			// Draw the separator
			if (IsAeroThemed())
			{
				CRect rcSeparator = m_pMenuMetrics->GetSeperatorRect(pdis->rcItem);
				m_pMenuMetrics->DrawThemeBackground(pdis->hDC, MENU_POPUPSEPARATOR, 0, &rcSeparator, NULL);
			}
			else
			{
				CRect rc = pdis->rcItem;
				CRect rcSep = pdis->rcItem;
				rcSep.left = m_pMenuMetrics->GetGutterRect(rc).Width();
				pDrawDC->SolidFill(RGB(255,255,255), rcSep);
				rcSep.top += (rc.bottom - rc.top)/2;
				pDrawDC->DrawEdge(rcSep,  EDGE_ETCHED, BF_TOP);
			}
		}
		else
		{
			// Draw the item background
			DrawMenuItemBkgnd(pdis);

			// Draw Checkmark or icon
			if (pdis->itemState & ODS_CHECKED)
				DrawMenuItemCheckmark(pdis);
			else
				DrawMenuItemIcon(pdis);

			// Draw the text.
			DrawMenuItemText(pdis);
		}

		if (IsAeroThemed())
		{
			// Draw the Submenu arrow
			if (pmid->mii.hSubMenu)
			{
				CRect rcSubMenu = pdis->rcItem;
				rcSubMenu.left = pdis->rcItem.right - m_pMenuMetrics->m_marItem.cxRightWidth - m_pMenuMetrics->m_marText.cxRightWidth;;
				m_pMenuMetrics->DrawThemeBackground(pdis->hDC, MENU_POPUPSUBMENU, m_pMenuMetrics->ToCheckStateId(pmid->mii.fType, iStateId), &rcSubMenu, NULL);
			}

			// Suppress further drawing to prevent an incorrect Submenu arrow being drawn
			CRect rc = pdis->rcItem;
			::ExcludeClipRect(pdis->hDC, rc.left, rc.top, rc.right, rc.bottom);
		}
	}

	inline void CFrame::DrawMenuItemBkgnd(LPDRAWITEMSTRUCT pdis)
	{
		// Draw the item background
		CRect rcSelection = m_pMenuMetrics->GetSelectionRect(pdis->rcItem);
		if (IsAeroThemed())
		{
			int iStateId = m_pMenuMetrics->ToItemStateId(pdis->itemState);
			m_pMenuMetrics->DrawThemeBackground(pdis->hDC, MENU_POPUPITEM, iStateId, &rcSelection, NULL);
		}
		else
		{
			BOOL bDisabled = pdis->itemState & ODS_GRAYED;
			BOOL bSelected = pdis->itemState & ODS_SELECTED;
			CRect rcDraw = pdis->rcItem;
			CDC* pDrawDC = FromHandle(pdis->hDC);
			MenuTheme* pMBT = GetMenuBarTheme();

			if ((bSelected) && (!bDisabled))
			{
				// draw selected item background
				pDrawDC->CreateSolidBrush(pMBT->clrHot1);
				pDrawDC->CreatePen(PS_SOLID, 1, pMBT->clrOutline);
				pDrawDC->Rectangle(rcDraw.left, rcDraw.top, rcDraw.right, rcDraw.bottom);
			}
			else
			{
				// draw non-selected item background
				rcDraw.left = m_pMenuMetrics->GetGutterRect(pdis->rcItem).Width();
				pDrawDC->SolidFill(RGB(255,255,255), rcDraw);
			}
		}
	}

	inline void CFrame::DrawMenuItemCheckmark(LPDRAWITEMSTRUCT pdis)
	// Draws the checkmark or radiocheck transparently
	{
		CRect rc = pdis->rcItem;
		UINT fType = ((MenuItemData*)pdis->itemData)->mii.fType;
		MenuTheme* pMBT = GetMenuBarTheme();
		CRect rcBk;
		CDC* pDrawDC = FromHandle(pdis->hDC);

		if (IsAeroThemed())
		{
			int iStateId = m_pMenuMetrics->ToItemStateId(pdis->itemState);
			MenuItemData* pmid = (MenuItemData*)pdis->itemData;
			CRect rcCheckBackground = m_pMenuMetrics->GetCheckBackgroundRect(pdis->rcItem);
			m_pMenuMetrics->DrawThemeBackground(pdis->hDC, MENU_POPUPCHECKBACKGROUND, m_pMenuMetrics->ToCheckBackgroundStateId(iStateId), &rcCheckBackground, NULL);
			CRect rcCheck = m_pMenuMetrics->GetCheckRect(pdis->rcItem);
			m_pMenuMetrics->DrawThemeBackground(pdis->hDC, MENU_POPUPCHECK, m_pMenuMetrics->ToCheckStateId(pmid->mii.fType, iStateId), &rcCheck, NULL);
		}
		else
		{
			// Draw the checkmark's background rectangle first
			int Iconx = m_pMenuMetrics->m_sizeCheck.cx;
			int Icony = m_pMenuMetrics->m_sizeCheck.cy;
			int left = m_pMenuMetrics->m_marCheck.cxLeftWidth;
			int top = rc.top + (rc.Height() - Icony)/2;
			rcBk.SetRect(left, top, left + Iconx, top + Icony);

			pDrawDC->CreateSolidBrush(pMBT->clrHot2);
			pDrawDC->CreatePen(PS_SOLID, 1, pMBT->clrOutline);

			// Draw the checkmark's background rectangle
			pDrawDC->Rectangle(rcBk.left, rcBk.top, rcBk.right, rcBk.bottom);

			CMemDC MemDC(FromHandle(pdis->hDC));
			int cxCheck = ::GetSystemMetrics(SM_CXMENUCHECK);
			int cyCheck = ::GetSystemMetrics(SM_CYMENUCHECK);
			MemDC.CreateBitmap(cxCheck, cyCheck, 1, 1, NULL);
			CRect rcCheck( 0, 0, cxCheck, cyCheck);

			// Copy the check mark bitmap to hdcMem
			if (MFT_RADIOCHECK == fType)
				MemDC.DrawFrameControl(rcCheck, DFC_MENU, DFCS_MENUBULLET);
			else
				MemDC.DrawFrameControl(rcCheck, DFC_MENU, DFCS_MENUCHECK);

			int xoffset = (rcBk.Width() - rcCheck.Width()-1)/2;
			int yoffset = (rcBk.Height() - rcCheck.Height()-1)/2;

			xoffset += 2;

			// Draw a white or black check mark as required
			// Unfortunately MaskBlt isn't supported on Win95, 98 or ME, so we do it the hard way
			CMemDC MaskDC(FromHandle(pdis->hDC));
			MaskDC.CreateCompatibleBitmap(FromHandle(pdis->hDC), cxCheck, cyCheck);
			MaskDC.BitBlt(0, 0, cxCheck, cyCheck, &MaskDC, 0, 0, WHITENESS);

			if ((pdis->itemState & ODS_SELECTED))  // && (!tm.UseThemes))
			{
				// Draw a white checkmark
				MemDC.BitBlt(0, 0, cxCheck, cyCheck, &MemDC, 0, 0, DSTINVERT);
				MaskDC.BitBlt(0, 0, cxCheck, cyCheck, &MemDC, 0, 0, SRCAND);
				pDrawDC->BitBlt(rcBk.left + xoffset, rcBk.top + yoffset, cxCheck, cyCheck, &MaskDC, 0, 0, SRCPAINT);
			}
			else
			{
				// Draw a black checkmark
				int BullitOffset = ((MFT_RADIOCHECK == fType)/* && tm.UseThemes*/)? 1 : 0;
				MaskDC.BitBlt( -BullitOffset, BullitOffset, cxCheck, cyCheck, &MemDC, 0, 0, SRCAND);
				pDrawDC->BitBlt(rcBk.left + xoffset, rcBk.top + yoffset, cxCheck, cyCheck, &MaskDC, 0, 0, SRCAND);
			}
		}
	}

	inline void CFrame::DrawMenuItemIcon(LPDRAWITEMSTRUCT pdis)
	{
		if ( 0 == m_imlMenu.GetHandle() )
			return;

		// Get icon size
		int Iconx = m_pMenuMetrics->m_sizeCheck.cx;
		int Icony = m_pMenuMetrics->m_sizeCheck.cy;

		// get the drawing rectangle
		CRect rc = pdis->rcItem;
		int left = m_pMenuMetrics->m_marCheck.cxLeftWidth;
		int top = rc.top + (rc.Height() - Icony)/2;
		rc.SetRect(left, top, left + Iconx, top + Icony);

		// get the icon's location in the imagelist
		int iImage = -1;
		for (int i = 0 ; i < (int)m_vMenuIcons.size(); ++i)
		{
			if (pdis->itemID == m_vMenuIcons[i])
				iImage = i;
		}

		// draw the image
		if (iImage >= 0 )
		{
			BOOL bDisabled = pdis->itemState & ODS_GRAYED;
			CDC* pDC = FromHandle(pdis->hDC);
			if ((bDisabled) && (m_imlMenuDis.GetHandle()))
				m_imlMenuDis.Draw(pDC, iImage, CPoint(rc.left, rc.top), ILD_TRANSPARENT);
			else
				m_imlMenu.Draw(pDC, iImage, CPoint(rc.left, rc.top), ILD_TRANSPARENT);
		}
	}

	inline void CFrame::DrawMenuItemText(LPDRAWITEMSTRUCT pdis)
	{
		LPCTSTR ItemText = ((MenuItemData*)pdis->itemData)->GetItemText();
		BOOL bDisabled = pdis->itemState & ODS_GRAYED;
		COLORREF colorText = GetSysColor(bDisabled ?  COLOR_GRAYTEXT : COLOR_MENUTEXT);

		// Calculate the text rect size
		CRect rcText = m_pMenuMetrics->GetTextRect(pdis->rcItem);

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
		if (IsAeroThemed())
		{
			ULONG uAccel = ((pdis->itemState & ODS_NOACCEL) ? DT_HIDEPREFIX : 0);
			int iStateId = m_pMenuMetrics->ToItemStateId(pdis->itemState);

			// Draw the item text before the tab
			m_pMenuMetrics->DrawThemeText(pdis->hDC, MENU_POPUPITEM, iStateId, T2W(ItemText), nTab, DT_SINGLELINE | DT_LEFT | DT_VCENTER | uAccel, 0, &rcText);

			// Draw text after tab, right aligned
			if(nTab != -1)
				m_pMenuMetrics->DrawThemeText(pdis->hDC, MENU_POPUPITEM, iStateId, T2W(&ItemText[nTab + 1]), -1, DT_SINGLELINE | DT_RIGHT | DT_VCENTER | uAccel, 0, &rcText);
		}
		else
		{
			SetTextColor(pdis->hDC, colorText);
			int iMode = SetBkMode(pdis->hDC, TRANSPARENT);
			DrawText(pdis->hDC, ItemText, nTab, rcText, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

			// Draw text after tab, right aligned
			if(nTab != -1)
				DrawText(pdis->hDC, &ItemText[nTab + 1], -1, rcText, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

			SetBkMode(pdis->hDC, iMode);
		}
	}

	inline int CFrame::GetMenuItemPos(CMenu* pMenu, LPCTSTR szItem)
	// Returns the position of the menu item, given it's name
	{
		int nMenuItemCount = pMenu->GetMenuItemCount();
		MENUITEMINFO mii = {0};
		mii.cbSize = GetSizeofMenuItemInfo();

		for (int nItem = 0 ; nItem < nMenuItemCount; ++nItem)
		{
			std::vector<TCHAR> vTChar( MAX_MENU_STRING+1, _T('\0') );
			TCHAR* szStr = &vTChar[0];

			std::vector<TCHAR> vStripped( MAX_MENU_STRING+1, _T('\0') );
			TCHAR* szStripped = &vStripped[0];

			mii.fMask      = MIIM_TYPE;
			mii.fType      = MFT_STRING;
			mii.dwTypeData = szStr;
			mii.cch        = MAX_MENU_STRING;

			// Fill the contents of szStr from the menu item
			if (pMenu->GetMenuItemInfo(nItem, &mii, TRUE) && (lstrlen(szStr) <= MAX_MENU_STRING))
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

	inline CString CFrame::GetMRUEntry(UINT nIndex)
	{
		CString strPathName;
		if (nIndex < m_vMRUEntries.size())
		{
			strPathName = m_vMRUEntries[nIndex];

			// Now put the selected entry at Index 0
			AddMRUEntry(strPathName);
		}
		return strPathName;
	}

	inline CSize CFrame::GetTBImageSize(CBitmap* pbm)
	{
		assert(pbm);
		assert(pbm->GetHandle());
		BITMAP bmData = pbm->GetBitmapData();
		int cy = bmData.bmHeight;
		int cx = MAX(bmData.bmHeight, 16);

		return CSize(cx, cy);
	}

	inline CRect CFrame::GetViewRect() const
	{
		// Get the frame's client area
		CRect rcFrame = GetClientRect();

		// Get the statusbar's window area
		CRect rcStatus;
		if (GetStatusBar()->IsWindowVisible() || !IsWindowVisible())
			rcStatus = GetStatusBar()->GetWindowRect();

		// Get the top rebar or toolbar's window area
		CRect rcTop;
		if (IsReBarSupported() && m_bUseReBar)
			rcTop = GetReBar()->GetWindowRect();
		else
			if (GetToolBar()->IsWindow() && GetToolBar()->IsWindowVisible())
				rcTop = GetToolBar()->GetWindowRect();

		// Return client size less the rebar and status windows
		int top = rcFrame.top + rcTop.Height();
		int left = rcFrame.left;
		int right = rcFrame.right;
		int bottom = rcFrame.Height() - (rcStatus.Height());
		if ((bottom <= top) ||( right <= left))
			top = left = right = bottom = 0;

		CRect rcView(left, top, right, bottom);
		return rcView;
	}

	inline CString CFrame::GetThemeName() const
	{
		// Returns the XP theme name
		HMODULE hMod = ::LoadLibrary(_T("uxtheme.dll"));
		WCHAR ThemeName[31] = L"";
		if(hMod)
		{
			typedef HRESULT (__stdcall *PFNGETCURRENTTHEMENAME)(LPWSTR pszThemeFileName, int cchMaxNameChars,
				LPWSTR pszColorBuff, int cchMaxColorChars, LPWSTR pszSizeBuff, int cchMaxSizeChars);

			PFNGETCURRENTTHEMENAME pfn = (PFNGETCURRENTTHEMENAME)GetProcAddress(hMod, "GetCurrentThemeName");
			(*pfn)(0, 0, ThemeName, 30, 0, 0);

			::FreeLibrary(hMod);
		}

		return CString(ThemeName);
	}

/*	inline void CFrame::LoadCommonControls()
	{
		HMODULE hComCtl = 0;

		try
		{
			// Load the Common Controls DLL
			hComCtl = ::LoadLibrary(_T("COMCTL32.DLL"));
			if (!hComCtl)
				throw CWinException(_T("Failed to load COMCTL32.DLL"));

			if (GetComCtlVersion() > 470)
			{
				// Declare a pointer to the InItCommonControlsEx function
				typedef BOOL WINAPI INIT_EX(INITCOMMONCONTROLSEX*);
				INIT_EX* pfnInit = (INIT_EX*)::GetProcAddress(hComCtl, "InitCommonControlsEx");

				// Load the full set of common controls
				INITCOMMONCONTROLSEX InitStruct = {0};
				InitStruct.dwSize = sizeof(INITCOMMONCONTROLSEX);
				InitStruct.dwICC = ICC_COOL_CLASSES|ICC_DATE_CLASSES|ICC_INTERNET_CLASSES|ICC_NATIVEFNTCTL_CLASS|
							ICC_PAGESCROLLER_CLASS|ICC_USEREX_CLASSES|ICC_WIN95_CLASSES;

				// Call InitCommonControlsEx
				if(!((*pfnInit)(&InitStruct)))
					throw CWinException(_T("InitCommonControlsEx failed"));
			}
			else
			{
				::InitCommonControls();
			}

			::FreeLibrary(hComCtl);
		}

		catch (const CWinException &e)
		{
			e.what();
			if (hComCtl)
				::FreeLibrary(hComCtl);

			throw;
		}
	}
*/
	inline BOOL CFrame::LoadRegistryMRUSettings(UINT nMaxMRU /*= 0*/)
	{
		// Load the MRU list from the registry

		assert(!m_strKeyName.IsEmpty()); // KeyName must be set before calling LoadRegistryMRUSettings
		HKEY hKey = NULL;
		BOOL bRet = FALSE;

		m_nMaxMRU = MIN(nMaxMRU, 16);
		std::vector<CString> vMRUEntries;
		CString strKey = _T("Software\\") + m_strKeyName + _T("\\Recent Files");

		if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, strKey, 0, KEY_READ, &hKey))
		{
			CString PathName;
			CString SubKey;
			for (UINT i = 0; i < m_nMaxMRU; ++i)
			{
				DWORD dwType = REG_SZ;
				DWORD dwBufferSize = 0;
				SubKey.Format(_T("File %d"), i+1);

				if (ERROR_SUCCESS == RegQueryValueEx(hKey, SubKey, NULL, &dwType, NULL, &dwBufferSize))
				{
					// load the entry from the registry
					if (ERROR_SUCCESS == RegQueryValueEx(hKey, SubKey, NULL, &dwType, (LPBYTE)PathName.GetBuffer(dwBufferSize), &dwBufferSize))
					{
						PathName.ReleaseBuffer();

						if (PathName.GetLength() > 0)
							vMRUEntries.push_back( PathName );
					}
					else
					{
						PathName.ReleaseBuffer();
						TRACE(_T("CFrame::LoadRegistryMRUSettings: RegQueryValueEx failed\n"));
						if (hKey)
							RegCloseKey(hKey);
					}
				}
			}

			// successfully loaded all MRU values, so store them
			m_vMRUEntries = vMRUEntries;
			RegCloseKey(hKey);
			bRet = TRUE;
		}

		return bRet;
	}

	inline BOOL CFrame::LoadRegistrySettings(LPCTSTR szKeyName)
	{
		assert (NULL != szKeyName);
		m_strKeyName = szKeyName;

		CString strKey = _T("Software\\") + m_strKeyName + _T("\\Frame Settings");
		HKEY hKey = 0;
		BOOL bRet = FALSE;

		try
		{
			if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, strKey, 0, KEY_READ, &hKey))
			{
				DWORD dwType = REG_BINARY;
				DWORD BufferSize = sizeof(DWORD);
				DWORD dwTop, dwLeft, dwWidth, dwHeight, dwStatusBar, dwToolBar;
				if (ERROR_SUCCESS != RegQueryValueEx(hKey, _T("Top"), NULL, &dwType, (LPBYTE)&dwTop, &BufferSize))
					throw CWinException(_T("RegQueryValueEx Failed"));
				if (ERROR_SUCCESS != RegQueryValueEx(hKey, _T("Left"), NULL, &dwType, (LPBYTE)&dwLeft, &BufferSize))
					throw CWinException(_T("RegQueryValueEx Failed"));
				if (ERROR_SUCCESS != RegQueryValueEx(hKey, _T("Width"), NULL, &dwType, (LPBYTE)&dwWidth, &BufferSize))
					throw CWinException(_T("RegQueryValueEx Failed"));
				if (ERROR_SUCCESS != RegQueryValueEx(hKey, _T("Height"), NULL, &dwType, (LPBYTE)&dwHeight, &BufferSize))
					throw CWinException(_T("RegQueryValueEx Failed"));
				if (ERROR_SUCCESS != RegQueryValueEx(hKey, _T("StatusBar"), NULL, &dwType, (LPBYTE)&dwStatusBar, &BufferSize))
					throw CWinException(_T("RegQueryValueEx Failed"));
				if (ERROR_SUCCESS != RegQueryValueEx(hKey, _T("ToolBar"), NULL, &dwType, (LPBYTE)&dwToolBar, &BufferSize))
					throw CWinException(_T("RegQueryValueEx Failed"));

				m_rcPosition.top = dwTop;
				m_rcPosition.left = dwLeft;
				m_rcPosition.bottom = m_rcPosition.top + dwHeight;
				m_rcPosition.right = m_rcPosition.left + dwWidth;
				m_bShowStatusBar = dwStatusBar & 1;
				m_bShowToolBar = dwToolBar & 1;

				RegCloseKey(hKey);
				bRet = TRUE;
			}
		}

		catch (const CWinException& e)
		{
			TRACE("Failed to load values from registry, using defaults!\n");
			e.what();

			if (hKey)
				RegCloseKey(hKey);
		}

		return bRet;
	}

	inline void CFrame::MeasureMenuItem(MEASUREITEMSTRUCT *pmis)
	{
		int cxTotal = 0;
		int cyMax = 0;

		MenuItemData* pmid = (MenuItemData*)pmis->itemData;
		assert(::IsMenu(pmid->hMenu));	// Does itemData contain a valid MenuItemData struct?

		// Add icon/check width
		cxTotal += m_pMenuMetrics->m_sizeCheck.cx + m_pMenuMetrics->m_marCheckBackground.Width() + m_pMenuMetrics->m_marCheck.Width();

		if (pmid->mii.fType & MFT_SEPARATOR)
		{
			// separator height
			cyMax = m_pMenuMetrics->m_sizeSeparator.cy + m_pMenuMetrics->m_marItem.Height();
		}
		else
		{
			// Add check background horizontal padding.
			cxTotal += m_pMenuMetrics->m_marCheckBackground.cxLeftWidth + m_pMenuMetrics->m_marCheckBackground.cxRightWidth;

			// Add selection margin padding.
			cxTotal += m_pMenuMetrics->m_marItem.cxLeftWidth + m_pMenuMetrics->m_marItem.cxRightWidth;

			// Account for text size
			CSize sizeText = m_pMenuMetrics->GetTextSize(pmid);
			cxTotal += sizeText.cx;
			cyMax = MAX(cyMax, sizeText.cy);

			// Account for icon or check height
			cyMax = MAX(cyMax, m_pMenuMetrics->m_sizeCheck.cy + m_pMenuMetrics->m_marCheckBackground.Height() + m_pMenuMetrics->m_marCheck.Height());
		}

		// Return the composite sizes.
		pmis->itemWidth = cxTotal;
		pmis->itemHeight = cyMax;
	}

	inline LRESULT CFrame::OnActivate(WPARAM wParam, LPARAM lParam)
	{
		// Do default processing first
		DefWindowProc(WM_ACTIVATE, wParam, lParam);

		if (LOWORD(wParam) == WA_INACTIVE)
		{
			// Save the hwnd of the window which currently has focus
			// (this must be CFrame window itself or a child window
			if (!IsIconic()) m_hOldFocus = ::GetFocus();

			// Send a notification to the view window
			int idCtrl = ::GetDlgCtrlID(m_hOldFocus);
			NMHDR nhdr={0};
			nhdr.hwndFrom = m_hOldFocus;
			nhdr.idFrom = idCtrl;
			nhdr.code = UWN_FRAMELOSTFOCUS;
			if (GetView()->IsWindow())
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
			nhdr.code = UWN_FRAMEGOTFOCUS;
			if (GetView()->IsWindow())
				GetView()->SendMessage(WM_NOTIFY, (WPARAM)idCtrl, (LPARAM)&nhdr);
		}

		return 0L;
	}

	inline void CFrame::OnClose()
	{
		// Called in response to a WM_CLOSE message for the frame.
		ShowWindow(SW_HIDE);

		Destroy();
	}

	inline int CFrame::OnCreate(LPCREATESTRUCT pcs)
	// This is called when the frame window is being created.
	// Override this in CMainFrame if you wish to modify what happens here	
	{
		UNREFERENCED_PARAMETER(pcs);

		// Set the icon
		SetIconLarge(IDW_MAIN);
		SetIconSmall(IDW_MAIN);

		// Set the keyboard accelerators
		m_hAccel = LoadAccelerators(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_MAIN));
		GetApp()->SetAccelerators(m_hAccel, this);

		// Set the Caption
		SetWindowText(LoadString(IDW_MAIN));

		// Set the theme for the frame elements
		SetTheme();

		// Create the rebar and menubar
		if (IsReBarSupported() && m_bUseReBar)
		{
			// Create the rebar
			GetReBar()->Create(this);

			// Create the menu inside rebar
			GetMenuBar()->Create(GetReBar());
			AddMenuBarBand();
		}

		// Setup the menu
		SetFrameMenu(IDW_MAIN);
		m_pMenuMetrics = new CMenuMetrics(this);

		UpdateMRUMenu();

		// Create the ToolBar
		if (m_bUseToolBar)
		{
			CreateToolBar();
			ShowToolBar(m_bShowToolBar);
		}
		else
		{
			GetFrameMenu()->CheckMenuItem(IDW_VIEW_TOOLBAR, MF_UNCHECKED);
			GetFrameMenu()->EnableMenuItem(IDW_VIEW_TOOLBAR, MF_GRAYED);
		}

		// Create the status bar
		GetStatusBar()->Create(this);
		GetStatusBar()->SetFont(&m_fntStatusBar, FALSE);
		ShowStatusBar(m_bShowStatusBar);

		// Create the view window
		assert(GetView());			// Use SetView in CMainFrame's constructor to set the view window
		GetView()->Create(this);

		// Disable XP themes for the menubar
		if ( m_bUseThemes || (GetWinVersion() < 2600)  )	// themes or WinVersion < Vista
			GetMenuBar()->SetWindowTheme(L" ", L" ");

		// Start timer for Status updates
		if (m_bShowIndicatorStatus || m_bShowMenuStatus)
			SetTimer(ID_STATUS_TIMER, 200, NULL);

		// Reposition the child windows
		OnSysColorChange(0, 0);
		RecalcLayout();

		return 0;
	}

	inline LRESULT CFrame::OnCustomDraw(LPNMHDR pNMHDR)
	{
		// CustomDraw notification from WM_NOTIFY

		CWnd* pWnd = FromHandle(pNMHDR->hwndFrom);
		if (dynamic_cast<CToolBar*>(pWnd))
		{
			if (pNMHDR->hwndFrom == GetMenuBar()->GetHwnd())
				return CustomDrawMenuBar(pNMHDR);
			else
				return CustomDrawToolBar(pNMHDR);
		}

		return 0L;
	}

	inline void CFrame::OnDestroy()
	{
		SetMenu(NULL);
		KillTimer(ID_STATUS_TIMER);
		SaveRegistrySettings();

		GetMenuBar()->Destroy();
		GetToolBar()->Destroy();
		GetReBar()->Destroy();
		GetStatusBar()->Destroy();
		GetView()->Destroy();

		::PostQuitMessage(0);	// Terminates the application
	}

	inline LRESULT CFrame::OnDrawItem(WPARAM wParam, LPARAM lParam)
	// OwnerDraw is used to render the popup menu items
	{
		LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT) lParam;
		if (pdis->CtlType != ODT_MENU)
			return CWnd::WndProcDefault(WM_DRAWITEM, wParam, lParam);

		if (!IsRectEmpty(&pdis->rcItem))
			DrawMenuItem(pdis);

		return TRUE;
	}

	inline LRESULT CFrame::OnExitMenuLoop(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);
		UNREFERENCED_PARAMETER(lParam);

		if (m_bUseThemes)
		{
			for (UINT nItem = 0; nItem < m_vMenuItemData.size(); ++nItem)
			{
				// Undo OwnerDraw and put the text back
				MENUITEMINFO mii = {0};
				mii.cbSize = GetSizeofMenuItemInfo();

				mii.fMask = MIIM_TYPE | MIIM_DATA;
				mii.fType = m_vMenuItemData[nItem]->mii.fType;
				mii.dwTypeData = m_vMenuItemData[nItem]->GetItemText();
				mii.cch = lstrlen(m_vMenuItemData[nItem]->GetItemText());
				mii.dwItemData = 0;
				::SetMenuItemInfo(m_vMenuItemData[nItem]->hMenu, m_vMenuItemData[nItem]->nPos, TRUE, &mii);
			}

			m_vMenuItemData.clear();
		}

		return 0L;
	}

	inline LRESULT CFrame::OnHelp()
	{
		// Ensure only one dialog displayed even for multiple hits of the F1 button
		if (!m_AboutDialog.IsWindow())
		{
			// Store the window handle that currently has keyboard focus
			HWND hPrevFocus = ::GetFocus();
			if (hPrevFocus == GetMenuBar()->GetHwnd())
				hPrevFocus = m_hWnd;

			m_AboutDialog.DoModal(this);

			::SetFocus(hPrevFocus);
		}

		return TRUE;
	}

	inline LRESULT CFrame::OnHotItemChange(LPNMTBHOTITEM pTBHotItem)
	// Hot item change notification from ToolBar.
	{	
		UNREFERENCED_PARAMETER(pTBHotItem);

		if (m_bShowMenuStatus)
		{
			// Get the toolbar the point is over
			CToolBar* pToolBar = 0;
			if (IsReBarUsed())
			{
				// Get the ToolBar's CWnd
				CWnd* pWnd = FromHandle(GetReBar()->HitTest(GetCursorPos()));
				if (pWnd && (dynamic_cast<CToolBar*>(pWnd)) && !(dynamic_cast<CMenuBar*>(pWnd)))
					pToolBar = (CToolBar*)pWnd;
			}
			else
			{
				CWnd* pWnd = WindowFromPoint(GetCursorPos());
				if (pWnd && (dynamic_cast<CToolBar*>(pWnd)))
					pToolBar = (CToolBar*)pWnd;
			}

			if ((pToolBar) && (WindowFromPoint(GetCursorPos()) == pToolBar))
			{
				// Which toolbar button is the mouse cursor hovering over?
				int nButton = pToolBar->HitTest();
				if (nButton >= 0)
				{
					int nID = pToolBar->GetCommandID(nButton);
					// Only update the statusbar if things have changed
					if (nID != m_nOldID)
					{
						if (nID != 0)
							m_strStatusText = LoadString(nID);
						else
							m_strStatusText = LoadString(IDW_READY);

						if (GetStatusBar()->IsWindow())
							SetStatusText();
					}
					m_nOldID = nID;
				}
			}
			else
			{
				if (m_nOldID != -1)
				{
					m_strStatusText = LoadString(IDW_READY);
					SetStatusText();
				}
				m_nOldID = -1;
			}
		}

		return 0L;
	}

	inline LRESULT CFrame::OnInitMenuPopup(WPARAM wParam, LPARAM lParam)
	{
		// The system menu shouldn't be owner drawn
		if (HIWORD(lParam)) CWnd::WndProcDefault(WM_INITMENUPOPUP, wParam, lParam);

		if (m_bUseThemes)
		{
			CMenu* pMenu = FromHandle((HMENU)wParam);

			for (UINT i = 0; i < pMenu->GetMenuItemCount(); ++i)
			{
				MenuItemData* pItem = new MenuItemData;			// deleted in OnExitMenuLoop
				m_vMenuItemData.push_back(ItemDataPtr(pItem));	// Store pItem in smart pointer for later automatic deletion

				MENUITEMINFO mii = {0};
				mii.cbSize = GetSizeofMenuItemInfo();

				// Use old fashioned MIIM_TYPE instead of MIIM_FTYPE for MS VC6 compatibility
				mii.fMask = MIIM_STATE | MIIM_ID | MIIM_SUBMENU |MIIM_CHECKMARKS | MIIM_TYPE | MIIM_DATA;
				mii.dwTypeData = pItem->GetItemText();	// assign TCHAR pointer, text is assigned by GetMenuItemInfo
				mii.cch = MAX_MENU_STRING -1;

				// Send message for menu updates
				UINT menuItem = pMenu->GetMenuItemID(i);
				SendMessage(UWM_UPDATECOMMAND, (WPARAM)menuItem, 0);

				// Specify owner-draw for the menu item type
				if (pMenu->GetMenuItemInfo(i, &mii, TRUE))
				{
					if (0 == mii.dwItemData)
					{
						pItem->hMenu = *pMenu;
						pItem->nPos = i;
						pItem->mii = mii;
						mii.dwItemData = (DWORD_PTR)pItem;
						mii.fType |= MFT_OWNERDRAW;
						pMenu->SetMenuItemInfo(i, &mii, TRUE); // Store pItem in mii
					}
				}
			}
		}

		return 0L;
	}

	inline LRESULT CFrame::OnMeasureItem(WPARAM wParam, LPARAM lParam)
	// Called before the Popup menu is displayed, so that the MEASUREITEMSTRUCT
	//  values can be assigned with the menu item's dimensions.
	{
		LPMEASUREITEMSTRUCT pmis = (LPMEASUREITEMSTRUCT) lParam;
		if (pmis->CtlType != ODT_MENU)
			return CWnd::WndProcDefault(WM_MEASUREITEM, wParam, lParam);

		MeasureMenuItem(pmis);
		return TRUE;
	}

	inline LRESULT CFrame::OnMenuChar(WPARAM wParam, LPARAM lParam)
	{
		if ((IsMenuBarUsed()) && (LOWORD(wParam)!= VK_SPACE))
		{
			// Activate MenuBar for key pressed with Alt key held down
			GetMenuBar()->OnMenuChar(wParam, lParam);
			return -1L;
		}

		return FinalWindowProc(WM_MENUCHAR, wParam, lParam);
	}

	inline LRESULT CFrame::OnMenuSelect(WPARAM wParam, LPARAM lParam)
	{
		// Set the StatusBar text when we hover over a menu
		// Only popup submenus have status strings
		if (m_bShowMenuStatus)
		{
			int nID = LOWORD (wParam);
			CMenu* pMenu = FromHandle((HMENU) lParam);

			if ((pMenu != GetMenu()) && (nID != 0) && !(HIWORD(wParam) & MF_POPUP))
				m_strStatusText = LoadString(nID);
			else
				m_strStatusText = LoadString(IDW_READY);

			SetStatusText();
		}

		return 0L;
	}

	inline LRESULT CFrame::OnNotify(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);

		LPNMHDR pNMHDR = (LPNMHDR)lParam;
		switch (pNMHDR->code)
		{
		case NM_CUSTOMDRAW:		return OnCustomDraw(pNMHDR);
		case RBN_HEIGHTCHANGE:	return OnRBNHeightChange(pNMHDR);
		case RBN_LAYOUTCHANGED:	return OnRBNLayoutChanged(pNMHDR);
		case RBN_MINMAX:		return OnRBNMinMax(pNMHDR);
		case TBN_DROPDOWN:		return OnTBNDropDown((LPNMTOOLBAR)lParam);
		case TTN_GETDISPINFO:	return OnTTNGetDispInfo((LPNMTTDISPINFO)lParam);
		case UWN_UNDOCKED:		return OnUndocked();
		case TBN_HOTITEMCHANGE:	return OnHotItemChange((LPNMTBHOTITEM)lParam); 
		}

		return 0L;
	}

	inline LRESULT CFrame::OnRBNHeightChange(LPNMHDR pNMHDR)
	{
		// Notifcation sent by Rebar

		UNREFERENCED_PARAMETER(pNMHDR);

		RecalcLayout();
		Invalidate();

		return 0L;
	}

	inline LRESULT CFrame::OnRBNLayoutChanged(LPNMHDR pNMHDR)
	{
		// Notification ot rebar layout change

		UNREFERENCED_PARAMETER(pNMHDR);

		if (GetReBarTheme()->UseThemes && GetReBarTheme()->BandsLeft)
			GetReBar()->MoveBandsLeft();
		
		return 0L;
	}

	inline LRESULT CFrame::OnRBNMinMax(LPNMHDR pNMHDR)
	{
		// Notifcation sent by Rebar

		UNREFERENCED_PARAMETER(pNMHDR);

		if (m_ReBarTheme.UseThemes && m_ReBarTheme.ShortBands)
			return 1L;	// Suppress maximise or minimise rebar band
		
		return 0L;
	}

	inline LRESULT CFrame::OnTBNDropDown(LPNMTOOLBAR pNMTB)
	{	
		// Press of Dropdown botton on ToolBar
			
		int iItem = pNMTB->iItem;
		CToolBar* pTB = (CToolBar*)FromHandle(pNMTB->hdr.hwndFrom);
		assert(pTB);

		// a boolean expression
		m_bDrawArrowBkgrnd = (pTB->GetButtonStyle(iItem) & TBSTYLE_DROPDOWN);
				
		return 0L;
	}

	inline LRESULT CFrame::OnTTNGetDispInfo(LPNMTTDISPINFO pNMTDI)
	// Tool tip notification for the toolbar
	{
		// Tool tip notification for the toolbar
		
		CToolBar* pToolBar = 0;

		// Find the ToolBar that generated the tooltip
		CPoint pt(GetMessagePos());
		CWnd* pWnd = WindowFromPoint(pt);
		if (dynamic_cast<CToolBar*> (pWnd))
		{
			pToolBar = (CToolBar*)pWnd;
		}

		// Set the tooltip's text from the ToolBar button's CommandID
		if (pToolBar)
		{
			LPNMTTDISPINFO lpDispInfo = pNMTDI;
			int iIndex =  pToolBar->HitTest();
			if (iIndex >= 0)
			{
				int nID = pToolBar->GetCommandID(iIndex);
				if (nID > 0)
				{
					m_strTooltip = LoadString(nID);
					lpDispInfo->lpszText = (LPTSTR)m_strTooltip.c_str();
				}
				else
					m_strTooltip = _T("");
			}
		}
			
		return 0L;
	}

	inline LRESULT CFrame::OnUndocked()
	{
		// Notification from CDocker received via OnNotify

		m_hOldFocus = 0;
		return 0;
	}

	inline LRESULT CFrame::OnSetFocus(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);
		UNREFERENCED_PARAMETER(lParam);

		SetStatusText();
		return 0L;
	}

	inline LRESULT CFrame::OnSize(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);
		UNREFERENCED_PARAMETER(lParam);
		
		RecalcLayout();
		return 0L;
	}

	inline LRESULT CFrame::OnSysColorChange(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);
		UNREFERENCED_PARAMETER(lParam);

		// Honour theme color changes
		if (GetReBar()->IsWindow())
		{
			for (int nBand = 0; nBand <= GetReBar()->GetBandCount(); ++nBand)
			{
				GetReBar()->SetBandColor(nBand, GetSysColor(COLOR_BTNTEXT), GetSysColor(COLOR_BTNFACE));
			}
		}

		// Update the status bar font and text
		NONCLIENTMETRICS nm = {0};
		nm.cbSize = GetSizeofNonClientMetrics();
		SystemParametersInfo (SPI_GETNONCLIENTMETRICS, 0, &nm, 0);
		m_fntStatusBar.DeleteObject();
		m_fntStatusBar.CreateFontIndirect(&nm.lfStatusFont);
		GetStatusBar()->SetFont(&m_fntStatusBar, TRUE);
		SetStatusText();


		if (GetMenuBar()->IsWindow())
		{
			// Update the MenuBar font and button size
			m_fntMenuBar.DeleteObject();
			m_fntMenuBar.CreateFontIndirect(&nm.lfMenuFont);
			GetMenuBar()->SetFont(&m_fntMenuBar, TRUE);
			GetMenuBar()->SetMenu( GetFrameMenu()->GetHandle() );

			// Update the MenuBar band size
			int nBand = GetReBar()->GetBand(GetMenuBar()->GetHwnd());
			REBARBANDINFO rbbi = {0};
			CClientDC dcMenuBar(GetMenuBar());
			dcMenuBar.SelectObject(GetMenuBar()->GetFont());
			CSize sizeMenuBar = dcMenuBar.GetTextExtentPoint32(_T("\tSomeText"), lstrlen(_T("\tSomeText")));
			int MenuBar_Height = sizeMenuBar.cy + 6;
			rbbi.fMask      = RBBIM_CHILDSIZE;
			rbbi.cyMinChild = MenuBar_Height;
			rbbi.cyMaxChild = MenuBar_Height;
			GetReBar()->SetBandInfo(nBand, rbbi);
		}

		if (m_XPThemeName != GetThemeName())
			SetTheme();
		m_pMenuMetrics->Initialize();

		// Reposition and redraw everything
		RecalcLayout();
		RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);

		// Forward the message to the view window
		m_pView->PostMessage(WM_SYSCOLORCHANGE, 0L, 0L);

		return 0L;
	}

	inline LRESULT CFrame::OnSysCommand(WPARAM wParam, LPARAM lParam)
	{
		if ((SC_KEYMENU == wParam) && (VK_SPACE != lParam) && IsMenuBarUsed())
		{
			GetMenuBar()->OnSysCommand(wParam, lParam);
			return 0L;
		}

		if (SC_MINIMIZE == wParam)
			m_hOldFocus = ::GetFocus();

		// Pass remaining system commands on for default processing
		return FinalWindowProc(WM_SYSCOMMAND, wParam, lParam);
	}

	inline LRESULT CFrame::OnTimer(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);

		if (ID_STATUS_TIMER == wParam)
		{
			if ((m_bShowMenuStatus) && ( m_strStatusText != LoadString(IDW_READY) ) )
			{
				// Get the toolbar the point is over
				CToolBar* pToolBar = 0;
				if (IsReBarUsed())
				{
					// Get the ToolBar's CWnd
					CWnd* pWnd = FromHandle(GetReBar()->HitTest(GetCursorPos()));
					if (pWnd && (dynamic_cast<CToolBar*>(pWnd)) && !(dynamic_cast<CMenuBar*>(pWnd)))
						pToolBar = (CToolBar*)pWnd;
				}
				else
				{
					CWnd* pWnd = WindowFromPoint(GetCursorPos());
					if (pWnd && (dynamic_cast<CToolBar*>(pWnd)))
						pToolBar = (CToolBar*)pWnd;
				}

				if ((pToolBar) && (WindowFromPoint(GetCursorPos()) == pToolBar))
				{
					// Which toolbar button is the mouse cursor hovering over?
					int nButton = pToolBar->HitTest();
					if (nButton >= 0)
					{
						int nID = pToolBar->GetCommandID(nButton);
						// Only update the statusbar if things have changed
						if (nID != m_nOldID)
						{
							if (nID != 0)
								m_strStatusText = LoadString(nID);
							else
								m_strStatusText = LoadString(IDW_READY);

							if (GetStatusBar()->IsWindow())
								SetStatusText();
						}
						m_nOldID = nID;
					}
				}
				else
				{
					if (m_nOldID != -1)
					{
						m_strStatusText = LoadString(IDW_READY);
						SetStatusText();
					}
					m_nOldID = -1;
				}
			} 

			if (m_bShowIndicatorStatus)
				SetStatusIndicators();
		}

		return 0L;
	}

	inline void CFrame::OnViewStatusBar()
	{
		m_bShowStatusBar = !m_bShowStatusBar;
		ShowStatusBar(m_bShowStatusBar);
	}

	inline void CFrame::OnViewToolBar()
	{
		m_bShowToolBar = !m_bShowToolBar;
		ShowToolBar(m_bShowToolBar);
	}

  	inline void CFrame::PreCreate(CREATESTRUCT& cs)
	{
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

	inline void CFrame::RecalcLayout()
	{
		CWnd* pView = GetView();
		if ((!pView) || (!pView->GetHwnd()))
			return;

		// Resize the status bar
		if (GetStatusBar()->IsWindow() && m_bShowStatusBar)
		{
			GetStatusBar()->SetWindowPos(NULL, 0, 0, 0, 0, SWP_SHOWWINDOW);
			GetStatusBar()->Invalidate();
			SetStatusText();
		}

		// Resize the rebar or toolbar
		if (IsReBarUsed())
		{
			GetReBar()->SendMessage(WM_SIZE, 0L, 0L);
			GetReBar()->Invalidate();
		}
		else if (m_bUseToolBar && m_bShowToolBar)
			GetToolBar()->SendMessage(TB_AUTOSIZE, 0L, 0L);

		// Resize the View window
		CRect rClient = GetViewRect();
		if ((rClient.bottom - rClient.top) >= 0)
		{
			int x  = rClient.left;
			int y  = rClient.top;
			int cx = rClient.Width();
			int cy = rClient.Height();

			pView->SetWindowPos( NULL, x, y, cx, cy, SWP_SHOWWINDOW|SWP_ASYNCWINDOWPOS );
		}

		// Adjust rebar bands
		if (IsReBarUsed())
		{
			if (m_ReBarTheme.UseThemes && m_ReBarTheme.BandsLeft)
				GetReBar()->MoveBandsLeft();

			if (IsMenuBarUsed())
				SetMenuBarBandSize();
		}
	}

	inline void CFrame::RemoveMRUEntry(LPCTSTR szMRUEntry)
	{
		std::vector<CString>::iterator it;
		for (it = m_vMRUEntries.begin(); it != m_vMRUEntries.end(); ++it)
		{
			if ((*it) == szMRUEntry)
			{
				m_vMRUEntries.erase(it);
				break;
			}
		}

		UpdateMRUMenu();
	}

	inline BOOL CFrame::SaveRegistrySettings()
	{
		// Store the window position in the registry
		if (!m_strKeyName.IsEmpty())
		{
			CString strKeyName = _T("Software\\") + m_strKeyName + _T("\\Frame Settings");
			HKEY hKey = NULL;

			try
			{
				if (ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER, strKeyName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL))
					throw CWinException(_T("RegCreateKeyEx failed"));

				WINDOWPLACEMENT Wndpl = {0};
				Wndpl.length = sizeof(WINDOWPLACEMENT);

				if (GetWindowPlacement(Wndpl))
				{
					// Get the Frame's window position
					CRect rc = Wndpl.rcNormalPosition;
					DWORD dwTop = MAX(rc.top, 0);
					DWORD dwLeft = MAX(rc.left, 0);
					DWORD dwWidth = MAX(rc.Width(), 100);
					DWORD dwHeight = MAX(rc.Height(), 50);

					if (ERROR_SUCCESS != RegSetValueEx(hKey, _T("Top"), 0, REG_DWORD, (LPBYTE)&dwTop, sizeof(DWORD)))
						throw CWinException(_T("RegSetValueEx failed"));
					if (ERROR_SUCCESS != RegSetValueEx(hKey, _T("Left"), 0, REG_DWORD, (LPBYTE)&dwLeft, sizeof(DWORD)))
						throw CWinException(_T("RegSetValueEx failed"));
					if (ERROR_SUCCESS != RegSetValueEx(hKey, _T("Width"), 0, REG_DWORD, (LPBYTE)&dwWidth, sizeof(DWORD)))
						throw CWinException(_T("RegSetValueEx failed"));
					if (ERROR_SUCCESS != RegSetValueEx(hKey, _T("Height"), 0, REG_DWORD, (LPBYTE)&dwHeight, sizeof(DWORD)))
						throw CWinException(_T("RegSetValueEx failed"));
				}

				// Store the ToolBar and statusbar states
				DWORD dwShowToolBar = m_bShowToolBar;
				DWORD dwShowStatusBar = m_bShowStatusBar;

				if (ERROR_SUCCESS != RegSetValueEx(hKey, _T("ToolBar"), 0, REG_DWORD, (LPBYTE)&dwShowToolBar, sizeof(DWORD)))
					throw CWinException(_T("RegSetValueEx failed"));
				if (ERROR_SUCCESS != RegSetValueEx(hKey, _T("StatusBar"), 0, REG_DWORD, (LPBYTE)&dwShowStatusBar, sizeof(DWORD)))
					throw CWinException(_T("RegSetValueEx failed"));

				RegCloseKey(hKey);
			}

			catch (const CWinException& e)
			{
				TRACE("Failed to save registry settings\n");

				if (hKey)
				{
					// Roll back the registry changes by deleting this subkey
					RegDeleteKey(HKEY_CURRENT_USER ,strKeyName);
					RegCloseKey(hKey);
				}

				e.what();
				return FALSE;
			}

			// Store the MRU entries in the registry
			if (m_nMaxMRU > 0)
			{
				CString strKeyName = _T("Software\\") + m_strKeyName + _T("\\Recent Files");
				HKEY hKey = NULL;

				try
				{
					// Delete Old MRUs
					RegDeleteKey(HKEY_CURRENT_USER, strKeyName);

					// Add Current MRUs
					if (ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER, strKeyName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL))
						throw CWinException(_T("RegCreateKeyEx failed"));

					CString SubKey;
					CString strPathName;
					for (UINT i = 0; i < m_nMaxMRU; ++i)
					{
						SubKey.Format(_T("File %d"), i+1);
						
						if (i < m_vMRUEntries.size())
						{
							strPathName = m_vMRUEntries[i];

							if (ERROR_SUCCESS != RegSetValueEx(hKey, SubKey, 0, REG_SZ, (LPBYTE)strPathName.c_str(), (1 + strPathName.GetLength() )*sizeof(TCHAR)))
								throw CWinException(_T("RegSetValueEx failed"));
						}
					}

					RegCloseKey(hKey);
				}

				catch (const CWinException& e)
				{
					TRACE("Failed to save registry MRU settings\n");

					if (hKey)
					{
						// Roll back the registry changes by deleting this subkey
						RegDeleteKey(HKEY_CURRENT_USER ,strKeyName);
						RegCloseKey(hKey);
					}

					e.what();
					return FALSE;
				}
			}
		}

		return TRUE;
	}

	inline void CFrame::SetFrameMenu(INT ID_MENU)
	{
		// Sets the frame's menu from a Resource ID.
		// A resource ID of 0 removes the menu from the frame.
		HMENU hMenu = 0;
		if (ID_MENU != 0)
		{
		// Sets the frame's menu from a resource ID.
			hMenu = ::LoadMenu(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(ID_MENU));
			assert (hMenu);
		}

		SetFrameMenu(hMenu);
 	}

	inline void CFrame::SetFrameMenu(HMENU hMenu)
	{
		// Sets the frame's menu from a HMENU.
		m_Menu.Attach(hMenu);

		if (IsMenuBarUsed())
		{
			GetMenuBar()->SetMenu( GetFrameMenu()->GetHandle() );
			BOOL bShow = (hMenu != NULL);	// boolean expression
			ShowMenu(bShow);
		}
		else
			SetMenu(&m_Menu);
 	}

	inline UINT CFrame::SetMenuIcons(const std::vector<UINT>& MenuData, COLORREF crMask, UINT ToolBarID, UINT ToolBarDisabledID)
	{
		// Remove any existing menu icons
		m_imlMenu.DeleteImageList();
		m_imlMenuDis.DeleteImageList();
		m_vMenuIcons.clear();

		// Exit if no ToolBarID is specified
		if (ToolBarID == 0) return 0;

		// Add the menu icons from the bitmap IDs
		return AddMenuIcons(MenuData, crMask, ToolBarID, ToolBarDisabledID);
	}

	inline void CFrame::SetMenuBarBandSize()
	{
		// Sets the minimum width of the MenuBar band to the width of the rebar
		// This prevents other bands from moving to this MenuBar's row.

		CRect rcClient = GetClientRect();
		CReBar* RB = GetReBar();
		int nBand = RB->GetBand(GetMenuBar()->GetHwnd());
		CRect rcBorder = RB->GetBandBorders(nBand);

		REBARBANDINFO rbbi = {0};
		rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_SIZE;
		RB->GetBandInfo(nBand, rbbi);

		int Width;
		if ((m_ReBarTheme.UseThemes) && (m_ReBarTheme.LockMenuBand))
			Width = rcClient.Width() - rcBorder.Width() - 2;
		else
			Width = GetMenuBar()->GetMaxSize().cx;

		rbbi.cxMinChild = Width;
		rbbi.cx         = Width;

		RB->SetBandInfo(nBand, rbbi);
	}

	inline void CFrame::SetMenuTheme(MenuTheme* pMBT)
	{
		// Sets the theme colors for the MenuBar and the popup Menu items
		// Note: If Aero Themes are supported, they are used for popup menu items instead
		m_MenuBarTheme.UseThemes   = pMBT->UseThemes;
		m_MenuBarTheme.clrHot1     = pMBT->clrHot1;
		m_MenuBarTheme.clrHot2     = pMBT->clrHot2;
		m_MenuBarTheme.clrPressed1 = pMBT->clrPressed1;
		m_MenuBarTheme.clrPressed2 = pMBT->clrPressed2;
		m_MenuBarTheme.clrOutline  = pMBT->clrOutline;

		if (GetMenuBar()->IsWindow())
			GetMenuBar()->Invalidate();
	}

	inline void CFrame::SetReBarTheme(ReBarTheme* pRBT) 
	{ 
		m_ReBarTheme = *pRBT; 
		if (IsWindow())
		{
			int nBand = GetReBar()->GetBand(GetMenuBar()->GetHwnd());
			if (m_ReBarTheme.LockMenuBand)
				GetReBar()->ShowGripper(nBand, FALSE);
			else
				GetReBar()->ShowGripper(nBand, TRUE);
	
			Invalidate();
		}
	}

	inline void CFrame::SetStatusIndicators()
	{
		if (GetStatusBar()->IsWindow())
		{
			CString Status1 = (::GetKeyState(VK_CAPITAL) & 0x0001)? _T("\tCAP") : _T("");
			CString Status2 = (::GetKeyState(VK_NUMLOCK) & 0x0001)? _T("\tNUM") : _T("");
			CString Status3 = (::GetKeyState(VK_SCROLL)  & 0x0001)? _T("\tSCRL"): _T("");

			// Only update indicators if the text has changed
			if (Status1 != m_OldStatus[0]) 	GetStatusBar()->SetPartText(1, (Status1));
			if (Status2 != m_OldStatus[1])  GetStatusBar()->SetPartText(2, (Status2));
			if (Status3 != m_OldStatus[2])  GetStatusBar()->SetPartText(3, (Status3));

			m_OldStatus[0] = Status1;
			m_OldStatus[1] = Status2;
			m_OldStatus[2] = Status3;
		}
	}

	inline void CFrame::SetStatusText()
	{
		if (GetStatusBar()->IsWindow())
		{
			// Calculate the width of the text indicators
			CClientDC dcStatus(GetStatusBar());
			CSize csCAP  = dcStatus.GetTextExtentPoint32(_T("\tCAP"), lstrlen(_T("\tCAP")));
			CSize csNUM  = dcStatus.GetTextExtentPoint32(_T("\tNUM"), lstrlen(_T("\tNUM")));
			CSize csSCRL = dcStatus.GetTextExtentPoint32(_T("\tSCRL "), lstrlen(_T("\tSCRL ")));

			// Get the coordinates of the parent window's client area.
			CRect rcClient = GetClientRect();
			int width = MAX(300, rcClient.right);

			if (m_bShowIndicatorStatus)
			{
				// Create 4 panes
				GetStatusBar()->SetPartWidth(0, width - (csCAP.cx+csNUM.cx+csSCRL.cx+20));
				GetStatusBar()->SetPartWidth(1, csCAP.cx);
				GetStatusBar()->SetPartWidth(2, csNUM.cx);
				GetStatusBar()->SetPartWidth(3, csSCRL.cx);

				SetStatusIndicators();
			}

			// Place text in the 1st pane
			GetStatusBar()->SetPartText(0, m_strStatusText);
		}
	}

	inline void CFrame::SetTheme()
	{
		// Note: To modify theme colors, override this function in CMainframe,
		//        and make any modifications there.

		// Avoid themes if using less than 16 bit colors
		CClientDC DesktopDC(NULL);
		if (DesktopDC.GetDeviceCaps(BITSPIXEL) < 16)
			m_bUseThemes = FALSE;

		BOOL T = TRUE;
		BOOL F = FALSE;

		if (m_bUseThemes)
		{
			// Retrieve the XP theme name
			m_XPThemeName = GetThemeName();

			enum Themetype{ Modern, Grey, Blue, Silver, Olive };

			int Theme = Grey;
			if (GetWinVersion() < 2600) // Not for Vista and above
			{
				if (m_XPThemeName == _T("NormalColor"))
					Theme = Blue;
				if (m_XPThemeName == _T("Metallic"))
					Theme = Silver;
				if (m_XPThemeName == _T("HomeStead"))
					Theme = Olive;
			}
			else
				Theme = Modern;

			switch (Theme)
			{
			case Modern:
				{
					ToolBarTheme tt = {T, RGB(180, 250, 255), RGB(140, 190, 255), RGB(150, 220, 255), RGB(80, 100, 255), RGB(127, 127, 255)};
					ReBarTheme tr = {T, RGB(225, 230, 255), RGB(240, 242, 250), RGB(248, 248, 248), RGB(180, 200, 230), F, T, T, T, T, F};
					MenuTheme tm = {T, RGB(180, 250, 255), RGB(140, 190, 255), RGB(240, 250, 255), RGB(120, 170, 220), RGB(127, 127, 255)};

					m_ToolBarTheme = tt;
					SetMenuTheme(&tm);	// Sets the theme for popup menus and MenuBar
					m_ReBarTheme = tr;
				}
				break;

			case Grey:	// A color scheme suitable for 16 bit colors. Suitable for Windows older than XP.
				{
					ToolBarTheme tt = {T, RGB(182, 189, 210), RGB(182, 189, 210), RGB(133, 146, 181), RGB(133, 146, 181), RGB(10, 36, 106)};
					ReBarTheme tr = {T, RGB(212, 208, 200), RGB(212, 208, 200), RGB(230, 226, 222), RGB(220, 218, 208), F, T, T, T, T, F};
					MenuTheme tm = {T, RGB(182, 189, 210), RGB( 182, 189, 210), RGB(200, 196, 190), RGB(200, 196, 190), RGB(100, 100, 100)};

					m_ToolBarTheme = tt;
					SetMenuTheme(&tm);	// Sets the theme for popup menus and MenuBar
					m_ReBarTheme = tr;
				}
				break;
			case Blue:
				{
					// Used for XP default (blue) color scheme
					ToolBarTheme tt = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 140, 40), RGB(255, 180, 80), RGB(192, 128, 255)};
					ReBarTheme tr = {T, RGB(150,190,245), RGB(196,215,250), RGB(220,230,250), RGB( 70,130,220), F, T, T, T, T, F};
					MenuTheme tm = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(220,230,250), RGB(150,190,245), RGB(128, 128, 200)};

					m_ToolBarTheme = tt;
					SetMenuTheme(&tm);	// Sets the theme for popup menus and MenuBar
					m_ReBarTheme = tr;
				}
				break;

			case Silver:
				{
					// Used for XP Silver color scheme
					ToolBarTheme tt = {T, RGB(192, 210, 238), RGB(192, 210, 238), RGB(152, 181, 226), RGB(152, 181, 226), RGB(49, 106, 197)};
					ReBarTheme tr = {T, RGB(225, 220, 240), RGB(240, 240, 245), RGB(245, 240, 255), RGB(160, 155, 180), F, T, T, T, T, F};
					MenuTheme tm = {T, RGB(196, 215, 250), RGB( 120, 180, 220), RGB(240, 240, 245), RGB(170, 165, 185), RGB(128, 128, 150)};

					m_ToolBarTheme = tt;
					SetMenuTheme(&tm);	// Sets the theme for popup menus and MenuBar
					m_ReBarTheme = tr;
				}
				break;

			case Olive:
				{
					// Used for XP Olive color scheme
					ReBarTheme tr = {T, RGB(215, 216, 182), RGB(242, 242, 230), RGB(249, 255, 227), RGB(178, 191, 145), F, T, T, T, T, F};
					ToolBarTheme tt = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 140, 40), RGB(255, 180, 80), RGB(200, 128, 128)};
					MenuTheme tm = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(249, 255, 227), RGB(178, 191, 145), RGB(128, 128, 128)};

					m_ToolBarTheme = tt;
					SetMenuTheme(&tm);	// Sets the theme for popup menus and MenuBar
					m_ReBarTheme = tr;
				}
				break;
			}
		}

		RecalcLayout();
	}

	inline void CFrame::SetTBImageList(CToolBar* pToolBar, CImageList* pImageList, UINT nID, COLORREF crMask)
	// Sets the Image List for additional Toolbars.
	// The CImageList provided should be a member of CMainFrame.
	// A Disabled image list is created from ToolBarID if one doesn't already exist.
	{
		// Get the image size
		CBitmap bm(nID);
		CSize sz = GetTBImageSize(&bm);

		// Set the toolbar's image list
		pImageList->DeleteImageList();
		pImageList->Create(sz.cx, sz.cy, ILC_COLOR32 | ILC_MASK, 0, 0);
		pImageList->Add( &bm, crMask );
		pToolBar->SetImageList(pImageList);

		// Inform the Rebar of the change to the Toolbar
		if (GetReBar()->IsWindow())
		{
			SIZE MaxSize = pToolBar->GetMaxSize();
			GetReBar()->SendMessage(UWM_TOOLBARRESIZE, (WPARAM)pToolBar->GetHwnd(), (LPARAM)&MaxSize);
		}
	}

	inline void CFrame::SetTBImageListDis(CToolBar* pToolBar, CImageList* pImageList, UINT nID, COLORREF crMask)
	// Sets the Disabled Image List for additional Toolbars.
	// The CImageList provided should be a member of CMainFrame.
	{
		if (nID)
		{
			// Get the image size
			CBitmap bm(nID);
			CSize sz = GetTBImageSize(&bm);

			// Set the toolbar's image list
			pImageList->DeleteImageList();
			pImageList->Create(sz.cx, sz.cy, ILC_COLOR32 | ILC_MASK, 0, 0);
			pImageList->Add( &bm, crMask );
			pToolBar->SetDisableImageList(pImageList);
		}
		else
		{
			pImageList->DeleteImageList();
			pImageList->Attach( CreateDisabledImageList(m_ToolBarImages.GetHandle()) );
			pToolBar->SetDisableImageList(pImageList);
		}

		// Inform the Rebar of the change to the Toolbar
		if (GetReBar()->IsWindow())
		{
			SIZE MaxSize = pToolBar->GetMaxSize();
			GetReBar()->SendMessage(UWM_TOOLBARRESIZE, (WPARAM)pToolBar->GetHwnd(), (LPARAM)&MaxSize);
		}
	}

	inline void CFrame::SetTBImageListHot(CToolBar* pToolBar, CImageList* pImageList, UINT nID, COLORREF crMask)
	// Sets the Hot Image List for additional Toolbars.
	// The CImageList provided should be a member of CMainFrame.
	{
		if (nID)
		{
			// Get the image size
			CBitmap bm(nID);
			CSize sz = GetTBImageSize(&bm);

			// Set the toolbar's image list
			pImageList->DeleteImageList();
			pImageList->Create(sz.cx, sz.cy, ILC_COLOR32 | ILC_MASK, 0, 0);
			pImageList->Add( &bm, crMask );
			pToolBar->SetHotImageList(pImageList);
		}
		else
		{
			pImageList->DeleteImageList();
			pToolBar->SetHotImageList(0);
		}

		// Inform the Rebar of the change to the Toolbar
		if (GetReBar()->IsWindow())
		{
			SIZE MaxSize = pToolBar->GetMaxSize();
			GetReBar()->SendMessage(UWM_TOOLBARRESIZE, (WPARAM)pToolBar->GetHwnd(), (LPARAM)&MaxSize);
		}
	}

	inline void CFrame::SetToolBarImages(COLORREF crMask, UINT ToolBarID, UINT ToolBarHotID, UINT ToolBarDisabledID)
	// Either sets the imagelist or adds/replaces bitmap depending on ComCtl32.dll version
	// Assumes the width of the button image = height, minimum width = 16
	// The colour mask is ignored for 32bit bitmaps, but is required for 24bit bitmaps
	// The colour mask is often grey RGB(192,192,192) or magenta (255,0,255)
	// The Hot and disabled bitmap resources can be 0
	// A Disabled image list is created from ToolBarID if one isn't provided.
	{
		if (GetComCtlVersion() < 470)
		{
			// We are using COMCTL32.DLL version 4.0, so we can't use an ImageList.
			// Instead we simply set the bitmap.
			GetToolBar()->SetBitmap(ToolBarID);
			return;
		}

		// Set the button images
		SetTBImageList(GetToolBar(),    &m_ToolBarImages, ToolBarID, crMask);
		SetTBImageListHot(GetToolBar(), &m_ToolBarHotImages, ToolBarHotID, crMask);
		SetTBImageListDis(GetToolBar(), &m_ToolBarDisabledImages, ToolBarDisabledID, crMask);
	}

	inline void CFrame::SetupToolBar()
	{
		// Use this function to set the Resource IDs for the toolbar(s).

/*		// Set the Resource IDs for the toolbar buttons
		AddToolBarButton( IDM_FILE_NEW   );
		AddToolBarButton( IDM_FILE_OPEN  );
		AddToolBarButton( IDM_FILE_SAVE  );
		
		AddToolBarButton( 0 );						// Separator
		AddToolBarButton( IDM_EDIT_CUT,   FALSE );	// disabled button
		AddToolBarButton( IDM_EDIT_COPY,  FALSE );	// disabled button
		AddToolBarButton( IDM_EDIT_PASTE, FALSE );	// disabled button
		
		AddToolBarButton( 0 );						// Separator
		AddToolBarButton( IDM_FILE_PRINT );
		
		AddToolBarButton( 0 );						// Separator
		AddToolBarButton( IDM_HELP_ABOUT );
*/
	}

	inline void CFrame::SetToolBarTheme(ToolBarTheme* pTBT)
	{ 
		m_ToolBarTheme = *pTBT;
		if (GetToolBar()->IsWindow())
			GetToolBar()->GetParent()->RedrawWindow(0, 0, RDW_INVALIDATE|RDW_ALLCHILDREN); 
	}

	inline void CFrame::SetView(CWnd& wndView)
	// Sets or changes the View window displayed within the frame
	{
		if (m_pView != &wndView)
		{
			// Destroy the existing view window (if any)
			if (m_pView) m_pView->Destroy();

			// Assign the view window
			m_pView = &wndView;

			if (m_hWnd)
			{
				// The frame is already created, so create and position the new view too
				assert(GetView());			// Use SetView in CMainFrame's constructor to set the view window
				GetView()->Create(this);
				RecalcLayout();
			}
		}
	}

	inline void CFrame::ShowMenu(BOOL bShow)
	{
		if (bShow)
		{
			if (IsReBarUsed())
				GetReBar()->SendMessage(RB_SHOWBAND, GetReBar()->GetBand(GetMenuBar()->GetHwnd()), TRUE);
			else
				SetMenu(&m_Menu);
		}
		else
		{
			if (IsReBarUsed())
				GetReBar()->SendMessage(RB_SHOWBAND, GetReBar()->GetBand(GetMenuBar()->GetHwnd()), FALSE);
			else
				SetMenu(NULL);
		}

		if (GetReBar()->IsWindow())
		{
			if (m_ReBarTheme.UseThemes && m_ReBarTheme.BandsLeft)
				GetReBar()->MoveBandsLeft();
		}

		// Reposition the Windows
		RecalcLayout();
	}

	inline void CFrame::ShowStatusBar(BOOL bShow)
	{
		if (bShow)
		{
			m_Menu.CheckMenuItem(IDW_VIEW_STATUSBAR, MF_CHECKED);
			GetStatusBar()->ShowWindow(SW_SHOW);
			m_bShowStatusBar = TRUE;
		}
		else
		{
			m_Menu.CheckMenuItem(IDW_VIEW_STATUSBAR, MF_UNCHECKED);
			GetStatusBar()->ShowWindow(SW_HIDE);
			m_bShowStatusBar = FALSE;
		}

		// Reposition the Windows
		RecalcLayout();
	}

	inline void CFrame::ShowToolBar(BOOL bShow)
	{
		if (bShow)
		{
			m_Menu.CheckMenuItem(IDW_VIEW_TOOLBAR, MF_CHECKED);
			if (IsReBarUsed())
				GetReBar()->SendMessage(RB_SHOWBAND, GetReBar()->GetBand(*GetToolBar()), TRUE);
			else
				GetToolBar()->ShowWindow(SW_SHOW);
			m_bShowToolBar = TRUE;
		}
		else
		{
			m_Menu.CheckMenuItem(IDW_VIEW_TOOLBAR, MF_UNCHECKED);
			if (IsReBarUsed())
				GetReBar()->SendMessage(RB_SHOWBAND, GetReBar()->GetBand(*GetToolBar()), FALSE);
			else
				GetToolBar()->ShowWindow(SW_HIDE);
			m_bShowToolBar = FALSE;
		}

		if (GetReBar()->IsWindow())
		{
			if (m_ReBarTheme.UseThemes && m_ReBarTheme.BandsLeft)
				GetReBar()->MoveBandsLeft();
		}

		// Reposition the Windows
		RecalcLayout();
	}

	inline void CFrame::UpdateMRUMenu()
	{
		if (0 >= m_nMaxMRU) return;

		// Set the text for the MRU Menu
		CString strMRUArray[16];
		UINT MaxMRUArrayIndex = 0;
		if (m_vMRUEntries.size() > 0)
		{
			for (UINT n = 0; ((n < m_vMRUEntries.size()) && (n <= m_nMaxMRU)); ++n)
			{
				strMRUArray[n] = m_vMRUEntries[n];
				if (strMRUArray[n].GetLength() > MAX_MENU_STRING - 10)
				{
					// Truncate the string if its too long
					strMRUArray[n].Delete(0, strMRUArray[n].GetLength() - MAX_MENU_STRING +10);
					strMRUArray[n] = _T("... ") + strMRUArray[n];
				}

				// Prefix the string with its number
				CString strVal;
				strVal.Format(_T("%d "), n+1);
				strMRUArray[n] = strVal + strMRUArray[n];
				MaxMRUArrayIndex = n;
			}
		}
		else
		{
			strMRUArray[0] = _T("Recent Files");
		}

		// Set MRU menu items
		MENUITEMINFO mii = {0};
		mii.cbSize = GetSizeofMenuItemInfo();

		int nFileItem = 0;  // We place the MRU items under the left most menu item
		CMenu* pFileMenu = GetFrameMenu()->GetSubMenu(nFileItem);

		if (pFileMenu)
		{
			// Remove all but the first MRU Menu entry
			for (UINT u = IDW_FILE_MRU_FILE2; u <= IDW_FILE_MRU_FILE1 +16; ++u)
			{
				pFileMenu->DeleteMenu(u, MF_BYCOMMAND);
			}

			int MaxMRUIndex = (int)MIN(MaxMRUArrayIndex, m_nMaxMRU);

			for (int index = MaxMRUIndex; index >= 0; --index)
			{
				mii.fMask = MIIM_TYPE | MIIM_ID | MIIM_STATE;
				mii.fState = (0 == m_vMRUEntries.size())? MFS_GRAYED : 0;
				mii.fType = MFT_STRING;
				mii.wID = IDW_FILE_MRU_FILE1 + index;
				mii.dwTypeData = (LPTSTR)strMRUArray[index].c_str();

				BOOL bResult;
				if (index == MaxMRUIndex)
					// Replace the last MRU entry first
					bResult = pFileMenu->SetMenuItemInfo(IDW_FILE_MRU_FILE1, &mii, FALSE);
				else
					// Insert the other MRU entries next
					bResult = pFileMenu->InsertMenuItem(IDW_FILE_MRU_FILE1 + index + 1, &mii, FALSE);

				if (!bResult)
				{
					TRACE("Failed to set MRU menu item\n");
					break;
				}
			}
		}

		DrawMenuBar();
	}

	inline LRESULT CFrame::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_ACTIVATE:		return OnActivate(wParam, lParam);
		case WM_DRAWITEM:		return OnDrawItem(wParam, lParam);
		case WM_ERASEBKGND:		return 0L;
		case WM_EXITMENULOOP:	return OnExitMenuLoop(wParam, lParam);
		case WM_HELP:			return OnHelp();
		case WM_INITMENUPOPUP:	return OnInitMenuPopup(wParam, lParam);
		case WM_MENUCHAR:		return OnMenuChar(wParam, lParam);
		case WM_MEASUREITEM:	return OnMeasureItem(wParam, lParam);
		case WM_MENUSELECT:		return OnMenuSelect(wParam, lParam);
		case WM_SETFOCUS:		return OnSetFocus(wParam, lParam);
		case WM_SIZE:			return OnSize(wParam, lParam);	
		case WM_SYSCOLORCHANGE:	return OnSysColorChange(wParam, lParam);
		case WM_SYSCOMMAND:		return OnSysCommand(wParam, lParam);
		case WM_TIMER:			return OnTimer(wParam, lParam);	

		// Messages defined by Win32++
		case UWM_GETMENUTHEME:		return (LRESULT)&m_MenuBarTheme;
		case UWM_GETREBARTHEME:		return (LRESULT)&m_ReBarTheme;
		case UWM_GETTOOLBARTHEME:	return (LRESULT)&m_ToolBarTheme;
		
		} // switch uMsg

		return CWnd::WndProcDefault(uMsg, wParam, lParam);
	} // LRESULT CFrame::WndProcDefault(...)


} // namespace Win32xx

#endif // _WIN32XX_FRAME_H_
