// Win32++   Version 7.9 alpha
// Release Date: TBA
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2015  David Nash
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
//  CWinThread, CWinApp, CWnd, CWinException,
//  CCriticalSection.
//
// This file contains the declarations for the core set of classes required to
// create simple windows using Win32++.
//
// 1) CCriticalSection: This class is used internally to manage thread access
//            to shared resources. You can also use this class to lock and
//            release your own critical sections.
//
// 2) CWinException: This class is used internally by Win32++ to handle
//            exceptions. You can also use it to throw and catch exceptions.
//
// 3) CWinApp: This class is used start Win32++ and run the message loop. You
//            should inherit from this class to start Win32++ in your own
//            application.
//
// 4) CWinThread: This class is the parent class for CWinApp. It is also the
//            class used to create additional GUI and worker threads.
//
// 5) CWnd:   This class is used to represent a window. It provides a means
//            of creating the window, and handling its messages. Inherit
//            from this class to define and control windows.
//
//
// Note: This header file (or another Win32++ header file which includes it)
//       should be included before all other header files. It sets some
//       important macros which need to be set before including Windows.h
//       Including this file first also allows it to disable some pointless
//       warning messages (see below).



#ifndef _WIN32XX_WINCORE_H_
#define _WIN32XX_WINCORE_H_


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
  #pragma option -w-8026            // functions with exception specifications are not expanded inline
  #pragma option -w-8027		    // function not expanded inline
  #pragma option -w-8030			// Temporary used for 'rhs'
  #define STRICT 1
#endif

#ifdef __GNUC__
  #pragma GCC diagnostic ignored "-Wmissing-braces"
#endif

#ifdef _WIN32_WCE
  #include "wcestddef.h"
#endif


#include <assert.h>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <winsock2.h>
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <tchar.h>
#ifndef _WIN32_WCE
  #include <shlwapi.h>
  #include <process.h>
#endif
#include "shared_ptr.h"
//#include "winutils.h"			// included later in this file
//#include "c_string.h"			// included later in this file
//#include "gdi.h"				// included later in this file
//#include "menu.h"				// included later in this file
//#include "imagelist.h"		// included later in this file


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
// WM_APP range: 0x8000 through 0xBFFF
#define UWM_DOCKACTIVATE     (WM_APP + 0x3F01)  // Message - sent to dock ancestor when a docker is activated or deactivated.
#define UWM_DOCKDESTROYED	 (WM_APP + 0x3F02)	// Message - posted when docker is destroyed
#define UWM_DRAWRBBKGND      (WM_APP + 0x3F03)	// Message - sent by rebar to parent to perform background drawing. Return TRUE if handled.
#define UWM_DRAWSBBKGND      (WM_APP + 0x3F04)	// Message - sent by statusbar to parent to perform background drawing. Return TRUE if handled.
#define UWM_GETFRAMEVIEW     (WM_APP + 0x3F05)  // Message - returns the HWND of the frame's view window
#define UWM_GETMBTHEME       (WM_APP + 0x3F06)	// Message - returns a pointer to MenuBarTheme
#define UWM_GETRBTHEME       (WM_APP + 0x3F07)	// Message - returns a pointer to ReBarTheme
#define UWM_GETSBTHEME       (WM_APP + 0x3F08)	// Message - returns a pointer to StatusBarTheme
#define UWM_GETTBTHEME       (WM_APP + 0x3F09)  // Message - returns a pointer to ToolBarTheme
#define UWM_POPUPMENU		 (WM_APP + 0x3F0A)	// Message - creates the menubar popup menu
#define UWM_TBRESIZE         (WM_APP + 0x3F0B)  // Message - sent by toolbar to parent. Used by the rebar
#define UWM_TBWINPOSCHANGING (WM_APP + 0x3F0C)	// Message - sent to parent. Toolbar is resizing
#define UWM_UPDATECOMMAND    (WM_APP + 0x3F0D)  // Message - sent before a menu is displayed. Used by OnUpdate
#define UWN_BARSTART		 (WM_APP + 0x3F0E)	// Notification - docker bar selected for move
#define UWN_BARMOVE			 (WM_APP + 0x3F0F)	// Notification - docker bar moved
#define UWN_BAREND			 (WM_APP + 0x3F10)	// Notification - end of docker bar move
#define UWN_DOCKSTART		 (WM_APP + 0x3F11)	// Notification - about to start undocking
#define UWN_DOCKMOVE		 (WM_APP + 0x3F12)	// Notification - undocked docker is being moved
#define UWN_DOCKEND			 (WM_APP + 0x3F13)	// Notification - docker has been docked
#define UWN_TABCHANGED       (WM_APP + 0x3F14)	// Notification - tab size or position changed
#define UWN_TABDRAGGED       (WM_APP + 0x3F15)	// Notification - tab is being dragged
#define UWN_UNDOCKED		 (WM_APP + 0x3F16)	// Notification - sent by docker when undocked

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
	// Registered messages defined by Win32++
	const UINT UWM_CLEANUPTEMPS = RegisterWindowMessage(_T("UWM_CLEANUPTEMPS")); // Message - posted to cleanup temporary CDCs, CWnds etc.
	const UINT UWM_WINDOWCREATED = RegisterWindowMessage(_T("UWM_WINDOWCREATED"));	// Message - posted when a window is created or attached.

	////////////////////////////////////////////////
	// Forward declarations.
	//  These classes are defined later or elsewhere
	class CDC;
	class CGDIObject;
	class CMenu;
	class CMenuBar;
	class CWinApp;
	class CWnd;
	class CBitmap;
	class CBrush;
	class CFont;
	class CImageList;
	class CPalette;
	class CPen;
	class CRgn;

	// tString is a TCHAR std::string
	typedef std::basic_string<TCHAR> tString;
#ifndef _WIN32_WCE
	typedef std::basic_stringstream<TCHAR> tStringStream;
#endif

	// Some useful smart pointers
	typedef Shared_Ptr<CDC> DCPtr;
	typedef Shared_Ptr<CGDIObject> GDIPtr;
	typedef Shared_Ptr<CMenu> MenuPtr;
	typedef Shared_Ptr<CWnd> WndPtr;
	typedef Shared_Ptr<CBitmap> BitmapPtr;
	typedef Shared_Ptr<CBrush> BrushPtr;
	typedef Shared_Ptr<CFont> FontPtr;
	typedef Shared_Ptr<CImageList> ImageListPtr;
	typedef Shared_Ptr<CPalette> PalettePtr;
	typedef Shared_Ptr<CPen> PenPtr;
	typedef Shared_Ptr<CRgn> RgnPtr;

	enum Constants			// Defines the maximum size for TCHAR strings
	{
		MAX_MENU_STRING = 80,
		MAX_STRING_SIZE = 255
	};

	struct CDC_Data	// A structure that contains the data members for CDC
	{
		// Constructor
		CDC_Data() : hDC(0), Count(1L), IsManagedHDC(FALSE), hWnd(0), nSavedDCState(0) {}
		
		std::vector<GDIPtr> m_vGDIObjects;	// Smart pointers to internally created Bitmaps, Brushes, Fonts, Bitmaps and Regions
		HDC		hDC;			// The HDC belonging to this CDC
		long	Count;			// Reference count
		bool	IsManagedHDC;	// Delete/Release the HDC on destruction
		HWND	hWnd;			// The HWND of a Window or Client window DC
		int		nSavedDCState;	// The save state of the HDC.
	};

	struct CGDI_Data	// A structure that contains the data members for CGDIObject
	{
		// Constructor
		CGDI_Data() : hGDIObject(0), Count(1L), IsManagedObject(FALSE) {}
		
		HGDIOBJ hGDIObject;
		long	Count;
		bool	IsManagedObject;
	};

	struct CIml_Data	// A structure that contains the data members for CImageList
	{
		// Constructor
		CIml_Data() : hImageList(0), IsManagedHiml(FALSE), Count(1L) {}

		HIMAGELIST	hImageList;
		bool		IsManagedHiml;
		long		Count;
	};

	struct CMenu_Data	// A structure that contains the data members for CMenu
	{
		// Constructor
		CMenu_Data() : hMenu(0), IsManagedMenu(FALSE), Count(1L) {}

		std::vector<MenuPtr> vSubMenus;	// A vector of smart pointers to CMenu
		HMENU hMenu;
		bool IsManagedMenu;
		long Count;
	};	

	struct CompareHDC		// The comparison function object used by CWinApp::m_mapHDC
	{
		bool operator()(HDC const a, const HDC b) const
			{return ((DWORD_PTR)a < (DWORD_PTR)b);}
	};

	struct CompareGDI		// The comparison function object used by CWinApp::m_mapGDI
	{
		bool operator()(HGDIOBJ const a, const HGDIOBJ b) const
			{return ((DWORD_PTR)a < (DWORD_PTR)b);}
	};

	struct CompareHIMAGELIST // The comparison function object used by CWinApp::m_mapHIMAGELIST
	{
		bool operator()(HIMAGELIST const a, const HIMAGELIST b) const
			{return ((DWORD_PTR)a < (DWORD_PTR)b);}
	};

	struct CompareHMENU		// The comparison function object used by CWinApp::m_mapHMENU
	{
		bool operator()(HMENU const a, const HMENU b) const
			{return ((DWORD_PTR)a < (DWORD_PTR)b);}
	};

	struct CompareHWND		// The comparison function object used by CWinApp::m_mapHWND
	{
		bool operator()(HWND const a, const HWND b) const
			{return ((DWORD_PTR)a < (DWORD_PTR)b);}
	};

	struct TLSData			// Used for Thread Local Storage (TLS)
	{
		CWnd* pWnd;			// pointer to CWnd object for Window creation
		CWnd* pMainWnd;		// pointer to the main window for the thread (usually CFrame)
		CMenuBar* pMenuBar;	// pointer to CMenuBar object used for the WH_MSGFILTER hook
		HHOOK hMsgHook;		// WH_MSGFILTER hook for CMenuBar and Modal Dialogs
		long nDlgHooks;		// Number of Dialog MSG hooks

		std::map<HDC, DCPtr, CompareHDC> TmpDCs;		// Temporary CDC pointers
		std::map<HGDIOBJ, GDIPtr, CompareGDI> TmpGDIs;	// Temporary CGDIObject pointers
		std::map<HIMAGELIST, ImageListPtr, CompareHIMAGELIST> TmpImageLists;	// Temporary CImageList pointers
		std::map<HWND, WndPtr, CompareHWND> TmpWnds;	// Temporary CWnd pointers
		TLSData() : pWnd(0), pMainWnd(0), pMenuBar(0), hMsgHook(0), nDlgHooks(0) {}	// Constructor

#ifndef _WIN32_WCE
		std::map<HMENU, MenuPtr, CompareHMENU> TmpMenus;	// Temporary CMenu pointers
#endif
	};


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
		CCriticalSection& operator = ( const CCriticalSection& );

		CRITICAL_SECTION m_cs;
	};


	////////////////////////////////////////
	// Declaration of the CWinException class
	//
	// Note: Each function guarantees not to throw an exception

	class CWinException : public std::exception
	{
	public:
		CWinException(LPCTSTR pszText) throw ();
		~CWinException() throw() {}
		DWORD GetError() const throw ();
		LPCTSTR GetErrorString() const throw ();
		const char * what () const throw ();

	private:
		DWORD  m_Error;
		LPCTSTR m_pszText;
		TCHAR m_szErrorString[MAX_STRING_SIZE];
	};

	// Typedef for _beginthreadex's callback function
	typedef UINT (WINAPI *PFNTHREADPROC)(LPVOID);

	//////////////////////////////////////
	// Declaration of the CWinThread class
	//
	class CWinThread
	{
	public:
		CWinThread();
		CWinThread(PFNTHREADPROC pfnThreadProc, LPVOID pParam);
		virtual ~CWinThread();

		// Overridables
		virtual BOOL InitInstance();
		virtual int MessageLoop();
		virtual BOOL OnIdle(LONG lCount);
		virtual BOOL PreTranslateMessage(MSG Msg);

		// Operations
		void	CreateThread(unsigned initflag = 0, unsigned stack_size = 0, LPSECURITY_ATTRIBUTES pSecurityAttributes = NULL);
		HANDLE	GetThread()	const;
		int		GetThreadID() const;
		int		GetThreadPriority() const;
		DWORD	ResumeThread() const;
		HACCEL	GetAccelerators() const { return m_hAccel; }
		CWnd*	GetAcceleratorsWindow() const { return m_pWndAccel; }
		void	SetAccelerators(HACCEL hAccel, CWnd* pWndAccel);
		BOOL	SetThreadPriority(int nPriority) const;
		DWORD	SuspendThread() const;

	private:
		CWinThread(const CWinThread&);				// Disable copy construction
		CWinThread& operator = (const CWinThread&);	// Disable assignment operator

		void CleanupTemps();
		static	UINT WINAPI StaticThreadProc(LPVOID pCThread);

		PFNTHREADPROC m_pfnThreadProc;	// Callback function for worker threads
		LPVOID m_pThreadParams;			// Thread parameter for worker threads
		HANDLE m_hThread;				// Handle of this thread
		UINT m_nThreadID;				// ID of this thread
		DWORD m_dwThreadID;				// ID of this thread
		HACCEL m_hAccel;				// handle to the accelerator table
		CWnd* m_pWndAccel;				// pointer to the window for accelerator keys

	};

	///////////////////////////////////
	// Declaration of the CWinApp class
	//
	class CWinApp : public CWinThread
	{
		// Provide these access to CWinApp's private members:
		friend class CDC;
		friend class CDialog;
		friend class CFrame;
		friend class CGDIObject;
		friend class CImageList;
		friend class CMenu;
		friend class CMenuBar;
		friend class CPropertyPage;
		friend class CPropertySheet;
		friend class CTaskDialog;
		friend class CWinThread;
		friend class CWnd;
		friend CWinApp* GetApp();

		typedef Shared_Ptr<TLSData> TLSDataPtr;

	public:
		CWinApp();
		virtual ~CWinApp();

		// Overridables
		virtual BOOL InitInstance();
		virtual int Run();

		// Operations
		HINSTANCE GetInstanceHandle() const { return m_hInstance; }
		HINSTANCE GetResourceHandle() const { return (m_hResource ? m_hResource : m_hInstance); }
		HCURSOR LoadCursor(LPCTSTR lpszResourceName) const;
		HCURSOR LoadCursor(int nIDCursor) const;
		HCURSOR LoadStandardCursor(LPCTSTR lpszCursorName) const;
		HICON	LoadIcon(LPCTSTR lpszResourceName) const;
		HICON	LoadIcon(int nIDIcon) const;
		HICON   LoadStandardIcon(LPCTSTR lpszIconName) const;
		HANDLE  LoadImage(LPCTSTR lpszResourceName, UINT uType, int cx, int  cy, UINT fuLoad = LR_DEFAULTCOLOR) const;
		HANDLE  LoadImage(int nIDImage, UINT uType, int cx, int cy, UINT fuLoad = LR_DEFAULTCOLOR) const;
		HCURSOR SetCursor(HCURSOR hCursor) const;
		void	SetResourceHandle(HINSTANCE hResource);

	private:
		CWinApp(const CWinApp&);				// Disable copy construction
		CWinApp& operator = (const CWinApp&);	// Disable assignment operator
		
		CDC_Data* GetCDCDataFromMap(HDC hDC);
		CGDI_Data* GetCGDIDataFromMap(HGDIOBJ hObject);
		CIml_Data* GetCImlDataFromMap(HIMAGELIST himl);
		CMenu_Data* GetCMenuDataFromMap(HMENU hMenu);

#ifdef USE_OBSOLETE_CODE
		CDC* GetCDCFromMap(HDC hDC);
		CGDIObject* GetCGDIObjectFromMap(HGDIOBJ hObject);
		CImageList* GetCImageListFromMap(HIMAGELIST hImageList);
		CMenu* GetCMenuFromMap(HMENU hMenu);
#endif
		
		CWnd* GetCWndFromMap(HWND hWnd);
		void	CleanupTemps();
		TLSData* GetTlsData() const;
		void	SetCallback();
		TLSData* SetTlsData();
		static CWinApp* SetnGetThis(CWinApp* pThis = 0);

		std::map<HDC, CDC_Data*, CompareHDC> m_mapCDCData;
		std::map<HGDIOBJ, CGDI_Data*, CompareGDI> m_mapCGDIData;
		std::map<HIMAGELIST, CIml_Data*, CompareHIMAGELIST> m_mapCImlData;
		std::map<HMENU, CMenu_Data*, CompareHMENU> m_mapCMenuData;

#ifdef USE_OBSOLETE_CODE
		std::map<HDC, CDC*, CompareHDC> m_mapHDC;			// maps device context handles to CDC objects
		std::map<HGDIOBJ, CGDIObject*, CompareGDI> m_mapGDI;	// maps GDI handles to CGDIObjects.
		std::map<HIMAGELIST, CImageList*, CompareHIMAGELIST> m_mapHIMAGELIST;	// maps HIMAGELIST to CImageList.
		std::map<HMENU, CMenu*, CompareHMENU> m_mapHMENU;	// maps menu handles to CMenu objects
#endif

		std::map<HWND, CWnd*, CompareHWND> m_mapHWND;		// maps window handles to CWnd objects
		std::vector<TLSDataPtr> m_vTLSData;		// vector of TLSData smart pointers, one for each thread
		CCriticalSection m_csMapLock;	// thread synchronisation for m_mapHWND
		CCriticalSection m_csTLSLock;	// thread synchronisation for m_vTLSData
		CCriticalSection m_csAppStart;	// thread synchronisation for application startup
		HINSTANCE m_hInstance;			// handle to the applications instance
		HINSTANCE m_hResource;			// handle to the applications resources
		DWORD m_dwTlsData;				// Thread Local Storage data
		WNDPROC m_Callback;				// callback address of CWnd::StaticWndowProc

	};

}

