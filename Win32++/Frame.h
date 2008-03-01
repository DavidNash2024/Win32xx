// Win32++  Version 6.0
// Released: 4th March, 2008 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: http://users.bigpond.net.au/programming/
//
//
// Copyright (c) 2005-2008  David Nash
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
// Frame.h
//  Declaration of the CFrame, CStatusbar and CToolbar classes

// The classes declared in this file support SDI (Single Document Interface)
// frames on Win32/Win64 operating systems (not Windows CE). For Windows CE,
// use WCEframe.h instead. SDI frames are a simple frame which supports a
// single client window. Refer to MDI.h for frames that support several
// child windows.

// CFrame also includes each of the following classes as members:
// * CRebar for managing the frame's rebar control.
// * CMenubar for managing the menu inside the rebar.
// * CToolbar for managing the frame's toolbar.
// * CStatusbar for managing the frame's status bar.
// In each case these members is exposed by a GetXXX function, allowing 
// them to be altered or sent messages.

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

#include "WinCore.h"
#include "dialog.h"
#include "GDI.h"


namespace Win32xx
{

	////////////////////////////////////////////////
	// Declarations for structures for themes
	//
	struct ThemeMenu
	{
		BOOL UseThemes;			// TRUE if themes are used
		COLORREF clrHot1;		// Color 1 for hot button
		COLORREF clrHot2;		// Color 2 for hot button
		COLORREF clrPressed1;	// Color 1 for pressed button
		COLORREF clrPressed2;	// Color 2 for pressed button
		COLORREF clrOutline;	// Colour for border outline
	};

	struct ThemeRebar
	{
		BOOL UseThemes;			// TRUE if themes are used
		COLORREF clrBkgnd1;		// Color 1 for rebar background
		COLORREF clrBkgnd2;		// Color 2 for rebar background
		COLORREF clrBand1;		// Color 1 for rebar band background. Use NULL if not required
		COLORREF clrBand2;		// Color 2 for rebar band background. Use NULL if not required
		BOOL FlatStyle;			// Bands are rendered with flat rather than raised style
		BOOL KeepBandsLeft;		// TRUE if we always keep bands left
		BOOL LockMenuBand;		// Lock Menubar's band up top, without gripper
		BOOL RoundBorders;		// Use rounded band borders
		BOOL ShortBands;        // Allows bands to be shorter than maximum available width
		BOOL UseLines;			// Displays horizontal lines between bands
	};

	struct ThemeToolbar
	{
		BOOL UseThemes;			// TRUE if themes are used
		COLORREF clrHot1;		// Color 1 for hot button
		COLORREF clrHot2;		// Color 2 for hot button
		COLORREF clrPressed1;	// Color 1 for pressed button
		COLORREF clrPressed2;	// Color 2 for pressed button
		COLORREF clrOutline;	// Colour for border outline
	};


	//////////////////////////////////////
	// Declaration of the CStatusbar class
	//
	class CStatusbar : public CWnd
	{
	public:
		CStatusbar();
		virtual ~CStatusbar() {}
		virtual void CreatePanes(int iPanes, const int iPaneWidths[]) const;
		virtual tString GetPaneText(int iPane);
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void SetPaneText(int iPane, LPCTSTR szText, UINT Style = 0) const;
		virtual void SetPaneWidth(int iPane, int iWidth) const;

	};


	////////////////////////////////////
	// Declaration of the CToolbar class
	//
	class CToolbar : public CWnd
	{
	public:
		CToolbar();
		virtual ~CToolbar();

		// These are the functions you might wish to override
		virtual void AddBitmap(int iNumButtons, UINT ToolbarID);
		virtual void ReplaceBitmap(int iNumButtons, UINT NewToolbarID);
		virtual int  SetButtons(const std::vector<UINT>& ToolbarData) const;
		virtual void SetButtonText(int iButtonID, LPCTSTR szText);
		virtual void SetImageList(int iNumButtons, COLORREF crMask, UINT ToolbarID, UINT ToolbarHotID = 0, UINT ToolbarDisabledID = 0);

		// These functions aren't intended to be overridden
		int  CommandToIndex(int iButtonID) const;
		void DisableButton(int iButtonID) const;
		void EnableButton(int iButtonID) const;
		int  GetButtonCount() const;
		UINT GetButtonState(int iButtonID) const;
		BYTE GetButtonStyle(int iButtonID) const;
		int  GetCommandID(int iIndex) const;
		RECT GetItemRect(int iIndex) const;
		SIZE GetMaxSize() const;
		ThemeToolbar& GetToolbarTheme() {return m_Theme;}
		BOOL HasText() const;
		int  HitTest() const;
		void SetBitmapSize(int cx, int cy) const;
		void SetButtonSize(int cx, int cy) const;
		void SetButtonState(int iButtonID, UINT State) const;
		void SetButtonStyle(int iButtonID, BYTE Style) const;
		void SetCommandID(int iIndex, int iButtonID) const;
		void SetToolbarTheme(ThemeToolbar& Theme);

