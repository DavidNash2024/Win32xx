// Latest verion available at:
// http://sourceforge.net/projects/win32-framework


// Win32++  Version 5.6
// Released: 10th November, 2007 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: http://users.bigpond.net.au/programming/
//
//
// Copyright (c) 2005-2007  David Nash
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


//////////////////////////////////////////
// Frame.h
//  Declaration of the CFrame, CStatusbar and CToolbar classes

#ifndef FRAME_H
#define FRAME_H

#include "WinCore.h"
#include "dialog.h"


namespace Win32xx
{
	////////////////////////////////////////////////
	// Declarations for some useful functions
	//
	void GradientFill(HDC hDC, COLORREF Color1, COLORREF Color2, LPRECT pRc, BOOL bVertical);
	void SolidFill(HDC hDC, COLORREF Color, LPRECT pRc);


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
		CStatusbar() {}
		virtual ~CStatusbar() {}
		virtual void CreatePanes(int iPanes, const int iPaneWidths[]);
		virtual LPCTSTR GetPaneText(int iPane);
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void SetPaneText(int iPane, LPCTSTR szText, UINT Style = 0);
		virtual void SetPaneWidth(int iPane, int iWidth);

	private:
		TCHAR m_szText[80];
	};


	////////////////////////////////////
	// Declaration of the CToolbar class
	//
	class CToolbar : public CWnd
	{
	public:
		CToolbar();
		virtual ~CToolbar();
		virtual void AddBitmap(int iNumButtons, UINT ToolbarID);
		virtual int  CommandToIndex(int iButtonID);
		virtual void DisableButton(int iButtonID);
		virtual void EnableButton(int iButtonID);
		virtual int  GetButtonCount();
		virtual UINT GetButtonState(int iButtonID);
		virtual BYTE GetButtonStyle(int iButtonID);
		virtual int  GetCommandID(int iIndex);
		virtual RECT GetItemRect(int iIndex);
		virtual SIZE GetMaxSize();
		virtual BOOL HasText();
		virtual int  HitTest();
		virtual void ReplaceBitmap(int iNumButtons, UINT NewToolbarID);
		virtual void SetBitmapSize(int cx, int cy);
		virtual int  SetButtons(const std::vector<UINT> ToolbarData);
		virtual void SetButtonSize(int cx, int cy);
		virtual void SetButtonState(int iButtonID, UINT State);
		virtual void SetButtonStyle(int iButtonID, BYTE Style);
		virtual void SetButtonText(int iButtonID, LPCTSTR szText);
		virtual void SetCommandID(int iIndex, int iButtonID);
		virtual void SetImageList(int iNumButtons, COLORREF crMask, UINT ToolbarID, UINT ToolbarHotID = 0, UINT ToolbarDisabledID = 0);

		ThemeToolbar& GetToolbarTheme() {return m_Theme;}
		void SetToolbarTheme(ThemeToolbar& Theme);

	protected:
		virtual void OnCreate();
		virtual LRESULT OnCustomDraw(NMHDR* pNMHDR);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		struct ItemData
		// Each Dropdown menu item has this data
		{
			HMENU hMenu;
			UINT  nPos;
			UINT  fType;
			TCHAR Text[MAX_MENU_STRING];
			HMENU hSubMenu;
		};

		HIMAGELIST m_hImageList;
		HIMAGELIST m_hImageListHot;
		HIMAGELIST m_hImageListDis;

	private:
		std::map<tString, int> m_StringMap;
		UINT m_OldToolbarID;		// Bitmap Resource ID, used in AddBitmap/ReplaceBitmap
		ThemeToolbar m_Theme;
		BOOL m_bDrawArrowBkgrnd;

	};  // class CToolbar


	////////////////////////////////////
	// Declaration of the CRebar class
	//
	class CRebar : public CWnd
	{
	public:
		CRebar();
		virtual ~CRebar();
		virtual BOOL DeleteBand(const int nBand);
		virtual int GetBand(const HWND hWnd) const;
		virtual RECT GetBandBorders(int nBand);
		virtual int GetBandCount() const;
		virtual BOOL GetBandInfo(const int nBand, LPREBARBANDINFO prbbi) const;
		virtual RECT GetBandRect(int i);
		virtual BOOL GetBarInfo(LPREBARINFO prbi) const;
		virtual int GetRowHeight(int nRow) const;
		virtual BOOL InsertBand(const int nBand, LPREBARBANDINFO prbbi);
		virtual BOOL IsBandVisible(int nBand);
		virtual BOOL OnEraseBkgnd(HDC hDC);
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void MoveBandsLeft();
		virtual void ResizeBand(const int nBand, SIZE sz);
		virtual void SetBandColor(const int nBand, const COLORREF clrFore, const COLORREF clrBack);
		virtual void SetBandBitmap(const int nBand, const HBITMAP hBackground);
		virtual BOOL SetBandInfo(const int nBand, LPREBARBANDINFO prbbi);
		virtual BOOL SetBarInfo(LPREBARINFO prbi);
		virtual BOOL ShowBand(int nBand, BOOL fShow);
		virtual void ShowGripper(int nBand, BOOL fShow);
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		ThemeRebar& GetRebarTheme() {return m_Theme;}
		void SetRebarTheme(ThemeRebar& Theme);

	private:
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

		HMENU GetMenu() {return m_hTopMenu;}
		void SetMenu(HMENU hMenu);
		ThemeMenu& GetMenubarTheme() {return m_ThemeMenu;}
		void SetMenubarTheme(ThemeMenu& Theme);

	protected:
		virtual void OnCreate();
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		void DoAltKey(WORD KeyCode);
		void DoPopupMenu();
		void DrawAllMDIButtons(HDC hDC);
		void DrawMDIButton(HDC hDC, int iButton, UINT uState);
		void ExitMenu();
		HWND GetActiveMDIChild();
		void GrabFocus();
		BOOL IsMDIChildMaxed();
		BOOL IsMDIFrame();
		LRESULT OnCustomDraw(NMHDR* pNMHDR);
		void OnKeyDown(WPARAM wParam, LPARAM lParam);
		void OnLButtonDown(WPARAM wParam, LPARAM lParam);
		void OnLButtonUp(WPARAM wParam, LPARAM lParam);
		void OnMouseLeave();
		void OnMouseMove(WPARAM wParam, LPARAM lParam);
		BOOL OnMenuInput(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void OnWindowPosChanged();
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

		BOOL m_bExitAfter;			// Exit after Popup menu ends
		BOOL m_bKeyMode;			// keyboard navigation mode
		BOOL m_bMenuActive;			// popup menu active
		BOOL m_bSelPopup;           // a popup (cascade) menu is selected
		HMENU m_hPopupMenu;			// handle to the popup menu
		HMENU m_hSelMenu;			// handle to the casceded popup menu
		HMENU m_hTopMenu;			// handle to the top level menu
		HWND m_hPrevFocus;			// handle to window which had focus
		RECT m_MDIRect[3];			// array of RECT for MDI buttons
		int m_nHotItem;				// hot item
		int m_nMDIButton;           // the MDI button pressed
		POINT m_OldMousePos;        // old Mouse position
		ThemeMenu m_ThemeMenu;		// Theme structure

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
		virtual HIMAGELIST CreateDisabledImageList(HIMAGELIST hImageList);
		virtual RECT GetClientSize();
		virtual int  GetMenuItemPos(HMENU hMenu, LPCTSTR szItem);
		virtual BOOL OnDrawItem(WPARAM wParam, LPARAM lParam);
		virtual void OnInitMenuPopup(WPARAM wParam, LPARAM lParam);
		virtual BOOL OnMeasureItem(WPARAM wParam, LPARAM lParam);
		virtual void RevertPopupMenu(HMENU hMenu);
		virtual void SetStatusIndicators();
		virtual void SetStatusText();
		virtual void SetTheme();
		virtual void RecalcLayout();
		virtual void UpdateCheckMarks();

		// Its unlikely you would need to override these functions
		virtual HMENU GetFrameMenu() {return m_hMenu;}
		virtual void SetFrameMenu(INT ID_MENU);
		virtual ThemeMenu& GetMenuTheme() {return m_ThemeMenu;}
		virtual void SetMenuTheme(ThemeMenu& Theme);
		virtual CWnd* GetView() {return m_pView;}
		virtual void SetView(CWnd& pView);

		virtual CMenubar& GetMenubar() {return m_Menubar;}
		virtual CRebar& GetRebar() {return m_Rebar;}
		virtual CStatusbar& GetStatusbar() {return m_Statusbar;}
		virtual CToolbar& GetToolbar() {return m_Toolbar;}

		BOOL IsMDIFrame() {return m_bIsMDIFrame;}
		BOOL IsMenubarUsed() {return (m_Menubar.GetHwnd() != 0);}
		BOOL IsRebarSupported() {return (GetComCtlVersion() >= 470);}
		BOOL IsRebarUsed() {return (m_Rebar.GetHwnd() != 0);}

	protected:
		// These are the functions you might wish to override
		virtual BOOL AddMenuIcon(int nID_MenuItem, HICON hIcon, int cx = 16, int cy = 16);
		virtual int  AddMenuIcons(const std::vector<UINT> MenuData, UINT nID_Image, COLORREF crMask);
		virtual void AddMenubarBand(int Menubar_Height = MENUBAR_HEIGHT);
		virtual void AddToolbarBand(CToolbar& TB, std::vector<UINT> TBData, COLORREF clrMask, UINT ID_Normal, UINT ID_HOT = 0, UINT ID_Disabled = 0);
		virtual void DrawCheckmark(LPDRAWITEMSTRUCT pdis);
		virtual void DrawMenuIcon(LPDRAWITEMSTRUCT pdis, BOOL bDisabled);
		virtual void DrawMenuText(HDC hDC, LPCTSTR ItemText, RECT rc, COLORREF colorText);
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

		std::vector<UINT> m_ToolbarData;
		std::vector<ItemData*> m_vpItemData;	// vector of ItemData pointers
		std::vector<UINT> m_MenuData;			// vector of menu icons
		BOOL m_bIsMDIFrame;			// TRUE if this is a MDI frame
        BOOL m_bShowIndicatorStatus;	// set to TRUE to see indicators in status bar
		BOOL m_bShowMenuStatus;		// set to TRUE to see menu and toolbar updates in status bar
		BOOL m_bUseRebar;			// set to TRUE if Rebars are to be used
		BOOL m_bUseThemes;          // set to TRUE for themes
		tString m_StatusText;		// a TCHAR std::string for status text
		ThemeMenu m_ThemeMenu;	// Theme structure
		HIMAGELIST m_hImageList;	// Imagelist of menu icons
		HIMAGELIST m_hImageListDis;	// Imagelist of disabled menu icons


	private:
		void LoadCommonControls(INITCOMMONCONTROLSEX InitStruct);
		BOOL OnCommandFrame(WPARAM wPAram, LPARAM lParam);
		LRESULT OnNotifyFrame(WPARAM wParam, LPARAM lParam);

		CMenubar m_Menubar;			// CMenubar object
		CRebar m_Rebar;				// CRebar object
		CStatusbar m_Statusbar;		// CStatusbar object
		CToolbar m_Toolbar;			// CToolbar object
		HMENU m_hMenu;				// handle to the frame menu
		CWnd* m_pView;				// pointer to the View CWnd object

	};  // class CFrame

} // namespace Win32xx

#endif // FRAME_H