#include "winutils.h"
#include "c_string.h"


namespace Win32xx
{
	////////////////////////////////
	// Declaration of the CWnd class
	//
	class CWnd
	{
	friend class CMDIChild;
	friend class CDialog;
	friend class CPropertyPage;
	friend class CTaskDialog;
	friend class CWinApp;
	friend class CWinThread;

	public:
		CWnd();				// Constructor
		virtual ~CWnd();	// Destructor

		// These virtual functions can be overridden
		virtual BOOL Attach(HWND hWnd);
		virtual BOOL AttachDlgItem(UINT nID, HWND hwndParent);
		virtual void CenterWindow() const;

#ifdef USE_OBSOLETE_CODE
		virtual HWND Create(CWnd* pParent);
		virtual HWND CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rc, CWnd* pParent, UINT nID, LPVOID lpParam = NULL);
#endif

		virtual HWND Create(HWND hParent = NULL);
		virtual HWND CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU nIDorHMenu, LPVOID lpParam = NULL);
		virtual HWND CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rc, HWND hWndParent, UINT nID, LPVOID lpParam = NULL);
		virtual void Destroy();
		virtual HWND Detach();

#ifdef USE_OBSOLETE_CODE
		static	CWnd* FromHandle(HWND hWnd);
#endif

		static  CWnd* GetCWndPtr(HWND hWnd);
		virtual HICON SetIconLarge(int nIcon);
		virtual HICON SetIconSmall(int nIcon);

		// Attributes
		HWND GetHwnd() const				{ return m_hWnd; }
		WNDPROC GetPrevWindowProc() const	{ return m_PrevWindowProc; }

		// Wrappers for Win32 API functions
		// These functions aren't virtual, and shouldn't be overridden
		HDC   BeginPaint(PAINTSTRUCT& ps) const;
		BOOL  BringWindowToTop() const;
		LRESULT CallWindowProc(WNDPROC lpPrevWndFunc, UINT Msg, WPARAM wParam, LPARAM lParam) const;
		BOOL  CheckDlgButton(int nIDButton, UINT uCheck) const;
		BOOL  CheckRadioButton(int nIDFirstButton, int nIDLastButton, int nIDCheckButton) const;
		CWnd  ChildWindowFromPoint(POINT pt) const;
		BOOL  ClientToScreen(POINT& pt) const;
		BOOL  ClientToScreen(RECT& rc) const;
		LRESULT DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam) const;
		HDWP  DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) const;
		HDWP  DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, const RECT& rc, UINT uFlags) const;
		BOOL  DrawMenuBar() const;
		BOOL  EnableWindow(BOOL bEnable = TRUE) const;
		BOOL  EndPaint(PAINTSTRUCT& ps) const;
		CWnd  GetActiveWindow() const;
		CWnd  GetAncestor(UINT gaFlag = 3 /*= GA_ROOTOWNER*/) const;
		CWnd  GetCapture() const;
		ULONG_PTR GetClassLongPtr(int nIndex) const;
		CString GetClassName() const;
		CRect GetClientRect() const;
		CDC   GetDC() const;
		CDC   GetDCEx(HRGN hrgnClip, DWORD flags) const;
		CWnd  GetDesktopWindow() const;
		int	  GetDlgCtrlID() const;
		CWnd  GetDlgItem(int nIDDlgItem) const;
		UINT  GetDlgItemInt(int nIDDlgItem, BOOL* lpTranslated, BOOL bSigned) const;
		CString GetDlgItemText(int nIDDlgItem) const;
		CWnd  GetFocus() const;
		CFont GetFont() const;
		HICON GetIcon(BOOL bBigIcon) const;
		CWnd  GetNextDlgGroupItem(HWND hCtl, BOOL bPrevious) const;
		CWnd  GetNextDlgTabItem(HWND hCtl, BOOL bPrevious) const;
		CWnd  GetParent() const;
		BOOL  GetScrollInfo(int fnBar, SCROLLINFO& si) const;
		CRect GetUpdateRect(BOOL bErase) const;
		int GetUpdateRgn(HRGN hRgn, BOOL bErase) const;
		CWnd  GetWindow(UINT uCmd) const;
		CDC   GetWindowDC() const;
		LONG_PTR GetWindowLongPtr(int nIndex) const;
		CRect GetWindowRect() const;
		CString GetWindowText() const;
		int   GetWindowTextLength() const;
		void  Invalidate(BOOL bErase = TRUE) const;
		BOOL  InvalidateRect(LPCRECT lpRect, BOOL bErase = TRUE) const;
		BOOL  InvalidateRgn(HRGN hRgn, BOOL bErase = TRUE) const;
		BOOL  IsChild(HWND hwndChild) const;
		BOOL  IsDialogMessage(LPMSG lpMsg) const;
		UINT  IsDlgButtonChecked(int nIDButton) const;
		BOOL  IsWindow() const;
		BOOL  IsWindowEnabled() const;
		BOOL  IsWindowVisible() const;
		BOOL  KillTimer(UINT_PTR uIDEvent) const;
		int   MessageBox(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) const;
		void  MapWindowPoints(HWND hWndTo, POINT& pt) const;
		void  MapWindowPoints(HWND hWndTo, RECT& rc) const;
		void  MapWindowPoints(HWND hWndTo, LPPOINT ptArray, UINT nCount) const;
		BOOL  MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE) const;
		BOOL  MoveWindow(const RECT& rc, BOOL bRepaint = TRUE) const;
		BOOL  PostMessage(UINT uMsg, WPARAM wParam = 0L, LPARAM lParam = 0L) const;
		BOOL  PostMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;
		BOOL  RedrawWindow(LPCRECT lpRectUpdate = NULL, HRGN hRgn = NULL, UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN) const;
		int   ReleaseDC(HDC hDC) const;
		BOOL  ScreenToClient(POINT& Point) const;
		BOOL  ScreenToClient(RECT& rc) const;
		LRESULT SendDlgItemMessage(int nIDDlgItem, UINT Msg, WPARAM wParam, LPARAM lParam) const;
		LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0L, LPARAM lParam = 0L) const;
		LRESULT SendMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;
		BOOL  SendNotifyMessage(UINT Msg, WPARAM wParam, LPARAM lParam) const;
		CWnd  SetActiveWindow() const;
		CWnd  SetCapture() const;
		ULONG_PTR SetClassLongPtr(int nIndex, LONG_PTR dwNewLong) const;
		BOOL  SetDlgItemInt(int nIDDlgItem, UINT uValue, BOOL bSigned) const;
		BOOL  SetDlgItemText(int nIDDlgItem, LPCTSTR lpString) const;
		CWnd  SetFocus() const;
		void  SetFont(HFONT hFont, BOOL bRedraw = TRUE) const;
		BOOL  SetForegroundWindow() const;
		HICON SetIcon(HICON hIcon, BOOL bBigIcon) const;
		CWnd  SetParent(HWND hWndParent) const;
		BOOL  SetRedraw(BOOL bRedraw = TRUE) const;
		int   SetScrollInfo(int fnBar, const SCROLLINFO& si, BOOL fRedraw) const;
		UINT_PTR SetTimer(UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc) const;
		LONG_PTR SetWindowLongPtr(int nIndex, LONG_PTR dwNewLong) const;
		BOOL  SetWindowPos(HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) const;
		BOOL  SetWindowPos(HWND hWndInsertAfter, const RECT& rc, UINT uFlags) const;
		int   SetWindowRgn(CRgn& Rgn, BOOL bRedraw = TRUE) const;
		BOOL  SetWindowText(LPCTSTR lpString) const;
		HRESULT SetWindowTheme(LPCWSTR pszSubAppName, LPCWSTR pszSubIdList) const;
		BOOL  ShowWindow(int nCmdShow = SW_SHOWNORMAL) const;
		BOOL  UpdateWindow() const;
		BOOL  ValidateRect(LPCRECT prc) const;
		BOOL  ValidateRgn(HRGN hRgn) const;
		static CWnd WindowFromPoint(POINT pt);

  #ifndef _WIN32_WCE
		BOOL  CloseWindow() const;
		int   DlgDirList(LPTSTR lpPathSpec, int nIDListBox, int nIDStaticPath, UINT uFileType) const;
		int   DlgDirListComboBox(LPTSTR lpPathSpec, int nIDComboBox, int nIDStaticPath, UINT uFiletype) const;
		BOOL  DlgDirSelectEx(LPTSTR lpString, int nCount, int nIDListBox) const;
		BOOL  DlgDirSelectComboBoxEx(LPTSTR lpString, int nCount, int nIDComboBox) const;
		BOOL  DrawAnimatedRects(int idAni, RECT& rcFrom, RECT& rcTo) const;
		BOOL  DrawCaption(HDC hDC, RECT& rc, UINT uFlags) const;
		BOOL  EnableScrollBar(UINT uSBflags, UINT uArrows) const;
		CWnd  GetLastActivePopup() const;
		CMenu GetMenu() const;
		int   GetScrollPos(int nBar) const;
		BOOL  GetScrollRange(int nBar, int& MinPos, int& MaxPos) const;
		CMenu GetSystemMenu(BOOL bRevert) const;
		CWnd  GetTopWindow() const;
		BOOL  GetWindowPlacement(WINDOWPLACEMENT& pWndpl) const;
		BOOL  HiliteMenuItem(HMENU hMenu, UINT uItemHilite, UINT uHilite) const;
		BOOL  IsIconic() const;
		BOOL  IsZoomed() const;
		BOOL  LockWindowUpdate() const;
		BOOL  OpenIcon() const;
		void  Print(HDC hDC, DWORD dwFlags) const;
		BOOL  SetMenu(HMENU hMenu) const;
		BOOL  ScrollWindow(int XAmount, int YAmount, LPCRECT lprcScroll, LPCRECT lprcClip) const;
		int   ScrollWindowEx(int dx, int dy, LPCRECT lprcScroll, LPCRECT lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate, UINT flags) const;
		int   SetScrollPos(int nBar, int nPos, BOOL bRedraw) const;
		BOOL  SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw) const;
		BOOL  SetWindowPlacement(const WINDOWPLACEMENT& wndpl) const;
		BOOL  ShowOwnedPopups(BOOL fShow) const;
		BOOL  ShowScrollBar(int nBar, BOOL bShow) const;
		BOOL  ShowWindowAsync(int nCmdShow) const;
		BOOL  UnLockWindowUpdate() const;
		CWnd  WindowFromDC(HDC hDC) const;

    #ifndef WIN32_LEAN_AND_MEAN
		void  DragAcceptFiles(BOOL fAccept) const;
    #endif
  #endif

		static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		operator HWND() const { return m_hWnd; }

	protected:
		// Override these functions as required
		virtual LRESULT FinalWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual void OnAttach();
		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
		virtual void OnClose();
		virtual int  OnCreate(LPCREATESTRUCT pcs);
		virtual void OnDestroy();
		virtual void OnDraw(CDC& dc);
		virtual BOOL OnEraseBkgnd(CDC& dc);

#ifdef USE_OBSOLETE_CODE
		virtual void OnDraw(CDC* pDC);
		virtual BOOL OnEraseBkgnd(CDC* pDC);
#endif

		virtual void OnInitialUpdate();
		virtual void OnMenuUpdate(UINT nID);
		virtual LRESULT OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void PreRegisterClass(WNDCLASS& wc);
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

		HWND m_hWnd;					// handle to this object's window

	private:
		struct DataMembers	// A structure that contains the data members
		{
			DataMembers()	// Constructor
			{
				::ZeroMemory(&cs, sizeof(CREATESTRUCT));
				::ZeroMemory(&wc, sizeof(WNDCLASS));
			}

			CREATESTRUCT cs;
			WNDCLASS	 wc;
		};

		CWnd(const CWnd&);				// Disable copy construction
		CWnd& operator = (const CWnd&); // Disable assignment operator
		CWnd(HWND hWnd);				// Private constructor used internally

		void AddToMap();
		void Cleanup();
		LRESULT MessageReflect(HWND hwndParent, UINT uMsg, WPARAM wParam, LPARAM lParam);
		BOOL RegisterClass(WNDCLASS& wc);
		BOOL RemoveFromMap();
		void Subclass(HWND hWnd);

		Shared_Ptr<DataMembers> m_pData;
		WNDPROC	m_PrevWindowProc;

	}; // class CWnd

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "gdi.h"
#include "menu.h"
#include "imagelist.h"

