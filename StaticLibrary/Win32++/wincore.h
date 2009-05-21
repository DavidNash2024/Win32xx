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


///////////////////////////////////////////////////////
// wincore.h
//  Declaration of the following classes:
//  CWinApp, CWnd, CWinException, CCriticalSection,
//  CPoint, CRect, and CSize
//
// This file contains the declarations for the core set of classes required to
// create simple windows using Win32++.  Seven classes are declared here:
//
// 1) CCriticalSection: This class is used internally to manage thread access
//            to shared resources. You can also use this class to lock and
//            release your own critical sections.
//
// 2) CWinException: This class is used internally by Win32++ to handle
//            exceptions. You can also use it to throw and catch exceptions.
//
// 3) WinApp: This class is used start Win32++ and run the message loop. You
//            should inherit from this class to start Win32++ in your own
//            application.
//
// 4) CWnd:   This class is used to represent a window. It provides a means
//            of creating the window, and handling its messages. Inherit
//            from this class to define and control windows.
//
// 5) CPoint, CRect and CSize:
//            These class can be used in place of a POINT, RECT or SIZE
//            repectively.
//
// Note: This header file (or another Win32++ header file which includes it)
//       should be included before all other header files. It sets some
//       important macros which need to be set before including Windows.h
//       Including this file first also allows it to disable some pointless
//       warning messages (see below).



#ifndef _WINCORE_H_
#define _WINCORE_H_


// Remove pointless warning messages for MS compilers prior to VS 2008 
#if defined (_MSC_VER) && _MSC_VER <= 1500
  #pragma warning (disable : 4511) // copy operator could not be generated
  #pragma warning (disable : 4512) // assignment operator could not be generated
  #pragma warning (disable : 4702) // unreachable code (bugs in Microsoft's STL)
  #pragma warning (disable : 4786) // identifier was truncated
  #pragma warning (disable : 4996) // function or variable may be unsafe (deprecated)
  #ifndef _CRT_SECURE_NO_WARNINGS
    #define _CRT_SECURE_NO_WARNINGS // eliminate deprecation warnings for VS2005
  #endif
#endif // _MSC_VER
#ifdef __BORLANDC__
  #pragma option -w-8027		   // function not expanded inline
#endif

// VS 2008 requires _WIN32_WINNT to be set. (fails on XP and Win2000 without this fix)
#ifndef _WIN32_WINNT
  #define _WIN32_WINNT 0x0400
#endif

#ifdef _WIN32_WCE
  #include "wcestddef.h"
#endif

#ifndef STRICT
  #define STRICT 1
#endif

#define _WINSOCKAPI_            // Prevent winsock.h #include's.

#include <vector>
#include <string>
#include <map>
#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <shlwapi.h>


// For compilers lacking Win64 support
#ifndef  GetWindowLongPtr
  #define GetWindowLongPtr   GetWindowLong
  #define SetWindowLongPtr   SetWindowLong
  #define GWLP_WNDPROC       GWL_WNDPROC
  #define GWLP_HINSTANCE     GWL_HINSTANCE
  #define GWLP_ID            GWL_ID
  #define GWLP_USERDATA      GWL_USERDATA
  #define DWLP_DLGPROC       DWL_DLGPROC
  #define DWLP_MSGRESULT     DWL_MSGRESULT
  #define DWLP_USER          DWL_USER
  #define DWORD_PTR          DWORD
  #define LONG_PTR           LONG
  #define ULONG_PTR          LONG
#endif
#ifndef GetClassLongPtr
  #define GetClassLongPtr    GetClassLong
  #define SetClassLongPtr    SetClassLong
  #define GCLP_HBRBACKGROUND GCL_HBRBACKGROUND
  #define GCLP_HCURSOR       GCL_HCURSOR
  #define GCLP_HICON         GCL_HICON
  #define GCLP_HICONSM       GCL_HICONSM
  #define GCLP_HMODULE       GCL_HMODULE
  #define GCLP_MENUNAME      GCL_MENUNAME
  #define GCLP_WNDPROC       GCL_WNDPROC
