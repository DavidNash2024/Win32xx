// Win32++  Version 5.1 Beta
// Modified: 6th February, 2007 by:
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
#include <map>
#include <string>
#include <vector>


namespace Win32xx
{

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
		virtual void GetItemRect(int iIndex, RECT* lpRect);
		virtual int  HitTest();
		virtual void SetBitmapSize(int cx, int cy);
		virtual int  SetButtons(const std::vector<UINT> ToolbarData);
		virtual void SetButtonSize(int cx, int cy);
		virtual void SetButtonState(int iButtonID, UINT State);
		virtual void SetButtonStyle(int iButtonID, BYTE Style);
		virtual void SetButtonText(int iButtonID, LPCTSTR szText);
		virtual void SetCommandID(int iIndex, int iButtonID);
		virtual void SetImageList(int iNumButtons, COLORREF crMask, UINT ToolbarID, UINT ToolbarHotID, UINT ToolbarDisabledID);

	protected:
		virtual void OnCreate();
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		HIMAGELIST m_hImageList;
		HIMAGELIST m_hImageListHot;
		HIMAGELIST m_hImageListDis;
		std::map<std::basic_string<TCHAR>, int> m_StringMap;

	};  // class CToolbar


	////////////////////////////////////
	// Declaration of the CRebar class
	//
	class CRebar : public CWnd
	{
	public:
		CRebar();
		virtual ~CRebar(){}
		virtual BOOL DeleteBand(const int nBand);
		virtual int GetBand(const HWND hWnd) const;
		virtual int GetBandCount() const;
		virtual BOOL GetBandInfo(const int nBand, LPREBARBANDINFO prbbi) const;
		virtual BOOL GetBarInfo(LPREBARINFO prbi) const;
		virtual BOOL InsertBand(const int nBand, LPREBARBANDINFO prbbi);
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void ResizeBand(const int nBand, const int nSize);
		virtual void SetBandColor(const int nBand, const COLORREF clrFore, const COLORREF clrBack);
		virtual	void SetBandBitmap(const int nBand, const HBITMAP hBackground);
		virtual BOOL SetBandInfo(const int nBand, LPREBARBANDINFO prbbi);
		virtual BOOL SetBarInfo(LPREBARINFO prbi);

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
		virtual void SysCommand(WPARAM wParam, LPARAM lParam);
		virtual void SetMenu(HMENU hMenu);

	protected:
		virtual void DoAltKey(WORD KeyCode);
		virtual void DoPopupMenu();
		virtual void DrawMDIButtons(HDC hDC);
		virtual void ExitMenu();
		virtual	void GrabFocus();
		virtual void OnCreate();
		virtual void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
		virtual void OnKeyDown(WPARAM wParam, LPARAM lParam);
		virtual void OnLButtonDown(WPARAM wParam, LPARAM lParam);
		virtual void OnLButtonUp(WPARAM wParam, LPARAM lParam);
		virtual void OnMDISetMenu(WPARAM wParam, LPARAM lParam);
		virtual void OnMouseMove(WPARAM wParam, LPARAM lParam);
		virtual BOOL OnMenuInput(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void OnWindowPosChanged();
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual void ReleaseFocus();
		virtual void SetHotItem(int nHot);
		static LRESULT CALLBACK StaticIconHook(int nCode, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK StaticMsgHook(int nCode, WPARAM wParam, LPARAM lParam);
		virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		enum Constants
		{
			MAX_MENU_STRING = 32,
			USER_POPUPMENU  = WM_APP + 2,	// creates the popup menu
		};

		enum MDIButtonType
		{
			MDI_MIN = 1,
			MDI_RESTORE = 2,
			MDI_CLOSE = 3,
		};

		BOOL m_bExitAfter;			// Exit after Popup menu ends
		BOOL m_bKeyMode;			// keyboard navigation mode
		BOOL m_bMenuActive;			// popup menu active
		BOOL m_bSelPopup;           // a popup (cascade) menu is selected
		HMENU m_hPopupMenu;			// handle to the popup menu
		HMENU m_hSelMenu;			// handle to the casceded popup menu
		HMENU m_hTopMenu;			// handle to the top level menu
		HWND m_hPrevFocus;			// handle to window which had focus
		RECT m_IconRect;			// Dimensions of the icon in the menubar
		RECT m_MDIRect[3];			// array of RECT for MDI buttons
		int m_nHotItem;				// hot item
		int m_nButtonCount;			// number of top level menu items
		int m_nMaxedFlag;			// 1 if Maxed MDI Child, 0 otherwise
		int m_nMDIButton;           // the MDI button pressed
		POINT m_OldMousePos;        // old Mouse position

	};  // class CMenubar

	//////////////////////////////////
	// Declaration of the CFrame class
	//
	class CFrame : public CWnd
	{
		friend class CMDIFrame;
		friend class CToolbar;
		friend class CWinApp;
	public:
		CFrame();
		virtual ~CFrame();
		virtual RECT GetClientSize();
		virtual HMENU GetFrameMenu() {return m_hMenu;}
		virtual CMenubar& GetMenubar() {return m_Menubar;}
		virtual CStatusbar& GetStatusbar() {return m_Statusbar;}
		virtual CRebar& GetRebar() {return m_Rebar;}
		virtual CToolbar& GetToolbar() {return m_Toolbar;}
		virtual CWnd* GetView() {return m_pView;}
		virtual BOOL IsMDIFrame() {return m_bIsMDIFrame;}
		virtual BOOL IsMenubarUsed() {return (m_Menubar.GetHwnd() != 0);}
		virtual BOOL IsRebarSupported() {return m_bSupportRebars;}
		virtual BOOL IsRebarUsed() {return (m_Rebar.GetHwnd() != 0);}
		virtual void SetView(CWnd& pView);
		virtual void SetStatusIndicators();
		virtual void SetStatusText();

	protected:
		virtual void AddMenubarBand(int Menubar_Height = MENUBAR_HEIGHT);
		virtual void AddToolbarBand(int Toolbar_Height = TOOLBAR_HEIGHT);
		virtual void LoadCommonControls(INITCOMMONCONTROLSEX InitStruct);
		virtual BOOL OnCommand(UINT nID);
		virtual void OnCreate();
		virtual void OnHelp();
		virtual void OnMenuSelect(WPARAM wParam, LPARAM lParam);
		virtual	LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual void OnSetFocus();
		virtual void OnSysColorChange();
		virtual void OnViewStatusbar();
		virtual void OnViewToolbar();
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void RecalcLayout();
		virtual void SetBackground(HBITMAP);
		virtual void SetButtons(const std::vector<UINT> ToolbarData);
		virtual void ToolbarNotify(int nButton);
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		std::vector<UINT> m_ToolbarData;
        BOOL m_bShowIndicatorStatus;	// set to TRUE to see indicators in status bar
		BOOL m_bShowMenuStatus;		// set to TRUE to see menu and toolbar updates in status bar
		BOOL m_bUseRebar;			// set to TRUE if Rebars are to be used
		std::basic_string<TCHAR> m_StatusText;		// a TCHAR std::string for status text

	private:
		CMenubar m_Menubar;			// CMenubar object
		CRebar m_Rebar;				// CRebar object
		CStatusbar m_Statusbar;		// CStatusbar object
		CToolbar m_Toolbar;			// CToolbar object
		BOOL m_bIsMDIFrame;			// TRUE if this is a MDI frame
		BOOL m_bSupportRebars;		// TRUE if rebars are supported by the OS
		HMENU m_hMenu;				// handle to the frame menu
		CWnd* m_pView;				// pointer to the View CWnd object
		enum Constants
		{
			ID_STATUS_TIMER = 1,
			MENUBAR_HEIGHT  = 22,
			TOOLBAR_HEIGHT  = 24,
			USER_REARRANGED = WM_APP + 1	// frame window rearranged message
		};
	};  // class CFrame

} //namespace Win32xx

#endif // FRAME_H
