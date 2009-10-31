// Win32++  Version 6.7
// Released: 6th November, 2009 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
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


// Remove pointless warning messages
#ifdef _MSC_VER
  #pragma warning (disable : 4996) // function or variable may be unsafe (deprecated)
  #ifndef _CRT_SECURE_NO_WARNINGS
    #define _CRT_SECURE_NO_WARNINGS // eliminate deprecation warnings for VS2005/VS2010
  #endif
  #if _MSC_VER < 1500
    #pragma warning (disable : 4511) // copy operator could not be generated
    #pragma warning (disable : 4512) // assignment operator could not be generated
    #pragma warning (disable : 4702) // unreachable code (bugs in Microsoft's STL)
    #pragma warning (disable : 4786) // identifier was truncated
  #endif
#endif

#ifdef __BORLANDC__
  #pragma option -w-8027		   // function not expanded inline
  #define STRICT 1
#endif

#ifdef _WIN32_WCE
  #include "wcestddef.h"
#endif

#define _WINSOCKAPI_            // Prevent winsock.h #include's.

#include <vector>
#include <string>
#include <map>
#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <shlwapi.h>
#include <assert.h>


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
#define UWM_UPDATE_COMMAND  (WM_APP + 14)   // Message - sent before a menu is displayed. Used by OnUpdate
#define UWM_DOCK_ACTIVATED  (WM_APP + 15)   // Message - sent to dock ancestor when a docker is activated or deactivated.


// Automatically include the Win32xx namespace
// define NO_USING_NAMESPACE to skip this step
namespace Win32xx {}
#ifndef NO_USING_NAMESPACE
  using namespace Win32xx;
#endif

// define useful macros from WindowsX.h
#ifndef GET_X_LPARAM
  #define GET_X_LPARAM(lp)  ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
  #define GET_Y_LPARAM(lp)  ((int)(short)HIWORD(lp))
#endif

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
	int  GetWinVersion();
	int  GetComCtlVersion();
	UINT GetSizeofNonClientMetrics();
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

		BOOL operator == (const RECT& rc)
		{ return ::EqualRect(this, &rc); }

		BOOL operator != (const RECT& rc)
		{ return !::EqualRect(this, &rc); }

		void  operator=(const RECT& srcRect)
		{ ::CopyRect(this, &srcRect); }

		int Height()
		{ return bottom - top; }

		int Width()
		{ return right - left; }

		void CopyRect(const RECT& rc)
		{ ::CopyRect(this, &rc); }

		BOOL EqualRect(const RECT& rc)
		{ return ::EqualRect(&rc, this); }

		BOOL InflateRect(int dx, int dy)
		{ return ::InflateRect(this, dx, dy); }

		BOOL IntersectRect(const RECT& rc1, const RECT& rc2)
		{ return ::IntersectRect(this, &rc1, &rc2); }

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

		BOOL SubtractRect(const RECT& rc1, const RECT& rc2)
		{ return ::SubtractRect(this, &rc1, &rc2); }

		BOOL UnionRect(const RECT& rc1, const RECT& rc2)
		{ return ::UnionRect(this, &rc1, &rc2); }
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
	friend class CDialog;
	friend class CPropertyPage;
	friend class CSplitter;

	public:
		CWnd();				// Constructor
		virtual ~CWnd();	// Destructor

		// These are the functions can be overridden
		virtual BOOL Attach(HWND hWnd);
		virtual BOOL AttachDlgItem(UINT nID, CWnd* pParent);
		virtual void CenterWindow() const;
		virtual HWND Create(HWND hWndParent = NULL);
		virtual HWND CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hParent, HMENU hMenu, LPVOID lpParam = NULL);
		virtual HWND CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rc, HWND hParent, HMENU hMenu, LPVOID lpParam = NULL);
		virtual void Destroy();
		virtual HWND Detach();
		virtual HWND GetAncestor() const;
		virtual tString GetClassString() const;
		virtual tString GetDlgItemString(int nIDDlgItem) const;
		virtual tString GetWindowString() const;
		HBITMAP LoadBitmap(LPCTSTR lpBitmapName) const;
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void PreRegisterClass(WNDCLASS& wc);
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		virtual BOOL IsContainer() const { return FALSE; }
		virtual BOOL IsDocker() const	 { return FALSE; }
		virtual BOOL IsFrame() const     { return FALSE; }
		virtual BOOL IsMenubar() const   { return FALSE; }
		virtual BOOL IsMDIChild() const  { return FALSE; }
		virtual BOOL IsMDIFrame() const	 { return FALSE; }
		virtual BOOL IsRebar() const     { return FALSE; }
		virtual BOOL IsStatusbar() const { return FALSE; }
		virtual BOOL IsTab() const       { return FALSE; }
		virtual BOOL IsTabbedMDI() const { return FALSE; }
		virtual BOOL IsToolbar() const	 { return FALSE; }
		virtual LPCTSTR LoadString(UINT nID);
		virtual HICON SetIconLarge(int nIcon);
		virtual HICON SetIconSmall(int nIcon);

		HWND GetHwnd() const				{ return m_hWnd; }
		WNDPROC GetPrevWindowProc() const	{ return m_PrevWindowProc; }

		// Wrappers for Win32 API functions
		// These functions aren't virtual, and shouldn't be overridden
		BOOL BringWindowToTop() const;
		LRESULT CallWindowProc(WNDPROC lpPrevWndFunc, UINT Msg, WPARAM wParam, LPARAM lParam) const;
		BOOL CheckDlgButton(int nIDButton, UINT uCheck) const;
		LRESULT DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam) const;
		HDWP DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) const;
		HDWP DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, const RECT& rc, UINT uFlags) const;
		BOOL DrawMenuBar() const;
		BOOL EnableWindow(BOOL bEnable = TRUE) const;
		static CWnd* FromHandle(HWND hWnd);
		ULONG_PTR GetClassLongPtr(int nIndex) const;
		CRect GetClientRect() const;
		HDC  GetDC() const;
		HDC  GetDCEx(HRGN hrgnClip, DWORD flags) const;
		HWND GetDlgItem(int nIDDlgItem) const;
		int  GetDlgItemInt(int nIDDlgItem, BOOL* lpTranslated, BOOL bSigned) const;
		HWND GetParent() const;
		BOOL GetScrollInfo(int fnBar, SCROLLINFO& si) const;
		HWND GetWindow(UINT uCmd) const;
		HDC  GetWindowDC() const;
		LONG_PTR GetWindowLongPtr(int nIndex) const;
		CRect GetWindowRect() const;
		void Invalidate(BOOL bErase = TRUE) const;
		BOOL InvalidateRect(LPCRECT lpRect, BOOL bErase = TRUE) const;
		BOOL InvalidateRgn(CONST HRGN hRgn, BOOL bErase = TRUE) const;
		BOOL IsChild(HWND hWndParent) const;
		BOOL IsWindow() const;
		BOOL IsWindowEnabled() const;
		BOOL IsWindowVisible() const;
		int  MessageBox(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) const;
		void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE) const;
		void MoveWindow(const RECT& rc, BOOL bRepaint = TRUE) const;
		BOOL PostMessage(UINT uMsg, WPARAM wParam = 0L, LPARAM lParam = 0L) const;
		BOOL PostMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;
		BOOL RedrawWindow(LPCRECT lpRectUpdate = NULL, HRGN hRgn = NULL, UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE ) const;
		int  ReleaseDC(HDC hDC) const;
		LRESULT SendDlgItemMessage(int nIDDlgItem, UINT Msg, WPARAM wParam, LPARAM lParam) const;
		LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0L, LPARAM lParam = 0L) const;
		LRESULT SendMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;
		HWND SetActiveWindow() const;
		HWND SetCapture() const;
		ULONG_PTR SetClassLongPtr(int nIndex, LONG_PTR dwNewLong) const;
		BOOL SetDlgItemInt(int nIDDlgItem, UINT uValue, BOOL bSigned) const;
		BOOL SetDlgItemText(int nIDDlgItem, LPCTSTR lpString) const;
		HWND SetFocus() const;
		BOOL SetForegroundWindow() const;
		HWND SetParent(HWND hParent) const;
		BOOL SetRedraw(BOOL bRedraw = TRUE) const;
		int  SetScrollInfo(int fnBar, const SCROLLINFO& si, BOOL fRedraw) const;
		LONG_PTR SetWindowLongPtr(int nIndex, LONG_PTR dwNewLong) const;
		BOOL SetWindowPos(HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) const;
		BOOL SetWindowPos(HWND hWndInsertAfter, const RECT& rc, UINT uFlags) const;
		int SetWindowRgn(HRGN hRgn, BOOL bRedraw = TRUE) const;
		BOOL SetWindowText(LPCTSTR lpString) const;
		BOOL ShowWindow(int nCmdShow = SW_SHOWNORMAL) const;
		BOOL UpdateWindow() const;
		BOOL ValidateRect(LPCRECT prc) const;
		BOOL ValidateRgn(HRGN hRgn) const;

#ifndef _WIN32_WCE
		BOOL CloseWindow() const;
		BOOL EnableScrollBar(UINT uSBflags, UINT uArrows) const;
		HMENU GetMenu() const;
		int  GetScrollPos(int nBar) const;
		BOOL GetScrollRange(int nBar, int& MinPos, int& MaxPos) const;
		BOOL GetWindowPlacement(WINDOWPLACEMENT& pWndpl) const;
		BOOL IsIconic() const;
		BOOL IsZoomed() const;
		BOOL LockWindowUpdate(HWND hWndLock) const;
		BOOL SetMenu(HMENU hMenu) const;
		BOOL ScrollWindow(int XAmount, int YAmount, LPCRECT prcScroll, LPCRECT prcClip) const;
		int  ScrollWindowEx(int dx, int dy, LPCRECT prcScroll, LPCRECT prcClip, HRGN hrgnUpdate, LPRECT prcUpdate, UINT flags) const;
		int  SetScrollPos(int nBar, int nPos, BOOL bRedraw) const;
		BOOL SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw) const;
		BOOL SetWindowPlacement(const WINDOWPLACEMENT& wndpl) const;
		BOOL ShowScrollBar(int nBar, BOOL bShow) const;