#endif

// For Visual Studio 6 (without an updated platform SDK) and Dev-C++
#ifndef OPENFILENAME_SIZE_VERSION_400
  #define OPENFILENAME_SIZE_VERSION_400 sizeof(OPENFILENAME)
#endif


// Messages defined by Win32++
#define UWM_POPUPMENU		(WM_APP + 1)	// Message - creates the menubar popup menu
#define UWM_DOCK_START		(WM_APP + 2)	// Notification - about to start undocking
#define UWM_DOCK_MOVE		(WM_APP + 3)	// Notification - undocked docker is being moved
#define UWM_DOCK_END		(WM_APP + 4)	// Notification - docker has been docked
#define UWM_BAR_START		(WM_APP + 5)	// Notification - docker bar selected for move
#define UWM_BAR_MOVE		(WM_APP + 6)	// Notification - docker bar moved
#define UWM_BAR_END			(WM_APP + 7)	// Notification - end of docker bar move
#define UWM_UNDOCKED		(WM_APP + 8)	// Notification - sent by docker when undocked
#define UWM_FRAMELOSTFOCUS	(WM_APP + 9)    // Notification - sent by frame to view window when focus lost
#define UWM_FRAMEGOTFOCUS	(WM_APP + 10)   // Notification - sent by frame to view window when focus acquired
#define UWM_DOCK_DESTROYED	(WM_APP + 11)	// Message - posted when docker is destroyed
#define UWM_TAB_CHANGED     (WM_APP + 12)	// Notification - tab layout changed
#define UWM_TOOLBAR_RESIZE  (WM_APP + 13)   // Message - sent by toolbar to parent. Used by the rebar


// Automatically include the Win32xx namespace
// define NO_USING_NAMESPACE to skip this step
namespace Win32xx {}
#ifndef NO_USING_NAMESPACE
  using namespace Win32xx;
#endif

// define useful macros from WindowsX.h
#define GET_X_LPARAM(lp)  ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)  ((int)(short)HIWORD(lp))

// Required for WinCE
#ifndef TLS_OUT_OF_INDEXES
  #define TLS_OUT_OF_INDEXES ((DWORD_PTR) -1)
#endif
#ifndef WM_PARENTNOTIFY
  #define WM_PARENTNOTIFY 0x0210
#endif


// Define our own MIN and MAX macros 
// this avoids inconcistancies with Dev-C++ and other compilers, and
// avoids conflicts between typical min/max macros and std::min/std::max
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))


namespace Win32xx
{
	// tString is a TCHAR std::string
	typedef std::basic_string<TCHAR> tString;

	
	////////////////////////////////////////////////
	// Forward declarations.
	//  These classes are defined later or elsewhere
	class CWinApp;
	class CWnd;


	//////////////////////////////////////////////////
	// Global functions	(within the Win32xx namespace)
	
	CWinApp* GetApp();
	void DebugWarnMsg(LPCTSTR WarnMsg);
	void DebugErrMsg(LPCTSTR ErrorMsg);
	void TRACE(LPCTSTR str);

  #ifndef _WIN32_WCE		// for Win32/64 operating systems
	int GetWinVersion();
	int GetComCtlVersion();
	BOOL IsXPThemed();
	BOOL IsLeftButtonDown();
  #endif // #ifndef _WIN32_WCE

  // Required for WinCE  
  #ifndef lstrcpyn	
	LPTSTR lstrcpyn(LPTSTR lpstrDest, LPCTSTR lpstrSrc, int nLength);
  #endif // !lstrcpyn

	tString CharToTString(const char* s);
	std::string TCharToString(LPCTSTR t);



	enum Constants
	{
		MAX_MENU_STRING = 80,
		MAX_STRING_SIZE = 255,
	};