	protected:
		virtual void OnCreate();
		virtual LRESULT OnCustomDraw(NMHDR* pNMHDR);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		HIMAGELIST m_hImageList;			// Imagelist for normal buttons
		HIMAGELIST m_hImageListHot;			// Imagelist for hot buttons
		HIMAGELIST m_hImageListDis;			// Imagelist for disabled buttons
		std::map<tString, int> m_StringMap;	// a map of strings used in SetButtonText
		UINT m_OldToolbarID;				// Bitmap Resource ID, used in AddBitmap/ReplaceBitmap
		ThemeToolbar m_Theme;				// The theme structure
		BOOL m_bDrawArrowBkgrnd;			// True if a seperate arrow background is to be drawn

	};  // class CToolbar


	////////////////////////////////////
	// Declaration of the CRebar class
	//
	class CRebar : public CWnd
	{
	public:
		CRebar();
		virtual ~CRebar();
				
		// These are the functions you might wish to override
		virtual void MoveBandsLeft();
		virtual void ShowGripper(int nBand, BOOL fShow) const;
		virtual BOOL ShowBand(int nBand, BOOL fShow) const;
		virtual void ResizeBand(const int nBand, SIZE sz) const;
		
		// These functions aren't intended to be overridden
		void DeleteBand(const int nBand) const ;
		int  GetBand(const HWND hWnd) const;
		RECT GetBandBorders(int nBand) const;
		int  GetBandCount() const;
		void GetBandInfo(const int nBand, LPREBARBANDINFO prbbi) const;
		RECT GetBandRect(int i) const;
		void GetBarInfo(LPREBARINFO prbi) const;
		int  GetRowHeight(int nRow) const;
		void InsertBand(const int nBand, LPREBARBANDINFO prbbi) const;
		BOOL IsBandVisible(int nBand) const;	
		void SetBandColor(const int nBand, const COLORREF clrFore, const COLORREF clrBack) const;
		void SetBandBitmap(const int nBand, const HBITMAP hBackground) const;
		void SetBandInfo(const int nBand, LPREBARBANDINFO prbbi) const;
		void SetBarInfo(LPREBARINFO prbi) const;
		ThemeRebar& GetRebarTheme() {return m_Theme;}
		void SetRebarTheme(ThemeRebar& Theme);

	protected:
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		virtual BOOL OnEraseBkgnd(HDC hDC);

		ThemeRebar m_Theme;
		BOOL m_bIsDragging;
	};


	////////////////////////////////////
	// Declaration of the CMenubar class
	//
	class CMenubar : public CToolbar
	{
	public:
		CMenubar();
		virtual ~CMenubar();
		virtual void MenuChar(WPARAM wParam, LPARAM lParam);
		virtual void SysCommand(WPARAM wParam, LPARAM lParam);

		HMENU GetMenu() const {return m_hTopMenu;}
		void SetMenu(HMENU hMenu);
		ThemeMenu& GetMenubarTheme() {return m_ThemeMenu;}
		void SetMenubarTheme(ThemeMenu& Theme);

	protected:
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

		enum Constants
		{
			USER_POPUPMENU  = WM_APP + 2,	// creates the popup menu
		};

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
		RECT  m_MDIRect[3];		// array of RECT for MDI buttons
		int   m_nHotItem;		// hot item
		int   m_nMDIButton;		// the MDI button (MDIButtonType) pressed
		POINT m_OldMousePos;	// old Mouse position
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
		virtual RECT GetClientSize();
		virtual int  GetMenuItemPos(HMENU hMenu, LPCTSTR szItem);
		virtual void OnDrawItem(WPARAM wParam, LPARAM lParam);
		virtual void OnExitMenuLoop();
		virtual void OnInitMenuPopup(WPARAM wParam, LPARAM lParam);
		virtual void OnMeasureItem(WPARAM wParam, LPARAM lParam);
		virtual void SetStatusIndicators();
		virtual void SetStatusText();
		virtual void SetTheme();
		virtual void RecalcLayout();
		virtual void UpdateCheckMarks();

		// These functions aren't intended to be overridden
		HMENU GetFrameMenu() const	{return m_hMenu;}
		ThemeMenu& GetMenuTheme()	{return m_ThemeMenu;}
		CWnd* GetView() const		{return m_pView;}
		void SetFrameMenu(INT ID_MENU);
		void SetMenuTheme(ThemeMenu& Theme);
		void SetView(CWnd& pView);

		CMenubar& GetMenubar()		{return m_Menubar;}
		CRebar& GetRebar()			{return m_Rebar;}
		CStatusbar& GetStatusbar()	{return m_Statusbar;}
		CToolbar& GetToolbar()		{return m_Toolbar;}

		BOOL IsMDIFrame() const			{return m_bIsMDIFrame;}
		BOOL IsMenubarUsed() const		{return (m_Menubar != 0);}
		BOOL IsRebarSupported() const	{return (GetComCtlVersion() >= 470);}
		BOOL IsRebarUsed() const		{return (m_Rebar != 0);}

