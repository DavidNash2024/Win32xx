// Win32++  Version 6.1
// Released: 7th June, 2008 by:
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


///////////////////////////////////////////////////////
// WinCore.h
//  Declaration of the following classes:
//  CWinApp, CWnd, CWinClass, CWinException, and CCriticalSection
//
// This file contains the declarations for the core set of classes required to
// create simple windows using Win32++.  Five classes are declared here:
//
// 1) WinApp: This class is used to define the message loop. Inherit from
//            this class and use it to start the Win32++ application.
//
// 2) CWnd:   This class is used to represent a window. It provides a means
//            of creating the window, and handling its messages. Inherit
//            from this class to define the window to be created.
//
// 3) CWinClass: This class is internally by Win32++ to register the various
//            "window classes" (not to be confused with a C++ class). You can
//            also use it to register your own "window class".
//
// 4) CWinException: This class is used internally by Win32++ to handle
//            exceptions. You can also use it to throw and catch exceptions.
//
// 5) CCriticalSection: This class is used internally to manage thread access
//            to shared resources. You can also use this class to lock and
//            release your own critical sections.


#ifndef WINCORE_H
#define WINCORE_H


// Remove pointless warning messages
#ifdef _MSC_VER
  #pragma warning (disable : 4511) // copy operator could not be generated
  #pragma warning (disable : 4512) // assignment operator could not be generated
  #pragma warning (disable : 4702) // unreachable code
  #pragma warning (disable : 4786) // identifier was truncated
  #pragma warning (disable : 4996) // function or variable may be unsafe (deprecated)
  #ifndef _CRT_SECURE_NO_WARNINGS
    #define _CRT_SECURE_NO_WARNINGS  // eliminate deprecation warnings for VS2005
  #endif
#endif // _MSC_VER
#ifdef __BORLANDC__
  #pragma option -w-8027		   // function not expanded inline
#endif

// Required for VS 2008 (fails on XP and Win2000 without this fix)
#ifndef _WIN32_WINNT
  #define _WIN32_WINNT 0x0500
#endif

#ifdef _WIN32_WCE
#include "WCEstddef.h"
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
#ifndef _WIN32_WCE	// Windows CE doesn't support streams
  #include <sstream>
#endif

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


// Automatically include the Win32xx namespace
// define NO_USING_NAMESPACE to skip this step
namespace Win32xx {}
#ifndef NO_USING_NAMESPACE
  using namespace Win32xx;
#endif

// Define min and max for Dev-C++ compatibility
#ifndef max
  #define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
  #define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

namespace Win32xx
{
	///////////////////////////////////////////
	// Some useful type declarations and macros
	//
	typedef std::basic_string<TCHAR> tString;
	#ifndef _WIN32_WCE
		typedef std::basic_stringstream<TCHAR> tStringStream;
	#endif

	// TRACE sends a string to the debug/output pane, or an external debugger
	//  we use an inline instead of a macro for TRACE to avoid C4390 warnings
	inline void TRACE(LPCTSTR str)
	{
	#ifdef _DEBUG
		OutputDebugString(str);
	#else
		UNREFERENCED_PARAMETER(str); // no-op
	#endif
	}


	////////////////////////////////////////////////
	// Forward declarations.
	//  These classes are defined later or elsewhere
	class CWinApp;
	class CFrame;
	class CWnd;


	enum Constants
	{
		MAX_MENU_STRING = 80,
		MAX_STRING_SIZE = 255,
	};

	// The comparison function object used by CWinApp::m_HWNDmap
	struct CompareHWND
	{
		bool operator()(HWND const a, const HWND b) const
			{return ((DWORD_PTR)a < (DWORD_PTR)b);}
	};

	// Used for Thread Local Storage (TLS)
	struct TLSData
	{
		CWnd* pCWnd;		// pointer to CWnd object for Window creation
		HHOOK  hCBTHook;	// CBT hook for Window creation
		CWnd* pMenubar;		// pointer to CMenubar object
		HHOOK  hMenuHook;	// MSG hook for CMenubar
	};


	/////////////////////////////////////////
	// Declarations for the CCriticalSection class
	//