	// The comparison function object used by CWinApp::m_mapHWND
	struct CompareHWND
	{
		bool operator()(HWND const a, const HWND b) const
			{return ((DWORD_PTR)a < (DWORD_PTR)b);}
	};

	// Used for Thread Local Storage (TLS)
	struct TLSData
	{
		CWnd* pCWnd;		// pointer to CWnd object for Window creation
		CWnd* pMenubar;		// pointer to CMenubar object used for the WH_MSGFILTER hook
		HHOOK hMenuHook;	// WH_MSGFILTER hook for CMenubar (used when popup menu is active)
	};


	/////////////////////////////////////////
	// Definition of the CSize class
	// This class can be used to replace the SIZE structure
	class CSize : public SIZE
	{
	public:
		CSize()						{ cx = 0; cy = 0; }
		CSize(int CX, int CY)		{ cx = CX; cy = CY; }
		CSize(SIZE sz)				{ cx = sz.cx; cy = sz.cy; }
		CSize(POINT pt)				{ cx = pt.x;  cx = pt.y; }
		CSize(DWORD dw)				{ cx = (short)LOWORD(dw); cx = (short)HIWORD(dw); }
		void SetSize(int CX, int CY){ cx = CX; cy = CY; }
		BOOL operator == (SIZE sz)	{ return (cx == sz.cx && cy == sz.cy); }
		BOOL operator != (SIZE sz)	{ return (cx != sz.cx || cy != sz.cy); }
		operator LPSIZE()			{ return this; }
	};

	/////////////////////////////////////////
	// Definition of the CPoint class
	// This class can be used to replace the POINT structure
	class CPoint : public POINT
	{
	public:
		CPoint()					{ x = 0; y = 0; }
		CPoint(int X, int Y)		{ x = X; y = Y; }
		CPoint(SIZE sz)				{ x = sz.cx; y = sz.cy; }
		CPoint(POINT pt)			{ x = pt.x ; y = pt.y; }
		CPoint(DWORD dw)			{ x = (short)LOWORD(dw); y = (short)HIWORD(dw); }
		void Offset(int dx, int dy)	{ x += dx; y += dy; }
		void SetPoint(int X, int Y)	{ x = X; y = Y; }
		BOOL operator == (POINT pt)	{ return ((x == pt.x) && (y == pt.y)); }
		BOOL operator != (POINT pt)	{ return ((x != pt.x) || (y != pt.y)); }
		operator LPPOINT()			{ return this; }
	};

	CPoint GetCursorPos();


	/////////////////////////////////////////
	// Definition of the CRect class
	// This class can be used to replace the RECT structure.
	class CRect : public RECT
	{
	public:
		CRect()
		{ left = top = right = bottom = 0; }

		CRect(int Left, int Top, int Right, int Bottom)
		{ left = Left; top = Top; right = Right; bottom = Bottom; }

		CRect(RECT rc)
		{ left = rc.left; top = rc.top; right = rc.right; bottom = rc.bottom; }

		CRect(POINT pt, SIZE sz)
		{ right = (left = pt.x) + sz.cx; bottom = (top = pt.y) + sz.cy; }

		CRect(POINT topLeft, POINT bottomRight)
		{ left = topLeft.x; top = topLeft.y; right = bottomRight.x; bottom = bottomRight.y; }

		operator LPRECT()
		{ return this; }

		operator LPCRECT() const
		{ return this; }

		BOOL operator == (RECT& rc)
		{ return ::EqualRect(this, &rc); }

		BOOL operator != (RECT& rc)
		{ return !::EqualRect(this, &rc); }

		void  operator=( RECT& srcRect)
		{ ::CopyRect(this, &srcRect); }

		int Height()
		{ return bottom - top; }

		int Width()
		{ return right - left; }

		void CopyRect(RECT* pRect)
		{ ::CopyRect(pRect, this); }