	protected:
		// These are the functions you might wish to override
		virtual BOOL AddMenuIcon(int nID_MenuItem, HICON hIcon, int cx = 16, int cy = 16);
		virtual int  AddMenuIcons(const std::vector<UINT>& MenuData, UINT nID_Image, COLORREF crMask);
		virtual void AddMenubarBand(int Menubar_Height = MENUBAR_HEIGHT);
		virtual void AddToolbarBand(CToolbar& TB, const std::vector<UINT>& TBData, COLORREF clrMask, UINT ID_Normal, UINT ID_HOT = 0, UINT ID_Disabled = 0);
		virtual void DrawCheckmark(LPDRAWITEMSTRUCT pdis);
		virtual void DrawMenuIcon(LPDRAWITEMSTRUCT pdis, BOOL bDisabled);
		virtual void DrawMenuText(HDC DrawDC, LPCTSTR ItemText, RECT rc, COLORREF colorText);
		virtual void OnCreate();
		virtual void OnHelp();
		virtual void OnMenuSelect(WPARAM wParam, LPARAM lParam);
		virtual void OnSetFocus();
		virtual void OnSysColorChange();
		virtual	void OnTimer(WPARAM wParam);
		virtual void OnViewStatusbar();
		virtual void OnViewToolbar();
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// Its unlikely you would need to override these functions
		virtual void SetMenubarBandSize();

		enum Constants
		{
			ID_STATUS_TIMER = 1,
			MENUBAR_HEIGHT  = 22,
			POST_TEXT_GAP   = 16,			// for owner draw menu item
			USER_REARRANGED = WM_APP + 1	// frame window rearranged message
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
		tString m_StatusText;				// a TCHAR std::string for status text
		ThemeMenu m_ThemeMenu;				// Theme structure
		HIMAGELIST m_hImageList;			// Imagelist of menu icons
		HIMAGELIST m_hImageListDis;			// Imagelist of disabled menu icons

	private:
		void LoadCommonControls(INITCOMMONCONTROLSEX InitStruct);
		BOOL OnCommandFrame(WPARAM wPAram, LPARAM lParam);
		LRESULT OnNotifyFrame(WPARAM wParam, LPARAM lParam);

		CDialog* m_pAboutDialog;// Pointer to the about dialog 
		CMenubar m_Menubar;		// CMenubar object
		CRebar m_Rebar;			// CRebar object
		CStatusbar m_Statusbar;	// CStatusbar object
		CToolbar m_Toolbar;		// CToolbar object
		HMENU m_hMenu;			// handle to the frame menu
		CWnd* m_pView;			// pointer to the View CWnd object
		LPCTSTR m_OldStatus[3];	// Array of TCHAR pointers;

	};  // class CFrame


	//////////////////////////////////
	// Declaration of global functions
	//
	inline HIMAGELIST CreateDisabledImageList(HIMAGELIST hImageList)
	// Returns a greyed image list, created from hImageList
	{
		int cx, cy;
		int nCount = ImageList_GetImageCount(hImageList);
		if (0 == nCount)
			return NULL;

		ImageList_GetIconSize(hImageList, &cx, &cy);

		// Create the destination ImageList
		HIMAGELIST hImageListDis = ImageList_Create(cx, cy, ILC_COLOR32 | ILC_MASK, nCount, 0);

		// Process each image in the ImageList
		for (int i = 0 ; i < nCount; i++)
		{
			CDC DesktopDC = ::GetDC(NULL);
			CDC MemDC = ::CreateCompatibleDC(NULL);
			MemDC.CreateCompatibleBitmap(DesktopDC, cx, cx);
			RECT rc;
			SetRect(&rc, 0, 0, cx, cx);

			// Set the mask color to magenta for the new ImageList
			COLORREF crMask = RGB(255,0,255);
			SolidFill(MemDC, crMask, &rc);

			// Draw the image on the memory DC
			ImageList_Draw(hImageList, i, MemDC, 0, 0, ILD_TRANSPARENT);

			// Convert colored pixels to gray
			for (int x = 0 ; x < cx; x++)
			{
				for (int y = 0; y < cy; y++)
				{
					COLORREF clr = ::GetPixel(MemDC, x, y);

					if (clr != crMask)
					{
						BYTE byGray = 95 + (GetRValue(clr) *3 + GetGValue(clr)*6 + GetBValue(clr))/20;
						::SetPixel(MemDC, x, y, RGB(byGray, byGray, byGray));
					}
				}
			}

			// Detach the bitmap so we can use it.
			HBITMAP hbm = MemDC.DetachBitmap();
			ImageList_AddMasked(hImageListDis, hbm, crMask);
			::DeleteObject(hbm);
		}

		return hImageListDis;
	}

} // namespace Win32xx

#endif // FRAME_H