#endif

		static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		operator HWND() const {return m_hWnd;}

	protected:
		// Override these functions as required
		virtual LRESULT FinalWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
		virtual void OnCreate();
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual void OnInitialUpdate();
		virtual LRESULT OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void OnPaint(HDC hDC);
		virtual void OnMenuUpdate(UINT nID);
		virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

		HWND m_hWnd;				// handle to this object's window

	private:
		CWnd(const CWnd&);				// Disable copy construction
		CWnd& operator = (const CWnd&); // Disable assignment operator
		void AddToMap();
		LRESULT MessageReflect(HWND hwndParent, UINT uMsg, WPARAM wParam, LPARAM lParam);
		BOOL RegisterClass(WNDCLASS& wc);
		BOOL RemoveFromMap();
		void Subclass(HWND hWnd);

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
		friend class CDialog;
		friend class CPropertyPage;
		friend class CPropertySheet;
		friend CWinApp* GetApp();	// GetApp needs access to SetnGetThis

	public:
		CWinApp();
		virtual ~CWinApp();

		// These are the functions you might wish to override
		virtual BOOL InitInstance();
		virtual int  MessageLoop();
		virtual int Run();

		DWORD GetTlsIndex() const {return m_dwTlsIndex;}
		HINSTANCE GetInstanceHandle() const {return m_hInstance;}
		HINSTANCE GetResourceHandle() const {return (m_hResource ? m_hResource : m_hInstance);}
		void SetResourceHandle(HINSTANCE hResource) {m_hResource = hResource;}

	private:
		CWinApp(const CWinApp&);				// Disable copy construction
		CWinApp& operator = (const CWinApp&);	// Disable assignment operator
		CWnd* GetCWndFromMap(HWND hWnd);
		void DefaultClass();
		static CWinApp* SetnGetThis(CWinApp* pThis = 0);
		TLSData* SetTlsIndex();

		CCriticalSection m_csMapLock;	// thread synchronisation for m_mapHWND
		CCriticalSection m_csTlsData;	// thread synchronisation for m_ csvTlsData
		HINSTANCE m_hInstance;			// handle to the applications instance
		HINSTANCE m_hResource;			// handle to the applications resources
		std::map<HWND, CWnd*, CompareHWND> m_mapHWND;	// maps window handles to CWnd objects
		std::vector<TLSData*> m_vTLSData;	// vector of TLSData pointers, one for each thread
		DWORD m_dwTlsIndex;				// Thread Local Storage index
		WNDPROC m_Callback;				// callback address of CWnd::StaticWndowProc

	};
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace Win32xx
{

	//////////////////////////////////////////////////
	// Global functions	(within the Win32xx namespace)

	// Returns a pointer to the CWinApp derrived class
	inline CWinApp* GetApp()
	{
		return CWinApp::SetnGetThis();
	}

	// Displays an error message in a message box. Debug mode only.
	inline void DebugWarnMsg(LPCTSTR WarnMsg)
	{
	#ifdef _DEBUG
		TRACE(_T("*** Warning:  "));
		TRACE(WarnMsg);
		TRACE(_T("\n"));
		::MessageBox (0, WarnMsg, _T("Warning"), MB_ICONINFORMATION | MB_OK);
	#else
		UNREFERENCED_PARAMETER(WarnMsg); // no-op
	#endif  //_DEBUG
	}

	// Displays a warning message in a messagebox. Debug mode only
	inline void DebugErrMsg(LPCTSTR ErrorMsg)
	{
	#ifdef _DEBUG
		TRACE(_T("*** Error:  "));
		TRACE(ErrorMsg);
		TRACE(_T("\n"));
		::MessageBox (0, ErrorMsg, _T("Error"), MB_ICONEXCLAMATION | MB_OK);
	#else
		UNREFERENCED_PARAMETER(ErrorMsg); // no-op
	#endif  //_DEBUG
	}

	// TRACE sends a string to the debug/output pane, or an external debugger
	inline void TRACE(LPCTSTR str)
	{
	#ifdef _DEBUG
		OutputDebugString(str);
	#else
		UNREFERENCED_PARAMETER(str); // no-op
	#endif
	}

  #ifndef _WIN32_WCE		// for Win32/64 operating systems

	inline int GetWinVersion()
	{
		DWORD dwVersion = GetVersion();
		int Platform = (dwVersion < 0x80000000)? 2:1;
		int MajorVer = LOBYTE(LOWORD(dwVersion));
		int MinorVer = HIBYTE(LOWORD(dwVersion));

		int nVersion =  1000*Platform + 100*MajorVer + MinorVer;

		// Return values and window versions:
		//  1400     Windows 95
		//  1410     Windows 98
		//  1490     Windows ME
		//  2400     Windows NT
		//  2500     Windows 2000
		//  2501     Windows XP
		//  2502     Windows Server 2003
		//  2600     Windows Vista and Windows Server 2008

		return nVersion;
	}

	inline int GetComCtlVersion()
	{
		// Load the Common Controls DLL
		HMODULE hComCtl = ::LoadLibraryA("COMCTL32.DLL");
		if (!hComCtl)
			return 0;

		int ComCtlVer = 400;

		if (::GetProcAddress(hComCtl, "InitCommonControlsEx"))
		{
			// InitCommonControlsEx is unique to 4.7 and later
			ComCtlVer = 470;

			if (::GetProcAddress(hComCtl, "DllInstall"))
			{
				// DllInstall is unique to 4.71 and later
				ComCtlVer = 471;

				typedef HRESULT CALLBACK DLLGETVERSION(DLLVERSIONINFO*);
				DLLGETVERSION* pfnDLLGetVersion = NULL;

				pfnDLLGetVersion = (DLLGETVERSION*)::GetProcAddress(hComCtl, "DllGetVersion");
				if(pfnDLLGetVersion)
				{
					DLLVERSIONINFO dvi;
					dvi.cbSize = sizeof dvi;
					if(NOERROR == pfnDLLGetVersion(&dvi))
					{
						DWORD dwVerMajor = dvi.dwMajorVersion;
						DWORD dwVerMinor = dvi.dwMinorVersion;
						ComCtlVer = 100 * dwVerMajor + dwVerMinor;
					}
				}
			}
		}

		::FreeLibrary(hComCtl);

		// return values and DLL versions
		// 400  dll ver 4.00	Windows 95/Windows NT 4.0
		// 470  dll ver 4.70	Internet Explorer 3.x
		// 471  dll ver 4.71	Internet Explorer 4.0
		// 472  dll ver 4.72	Internet Explorer 4.01 and Windows 98
		// 580  dll ver 5.80	Internet Explorer 5
		// 581  dll ver 5.81	Windows 2000 and Windows ME
		// 582  dll ver 5.82	Windows XP or Vista without XP themes
		// 600  dll ver 6.00	Windows XP with XP themes
		// 610  dll ver 6.10	Windows Vista with XP themes

		return ComCtlVer;
	}

	inline UINT GetSizeofNonClientMetrics()
	{
		// This function correctly determines the sizeof NONCLIENTMETRICS
		UINT uSize = sizeof (NONCLIENTMETRICS);

	#if (WINVER >= 0x0600)
		if (GetWinVersion() < 2600)		// Is OS version less than Vista
			uSize -= sizeof(int);		// Adjust size back to correct value
	#endif

		return uSize;
	}

	// A global function to report the state of the left mouse button
	inline BOOL IsLeftButtonDown()
	{
		SHORT state;
		if (GetSystemMetrics(SM_SWAPBUTTON))
			// Mouse buttons are swapped
			state = GetAsyncKeyState(VK_RBUTTON);
		else
			// Mouse buttons are not swapped
			state = GetAsyncKeyState(VK_LBUTTON);

		// returns true if the left mouse button is down
		return (state & 0x8000);
	}

	inline BOOL IsXPThemed()
	{
		BOOL bIsXPThemed = FALSE;

		// Test if Windows version is XP or greater
		if (GetWinVersion() >= 2501)
		{
			HMODULE hMod = ::LoadLibrary(_T("uxtheme.dll"));
			if(hMod)
			{
				// Declare pointers to functions
				FARPROC pIsAppThemed   = ::GetProcAddress(hMod, "IsAppThemed");
				FARPROC pIsThemeActive = ::GetProcAddress(hMod, "IsThemeActive");

				if(pIsAppThemed && pIsThemeActive)
				{
					if(pIsAppThemed() && pIsThemeActive())
					{
						// Test if ComCtl32 dll used is version 6 or later
						bIsXPThemed = (GetComCtlVersion() >= 600);
					}
				}
				::FreeLibrary(hMod);
			}
		}

		return bIsXPThemed;
	}

  #endif // #ifndef _WIN32_WCE

  // Required for WinCE
  #ifndef lstrcpyn
	inline LPTSTR lstrcpyn(LPTSTR lpstrDest, LPCTSTR lpstrSrc, int nLength)
	{
		if(NULL == lpstrDest || NULL == lpstrSrc || nLength <= 0)
			return NULL;
		int nLen = MIN((int)lstrlen(lpstrSrc), nLength - 1);
		LPTSTR lpstrRet = (LPTSTR)memcpy(lpstrDest, lpstrSrc, nLen * sizeof(TCHAR));
		lpstrDest[nLen] = _T('\0');
		return lpstrRet;
	}
  #endif // !lstrcpyn

	inline tString CharToTString(const char* s)
	{
		// Handy for converting char to TCHAR
		tString tstr;
  #ifdef UNICODE
		size_t len = strlen(s);
		if (0 == len) return tstr;

		TCHAR* t = new TCHAR[len +1];
		if (NULL == t) throw std::bad_alloc();

		mbstowcs(t, s, len);
		tstr = t;
		delete []t;
  #else
		tstr = s;
  #endif
		return tstr;
	}

	inline std::string TCharToString(LPCTSTR t)
	{
		// Handy for converting TCHAR to char
		// If the conversion fails, an empty string is returned.
		std::string str;
  #ifdef UNICODE
		// calculate the size of the char string required
		// Note: If wcstombs encounters a wide character it cannot convert
		//       to a multibyte character, it returns –1.
		size_t len = wcstombs(0, t, 0);
		if (len <= 0) return str;

		char* c = new char[len + 1];
		if (NULL == c) throw std::bad_alloc();
		c[0] = '\0';

		wcstombs(c, t, len);
		c[len] = '\0';
		str = c;
		delete []c;
  #else
		str = t;
  #endif
		return str;
	}

	inline CPoint GetCursorPos()
	{
		CPoint pt;
		::GetCursorPos(&pt);
		return pt;
	}



	///////////////////////////////////
	// Definitions for the CWinException class
	//
	inline void CWinException::MessageBox() const
	{
		TCHAR buf1 [MAX_STRING_SIZE/2 -10] = _T("");
		TCHAR buf2 [MAX_STRING_SIZE/2 -10] = _T("");
		TCHAR buf3 [MAX_STRING_SIZE]       = _T("");

		lstrcpyn(buf1, m_msg, MAX_STRING_SIZE/2 -10);

		// Display Last Error information if it's useful
		if (m_err != 0)
		{
			DWORD dwFlags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
			::FormatMessage(dwFlags, NULL, m_err,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf2, MAX_STRING_SIZE/2 -10, NULL);

			::wsprintf(buf3, _T("%s\n\n     %s\n\n"), buf1, buf2);
		}
		else
			::wsprintf(buf3, _T("%s"), buf1);

		TRACE(_T("*** ERROR: An Exception occured ***\n"));
		TRACE(buf3);
		TRACE(_T("\n\n"));

		DebugErrMsg(buf3);
	}


	///////////////////////////////////
	// Definitions for the CWinApp class
	//

	// To begin Win32++, inherit your application class from this one.
	// You should run only one instance of the class inherited from this.
	inline CWinApp::CWinApp() : m_Callback(NULL)
	{
		try
		{
			// Test if this is the first instance of CWinApp
			if (0 == SetnGetThis() )
			{
				m_dwTlsIndex = ::TlsAlloc();
				if (m_dwTlsIndex == TLS_OUT_OF_INDEXES)
				{
					// We only get here in the unlikely event that all TLS indexes are already allocated by this app
					// At least 64 TLS indexes per process are allowed. Win32++ requires only one TLS index.
					throw CWinException(_T("CWinApp::CWinApp  Failed to allocate TLS Index"));
				}
				SetnGetThis(this);
			}
			else
			{
				// We get here if Win32++ is used incorrectly, i.e. more than one instance
				// of a CWinApp derived class is started.
 				throw CWinException(_T("Error!  An instance of CWinApp (or a class derived from CWinApp) is already running"));
			}

		    // Get store the instance handle
			m_hInstance = GetModuleHandle(0);

			m_hResource = m_hInstance;
			DefaultClass();
		}

		catch (const CWinException &e)
		{
			// Indicate the problem
			e.MessageBox();
			throw;
		}
	}

	inline CWinApp::~CWinApp()
	{
		// Check that all CWnd windows are destroyed
		std::map<HWND, CWnd*, CompareHWND>::iterator m;
		for (m = m_mapHWND.begin(); m != m_mapHWND.end(); ++m)
		{
			HWND hWnd = (*m).first;
			if (::IsWindow(hWnd))
				::DestroyWindow(hWnd);
		}
		m_mapHWND.clear();

		// Do remaining tidy up
		if (m_dwTlsIndex != TLS_OUT_OF_INDEXES)
		{
			::TlsSetValue(GetTlsIndex(), NULL);
			::TlsFree(m_dwTlsIndex);
		}

		std::vector<TLSData*>::iterator iter;
		for(iter = m_vTLSData.begin(); iter != m_vTLSData.end(); ++iter)
		{
			delete *(iter);
		}

		SetnGetThis((CWinApp*)-1);
	}

	inline void CWinApp::DefaultClass()
	{
		// Register a default window class so we can get the callback
		// address of CWnd::StaticWindowProc

		WNDCLASS wcDefault = {0};

		LPCTSTR szClassName		= _T("Win32++ Window");
		wcDefault.hInstance		= GetInstanceHandle();
		wcDefault.lpfnWndProc	= CWnd::StaticWindowProc;
		wcDefault.lpszClassName = szClassName;
		wcDefault.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
		wcDefault.hCursor		= ::LoadCursor(NULL, IDC_ARROW);
		wcDefault.style			= CS_DBLCLKS;

		if (!::GetClassInfo(GetInstanceHandle(), szClassName, &wcDefault))
			if (0 == ::RegisterClass(&wcDefault))
				throw CWinException(_T("CWinApp::DefaultClass ... Failed to set Default class"));

		// Retrieve the class information
		ZeroMemory(&wcDefault, sizeof(wcDefault));
		if (!::GetClassInfo(GetInstanceHandle(), szClassName, &wcDefault))
			throw CWinException(_T("CWinApp::DefaultClass ... Failed to get Default class info"));

		// Save the callback address of CWnd::StaticWindowProc
		m_Callback = wcDefault.lpfnWndProc;
	}

	inline CWnd* CWinApp::GetCWndFromMap(HWND hWnd)
	{
		// Allocate an iterator for our HWND map
		std::map<HWND, CWnd*, CompareHWND>::iterator m;

		// Find the CWnd pointer mapped to this HWND
		m_csMapLock.Lock();
		m = m_mapHWND.find(hWnd);
		m_csMapLock.Release();
		if (m != m_mapHWND.end())
			return m->second;
		else
			return 0;
	}

	inline BOOL CWinApp::InitInstance()
	{
		// InitInstance contains the initialization code for your application
		// You should override this function with the code to run when the application starts.

		// return TRUE to indicate success. FALSE will end the application
		return TRUE;
	}

	inline int CWinApp::MessageLoop()
	{
		// This gets any messages queued for the application, and dispatches them.
		MSG Msg;
		int status;

		while((status = ::GetMessage(&Msg, NULL, 0, 0))!= 0)
		{
			if (-1 == status) return -1;

			BOOL Processed = FALSE;

			// only pre-translate input events
			if ((Msg.message >= WM_KEYFIRST && Msg.message <= WM_KEYLAST) ||
				(Msg.message >= WM_MOUSEFIRST && Msg.message <= WM_MOUSELAST))
			{
				// search through the chain of parents for first valid CWnd.
				// Some pretranslatable messages come from non-CWnd windows,
				// such as the tab control within propertysheets.
				for (HWND hWnd = Msg.hwnd; hWnd != NULL; hWnd = ::GetParent(hWnd))
				{
					CWnd* pWnd = GetCWndFromMap(hWnd);
					if (pWnd)
					{
						Processed = pWnd->PreTranslateMessage(&Msg);
						break;
					}
				}
			}

			if (!Processed)
			{
				::TranslateMessage(&Msg);
				::DispatchMessage(&Msg);
			}
		}
		return LOWORD(Msg.wParam);
	}

	inline int CWinApp::Run()
	{
		try
		{
			// InitInstance runs the App's initialization code
			if (InitInstance())
			{
				// Dispatch the window messages
				return MessageLoop();
			}
			else
			{
				::PostQuitMessage(-1);
				return -1;
			}
		}

		catch (const std::bad_alloc &)
		{
			::MessageBox(NULL, _T("Memory allocation failure"), _T("Bad Alloc Exception"), MB_OK);
			throw;	// Rethrow bad alloc exception
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
			throw;	// Rethrow exception
		}

		catch (...)
		{
			::MessageBox(NULL, _T("Unknown Exception"), _T("Error"), MB_OK);
			throw;	// Rethrow unknown exception
		}
	}

	inline CWinApp* CWinApp::SetnGetThis(CWinApp* pThis /*= 0*/)
	{
		// This function stores the 'this' pointer in a static variable.
		// Once stored, it can be used later to return the 'this' pointer.
		// CWinApp's Destructor calls this function with a value of -1.
		static CWinApp* pWinApp = 0;

		if ((CWinApp*)-1 == pThis)
			pWinApp = 0;
		else if (0 == pWinApp)
			pWinApp = pThis;

		return pWinApp;
	}

	inline TLSData* CWinApp::SetTlsIndex()
	{
		try
		{
			TLSData* pTLSData = (TLSData*)::TlsGetValue(GetTlsIndex());
			if (NULL == pTLSData)
			{
				pTLSData = new TLSData;
				// Some MS compilers (including VS2003 under some circumstances) return NULL instead of throwing
				//  an exception when new fails. We make sure an exception gets thrown!
				if (NULL == pTLSData)
					throw std::bad_alloc();

				ZeroMemory(pTLSData, sizeof(TLSData));
				::TlsSetValue(GetTlsIndex(), pTLSData);

				// Store pointer in vector for deletion in destructor
				m_csTlsData.Lock();
				m_vTLSData.push_back(pTLSData);
				m_csTlsData.Release();
			}
			return pTLSData;
		}

		catch (const std::bad_alloc &)
		{
			DebugErrMsg(_T("Failed to allocate mememory in CWinApp::SetTlsIndex"));
			throw; // Critical problem, so rethrow
		}
	}


	////////////////////////////////////////
	// Definitions for the CWnd class
	//
	inline CWnd::CWnd() : m_hWnd(NULL), m_hIconLarge(NULL),
					m_hIconSmall(NULL), m_PrevWindowProc(NULL)
	{
		// Note: m_hWnd is set in CWnd::CreateEx(...)
		::ZeroMemory(&m_cs, sizeof(CREATESTRUCT));
		::ZeroMemory(&m_wc, sizeof(WNDCLASS));
	}

	inline CWnd::~CWnd()
	{
		// Destroy the window for this object
		Destroy();
	}

	inline void CWnd::AddToMap()
	{
		// Store the window handle and CWnd pointer in the HWND map
		GetApp()->m_csMapLock.Lock();
		if (m_hWnd == 0)
			throw CWinException(_T("CWnd::AddToMap  can't add a NULL HWND"));
		if (GetApp()->GetCWndFromMap(m_hWnd))
			throw CWinException(_T("CWnd::AddToMap  HWND already in map"));

		GetApp()->m_mapHWND.insert(std::make_pair(m_hWnd, this));
		GetApp()->m_csMapLock.Release();
	}

	inline BOOL CWnd::Attach(HWND hWnd)
	// Subclasses an existing window and attaches it to a CWnd
	{
		try
		{
			if (!::IsWindow(hWnd))
				throw CWinException(_T("Attach failed, not a valid hwnd"));

			if (0 != GetApp()->GetCWndFromMap(hWnd))
				throw CWinException(_T("Window already attached to this CWnd object"));

			Subclass(hWnd);

			// Store the CWnd pointer in the HWND map
			AddToMap();
			OnInitialUpdate();
			return TRUE;
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
			throw;
		}
	}

	inline BOOL CWnd::AttachDlgItem(UINT nID, CWnd* pParent)
	// Use to convert a dialog item to a CWnd object
	{
		HWND hWnd = ::GetDlgItem(pParent->GetHwnd(), nID);

		return Attach(hWnd);
	}

	inline void CWnd::CenterWindow() const
	{
		// Centers this window over it's parent

		CRect rc = GetWindowRect();
		CRect rcParent;
		CRect rcDesktop;

		// Get screen dimensions excluding task bar
		::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcDesktop, 0);

		// Get the parent window dimensions (parent could be the desktop)
		if (GetParent() != NULL) ::GetWindowRect(GetParent(), &rcParent);
		else rcParent = rcDesktop;

  #ifndef _WIN32_WCE
	// required for Dev-C++ and VC6
    #ifndef MONITOR_DEFAULTTONEAREST
		#define MONITOR_DEFAULTTONEAREST    0x00000002
    #endif
    #ifndef HMONITOR
		DECLARE_HANDLE(HMONITOR);
    #endif
    #ifndef MONITORINFO
		typedef struct tagMONITORINFO
		{
			DWORD   cbSize;
			RECT    rcMonitor;
			RECT    rcWork;
			DWORD   dwFlags;
		} MONITORINFO, *LPMONITORINFO;
    #endif
		// Import the GetMonitorInfo and MonitorFromWindow functions
		HMODULE hUser32 = LoadLibrary(_T("USER32.DLL"));
		typedef BOOL (WINAPI* LPGMI)(HMONITOR hMonitor, LPMONITORINFO lpmi);
		typedef HMONITOR (WINAPI* LPMFW)(HWND hwnd, DWORD dwFlags);
		LPMFW pfnMonitorFromWindow = (LPMFW)::GetProcAddress(hUser32, "MonitorFromWindow");
    #ifdef UNICODE
		LPGMI pfnGetMonitorInfo = (LPGMI)::GetProcAddress(hUser32, "GetMonitorInfoW");
	#else
		LPGMI pfnGetMonitorInfo = (LPGMI)::GetProcAddress(hUser32, "GetMonitorInfoA");
	#endif

		// Take multi-monitor systems into account
		if (pfnGetMonitorInfo && pfnMonitorFromWindow)
		{
			HMONITOR hActiveMonitor = pfnMonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
			MONITORINFO mi = { sizeof(mi), 0};

			if(pfnGetMonitorInfo(hActiveMonitor, &mi))
			{
				rcDesktop = mi.rcWork;
				if (GetParent() == NULL) rcParent = mi.rcWork;
			}
		}
		FreeLibrary(hUser32);
  #endif

		// Calculate point to center the dialog over the portion of parent window on this monitor
		rcParent.IntersectRect(rcParent, rcDesktop);
		int x = rcParent.left + (rcParent.Width() - rc.Width())/2;
		int y = rcParent.top + (rcParent.Height() - rc.Height())/2;

		// Keep the dialog wholly on the monitor display
		x = (x < rcDesktop.left)? rcDesktop.left : x;
		x = (x > rcDesktop.right - rc.Width())? rcDesktop.right - rc.Width() : x;
		y = (y < rcDesktop.top) ? rcDesktop.top: y;
		y = (y > rcDesktop.bottom - rc.Height())? rcDesktop.bottom - rc.Height() : y;

		SetWindowPos(HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
	}

	inline HWND CWnd::Create(HWND hWndParent /* = NULL */)
	// Default Window Creation.
	{
		try
		{
			// Test if Win32++ has been started
			if (0 == GetApp())
				throw CWinException(_T("Win32++ has not been initialised properly.\n Start the Win32++ by inheriting from CWinApp."));

			// Set the WNDCLASS parameters
			PreRegisterClass(m_wc);
			if (m_wc.lpszClassName)
			{
				RegisterClass(m_wc);
				m_cs.lpszClass = m_wc.lpszClassName;
			}

			// Set the CREATESTRUCT parameters
			PreCreate(m_cs);

			// Set the Window Class Name
			if (!m_cs.lpszClass)
				m_cs.lpszClass = _T("Win32++ Window");

			// Set Parent
			if (!hWndParent && m_cs.hwndParent)
				hWndParent = m_cs.hwndParent;

			// Set the window style
			DWORD dwStyle;
			DWORD dwOverlappedStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
			if (m_cs.style)
				dwStyle = m_cs.style;
			else
				dwStyle = WS_VISIBLE | ((hWndParent)? WS_CHILD : dwOverlappedStyle);

			// Set window size and position
			int x  = (m_cs.cx || m_cs.cy)? m_cs.x  : CW_USEDEFAULT;
			int cx = (m_cs.cx || m_cs.cy)? m_cs.cx : CW_USEDEFAULT;
			int y  = (m_cs.cx || m_cs.cy)? m_cs.y  : CW_USEDEFAULT;
			int cy = (m_cs.cx || m_cs.cy)? m_cs.cy : CW_USEDEFAULT;

			// Create the window
			CreateEx(m_cs.dwExStyle, m_cs.lpszClass, m_cs.lpszName, dwStyle, x, y,
				cx, cy, hWndParent, m_cs.hMenu, m_cs.lpCreateParams);
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		return m_hWnd;
	}

	inline HWND CWnd::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rc, HWND hParent, HMENU hMenu, LPVOID lpParam /*= NULL*/)
	{
		int x = rc.left;
		int y = rc.top;
		int cx = rc.right - rc.left;
		int cy = rc.bottom - rc.top;
		return CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, x, y, cx, cy, hParent, hMenu, lpParam);
	}

	inline HWND CWnd::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hParent, HMENU hMenu, LPVOID lpParam /*= NULL*/)
	{
		try
		{
			// Test if Win32++ has been started
			if (0 == GetApp())
				throw CWinException(_T("Win32++ has not been initialised properly.\n Start the Win32++ by inheriting from CWinApp."));

			// Only one window per CWnd instance allowed
			if (::IsWindow(m_hWnd))
				throw CWinException(_T("CWnd::CreateEx ... Window already exists"));

			// Prepare the CWnd if it has been reused
			Destroy();

			// Ensure a window class is registered
			TCHAR ClassName[MAX_STRING_SIZE +1] = _T("");
			if (0 == lstrlen(lpszClassName) )
				lstrcpyn (ClassName, _T("Win32++ Window"), MAX_STRING_SIZE);
			else
				// Create our own local copy of szClassName.
				lstrcpyn(ClassName, lpszClassName, MAX_STRING_SIZE);

			WNDCLASS wc = {0};
			wc.lpszClassName = ClassName;
			wc.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
			wc.hCursor		 = ::LoadCursor(NULL, IDC_ARROW);
			
			if (!RegisterClass(wc))	// Register the window class (if not already registered)
				throw CWinException(_T("CWnd::CreateEx  Failed to register window class"));

			// Ensure this thread has the TLS index set
			TLSData* pTLSData = GetApp()->SetTlsIndex();

			// Store the CWnd pointer in thread local storage
			pTLSData->pCWnd = this;

			// Create window
			m_hWnd = ::CreateWindowEx(dwExStyle, ClassName, lpszWindowName, dwStyle, x, y, nWidth, nHeight,
									hParent, hMenu, GetApp()->GetInstanceHandle(), lpParam);

			// Now handle window creation failure
			if (!m_hWnd)
				throw CWinException(_T("CWnd::CreateEx ... Failed to Create Window"));

			// Automatically subclass predefined window class types
			::GetClassInfo(GetApp()->GetInstanceHandle(), lpszClassName, &wc);
			if (wc.lpfnWndProc != GetApp()->m_Callback)
			{
				Subclass(m_hWnd);

				// Send a message to force the HWND to be added to the map
				::SendMessage(m_hWnd, WM_NULL, 0L, 0L);

				OnCreate(); // We missed the WM_CREATE message, so call OnCreate now
			}

			// Clear the CWnd pointer from TLS
			pTLSData->pCWnd = NULL;

			// Window creation is complete. Now call OnInitialUpdate
			OnInitialUpdate();
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		return m_hWnd;

	} // HWND CWnd::CreateEx()

	inline LRESULT CWnd::FinalWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	// Pass messages on to the appropriate default window procedure
	// CMDIChild and CMDIFrame override this function
	{
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	inline void CWnd::Destroy()
	{
		if (IsWindow()) ::DestroyWindow(m_hWnd);

		// Return the CWnd to its default state
		if (m_hIconLarge) ::DestroyIcon(m_hIconLarge);
		if (m_hIconSmall) ::DestroyIcon(m_hIconSmall);

		RemoveFromMap();
		m_hIconLarge = NULL;
		m_hIconSmall = NULL;
		m_hWnd = NULL;
		m_PrevWindowProc = NULL;
	}

	inline HWND CWnd::Detach()
	// Reverse an Attach
	{
		//Only a subclassed window can be detached
		if (0 == m_PrevWindowProc)
			throw CWinException(_T("CWnd::Detach  Unable to detach this window"));

		::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)m_PrevWindowProc);

		// Clear member variables
		HWND hWnd = m_hWnd;
		Destroy();

		return hWnd;
	}

	inline CWnd* CWnd::FromHandle(HWND hWnd)
	{
		// Returns the CWnd object associated with the window handle
		return GetApp()->GetCWndFromMap(hWnd);
	}

	inline HWND CWnd::GetAncestor() const
	// The GetAncestor function retrieves the handle to the ancestor (root parent)
	// of the window.
	{
		// Returns the root parent.  Supports Win95
		HWND hWnd = m_hWnd;
		HWND hWndParent = ::GetParent(hWnd);
		while (::IsChild(hWndParent, hWnd))
		{
			hWnd = hWndParent;
			hWndParent = ::GetParent(hWnd);
		}

		return hWnd;
	}

	inline tString CWnd::GetClassString() const
	// Retrieves the name of the class to which the specified window belongs
	{
		TCHAR szString[MAX_STRING_SIZE +1];
		tString tstr;
		::GetClassName(m_hWnd, szString, MAX_STRING_SIZE);
		tstr = szString;

		return tstr;
	}

	inline tString CWnd::GetDlgItemString(int nIDDlgItem) const
	// The GetDlgItemString function retrieves the title or text associated
	// with a control in a dialog box.
	{
		int nLength = ::GetWindowTextLength(GetDlgItem(nIDDlgItem));

		tString tstr;
		if (nLength > 0)
		{
			TCHAR szString[MAX_STRING_SIZE +1];
			::GetDlgItemText(m_hWnd, nIDDlgItem, szString, MAX_STRING_SIZE);
			tstr = szString;
		}

		return tstr;
	}

	inline tString CWnd::GetWindowString() const
	// Gets the window title for an ordinary window, or the text in an edit control
	{
		tString tstr;
		int nLength = ::GetWindowTextLength(m_hWnd);
		if (nLength > 0)
		{
			TCHAR* pszString = new TCHAR[nLength+1];
			if (NULL == pszString) throw std::bad_alloc();
			if (0 != ::GetWindowText(m_hWnd, pszString, nLength+1))
				tstr = pszString;

			delete [] pszString;
		}
		return tstr;
	}

	inline HBITMAP CWnd::LoadBitmap(LPCTSTR lpBitmapName) const
	{
		if (0 == GetApp())
			throw CWinException(_T("LoadBitmap ... Win32++ has not been initialised successfully."));

		HBITMAP hBitmap;

		// Try to load the bitmap from the resource handle first
		hBitmap = ::LoadBitmap(GetApp()->GetResourceHandle(), lpBitmapName);

		// The bitmap resource might be in the application's resources instead
		if (!hBitmap)
			hBitmap = ::LoadBitmap(GetApp()->GetInstanceHandle(), lpBitmapName);

		// No bitmap found, so display warning message
		if (!hBitmap)
			TRACE(_T("Unable to load bitmap\n"));

		return hBitmap;
	}

	inline LPCTSTR CWnd::LoadString(UINT nID)
	{
		// Returns the string associated with a Resource ID

		if (0 == GetApp())
			throw CWinException(_T("LoadString ... Win32++ has not been initialised successfully."));

		m_tsLoadString = _T("");
		TCHAR szString[MAX_STRING_SIZE +1] = _T("");
		if (!::LoadString (GetApp()->GetResourceHandle(), nID, szString, MAX_STRING_SIZE))
		{
			// The string resource might be in the application's resources instead
			if (::LoadString (GetApp()->GetInstanceHandle(), nID, szString, MAX_STRING_SIZE))
			{
				m_tsLoadString = szString;
				return (LPCTSTR) m_tsLoadString.c_str();
			}

			TCHAR msg[80] = _T("");
			::wsprintf(msg, _T("LoadString - No string resource for %d\n"), nID);
			TRACE(msg);
		}

		m_tsLoadString = szString;

		// Never return a pointer to a local variable, it is out of scope when the function returns.
		// We return a pointer to a member variable so it remains in scope.
		return m_tsLoadString.c_str();
	}

	inline BOOL CWnd::OnCommand(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		// Override this to handle WM_COMMAND messages, for example

		//	switch (LOWORD(wParam))
		//	{
		//	case IDM_FILE_NEW:
		//		OnFileNew();
		//		TRUE;	// return TRUE for handled commands
		//	}

		// return FALSE for unhandled commands
		return FALSE;
	}

	inline void CWnd::OnCreate()
	{
		// This function is called when a WM_CREATE message is recieved
		// Override it in your derived class to automatically perform tasks
		//  during window creation.
	}

	inline void CWnd::OnInitialUpdate()
	{
		// This function is called automatically once the window is created
		// Override it in your derived class to automatically perform tasks
		// after window creation.
	}

	inline LRESULT CWnd::MessageReflect(HWND hWndParent, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// A function used to call OnMessageReflect. You shouldn't need to call or
		//  override this function.

		HWND hWnd = NULL;
		switch (uMsg)
		{
		case WM_COMMAND:
		case WM_CTLCOLORBTN:
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORDLG:
		case WM_CTLCOLORLISTBOX:
		case WM_CTLCOLORSCROLLBAR:
		case WM_CTLCOLORSTATIC:
		case WM_CHARTOITEM:
		case WM_VKEYTOITEM:
		case WM_HSCROLL:
		case WM_VSCROLL:
			hWnd = (HWND)lParam;
			break;

		case WM_DRAWITEM:
		case WM_MEASUREITEM:
		case WM_DELETEITEM:
		case WM_COMPAREITEM:
			hWnd = ::GetDlgItem(hWndParent, (int)wParam);
			break;

		case WM_PARENTNOTIFY:
			switch(LOWORD(wParam))
			{
			case WM_CREATE:
			case WM_DESTROY:
				hWnd = (HWND)lParam;
				break;
			}
		}

		CWnd* Wnd = FromHandle(hWnd);

		if (Wnd != NULL)
			return Wnd->OnMessageReflect(uMsg, wParam, lParam);

		return 0L;
	}

	inline LRESULT CWnd::OnMessageReflect(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		// This function processes those special messages (see above) sent
		// by some older controls, and reflects them back to the originating CWnd object.
		// Override this function in your derrived class to handle these special messages.

		// Your overriding function should look like this ...

		// switch (uMsg)
		// {
		//		Handle your reflected messages here
		// }

		// return 0L for unhandled messages
		return 0L;
	}

	inline LRESULT CWnd::OnNotify(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		// You can use either OnNotifyReflect or OnNotify to handle notifications
		// Override OnNotifyReflect to handle notifications in the CWnd class that
		//   generated the notification.   OR
		// Override OnNotify to handle notifications in the PARENT of the CWnd class
		//   that generated the notification.

		// Your overriding function should look like this ...

		// switch (((LPNMHDR)lParam)->code)
		// {
		//		Handle your notifications from the CHILD window here
		//      Return the value recommended by the Windows API documentation.
		//      For many notifications, the return value doesn't matter, but for some it does.
		// }

		// return 0L for unhandled notifications
		return 0L;
	}

	inline LRESULT CWnd::OnNotifyReflect(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		// Override OnNotifyReflect to handle notifications in the CWnd class that
		//   generated the notification.

		// Your overriding function should look like this ...

		// switch (((LPNMHDR)lParam)->code)
		// {
		//		Handle your notifications from this window here
		//      Return the value recommended by the Windows API documentation.
		// }

		// return 0L for unhandled notifications
		return 0L;
	}

	inline void CWnd::OnPaint(HDC)
	// Called when part of the client area of the window needs to be painted
	{
		// Override this function in your derived class to perform drawing tasks.
	}

	inline void CWnd::OnMenuUpdate(UINT /*nID*/)
	// Called when menu items are about to be displayed
	{
		// Override this function to modify the behaviour of menu items,
		// such as adding or removing checkmarks
	}

	inline void CWnd::PreCreate(CREATESTRUCT& cs)
	// Called by CWnd::Create to set some window parameters
	{
		// Test if Win32++ has been started
		if (0 == GetApp())
			throw CWinException(_T("Win32++ has not been initialised properly.\n Start the Win32++ by inheriting from CWinApp."));

		m_cs.cx             = cs.cx;
		m_cs.cy             = cs.cy;
		m_cs.dwExStyle      = cs.dwExStyle;
		m_cs.hInstance      = GetApp()->GetInstanceHandle();
		m_cs.hMenu          = cs.hMenu;
		m_cs.hwndParent     = cs.hwndParent;
		m_cs.lpCreateParams = cs.lpCreateParams;
		m_cs.lpszClass      = cs.lpszClass;
		m_cs.lpszName       = cs.lpszName;
		m_cs.style          = cs.style;
		m_cs.x              = cs.x;
		m_cs.y              = cs.y;

		// Overide this function in your derived class to set the
		// CREATESTRUCT values prior to window creation.
		// The cs.lpszClass parameter should NOT be specified if the
		// PreRegisterClass function is used to create a window class.
	}

	inline void CWnd::PreRegisterClass(WNDCLASS& wc)
	// Called by CWnd::Create to set some window parameters
	//  Useful for setting the background brush and cursor
	{
		// Test if Win32++ has been started
		if (0 == GetApp())
			throw CWinException(_T("Win32++ has not been initialised properly.\n Start the Win32++ by inheriting from CWinApp."));

		m_wc.style			= wc.style;
		m_wc.lpfnWndProc	= CWnd::StaticWindowProc;
		m_wc.cbClsExtra		= wc.cbClsExtra;
		m_wc.cbWndExtra		= wc.cbWndExtra;
		m_wc.hInstance		= GetApp()->GetInstanceHandle();
		m_wc.hIcon			= wc.hIcon;
		m_wc.hCursor		= wc.hCursor;
		m_wc.hbrBackground	= wc.hbrBackground;
		m_wc.lpszMenuName	= wc.lpszMenuName;
		m_wc.lpszClassName  = wc.lpszClassName;

		// Overide this function in your derived class to set the
		// WNDCLASS values prior to window creation.

		// ADDITIONAL NOTES:
		// 1) The lpszClassName must be set for this function to take effect.
		// 2) The lpfnWndProc is always CWnd::StaticWindowProc.
		// 3) No other defaults are set, so the following settings might prove useful
		//     wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		//     wc.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
		//     wc.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
		// 4) The styles that can be set here are WNDCLASS styles. These are a different
		//     set of styles to those set by CREATESTRUCT (used in PreCreate).
		// 5) RegisterClassEx is not used because its not supported on WinCE.
		//     To set a small icon for the window, use SetIconSmall.
	}

	inline BOOL CWnd::PreTranslateMessage(MSG* /*pMsg*/)
	{
		// Override this function if your class requires input messages to be
		// translated before normal processing. Function which translate messages
		// include TranslateAccelerator, TranslateMDISysAccel and IsDialogMessage.
		// Return TRUE if the message is translated.

		return FALSE;
	}

	inline BOOL CWnd::RegisterClass(WNDCLASS& wc)
	// A private function used by the PreRegisterClass function to register a
	//  window class prior to window creation
	{
		try
		{
			// Test if Win32++ has been started
			if (0 == GetApp())
				throw CWinException(_T("Win32++ has not been initialised properly.\n Start the Win32++ by inheriting from CWinApp."));

			if (0 == (lstrlen(wc.lpszClassName) ) || (lstrlen(wc.lpszClassName) >  MAX_STRING_SIZE))
				throw CWinException(_T("CWnd::RegisterClass   Invalid class name"));

			// Check to see if this classname is already registered
			WNDCLASS wcTest = {0};
			if (::GetClassInfo(GetApp()->GetInstanceHandle(), wc.lpszClassName, &wcTest))
			{
				wc = wcTest;
				return TRUE;
			}

			// Set defaults
			wc.hInstance	= GetApp()->GetInstanceHandle();
			wc.lpfnWndProc	= CWnd::StaticWindowProc;

			// Register the WNDCLASS structure
			if (!::RegisterClass(&wc))
				throw CWinException(_T("Failed to register Window Class"));

			return TRUE;
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		return FALSE;
	}

	inline BOOL CWnd::RemoveFromMap()
	{
		// Allocate an iterator for our HWND map
		std::map<HWND, CWnd*, CompareHWND>::iterator m;

		CWinApp* pApp = GetApp();
		if (pApp)
		{
			// Erase the CWnd pointer entry from the map
			pApp->m_csMapLock.Lock();
			for (m = pApp->m_mapHWND.begin(); m != pApp->m_mapHWND.end(); ++m)
			{
				if (this == m->second)
				{
					pApp->m_mapHWND.erase(m);
					pApp->m_csMapLock.Release();
					return TRUE;
				}
			}

			pApp->m_csMapLock.Release();
		}
		return FALSE;
	}

	inline HICON CWnd::SetIconLarge(int nIcon)
	{
		m_hIconLarge = (HICON) (::LoadImage (GetApp()->GetResourceHandle(), MAKEINTRESOURCE (nIcon), IMAGE_ICON,
		::GetSystemMetrics (SM_CXICON), ::GetSystemMetrics (SM_CYICON), 0));

		::SendMessage (m_hWnd, WM_SETICON, WPARAM (ICON_BIG), LPARAM (m_hIconLarge));
		return m_hIconLarge;
	}

	inline HICON CWnd::SetIconSmall(int nIcon)
	{
		m_hIconSmall = (HICON) (::LoadImage (GetApp()->GetResourceHandle(), MAKEINTRESOURCE (nIcon), IMAGE_ICON,
		::GetSystemMetrics (SM_CXSMICON), ::GetSystemMetrics (SM_CYSMICON), 0));

		::SendMessage (m_hWnd, WM_SETICON, WPARAM (ICON_SMALL), LPARAM (m_hIconSmall));
		return m_hIconSmall;
	}



	inline LRESULT CALLBACK CWnd::StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	// All CWnd windows direct their messages here. This function redirects the message
	// to the CWnd's WndProc function.
	{
		try
		{
			CWnd* w = GetApp()->GetCWndFromMap(hWnd);
			if (0 != w)
			{
				// CWnd pointer found, so call the CWnd's WndProc
				return w->WndProc(uMsg, wParam, lParam);
			}
			else
			{
				// The CWnd pointer wasn't found in the map, so add it now

				// Retrieve the pointer to the TLS Data
				TLSData* pTLSData = (TLSData*)TlsGetValue(GetApp()->GetTlsIndex());
				if (NULL == pTLSData)
					throw CWinException(_T("CWnd::StaticCBTProc ... Unable to get TLS"));

				// Retrieve pointer to CWnd object from Thread Local Storage TLS
				w = pTLSData->pCWnd;
				if (NULL == w)
					throw CWinException(_T("CWnd::StaticWindowProc .. Failed to route message"));

				pTLSData->pCWnd = NULL;

				// Store the CWnd pointer in the HWND map
				w->m_hWnd = hWnd;
				w->AddToMap();

				return w->WndProc(uMsg, wParam, lParam);
			}
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (const std::bad_alloc &)
		{
			DebugErrMsg(_T("Memory allocation failure"));
			throw;	// Rethrow exception
		}

		catch (...)
		{
			DebugErrMsg(_T("Unknown Exception in CWnd::StaticWindowProc"));
			throw;	// Rethrow unknown exception
		}

		return 0L;

	} // LRESULT CALLBACK StaticWindowProc(...)

	inline void CWnd::Subclass(HWND hWnd)
	// A private function used by CreateEx, Attach and AttachDlgItem
	{
		if (m_PrevWindowProc)
			throw CWinException(_T("Subclass failed.  Already Subclassed"));

		// Subclass the window to pass messages to WndProc
		WNDPROC WndProc = (WNDPROC)::GetWindowLongPtr(hWnd, GWLP_WNDPROC);
		if (WndProc == GetApp()->m_Callback)
			throw CWinException(_T("Subclass failed.  Already sending messages to StaticWindowProc"));
		m_PrevWindowProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)CWnd::StaticWindowProc);
		if (NULL == m_PrevWindowProc)
			throw CWinException(_T("Subclass failed."));

		m_hWnd = hWnd;
	}

	inline LRESULT CWnd::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// Override this function in your class derrived from CWnd to handle
		//  window messages. A typical function might look like this:

		//	switch (uMsg)
		//	{
		//	case MESSAGE1:		// Some Windows API message
		//		OnMessage1();	// A user defined function
		//		break;			// Also do default processing
		//	case MESSAGE2:
		//		OnMessage2();
		//		return x;		// Don't do default processing, but instead return
		//						//  a value recommended by the Windows API documentation
		//	}

		// Always pass unhandled messages on to WndProcDefault
		return WndProcDefault(uMsg, wParam, lParam);
	}

	inline LRESULT CWnd::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	// All WndProc functions should pass unhandled window messages to this function
	{
    	switch (uMsg)
		{
		case WM_COMMAND:
			{
				// Refelect this message if it's from a control
				CWnd* pWnd = FromHandle((HWND)lParam);
				if (pWnd != NULL)
					if (pWnd->OnMessageReflect(uMsg, wParam, lParam))
						return TRUE;

				// Handle user commands
				if (OnCommand(wParam, lParam))
					return TRUE;
			}
			break;  // Note: Some MDI commands require default processing
		case WM_CREATE:
			OnCreate();
			break;
	// An example of how to end the application when the window closes
	//  If needed, put this in the class you inherit from CWnd
	//	case WM_DESTROY:
	//		::PostQuitMessage(0);
	//		return 0L;
		case WM_NOTIFY:
			{
				// Do Notification reflection if it came from a CWnd object
				HWND hwndFrom = ((LPNMHDR)lParam)->hwndFrom;
				CWnd* pWndFrom = FromHandle(hwndFrom);
				LRESULT lr = 0L;

				if (!(IsRebar()))	// Skip notification reflection for rebars to avoid double handling
				{
					if (pWndFrom != NULL)
					{
						lr = pWndFrom->OnNotifyReflect(wParam, lParam);
						if (lr) return lr;
					}
					else
					{
						// Some controls (eg ListView) have child windows.
						// Reflect those notifications too.
						CWnd* pWndFromParent = FromHandle(::GetParent(hwndFrom));
						if (pWndFromParent != NULL)
						{
							lr = pWndFromParent->OnNotifyReflect(wParam, lParam);
							if (lr) return lr;
						}
					}
				}

				// Handle user notifications
				lr = OnNotify(wParam, lParam);
				if (lr) return lr;
			}
			break;

		case WM_PAINT:
			{
				if (m_PrevWindowProc) break; // Allow normal painting for subclassed windows

				if (::GetUpdateRect(m_hWnd, NULL, FALSE))
				{
					::PAINTSTRUCT ps;
					HDC hDC = ::BeginPaint(m_hWnd, &ps);

					OnPaint(hDC);
					::EndPaint(m_hWnd, &ps);
				}
				else
				// RedrawWindow can require repainting without an update rect
				{
					HDC hDC = ::GetDC(m_hWnd);

					OnPaint(hDC);
					::ReleaseDC(m_hWnd, hDC);
				}
			}
			return 0L;

		// A set of messages to be reflected back to the control that generated them
		case WM_CTLCOLORBTN:
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORDLG:
		case WM_CTLCOLORLISTBOX:
		case WM_CTLCOLORSCROLLBAR:
		case WM_CTLCOLORSTATIC:
		case WM_DRAWITEM:
		case WM_MEASUREITEM:
		case WM_DELETEITEM:
		case WM_COMPAREITEM:
		case WM_CHARTOITEM:
		case WM_VKEYTOITEM:
		case WM_HSCROLL:
		case WM_VSCROLL:
		case WM_PARENTNOTIFY:
			{
				if (m_PrevWindowProc) break; // Suppress for subclassed windows

				LRESULT lr = MessageReflect(m_hWnd, uMsg, wParam, lParam);
				if (lr) return lr;	// Message processed so return
			}
			break;				// Do default processing when message not already processed

		case UWM_UPDATE_COMMAND:
			OnMenuUpdate((UINT)wParam); // Perform menu updates
		break;

		} // switch (uMsg)

		// Now hand all messages to the default procedure
		if (m_PrevWindowProc)
			return ::CallWindowProc(m_PrevWindowProc, m_hWnd, uMsg, wParam, lParam);
		else
			return FinalWindowProc(uMsg, wParam, lParam);

	} // LRESULT CWnd::WindowProc(...)


	//
	// Wrappers for Win32 API functions
	//

	inline BOOL CWnd::BringWindowToTop() const
	// The BringWindowToTop function brings the specified window to the top
	// of the Z order. If the window is a top-level window, it is activated.
	{
		return ::BringWindowToTop(m_hWnd);
	}

	inline LRESULT CWnd::CallWindowProc(WNDPROC lpPrevWndFunc, UINT Msg, WPARAM wParam, LPARAM lParam) const
	{
		return ::CallWindowProc(lpPrevWndFunc, m_hWnd, Msg, wParam, lParam);
	}

	inline BOOL CWnd::CheckDlgButton(int nIDButton, UINT uCheck) const
	// The CheckDlgButton function changes the check state of a button control.
	{
		return ::CheckDlgButton(m_hWnd, nIDButton, uCheck);
	}

	inline HDWP CWnd::DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) const
	// The DeferWindowPos function updates the specified multiple-window – position structure for the window.
	{
		return ::DeferWindowPos(hWinPosInfo, m_hWnd, hWndInsertAfter, x, y, cx, cy, uFlags);
	}

	inline HDWP CWnd::DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, const RECT& rc, UINT uFlags) const
	// The DeferWindowPos function updates the specified multiple-window – position structure for the window.
	{
		return ::DeferWindowPos(hWinPosInfo, m_hWnd, hWndInsertAfter, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, uFlags);
	}

	inline LRESULT CWnd::DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam) const
	// This function provides default processing for any window messages that an application does not process.
	{
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	inline BOOL CWnd::DrawMenuBar() const
	{
		return ::DrawMenuBar(m_hWnd);
	}

	inline BOOL CWnd::EnableWindow(BOOL bEnable /*= TRUE*/) const
	// The EnableWindow function enables or disables mouse and
	// keyboard input to the window.
	{
		return ::EnableWindow(m_hWnd, bEnable);
	}

	inline ULONG_PTR CWnd::GetClassLongPtr(int nIndex) const
	// The GetClassLongPtr function retrieves the specified value from the
	// WNDCLASSEX structure associated with the window.
	{
		return ::GetClassLongPtr(m_hWnd, nIndex);
	}

	inline CRect CWnd::GetClientRect() const
	// The GetClientRect function retrieves the coordinates of a window's client area.
	// The client coordinates specify the upper-left and lower-right corners of the
	// client area. Because client coordinates are relative to the upper-left corner
	// of a window's client area, the coordinates of the upper-left corner are (0,0).
	{
		CRect rc;
		::GetClientRect(m_hWnd, &rc);
		return rc;
	}

	inline HDC CWnd::GetDC() const
	// The GetDC function retrieves a handle to a display device context (DC) for the
	// client area of the window.
	{
		return ::GetDC(m_hWnd);
	}

	inline HDC CWnd::GetDCEx(HRGN hrgnClip, DWORD flags) const
	// The GetDCEx function retrieves a handle to a display device context (DC) for the
	// client area or entire area of a window
	{
		return ::GetDCEx(m_hWnd, hrgnClip, flags);
	}

	inline HWND CWnd::GetDlgItem(int nIDDlgItem) const
	// The GetDlgItem function retrieves a handle to a control in the dialog box.
	{
		return ::GetDlgItem(m_hWnd, nIDDlgItem);
	}

	inline int CWnd::GetDlgItemInt(int nIDDlgItem, BOOL* lpTranslated, BOOL bSigned) const
	{
		return ::GetDlgItemInt(m_hWnd, nIDDlgItem, lpTranslated, bSigned);
	}

	inline HWND CWnd::GetParent() const
	{
		return ::GetParent(m_hWnd);
	}

	inline LONG_PTR CWnd::GetWindowLongPtr(int nIndex) const
	// The GetWindowLongPtr function retrieves information about the window.
	{
		return ::GetWindowLongPtr(m_hWnd, nIndex);
	}

	inline BOOL CWnd::GetScrollInfo(int fnBar, SCROLLINFO& si) const
	// The GetScrollInfo function retrieves the parameters of a scroll bar, including
	// the minimum and maximum scrolling positions, the page size, and the position
	// of the scroll box (thumb).
	{
		return ::GetScrollInfo(m_hWnd, fnBar, &si);
	}

	inline HWND CWnd::GetWindow(UINT uCmd) const
	// The GetWindow function retrieves a handle to a window that has the specified
	// relationship (Z-Order or owner) to the specified window.
	// Possible uCmd values: GW_CHILD, GW_ENABLEDPOPUP, GW_HWNDFIRST, GW_HWNDLAST,
	// GW_HWNDNEXT, GW_HWNDPREV, GW_OWNER
	{
		return ::GetWindow(m_hWnd, uCmd);
	}

	inline HDC CWnd::GetWindowDC() const
	// The GetWindowDC function retrieves the device context (DC) for the entire
	// window, including title bar, menus, and scroll bars.
	{
		return ::GetWindowDC(m_hWnd);
	}

	inline CRect CWnd::GetWindowRect() const
	// retrieves the dimensions of the bounding rectangle of the window.
	// The dimensions are given in screen coordinates that are relative to the
	// upper-left corner of the screen.
	{
		CRect rc;
		::GetWindowRect(m_hWnd, &rc);
		return rc;
	}

	inline void CWnd::Invalidate(BOOL bErase /*= TRUE*/) const
	// The Invalidate function adds the entire client area the window's update region.
	// The update region represents the portion of the window's client area that must be redrawn.
	{
		::InvalidateRect(m_hWnd, NULL, bErase);
	}

	inline BOOL CWnd::InvalidateRect(LPCRECT lpRect, BOOL bErase /*= TRUE*/) const
	// The InvalidateRect function adds a rectangle to the window's update region.
	// The update region represents the portion of the window's client area that must be redrawn.
	{
		return ::InvalidateRect(m_hWnd, lpRect, bErase);
	}

	inline BOOL CWnd::InvalidateRgn(CONST HRGN hRgn, BOOL bErase /*= TRUE*/) const
	// The InvalidateRgn function invalidates the client area within the specified region
	// by adding it to the current update region of a window. The invalidated region,
	// along with all other areas in the update region, is marked for painting when the
	// next WM_PAINT message occurs.
	{
		return ::InvalidateRgn(m_hWnd, hRgn, bErase);
	}

	inline BOOL CWnd::IsChild(HWND hWndParent) const
	// The IsChild function tests whether a window is a child window or descendant window
	// of a parent window's CWnd.
	{
		return ::IsChild(hWndParent, m_hWnd);
	}

	inline BOOL CWnd::IsWindowEnabled() const
	// The IsWindowEnabled function determines whether the window is enabled
	// for mouse and keyboard input.
	{
		return ::IsWindowEnabled(m_hWnd);
	}

	inline BOOL CWnd::IsWindowVisible() const
	// The IsWindowVisible function retrieves the visibility state of the window.
	{
		return ::IsWindowVisible(m_hWnd);
	}

	inline BOOL CWnd::IsWindow() const
	// The IsWindow function determines whether the window exists.
	{
		return ::IsWindow(m_hWnd);
	}

	inline int CWnd::MessageBox(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) const
	// The MessageBox function creates, displays, and operates a message box.
	// Possible combinations of uType values include: MB_OK, MB_HELP, MB_OKCANCEL, MB_RETRYCANCEL,
	// MB_YESNO, MB_YESNOCANCEL, MB_ICONEXCLAMATION, MB_ICONWARNING, MB_ICONERROR (+ many others).
	{
		return ::MessageBox(m_hWnd, lpText, lpCaption, uType);
	}

	inline void CWnd::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint /* = TRUE*/) const
	// The MoveWindow function changes the position and dimensions of the window.
	{
		::MoveWindow(m_hWnd, x, y, nWidth, nHeight, bRepaint = TRUE);
	}

	inline void CWnd::MoveWindow(const RECT& rc, BOOL bRepaint /* = TRUE*/) const
	// The MoveWindow function changes the position and dimensions of the window.
	{
		::MoveWindow(m_hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, bRepaint);
	}

	inline BOOL CWnd::PostMessage(UINT uMsg, WPARAM wParam /*= 0L*/, LPARAM lParam /*= 0L*/) const
	// The PostMessage function places (posts) a message in the message queue
	// associated with the thread that created the window and returns without
	// waiting for the thread to process the message.
	{
		return ::PostMessage(m_hWnd, uMsg, wParam, lParam);
	}

	inline BOOL CWnd::PostMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
	// Required by by some macros
	{
		return ::PostMessage(hWnd, uMsg, wParam, lParam);
	}

	inline BOOL CWnd::RedrawWindow(LPCRECT lpRectUpdate, HRGN hRgn, UINT flags) const
	// The RedrawWindow function updates the specified rectangle or region in a window's client area.
	{
		return ::RedrawWindow(m_hWnd, lpRectUpdate, hRgn, flags);
	}

	inline int CWnd::ReleaseDC(HDC hDC) const
	// The ReleaseDC function releases a device context (DC), freeing it for use
	// by other applications.
	{
		return ::ReleaseDC(m_hWnd, hDC);
	}

	inline LRESULT CWnd::SendDlgItemMessage(int nIDDlgItem, UINT Msg, WPARAM wParam, LPARAM lParam) const
	// The SendDlgItemMessage function sends a message to the specified control in a dialog box.
	{
		return ::SendDlgItemMessage(m_hWnd, nIDDlgItem, Msg, wParam, lParam);
	}

	inline LRESULT CWnd::SendMessage(UINT uMsg, WPARAM wParam /*= 0L*/, LPARAM lParam /*= 0L*/) const
	// The SendMessage function sends the specified message to a window or windows.
	// It calls the window procedure for the window and does not return until the
	// window procedure has processed the message.
	{
		return ::SendMessage(m_hWnd, uMsg, wParam, lParam);
	}

	inline LRESULT CWnd::SendMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
	// Required by by some macros
	{
		return ::SendMessage(hWnd, uMsg, wParam, lParam);
	}

	inline HWND CWnd::SetActiveWindow() const
	// The SetActiveWindow function activates the window, but
	// not if the application is in the background.
	{
		return ::SetActiveWindow(m_hWnd);
	}

	inline HWND CWnd::SetCapture() const
	// The SetCapture function sets the mouse capture to the window.
	// SetCapture captures mouse input either when the mouse is over the capturing
	// window, or when the mouse button was pressed while the mouse was over the
	// capturing window and the button is still down.
	{
		return ::SetCapture(m_hWnd);
	}

	inline ULONG_PTR CWnd::SetClassLongPtr(int nIndex, LONG_PTR dwNewLong) const
	// The SetClassLongPtr function replaces the specified value at the specified offset in the
	// extra class memory or the WNDCLASSEX structure for the class to which the window belongs.
	{
		return ::SetClassLongPtr(m_hWnd, nIndex, dwNewLong);
	}

	inline HWND CWnd::SetFocus() const
	// The SetFocus function sets the keyboard focus to the window.
	{
		return ::SetFocus(m_hWnd);
	}

	inline BOOL CWnd::SetForegroundWindow() const
	// The SetForegroundWindow function puts the thread that created the window into the
	// foreground and activates the window.
	{
		return ::SetForegroundWindow(m_hWnd);
	}

	inline HWND CWnd::SetParent(HWND hParent) const
	// The SetParent function changes the parent window of the child window.
	{
		return ::SetParent(m_hWnd, hParent);
	}

	inline BOOL CWnd::SetRedraw(BOOL bRedraw /*= TRUE*/) const
	// This function allows changes in that window to be redrawn or prevents changes
	// in that window from being redrawn.
	{
		return (BOOL)::SendMessage(m_hWnd, WM_SETREDRAW, (WPARAM)bRedraw, 0L);
	}

	inline LONG_PTR CWnd::SetWindowLongPtr(int nIndex, LONG_PTR dwNewLong) const
	// The SetWindowLongPtr function changes an attribute of the window.
	{
		return ::SetWindowLongPtr(m_hWnd, nIndex, dwNewLong);
	}

	inline BOOL CWnd::SetWindowPos(HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) const
	// The SetWindowPos function changes the size, position, and Z order of a child, pop-up,
	// or top-level window.
	{
		return ::SetWindowPos(m_hWnd, hWndInsertAfter, x, y, cx, cy, uFlags);
	}

	inline BOOL CWnd::SetWindowPos(HWND hWndInsertAfter, const RECT& rc, UINT uFlags) const
	// The SetWindowPos function changes the size, position, and Z order of a child, pop-up,
	// or top-level window.
	{
		return ::SetWindowPos(m_hWnd, hWndInsertAfter, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, uFlags);
	}

	inline int CWnd::SetWindowRgn(HRGN hRgn, BOOL bRedraw /*= TRUE*/) const
	// The SetWindowRgn function sets the window region of the window.
	// The window region determines the area within the window where the system permits drawing.
	{
		return ::SetWindowRgn(m_hWnd, hRgn, bRedraw);
	}

	inline BOOL CWnd::SetDlgItemInt(int nIDDlgItem, UINT uValue, BOOL bSigned) const
	{
		return ::SetDlgItemInt(m_hWnd, nIDDlgItem, uValue, bSigned);
	}

	inline BOOL CWnd::SetDlgItemText(int nIDDlgItem, LPCTSTR lpString) const
	// The SetDlgItemText function sets the title or text of a control in a dialog box.
	{
		return ::SetDlgItemText(m_hWnd, nIDDlgItem, lpString);
	}

	inline BOOL CWnd::SetWindowText(LPCTSTR lpString) const
	// The SetWindowText function changes the text of the window's title bar (if it has one).
	{
		return ::SetWindowText(m_hWnd, lpString);
	}

	inline BOOL CWnd::ShowWindow(int nCmdShow /*= SW_SHOWNORMAL*/) const
	// The ShowWindow function sets the window's show state.
	{
		return ::ShowWindow(m_hWnd, nCmdShow);
	}

	inline BOOL CWnd::UpdateWindow() const
	// The UpdateWindow function updates the client area of the window by sending a
	// WM_PAINT message to the window if the window's update region is not empty.
	// If the update region is empty, no message is sent.
	{
		return ::UpdateWindow(m_hWnd);
	}

	inline BOOL CWnd::ValidateRect(LPCRECT prc) const
	// The ValidateRect function validates the client area within a rectangle by
	// removing the rectangle from the update region of the window.
	{
		return ::ValidateRect(m_hWnd, prc);
	}

	inline BOOL CWnd::ValidateRgn(HRGN hRgn) const
	// The ValidateRgn function validates the client area within a region by
	// removing the region from the current update region of the window.
	{
		return ::ValidateRgn(m_hWnd, hRgn);
	}

