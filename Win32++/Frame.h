// Win32++  Version 5.3
// Released: 20th June, 2007 by:
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
	struct REBARTHEME
	{
		BOOL UseThemes;
		COLORREF clrBkGnd1;
		COLORREF clrBkGnd2;
		COLORREF clrBand1;
		COLORREF clrBand2;
		BOOL FlatStyle;			// Win2000 and above required
		BOOL KeepBandsLeft;
		BOOL LockMenuBand;
		BOOL RoundBorders;		// Win2000 and above required
		BOOL ShortBands;
		BOOL UseLines;
	};

	struct TOOLBARTHEME
	{
		BOOL UseThemes;
		COLORREF clrHot1;
		COLORREF clrHot2;
		COLORREF clrPressed1;
		COLORREF clrPressed2;
		COLORREF clrOutline;
	};

	//////////////////////////////////////
	// Declaration of the CStatusbar class
	//
	class CStatusbar : public CWnd
	{
	public:
		CStatusbar() {}
		virtual ~CStatusbar() {}
		virtual LPCTSTR GetPaneText(int iPane);
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void CreatePanes(int iPanes, const int iPaneWidths[]);
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
		virtual TOOLBARTHEME& GetTheme() {return m_Theme;}
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
		virtual void SetTheme(TOOLBARTHEME& Theme);

	protected:
		virtual void OnCreate();
		virtual LRESULT OnCustomDraw(NMHDR* pNMHDR);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		HIMAGELIST m_hImageList;
		HIMAGELIST m_hImageListHot;
		HIMAGELIST m_hImageListDis;
		std::map<tString, int> m_StringMap;
		UINT m_OldToolbarID;		// Bitmap Resource ID, used in AddBitmap/ReplaceBitmap
		TOOLBARTHEME m_Theme;

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
		virtual BOOL GetBandRect(int i, LPRECT pRect);
		virtual BOOL GetBarInfo(LPREBARINFO prbi) const;
		virtual int GetRowHeight(int nRow) const;
		virtual REBARTHEME& GetTheme() {return m_Theme;}
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
		virtual void SetTheme(REBARTHEME& Theme);
		virtual BOOL ShowBand(int nBand, BOOL fShow);
		virtual void ShowGripper(int nBand, BOOL fShow);
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		REBARTHEME m_Theme;
	};


	////////////////////////////////////
	// Declaration of the CMenubar class
	//
	class CMenubar : public CToolbar
	{
	public:
		CMenubar();
		virtual ~CMenubar();
		virtual HMENU GetMenu() {return m_hTopMenu;}
		virtual void MenuChar(WPARAM wParam, LPARAM lParam);
		virtual void SetIcons(const std::vector<UINT> ToolbarData, UINT nID_Image, COLORREF crMask);
		virtual void SetIcons(const std::vector<UINT> ImageData, HIMAGELIST hImageList);
		virtual void SysCommand(WPARAM wParam, LPARAM lParam);
		virtual void SetMenu(HMENU hMenu);

	protected:
		virtual void DoAltKey(WORD KeyCode);
		virtual void DoPopupMenu();
		virtual void DrawBackground(HDC hDC, RECT rc);
		virtual void DrawCheckmark(LPDRAWITEMSTRUCT pdis);
		virtual void DrawIcon(LPDRAWITEMSTRUCT pdis);
		virtual void DrawMDIButtons(HDC hDC);
		virtual void DrawMenuText(HDC hDC, LPCTSTR ItemText, RECT rc, COLORREF colorText);
		virtual void ExitMenu();
		virtual	void GrabFocus();
		virtual BOOL IsMDIChildMaxed();
		virtual void OnCreate();
		virtual LRESULT OnCustomDraw(NMHDR* pNMHDR);
		virtual BOOL OnDrawItem(WPARAM wParam, LPARAM lParam);
		virtual void OnInitMenuPopup(WPARAM wParam, LPARAM lParam);
		virtual void OnKeyDown(WPARAM wParam, LPARAM lParam);
		virtual void OnLButtonDown(WPARAM wParam, LPARAM lParam);
		virtual void OnLButtonUp(WPARAM wParam, LPARAM lParam);
		virtual void OnMDISetMenu(WPARAM wParam, LPARAM lParam);
		virtual BOOL OnMeasureItem(WPARAM wParam, LPARAM lParam);
		virtual void OnMouseMove(WPARAM wParam, LPARAM lParam);
		virtual BOOL OnMenuInput(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void OnWindowPosChanged();
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual void ReleaseFocus();
		virtual void RevertPopupMenu(HMENU hMenu);
		virtual void SetHotItem(int nHot);
		static LRESULT CALLBACK StaticMsgHook(int nCode, WPARAM wParam, LPARAM lParam);
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		enum Constants
		{
			USER_POPUPMENU  = WM_APP + 2,	// creates the popup menu
			POST_TEXT_GAP   = 16,			// for owner draw menu item
		};

		enum MDIButtonType
		{
			MDI_MIN = 1,
			MDI_RESTORE = 2,
			MDI_CLOSE = 3,
		};

		struct ItemData
		{
			HMENU hMenu;
			UINT  nPos;
			UINT  fType;
			TCHAR Text[MAX_MENU_STRING];
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
		HIMAGELIST m_hImageList;	// Images for popup menu icons
		std::vector<UINT> m_ImageData;


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
		virtual HMENU GetFrameMenu() {return m_hMenu;}
		virtual CMenubar& GetMenubar() {return m_Menubar;}
		virtual int GetMenuItemPos(HMENU hMenu, LPCTSTR szItem);
		virtual CStatusbar& GetStatusbar() {return m_Statusbar;}
		virtual CRebar& GetRebar() {return m_Rebar;}
		virtual CToolbar& GetToolbar() {return m_Toolbar;}
		virtual CWnd* GetView() {return m_pView;}
		virtual BOOL IsMDIFrame() {return m_bIsMDIFrame;}
		virtual BOOL IsMenubarUsed() {return (m_Menubar.GetHwnd() != 0);}
		virtual BOOL IsRebarSupported() {return (GetComCtlVersion() >= 470);}
		virtual BOOL IsRebarUsed() {return (m_Rebar.GetHwnd() != 0);}
		virtual void RecalcLayout();
		virtual void SetFrameMenu(INT ID_MENU);
		virtual void SetStatusIndicators();
		virtual void SetStatusText();
		virtual void SetView(CWnd& pView);
		virtual void UpdateCheckMarks();

	protected:
		virtual void AddMenubarBand(int Menubar_Height = MENUBAR_HEIGHT);
		virtual void AddToolbarBand(CToolbar& TB, std::vector<UINT> TBData, COLORREF clrMask, UINT ID_Normal, UINT ID_HOT = 0, UINT ID_Disabled = 0);
		virtual void LoadCommonControls(INITCOMMONCONTROLSEX InitStruct);
		virtual BOOL OnCommandFrame(WPARAM wPAram, LPARAM lParam);
		virtual void OnCreate();
		virtual void OnHelp();
		virtual void OnMenuSelect(WPARAM wParam, LPARAM lParam);
		virtual	LRESULT OnNotifyFrame(WPARAM wParam, LPARAM lParam);
		virtual void OnSetFocus();
		virtual void OnSysColorChange();
		virtual	void OnTimer(WPARAM wParam);
		virtual void OnViewStatusbar();
		virtual void OnViewToolbar();
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void SetBackground(HBITMAP);
		virtual void SetMenubarBandSize();
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		std::vector<UINT> m_ToolbarData;
		BOOL m_bIsMDIFrame;			// TRUE if this is a MDI frame
        BOOL m_bShowIndicatorStatus;	// set to TRUE to see indicators in status bar
		BOOL m_bShowMenuStatus;		// set to TRUE to see menu and toolbar updates in status bar
		BOOL m_bUseRebar;			// set to TRUE if Rebars are to be used
		tString m_StatusText;		// a TCHAR std::string for status text
		enum Constants
		{
			ID_STATUS_TIMER = 1,
			MENUBAR_HEIGHT  = 22,
			USER_REARRANGED = WM_APP + 1	// frame window rearranged message
		};

	private:
		CMenubar m_Menubar;			// CMenubar object
		CRebar m_Rebar;				// CRebar object
		CStatusbar m_Statusbar;		// CStatusbar object
		CToolbar m_Toolbar;			// CToolbar object
		HMENU m_hMenu;				// handle to the frame menu
		CWnd* m_pView;				// pointer to the View CWnd object

	};  // class CFrame

} //namespace Win32xx

#endif // FRAME_H