		BOOL EqualRect(const RECT* pRect)
		{ return ::EqualRect(pRect, this); }

		BOOL InflateRect(int dx, int dy)
		{ return ::InflateRect(this, dx, dy); }

		BOOL IntersectRect(LPCRECT lpRect1, LPCRECT lpRect2)
		{ return ::IntersectRect(this, lpRect1, lpRect2); }

		BOOL IsRectEmpty()
		{ return ::IsRectEmpty(this);}

		BOOL OffsetRect(int dx, int dy)
		{ return ::OffsetRect(this, dx, dy); }

		BOOL PtInRect(POINT pt)
		{ return ::PtInRect(this, pt); }

		BOOL SetRect(int left, int top, int right, int bottom)
		{ return ::SetRect(this, left, top, right, bottom); }

		BOOL SetRectEmpty()
		{ return ::SetRectEmpty(this); }

		BOOL SubtractRect(const RECT* pRcSrc1, const RECT* pRcSrc2)
		{ return ::SubtractRect(this, pRcSrc1, pRcSrc2); }

		BOOL UnionRect(const RECT* pRcSrc1, const RECT* pRcSrc2)
		{ return ::UnionRect(this, pRcSrc1, pRcSrc2); }
	};


	/////////////////////////////////////////
	// Declarations for the CCriticalSection class
	//

	// This class is used for thread synchronisation
	class CCriticalSection
	{
		public:
		CCriticalSection()	{ ::InitializeCriticalSection(&m_cs); }
		~CCriticalSection()	{ ::DeleteCriticalSection(&m_cs); }

		void Lock() 	{ ::EnterCriticalSection(&m_cs); }
		void Release()	{ ::LeaveCriticalSection(&m_cs); }

		private:
		CRITICAL_SECTION m_cs;
	};


	////////////////////////////////////////
	// Declaration of the CWinException class
	//
	class CWinException
	{
	public:
		CWinException (LPCTSTR msg) : m_err (::GetLastError()), m_msg(msg) {}
		LPCTSTR What() const {return m_msg;}
		void MessageBox() const;

	private:
		DWORD  m_err;
		LPCTSTR m_msg;

	};

	////////////////////////////////
	// Declaration of the CWnd class
	//
	class CWnd
	{
	friend class CMDIChild;

	public:
		CWnd();				// Constructor
		virtual ~CWnd();	// Destructor

		// These are the functions can be overridden
		virtual HWND Create(HWND hWndParent = NULL);
		virtual void Destroy();
		virtual LRESULT DefWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void PreRegisterClass(WNDCLASS& wc);
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		virtual BOOL IsContainer() const { return FALSE; }
		virtual BOOL IsDocker() const  { return FALSE; }
		virtual BOOL IsFrame() const     { return FALSE; }
		virtual BOOL IsMenubar() const   { return FALSE; }
		virtual BOOL IsMDIChild() const  { return FALSE; }
		virtual BOOL IsMDIFrame() const	 { return FALSE; }
		virtual BOOL IsRebar() const     { return FALSE; }
		virtual BOOL IsStatusbar() const { return FALSE; }
		virtual BOOL IsTab() const       { return FALSE; }
		virtual BOOL IsTabbedMDI() const { return FALSE; }
		virtual BOOL IsToolbar() const	 { return FALSE; }

