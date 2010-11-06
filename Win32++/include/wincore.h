// Win32++  Version 7.0 (Prerelease version)
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2010  David Nash
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
  #pragma option -w-8026            // Functioms with exception specifiations are not expanded inline
  #pragma option -w-8027		    // function not expanded inline
  #define STRICT 1
#endif

#ifdef _WIN32_WCE
  #include "wcestddef.h"
#endif

#define _WINSOCKAPI_            // Prevent winsock.h #include's.

#include <assert.h>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <shlwapi.h>
#include "shared_ptr.h"
#include "winutils.h"
#include "gdi.h"

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
#define UWM_GETMENUTHEME    (WM_APP + 16)	// Message - returns a pointer to MenuTheme
#define UWM_GETREBARTHEME   (WM_APP + 17)	// Message - returns a pointer to CToolBar
#define UWM_GETTOOLBARTHEME (WM_APP + 18)   // Message - returns a pointer to ToolBarTheme


// Automatically include the Win32xx namespace
// define NO_USING_NAMESPACE to skip this step
namespace Win32xx {}
#ifndef NO_USING_NAMESPACE
  using namespace Win32xx;
#endif

// Required for WinCE
#ifndef TLS_OUT_OF_INDEXES
  #define TLS_OUT_OF_INDEXES ((DWORD_PTR) -1)
#endif
#ifndef WM_PARENTNOTIFY
  #define WM_PARENTNOTIFY 0x0210
#endif


namespace Win32xx
{


	////////////////////////////////////////////////
	// Forward declarations.
	//  These classes are defined later or elsewhere
	class CWinApp;
	class CWnd;

	// tString is a TCHAR std::string
	typedef std::basic_string<TCHAR> tString;

	// WndPtr is a Shared_Ptr of CWnd
	typedef Shared_Ptr<CWnd> WndPtr;


	//////////////////////////////////////////////////
	// Global functions	(within the Win32xx namespace)

	CWinApp* GetApp();
	void TRACE(LPCTSTR str);

  #ifndef _WIN32_WCE		// for Win32/64 operating systems
	int  GetWinVersion();
	int  GetComCtlVersion();
	UINT GetSizeofMenuItemInfo();
	UINT GetSizeofNonClientMetrics();
	BOOL IsAeroThemed();
	BOOL IsXPThemed();
	BOOL IsLeftButtonDown();
  #endif // #ifndef _WIN32_WCE

  #ifndef lstrcpyn			// Required for WinCE
	LPTSTR lstrcpyn(LPTSTR lpstrDest, LPCTSTR lpstrSrc, int nLength);
  #endif // !lstrcpyn

	enum Constants			// Defines the maximum size for TCHAR strings
	{
		MAX_MENU_STRING = 80,
		MAX_STRING_SIZE = 255,
	};

	struct CompareHWND		// The comparison function object used by CWinApp::m_mapHWND
	{
		bool operator()(HWND const a, const HWND b) const
			{return ((DWORD_PTR)a < (DWORD_PTR)b);}
	};

	struct TLSData			// Used for Thread Local Storage (TLS)
	{
		CWnd* pCWnd;		// pointer to CWnd object for Window creation
		CWnd* pMenuBar;		// pointer to CMenuBar object used for the WH_MSGFILTER hook
		HHOOK hHook;		// WH_MSGFILTER hook for CMenuBar and Modeless Dialogs
		std::vector<char>  vChar;	// A vector used as a char array for text conversions
		std::vector<WCHAR> vWChar;	// A vector used as a WCHAR array for text conversions
		std::vector<WndPtr> vTmpWnds;	// A vector of temporary CWnd pointers

		TLSData() : pCWnd(0), pMenuBar(0), hHook(0) {}
	};

	////////////////////////////////////////
	// Global functions for text conversions
	//
	inline LPCWSTR CharToWide(LPCSTR pChar);
	inline LPCSTR WideToChar(LPCWSTR pWChar);
	inline LPCTSTR CharToTChar(LPCSTR pChar);
	inline LPCSTR TCharToChar(LPCTSTR pTChar);
	inline LPCWSTR TCharToWide(LPCTSTR pTChar);


	/////////////////////////////////////////
	// Declarations for the CCriticalSection class
	// This class is used for thread synchronisation
	class CCriticalSection
	{
	public:
		CCriticalSection()	{ ::InitializeCriticalSection(&m_cs); }
		~CCriticalSection()	{ ::DeleteCriticalSection(&m_cs); }

		void Lock() 	{ ::EnterCriticalSection(&m_cs); }
		void Release()	{ ::LeaveCriticalSection(&m_cs); }

	private:
		CCriticalSection ( const CCriticalSection& );
		void operator = ( const CCriticalSection& );

		CRITICAL_SECTION m_cs;
	};


	////////////////////////////////////////
	// Declaration of the CWinException class
	//
	class CWinException : public std::exception
	{
	public:
		CWinException (LPCTSTR msg);
		virtual ~CWinException() throw() {}

		virtual LPCTSTR GetErrorString() const { return m_ErrorString; }
		virtual const char * what () const throw ();
	private:
		DWORD  m_err;
		LPCTSTR m_msg;
		TCHAR m_ErrorString[MAX_STRING_SIZE];
	};

	////////////////////////////////
	// Declaration of the CWnd class
	//
	class CWnd
	{
	friend class CMDIChild;
	friend class CDialog;
	friend class CPropertyPage;
	friend class CWinApp;

	public:
		CWnd();				// Constructor
		virtual ~CWnd();	// Destructor