	// This class is used for thread synchronisation
	class CCriticalSection
	{
		public:
		CCriticalSection()	{::InitializeCriticalSection(&m_cs);}
		~CCriticalSection()	{::DeleteCriticalSection(&m_cs);}

		void Lock()		{::EnterCriticalSection(&m_cs);}
		void Release()	{::LeaveCriticalSection(&m_cs);}

		private:
		CRITICAL_SECTION m_cs;
	};

	////////////////////////////////
	// Declaration of the CWnd class
	//
	class CWnd
	{

	public:
		CWnd();				// Constructor
		virtual ~CWnd();	// Destructor

		// These are the functions you might wish to override
		virtual HWND CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hParent, HMENU hMenu, LPVOID lpParam = NULL);
		virtual HWND CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, RECT rSize, HWND hParent, HMENU hMenu, LPVOID lpParam = NULL);
		virtual HWND Create(HWND hWndParent = NULL);
		virtual LRESULT DefWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void PreRegisterClass(WNDCLASS& wc);

		// These functions aren't intended to be overridden
		virtual BOOL Attach(HWND hWnd);
		virtual BOOL AttachDlgItem(UINT nID, CWnd* pParent);
		virtual void CenterWindow();
		virtual void DestroyWindow();
		virtual HWND Detach();
		virtual CWnd* FromHandle(HWND hWnd) const;
		virtual HWND GetAncestor(HWND hWnd) const;
		virtual tString GetDlgItemString(int nIDDlgItem);
		virtual tString GetWindowString();
		virtual HBITMAP LoadBitmap(LPCTSTR lpBitmapName);
		virtual LPCTSTR LoadString(UINT nID);
		virtual LRESULT OnMessage(HWND hwndParent, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual BOOL RegisterClass(WNDCLASS& wc);
		virtual void SetParent(HWND hParent);

		// These functions aren't virtual, so there's no point overriding them
		HWND GetHwnd() const {return m_hWnd;}
		static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		operator HWND() const {return m_hWnd;}

	protected:
		// These are the functions you might wish to override
		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
		virtual BOOL OnCommandFrame(WPARAM /*wParam*/, LPARAM /*lParam*/) {return 0L;}
		virtual void OnCreate();
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual	LRESULT OnNotifyFrame(WPARAM /*wParam*/, LPARAM /*lParam*/) {return 0L;}
		virtual void OnInitialUpdate();
		virtual void OnPaint(HDC hDC);
		virtual HICON SetIconLarge(int nIcon);
		virtual HICON SetIconSmall(int nIcon);
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// Its unlikely you would need to override these functions
		virtual LRESULT CallPrevWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual BOOL IsMDIChild() const {return FALSE;}
		void AddToMap();

		CREATESTRUCT m_cs;		// defines initialisation parameters for PreCreate and Create
		HWND m_hWnd;			// handle to this object's window
		HWND m_hWndParent;		// handle to this object's window parent
		WNDCLASS m_wc;			// defines initialisation parameters for RegisterClass
		CWinApp* pWinApp;

	private:
		CWnd(const CWnd&);				// Disable copy construction
		CWnd& operator = (const CWnd&); // Disable assignment operator
		BOOL RemoveFromMap();
		void Subclass();

		HICON m_hIconLarge;			// handle to the window's large icon
		HICON m_hIconSmall;			// handle to the window's small icon
		WNDPROC m_PrevWindowProc;	// pre-subclassed Window Procedure
		tString m_LoadString;		// a TCHAR std::string, temporary storage for strings

	}; // class CWnd


	///////////////////////////////
	// Declaration of the CWinApp class
	//
	class CWinApp
	{
		friend class CWnd;	// CWnd needs access to CWinApp's private members

	public:
		CWinApp();
		virtual ~CWinApp();

		// These are the functions you might wish to override
		virtual BOOL InitInstance();
		virtual int  MessageLoop();
		virtual int  Run();
		virtual void SetAccelerators(UINT ID_ACCEL, HWND hWndAccel);
		virtual void SetFrame(CFrame* pFrame);

		// These functions aren't intended to be overridden
		CFrame* GetFrame() {return m_pFrame;}
		DWORD GetTlsIndex() {return m_TlsIndex;}
		CWnd* GetCWndFromMap(HWND hWnd);
		HINSTANCE GetInstanceHandle() const {return m_hInstance;}
		HINSTANCE GetResourceHandle() const {return (m_hResource ? m_hResource : m_hInstance);}
		void SetResourceHandle(HINSTANCE hResource) {m_hResource = hResource;}
		static CWinApp* SetnGetThis(CWinApp* pThis = 0);
	/*	{
			static CWinApp* pWinApp = 0;
			if (pWinApp == 0)
			{
				pWinApp = pThis;
			}

			return pWinApp;
		} */
		TLSData* SetTlsIndex();

	protected:
		HACCEL m_hAccelTable;		// handle to the accelerator table
		HWND m_hWndAccel;			// handle to the window for accelerator keys

	private:
		enum Constants
		{
			TRACE_HEIGHT = 200,
			TRACE_WIDTH  = 400
		};

		void DefaultClass();

		CCriticalSection m_MapLock;	// thread synchronisation for m_HWNDmap
		HINSTANCE m_hInstance;		// handle to the applications instance
		HINSTANCE m_hResource;		// handle to the applications resources
		CFrame* m_pFrame;			// pointer to the CFrame object
		std::map<HWND, CWnd*, CompareHWND> m_HWNDmap;	// maps window handles to CWnd objects
		std::vector<TLSData*> m_ThreadData;	// vector of TLSData pointers, one for each thread
		DWORD m_TlsIndex;			// Thread Local Storage index
		WNDPROC m_Callback;			// callback address of CWnd::StaticWndowProc
	};


	//////////////////////////////////////////////////
	// Global functions	(within the Win32xx namespace)

	// Returns a pointer to the CWinApp object
	inline CWinApp* GetApp()
	{
		return CWinApp::SetnGetThis();
	}

	// Displays an error message in a message box. Debug mode only.
	inline void DebugWarnMsg(LPCTSTR WarnMsg)
	{
	#ifdef _DEBUG
		::MessageBox (0, WarnMsg, _T("Warning"), MB_ICONINFORMATION | MB_OK);
	#else
		UNREFERENCED_PARAMETER(WarnMsg); // no-op
	#endif  //_DEBUG
	}

	// Displays a warning message in a messagebox. Debug mode only
	inline void DebugErrMsg(LPCTSTR ErrorMsg)
	{
	#ifdef _DEBUG
		::MessageBox (0, ErrorMsg, _T("Error"), MB_ICONEXCLAMATION | MB_OK);
	#else
		UNREFERENCED_PARAMETER(ErrorMsg); // no-op
	#endif  //_DEBUG
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
		//  2600     Windows Vista

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

  #ifndef lstrcpyn
	// Required for WinCE
	inline LPTSTR lstrcpyn(LPTSTR lpstrDest, LPCTSTR lpstrSrc, int nLength)
	{
		if(NULL == lpstrDest || NULL == lpstrSrc || nLength <= 0)
			return NULL;
		int nLen = min((int)lstrlen(lpstrSrc), nLength - 1);
		LPTSTR lpstrRet = (LPTSTR)memcpy(lpstrDest, lpstrSrc, nLen * sizeof(TCHAR));
		lpstrDest[nLen] = 0;
		return lpstrRet;
	}
  #endif // !lstrcpyn

	inline tString CharToTString(const char* s)
	{
		// Handy for converting char to TCHAR
		tString tstr;
  #ifdef UNICODE
		int len = 1 + strlen(s);

		TCHAR* t = new TCHAR[len];
		if (NULL == t) throw std::bad_alloc();

		mbstowcs(t, s, len);
		tstr = t;
		delete []t;
  #else
		tstr = s;
  #endif
		return tstr;
	}

	inline std::string TcharToString(LPCTSTR t)
	{
		// Handy for converting TCHAR to char
		std::string str;
  #ifdef UNICODE
		int len = 1 + lstrlen(t);

		char* c = new char[len];
		if (NULL == c) throw std::bad_alloc();

		wcstombs(c, t, len);
		str = c;
		delete []c;
  #else
		str = t;
  #endif
		return str;
	}

  #ifndef TLS_OUT_OF_INDEXES
	#define TLS_OUT_OF_INDEXES ((DWORD)0xFFFFFFFF)
  #endif

  #ifndef WM_PARENTNOTIFY
    #define WM_PARENTNOTIFY 0x0210
  #endif

	////////////////////////////////////////
	// Declaration of the CWinException class
	//
	class CWinException
	{
	public:
		CWinException (LPCTSTR msg) : m_err (::GetLastError()), m_msg(msg) {}
		LPCTSTR What() const {return m_msg;}
		void MessageBox() const
		{
			TCHAR buf1 [MAX_STRING_SIZE/2 -10] = _T("");
			TCHAR buf2 [MAX_STRING_SIZE/2 -10] = _T("");
			TCHAR buf3 [MAX_STRING_SIZE]       = _T("");

			lstrcpyn(buf1, m_msg, MAX_STRING_SIZE/2 -10);

			// Display Last Error information if it's useful
			if (m_err != 0)
			{
				::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, m_err,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf2, MAX_STRING_SIZE/2 -10, NULL);

				::wsprintf(buf3, _T("%s\n\n     %s\n\n"), buf1, buf2);
			}
			else
				::wsprintf(buf3, _T("%s"), buf1);

			::MessageBox (0, buf3, _T("Error"), MB_ICONEXCLAMATION | MB_OK);
		}

	private:
		DWORD  m_err;
		LPCTSTR m_msg;

	};

	///////////////////////////////////
	// Definitions for the CWinApp class
	//

	// To begin Win32++, inherit your application class from this one.
	// You should run only one instance of the class inherited from this.
	inline CWinApp::CWinApp() : m_hAccelTable(NULL), m_hWndAccel(NULL), m_pFrame(NULL), m_Callback(NULL)
	{
		try
		{
			// Test if this is the first instance of CWinApp
			if (0 == GetApp() )
			{
				m_TlsIndex = ::TlsAlloc();
				if (m_TlsIndex == TLS_OUT_OF_INDEXES)
				{
					// We only get here in the unlikely event that all TLS indexes are already allocated by this app
					// At least 64 TLS indexes per process are allowed. Win32++ requires only one TLS index.
					throw CWinException(_T("CWinApp::CWinApp  Failed to allocate TLS Index"));
				}
			}
			else
			{
				// We get here if Win32++ is used incorrectly, i.e. more than one instance
				// of a CWinApp derived class is started.
 				throw CWinException(_T("Error!  An instance of CWinApp (or a class derived from CWinApp) is already running"));
			}

			m_hInstance = (HINSTANCE) ::GetModuleHandle(0);
			m_hResource = m_hInstance;
			DefaultClass();
			SetnGetThis(this);
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
		for (m = m_HWNDmap.begin(); m != m_HWNDmap.end(); ++m)
		{
			(*m).second->DestroyWindow();
		}
		m_HWNDmap.clear();

		// Do remaining tidy up
		if (m_TlsIndex != TLS_OUT_OF_INDEXES)
		{
			::TlsSetValue(GetTlsIndex(), NULL);
			::TlsFree(m_TlsIndex);
			m_TlsIndex = TLS_OUT_OF_INDEXES;
		}

		std::vector<TLSData*>::iterator iter;
		for(iter = m_ThreadData.begin(); iter != m_ThreadData.end(); ++iter)
		{
			delete *(iter);
		}

		if (m_hAccelTable)
			::DestroyAcceleratorTable(m_hAccelTable);
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
		m_MapLock.Lock();
		m = m_HWNDmap.find(hWnd);
		m_MapLock.Release();
		if (m != m_HWNDmap.end())
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

		MSG uMsg;
		int status;

		while((status = ::GetMessage(&uMsg, NULL, 0, 0))!= 0)
		{
			if (-1 == status) return -1;

			if (!::TranslateAccelerator(m_hWndAccel, m_hAccelTable, &uMsg))
			{
				::TranslateMessage(&uMsg);
				::DispatchMessage(&uMsg);
			}
		}
		return LOWORD(uMsg.wParam);
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
			throw;	// Rethrow unknown exception
		}

		catch (...)
		{
			::MessageBox(NULL, _T("Unknown Exception"), _T("Error"), MB_OK);
			throw;	// Rethrow unknown exception
		}
	}

	inline void CWinApp::SetAccelerators(UINT ID_ACCEL, HWND hWndAccel)
	// ID_ACCEL is the resource ID of the accelerator table
	// hWndAccel is the window handle for translated messages
	{

		if (m_hAccelTable)
			::DestroyAcceleratorTable(m_hAccelTable);

		m_hWndAccel = hWndAccel;

		m_hAccelTable = ::LoadAccelerators(m_hResource, MAKEINTRESOURCE(ID_ACCEL));
		if (!m_hAccelTable)
			TRACE(_T("Load Accelerators failed\n"));
	}

	inline void CWinApp::SetFrame(CFrame* pFrame)
	{
		// Store the pointer to the (first) frame
		if (0 == m_pFrame)
			m_pFrame = pFrame;
	}

	inline CWinApp* CWinApp::SetnGetThis(CWinApp* pThis /*= 0*/)
	{
		// This function stores the 'this' pointer in a static variable.
		// Once stored, it can be used later to return the 'this' pointer.
		static CWinApp* pWinApp = 0;

		if (pWinApp == 0)
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
				m_ThreadData.push_back(pTLSData);
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
	inline CWnd::CWnd() : m_hWnd(NULL), m_hWndParent(NULL), pWinApp(NULL), m_hIconLarge(NULL),
					m_hIconSmall(NULL), m_PrevWindowProc(NULL)
	{
		// Note: m_hWnd and m_hWndParent are set in CWnd::CreateEx(...)
		::ZeroMemory(&m_cs, sizeof(CREATESTRUCT));
		::ZeroMemory(&m_wc, sizeof(WNDCLASS));
	}

	inline CWnd::~CWnd()
	{
		// Destroy the window for this object
		if (::IsWindow(m_hWnd)) DestroyWindow();

		// Destroy the GDI objects
		if (m_hIconLarge) ::DestroyIcon(m_hIconLarge);
		if (m_hIconSmall) ::DestroyIcon(m_hIconSmall);

		// Remove the map entries
		if (GetApp())
		{
			RemoveFromMap();
		}
	}

	inline void CWnd::AddToMap()
	{
		// Store the window handle and CWnd pointer in the HWND map
		GetApp()->m_MapLock.Lock();
		if (m_hWnd == 0)
			throw CWinException(_T("CWnd::AddToMap  can't add a NULL HWND"));
		if (GetApp()->GetCWndFromMap(m_hWnd))
			throw CWinException(_T("CWnd::AddToMap  HWND already in map"));

		GetApp()->m_HWNDmap.insert(std::make_pair(m_hWnd, this));
		GetApp()->m_MapLock.Release();
	}

	inline BOOL CWnd::Attach(HWND hWnd)
	// Subclasses an existing window and attaches it to a CWnd
	{
		try
		{
			if (IsWindow(hWnd))
			{
				if (0 != GetApp()->GetCWndFromMap(hWnd))
					throw CWinException(_T("Window already attached to this CWnd object"));

				m_hWnd = hWnd;
				m_hWndParent = ::GetParent(hWnd);
				Subclass();

				if (m_PrevWindowProc)
				{
					// Store the CWnd pointer in the HWND map
					AddToMap();
					return TRUE;
				}
				else
				{
					m_hWnd = NULL;
					m_hWndParent = NULL;
					throw CWinException(_T("CWnd::Attach .. Subclass failed"));
				}
			}
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
			throw;
		}

		return FALSE;
	}

	inline BOOL CWnd::AttachDlgItem(UINT nID, CWnd* pParent)
	// Use to convert a dialog item to a CWnd object
	{
		HWND hWnd = ::GetDlgItem(pParent->GetHwnd(), nID);

		return Attach(hWnd);
	}

	inline LRESULT CWnd::CallPrevWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::CallWindowProc(m_PrevWindowProc, hWnd, uMsg, wParam, lParam);
	}

	inline void CWnd::CenterWindow()
	{
		// Centers this window over it's parent

		RECT rc;
		RECT rcParent;
		RECT rcDesktop;

		// Get screen dimensions excluding task bar
		::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcDesktop, 0);
		int iWidth = rcDesktop.right;
		int iHeight = rcDesktop.bottom;

		// Get the dialog dimensions
		::GetWindowRect(m_hWnd, &rc);

		// Get the parent window dimensions (parent could be the desktop)
		if (m_hWndParent != NULL) ::GetWindowRect(m_hWndParent, &rcParent);
		else rcParent = rcDesktop;

		// Calculate point to center the dialog on the parent window
		int x = rcParent.left + ((rcParent.right - rcParent.left) - (rc.right - rc.left))/2;
		int y = rcParent.top + ((rcParent.bottom - rcParent.top) - (rc.bottom - rc.top))/2;

		// Keep the dialog wholly on the desktop
		if (x < 0) x = 0;
		if (y < 0) y = 0;
		if (x > iWidth - (rc.right - rc.left))
			x = iWidth - (rc.right - rc.left);
		if (y > iHeight - (rc.bottom - rc.top))
			y = iHeight - (rc.bottom - rc.top);

		::SetWindowPos(m_hWnd, HWND_TOP, x, y, 0, 0,  SWP_NOSIZE);

	} // POINT CWnd::CenterWindow()

	inline HWND CWnd::Create(HWND hWndParent /* = NULL */)
	// Default Window Creation.
	{
		// Set the CREATESTRUCT parameters
		PreCreate(m_cs);

		// Set the WNDCLASS parameters
		PreRegisterClass(m_wc);
		if (m_wc.lpszClassName)
		{
			RegisterClass(m_wc);
			m_cs.lpszClass = m_wc.lpszClassName;
			m_cs.style |= m_wc.style;
		}

		// Set the Window Class Name
		TCHAR szClassName[MAX_STRING_SIZE + 1] = _T("Win32++ Window");
		if (m_cs.lpszClass)
			lstrcpy(szClassName, m_cs.lpszClass);

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
		CreateEx(m_cs.dwExStyle, szClassName, m_cs.lpszName, dwStyle, x, y,
			cx, cy, hWndParent, m_cs.hMenu, m_cs.lpCreateParams);

		return m_hWnd;
	}

	inline HWND CWnd::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, RECT rSize, HWND hParent, HMENU hMenu, LPVOID lpParam /*= NULL*/)
	{
		int x = rSize.left;
		int y = rSize.top;
		int nWidth = rSize.right - rSize.left;
		int nHeight = rSize.bottom - rSize.top;
		return CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, x, y, nWidth, nHeight, hParent, hMenu, lpParam);
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

			// Ensure a window class is registered
			TCHAR ClassName[MAX_STRING_SIZE] = _T("");
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

			m_hWndParent = hParent;

			// Automatically subclass predefined window class types
			::GetClassInfo(GetApp()->GetInstanceHandle(), lpszClassName, &wc);
			if (wc.lpfnWndProc != GetApp()->m_Callback)
			{
				Subclass();

				// Send a message to force the HWND to be added to the map
				::SendMessage(m_hWnd, WM_NULL, 0, 0);

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

	} // void CWnd::CreateEx()

	inline LRESULT CWnd::DefWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	// Pass messages on to the appropriate default window procedure
	// CMDIChild and CMDIFrame override this function
	{
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	inline void CWnd::DestroyWindow()
	{
		if (m_hIconLarge != NULL)
			::DestroyIcon(m_hIconLarge);
		m_hIconLarge = NULL;

		if (m_hIconSmall != NULL)
			::DestroyIcon(m_hIconSmall);
		m_hIconSmall = NULL;

		if (::IsWindow(m_hWnd))
			::DestroyWindow(m_hWnd);

		m_hWnd = NULL;
		m_hWndParent = NULL;
		m_PrevWindowProc = NULL;
	}

	inline HWND CWnd::Detach()
	{
		//Only a subclassed window can be detached
		if (0 == m_PrevWindowProc)
			throw CWinException(_T("CWnd::Detach  Unable to detach this window"));

		::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)m_PrevWindowProc);

		// Remove the map entry
		if (!RemoveFromMap())
			throw CWinException(_T("CWnd::Detach  Unable to find window to detach"));

		// Clear member variables
		HWND hWnd = m_hWnd;
		m_hWnd = NULL;
		m_hWndParent = NULL;
		m_PrevWindowProc = NULL;
		return hWnd;
	}

	inline CWnd* CWnd::FromHandle(HWND hWnd) const
	{
		// Returns the CWnd object associated with the window handle
		return GetApp()->GetCWndFromMap(hWnd);
	}

	inline HWND CWnd::GetAncestor(HWND hWnd) const
	{
		// Returns the root parent.  Supports Win95
		HWND hWndParent = ::GetParent(hWnd);
		while (::IsChild(hWndParent, hWnd))
		{
			hWnd = hWndParent;
			hWndParent = ::GetParent(hWnd);
		}

		return hWnd;
	}

	inline tString CWnd::GetDlgItemString(int nIDDlgItem)
	{
		int nLength = ::GetWindowTextLength(GetDlgItem(m_hWnd, nIDDlgItem));

		tString String;
		if (nLength > 0)
		{
			TCHAR szString[MAX_STRING_SIZE];
			::GetDlgItemText(m_hWnd, nIDDlgItem, szString, MAX_STRING_SIZE);
			String = szString;
		}

		return String;
	}

	inline tString CWnd::GetWindowString()
	{
		// Gets the window title for an ordinary window, or the text in an edit control

		int nLength = ::GetWindowTextLength(m_hWnd);

		tString String;
		if (nLength > 0)
		{
			TCHAR szString[MAX_STRING_SIZE];
			::GetWindowText(m_hWnd, szString, MAX_STRING_SIZE);
			String = szString;
		}
		return String;
	}

	inline HBITMAP CWnd::LoadBitmap(LPCTSTR lpBitmapName)
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

		m_LoadString = _T("");
		TCHAR szString[MAX_STRING_SIZE] = _T("");
		if (!::LoadString (GetApp()->GetResourceHandle(), nID, szString, MAX_STRING_SIZE))
		{
			// The string resource might be in the application's resources instead
			if (::LoadString (GetApp()->GetInstanceHandle(), nID, szString, MAX_STRING_SIZE))
			{
				m_LoadString = szString;
				return (LPCTSTR) m_LoadString.c_str();
			}

			TCHAR msg[80] = _T("");
			::wsprintf(msg, _T("LoadString - No string resource for %d\n"), nID);
			TRACE(msg);
		}

		m_LoadString = szString;

		// Never return a pointer to a local variable, it is out of scope when the function returns.
		// We return a pointer to a member variable so it remains in scope.
		return m_LoadString.c_str();
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

	inline LRESULT CWnd::OnMessage(HWND hWndParent, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		//      Return the value recommended by the Win32 API documentation.
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
		//      Return the value recommended by the Win32 API documentation.
		// }

		// return 0L for unhandled notifications
		return 0L;
	}

	inline void CWnd::OnPaint(HDC)
	{
		// Override this function in your derived class to perform drawing tasks.
	}

	inline void CWnd::PreCreate(CREATESTRUCT& cs)
	// Called by CWnd::Create to set some window parameters
	//  Useful for setting the window to a predefined type (eg TOOLBARCLASSNAME)
	{
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
		// CREATESTRUCT values prior to window creation
	}

	inline void CWnd::PreRegisterClass(WNDCLASS& wc)
	// Called by CWnd::Create to set some window parameters
	//  Useful for setting the background brush and cursor
	{
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
		// WNDCLASS values prior to window creation. Be sure to set
		// the ClassnName for this function to take effect
	}

	inline BOOL CWnd::RegisterClass(WNDCLASS& wc)
	{
		try
		{
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

		// Erase the CWnd pointer entry from the map
		GetApp()->m_MapLock.Lock();
		for (m = GetApp()->m_HWNDmap.begin(); m != GetApp()->m_HWNDmap.end(); ++m)
		{
			if (this == m->second)
			{
				GetApp()->m_HWNDmap.erase(m);
				GetApp()->m_MapLock.Release();
				return TRUE;
			}
		}

		GetApp()->m_MapLock.Release();
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

	inline void CWnd::SetParent(HWND hParent)
	{
		if (::IsWindow(hParent))
		{
			m_hWndParent = hParent;
			if (::IsWindow(m_hWnd))
				::SetParent(m_hWnd, hParent);
		}
		else
			throw CWinException(_T("CWnd::SetParent ... Failed to set parent"));

	}

	inline LRESULT CALLBACK CWnd::StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		try
		{
			CWnd* w = GetApp()->GetCWndFromMap(hWnd);
			if (0 != w)
			{
				// CWnd pointer found, so call the CWnd's WndProc
				return w->WndProc(hWnd, uMsg, wParam, lParam);
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

				return w->WndProc(hWnd, uMsg, wParam, lParam);
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

	inline void CWnd::Subclass()
	// A private function used by CreateEx, Attach and AttachDlgItem
	{
		if (m_PrevWindowProc)
			throw CWinException(_T("Subclass failed.  Already Subclassed"));

		// Subclass the window to pass messages to WndProc
		WNDPROC WndProc = (WNDPROC)::GetWindowLongPtr(m_hWnd, GWLP_WNDPROC);
		if (WndProc == GetApp()->m_Callback)
			throw CWinException(_T("Subclass failed.  Already sending messages to StaticWindowProc"));
		m_PrevWindowProc = (WNDPROC)::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)CWnd::StaticWindowProc);
	}

	inline LRESULT CWnd::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// Override this function in your class derrived from CWnd to handle
		//  window messages. A typical function might look like this:

		//	switch (uMsg)
		//	{
		//	case MESSAGE1:		// Some Win32 API message
		//		OnMessage1();	// A user defined function
		//		break;			// Also do default processing
		//	case MESSAGE2:
		//		OnMessage2();
		//		return x;		// Don't do default processing, but instead return
		//						//  a value recommended by the Win32 API documentation
		//	}

		// Always pass unhandled messages on to WndProcDefault
		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

	inline LRESULT CWnd::WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// All unhandled window messages (excluding dialogs) end up here

		LRESULT lr;

		// Use CallWindowProc for subclassed windows
		if (m_PrevWindowProc)
			return ::CallWindowProc(m_PrevWindowProc, hWnd, uMsg, wParam, lParam);

    	switch (uMsg)
		{
		case WM_COMMAND:
			{
				// Refelect this message if it's from a control
				CWnd* Wnd = FromHandle((HWND)lParam);
				if (Wnd != NULL)
					if (Wnd->OnMessageReflect(uMsg, wParam, lParam))
						return TRUE;

				// Handle user commands
				if (OnCommand(wParam, lParam))
					return TRUE;

				// Handle Win32++ frame commands
				if (OnCommandFrame(wParam, lParam))
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
				// Handle the Win32++ frame notifications
				lr = OnNotifyFrame(wParam, lParam);
				if (lr) return lr;

				// Do Notification reflection if it came from a CWnd object
				HWND hwndFrom = ((LPNMHDR)lParam)->hwndFrom;
				CWnd* WndFrom = FromHandle(hwndFrom);
				if (WndFrom != NULL)
				{
					lr = WndFrom->OnNotifyReflect(wParam, lParam);
					if (lr) return lr;
				}

				if (m_hWnd != ::GetParent(hwndFrom))
				{
					// Some controls (eg ListView) have child windows.
					// Reflect those notifications too.
					CWnd* WndFromParent = FromHandle(GetParent(hwndFrom));
					if (WndFromParent != NULL)
					{
						lr = WndFromParent->OnNotifyReflect(wParam, lParam);
						if (lr) return lr;
					}
				}

				// Handle user notifications
				return  OnNotify(wParam, lParam);
			}

		case WM_PAINT:
			{
				if (::GetUpdateRect(hWnd, NULL, FALSE))
				{
					::PAINTSTRUCT ps;
					HDC hDC = ::BeginPaint(hWnd, &ps);

					OnPaint(hDC);
					::EndPaint(hWnd, &ps);
				}
				else
				// RedrawWindow can require repainting without an update rect
				{
					HDC hDC = ::GetDC(hWnd);

					OnPaint(hDC);
					::ReleaseDC(hWnd, hDC);
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
			lr = OnMessage(hWnd, uMsg, wParam, lParam);
			if (lr) return lr;	// Message processed so return
			break;				// Do default processing when message not already processed

		} // switch (uMsg)

		// Now hand all messages to the default procedure
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	} // LRESULT CWnd::WindowProc(...)


}; // namespace Win32xx


#endif // WINCORE_H