		// These functions aren't intended to be overridden
		BOOL Attach(HWND hWnd);
		BOOL AttachDlgItem(UINT nID, CWnd* pParent);
		BOOL BringWindowToTop() const;
		void CenterWindow() const;
		BOOL CheckDlgButton(int nIDButton, UINT uCheck) const;
		HWND CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hParent, HMENU hMenu, LPVOID lpParam = NULL);
		HWND CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, CRect& rc, HWND hParent, HMENU hMenu, LPVOID lpParam = NULL);
		LRESULT DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		HDWP DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags);
		HDWP DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, RECT rc, UINT uFlags);
		void DestroyWindow();
		HWND Detach();
		BOOL DrawMenuBar() const;
		BOOL EnableWindow(BOOL bEnable = TRUE) const;
		static CWnd* FromHandle(HWND hWnd);
		HWND GetAncestor() const;
		tString GetClassString() const;
		ULONG_PTR GetClassLongPtr(int nIndex) const;
		CRect GetClientRect() const;
		HDC  GetDC() const;
		HDC  GetDCEx(HRGN hrgnClip, DWORD flags) const;
		HWND GetDlgItem(int nIDDlgItem) const;
		tString GetDlgItemString(int nIDDlgItem) const;
		HWND GetHwnd() const {return m_hWnd;}
		HWND GetParent() const;
		BOOL GetScrollInfo(int fnBar, SCROLLINFO& si) const;
		HWND GetWindow(UINT uCmd) const;
		HDC  GetWindowDC() const;
		LONG_PTR GetWindowLongPtr(int nIndex) const;
		CRect GetWindowRect() const;
		tString GetWindowString() const;
		void Invalidate(BOOL bErase = TRUE) const;
		BOOL InvalidateRect(CONST RECT* lpRect, BOOL bErase = TRUE) const;
		BOOL InvalidateRgn(CONST HRGN hRgn, BOOL bErase = TRUE) const;
		BOOL IsChild(const CWnd* pWndParent) const;
		BOOL IsEnabled() const;
		BOOL IsVisible() const;
		BOOL IsWindow() const;
		HBITMAP LoadBitmap(LPCTSTR lpBitmapName) const;
		LPCTSTR LoadString(UINT nID);
		int  MessageBox(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) const;
		LRESULT MessageReflect(HWND hwndParent, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE) const;
		void MoveWindow(CRect& rc, BOOL bRepaint = TRUE) const;
		BOOL PostMessage(UINT uMsg, WPARAM wParam = 0L, LPARAM lParam = 0L) const;
		BOOL RedrawWindow(CRect* lpRectUpdate = NULL, HRGN hRgn = NULL, UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE ) const;
		BOOL RegisterClass(WNDCLASS& wc);
		int  ReleaseDC(HDC hDC) const;
		LRESULT SendDlgItemMessage(int nIDDlgItem, UINT Msg, WPARAM wParam, LPARAM lParam) const;
		LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0L, LPARAM lParam = 0L) const;
		HWND SetActiveWindow() const;
		HWND SetCapture() const;
		ULONG_PTR SetClassLongPtr(int nIndex, LONG_PTR dwNewLong) const;
		BOOL SetDlgItemText(int nIDDlgItem, LPCTSTR lpString) const;
		HWND SetFocus() const;
		BOOL SetForegroundWindow() const;
		HWND SetParent(HWND hParent) const;
		BOOL SetRedraw(BOOL bRedraw = TRUE) const;
		int  SetScrollInfo(int fnBar, SCROLLINFO& si, BOOL fRedraw) const;
		LONG_PTR SetWindowLongPtr(int nIndex, LONG_PTR dwNewLong) const;
		BOOL SetWindowPos(HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) const;
		BOOL SetWindowPos(HWND hWndInsertAfter, RECT rc, UINT uFlags) const;
		int SetWindowRgn(HRGN hRgn, BOOL bRedraw = TRUE) const;
		BOOL SetWindowText(LPCTSTR lpString) const;
		BOOL ShowWindow(int nCmdShow = SW_SHOWNORMAL) const;
		BOOL UpdateWindow() const;
		BOOL ValidateRect(CRect& rc) const;
		BOOL ValidateRgn(HRGN hRgn) const;