namespace Win32xx
{

	//////////////////////////////////////////
	// Definitions for the CWinException class
	//
	inline CWinException::CWinException(LPCTSTR pszText) throw () : m_Error(::GetLastError()), m_pszText(pszText)
	{
		memset(m_szErrorString, 0, MAX_STRING_SIZE * sizeof(TCHAR));

		if (m_Error != 0)
		{
			DWORD dwFlags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
			::FormatMessage(dwFlags, NULL, m_Error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), m_szErrorString, MAX_STRING_SIZE-1, NULL);
		}
		else
			lstrcpyn(m_szErrorString, pszText, MAX_STRING_SIZE);
	}

	inline DWORD CWinException::GetError() const throw ()
	{
		return m_Error;
	}

	inline LPCTSTR CWinException::GetErrorString() const throw ()
	{
		return m_szErrorString;
	}

	inline const char * CWinException::what() const throw ()
	{
		// Sends the last error string to the debugger (typically displayed in the IDE's output window).
		::OutputDebugString(m_szErrorString);
		return "CWinException thrown";
	}

	///////////////////////////////////////
	// Definitions for the CWinThread class
	//
	inline CWinThread::CWinThread() : m_pfnThreadProc(0), m_pThreadParams(0), m_hThread(0),
		                               m_nThreadID(0), m_hAccel(0), m_pWndAccel(0)
	// Override CWinThread and use this constructor for GUI threads.
	// InitInstance will be called when the thread runs.
	{
	}

	inline CWinThread::CWinThread(PFNTHREADPROC pfnThreadProc, LPVOID pParam) : m_pfnThreadProc(0),
		                m_pThreadParams(0), m_hThread(0), m_nThreadID(0), m_hAccel(0), m_pWndAccel(0)
	// Use CWinThread directly and call this constructor for worker threads.
	// Specify a pointer to the function to run when the thread starts.
	// Specifying pParam for a worker thread is optional.
	{
		m_pfnThreadProc = pfnThreadProc;
		m_pThreadParams = pParam;
	}

	inline CWinThread::~CWinThread()
	{
		if (m_hThread)
		{
			// A thread's state is set to signalled when the thread terminates.
			// If your thread is still running at this point, you have a bug.
			if (WaitForSingleObject(m_hThread, 0) != 0)
			{
				TRACE("*** Error *** Ending CWinThread before ending its thread\n");
				assert(FALSE);
			}

			// Close the thread's handle
			::CloseHandle(m_hThread);
		}
	}

	inline void CWinThread::CleanupTemps()
	// Removes all Temporary CWnds and CMenus belonging to this thread
	{
		// Retrieve the pointer to the TLS Data
		TLSData* pTLSData = GetApp()->GetTlsData();
		assert(pTLSData);

		pTLSData->TmpDCs.clear();
		pTLSData->TmpGDIs.clear();
		pTLSData->TmpImageLists.clear();
		pTLSData->TmpWnds.clear();

	#ifndef _WIN32_WCE
		pTLSData->TmpMenus.clear();
	#endif

	}

	inline void CWinThread::CreateThread(unsigned initflag /* = 0 */, unsigned stack_size/* = 0 */, LPSECURITY_ATTRIBUTES pSecurityAttributes /*= NULL*/)
	{
		// Valid argument values:
		// initflag					Either CREATE_SUSPENDED or 0
		// stack_size				Either the stack size or 0
		// pSecurityAttributes		Either a pointer to SECURITY_ATTRIBUTES or 0

		if (NULL == m_pfnThreadProc) m_pfnThreadProc = CWinThread::StaticThreadProc;
		if (NULL == m_pThreadParams) m_pThreadParams = this;

#ifdef _WIN32_WCE
		m_hThread = (HANDLE)::CreateThread(pSecurityAttributes, stack_size, (LPTHREAD_START_ROUTINE)m_pfnThreadProc, m_pThreadParams, initflag, &m_dwThreadID);
#else
		m_hThread = (HANDLE)::_beginthreadex(pSecurityAttributes, stack_size, (unsigned int (__stdcall *)(void *))m_pfnThreadProc, m_pThreadParams, initflag, &m_nThreadID);
#endif

		if (m_hThread == 0)
			throw CWinException(_T("Failed to create thread"));
	}

	inline HANDLE CWinThread::GetThread() const
	{
		assert(m_hThread);
		return m_hThread;
	}

	inline int CWinThread::GetThreadID() const
	{
		assert(m_hThread);

#ifdef _WIN32_WCE
		return m_dwThreadID;
#endif

		return m_nThreadID;
	}

	inline int CWinThread::GetThreadPriority() const
	{
		assert(m_hThread);
		return ::GetThreadPriority(m_hThread);
	}

	inline BOOL CWinThread::InitInstance()
	{
		// Override this function to perform tasks when the thread starts.

		// return TRUE to run a message loop, otherwise return FALSE.
		// A thread with a window must run a message loop.
		return FALSE;
	}

	inline int CWinThread::MessageLoop()
	{
		// This gets any messages queued for the application, and dispatches them.
		MSG Msg;
		ZeroMemory(&Msg, sizeof(MSG));
		int status = 1;
		LONG lCount = 0;

		while (status != 0)
		{
			// While idle, perform idle processing until OnIdle returns FALSE
			// Exclude some messages to avoid calling OnIdle excessively
			while (!::PeekMessage(&Msg, 0, 0, 0, PM_NOREMOVE) &&
								(Msg.message != WM_TIMER) &&
								(Msg.message != WM_MOUSEMOVE) &&
								(Msg.message != WM_SETCURSOR) &&
								OnIdle(lCount) != FALSE  )
			{
				++lCount;
			}


			lCount = 0;

			// Now wait until we get a message
			if ((status = ::GetMessage(&Msg, NULL, 0, 0)) == -1)
				return -1;

			if (Msg.message == UWM_CLEANUPTEMPS)
			{
				CleanupTemps();
			}
			else
			{
				if (!PreTranslateMessage(Msg))
				{
					::TranslateMessage(&Msg);
					::DispatchMessage(&Msg);
				}
			}
		}

		return LOWORD(Msg.wParam);
	}

	inline BOOL CWinThread::OnIdle(LONG lCount)
	{
		UNREFERENCED_PARAMETER(lCount);

		return FALSE;
	}

	inline BOOL CWinThread::PreTranslateMessage(MSG Msg)
	{
		// This functions is called by the MessageLoop. It processes the
		// keyboard accelerator keys and calls CWnd::PreTranslateMessage for
		// keyboard and mouse events.

		BOOL Processed = FALSE;

		// only pre-translate mouse and keyboard input events
		if ((Msg.message >= WM_KEYFIRST && Msg.message <= WM_KEYLAST) ||
			(Msg.message >= WM_MOUSEFIRST && Msg.message <= WM_MOUSELAST))
		{
			// Process keyboard accelerators
			if (GetAcceleratorsWindow() && ::TranslateAccelerator(*GetAcceleratorsWindow(), GetAccelerators(), &Msg))
				Processed = TRUE;
			else
			{
				// Search the chain of parents for pretranslated messages.
				for (HWND hWnd = Msg.hwnd; hWnd != NULL; hWnd = ::GetParent(hWnd))
				{
					CWnd* pWnd = GetApp()->GetCWndFromMap(hWnd);
					if (pWnd)
					{
						Processed = pWnd->PreTranslateMessage(&Msg);
						if(Processed)
							break;
					}
				}
			}
		}

		return Processed;
	}

	inline DWORD CWinThread::ResumeThread() const
	{
		assert(m_hThread);
		return ::ResumeThread(m_hThread);
	}

	inline void CWinThread::SetAccelerators(HACCEL hAccel, CWnd* pWndAccel)
	// nID is the resource ID of the accelerator table
	// pWndAccel is the window pointer for translated messages
	{
		assert (hAccel);
		assert (pWndAccel);

		m_pWndAccel = pWndAccel;
		m_hAccel = hAccel;
	}

	inline BOOL CWinThread::SetThreadPriority(int nPriority) const
	{
		assert(m_hThread);
		return ::SetThreadPriority(m_hThread, nPriority);
	}


	inline DWORD CWinThread::SuspendThread() const
	{
		assert(m_hThread);
		return ::SuspendThread(m_hThread);
	}

	inline UINT WINAPI CWinThread::StaticThreadProc(LPVOID pCThread)
	// When the thread starts, it runs this function.
	{
		// Get the pointer for this CMyThread object
		CWinThread* pThread = static_cast<CWinThread*>(pCThread);
		assert(dynamic_cast<CWinThread*>(pThread));

		if (pThread->InitInstance())
		{
			GetApp()->SetTlsData();
			return pThread->MessageLoop();
		}

		return 0;
	}


	////////////////////////////////////
	// Definitions for the CWinApp class
	//

	// To begin Win32++, inherit your application class from this one.
	// You must run only one instance of the class inherited from this.
	inline CWinApp::CWinApp() : m_Callback(NULL)//, m_hAccel(0), m_pWndAccel(0)
	{
		try
		{
			m_csAppStart.Lock();
			assert( 0 == SetnGetThis() );	// Test if this is the first instance of CWinApp

			m_dwTlsData = ::TlsAlloc();
			if (m_dwTlsData == TLS_OUT_OF_INDEXES)
			{
				// We only get here in the unlikely event that all TLS indexes are already allocated by this app
				// At least 64 TLS indexes per process are allowed. Win32++ requires only one TLS index.
				m_csAppStart.Release();
				throw CWinException(_T("CWinApp::CWinApp  Failed to allocate TLS Index"));
			}

			SetnGetThis(this);
			m_csAppStart.Release();

			// Set the instance handle
	#ifdef _WIN32_WCE
			m_hInstance = (HINSTANCE)GetModuleHandle(0);
	#else
			MEMORY_BASIC_INFORMATION mbi;
			ZeroMemory(&mbi, sizeof(MEMORY_BASIC_INFORMATION));
			static int Address = 0;
			VirtualQuery( &Address, &mbi, sizeof(mbi) );
			assert(mbi.AllocationBase);
			m_hInstance = (HINSTANCE)mbi.AllocationBase;
	#endif

			m_hResource = m_hInstance;
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
		// Ensure all temporary objects are destroyed before CWinApp is deconstructed
		// These maps contain smart pointers, so clearing them calls the destructor
		// on their contents.
		std::vector<TLSDataPtr>::iterator iter;
		for (iter = m_vTLSData.begin(); iter != m_vTLSData.end(); ++iter)
		{
			(*iter)->TmpDCs.clear();
			(*iter)->TmpWnds.clear();
			(*iter)->TmpGDIs.clear();
			(*iter)->TmpImageLists.clear();
#ifndef _WIN32_WCE
			(*iter)->TmpMenus.clear();
#endif
		}

		// Forcibly destroy any remaining windows now. Windows created from
		//  static CWnds or dangling pointers are destroyed here.
		std::map<HWND, CWnd*, CompareHWND>::iterator m;
		for (m = m_mapHWND.begin(); m != m_mapHWND.end(); ++m)
		{
			HWND hWnd = (*m).first;
			if (::IsWindow(hWnd))
			{
				::DestroyWindow(hWnd);
			}
		}

		// Do remaining tidy up
		m_vTLSData.clear();
		if (m_dwTlsData != TLS_OUT_OF_INDEXES)
		{
			::TlsSetValue(m_dwTlsData, NULL);
			::TlsFree(m_dwTlsData);
		}

		SetnGetThis(reinterpret_cast<CWinApp*>(-1));
	}

	inline void CWinApp::CleanupTemps()
	// Removes all temporaries belonging to this thread
	{
		// Retrieve the pointer to the TLS Data
		TLSData* pTLSData = GetApp()->GetTlsData();
		assert(pTLSData);

		pTLSData->TmpDCs.clear();
		pTLSData->TmpGDIs.clear();
		pTLSData->TmpImageLists.clear();
		pTLSData->TmpWnds.clear();

	#ifndef _WIN32_WCE
		pTLSData->TmpMenus.clear();
	#endif

	}

	inline CDC_Data* CWinApp::GetCDCDataFromMap(HDC hDC)
	{
		std::map<HDC, CDC_Data*, CompareHDC>::iterator m;

		// Find the CDC data mapped to this HDC
		CDC_Data* pCDCData = 0;
		m_csMapLock.Lock();
		m = m_mapCDCData.find(hDC);

		if (m != m_mapCDCData.end())
			pCDCData = m->second;

		m_csMapLock.Release();
		return pCDCData;
	}

	inline CGDI_Data* CWinApp::GetCGDIDataFromMap(HGDIOBJ hObject)
	{
		std::map<HGDIOBJ, CGDI_Data*, CompareGDI>::iterator m;

		// Find the CGDIObject data mapped to this HGDIOBJ
		CGDI_Data* pCGDIData = 0;
		m_csMapLock.Lock();
		m = m_mapCGDIData.find(hObject);

		if (m != m_mapCGDIData.end())
			pCGDIData = m->second;

		m_csMapLock.Release();
		return pCGDIData;
	}

	inline CIml_Data* CWinApp::GetCImlDataFromMap(HIMAGELIST himl)
	{
		std::map<HIMAGELIST, CIml_Data*, CompareHIMAGELIST>::iterator m;
		
		// Find the CImageList data mapped to this HIMAGELIST
		CIml_Data* pCImlData = 0;
		m_csMapLock.Lock();
		m = m_mapCImlData.find(himl);

		if (m != m_mapCImlData.end())
			pCImlData = m->second;

		m_csMapLock.Release();
		return pCImlData;
	}

	inline CMenu_Data* CWinApp::GetCMenuDataFromMap(HMENU hMenu)
	{
		std::map<HMENU, CMenu_Data*, CompareHMENU>::iterator m;

		// Find the CMenu data mapped to this HMENU
		CMenu_Data* pCMenuData = 0;
		m_csMapLock.Lock();
		m = m_mapCMenuData.find(hMenu);

		if (m != m_mapCMenuData.end())
			pCMenuData = m->second;

		m_csMapLock.Release();
		return pCMenuData;
	}

#ifdef USE_OBSOLETE_CODE
	inline CDC* CWinApp::GetCDCFromMap(HDC hDC)
	{
		// Allocate an iterator for our HWND map
		std::map<HDC, CDC*, CompareHDC>::iterator m;

		// Find the CDC pointer mapped to this HDC
		CDC* pDC = 0;
		m_csMapLock.Lock();
		m = m_mapHDC.find(hDC);

		if (m != m_mapHDC.end())
			pDC = m->second;

		m_csMapLock.Release();
		return pDC;
	}

	inline CGDIObject* CWinApp::GetCGDIObjectFromMap(HGDIOBJ hObject)
	{
		// Allocate an iterator for our HWND map
		std::map<HGDIOBJ, CGDIObject*, CompareGDI>::iterator m;

		// Find the CGDIObject pointer mapped to this HGDIOBJ
		CGDIObject* pObject = 0;
		m_csMapLock.Lock();
		m = m_mapGDI.find(hObject);

		if (m != m_mapGDI.end())
			pObject = m->second;

		m_csMapLock.Release();
		return pObject;
	}

	inline CImageList* CWinApp::GetCImageListFromMap(HIMAGELIST hImageList)
	{
		std::map<HIMAGELIST, CImageList*, CompareHIMAGELIST>::iterator m;

		// Find the CMenu pointer mapped to this HMENU
		CImageList* pImageList = 0;
		m_csMapLock.Lock();
		m = m_mapHIMAGELIST.find(hImageList);

		if (m != m_mapHIMAGELIST.end())
			pImageList = m->second;

		m_csMapLock.Release();
		return pImageList;
	}

	inline CMenu* CWinApp::GetCMenuFromMap(HMENU hMenu)
	{
		std::map<HMENU, CMenu*, CompareHMENU>::iterator m;

		// Find the CMenu pointer mapped to this HMENU
		CMenu* pMenu = 0;
		m_csMapLock.Lock();
		m = m_mapHMENU.find(hMenu);

		if (m != m_mapHMENU.end())
			pMenu = m->second;

		m_csMapLock.Release();
		return pMenu;
	}
#endif

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

	inline TLSData* CWinApp::GetTlsData() const
	{
		return static_cast<TLSData*>(TlsGetValue(GetApp()->m_dwTlsData));
	}

	inline BOOL CWinApp::InitInstance()
	{
		// InitInstance contains the initialization code for your application
		// You should override this function with the code to run when the application starts.

		// return TRUE to indicate success. FALSE will end the application
		return TRUE;
	}

	inline HCURSOR CWinApp::LoadCursor(LPCTSTR lpszResourceName) const
	// Loads the cursor resource from the resource script (resource.rc)
	{
		return ::LoadCursor(GetApp()->GetResourceHandle(), lpszResourceName);
	}

	inline HCURSOR CWinApp::LoadCursor(int nIDCursor) const
	// Loads the cursor resource from the resource script (resource.rc)
	{
		return ::LoadCursor(GetApp()->GetResourceHandle(), MAKEINTRESOURCE (nIDCursor));
	}

	inline HCURSOR CWinApp::LoadStandardCursor(LPCTSTR lpszCursorName) const
	// Returns the handle of a standard cursor. Standard cursors include:
	// IDC_APPSTARTING, IDC_ARROW, IDC_CROSS, IDC_HAND, IDC_HELP, IDC_IBEAM, IDC_NO, IDC_SIZEALL,
	// IDC_SIZENESW, IDC_SIZENS, IDC_SIZENWSE, IDC_SIZEWE, IDC_UPARROW, IDC_WAIT
	{
		return ::LoadCursor(0, lpszCursorName);
	}

	inline HICON CWinApp::LoadIcon(LPCTSTR lpszResourceName) const
	// Loads the icon resource whose size conforms to the SM_CXICON and SM_CYICON system metric values
	// For other icon sizes, use the LoadImage windows API function.
	{
		return ::LoadIcon(GetApp()->GetResourceHandle(), lpszResourceName);
	}

	inline HICON CWinApp::LoadIcon(int nIDIcon) const
	// Loads the icon resource whose size conforms to the SM_CXICON and SM_CYICON system metric values
	{
		return ::LoadIcon(GetApp()->GetResourceHandle(), MAKEINTRESOURCE (nIDIcon));
	}

	inline HICON CWinApp::LoadStandardIcon(LPCTSTR lpszIconName) const
	// Returns the handle of a standard Icon. Standard Icons include:
	// IDI_APPLICATION, IDI_ASTERISK, IDI_ERROR, IDI_EXCLAMATION,
	// IDI_HAND, IDI_INFORMATION, IDI_QUESTION, IDI_WARNING
	{
		return ::LoadIcon(0, lpszIconName);
	}

	inline HANDLE CWinApp::LoadImage(LPCTSTR lpszResourceName, UINT uType, int cx, int cy, UINT fuLoad) const
	// Loads an icon, cursor, animated cursor, or bitmap image
	// uType is the image type. It can be IMAGE_BITMAP, IMAGE_CURSOR or IMAGE_ICON
	// cx and cy are the desired width and height in pixels
	// fuLoad can be LR_DEFAULTCOLOR, LR_CREATEDIBSECTION, LR_DEFAULTSIZE, LR_LOADFROMFILE,
	//   LR_LOADMAP3DCOLORS, R_LOADTRANSPARENT, LR_MONOCHROME, LR_SHARED, LR_VGACOLOR.
	{
		return ::LoadImage(GetApp()->GetResourceHandle(), lpszResourceName, uType, cx, cy, fuLoad);
	}

	inline HANDLE CWinApp::LoadImage(int nIDImage, UINT uType, int cx, int cy, UINT fuLoad) const
	// Loads an icon, cursor, animated cursor, or bitmap
	// uType is the image type. It can be IMAGE_BITMAP, IMAGE_CURSOR or IMAGE_ICON
	// cx and cy are the desired width and height in pixels
	// fuLoad can be LR_DEFAULTCOLOR, LR_CREATEDIBSECTION, LR_DEFAULTSIZE, LR_LOADFROMFILE,
	//   LR_LOADMAP3DCOLORS, R_LOADTRANSPARENT, LR_MONOCHROME, LR_SHARED, LR_VGACOLOR.
	{
		return ::LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE (nIDImage), uType, cx, cy, fuLoad);
	}

	inline int CWinApp::Run()
	{
		// InitInstance runs the App's initialization code
		if (InitInstance())
		{
			// Dispatch the window messages
			return MessageLoop();
		}
		else
		{
			TRACE("InitInstance failed!  Terminating program\n");
			::PostQuitMessage(-1);
			return -1;
		}
	}

	inline void CWinApp::SetCallback()
	{
		// Registers a temporary window class so we can get the callback
		// address of CWnd::StaticWindowProc.
		// This technique works for all Window versions, including WinCE.

		WNDCLASS wcDefault;
		ZeroMemory(&wcDefault, sizeof(WNDCLASS));

		LPCTSTR szClassName		= _T("Win32++ Temporary Window Class");
		wcDefault.hInstance		= GetInstanceHandle();
		wcDefault.lpfnWndProc	= CWnd::StaticWindowProc;
		wcDefault.lpszClassName = szClassName;

		::RegisterClass(&wcDefault);

		// Retrieve the class information
		ZeroMemory(&wcDefault, sizeof(wcDefault));
		::GetClassInfo(GetInstanceHandle(), szClassName, &wcDefault);

		// Save the callback address of CWnd::StaticWindowProc
		assert(wcDefault.lpfnWndProc);	// Assert fails when running UNICODE build on ANSI OS.
		m_Callback = wcDefault.lpfnWndProc;
		::UnregisterClass(szClassName, GetInstanceHandle());
	}

	inline HCURSOR CWinApp::SetCursor(HCURSOR hCursor) const
	// Sets the current cursor and returns the previous one.
	// Note:The cursor will be set to the window's class cursor (if one is set) each time the
	//  mouse is moved over the window. You can specify different cursors for different
	//  conditions while processing WM_SETCURSOR
	{
		return ::SetCursor(hCursor);
	}

	inline CWinApp* CWinApp::SetnGetThis(CWinApp* pThis /*= 0*/)
	{
		// This function stores the 'this' pointer in a static variable.
		// Once stored, it can be used later to return the 'this' pointer.
		// CWinApp's constructor calls this function and sets the static variable.
		// CWinApp's destructor calls this function with a value of -1.

		static CWinApp* pWinApp = 0;

		if (reinterpret_cast<CWinApp*>(-1) == pThis)
			pWinApp = 0;
		else if (!pWinApp)
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

	inline TLSData* CWinApp::SetTlsData()
	{
		TLSData* pTLSData = GetApp()->GetTlsData();
		if (NULL == pTLSData)
		{
			pTLSData = new TLSData;

			m_csTLSLock.Lock();
			m_vTLSData.push_back(pTLSData);	// store as a Shared_Ptr
			m_csTLSLock.Release();

			::TlsSetValue(m_dwTlsData, pTLSData);
		}

		return pTLSData;
	}


	////////////////////////////////////////
	// Definitions for the CWnd class
	//
	inline CWnd::CWnd() : m_hWnd(NULL)
	{
		// Note: m_hWnd is set in CWnd::CreateEx(...)
		//       m_pData is assigned in CWnd::Create(...)
	}

	inline CWnd::CWnd(HWND hWnd)
	{
		// A private constructor, used internally.
		
		m_hWnd = hWnd;
	}

	inline CWnd::~CWnd()
	{
		// Destroys the window for this object and cleans up resources.
		Destroy();
	}

	inline void CWnd::AddToMap()
	// Store the window handle and CWnd pointer in the HWND map
	{
		assert( GetApp() );
		GetApp()->m_csMapLock.Lock();

		assert(!GetApp()->GetCWndFromMap(m_hWnd));

		GetApp()->m_mapHWND.insert(std::make_pair(m_hWnd, this));
		GetApp()->m_csMapLock.Release();
	}

	inline BOOL CWnd::Attach(HWND hWnd)
	// Subclass an existing window and attach it to a CWnd
	{
		assert( GetApp() );
		assert( ::IsWindow(hWnd) );
		assert( !IsWindow() );

		// Ensure this thread has the TLS index set
		// Note: Perform the attach from the same thread as the window's message loop
		GetApp()->SetTlsData();

		Cleanup();			// Cleanup any previous attachment
		Subclass(hWnd);		// Set the window's callback to CWnd::StaticWindowProc
		AddToMap();			// Store the CWnd pointer in the HWND map

		OnAttach();
		PostMessage(UWM_WINDOWCREATED);

		return TRUE;
	}

	inline BOOL CWnd::AttachDlgItem(UINT nID, HWND hwndParent)
	// Converts a dialog item to a CWnd object
	{
		assert(::IsWindow(hwndParent));

		HWND hWnd = ::GetDlgItem(hwndParent, nID);
		return Attach(hWnd);
	}

	inline void CWnd::CenterWindow() const
	// Centers this window over it's parent
	{

	// required for multi-monitor support with Dev-C++ and VC6
#ifndef _WIN32_WCE
  #ifndef MONITOR_DEFAULTTONEAREST
	#define MONITOR_DEFAULTTONEAREST    0x00000002

		DECLARE_HANDLE(HMONITOR);

		typedef struct tagMONITORINFO
		{
			DWORD   cbSize;
			RECT    rcMonitor;
			RECT    rcWork;
			DWORD   dwFlags;
		} MONITORINFO, *LPMONITORINFO;

  #endif	// MONITOR_DEFAULTTONEAREST
#endif	// _WIN32_WCE

		assert(IsWindow());

		CRect rc = GetWindowRect();
		CRect rcParent;
		CRect rcDesktop;

		// Get screen dimensions excluding task bar
		::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcDesktop, 0);

		// Get the parent window dimensions (parent could be the desktop)
		if (GetParent() != NULL) rcParent = GetParent().GetWindowRect();
		else rcParent = rcDesktop;

	#ifndef _WIN32_WCE
		// Import the GetMonitorInfo and MonitorFromWindow functions
		HMODULE hUser32 = LoadLibrary(_T("USER32.DLL"));
		typedef BOOL (WINAPI* LPGMI)(HMONITOR hMonitor, LPMONITORINFO lpmi);
		typedef HMONITOR (WINAPI* LPMFW)(HWND hwnd, DWORD dwFlags);
		LPMFW pfnMonitorFromWindow = (LPMFW)::GetProcAddress(hUser32, "MonitorFromWindow");
	#ifdef _UNICODE
		LPGMI pfnGetMonitorInfo = (LPGMI)::GetProcAddress(hUser32, "GetMonitorInfoW");
	#else
		LPGMI pfnGetMonitorInfo = (LPGMI)::GetProcAddress(hUser32, "GetMonitorInfoA");
	#endif

		// Take multi-monitor systems into account
		if (pfnGetMonitorInfo && pfnMonitorFromWindow)
		{
			HMONITOR hActiveMonitor = pfnMonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
			MONITORINFO mi;
			ZeroMemory(&mi, sizeof(MONITORINFO));
			mi.cbSize = sizeof(MONITORINFO);

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

		SetWindowPos(0, x, y, 0, 0, SWP_NOSIZE);
	}

	inline void CWnd::Cleanup()
	// Returns the CWnd to its default state
	{
		if ( GetApp() )
			RemoveFromMap();

		m_hWnd = NULL;
		m_PrevWindowProc = NULL;
	}

#ifdef USE_OBSOLETE_CODE

	inline HWND CWnd::Create(CWnd* pParent /* = NULL */)
	// Creates the window. This is the default method of window creation.
	{
		// Test if Win32++ has been started
		assert( GetApp() );
		if (m_pData.get() == 0)
			m_pData = new DataMembers;

		// Set the WNDCLASS parameters
		PreRegisterClass(m_pData->wc);
		if (m_pData->wc.lpszClassName)
		{
			RegisterClass(m_pData->wc);
			m_pData->cs.lpszClass = m_pData->wc.lpszClassName;
		}
		else
			m_pData->cs.lpszClass = _T("Win32++ Window");

		// Set Parent
		HWND hWndParent = pParent? pParent->GetHwnd() : 0;

		// Set a reasonable default window style
		DWORD dwOverlappedStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
		m_pData->cs.style = WS_VISIBLE | ((hWndParent)? WS_CHILD : dwOverlappedStyle);

		// Set a reasonable default window position
		if (NULL == pParent)
		{
			m_pData->cs.x  = CW_USEDEFAULT;
			m_pData->cs.cx = CW_USEDEFAULT;
			m_pData->cs.y  = CW_USEDEFAULT;
			m_pData->cs.cy = CW_USEDEFAULT;
		}

		// Allow the CREATESTRUCT parameters to be modified
		PreCreate(m_pData->cs);

		DWORD dwStyle = m_pData->cs.style & ~WS_VISIBLE;

		// Create the window
#ifndef _WIN32_WCE
		CreateEx(m_pData->cs.dwExStyle, m_pData->cs.lpszClass, m_pData->cs.lpszName, dwStyle, m_pData->cs.x, m_pData->cs.y,
				m_pData->cs.cx, m_pData->cs.cy, hWndParent, m_pData->cs.hMenu, m_pData->cs.lpCreateParams);

		if (m_pData->cs.style & WS_VISIBLE)
		{
			if		(m_pData->cs.style & WS_MAXIMIZE) ShowWindow(SW_MAXIMIZE);
			else if (m_pData->cs.style & WS_MINIMIZE) ShowWindow(SW_MINIMIZE);
			else	ShowWindow();
		}

#else
		CreateEx(m_pData->cs.dwExStyle, m_pData->cs.lpszClass, m_pData->cs.lpszName, m_pData->cs.style, m_pData->cs.x, m_pData->cs.y,
				m_pData->cs.cx, m_pData->cs.cy, hWndParent, 0, m_pData->cs.lpCreateParams);
#endif

		return m_hWnd;
	}

	inline HWND CWnd::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rc, CWnd* pParent, UINT nID, LPVOID lpParam /*= NULL*/)
	// Creates the window by specifying all the window creation parameters
	{
		int x = rc.left;
		int y = rc.top;
		int cx = rc.right - rc.left;
		int cy = rc.bottom - rc.top;
		HWND hWndParent = pParent? pParent->GetHwnd() : 0;
		HMENU hMenu = pParent? (HMENU)(INT_PTR)nID : ::LoadMenu(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(nID));

		return CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, x, y, cx, cy, hWndParent, hMenu, lpParam);
	}

