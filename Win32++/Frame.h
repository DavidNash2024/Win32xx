// Win32++  Version 5.5
// Released: 4th October, 2007 by:
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

	struct ThemeMenubar
	{
		BOOL UseThemes;			// TRUE if themes are used
		COLORREF clrHot1;		// Color 1 for hot button
		COLORREF clrHot2;		// Color 2 for hot button
		COLORREF clrPressed1;	// Color 1 for pressed button and side bar in menu item
		COLORREF clrPressed2;	// Color 2 for pressed button and side bar in menu item
		COLORREF clrOutline;	// Colour for border outline
	};

	struct ThemeRebar
	{
		BOOL UseThemes;			// TRUE if themes are used
		COLORREF clrBkGnd1;		// Color 1 for rebar background
		COLORREF clrBkGnd2;		// Color 2 for rebar background
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
		virtual void PreCreate(CREATESTRUCT& cs);

		LPCTSTR GetPaneText(int iPane);
		void CreatePanes(int iPanes, const int iPaneWidths[]);
		void SetPaneText(int iPane, LPCTSTR szText, UINT Style = 0);
		void SetPaneWidth(int iPane, int iWidth);

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
		
		void AddBitmap(int iNumButtons, UINT ToolbarID);
		int  CommandToIndex(int iButtonID);
		void CreateDisabledImageList();
		void DisableButton(int iButtonID);
		void EnableButton(int iButtonID);
		int  GetButtonCount();
		UINT GetButtonState(int iButtonID);
		BYTE GetButtonStyle(int iButtonID);
		int  GetCommandID(int iIndex);
		RECT GetItemRect(int iIndex);
		SIZE GetMaxSize();
		ThemeToolbar& GetTheme() {return m_Theme;}
		BOOL HasText();
		int  HitTest();
		void ReplaceBitmap(int iNumButtons, UINT NewToolbarID);
		void SetBitmapSize(int cx, int cy);
		int  SetButtons(const std::vector<UINT> ToolbarData);
		void SetButtonSize(int cx, int cy);
		void SetButtonState(int iButtonID, UINT State);
		void SetButtonStyle(int iButtonID, BYTE Style);
		void SetButtonText(int iButtonID, LPCTSTR szText);
		void SetCommandID(int iIndex, int iButtonID);
		void SetImageList(int iNumButtons, COLORREF crMask, UINT ToolbarID, UINT ToolbarHotID = 0, UINT ToolbarDisabledID = 0);
		void SetTheme(ThemeToolbar& Theme);

	protected:
		virtual void OnCreate();
		virtual LRESULT OnCustomDraw(NMHDR* pNMHDR);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
		virtual BOOL OnEraseBkGnd(HDC hDC);
		virtual void MoveBandsLeft();
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void ResizeBand(const int nBand, SIZE sz);
		virtual void SetBandColor(const int nBand, const COLORREF clrFore, const COLORREF clrBack);
		virtual	void SetBandBitmap(const int nBand, const HBITMAP hBackground);
		virtual BOOL SetBandInfo(const int nBand, LPREBARBANDINFO prbbi);
		virtual BOOL SetBarInfo(LPREBARINFO prbi);
		virtual BOOL ShowBand(int nBand, BOOL fShow);
		virtual void ShowGripper(int nBand, BOOL fShow);
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		ThemeRebar& GetTheme() {return m_Theme;}
		void SetTheme(ThemeRebar& Theme);

	private:
		ThemeRebar m_Theme;
	};


	////////////////////////////////////
	// Declaration of the CMenubar class
	//
	class CMenubar : public CToolbar
	{
	public:
		CMenubar();
		virtual ~CMenubar();		
		
		HMENU GetMenu() {return m_hTopMenu;}	
		void MenuChar(WPARAM wParam, LPARAM lParam);
		void SetIcons(const std::vector<UINT> ToolbarData, UINT nID_Image, COLORREF crMask);
		void SetIcons(const std::vector<UINT> ImageData, HIMAGELIST hImageList);
		void SysCommand(WPARAM wParam, LPARAM lParam);
		void SetMenu(HMENU hMenu);
		void SetTheme(ThemeMenubar& Theme);	
		ThemeMenubar& GetTheme() {return m_Theme;}

	protected:
		virtual void OnCreate();
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		void DoAltKey(WORD KeyCode);
		void DoPopupMenu();
		void DrawCheckmark(LPDRAWITEMSTRUCT pdis);
		void DrawIcon(LPDRAWITEMSTRUCT pdis, BOOL bDisabled);
		void DrawAllMDIButtons(HDC hDC);
		void DrawMDIButton(HDC hDC, int iButton, UINT uState);
		void DrawMenuText(HDC hDC, LPCTSTR ItemText, RECT rc, COLORREF colorText);
		void ExitMenu();
		void GrabFocus();
		BOOL IsMDIChildMaxed();		
		LRESULT OnCustomDraw(NMHDR* pNMHDR);
		BOOL OnDrawItem(WPARAM wParam, LPARAM lParam);
		void OnInitMenuPopup(WPARAM wParam, LPARAM lParam);
		void OnKeyDown(WPARAM wParam, LPARAM lParam);
		void OnLButtonDown(WPARAM wParam, LPARAM lParam);
		void OnLButtonUp(WPARAM wParam, LPARAM lParam);
		void OnMDISetMenu(WPARAM wParam, LPARAM lParam);
		BOOL OnMeasureItem(WPARAM wParam, LPARAM lParam);
		void OnMouseLeave();
		void OnMouseMove(WPARAM wParam, LPARAM lParam);
		BOOL OnMenuInput(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void OnWindowPosChanged();
		void ReleaseFocus();
		void RevertPopupMenu(HMENU hMenu);
		void SetHotItem(int nHot);
		static LRESULT CALLBACK StaticMsgHook(int nCode, WPARAM wParam, LPARAM lParam);	

	private:
		enum Constants
		{
			USER_POPUPMENU  = WM_APP + 2,	// creates the popup menu
			POST_TEXT_GAP   = 16,			// for owner draw menu item
		};

		enum MDIButtonType
		{
			MDI_MIN = 0,
			MDI_RESTORE = 1,
			MDI_CLOSE = 2,
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
		std::vector<ItemData*> m_vpItemData;	// vector or ItemData pointers
		std::vector<UINT> m_ImageData;
		ThemeMenubar m_Theme;		// Theme structure


	};  // class CMenubar

	//////////////////////////////////
	// Declaration of the CFrame class
	//
	class CFrame : public CWnd
	{
	public:
		CFrame();
		virtual ~CFrame();
		virtual RECT GetClientSize();
		virtual int GetMenuItemPos(HMENU hMenu, LPCTSTR szItem);
		virtual void SetStatusIndicators();
		virtual void SetStatusText();
		virtual void SetTheme();
		virtual void RecalcLayout();
		virtual void UpdateCheckMarks();
		
		HMENU GetFrameMenu() {return m_hMenu;}
		CMenubar& GetMenubar() {return m_Menubar;}	
		CRebar& GetRebar() {return m_Rebar;}
		CStatusbar& GetStatusbar() {return m_Statusbar;}
		CToolbar& GetToolbar() {return m_Toolbar;}
		CWnd* GetView() {return m_pView;}
		BOOL IsMDIFrame() {return m_bIsMDIFrame;}
		BOOL IsMenubarUsed() {return (m_Menubar.GetHwnd() != 0);}
		BOOL IsRebarSupported() {return (GetComCtlVersion() >= 470);}
		BOOL IsRebarUsed() {return (m_Rebar.GetHwnd() != 0);}
		void SetFrameMenu(INT ID_MENU);
		void SetView(CWnd& pView);	

	protected:
		virtual void AddMenubarBand(int Menubar_Height = MENUBAR_HEIGHT);
		virtual void AddToolbarBand(CToolbar& TB, std::vector<UINT> TBData, COLORREF clrMask, UINT ID_Normal, UINT ID_HOT = 0, UINT ID_Disabled = 0);	
		virtual void OnCreate();
		virtual void OnHelp();
		virtual void OnMenuSelect(WPARAM wParam, LPARAM lParam);
		virtual void OnSetFocus();
		virtual void OnSysColorChange();
		virtual	void OnTimer(WPARAM wParam);
		virtual void OnViewStatusbar();
		virtual void OnViewToolbar();
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void SetMenubarBandSize();
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		std::vector<UINT> m_ToolbarData;
		BOOL m_bIsMDIFrame;			// TRUE if this is a MDI frame
        BOOL m_bShowIndicatorStatus;	// set to TRUE to see indicators in status bar
		BOOL m_bShowMenuStatus;		// set to TRUE to see menu and toolbar updates in status bar
		BOOL m_bUseRebar;			// set to TRUE if Rebars are to be used
		BOOL m_bUseThemes;          // set to TRUE for themes
		tString m_StatusText;		// a TCHAR std::string for status text
		enum Constants
		{
			ID_STATUS_TIMER = 1,
			MENUBAR_HEIGHT  = 22,
			USER_REARRANGED = WM_APP + 1	// frame window rearranged message
		};

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

} //namespace Win32xx

#endif // FRAME_H