		// These virtual functions can be overridden
		virtual BOOL Attach(HWND hWnd);
		virtual BOOL AttachDlgItem(UINT nID, CWnd* pParent);
		virtual void CenterWindow() const;
		virtual HWND Create(HWND hWndParent = NULL);
		virtual HWND CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hParent, HMENU hMenu, LPVOID lpParam = NULL);
		virtual HWND CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rc, HWND hParent, HMENU hMenu, LPVOID lpParam = NULL);
		virtual void Destroy();
		virtual HWND Detach();
		virtual tString GetWindowType() const { return _T("CWnd"); }
		virtual void Invalidate(BOOL bErase = TRUE) const;
		virtual HICON SetIconLarge(int nIcon);
		virtual HICON SetIconSmall(int nIcon);

		// Attributes
		static CWnd* FromHandle(HWND hWnd);
		HWND GetHwnd() const				{ return m_hWnd; }
		WNDPROC GetPrevWindowProc() const	{ return m_PrevWindowProc; }

		// Wrappers for Win32 API functions
		// These functions aren't virtual, and shouldn't be overridden
		BOOL BringWindowToTop() const;
		LRESULT CallWindowProc(WNDPROC lpPrevWndFunc, UINT Msg, WPARAM wParam, LPARAM lParam) const;
		BOOL CheckDlgButton(int nIDButton, UINT uCheck) const;
		LRESULT DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam) const;
		HDWP  DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) const;
		HDWP  DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, const RECT& rc, UINT uFlags) const;
		BOOL  DrawMenuBar() const;
		BOOL  EnableWindow(BOOL bEnable = TRUE) const;
		CWnd* GetActiveWindow() const;
		CWnd* GetAncestor() const;
		CWnd* GetCapture() const;
		ULONG_PTR GetClassLongPtr(int nIndex) const;
		LPCTSTR GetClassName() const;
		CRect GetClientRect() const;
		HDC   GetDC() const;
		HDC   GetDCEx(HRGN hrgnClip, DWORD flags) const;
		CWnd* GetDlgItem(int nIDDlgItem) const;
		UINT  GetDlgItemInt(int nIDDlgItem, BOOL* lpTranslated, BOOL bSigned) const;
		LPCTSTR GetDlgItemText(int nIDDlgItem) const;
		CWnd* GetFocus() const;
		HFONT GetFont() const;
		HICON GetIcon(BOOL bBigIcon) const;
		CWnd* GetParent() const;
		BOOL  GetScrollInfo(int fnBar, SCROLLINFO& si) const;
		CWnd* GetWindow(UINT uCmd) const;
		HDC   GetWindowDC() const;
		LONG_PTR GetWindowLongPtr(int nIndex) const;
		CRect GetWindowRect() const;
		LPCTSTR GetWindowText() const;
		int   GetWindowTextLength() const;
		BOOL  InvalidateRect(LPCRECT lpRect, BOOL bErase = TRUE) const;
		BOOL  InvalidateRgn(CONST HRGN hRgn, BOOL bErase = TRUE) const;
		BOOL  IsChild(CWnd* pChild) const;
		BOOL  IsWindow() const;
		BOOL  IsWindowEnabled() const;
		BOOL  IsWindowVisible() const;
		BOOL  KillTimer(UINT_PTR uIDEvent) const;
		HBITMAP LoadBitmap(LPCTSTR lpBitmapName) const;
		LPCTSTR LoadString(UINT nID);
		int   MessageBox(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) const;
		BOOL  MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE) const;
		BOOL  MoveWindow(const RECT& rc, BOOL bRepaint = TRUE) const;
		BOOL  PostMessage(UINT uMsg, WPARAM wParam = 0L, LPARAM lParam = 0L) const;
		BOOL  PostMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;
		BOOL  RedrawWindow(LPCRECT lpRectUpdate = NULL, HRGN hRgn = NULL, UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN) const;
		int   ReleaseDC(HDC hDC) const;
		LRESULT SendDlgItemMessage(int nIDDlgItem, UINT Msg, WPARAM wParam, LPARAM lParam) const;
		LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0L, LPARAM lParam = 0L) const;
		LRESULT SendMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;
		CWnd* SetActiveWindow() const;
		CWnd* SetCapture() const;
		ULONG_PTR SetClassLongPtr(int nIndex, LONG_PTR dwNewLong) const;
		BOOL  SetDlgItemInt(int nIDDlgItem, UINT uValue, BOOL bSigned) const;
		BOOL  SetDlgItemText(int nIDDlgItem, LPCTSTR lpString) const;
		CWnd* SetFocus() const;
		void  SetFont(HFONT hFont, BOOL bRedraw) const;
		BOOL  SetForegroundWindow() const;
		HICON SetIcon(HICON hIcon, BOOL bBigIcon) const;
		CWnd* SetParent(CWnd* pWndParent) const;
		BOOL  SetRedraw(BOOL bRedraw = TRUE) const;
		int   SetScrollInfo(int fnBar, const SCROLLINFO& si, BOOL fRedraw) const;
		UINT_PTR SetTimer(UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc) const;
		LONG_PTR SetWindowLongPtr(int nIndex, LONG_PTR dwNewLong) const;
		BOOL  SetWindowPos(HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) const;
		BOOL  SetWindowPos(HWND hWndInsertAfter, const RECT& rc, UINT uFlags) const;
		int   SetWindowRgn(HRGN hRgn, BOOL bRedraw = TRUE) const;
		BOOL  SetWindowText(LPCTSTR lpString) const;
		HRESULT SetWindowTheme(LPCWSTR pszSubAppName, LPCWSTR pszSubIdList) const;
		BOOL  ShowWindow(int nCmdShow = SW_SHOWNORMAL) const;
		BOOL  UpdateWindow() const;
		BOOL  ValidateRect(LPCRECT prc) const;
		BOOL  ValidateRgn(HRGN hRgn) const;

  #ifndef _WIN32_WCE
		BOOL  CloseWindow() const;
		BOOL  EnableScrollBar(UINT uSBflags, UINT uArrows) const;
		CWnd* GetLastActivePopup() const;
		HMENU GetMenu() const;
		int   GetScrollPos(int nBar) const;
		BOOL  GetScrollRange(int nBar, int& MinPos, int& MaxPos) const;
		CWnd* GetTopWindow() const;
		BOOL  GetWindowPlacement(WINDOWPLACEMENT& pWndpl) const;
		BOOL  IsIconic() const;
		BOOL  IsZoomed() const;
		BOOL  LockWindowUpdate() const;
		BOOL  OpenIcon() const;
		BOOL  SetMenu(HMENU hMenu) const;
		BOOL  ScrollWindow(int XAmount, int YAmount, LPCRECT prcScroll, LPCRECT prcClip) const;
		int   ScrollWindowEx(int dx, int dy, LPCRECT prcScroll, LPCRECT prcClip, HRGN hrgnUpdate, LPRECT prcUpdate, UINT flags) const;
		int   SetScrollPos(int nBar, int nPos, BOOL bRedraw) const;
		BOOL  SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw) const;
		BOOL  SetWindowPlacement(const WINDOWPLACEMENT& wndpl) const;
		BOOL  ShowOwnedPopups(BOOL fShow) const;
		BOOL  ShowScrollBar(int nBar, BOOL bShow) const;
		BOOL  ShowWindowAsync(int nCmdShow) const;
		BOOL  UnLockWindowUpdate() const;
		CWnd* WindowFromDC(HDC hDC) const;
  #endif

		static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		operator HWND() const {return m_hWnd;}

	protected:
		// Override these functions as required
		virtual LRESULT FinalWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
		virtual void OnCreate();
		virtual void OnInitialUpdate();
		virtual void OnMenuUpdate(UINT nID);		
		virtual LRESULT OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void OnPaint(CDC& dc);
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void PreRegisterClass(WNDCLASS& wc);
		virtual BOOL PreTranslateMessage(MSG* pMsg);
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
		mutable std::vector<TCHAR> m_vTChar;	// A vector used as a TCHAR array for string functions
		BOOL m_IsTmpWnd;			// True if this CWnd is a TmpWnd 

	}; // class CWnd


	///////////////////////////////////
	// Declaration of the CWinApp class
	//
	class CWinApp
	{
		friend class CWnd;			// CWnd needs access to CWinApp's private members
		friend class CDialog;
		friend class CMenuBar;
		friend class CPropertyPage;
		friend class CPropertySheet;
		friend CWinApp* GetApp();	// GetApp needs access to SetnGetThis
		friend LPCWSTR CharToWide(LPCSTR pChar);
		friend LPCSTR WideToChar(LPCWSTR pWChar);

		typedef Shared_Ptr<TLSData> TLSDataPtr;

	public:
		CWinApp();
		virtual ~CWinApp();

		HINSTANCE GetInstanceHandle() const { return m_hInstance; }
		HINSTANCE GetResourceHandle() const { return (m_hResource ? m_hResource : m_hInstance); }	
		void SetResourceHandle(HINSTANCE hResource);

		// These are the functions you might wish to override
		virtual BOOL InitInstance();
		virtual int  MessageLoop();
		virtual int Run();

	private:
		CWinApp(const CWinApp&);				// Disable copy construction
		CWinApp& operator = (const CWinApp&);	// Disable assignment operator
		void AddTmpWnd(HWND hWnd);
		CWnd* GetCWndFromMap(HWND hWnd);
		DWORD GetTlsIndex() const {return m_dwTlsIndex;}
		void RemoveTmpWnds();
		void SetCallback();
		TLSData* SetTlsIndex();
		static CWinApp* SetnGetThis(CWinApp* pThis = 0);

		std::map<HWND, CWnd*, CompareHWND> m_mapHWND;	// maps window handles to CWnd objects
		std::vector<TLSDataPtr> m_vTLSData;	// vector of TLSData smart pointers, one for each thread
		CCriticalSection m_csMapLock;	// thread synchronisation for m_mapHWND
		CCriticalSection m_csTlsData;	// thread synchronisation for m_ csvTlsData
		HINSTANCE m_hInstance;			// handle to the applications instance
		HINSTANCE m_hResource;			// handle to the applications resources
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
		//  2601     Windows 7

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
		// 616  dll ver 6.16    Windows Vista SP1 or Windows 7 with XP themes

		return ComCtlVer;
	}

	inline UINT GetSizeofMenuItemInfo()
	{
		UINT uSize = sizeof(MENUITEMINFO);
		// For Win95 and NT, cbSize needs to be 44
		if (1400 == (GetWinVersion()) || (2400 == GetWinVersion()))
			uSize = 44;

		return uSize;
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

	inline BOOL IsAeroThemed()
	{
		BOOL bIsAeroThemed = FALSE;

		// Test if Windows version is XP or greater
		if (GetWinVersion() >= 2501)
		{
			HMODULE hMod = ::LoadLibrary(_T("uxtheme.dll"));
			if(hMod)
			{
				// Declare pointers to functions
				FARPROC pIsCompositionActive    = ::GetProcAddress(hMod, "IsCompositionActive");

				if(pIsCompositionActive)
				{
					if(pIsCompositionActive())
					{
						bIsAeroThemed = TRUE;
					}
				}
				::FreeLibrary(hMod);
			}
		}

		return bIsAeroThemed;
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

	inline CPoint GetCursorPos()
	{
		CPoint pt;
		::GetCursorPos(&pt);
		return pt;
	}

	//////////////////////////////////////////////////////////////////////////////////
	// Global functions for text conversions
	// Note:  The pointers returned should be be used immediately. Otherwise, copy the
	//        array pointed to a local array or std::string. Subsequent calls to these
	//        functions will change the text array the returned pointer points to.
	//
	//        Using TLS (Thread Local Storage) allows these functions to be thread safe.
	//
	inline LPCWSTR CharToWide(LPCSTR pChar)
	{
		assert( GetApp() );

		// Ensure this thread has the TLS index set
		TLSData* pTLSData = GetApp()->SetTlsIndex();

		// Resize the vector and assign null WCHAR to each element
		int length = (int)strlen(pChar)+1;
		pTLSData->vWChar.assign(length, L'\0');
		WCHAR* pWCharArray = &pTLSData->vWChar.front();

		// Fill our vector with the converted WCHAR array
		MultiByteToWideChar(CP_ACP, 0, pChar, -1, pWCharArray, length);

		// return a pointer to the first element in the vector
		return pWCharArray;
	}

	inline LPCSTR WideToChar(LPCWSTR pWChar)
	{
		assert( GetApp() );

		// Ensure this thread has the TLS index set
		TLSData* pTLSData = GetApp()->SetTlsIndex();

		// Resize the vector and assign null char to each element
		int length = (int)wcslen(pWChar)+1;
		pTLSData->vChar.assign(length, '\0');
		char* pCharArray = &pTLSData->vChar.front();

		// Fill our vector with the converted char array
		WideCharToMultiByte(CP_ACP, 0, pWChar, -1, pCharArray, length, NULL,NULL);

		// return a pointer to the first element in the vector
		return pCharArray;
	}

	inline LPCTSTR CharToTChar(LPCSTR pChar)
	{

#ifdef UNICODE
		return CharToWide(pChar);
#else
		return pChar;
#endif

	}

	inline LPCSTR TCharToChar(LPCTSTR pTChar)
	{

#ifdef UNICODE
		return WideToChar(pTChar);
#else
		return pTChar;
#endif

	}

	inline LPCWSTR TCharToWide(LPCTSTR pTChar)
	{

  #ifdef UNICODE
		return pTChar;
  #else
		return CharToWide(pTChar);
  #endif

	}

	inline LPCTSTR WideToTChar(LPWSTR pWChar)
	{

  #ifdef UNICODE
		return pWChar;
  #else
		return WideToChar(pWChar);
  #endif

	}


	//////////////////////////////////////////
	// Definitions for the CWinException class
	//
	inline CWinException::CWinException (LPCTSTR msg) : m_err (::GetLastError()), m_msg(msg)
	{
		memset(m_ErrorString, 0, MAX_STRING_SIZE * sizeof(TCHAR));

		if (m_err != 0)
		{
			DWORD dwFlags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
			::FormatMessage(dwFlags, NULL, m_err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), m_ErrorString, MAX_STRING_SIZE, NULL);
		}
	}

	inline const char * CWinException::what () const throw ()
	{
		// This sends text to the debugger (typically displayed in the IDE's output window).

		OutputDebugString( _T("*** An exception occurred ***\n") );
		OutputDebugString( m_msg );
		OutputDebugString( _T("\n") );
		OutputDebugString( GetErrorString() );
		OutputDebugString( _T("\n") );
		OutputDebugString( _T("*** End of exception report ***\n") );

		return "CWinException thrown";
	}


	////////////////////////////////////
	// Definitions for the CWinApp class
	//

	// To begin Win32++, inherit your application class from this one.
	// You must run only one instance of the class inherited from this.
	inline CWinApp::CWinApp() : m_Callback(NULL)
	{
		try
		{
			assert( 0 == SetnGetThis() );	// Test if this is the first instance of CWinApp

			m_dwTlsIndex = ::TlsAlloc();
			if (m_dwTlsIndex == TLS_OUT_OF_INDEXES)
			{
				// We only get here in the unlikely event that all TLS indexes are already allocated by this app
				// At least 64 TLS indexes per process are allowed. Win32++ requires only one TLS index.
				throw CWinException(_T("CWinApp::CWinApp  Failed to allocate TLS Index"));
			}

			SetnGetThis(this);

			// Set the instance handle
	#ifdef _WIN32_WCE
			m_hInstance = (HINSTANCE)GetModuleHandle(0);
	#else
			MEMORY_BASIC_INFORMATION mbi = {0};
			VirtualQuery( (LPCVOID)SetnGetThis, &mbi, sizeof(mbi) );
			assert(mbi.AllocationBase);
			m_hInstance = (HINSTANCE)mbi.AllocationBase;
			m_hResource = m_hInstance;
	#endif

			SetCallback();
		}

		catch (const CWinException &e)
		{
			e.what();
			throw;
		}
	}

	inline CWinApp::~CWinApp()
	{
		std::vector<TLSDataPtr>::iterator iter;
		for (iter = m_vTLSData.begin(); iter < m_vTLSData.end(); ++iter)
		{
			(*iter)->vTmpWnds.clear();
		}
		
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

		SetnGetThis((CWinApp*)-1);
	}

	inline void CWinApp::AddTmpWnd(HWND hWnd)
	{
		// TmpWnds are created if required to support functions like CWnd::GetParent.
		// The TmpWnds are temporary, deleted when a CWnd is destroyed.
		CWnd* pWnd = new CWnd;
		pWnd->m_hWnd = hWnd;
		pWnd->AddToMap();
		pWnd->m_IsTmpWnd = TRUE;

		// Ensure this thread has the TLS index set
		TLSData* pTLSData = GetApp()->SetTlsIndex();

		pTLSData->vTmpWnds.push_back(pWnd); // save TmpWnd as a smart pointer
	}

	inline void CWinApp::SetCallback()
	{
		// Registers a temporary window class so we can get the callback
		// address of CWnd::StaticWindowProc

		WNDCLASS wcDefault = {0};

		LPCTSTR szClassName		= _T("Win32++ Temporary Window Class");
		wcDefault.hInstance		= GetInstanceHandle();
		wcDefault.lpfnWndProc	= CWnd::StaticWindowProc;
		wcDefault.lpszClassName = szClassName;

		::RegisterClass(&wcDefault);

		// Retrieve the class information
		ZeroMemory(&wcDefault, sizeof(wcDefault));
		::GetClassInfo(GetInstanceHandle(), szClassName, &wcDefault);

		// Save the callback address of CWnd::StaticWindowProc
		assert(wcDefault.lpfnWndProc);
		m_Callback = wcDefault.lpfnWndProc;
		::UnregisterClass(szClassName, GetInstanceHandle());
	}

	inline CWnd* CWinApp::GetCWndFromMap(HWND hWnd)
	{
		// Allocate an iterator for our HWND map
		std::map<HWND, CWnd*, CompareHWND>::iterator m;
		
		// Find the CWnd pointer mapped to this HWND
		CWnd* pWnd = 0;
		m_csMapLock.Lock();
		m = m_mapHWND.find(hWnd);
		
		if (m != m_mapHWND.end())
			pWnd = m->second;

		m_csMapLock.Release();
		return pWnd;
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
						if(Processed)
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

	inline void CWinApp::RemoveTmpWnds()
	{
		// Removes all TmpWnds belonging to this thread

		// Retrieve the pointer to the TLS Data
		TLSData* pTLSData = (TLSData*)TlsGetValue(GetApp()->GetTlsIndex());
		
		if (pTLSData)
			pTLSData->vTmpWnds.clear();
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

		catch (const CWinException &e)
		{
			e.what();
			return -1;
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

	inline void CWinApp::SetResourceHandle(HINSTANCE hResource)
	{
		// This function can be used to load a resource dll.
		// A resource dll can be used to define resources in different languages.
		// To use this function, place code like this in InitInstance
		//
		// HINSTANCE hResource = LoadLibrary(_T("MyResourceDLL.dll"));
		// SetResourceHandle(hResource);

		m_hResource = hResource;
	}

	inline TLSData* CWinApp::SetTlsIndex()
	{
		TLSData* pTLSData = (TLSData*)::TlsGetValue(GetTlsIndex());
		if (NULL == pTLSData)
		{
			pTLSData = new TLSData;

			m_csTlsData.Lock();
			m_vTLSData.push_back(pTLSData);	// store as a Shared_Ptr
			m_csTlsData.Release();

			::TlsSetValue(GetTlsIndex(), pTLSData);
		}

		return pTLSData;
	}


	////////////////////////////////////////
	// Definitions for the CWnd class
	//
	inline CWnd::CWnd() : m_hWnd(NULL), m_hIconLarge(NULL), m_hIconSmall(NULL),
						m_PrevWindowProc(NULL), m_IsTmpWnd(FALSE)
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
	// Store the window handle and CWnd pointer in the HWND map
	{
		assert( GetApp() );
		GetApp()->m_csMapLock.Lock();

		assert(::IsWindow(m_hWnd));
		assert(!GetApp()->GetCWndFromMap(m_hWnd));

		GetApp()->m_mapHWND.insert(std::make_pair(m_hWnd, this));
		GetApp()->m_csMapLock.Release();
	}

	inline BOOL CWnd::Attach(HWND hWnd)
	// Subclasses an existing window and attaches it to a CWnd
	{
		assert( GetApp() );
		assert(::IsWindow(hWnd));

		if (m_PrevWindowProc)
			Detach();

		Subclass(hWnd);

		// Store the CWnd pointer in the HWND map
		AddToMap();
		OnInitialUpdate();

		return TRUE;
	}

	inline BOOL CWnd::AttachDlgItem(UINT nID, CWnd* pParent)
	// Use to convert a dialog item to a CWnd object
	{
		assert(pParent->IsWindow());

		HWND hWnd = ::GetDlgItem(pParent->GetHwnd(), nID);
		return Attach(hWnd);
	}

	inline void CWnd::CenterWindow() const
	// Centers this window over it's parent
	{
		assert(::IsWindow(m_hWnd));

		CRect rc = GetWindowRect();
		CRect rcParent;
		CRect rcDesktop;

		// Get screen dimensions excluding task bar
		::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcDesktop, 0);

		// Get the parent window dimensions (parent could be the desktop)
		if (GetParent() != NULL) rcParent = GetParent()->GetWindowRect();
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

		// Test if Win32++ has been started
		assert( GetApp() );

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

		assert( GetApp() );		// Test if Win32++ has been started
		assert(!::IsWindow(m_hWnd));	// Only one window per CWnd instance allowed

		try
		{
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

			RegisterClass(wc);	// Register the window class (if not already registered)

			// Ensure this thread has the TLS index set
			TLSData* pTLSData = GetApp()->SetTlsIndex();

			// Store the CWnd pointer in thread local storage
			pTLSData->pCWnd = this;

			// Create window
			m_hWnd = ::CreateWindowEx(dwExStyle, ClassName, lpszWindowName, dwStyle, x, y, nWidth, nHeight,
									hParent, hMenu, GetApp()->GetInstanceHandle(), lpParam);

			// Now handle window creation failure
			if (!m_hWnd)
				throw CWinException(_T("Failed to Create Window"));

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
			e.what();
		}

		return m_hWnd;
	}

	inline LRESULT CWnd::FinalWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	// Pass messages on to the appropriate default window procedure
	// CMDIChild and CMDIFrame override this function
	{
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	inline void CWnd::Destroy()
	{
		// Remove TmpWnds
		if (m_IsTmpWnd)
			m_hWnd = NULL;
		else
			if (GetApp()) GetApp()->RemoveTmpWnds();
		
		if (IsWindow()) ::DestroyWindow(m_hWnd);

		// Return the CWnd to its default state
		if (m_hIconLarge) ::DestroyIcon(m_hIconLarge);
		if (m_hIconSmall) ::DestroyIcon(m_hIconSmall);
	
		if ( GetApp() ) RemoveFromMap();
		m_hIconLarge = NULL;
		m_hIconSmall = NULL;
		m_hWnd = NULL;
		m_PrevWindowProc = NULL;
		m_IsTmpWnd = FALSE;
	}

	inline HWND CWnd::Detach()
	// Reverse an Attach
	{
		assert(::IsWindow(m_hWnd));
		assert(0 != m_PrevWindowProc);	// Only a subclassed window can be detached

		::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)m_PrevWindowProc);

		// Clear member variables
		HWND hWnd = m_hWnd;

		return hWnd;
	}

	inline CWnd* CWnd::FromHandle(HWND hWnd)
	// Returns the CWnd object associated with the window handle
	{
		assert( GetApp() );
		CWnd* pWnd = GetApp()->GetCWndFromMap(hWnd);
		if (::IsWindow(hWnd) && pWnd == 0)
		{
			GetApp()->AddTmpWnd(hWnd);		
			pWnd = GetApp()->GetCWndFromMap(hWnd);
		}

		return pWnd;
	}

	inline CWnd* CWnd::GetAncestor() const
	// The GetAncestor function retrieves the handle to the ancestor (root parent)
	// of the window. Supports Win95.
	{
		assert(::IsWindow(m_hWnd));

		HWND hWnd = m_hWnd;
		HWND hWndParent = ::GetParent(hWnd);
		while (::IsChild(hWndParent, hWnd))
		{
			hWnd = hWndParent;
			hWndParent = ::GetParent(hWnd);
		}

		return FromHandle(hWnd);
	}

	inline LPCTSTR CWnd::GetClassName() const
	{
		assert(::IsWindow(m_hWnd));

		m_vTChar.assign(MAX_STRING_SIZE +1, _T('\0'));
		TCHAR* pTCharArray = &m_vTChar.front();
		::GetClassName(m_hWnd, pTCharArray, MAX_STRING_SIZE);

		return pTCharArray;
	}

	inline LPCTSTR CWnd::GetDlgItemText(int nIDDlgItem) const
	{
		assert(::IsWindow(m_hWnd));

		int nLength = ::GetWindowTextLength(::GetDlgItem(m_hWnd, nIDDlgItem));
		m_vTChar.assign(nLength +1, _T('\0'));
		TCHAR* pTCharArray = &m_vTChar.front();

		::GetDlgItemText(m_hWnd, nIDDlgItem, pTCharArray, nLength+1);

		return pTCharArray;
	}

	inline LPCTSTR CWnd::GetWindowText() const
	{
		assert(::IsWindow(m_hWnd));
		int nLength = ::GetWindowTextLength(m_hWnd);

		m_vTChar.assign(nLength+1, _T('\0'));
		TCHAR* pTCharArray = &m_vTChar.front();
		::GetWindowText(m_hWnd, pTCharArray, nLength+1);

		return pTCharArray;
	}

	inline HBITMAP CWnd::LoadBitmap(LPCTSTR lpBitmapName) const
	{
		assert (GetApp());

		HBITMAP hBitmap;

		// Try to load the bitmap from the resource handle first
		hBitmap = ::LoadBitmap(GetApp()->GetResourceHandle(), lpBitmapName);

		// The bitmap resource might be in the application's resources instead
		if (!hBitmap)
			hBitmap = ::LoadBitmap(GetApp()->GetInstanceHandle(), lpBitmapName);

		// No bitmap found, so display warning message
		if (!hBitmap)
			TRACE(_T("**WARNING** Unable to load bitmap\n"));

		return hBitmap;
	}

	inline LPCTSTR CWnd::LoadString(UINT nID)
	// Returns the string associated with a Resource ID
	{
		assert(GetApp());

		m_vTChar.assign(MAX_STRING_SIZE +1, _T('\0'));
		TCHAR* pTCharArray = &m_vTChar.front();

		if (!::LoadString (GetApp()->GetResourceHandle(), nID, pTCharArray, MAX_STRING_SIZE))
		{
			// The string resource might be in the application's resources instead
			if (!::LoadString (GetApp()->GetInstanceHandle(), nID, pTCharArray, MAX_STRING_SIZE))
			{
				TCHAR msg[80] = _T("");
				::wsprintf(msg, _T("**WARNING** LoadString - No string resource for %d\n"), nID);
				TRACE(msg);
			}
		}

		// Never return a pointer to a local variable, it is out of scope when the function returns.
		// We return a pointer to a member variable so it remains in scope.
		return pTCharArray;
	}

	inline BOOL CWnd::OnCommand(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);
		UNREFERENCED_PARAMETER(lParam);

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

		CWnd* Wnd = GetApp()->GetCWndFromMap(hWnd);

		if (Wnd != NULL)
			return Wnd->OnMessageReflect(uMsg, wParam, lParam);

		return 0L;
	}

	inline LRESULT CWnd::OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(uMsg);
		UNREFERENCED_PARAMETER(wParam);
		UNREFERENCED_PARAMETER(lParam);
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

	inline LRESULT CWnd::OnNotify(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);
		UNREFERENCED_PARAMETER(lParam);

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

	inline LRESULT CWnd::OnNotifyReflect(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);
		UNREFERENCED_PARAMETER(lParam);

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

	inline void CWnd::OnPaint(CDC& dc)
	// Called when part of the client area of the window needs to be painted
	{
         
    #ifndef __GNUC__          
		UNREFERENCED_PARAMETER(dc);
	#endif
	
	    // Override this function in your derived class to perform drawing tasks. 
	}

	inline void CWnd::OnMenuUpdate(UINT nID)
	// Called when menu items are about to be displayed
	{
		UNREFERENCED_PARAMETER(nID);

		// Override this function to modify the behaviour of menu items,
		// such as adding or removing checkmarks
	}

	inline void CWnd::PreCreate(CREATESTRUCT& cs)
	// Called by CWnd::Create to set some window parameters
	{
		// Test if Win32++ has been started
		assert(GetApp());	// Test if Win32++ has been started

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
		assert( GetApp() );

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

	inline BOOL CWnd::PreTranslateMessage(MSG* pMsg)
	{
		UNREFERENCED_PARAMETER(pMsg);

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
		assert( GetApp() );
		assert( (0 != lstrlen(wc.lpszClassName) && ( lstrlen(wc.lpszClassName) <=  MAX_STRING_SIZE) ) );

		// Check to see if this classname is already registered
		WNDCLASS wcTest = {0};
		BOOL Done = FALSE;

		if (::GetClassInfo(GetApp()->GetInstanceHandle(), wc.lpszClassName, &wcTest))
		{
			wc = wcTest;
			Done = TRUE;
		}

		if (!Done)
		{
			// Set defaults
			wc.hInstance	= GetApp()->GetInstanceHandle();
			wc.lpfnWndProc	= CWnd::StaticWindowProc;

			// Register the WNDCLASS structure
			if ( !::RegisterClass(&wc) )
				throw CWinException(_T("Failed to register window class"));

			Done = TRUE;
		}

		return Done;
	}

	inline BOOL CWnd::RemoveFromMap()
	{
		assert( GetApp() );
		BOOL Success = FALSE;

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
					Success = TRUE;
					break;
				}
			}

			pApp->m_csMapLock.Release();
		}

		return Success;
	}

	inline HICON CWnd::SetIconLarge(int nIcon)
	{
		assert( GetApp() );
		assert(::IsWindow(m_hWnd));

		m_hIconLarge = (HICON) (::LoadImage (GetApp()->GetResourceHandle(), MAKEINTRESOURCE (nIcon), IMAGE_ICON,
		::GetSystemMetrics (SM_CXICON), ::GetSystemMetrics (SM_CYICON), 0));

		if (m_hIconLarge)
			SendMessage (WM_SETICON, WPARAM (ICON_BIG), LPARAM (m_hIconLarge));
		else
			TRACE(_T("**WARNING** SetIconLarge Failed\n"));

		return m_hIconLarge;
	}

	inline HICON CWnd::SetIconSmall(int nIcon)
	{
		assert( GetApp() );
		assert(::IsWindow(m_hWnd));

		m_hIconSmall = (HICON) (::LoadImage (GetApp()->GetResourceHandle(), MAKEINTRESOURCE (nIcon), IMAGE_ICON,
		::GetSystemMetrics (SM_CXSMICON), ::GetSystemMetrics (SM_CYSMICON), 0));

		if (m_hIconSmall)
			SendMessage (WM_SETICON, WPARAM (ICON_SMALL), LPARAM (m_hIconSmall));
		else
			TRACE(_T("**WARNING** SetIconSmall Failed\n"));

		return m_hIconSmall;
	}

	inline LRESULT CALLBACK CWnd::StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	// All CWnd windows direct their messages here. This function redirects the message
	// to the CWnd's WndProc function.
	{
		assert( GetApp() );

		try
		{
			CWnd* w = GetApp()->GetCWndFromMap(hWnd);
			if (0 == w)
			{
				// The CWnd pointer wasn't found in the map, so add it now

				// Retrieve the pointer to the TLS Data
				TLSData* pTLSData = (TLSData*)TlsGetValue(GetApp()->GetTlsIndex());
				if (NULL == pTLSData)
					throw CWinException(_T("Unable to get TLS"));

				// Retrieve pointer to CWnd object from Thread Local Storage TLS
				w = pTLSData->pCWnd;
				if (NULL == w)
					throw CWinException(_T("Failed to route message"));

				pTLSData->pCWnd = NULL;

				// Store the CWnd pointer in the HWND map
				w->m_hWnd = hWnd;
				w->AddToMap();
			}

			return w->WndProc(uMsg, wParam, lParam);
		}

		catch (const CWinException &e)
		{
			// Most CWinExceptions will end up here unless caught earlier.
			e.what();
		}

		return 0L;

	} // LRESULT CALLBACK StaticWindowProc(...)

	inline void CWnd::Subclass(HWND hWnd)
	// A private function used by CreateEx, Attach and AttachDlgItem
	{
		assert(::IsWindow(hWnd));

		m_PrevWindowProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)CWnd::StaticWindowProc);
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
		LRESULT lr = 0L;

    	switch (uMsg)
		{
		case WM_COMMAND:
			{
				// Refelect this message if it's from a control
				CWnd* pWnd = GetApp()->GetCWndFromMap((HWND)lParam);
				if (pWnd != NULL)
					lr = pWnd->OnMessageReflect(uMsg, wParam, lParam);

				// Handle user commands
				if (!lr) 
					lr =  OnCommand(wParam, lParam);
				
				if (lr) return 0L;	
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
				CWnd* pWndFrom = GetApp()->GetCWndFromMap(hwndFrom);

				if (GetWindowType() != _T("CReBar"))	// Skip notification reflection for rebars to avoid double handling
				{
					if (pWndFrom != NULL)
						lr = pWndFrom->OnNotifyReflect(wParam, lParam);
					else
					{
						// Some controls (eg ListView) have child windows.
						// Reflect those notifications too.
						CWnd* pWndFromParent = GetApp()->GetCWndFromMap(::GetParent(hwndFrom));
						if (pWndFromParent != NULL)
							lr = pWndFromParent->OnNotifyReflect(wParam, lParam);
					}
				}

				// Handle user notifications
				if (!lr) lr = OnNotify(wParam, lParam);
				if (lr) return lr;
			}
			break;

		case WM_PAINT:
			{
				if (m_PrevWindowProc) break; // Allow normal painting for subclassed windows

				if (::GetUpdateRect(m_hWnd, NULL, FALSE))
				{
					::PAINTSTRUCT ps;
					CDC dc = ::BeginPaint(m_hWnd, &ps);

					OnPaint(dc);
					
					::EndPaint(m_hWnd, &ps);
					dc.DetachDC();
				}
				else
				// RedrawWindow can require repainting without an update rect
				{
					CDC dc = ::GetDC(m_hWnd);

					OnPaint(dc);
				//	::ReleaseDC(m_hWnd, hDC);
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
        assert(::IsWindow(m_hWnd));
		return ::BringWindowToTop(m_hWnd);
	}

	inline LRESULT CWnd::CallWindowProc(WNDPROC lpPrevWndFunc, UINT Msg, WPARAM wParam, LPARAM lParam) const
	{
        assert(::IsWindow(m_hWnd));
		return ::CallWindowProc(lpPrevWndFunc, m_hWnd, Msg, wParam, lParam);
	}

	inline BOOL CWnd::CheckDlgButton(int nIDButton, UINT uCheck) const
	// The CheckDlgButton function changes the check state of a button control.
	{
        assert(::IsWindow(m_hWnd));
		return ::CheckDlgButton(m_hWnd, nIDButton, uCheck);
	}

	inline HDWP CWnd::DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) const
	// The DeferWindowPos function updates the specified multiple-window – position structure for the window.
	{
        assert(::IsWindow(m_hWnd));
		return ::DeferWindowPos(hWinPosInfo, m_hWnd, hWndInsertAfter, x, y, cx, cy, uFlags);
	}

	inline HDWP CWnd::DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, const RECT& rc, UINT uFlags) const
	// The DeferWindowPos function updates the specified multiple-window – position structure for the window.
	{
		assert(::IsWindow(m_hWnd));
		return ::DeferWindowPos(hWinPosInfo, m_hWnd, hWndInsertAfter, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, uFlags);
	}

	inline LRESULT CWnd::DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam) const
	// This function provides default processing for any window messages that an application does not process.
	{
		assert(::IsWindow(m_hWnd));
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	inline BOOL CWnd::DrawMenuBar() const
	{
		assert(::IsWindow(m_hWnd));
		return ::DrawMenuBar(m_hWnd);
	}

	inline BOOL CWnd::EnableWindow(BOOL bEnable /*= TRUE*/) const
	// The EnableWindow function enables or disables mouse and
	// keyboard input to the window.
	{
		assert(::IsWindow(m_hWnd));
		return ::EnableWindow(m_hWnd, bEnable);
	}

	inline CWnd* CWnd::GetActiveWindow() const
	{
		return FromHandle( ::GetActiveWindow() );
	}

	inline CWnd* CWnd::GetCapture() const
	{
		return FromHandle( ::GetCapture() );
	}

	inline ULONG_PTR CWnd::GetClassLongPtr(int nIndex) const
	// The GetClassLongPtr function retrieves the specified value from the
	// WNDCLASSEX structure associated with the window.
	{
		assert(::IsWindow(m_hWnd));
		return ::GetClassLongPtr(m_hWnd, nIndex);
	}

	inline CRect CWnd::GetClientRect() const
	// The GetClientRect function retrieves the coordinates of a window's client area.
	// The client coordinates specify the upper-left and lower-right corners of the
	// client area. Because client coordinates are relative to the upper-left corner
	// of a window's client area, the coordinates of the upper-left corner are (0,0).
	{
		assert(::IsWindow(m_hWnd));
		CRect rc;
		::GetClientRect(m_hWnd, &rc);
		return rc;
	}

	inline HDC CWnd::GetDC() const
	// The GetDC function retrieves a handle to a display device context (DC) for the
	// client area of the window.
	{
		assert(::IsWindow(m_hWnd));
		return ::GetDC(m_hWnd);
	}

	inline HDC CWnd::GetDCEx(HRGN hrgnClip, DWORD flags) const
	// The GetDCEx function retrieves a handle to a display device context (DC) for the
	// client area or entire area of a window
	{
		assert(::IsWindow(m_hWnd));
		return ::GetDCEx(m_hWnd, hrgnClip, flags);
	}

	inline CWnd* CWnd::GetDlgItem(int nIDDlgItem) const
	// The GetDlgItem function retrieves a handle to a control in the dialog box.
	{
		assert(::IsWindow(m_hWnd));
		return FromHandle( ::GetDlgItem(m_hWnd, nIDDlgItem) );
	}

	inline UINT CWnd::GetDlgItemInt(int nIDDlgItem, BOOL* lpTranslated, BOOL bSigned) const
	{
		assert(::IsWindow(m_hWnd));
		return ::GetDlgItemInt(m_hWnd, nIDDlgItem, lpTranslated, bSigned);
	}

	inline CWnd* CWnd::GetFocus() const
	{
		return FromHandle( ::GetFocus() );
	}

	inline HFONT CWnd::GetFont() const
	// Retrieves the font with which the window is currently drawing its text.
	{
		assert(::IsWindow(m_hWnd));
		return (HFONT)SendMessage(WM_GETFONT, 0, 0);
	}

	inline HICON CWnd::GetIcon(BOOL bBigIcon) const
	// Retrieves a handle to the large or small icon associated with a window.
	{
		assert(::IsWindow(m_hWnd));
		return (HICON)SendMessage(WM_GETICON, (WPARAM)bBigIcon, 0);
	}

	inline CWnd* CWnd::GetParent() const
	{
		assert(::IsWindow(m_hWnd));
		return FromHandle( ::GetParent(m_hWnd) );
	}

	inline LONG_PTR CWnd::GetWindowLongPtr(int nIndex) const
	// The GetWindowLongPtr function retrieves information about the window.
	{
		assert(::IsWindow(m_hWnd));
		return ::GetWindowLongPtr(m_hWnd, nIndex);
	}

	inline BOOL CWnd::GetScrollInfo(int fnBar, SCROLLINFO& si) const
	// The GetScrollInfo function retrieves the parameters of a scroll bar, including
	// the minimum and maximum scrolling positions, the page size, and the position
	// of the scroll box (thumb).
	{
		assert(::IsWindow(m_hWnd));
		return ::GetScrollInfo(m_hWnd, fnBar, &si);
	}

	inline CWnd* CWnd::GetWindow(UINT uCmd) const
	// The GetWindow function retrieves a handle to a window that has the specified
	// relationship (Z-Order or owner) to the specified window.
	// Possible uCmd values: GW_CHILD, GW_ENABLEDPOPUP, GW_HWNDFIRST, GW_HWNDLAST,
	// GW_HWNDNEXT, GW_HWNDPREV, GW_OWNER
	{
		assert(::IsWindow(m_hWnd));
		return FromHandle( ::GetWindow(m_hWnd, uCmd) );
	}

	inline HDC CWnd::GetWindowDC() const
	// The GetWindowDC function retrieves the device context (DC) for the entire
	// window, including title bar, menus, and scroll bars.
	{
		assert(::IsWindow(m_hWnd));
		return ::GetWindowDC(m_hWnd);
	}

	inline CRect CWnd::GetWindowRect() const
	// retrieves the dimensions of the bounding rectangle of the window.
	// The dimensions are given in screen coordinates that are relative to the
	// upper-left corner of the screen.
	{
		assert(::IsWindow(m_hWnd));
		CRect rc;
		::GetWindowRect(m_hWnd, &rc);
		return rc;
	}

	inline int CWnd::GetWindowTextLength() const
	{
		assert(::IsWindow(m_hWnd));
		return ::GetWindowTextLength(m_hWnd);
	}

	inline void CWnd::Invalidate(BOOL bErase /*= TRUE*/) const
	// The Invalidate function adds the entire client area the window's update region.
	// The update region represents the portion of the window's client area that must be redrawn.
	{
		assert(::IsWindow(m_hWnd));
		::InvalidateRect(m_hWnd, NULL, bErase);
	}

	inline BOOL CWnd::InvalidateRect(LPCRECT lpRect, BOOL bErase /*= TRUE*/) const
	// The InvalidateRect function adds a rectangle to the window's update region.
	// The update region represents the portion of the window's client area that must be redrawn.
	{
		assert(::IsWindow(m_hWnd));
		return ::InvalidateRect(m_hWnd, lpRect, bErase);
	}

	inline BOOL CWnd::InvalidateRgn(CONST HRGN hRgn, BOOL bErase /*= TRUE*/) const
	// The InvalidateRgn function invalidates the client area within the specified region
	// by adding it to the current update region of a window. The invalidated region,
	// along with all other areas in the update region, is marked for painting when the
	// next WM_PAINT message occurs.
	{
		assert(::IsWindow(m_hWnd));
		return ::InvalidateRgn(m_hWnd, hRgn, bErase);
	}

	inline BOOL CWnd::IsChild(CWnd* pChild) const
	// The IsChild function tests whether a window is a child window or descendant window
	// of a parent window's CWnd.
	{
		assert(::IsWindow(m_hWnd));
		return ::IsChild(m_hWnd, pChild->GetHwnd());
	}

	inline BOOL CWnd::IsWindowEnabled() const
	// The IsWindowEnabled function determines whether the window is enabled
	// for mouse and keyboard input.
	{
		assert(::IsWindow(m_hWnd));
		return ::IsWindowEnabled(m_hWnd);
	}

	inline BOOL CWnd::IsWindowVisible() const
	// The IsWindowVisible function retrieves the visibility state of the window.
	{
		assert(::IsWindow(m_hWnd));
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
		assert(::IsWindow(m_hWnd));
		return ::MessageBox(m_hWnd, lpText, lpCaption, uType);
	}

	inline BOOL CWnd::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint /* = TRUE*/) const
	// The MoveWindow function changes the position and dimensions of the window.
	{
		assert(::IsWindow(m_hWnd));
		return ::MoveWindow(m_hWnd, x, y, nWidth, nHeight, bRepaint = TRUE);
	}

	inline BOOL CWnd::MoveWindow(const RECT& rc, BOOL bRepaint /* = TRUE*/) const
	// The MoveWindow function changes the position and dimensions of the window.
	{
		assert(::IsWindow(m_hWnd));
		return ::MoveWindow(m_hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, bRepaint);
	}

	inline BOOL CWnd::PostMessage(UINT uMsg, WPARAM wParam /*= 0L*/, LPARAM lParam /*= 0L*/) const
	// The PostMessage function places (posts) a message in the message queue
	// associated with the thread that created the window and returns without
	// waiting for the thread to process the message.
	{
		assert(::IsWindow(m_hWnd));
		return ::PostMessage(m_hWnd, uMsg, wParam, lParam);
	}

	inline BOOL CWnd::PostMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
	// Required by by some macros
	{
		assert(::IsWindow(m_hWnd));
		return ::PostMessage(hWnd, uMsg, wParam, lParam);
	}

	inline BOOL CWnd::RedrawWindow(LPCRECT lpRectUpdate, HRGN hRgn, UINT flags) const
	// The RedrawWindow function updates the specified rectangle or region in a window's client area.
	{
		assert(::IsWindow(m_hWnd));
		return ::RedrawWindow(m_hWnd, lpRectUpdate, hRgn, flags);
	}

	inline int CWnd::ReleaseDC(HDC hDC) const
	// The ReleaseDC function releases a device context (DC), freeing it for use
	// by other applications.
	{
		assert(::IsWindow(m_hWnd));
		return ::ReleaseDC(m_hWnd, hDC);
	}

	inline LRESULT CWnd::SendDlgItemMessage(int nIDDlgItem, UINT Msg, WPARAM wParam, LPARAM lParam) const
	// The SendDlgItemMessage function sends a message to the specified control in a dialog box.
	{
		assert(::IsWindow(m_hWnd));
		return ::SendDlgItemMessage(m_hWnd, nIDDlgItem, Msg, wParam, lParam);
	}

	inline LRESULT CWnd::SendMessage(UINT uMsg, WPARAM wParam /*= 0L*/, LPARAM lParam /*= 0L*/) const
	// The SendMessage function sends the specified message to a window or windows.
	// It calls the window procedure for the window and does not return until the
	// window procedure has processed the message.
	{
		assert(::IsWindow(m_hWnd));
		return ::SendMessage(m_hWnd, uMsg, wParam, lParam);
	}

	inline LRESULT CWnd::SendMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
	// Required by by some macros
	{
		assert(::IsWindow(m_hWnd));
		return ::SendMessage(hWnd, uMsg, wParam, lParam);
	}

	inline CWnd* CWnd::SetActiveWindow() const
	// The SetActiveWindow function activates the window, but
	// not if the application is in the background.
	{
		assert(::IsWindow(m_hWnd));
		return FromHandle( ::SetActiveWindow(m_hWnd) );
	}

	inline CWnd* CWnd::SetCapture() const
	// The SetCapture function sets the mouse capture to the window.
	// SetCapture captures mouse input either when the mouse is over the capturing
	// window, or when the mouse button was pressed while the mouse was over the
	// capturing window and the button is still down.
	{
		assert(::IsWindow(m_hWnd));
		return FromHandle( ::SetCapture(m_hWnd) );
	}

	inline ULONG_PTR CWnd::SetClassLongPtr(int nIndex, LONG_PTR dwNewLong) const
	// The SetClassLongPtr function replaces the specified value at the specified offset in the
	// extra class memory or the WNDCLASSEX structure for the class to which the window belongs.
	{
		assert(::IsWindow(m_hWnd));
		return ::SetClassLongPtr(m_hWnd, nIndex, dwNewLong);
	}

	inline CWnd* CWnd::SetFocus() const
	// The SetFocus function sets the keyboard focus to the window.
	{
		assert(::IsWindow(m_hWnd));
	//	FromHandle( ::SetFocus(m_hWnd) );
		return GetApp()->GetCWndFromMap( ::SetFocus(m_hWnd) );
	}

	inline void CWnd::SetFont(HFONT hFont, BOOL bRedraw) const
	// Specifies the font that the window will use when drawing text.
	{
		assert(::IsWindow(m_hWnd));
		SendMessage(WM_SETFONT, (WPARAM)hFont, (LPARAM)bRedraw);
	}

	inline HICON CWnd::SetIcon(HICON hIcon, BOOL bBigIcon) const
	// Associates a new large or small icon with a window.
	{
		assert(::IsWindow(m_hWnd));
		return (HICON)SendMessage(WM_SETICON, (WPARAM)bBigIcon, (LPARAM)hIcon);
	}

	inline BOOL CWnd::SetForegroundWindow() const
	// The SetForegroundWindow function puts the thread that created the window into the
	// foreground and activates the window.
	{
		assert(::IsWindow(m_hWnd));
		return ::SetForegroundWindow(m_hWnd);
	}

	inline CWnd* CWnd::SetParent(CWnd* pWndParent) const
	// The SetParent function changes the parent window of the child window.
	{
		assert(::IsWindow(m_hWnd));
		if (pWndParent)
		{
			HWND hParent = pWndParent->GetHwnd();
			return FromHandle(::SetParent(m_hWnd, hParent));
		}
		else
			return FromHandle(::SetParent(m_hWnd, 0));
	}

	inline BOOL CWnd::SetRedraw(BOOL bRedraw /*= TRUE*/) const
	// This function allows changes in that window to be redrawn or prevents changes
	// in that window from being redrawn.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)::SendMessage(m_hWnd, WM_SETREDRAW, (WPARAM)bRedraw, 0L);
	}

	inline LONG_PTR CWnd::SetWindowLongPtr(int nIndex, LONG_PTR dwNewLong) const
	// The SetWindowLongPtr function changes an attribute of the window.
	{
		assert(::IsWindow(m_hWnd));
		return ::SetWindowLongPtr(m_hWnd, nIndex, dwNewLong);
	}

	inline BOOL CWnd::SetWindowPos(HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) const
	// The SetWindowPos function changes the size, position, and Z order of a child, pop-up,
	// or top-level window.
	{
		assert(::IsWindow(m_hWnd));
		return ::SetWindowPos(m_hWnd, hWndInsertAfter, x, y, cx, cy, uFlags);
	}

	inline BOOL CWnd::SetWindowPos(HWND hWndInsertAfter, const RECT& rc, UINT uFlags) const
	// The SetWindowPos function changes the size, position, and Z order of a child, pop-up,
	// or top-level window.
	{
		assert(::IsWindow(m_hWnd));
		return ::SetWindowPos(m_hWnd, hWndInsertAfter, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, uFlags);
	}

	inline int CWnd::SetWindowRgn(HRGN hRgn, BOOL bRedraw /*= TRUE*/) const
	// The SetWindowRgn function sets the window region of the window.
	// The window region determines the area within the window where the system permits drawing.
	{
		assert(::IsWindow(m_hWnd));
		return ::SetWindowRgn(m_hWnd, hRgn, bRedraw);
	}

	inline BOOL CWnd::SetDlgItemInt(int nIDDlgItem, UINT uValue, BOOL bSigned) const
	{
		assert(::IsWindow(m_hWnd));
		return ::SetDlgItemInt(m_hWnd, nIDDlgItem, uValue, bSigned);
	}

	inline BOOL CWnd::SetDlgItemText(int nIDDlgItem, LPCTSTR lpString) const
	// The SetDlgItemText function sets the title or text of a control in a dialog box.
	{
		assert(::IsWindow(m_hWnd));
		return ::SetDlgItemText(m_hWnd, nIDDlgItem, lpString);
	}

	inline BOOL CWnd::SetWindowText(LPCTSTR lpString) const
	// The SetWindowText function changes the text of the window's title bar (if it has one).
	{
		assert(::IsWindow(m_hWnd));
		return ::SetWindowText(m_hWnd, lpString);
	}

	inline HRESULT CWnd::SetWindowTheme(LPCWSTR pszSubAppName, LPCWSTR pszSubIdList) const
	// Set the XP Theme for a window.
	// Exampes:
	//  SetWindowTheme(NULL, NULL);		// Reverts the window's XP theme back to default
	//  SetWindowTheme(L" ", L" ");		// Disables XP theme for the window
	{
		HRESULT hr = E_NOTIMPL;

#ifndef	_WIN32_WCE

		HMODULE hMod = ::LoadLibrary(_T("uxtheme.dll"));
		if(hMod)
		{
			typedef HRESULT (__stdcall *PFNSETWINDOWTHEME)(HWND hWnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList);
			PFNSETWINDOWTHEME pfn = (PFNSETWINDOWTHEME)GetProcAddress(hMod, "SetWindowTheme");

			hr = (*pfn)(m_hWnd, pszSubAppName, pszSubIdList);

			::FreeLibrary(hMod);
		}

#endif

		return hr;
	}

	inline BOOL CWnd::ShowWindow(int nCmdShow /*= SW_SHOWNORMAL*/) const
	// The ShowWindow function sets the window's show state.
	{
		assert(::IsWindow(m_hWnd));
		return ::ShowWindow(m_hWnd, nCmdShow);
	}

	inline BOOL CWnd::UpdateWindow() const
	// The UpdateWindow function updates the client area of the window by sending a
	// WM_PAINT message to the window if the window's update region is not empty.
	// If the update region is empty, no message is sent.
	{
		assert(::IsWindow(m_hWnd));
		return ::UpdateWindow(m_hWnd);
	}

	inline BOOL CWnd::ValidateRect(LPCRECT prc) const
	// The ValidateRect function validates the client area within a rectangle by
	// removing the rectangle from the update region of the window.
	{
		assert(::IsWindow(m_hWnd));
		return ::ValidateRect(m_hWnd, prc);
	}

	inline BOOL CWnd::ValidateRgn(HRGN hRgn) const
	// The ValidateRgn function validates the client area within a region by
	// removing the region from the current update region of the window.
	{
		assert(::IsWindow(m_hWnd));
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
		assert(::IsWindow(m_hWnd));
		return ::CloseWindow(m_hWnd);
	}

	inline BOOL CWnd::EnableScrollBar(UINT uSBflags, UINT uArrows) const
	{
		assert(::IsWindow(m_hWnd));
		return ::EnableScrollBar(m_hWnd, uSBflags, uArrows);
	}

	inline CWnd* CWnd::GetLastActivePopup() const
	{
		assert(::IsWindow(m_hWnd));
		return FromHandle( ::GetLastActivePopup(m_hWnd) );
	}

	inline HMENU CWnd::GetMenu() const
	// The GetMenu function retrieves a handle to the menu assigned to the window.
	{
		assert(::IsWindow(m_hWnd));
		return ::GetMenu(m_hWnd);
	}

	inline int CWnd::GetScrollPos(int nBar) const
	// The GetScrollPos function retrieves the current position of the scroll box
	// (thumb) in the specified scroll bar.
	{
		assert(::IsWindow(m_hWnd));
		return ::GetScrollPos(m_hWnd, nBar);
	}

	inline BOOL CWnd::GetScrollRange(int nBar, int& MinPos, int& MaxPos) const
	// The GetScrollRange function retrieves the current minimum and maximum scroll box
	// (thumb) positions for the specified scroll bar.
	{
		assert(::IsWindow(m_hWnd));
		return ::GetScrollRange(m_hWnd, nBar, &MinPos, &MaxPos );
	}

	inline CWnd* CWnd::GetTopWindow() const
	{
		assert(::IsWindow(m_hWnd));
		return FromHandle( ::GetTopWindow(m_hWnd) );
	}

	inline BOOL CWnd::GetWindowPlacement(WINDOWPLACEMENT& wndpl) const
	// The GetWindowPlacement function retrieves the show state and the restored,
	// minimized, and maximized positions of the window.
	{
		assert(::IsWindow(m_hWnd));
		return ::GetWindowPlacement(m_hWnd, &wndpl);
	}

	inline BOOL CWnd::IsIconic() const
	// The IsIconic function determines whether the window is minimized (iconic).
	{
		assert(::IsWindow(m_hWnd));
		return ::IsIconic(m_hWnd);
	}

	inline BOOL CWnd::IsZoomed() const
	// The IsZoomed function determines whether the window is maximized.
	{
		assert(::IsWindow(m_hWnd));
		return ::IsZoomed(m_hWnd);
	}

	inline BOOL CWnd::KillTimer(UINT_PTR uIDEvent) const
	// Destroys the specified timer.
	{
		assert(::IsWindow(m_hWnd));
		return ::KillTimer(m_hWnd, uIDEvent);
	}

	inline BOOL CWnd::LockWindowUpdate() const
	// Disables drawing in the window. Only one window can be locked at a time.
	// Use UnLockWindowUpdate to re-enable drawing in the window
	{
		assert(::IsWindow(m_hWnd));
		return ::LockWindowUpdate(m_hWnd);
	}

	inline BOOL CWnd::OpenIcon() const
	{
		assert(::IsWindow(m_hWnd));
		return ::OpenIcon(m_hWnd);
	}

	inline BOOL CWnd::ScrollWindow(int XAmount, int YAmount, LPCRECT prcScroll, LPCRECT prcClip) const
	{
		assert(::IsWindow(m_hWnd));
		return ::ScrollWindow(m_hWnd, XAmount, YAmount, prcScroll, prcClip);
	}

	inline int CWnd::ScrollWindowEx(int dx, int dy, LPCRECT prcScroll, LPCRECT prcClip, HRGN hrgnUpdate, LPRECT prcUpdate, UINT flags) const
	// The ScrollWindow function scrolls the contents of the window's client area.
	{
		assert(::IsWindow(m_hWnd));
		return ::ScrollWindowEx(m_hWnd, dx, dy, prcScroll, prcClip, hrgnUpdate, prcUpdate, flags);
	}

	inline BOOL CWnd::SetMenu(HMENU hMenu) const
	// The SetMenu function assigns a new menu to the specified window.
	{
		assert(::IsWindow(m_hWnd));
		return ::SetMenu(m_hWnd, hMenu);
	}

	inline int CWnd::SetScrollInfo(int fnBar, const SCROLLINFO& si, BOOL fRedraw) const
	// The SetScrollInfo function sets the parameters of a scroll bar, including
	// the minimum and maximum scrolling positions, the page size, and the
	// position of the scroll box (thumb).
	{
		assert(::IsWindow(m_hWnd));
		return ::SetScrollInfo(m_hWnd, fnBar, &si, fRedraw);
	}

	inline int CWnd::SetScrollPos(int nBar, int nPos, BOOL bRedraw) const
	// The SetScrollPos function sets the position of the scroll box (thumb) in
	// the specified scroll bar.
	{
		assert(::IsWindow(m_hWnd));
		return ::SetScrollPos(m_hWnd, nBar, nPos, bRedraw);
	}

	inline BOOL CWnd::SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw) const
	// The SetScrollRange function sets the minimum and maximum scroll box positions for the scroll bar.
	{
		assert(::IsWindow(m_hWnd));
		return ::SetScrollRange(m_hWnd, nBar, nMinPos, nMaxPos, bRedraw);
	}

	inline UINT_PTR CWnd::SetTimer(UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc) const
	// Creates a timer with the specified time-out value.
	{
		assert(::IsWindow(m_hWnd));
		return ::SetTimer(m_hWnd, nIDEvent, uElapse, lpTimerFunc);
	}

	inline BOOL CWnd::SetWindowPlacement(const WINDOWPLACEMENT& wndpl) const
	// The SetWindowPlacement function sets the show state and the restored, minimized,
	// and maximized positions of the window.
	{
		assert(::IsWindow(m_hWnd));
		return ::SetWindowPlacement(m_hWnd, &wndpl);
	}

	inline BOOL CWnd::ShowOwnedPopups(BOOL fShow) const
	{
		assert(::IsWindow(m_hWnd));
		return ::ShowOwnedPopups(m_hWnd, fShow);
	}

	inline BOOL CWnd::ShowScrollBar(int nBar, BOOL bShow) const
	{
		assert(::IsWindow(m_hWnd));
		return ::ShowScrollBar(m_hWnd, nBar, bShow);
	}

	inline BOOL CWnd::ShowWindowAsync(int nCmdShow) const
	{
		assert(::IsWindow(m_hWnd));
		return ::ShowWindowAsync(m_hWnd, nCmdShow);
	}

	inline BOOL CWnd::UnLockWindowUpdate() const
	// Enables drawing in the window. Only one window can be locked at a time.
	// Use LockWindowUpdate to disable drawing in the window
	{
		assert(::IsWindow(m_hWnd));
		return ::LockWindowUpdate(0);
	}

	inline CWnd* CWnd::WindowFromDC(HDC hDC) const
	{
		return FromHandle( ::WindowFromDC(hDC) );
	}

  #endif

}; // namespace Win32xx


#endif // _WINCORE_H_