#endif // USE_OBSOLETE_CODE

	inline HWND CWnd::Create(HWND hWndParent /* = NULL */)
	// Creates the window. This is the default method of window creation.
	{
		// Test if Win32++ has been started
		assert( GetApp() );
		
		if (m_pData.get() == 0)
			m_pData = new DataMembers;

		// Set the WNDCLASS parameters
		PreRegisterClass(m_pData->wc);
		if (m_pData->wc.lpszClassName)
		{
			RegisterClass(m_pData->wc);
			m_pData->cs.lpszClass = m_pData->wc.lpszClassName;
		}
		else
			m_pData->cs.lpszClass = _T("Win32++ Window");

		// Set a reasonable default window style
		DWORD dwOverlappedStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
		m_pData->cs.style = WS_VISIBLE | ((hWndParent)? WS_CHILD : dwOverlappedStyle);

		// Set a reasonable default window position
		if (NULL == hWndParent)
		{
			m_pData->cs.x  = CW_USEDEFAULT;
			m_pData->cs.cx = CW_USEDEFAULT;
			m_pData->cs.y  = CW_USEDEFAULT;
			m_pData->cs.cy = CW_USEDEFAULT;
		}

		// Allow the CREATESTRUCT parameters to be modified
		PreCreate(m_pData->cs);

		DWORD dwStyle = m_pData->cs.style & ~WS_VISIBLE;

		// Create the window
#ifndef _WIN32_WCE
		CreateEx(m_pData->cs.dwExStyle, m_pData->cs.lpszClass, m_pData->cs.lpszName, dwStyle, 
			    m_pData->cs.x, m_pData->cs.y, m_pData->cs.cx, m_pData->cs.cy, hWndParent, 
				m_pData->cs.hMenu, m_pData->cs.lpCreateParams);

		if (m_pData->cs.style & WS_VISIBLE)
		{
			if		(m_pData->cs.style & WS_MAXIMIZE) ShowWindow(SW_MAXIMIZE);
			else if (m_pData->cs.style & WS_MINIMIZE) ShowWindow(SW_MINIMIZE);
			else	ShowWindow();
		}

#else
		CreateEx(m_pData->cs.dwExStyle, m_pData->cs.lpszClass, m_pData->cs.lpszName, m_pData->cs.style, 
			    m_pData->cs.x, m_pData->cs.y, m_pData->cs.cx, m_pData->cs.cy, hWndParent,
				0, m_pData->cs.lpCreateParams);
#endif

		return m_hWnd;
	}

	inline HWND CWnd::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rc, HWND hWndParent, UINT nID, LPVOID lpParam /*= NULL*/)
	// Creates the window by specifying all the window creation parameters
	{
		int x = rc.left;
		int y = rc.top;
		int cx = rc.right - rc.left;
		int cy = rc.bottom - rc.top;
		HMENU hMenu = hWndParent? (HMENU)(INT_PTR)nID : ::LoadMenu(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(nID));

		return CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, x, y, cx, cy, hWndParent, hMenu, lpParam);
	}

	inline HWND CWnd::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU nIDorHMenu, LPVOID lpParam /*= NULL*/)
	// Creates the window by specifying all the window creation parameters
	{
		assert( GetApp() );		// Test if Win32++ has been started
		assert(!::IsWindow(m_hWnd));	// Only one window per CWnd instance allowed

		try
		{
			// Prepare the CWnd if it has been reused
			Destroy();

			// Ensure a window class is registered
			CString ClassName;
			if (lpszClassName == 0 || lpszClassName[0] == _T('\0'))
				ClassName = _T("Win32++ Window");
			else
				ClassName = lpszClassName;

			WNDCLASS wc;
			ZeroMemory(&wc, sizeof(WNDCLASS));
			wc.lpszClassName = ClassName;
			wc.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
			wc.hCursor		 = ::LoadCursor(NULL, IDC_ARROW);

			// Register the window class (if not already registered)
			if (RegisterClass(wc) == 0)
				throw CWinException(_T("Failed to register window class"));

			// Ensure this thread has the TLS index set
			TLSData* pTLSData = GetApp()->SetTlsData();

			// Store the CWnd pointer in thread local storage
			pTLSData->pWnd = this;

			// Create window
			m_hWnd = ::CreateWindowEx(dwExStyle, ClassName, lpszWindowName, dwStyle, x, y, nWidth, nHeight,
									hWndParent, nIDorHMenu, GetApp()->GetInstanceHandle(), lpParam);

			// Now handle window creation failure
			if (m_hWnd == 0)
				throw CWinException(_T("Failed to Create Window"));

			// Automatically subclass predefined window class types
			::GetClassInfo(GetApp()->GetInstanceHandle(), lpszClassName, &wc);
			if (wc.lpfnWndProc != GetApp()->m_Callback)
			{
				Subclass(m_hWnd);

				// Send a message to force the HWND to be added to the map
				SendMessage(WM_NULL, 0L, 0L);

				// Override this to perform tasks after the window is attached.
				OnAttach();
			}

			// Clear the CWnd pointer from TLS
			pTLSData->pWnd = NULL;
		}

		catch (const CWinException &e)
		{
			TRACE("\n*** Failed to create window ***\n");
			e.what();	// Display the last error message.

			// eat the exception (don't rethrow)
		}

		// Window creation is complete. Now call OnInitialUpdate
		PostMessage(UWM_WINDOWCREATED);

		return m_hWnd;
	}

	inline void CWnd::Destroy()
	// Destroys the window and returns the CWnd back to its default state, ready for reuse.
	{
		if (GetCWndPtr(m_hWnd) == this)
		{
			if (IsWindow())
				::DestroyWindow(m_hWnd);
		}

		// Return the CWnd to its default state
		Cleanup();
	}

	inline HWND CWnd::Detach()
	// Reverse an Attach
	{
		assert(IsWindow());
		assert(m_PrevWindowProc);	// Only previously attached CWnds can be detached

		SetWindowLongPtr(GWLP_WNDPROC, (LONG_PTR)m_PrevWindowProc);

		HWND hWnd = m_hWnd;
		Cleanup();

		return hWnd;
	}

	inline LRESULT CWnd::FinalWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	// Pass messages on to the appropriate default window procedure
	// CMDIChild and CMDIFrame override this function
	{
		if (m_PrevWindowProc)
			return ::CallWindowProc(m_PrevWindowProc, m_hWnd, uMsg, wParam, lParam);
		else
			return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

#ifdef USE_OBSOLETE_CODE
	inline CWnd* CWnd::FromHandle(HWND hWnd)
	// Returns the CWnd object associated with the window handle
	{
		TRACE("WARNING!! CWnd::FromHandle is obsolete. \n");

		assert( GetApp() );

		// Find any existing permanent CWnd from the map
		CWnd* pWnd = GetCWndPtr(hWnd);
		if ( NULL != hWnd && 0 == pWnd )
		{
			// Find any existing temporary CWnd for the HWND
			TLSData* pTLSData = GetApp()->SetTlsData();
			std::map<HWND, WndPtr, CompareHWND>::iterator m;
			m = pTLSData->TmpWnds.find(hWnd);

			if (m != pTLSData->TmpWnds.end())
				pWnd = m->second.get();

			if (!pWnd)
			{
				// No exiting CWnd for this HWND, so create one
				pWnd = new CWnd;
				pWnd->m_hWnd = hWnd;
				pWnd->m_IsManagedHwnd = FALSE;
				pTLSData->TmpWnds.insert(std::make_pair(hWnd, pWnd));

				::PostMessage(hWnd, UWM_CLEANUPTEMPS, 0, 0);
			}
		}

		return pWnd;
	}
#endif

	inline CWnd* CWnd::GetCWndPtr(HWND hWnd)
	// Retrieves the pointer to the CWnd associated with the specified HWND.
	{
		assert( GetApp() );
		return hWnd? GetApp()->GetCWndFromMap(hWnd) : 0;
	}

	inline CWnd CWnd::GetAncestor(UINT gaFlags /*= GA_ROOTOWNER*/) const
	// The GetAncestor function retrieves the ancestor (root parent)
	// of the window. Supports Win95.
	{
		assert(IsWindow());
		HWND hWnd = m_hWnd;

		// Load the User32 DLL
		typedef HWND WINAPI GETANCESTOR(HWND, UINT);
		GETANCESTOR* pfnGetAncestor = NULL;
		HMODULE hModule = ::LoadLibrary(_T("USER32.DLL"));

		if (hModule != 0)
		{
			// Declare a pointer to the GetAncestor function
#ifndef _WIN32_WCE
			pfnGetAncestor = (GETANCESTOR*)::GetProcAddress(hModule, "GetAncestor");
#else
			pfnGetAncestor = (GETANCESTOR*)::GetProcAddress(hModule, L"GetAncestor");
#endif

			if (pfnGetAncestor)
				hWnd = (*pfnGetAncestor)(m_hWnd, gaFlags);

			::FreeLibrary(hModule);
		}

		if (!pfnGetAncestor)
		{
			// Provide our own GetAncestor if necessary
			HWND hWndParent = ::GetParent(hWnd);
			while (::IsChild(hWndParent, hWnd))
			{
				hWnd = hWndParent;
				hWndParent = ::GetParent(hWnd);
			}
		}

		return CWnd(hWnd);
	}

	inline CString CWnd::GetClassName() const
	// Retrieves the name of the class to which the specified window belongs.
	{
		assert(IsWindow());

		CString str;
		::GetClassName(m_hWnd, str.GetBuffer(MAX_STRING_SIZE), MAX_STRING_SIZE);
		str.ReleaseBuffer();
		return str;
	}

	inline CString CWnd::GetDlgItemText(int nIDDlgItem) const
	// Retrieves the title or text associated with a control in a dialog box.
	{
		assert(IsWindow());

		int nLength = ::GetWindowTextLength(::GetDlgItem(m_hWnd, nIDDlgItem));
		CString str;
		::GetDlgItemText(m_hWnd, nIDDlgItem, str.GetBuffer(nLength), nLength+1);
		str.ReleaseBuffer();
		return str;
	}

	inline CString CWnd::GetWindowText() const
	// Retrieves the text of the window's title bar.
	{
		assert(IsWindow());

		int nLength = ::GetWindowTextLength(m_hWnd);
		CString str;
		::GetWindowText(m_hWnd, str.GetBuffer(nLength), nLength+1);
		str.ReleaseBuffer();
		return str;
	}

	inline void CWnd::OnClose()
	// Called in response to WM_CLOSE. Override to suppress destroying the window
	// A WM_CLOSE is sent by SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0) or by clicking X
	//  in the top right corner. Usually, only top level windows send a WM_CLOSE.
	{
		::DestroyWindow(m_hWnd);
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

	inline void CWnd::OnAttach()
	{
		// This function is called when a window is attached to the CWnd.
		// Override it to automatically perform tasks when the window is attached.
		// Note:  Window controls are attached.
	}

	inline int CWnd::OnCreate(LPCREATESTRUCT pcs)
	{
		// This function is called when a WM_CREATE message is received
		// Override it to automatically perform tasks during window creation.
		// Return 0 to continue creating the window.

		// Note: Window controls don't call OnCreate. They are sublcassed (attached)
		//  after their window is created.

		UNREFERENCED_PARAMETER (pcs);
		return 0;
	}

	inline void CWnd::OnDestroy()
	{
		// This function is called when a window is destroyed.
		// Override it to do additional tasks, such as ending the application
		//  with PostQuitMessage.
	}

	inline void CWnd::OnDraw(CDC& dc)
	// Called when part of the client area of the window needs to be drawn
	{
		UNREFERENCED_PARAMETER(dc);

	    // Override this function in your derived class to perform drawing tasks.
	}

	inline BOOL CWnd::OnEraseBkgnd(CDC& dc)
	// Called when the background of the window's client area needs to be erased.
	{
		UNREFERENCED_PARAMETER(dc);

	    // Override this function in your derived class to perform drawing tasks.

		// Return Value: Return FALSE to also permit default erasure of the background
		//				 Return TRUE to prevent default erasure of the background

		return FALSE;
	}

#ifdef USE_OBSOLETE_CODE
	inline void CWnd::OnDraw(CDC* pDC)
	// Called when part of the client area of the window needs to be drawn
	{
		UNREFERENCED_PARAMETER(pDC);

	    // Override this function in your derived class to perform drawing tasks.
	}

	inline BOOL CWnd::OnEraseBkgnd(CDC* pDC)
	// Called when the background of the window's client area needs to be erased.
	{
		UNREFERENCED_PARAMETER(pDC);

	    // Override this function in your derived class to perform drawing tasks.

		// Return Value: Return FALSE to also permit default erasure of the background
		//				 Return TRUE to prevent default erasure of the background

		return FALSE;
	}
#endif

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
		// Override this function in your derived class to handle these special messages.

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
		UNREFERENCED_PARAMETER(cs);

		// Override this function to set the CREATESTRUCT values prior to window creation.
		// Here we set the initial values for the following:
		//  window styles (WS_VISABLE, WS_CHILD, WS_WS_MAXIMIZEBOX etc.)
		//  window extended styles
		//  window position
		//  window menu
		//  window class name
		//  window name (caption)
	}

	inline void CWnd::PreRegisterClass(WNDCLASS& wc)
	// Called by CWnd::Create to set some window parameters.
	// Used to set the window type (ClassName) and for setting the background brush and cursor.
	{
		UNREFERENCED_PARAMETER(wc);

		// Override this function to set the WNDCLASS values prior to window creation.
		// for example, for a ToolBar, we use this:
		// wc.lpszClassName = TOOLBARCLASSNAME;

		// ADDITIONAL NOTES:
		// 1) The lpszClassName must be set for this function to take effect.
		// 2) No other defaults are set, so the following settings might prove useful
		//     wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		//     wc.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
		//     wc.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
		// 3) The styles that can be set here are WNDCLASS styles. These are a different
		//     set of styles to those set by CREATESTRUCT (used in PreCreate).
		// 4) RegisterClassEx is not used because its not supported on WinCE.
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
	// A private function used by the CreateEx function to register a window
	// class prior to window creation
	{
		assert( GetApp() );
		assert( ('\0' != wc.lpszClassName[0] && ( lstrlen(wc.lpszClassName) <=  MAX_STRING_SIZE) ) );

		// Check to see if this classname is already registered
		WNDCLASS wcTest;
		ZeroMemory(&wcTest, sizeof(WNDCLASS));
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
		BOOL Success = FALSE;

		if (GetApp())
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
						Success = TRUE;
						break;
					}
				}

				pApp->m_csMapLock.Release();
			}
		}

		return Success;
	}

	inline HICON CWnd::SetIconLarge(int nIcon)
	// Sets the large icon associated with the window
	{
		assert( GetApp() );
		assert(IsWindow());

		HICON hIconLarge = (HICON) (::LoadImage (GetApp()->GetResourceHandle(), MAKEINTRESOURCE (nIcon), IMAGE_ICON,
		::GetSystemMetrics (SM_CXICON), ::GetSystemMetrics (SM_CYICON), 0));

		if (hIconLarge != 0)
			SendMessage (WM_SETICON, WPARAM (ICON_BIG), LPARAM (hIconLarge));
		else
			TRACE("**WARNING** SetIconLarge Failed\n");

		return hIconLarge;
	}

	inline HICON CWnd::SetIconSmall(int nIcon)
	// Sets the small icon associated with the window
	{
		assert( GetApp() );
		assert(IsWindow());

		HICON hIconSmall = (HICON) (::LoadImage (GetApp()->GetResourceHandle(), MAKEINTRESOURCE (nIcon), IMAGE_ICON,
		::GetSystemMetrics (SM_CXSMICON), ::GetSystemMetrics (SM_CYSMICON), 0));

		if (hIconSmall != 0)
			SendMessage (WM_SETICON, WPARAM (ICON_SMALL), LPARAM (hIconSmall));
		else
			TRACE("**WARNING** SetIconSmall Failed\n");

		return hIconSmall;
	}

	inline LRESULT CALLBACK CWnd::StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	// All CWnd windows direct their messages here. This function redirects the message
	// to the CWnd's WndProc function.
	{
		assert( GetApp() );

		CWnd* w = GetApp()->GetCWndFromMap(hWnd);
		if (w == 0)
		{
			// The CWnd pointer wasn't found in the map, so add it now

			// Retrieve the pointer to the TLS Data
			TLSData* pTLSData = GetApp()->GetTlsData();
			assert(pTLSData);

			// Retrieve pointer to CWnd object from Thread Local Storage TLS
			w = pTLSData->pWnd;
			assert(w);				// pTLSData->pCWnd is assigned in CreateEx
			pTLSData->pWnd = NULL;

			// Store the CWnd pointer in the HWND map
			w->m_hWnd = hWnd;
			w->AddToMap();
		}

		return w->WndProc(uMsg, wParam, lParam);

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
		// Override this function in your class derived from CWnd to handle
		//  window messages. A typical function might look like this:

		//	switch (uMsg)
		//	{
		//	case MESSAGE1:	return OnMessage1();
		//	case MESSAGE2:  return OnMessage2();
		//	}

		// The message functions should return a value recommended by the Windows API documentation.
		// Alternatively, return FinalWindowProc to continue with default processing.

		// Always pass unhandled messages on to WndProcDefault
		return WndProcDefault(uMsg, wParam, lParam);
	}

	inline LRESULT CWnd::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	// All WndProc functions should pass unhandled window messages to this function
	{
		LRESULT lr = 0L;
		if (UWM_WINDOWCREATED == uMsg)
		{
			OnInitialUpdate();
			return 0L;
		}

    	switch (uMsg)
		{
		case WM_CLOSE:
			{
				OnClose();
				return 0L;
			}
		case WM_COMMAND:
			{
				// Reflect this message if it's from a control
				CWnd* pWnd = GetApp()->GetCWndFromMap((HWND)lParam);
				if (pWnd != NULL)
					lr = pWnd->OnCommand(wParam, lParam);

				// Handle user commands
				if (0L == lr)
					lr =  OnCommand(wParam, lParam);

				if (0L != lr) return 0L;
			}
			break;  // Note: Some MDI commands require default processing
		case WM_CREATE:
			return OnCreate((LPCREATESTRUCT) lParam);
		case WM_DESTROY:
			OnDestroy();
			break;	// Note: Some controls require default processing.
		case WM_NOTIFY:
			{
				// Do Notification reflection if it came from a CWnd object
				HWND hwndFrom = ((LPNMHDR)lParam)->hwndFrom;
				CWnd* pWndFrom = GetApp()->GetCWndFromMap(hwndFrom);

				// Skip notification reflection for rebars and tabs to avoid double handling
				CString ClassName = GetClassName();
				if ( (ClassName != _T("ReBarWindow32")) && (ClassName != _T("SysTabControl32")) )
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
				if (lr == 0L) lr = OnNotify(wParam, lParam);
				if (lr != 0L) return lr;
				break;
			}

		case WM_PAINT:
			{
				// Subclassed controls expect to do their own painting.
				// CustomDraw or OwnerDraw are normally used to modify the drawing of controls.
				if (m_PrevWindowProc) break;

				if (::GetUpdateRect(m_hWnd, NULL, FALSE))
				{
					CPaintDC dc(*this);

#ifdef USE_OBSOLETE_CODE
					OnDraw(&dc);
#endif

					OnDraw(dc);
				}
				else
				// RedrawWindow can require repainting without an update rect
				{
					CClientDC dc(*this);

#ifdef USE_OBSOLETE_CODE
					OnDraw(&dc);
#endif

					OnDraw(dc);
				}
			}
			return 0L;

		case WM_ERASEBKGND:
			{
				CDC dc((HDC)wParam);
				BOOL bResult;

#ifdef USE_OBSOLETE_CODE
				bResult = OnEraseBkgnd(&dc);
#endif
				
				bResult = OnEraseBkgnd(dc);
				if (bResult) return TRUE;
			}
			break;

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
			//	if (m_PrevWindowProc) break; // Suppress for subclassed windows

				lr = MessageReflect(m_hWnd, uMsg, wParam, lParam);
				if (lr != 0L) return lr;	// Message processed so return
			}
			break;				// Do default processing when message not already processed

		case UWM_UPDATECOMMAND:
			OnMenuUpdate((UINT)wParam); // Perform menu updates
		break;

		} // switch (uMsg)

		// Now hand all messages to the default procedure
		return FinalWindowProc(uMsg, wParam, lParam);

	} // LRESULT CWnd::WindowProc(...)


	//
	// Wrappers for Win32 API functions
	//

	inline HDC CWnd::BeginPaint(PAINTSTRUCT& ps) const
	// The BeginPaint function prepares the specified window for painting and fills a PAINTSTRUCT
	// structure with information about the painting. Consider using CPaintDC instead.
	{
        assert(IsWindow());
		return ::BeginPaint(m_hWnd, &ps);
	}

	inline BOOL CWnd::BringWindowToTop() const
	// The BringWindowToTop function brings the specified window to the top
	// of the Z order. If the window is a top-level window, it is activated.
	{
        assert(IsWindow());
		return ::BringWindowToTop(m_hWnd);
	}

	inline LRESULT CWnd::CallWindowProc(WNDPROC lpPrevWndFunc, UINT Msg, WPARAM wParam, LPARAM lParam) const
	{
        assert(IsWindow());
		return ::CallWindowProc(lpPrevWndFunc, m_hWnd, Msg, wParam, lParam);
	}

	inline BOOL CWnd::CheckDlgButton(int nIDButton, UINT uCheck) const
	// The CheckDlgButton function changes the check state of a button control.
	{
        assert(IsWindow());
		return ::CheckDlgButton(m_hWnd, nIDButton, uCheck);
	}

	inline BOOL CWnd::CheckRadioButton(int nIDFirstButton, int nIDLastButton, int nIDCheckButton) const
	// The CheckRadioButton function adds a check mark to (checks) a specified radio button in a group
	// and removes a check mark from (clears) all other radio buttons in the group.
	{
		assert(IsWindow());
		return ::CheckRadioButton(m_hWnd, nIDFirstButton, nIDLastButton, nIDCheckButton);
	}

	inline CWnd CWnd::ChildWindowFromPoint(POINT pt) const
	// Determines which, if any, of the child windows belonging to a parent window contains
	// the specified point. The search is restricted to immediate child windows.
	// Grandchildren, and deeper descendant windows are not searched.
	{
		assert(IsWindow());
		return CWnd(::ChildWindowFromPoint(m_hWnd, pt));
	}

	inline BOOL CWnd::ClientToScreen(POINT& pt) const
	// The ClientToScreen function converts the client-area coordinates of a specified point to screen coordinates.
	{
		assert(IsWindow());
		return ::ClientToScreen(m_hWnd, &pt);
	}

	inline BOOL CWnd::ClientToScreen(RECT& rc) const
	// The ClientToScreen function converts the client-area coordinates of a specified RECT to screen coordinates.
	{
		assert(IsWindow());
		return (BOOL)::MapWindowPoints(m_hWnd, NULL, (LPPOINT)&rc, 2);
	}

	inline HDWP CWnd::DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) const
	// The DeferWindowPos function updates the specified multiple-window – position structure for the window.
	// The hWndInsertAfter can one of:  HWND_BOTTOM, HWND_NOTOPMOST, HWND_TOP, or HWND_TOPMOST
	{
        assert(IsWindow());
		return ::DeferWindowPos(hWinPosInfo, m_hWnd, hWndInsertAfter, x, y, cx, cy, uFlags);
	}

	inline HDWP CWnd::DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, const RECT& rc, UINT uFlags) const
	// The DeferWindowPos function updates the specified multiple-window – position structure for the window.
	// The hWndInsertAfter can one of:  HWND_BOTTOM, HWND_NOTOPMOST, HWND_TOP, or HWND_TOPMOST
	{
		assert(IsWindow());
		return ::DeferWindowPos(hWinPosInfo, m_hWnd, hWndInsertAfter, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, uFlags);
	}

	inline LRESULT CWnd::DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam) const
	// This function provides default processing for any window messages that an application does not process.
	{
		assert(IsWindow());
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	inline BOOL CWnd::DrawMenuBar() const
	// The DrawMenuBar function redraws the menu bar of the specified window. If the menu bar changes after
	// the system has created the window, this function must be called to draw the changed menu bar.
	{
		assert(IsWindow());
		return ::DrawMenuBar(m_hWnd);
	}

	inline BOOL CWnd::EnableWindow(BOOL bEnable /*= TRUE*/) const
	// The EnableWindow function enables or disables mouse and
	// keyboard input to the window.
	{
		assert(IsWindow());
		return ::EnableWindow(m_hWnd, bEnable);
	}

	inline BOOL CWnd::EndPaint(PAINTSTRUCT& ps) const
	// The EndPaint function marks the end of painting in the specified window. This function is required for
	// each call to the BeginPaint function, but only after painting is complete.
	{
		assert(IsWindow());
		return ::EndPaint(m_hWnd, &ps);
	}

	inline CWnd CWnd::GetActiveWindow() const
	// The GetActiveWindow function retrieves the active window attached to the calling
	// thread's message queue.
	{
		return CWnd(::GetActiveWindow() );
	}

	inline CWnd CWnd::GetCapture() const
	// The GetCapture function retrieves the window (if any) that has captured the mouse.
	{
		return CWnd( ::GetCapture() );
	}

	inline ULONG_PTR CWnd::GetClassLongPtr(int nIndex) const
	// The GetClassLongPtr function retrieves the specified value from the
	// WNDCLASSEX structure associated with the window.
	{
		assert(IsWindow());
		return ::GetClassLongPtr(m_hWnd, nIndex);
	}

	inline CRect CWnd::GetClientRect() const
	// The GetClientRect function retrieves the coordinates of a window's client area.
	// The client coordinates specify the upper-left and lower-right corners of the
	// client area. Because client coordinates are relative to the upper-left corner
	// of a window's client area, the coordinates of the upper-left corner are (0,0).
	{
		assert(IsWindow());
		CRect rc;
		::GetClientRect(m_hWnd, &rc);
		return rc;
	}

	inline CDC CWnd::GetDC() const
	// The GetDC function retrieves the display device context (DC)
	// for the client area of the window. Use like this:
	// CDC dc = GetDC;
	{
		assert(IsWindow());
		return CClientDC(m_hWnd);
	}

	inline CDC CWnd::GetDCEx(HRGN hrgnClip, DWORD flags) const
	// The GetDCEx function retrieves a display device context (DC) for the
	// client area or entire area of a window. Use like this:
	// CDC dc = GetDCEx(hrgnClip, flags);
	{
		assert(IsWindow());
		return CClientDCEx(m_hWnd, hrgnClip, flags);
	}

	inline CWnd CWnd::GetDesktopWindow() const
	// The GetDesktopWindow function retrieves the desktop window.
	{
		return CWnd( ::GetDesktopWindow() );
	}

	inline int CWnd::GetDlgCtrlID() const
	// Retrieves the control ID value for any child window
	{
		assert(IsWindow());
		return ::GetDlgCtrlID(m_hWnd);
	}

	inline CWnd CWnd::GetDlgItem(int nIDDlgItem) const
	// The GetDlgItem function retrieves a handle to a control in the dialog box.
	{
		assert(IsWindow());
		return CWnd( ::GetDlgItem(m_hWnd, nIDDlgItem) );
	}

	inline UINT CWnd::GetDlgItemInt(int nIDDlgItem, BOOL* lpTranslated, BOOL bSigned) const
	// The GetDlgItemInt function translates the text of a specified control in a dialog box into an integer value.
	{
		assert(IsWindow());
		return ::GetDlgItemInt(m_hWnd, nIDDlgItem, lpTranslated, bSigned);
	}

	inline CWnd CWnd::GetFocus() const
	// The GetFocus function retrieves the window that has the keyboard focus, if the window
	// is attached to the calling thread's message queue.
	{
		return CWnd( ::GetFocus() );
	}

	inline CFont CWnd::GetFont() const
	// Retrieves the font with which the window is currently drawing its text.
	{
		assert(IsWindow());
		HFONT hFont = (HFONT)SendMessage(WM_GETFONT, 0, 0);
		return CFont(hFont);
	}

	inline HICON CWnd::GetIcon(BOOL bBigIcon) const
	// Retrieves a handle to the large or small icon associated with a window.
	{
		assert(IsWindow());
		return (HICON)SendMessage(WM_GETICON, (WPARAM)bBigIcon, 0);
	}

	inline CWnd CWnd::GetNextDlgGroupItem(HWND hCtl, BOOL bPrevious) const
	// The GetNextDlgGroupItem function retrieves the first control in a group of controls that
	// precedes (or follows) the specified control in a dialog box.
	{
		assert(IsWindow());
		return CWnd(::GetNextDlgGroupItem(m_hWnd, hCtl, bPrevious));
	}

	inline CWnd CWnd::GetNextDlgTabItem(HWND hCtl, BOOL bPrevious) const
	// The GetNextDlgTabItem function retrieves the first control that has the WS_TABSTOP style
	// that precedes (or follows) the specified control.
	{
		assert(IsWindow());
		return CWnd(::GetNextDlgTabItem(m_hWnd, hCtl, bPrevious));
	}

	inline CWnd CWnd::GetParent() const
	// The GetParent function retrieves the specified window's parent or owner.
	{
		assert(IsWindow());
		return CWnd(::GetParent(m_hWnd));
	}

	inline LONG_PTR CWnd::GetWindowLongPtr(int nIndex) const
	// The GetWindowLongPtr function retrieves information about the window.
	{
		assert(IsWindow());
		return ::GetWindowLongPtr(m_hWnd, nIndex);
	}

	inline BOOL CWnd::GetScrollInfo(int fnBar, SCROLLINFO& si) const
	// The GetScrollInfo function retrieves the parameters of a scroll bar, including
	// the minimum and maximum scrolling positions, the page size, and the position
	// of the scroll box (thumb).
	{
		assert(IsWindow());
		return ::GetScrollInfo(m_hWnd, fnBar, &si);
	}

	inline CRect CWnd::GetUpdateRect(BOOL bErase) const
	// The GetUpdateRect function retrieves the coordinates of the smallest rectangle that completely
	// encloses the update region of the specified window.
	{
		assert(IsWindow());
		CRect rc;
		::GetUpdateRect(m_hWnd, &rc, bErase);
		return rc;
	}

	inline int CWnd::GetUpdateRgn(HRGN hRgn, BOOL bErase) const
	// The GetUpdateRgn function retrieves the update region of a window by copying it into the specified region.
	{
		assert(IsWindow());
		return ::GetUpdateRgn(m_hWnd, hRgn, bErase);
	}

	inline CWnd CWnd::GetWindow(UINT uCmd) const
	// The GetWindow function retrieves a window that has the specified
	// relationship (Z-Order or owner) to the specified window.
	// Possible uCmd values: GW_CHILD, GW_ENABLEDPOPUP, GW_HWNDFIRST, GW_HWNDLAST,
	// GW_HWNDNEXT, GW_HWNDPREV, GW_OWNER
	{
		assert(IsWindow());
		return CWnd( ::GetWindow(m_hWnd, uCmd) );
	}

	inline CDC CWnd::GetWindowDC() const
	// The GetWindowDC function retrieves the device context (DC) for the entire window, 
	// including title bar, menus, and scroll bars. Use like this:
	// CDC dc = GetWindowDC();
	{
		assert(IsWindow());
		return CWindowDC(m_hWnd);
	}

	inline CRect CWnd::GetWindowRect() const
	// retrieves the dimensions of the bounding rectangle of the window.
	// The dimensions are given in screen coordinates that are relative to the
	// upper-left corner of the screen.
	{
		assert(IsWindow());
		CRect rc;
		::GetWindowRect(m_hWnd, &rc);
		return rc;
	}

	inline int CWnd::GetWindowTextLength() const
	// The GetWindowTextLength function retrieves the length, in characters, of the specified window's
	// title bar text (if the window has a title bar).
	{
		assert(IsWindow());
		return ::GetWindowTextLength(m_hWnd);
	}

	inline void CWnd::Invalidate(BOOL bErase /*= TRUE*/) const
	// The Invalidate function adds the entire client area the window's update region.
	// The update region represents the portion of the window's client area that must be redrawn.
	{
		assert(IsWindow());
		::InvalidateRect(m_hWnd, NULL, bErase);
	}

	inline BOOL CWnd::InvalidateRect(LPCRECT lpRect, BOOL bErase /*= TRUE*/) const
	// The InvalidateRect function adds a rectangle to the window's update region.
	// The update region represents the portion of the window's client area that must be redrawn.
	{
		assert(IsWindow());
		return ::InvalidateRect(m_hWnd, lpRect, bErase);
	}

	inline BOOL CWnd::InvalidateRgn(HRGN hRgn, BOOL bErase /*= TRUE*/) const
	// The InvalidateRgn function invalidates the client area within the specified region
	// by adding it to the current update region of a window. The invalidated region,
	// along with all other areas in the update region, is marked for painting when the
	// next WM_PAINT message occurs.
	{
		assert(IsWindow());
		return ::InvalidateRgn(m_hWnd, hRgn, bErase);
	}

	inline BOOL CWnd::IsChild(HWND hwndChild) const
	// The IsChild function tests whether a window is a child window or descendant window
	// of a parent window's CWnd.
	{
		assert(IsWindow());
		return ::IsChild(m_hWnd, hwndChild);
	}

	inline BOOL CWnd::IsDialogMessage(LPMSG lpMsg) const
	// The IsDialogMessage function determines whether a message is intended for the specified dialog box and,
	// if it is, processes the message.
	{
		assert(IsWindow());
		return ::IsDialogMessage(m_hWnd, lpMsg);
	}

	inline UINT CWnd::IsDlgButtonChecked(int nIDButton) const
	// The IsDlgButtonChecked function determines whether a button control has a check mark next to it
	// or whether a three-state button control is grayed, checked, or neither.
	{
		assert(IsWindow());
		return ::IsDlgButtonChecked(m_hWnd, nIDButton);
	}

	inline BOOL CWnd::IsWindowEnabled() const
	// The IsWindowEnabled function determines whether the window is enabled
	// for mouse and keyboard input.
	{
		assert(IsWindow());
		return ::IsWindowEnabled(m_hWnd);
	}

	inline BOOL CWnd::IsWindowVisible() const
	// The IsWindowVisible function retrieves the visibility state of the window.
	{
		assert(IsWindow());
		return ::IsWindowVisible(m_hWnd);
	}

	inline BOOL CWnd::IsWindow() const
	// The IsWindow function determines whether the window exists.
	{
		return ::IsWindow(m_hWnd);
	}

	inline void  CWnd::MapWindowPoints(HWND hWndTo, POINT& pt) const
	// The MapWindowPoints function converts (maps) a set of points from a coordinate space relative to one
	// window to a coordinate space relative to another window.
	{
		assert(IsWindow());
		::MapWindowPoints(m_hWnd, hWndTo, &pt, 1);
	}

	inline void CWnd::MapWindowPoints(HWND hWndTo, RECT& rc) const
	// The MapWindowPoints function converts (maps) a set of points from a coordinate space relative to one
	// window to a coordinate space relative to another window.
	{
		assert(IsWindow());
		::MapWindowPoints(m_hWnd, hWndTo, (LPPOINT)&rc, 2);
	}

	inline void CWnd::MapWindowPoints(HWND hWndTo, LPPOINT ptArray, UINT nCount) const
	// The MapWindowPoints function converts (maps) a set of points from a coordinate space relative to one
	// window to a coordinate space relative to another window.
	{
		assert(IsWindow());
		::MapWindowPoints(m_hWnd, hWndTo, (LPPOINT)ptArray, nCount);
	}

	inline int CWnd::MessageBox(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) const
	// The MessageBox function creates, displays, and operates a message box.
	// Possible combinations of uType values include: MB_OK, MB_HELP, MB_OKCANCEL, MB_RETRYCANCEL,
	// MB_YESNO, MB_YESNOCANCEL, MB_ICONEXCLAMATION, MB_ICONWARNING, MB_ICONERROR (+ many others).
	{
		assert(IsWindow());
		return ::MessageBox(m_hWnd, lpText, lpCaption, uType);
	}

	inline BOOL CWnd::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint /* = TRUE*/) const
	// The MoveWindow function changes the position and dimensions of the window.
	{
		assert(IsWindow());
		return ::MoveWindow(m_hWnd, x, y, nWidth, nHeight, bRepaint = TRUE);
	}

	inline BOOL CWnd::MoveWindow(const RECT& rc, BOOL bRepaint /* = TRUE*/) const
	// The MoveWindow function changes the position and dimensions of the window.
	{
		assert(IsWindow());
		return ::MoveWindow(m_hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, bRepaint);
	}

	inline BOOL CWnd::PostMessage(UINT uMsg, WPARAM wParam /*= 0L*/, LPARAM lParam /*= 0L*/) const
	// The PostMessage function places (posts) a message in the message queue
	// associated with the thread that created the window and returns without
	// waiting for the thread to process the message.
	{
		assert(IsWindow());
		return ::PostMessage(m_hWnd, uMsg, wParam, lParam);
	}

	inline BOOL CWnd::PostMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
	// Required by by some macros
	{
		assert(IsWindow());
		return ::PostMessage(hWnd, uMsg, wParam, lParam);
	}

	inline BOOL CWnd::RedrawWindow(LPCRECT lpRectUpdate, HRGN hRgn, UINT flags) const
	// The RedrawWindow function updates the specified rectangle or region in a window's client area.
	{
		assert(IsWindow());
		return ::RedrawWindow(m_hWnd, lpRectUpdate, hRgn, flags);
	}

	inline int CWnd::ReleaseDC(HDC hDC) const
	// The ReleaseDC function releases a device context (DC), freeing it for use
	// by other applications.
	{
		assert(IsWindow());
		return ::ReleaseDC(m_hWnd, hDC);
	}

	inline BOOL CWnd::ScreenToClient(POINT& Point) const
	// The ScreenToClient function converts the screen coordinates of a specified point on the screen to client-area coordinates.
	{
		assert(IsWindow());
		return ::ScreenToClient(m_hWnd, &Point);
	}

	inline BOOL CWnd::ScreenToClient(RECT& rc) const
	// The ScreenToClient function converts the screen coordinates of a specified RECT on the screen to client-area coordinates.
	{
		assert(IsWindow());
		return (BOOL)::MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rc, 2);
	}

	inline LRESULT CWnd::SendDlgItemMessage(int nIDDlgItem, UINT Msg, WPARAM wParam, LPARAM lParam) const
	// The SendDlgItemMessage function sends a message to the specified control in a dialog box.
	{
		assert(IsWindow());
		return ::SendDlgItemMessage(m_hWnd, nIDDlgItem, Msg, wParam, lParam);
	}

	inline LRESULT CWnd::SendMessage(UINT uMsg, WPARAM wParam /*= 0L*/, LPARAM lParam /*= 0L*/) const
	// The SendMessage function sends the specified message to a window or windows.
	// It calls the window procedure for the window and does not return until the
	// window procedure has processed the message.
	{
		assert(IsWindow());
		return ::SendMessage(m_hWnd, uMsg, wParam, lParam);
	}

	inline LRESULT CWnd::SendMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
	// Required by by some macros
	{
		assert(IsWindow());
		return ::SendMessage(hWnd, uMsg, wParam, lParam);
	}

	inline BOOL CWnd::SendNotifyMessage(UINT Msg, WPARAM wParam, LPARAM lParam) const
	// The SendNotifyMessage function sends the specified message to a window or windows. If the window was created by the
	// calling thread, SendNotifyMessage calls the window procedure for the window and does not return until the window procedure
	// has processed the message. If the window was created by a different thread, SendNotifyMessage passes the message to the
	// window procedure and returns immediately; it does not wait for the window procedure to finish processing the message.
	{
		assert(IsWindow());
		return ::SendNotifyMessage(m_hWnd, Msg, wParam, lParam);
	}

	inline CWnd CWnd::SetActiveWindow() const
	// The SetActiveWindow function activates the window, but
	// not if the application is in the background.
	{
		assert(IsWindow());
		return CWnd( ::SetActiveWindow(m_hWnd) );
	}

	inline CWnd CWnd::SetCapture() const
	// The SetCapture function sets the mouse capture to the window.
	// SetCapture captures mouse input either when the mouse is over the capturing
	// window, or when the mouse button was pressed while the mouse was over the
	// capturing window and the button is still down.
	{
		assert(IsWindow());
		return CWnd( ::SetCapture(m_hWnd) );
	}

	inline ULONG_PTR CWnd::SetClassLongPtr(int nIndex, LONG_PTR dwNewLong) const
	// The SetClassLongPtr function replaces the specified value at the specified offset in the
	// extra class memory or the WNDCLASSEX structure for the class to which the window belongs.
	{
		assert(IsWindow());
		return ::SetClassLongPtr(m_hWnd, nIndex, dwNewLong);
	}

	inline CWnd CWnd::SetFocus() const
	// The SetFocus function sets the keyboard focus to the window.
	{
		assert(IsWindow());
		return CWnd( ::SetFocus(m_hWnd) );
	}

	inline void CWnd::SetFont(HFONT hFont, BOOL bRedraw /* = TRUE*/) const
	// Specifies the font that the window will use when drawing text.
	{
		assert(IsWindow());
		SendMessage(WM_SETFONT, (WPARAM)hFont, (LPARAM)bRedraw);
	}

	inline HICON CWnd::SetIcon(HICON hIcon, BOOL bBigIcon) const
	// Associates a new large or small icon with a window.
	{
		assert(IsWindow());
		return (HICON)SendMessage(WM_SETICON, (WPARAM)bBigIcon, (LPARAM)hIcon);
	}

	inline BOOL CWnd::SetForegroundWindow() const
	// The SetForegroundWindow function puts the thread that created the window into the
	// foreground and activates the window.
	{
		assert(IsWindow());
		return ::SetForegroundWindow(m_hWnd);
	}

	inline CWnd CWnd::SetParent(HWND hWndParent) const
	// The SetParent function changes the parent window of the child window.
	{
		assert(IsWindow());
		return CWnd(::SetParent(m_hWnd, hWndParent));
	}

	inline BOOL CWnd::SetRedraw(BOOL bRedraw /*= TRUE*/) const
	// This function allows changes in that window to be redrawn or prevents changes
	// in that window from being redrawn.
	{
		assert(IsWindow());
		return (BOOL)::SendMessage(m_hWnd, WM_SETREDRAW, (WPARAM)bRedraw, 0L);
	}

	inline LONG_PTR CWnd::SetWindowLongPtr(int nIndex, LONG_PTR dwNewLong) const
	// The SetWindowLongPtr function changes an attribute of the window.
	{
		assert(IsWindow());
		return ::SetWindowLongPtr(m_hWnd, nIndex, dwNewLong);
	}

	inline BOOL CWnd::SetWindowPos(HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) const
	// The SetWindowPos function changes the size, position, and Z order of a child, pop-up,
	// or top-level window.
	// The pInsertAfter can one of:  &wndTop, &wndTopMost, &wndBottom, or &wndNoTopMost
	{
		assert(IsWindow());
	//	HWND hWndInsertAfter = pInsertAfter? pInsertAfter->GetHwnd() : (HWND)0;
		return ::SetWindowPos(m_hWnd, hWndInsertAfter, x, y, cx, cy, uFlags);
	}

	inline BOOL CWnd::SetWindowPos(HWND hWndInsertAfter, const RECT& rc, UINT uFlags) const
	// The SetWindowPos function changes the size, position, and Z order of a child, pop-up,
	// or top-level window.
	// The SetWindowPos function changes the size, position, and Z order of a child, pop-up,
	// or top-level window.
	// The pInsertAfter can one of:  &wndTop, &wndTopMost, &wndBottom, or &wndNoTopMost
	{
		assert(IsWindow());
		return ::SetWindowPos(m_hWnd, hWndInsertAfter, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, uFlags);
	}

	inline int CWnd::SetWindowRgn(CRgn& Rgn, BOOL bRedraw /*= TRUE*/) const
	// The SetWindowRgn function sets the window region of the window.
	// The window region determines the area within the window where the system permits drawing.
	// The window now owns the region so it is detached from Rgn.  
	{
		assert(IsWindow());
		HRGN hRgn = (HRGN)Rgn.GetHandle();
		int iResult = ::SetWindowRgn(m_hWnd, hRgn, bRedraw);
		if (iResult && hRgn)
			Rgn.Detach();	// The system owns the region now
		
		return iResult;
	}

	inline BOOL CWnd::SetDlgItemInt(int nIDDlgItem, UINT uValue, BOOL bSigned) const
	// The SetDlgItemInt function sets the text of a control in a dialog box to the string representation of a specified integer value.
	{
		assert(IsWindow());
		return ::SetDlgItemInt(m_hWnd, nIDDlgItem, uValue, bSigned);
	}

	inline BOOL CWnd::SetDlgItemText(int nIDDlgItem, LPCTSTR lpString) const
	// The SetDlgItemText function sets the title or text of a control in a dialog box.
	{
		assert(IsWindow());
		return ::SetDlgItemText(m_hWnd, nIDDlgItem, lpString);
	}

	inline UINT_PTR CWnd::SetTimer(UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc) const
	// Creates a timer with the specified time-out value.
	{
		assert(IsWindow());
		return ::SetTimer(m_hWnd, nIDEvent, uElapse, lpTimerFunc);
	}

	inline BOOL CWnd::SetWindowText(LPCTSTR lpString) const
	// The SetWindowText function changes the text of the window's title bar (if it has one).
	{
		assert(IsWindow());
		return ::SetWindowText(m_hWnd, lpString);
	}

	inline HRESULT CWnd::SetWindowTheme(LPCWSTR pszSubAppName, LPCWSTR pszSubIdList) const
	// Set the XP Theme for a window.
	// Examples:
	//  SetWindowTheme(NULL, NULL);		// Reverts the window's XP theme back to default
	//  SetWindowTheme(L" ", L" ");		// Disables XP theme for the window
	{
		HRESULT hr = E_NOTIMPL;

#ifndef	_WIN32_WCE

		HMODULE hMod = ::LoadLibrary(_T("uxtheme.dll"));
		if(hMod != 0)
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
		assert(IsWindow());
		return ::ShowWindow(m_hWnd, nCmdShow);
	}

	inline BOOL CWnd::UpdateWindow() const
	// The UpdateWindow function updates the client area of the window by sending a
	// WM_PAINT message to the window if the window's update region is not empty.
	// If the update region is empty, no message is sent.
	{
		assert(IsWindow());
		return ::UpdateWindow(m_hWnd);
	}

	inline BOOL CWnd::ValidateRect(LPCRECT prc) const
	// The ValidateRect function validates the client area within a rectangle by
	// removing the rectangle from the update region of the window.
	{
		assert(IsWindow());
		return ::ValidateRect(m_hWnd, prc);
	}

	inline BOOL CWnd::ValidateRgn(HRGN hRgn) const
	// The ValidateRgn function validates the client area within a region by
	// removing the region from the current update region of the window.
	{
		assert(IsWindow());
		return ::ValidateRgn(m_hWnd, hRgn);
	}

	inline CWnd CWnd::WindowFromPoint(POINT pt)
	// Retrieves the window that contains the specified point (in screen coordinates).
	{
		return CWnd(::WindowFromPoint(pt));
	}

	//
	// These functions aren't supported on WinCE
	//
  #ifndef _WIN32_WCE
	inline BOOL CWnd::CloseWindow() const
	// The CloseWindow function minimizes (but does not destroy) the window.
	// To destroy a window, an application can use the Destroy function.
	{
		assert(IsWindow());
		return ::CloseWindow(m_hWnd);
	}

	inline int CWnd::DlgDirList(LPTSTR lpPathSpec, int nIDListBox, int nIDStaticPath, UINT uFileType) const
	// The DlgDirList function replaces the contents of a list box with the names of the subdirectories and files
	// in a specified directory. You can filter the list of names by specifying a set of file attributes.
	{
		assert(IsWindow());
		return ::DlgDirList(m_hWnd, lpPathSpec, nIDListBox, nIDStaticPath, uFileType);
	}

	inline int CWnd::DlgDirListComboBox(LPTSTR lpPathSpec, int nIDComboBox, int nIDStaticPath, UINT uFiletype) const
	// The DlgDirListComboBox function replaces the contents of a combo box with the names of the subdirectories
	// and files in a specified directory. You can filter the list of names by specifying a set of file attributes.
	{
		assert(IsWindow());
		return ::DlgDirListComboBox(m_hWnd, lpPathSpec, nIDComboBox, nIDStaticPath, uFiletype);
	}

	inline BOOL CWnd::DlgDirSelectEx(LPTSTR lpString, int nCount, int nIDListBox) const
	// The DlgDirSelectEx function retrieves the current selection from a single-selection list box. It assumes that the list box
	// has been filled by the DlgDirList function and that the selection is a drive letter, filename, or directory name.
	{
		assert(IsWindow());
		return ::DlgDirSelectEx(m_hWnd, lpString, nCount, nIDListBox);
	}

	inline BOOL CWnd::DlgDirSelectComboBoxEx(LPTSTR lpString, int nCount, int nIDComboBox) const
	// The DlgDirSelectComboBoxEx function retrieves the current selection from a combo box filled by using the
	// DlgDirListComboBox function. The selection is interpreted as a drive letter, a file, or a directory name.
	{
		assert(IsWindow());
		return ::DlgDirSelectComboBoxEx(m_hWnd, lpString, nCount, nIDComboBox);
	}

    #ifndef WIN32_LEAN_AND_MEAN
    inline void CWnd::DragAcceptFiles(BOOL fAccept) const
	// Registers whether a window accepts dropped files.
	{
		assert(IsWindow());
		::DragAcceptFiles(m_hWnd, fAccept);
	}
    #endif

	inline BOOL CWnd::DrawAnimatedRects(int idAni, RECT& rcFrom, RECT& rcTo) const
	// The DrawAnimatedRects function draws a wire-frame rectangle and animates it to indicate the opening of
	// an icon or the minimizing or maximizing of a window.
	{
		assert(IsWindow());
		return ::DrawAnimatedRects(m_hWnd, idAni, &rcFrom, &rcTo);
	}

	inline BOOL CWnd::DrawCaption(HDC hDC, RECT& rc, UINT uFlags) const
	// The DrawCaption function draws a window caption.
	{
		assert(IsWindow());
		return ::DrawCaption(m_hWnd, hDC, &rc, uFlags);
	}

	inline BOOL CWnd::EnableScrollBar(UINT uSBflags, UINT uArrows) const
	// The EnableScrollBar function enables or disables one or both scroll bar arrows.
	{
		assert(IsWindow());
		return ::EnableScrollBar(m_hWnd, uSBflags, uArrows);
	}

	inline CWnd CWnd::GetLastActivePopup() const
	// The GetLastActivePopup function determines which pop-up window owned by the specified window was most recently active.
	{
		assert(IsWindow());
		return CWnd( ::GetLastActivePopup(m_hWnd) );
	}

	inline CMenu CWnd::GetMenu() const
	// The GetMenu function retrieves a handle to the menu assigned to the window.
	{
		assert(IsWindow());
		return CMenu(::GetMenu(m_hWnd));
	}

	inline int CWnd::GetScrollPos(int nBar) const
	// The GetScrollPos function retrieves the current position of the scroll box
	// (thumb) in the specified scroll bar.
	{
		assert(IsWindow());
		return ::GetScrollPos(m_hWnd, nBar);
	}

	inline BOOL CWnd::GetScrollRange(int nBar, int& MinPos, int& MaxPos) const
	// The GetScrollRange function retrieves the current minimum and maximum scroll box
	// (thumb) positions for the specified scroll bar.
	{
		assert(IsWindow());
		return ::GetScrollRange(m_hWnd, nBar, &MinPos, &MaxPos );
	}

	inline CMenu CWnd::GetSystemMenu(BOOL bRevert) const
	// The GetSystemMenu function allows the application to access the window menu (also known as the system menu
	// or the control menu) for copying and modifying.
	{
		assert(IsWindow());
		return CMenu( ::GetSystemMenu(m_hWnd, bRevert) );
	}

	inline CWnd CWnd::GetTopWindow() const
	// The GetTopWindow function examines the Z order of the child windows associated with the parent window and
	// retrieves a handle to the child window at the top of the Z order.
	{
		assert(IsWindow());
		return CWnd( ::GetTopWindow(m_hWnd) );
	}

	inline BOOL CWnd::GetWindowPlacement(WINDOWPLACEMENT& wndpl) const
	// The GetWindowPlacement function retrieves the show state and the restored,
	// minimized, and maximized positions of the window.
	{
		assert(IsWindow());
		return ::GetWindowPlacement(m_hWnd, &wndpl);
	}

	inline BOOL CWnd::HiliteMenuItem(HMENU hMenu, UINT uItemHilite, UINT uHilite) const
	// The HiliteMenuItem function highlights or removes the highlighting from an item in a menu bar.
	{
		assert(IsWindow());
		return ::HiliteMenuItem(m_hWnd, hMenu, uItemHilite, uHilite);
	}

	inline BOOL CWnd::IsIconic() const
	// The IsIconic function determines whether the window is minimized (iconic).
	{
		assert(IsWindow());
		return ::IsIconic(m_hWnd);
	}

	inline BOOL CWnd::IsZoomed() const
	// The IsZoomed function determines whether the window is maximized.
	{
		assert(IsWindow());
		return ::IsZoomed(m_hWnd);
	}

	inline BOOL CWnd::KillTimer(UINT_PTR uIDEvent) const
	// Destroys the specified timer.
	{
		assert(IsWindow());
		return ::KillTimer(m_hWnd, uIDEvent);
	}

	inline BOOL CWnd::LockWindowUpdate() const
	// Disables drawing in the window. Only one window can be locked at a time.
	// Use UnLockWindowUpdate to re-enable drawing in the window
	{
		assert(IsWindow());
		return ::LockWindowUpdate(m_hWnd);
	}

	inline BOOL CWnd::OpenIcon() const
	// The OpenIcon function restores a minimized (iconic) window to its previous size and position.
	{
		assert(IsWindow());
		return ::OpenIcon(m_hWnd);
	}

	inline void CWnd::Print(HDC hDC, DWORD dwFlags) const
	// Requests that the window draw itself in the specified device context, most commonly in a printer device context.
	{
		assert(IsWindow());
		SendMessage(m_hWnd, WM_PRINT, (WPARAM)hDC, (LPARAM)dwFlags);
	}

	inline BOOL CWnd::ScrollWindow(int XAmount, int YAmount, LPCRECT lprcScroll, LPCRECT lprcClip) const
	// The ScrollWindow function scrolls the contents of the specified window's client area.
	{
		assert(IsWindow());
		return ::ScrollWindow(m_hWnd, XAmount, YAmount, lprcScroll, lprcClip);
	}

	inline int CWnd::ScrollWindowEx(int dx, int dy, LPCRECT lprcScroll, LPCRECT lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate, UINT flags) const
	// The ScrollWindow function scrolls the contents of the window's client area.
	{
		assert(IsWindow());
		return ::ScrollWindowEx(m_hWnd, dx, dy, lprcScroll, lprcClip, hrgnUpdate, lprcUpdate, flags);
	}

	inline BOOL CWnd::SetMenu(HMENU hMenu) const
	// The SetMenu function assigns a menu to the specified window.
	// A hMenu of NULL removes the menu.
	{
		assert(IsWindow());
		return ::SetMenu(m_hWnd, hMenu);
	}

	inline int CWnd::SetScrollInfo(int fnBar, const SCROLLINFO& si, BOOL fRedraw) const
	// The SetScrollInfo function sets the parameters of a scroll bar, including
	// the minimum and maximum scrolling positions, the page size, and the
	// position of the scroll box (thumb).
	{
		assert(IsWindow());
		return ::SetScrollInfo(m_hWnd, fnBar, &si, fRedraw);
	}

	inline int CWnd::SetScrollPos(int nBar, int nPos, BOOL bRedraw) const
	// The SetScrollPos function sets the position of the scroll box (thumb) in
	// the specified scroll bar.
	{
		assert(IsWindow());
		return ::SetScrollPos(m_hWnd, nBar, nPos, bRedraw);
	}

	inline BOOL CWnd::SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw) const
	// The SetScrollRange function sets the minimum and maximum scroll box positions for the scroll bar.
	{
		assert(IsWindow());
		return ::SetScrollRange(m_hWnd, nBar, nMinPos, nMaxPos, bRedraw);
	}

	inline BOOL CWnd::SetWindowPlacement(const WINDOWPLACEMENT& wndpl) const
	// The SetWindowPlacement function sets the show state and the restored, minimized,
	// and maximized positions of the window.
	{
		assert(IsWindow());
		return ::SetWindowPlacement(m_hWnd, &wndpl);
	}

	inline BOOL CWnd::ShowOwnedPopups(BOOL fShow) const
	// The ShowOwnedPopups function shows or hides all pop-up windows owned by the specified window.
	{
		assert(IsWindow());
		return ::ShowOwnedPopups(m_hWnd, fShow);
	}

	inline BOOL CWnd::ShowScrollBar(int nBar, BOOL bShow) const
	// The ShowScrollBar function shows or hides the specified scroll bar.
	{
		assert(IsWindow());
		return ::ShowScrollBar(m_hWnd, nBar, bShow);
	}

	inline BOOL CWnd::ShowWindowAsync(int nCmdShow) const
	// The ShowWindowAsync function sets the show state of a window created by a different thread.
	{
		assert(IsWindow());
		return ::ShowWindowAsync(m_hWnd, nCmdShow);
	}

	inline BOOL CWnd::UnLockWindowUpdate() const
	// Enables drawing in the window. Only one window can be locked at a time.
	// Use LockWindowUpdate to disable drawing in the window
	{
		assert(IsWindow());
		return ::LockWindowUpdate(0);
	}

	inline CWnd CWnd::WindowFromDC(HDC hDC) const
	// The WindowFromDC function returns a handle to the window associated with the specified display device context (DC).
	{
		return CWnd( ::WindowFromDC(hDC) );
	}


  #endif

} // namespace Win32xx


#endif // _WIN32XX_WINCORE_H_