#ifndef _WIN32_WCE
		BOOL CloseWindow() const;
		HMENU GetMenu() const;
		int  GetScrollPos(int nBar) const;
		BOOL GetScrollRange(int nBar, int& MinPos, int& MaxPos) const;
		BOOL GetWindowPlacement(WINDOWPLACEMENT& pWndpl) const;
		BOOL IsIconic() const;
		BOOL IsZoomed() const;
		BOOL LockWindowUpdate(HWND hWndLock) const;
		BOOL SetMenu(HMENU hMenu) const;
		BOOL ScrollWindow(int XAmount, int YAmount, RECT& Rect, RECT& ClipRect) const;
		int  SetScrollPos(int nBar, int nPos, BOOL bRedraw) const;
		BOOL SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw) const;
		BOOL SetWindowPlacement(const WINDOWPLACEMENT& wndpl) const;
#endif

		static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		operator HWND() const {return m_hWnd;}

		// Required by some macros
		BOOL PostMessage(HWND hWnd, UINT uMsg, WPARAM wParam = 0L, LPARAM lParam = 0L) const
			{return ::PostMessage(hWnd, uMsg, wParam, lParam);}
		LRESULT SendMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
			{ return ::SendMessage(hWnd, uMsg, wParam, lParam);}

	protected:
		// These can be overridden
		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
		virtual void OnCreate();
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual void OnInitialUpdate();
		virtual void OnPaint(HDC hDC);
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// These functions aren't intended to be overridden
		void AddToMap();
		LRESULT CallPrevWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		HICON SetIconLarge(int nIcon);
		HICON SetIconSmall(int nIcon);
		
		HWND m_hWnd;				// handle to this object's window

	private:
		CWnd(const CWnd&);				// Disable copy construction
		CWnd& operator = (const CWnd&); // Disable assignment operator
		BOOL RemoveFromMap();
		void Subclass();

		WNDCLASS m_wc;				// defines initialisation parameters for RegisterClass
		CREATESTRUCT m_cs;			// defines initialisation parameters for PreCreate and Create
		HICON m_hIconLarge;			// handle to the window's large icon
		HICON m_hIconSmall;			// handle to the window's small icon
		WNDPROC m_PrevWindowProc;	// pre-subclassed Window Procedure
		tString m_tsLoadString;		// a TCHAR std::string, temporary storage for strings

	}; // class CWnd


	///////////////////////////////
	// Declaration of the CWinApp class
	//
	class CWinApp
	{
		friend class CWnd;			// CWnd needs access to CWinApp's private members
		friend CWinApp* GetApp();	// GetApp needs access to SetnGetThis

	public:
		CWinApp();
		virtual ~CWinApp();

		// These are the functions you might wish to override
		virtual BOOL InitInstance();
		virtual int  MessageLoop();

		// These functions aren't intended to be overridden
		DWORD GetTlsIndex() const {return m_dwTlsIndex;}
		CWnd* GetCWndFromMap(HWND hWnd);
		HINSTANCE GetInstanceHandle() const {return m_hInstance;}
		HINSTANCE GetResourceHandle() const {return (m_hResource ? m_hResource : m_hInstance);}
		int Run();
		void SetResourceHandle(HINSTANCE hResource) {m_hResource = hResource;}
		TLSData* SetTlsIndex();

	private:
		CWinApp(const CWinApp&);				// Disable copy construction
		CWinApp& operator = (const CWinApp&);	// Disable assignment operator
		void DefaultClass();
		static CWinApp* SetnGetThis(CWinApp* pThis = 0);

		CCriticalSection m_csMapLock;	// thread synchronisation for m_mapHWND
		HINSTANCE m_hInstance;			// handle to the applications instance
		HINSTANCE m_hResource;			// handle to the applications resources
		std::map<HWND, CWnd*, CompareHWND> m_mapHWND;	// maps window handles to CWnd objects
		std::vector<TLSData*> m_vTLSData;	// vector of TLSData pointers, one for each thread
		DWORD m_dwTlsIndex;				// Thread Local Storage index
		WNDPROC m_Callback;				// callback address of CWnd::StaticWndowProc

	};
}	


#endif // _WINCORE_H_