//
// These functions aren't supported on WinCE
//
#ifndef _WIN32_WCE
	inline BOOL CWnd::CloseWindow() const
	// The CloseWindow function minimizes (but does not destroy) the window.
	// To destroy a window, an application can use the Destroy function.
	{
		return ::CloseWindow(m_hWnd);
	}

	inline BOOL CWnd::EnableScrollBar(UINT uSBflags, UINT uArrows) const
	{
		return ::EnableScrollBar(m_hWnd, uSBflags, uArrows);
	}

	inline HMENU CWnd::GetMenu() const
	// The GetMenu function retrieves a handle to the menu assigned to the window.
	{
		return ::GetMenu(m_hWnd);
	}

	inline int CWnd::GetScrollPos(int nBar) const
	// The GetScrollPos function retrieves the current position of the scroll box
	// (thumb) in the specified scroll bar.
	{
		return ::GetScrollPos(m_hWnd, nBar);
	}

	inline BOOL CWnd::GetScrollRange(int nBar, int& MinPos, int& MaxPos) const
	// The GetScrollRange function retrieves the current minimum and maximum scroll box
	// (thumb) positions for the specified scroll bar.
	{
		return ::GetScrollRange(m_hWnd, nBar, &MinPos, &MaxPos );
	}

	inline BOOL CWnd::GetWindowPlacement(WINDOWPLACEMENT& wndpl) const
	// The GetWindowPlacement function retrieves the show state and the restored,
	// minimized, and maximized positions of the window.
	{
		return ::GetWindowPlacement(m_hWnd, &wndpl);
	}

	inline BOOL CWnd::IsIconic() const
	// The IsIconic function determines whether the window is minimized (iconic).
	{
		return ::IsIconic(m_hWnd);
	}

	inline BOOL CWnd::IsZoomed() const
	// The IsZoomed function determines whether the window is maximized.
	{
		return ::IsZoomed(m_hWnd);
	}

	inline BOOL CWnd::LockWindowUpdate(HWND hWndLock) const
	// Disables or enables drawing in the specified window. Only one window can be locked at a time.
	// Use a hWndLock of NULL to re-enable drawing in the window
	{
		return ::LockWindowUpdate(hWndLock);
	}

	inline BOOL CWnd::ScrollWindow(int XAmount, int YAmount, LPCRECT prcScroll, LPCRECT prcClip) const
	{
		return ::ScrollWindow(m_hWnd, XAmount, YAmount, prcScroll, prcClip);
	}

	inline int CWnd::ScrollWindowEx(int dx, int dy, LPCRECT prcScroll, LPCRECT prcClip, HRGN hrgnUpdate, LPRECT prcUpdate, UINT flags) const
	// The ScrollWindow function scrolls the contents of the window's client area.
	{
		return ::ScrollWindowEx(m_hWnd, dx, dy, prcScroll, prcClip, hrgnUpdate, prcUpdate, flags);
	}

	inline BOOL CWnd::SetMenu(HMENU hMenu) const
	// The SetMenu function assigns a new menu to the specified window.
	{
		return ::SetMenu(m_hWnd, hMenu);
	}

	inline int CWnd::SetScrollInfo(int fnBar, const SCROLLINFO& si, BOOL fRedraw) const
	// The SetScrollInfo function sets the parameters of a scroll bar, including
	// the minimum and maximum scrolling positions, the page size, and the
	// position of the scroll box (thumb).
	{
		return ::SetScrollInfo(m_hWnd, fnBar, &si, fRedraw);
	}

	inline int CWnd::SetScrollPos(int nBar, int nPos, BOOL bRedraw) const
	// The SetScrollPos function sets the position of the scroll box (thumb) in
	// the specified scroll bar
	{
		return ::SetScrollPos(m_hWnd, nBar, nPos, bRedraw);
	}

	inline BOOL CWnd::SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw) const
	// The SetScrollRange function sets the minimum and maximum scroll box positions for the scroll bar.
	{
		return ::SetScrollRange(m_hWnd, nBar, nMinPos, nMaxPos, bRedraw);
	}

	inline BOOL CWnd::SetWindowPlacement(const WINDOWPLACEMENT& wndpl) const
	// The SetWindowPlacement function sets the show state and the restored, minimized,
	// and maximized positions of the window.
	{
		return ::SetWindowPlacement(m_hWnd, &wndpl);
	}

	inline BOOL CWnd::ShowScrollBar(int nBar, BOOL bShow) const
	{
		return ::ShowScrollBar(m_hWnd, nBar, bShow);
	}
#endif

}; // namespace Win32xx


#endif // _WINCORE_H_

