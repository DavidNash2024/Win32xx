// Win32++  Version 6.01
// Released: 20th March, 2008 by:
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

#ifndef DWORD_PTR
  #define DWORD_PTR DWORD
#endif

#ifdef _MSC_VER
  #pragma warning (disable : 4511) // copy operator could not be generated
  #pragma warning (disable : 4512) // assignment operator could not be generated
  #pragma warning (disable : 4702) // unreachable code
  #define _CRT_SECURE_NO_WARNINGS  // Eliminate deprecation warnings for VS2005
#endif // _MSC_VER

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


// Automatically include the Win32xx namespace
// define NO_USING_NAMESPACE to skip this step
namespace Win32xx {}
#ifndef NO_USING_NAMESPACE
  using namespace Win32xx;
#endif

///////////////////////////////////////////
// Some useful type declarations and macros
//

typedef std::basic_string<TCHAR> tString;
#ifndef _WIN32_WCE
  typedef std::basic_stringstream<TCHAR> tStringStream;
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

		CREATESTRUCT m_cs;		// defines initialisation parameters for PreCreate and Create
		HWND m_hWnd;			// handle to this object's window
		HWND m_hWndParent;		// handle to this object's window parent
		WNDCLASS m_wc;			// defines initialisation parameters for RegisterClass

	private:
		CWnd(const CWnd&);				// Disable copy construction
		CWnd& operator = (const CWnd&); // Disable assignment operator
		void Subclass();

		HICON m_hIconLarge;			// handle to the window's large icon
		HICON m_hIconSmall;			// handle to the window's small icon
		WNDPROC m_PrevWindowProc;	// Pre-Subclassed Window Procedure
		tString m_LoadString;		// a TCHAR std::string, temporary storage for strings

	}; // class CWnd


	///////////////////////////////
	// Declaration of the CWinApp class
	//
	class CWinApp
	{
		friend class CWnd;		// CWnd uses m_MapLock and SetTlsIndex
		friend class CDialog;	// CDialog uses m_MapLock
		friend class CMenubar;	// CMenubar uses GetTlsIndex
		friend class CPropertyPage; // CPropertyPage uses m_MapLock
		friend class CPropertySheet; // CPropertSheet uses m_MapLock and SetTlsIndex

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
		static CWinApp* GetApp() {return st_pTheApp;}
		HINSTANCE GetInstanceHandle() const {return m_hInstance;}
		HINSTANCE GetResourceHandle() const {return (m_hResource ? m_hResource : m_hInstance);}
		void SetResourceHandle(HINSTANCE hResource) {m_hResource = hResource;}

	protected:
		HACCEL m_hAccelTable;		// handle to the accelerator table
		HWND m_hWndAccel;			// handle to the window for accelerator keys

	private:
		enum Constants
		{
			TRACE_HEIGHT = 200,
			TRACE_WIDTH  = 400
		};

		void AddToMap(HWND hWnd, CWnd* w);
		CWnd* GetCWndFromMap(HWND hWnd);
		BOOL RemoveFromMap(CWnd* w);
		DWORD GetTlsIndex() {return st_dwTlsIndex;}
		TLSData* SetTlsIndex();

		CCriticalSection m_MapLock;	// thread synchronisation for m_HWNDmap
		HINSTANCE m_hInstance;		// handle to the applications instance
		HINSTANCE m_hResource;		// handle to the applications resources
		BOOL m_IsTlsAllocatedHere;	// a flag set for the Thread Local Storage
		CFrame* m_pFrame;			// pointer to the CFrame object
		std::map<HWND, CWnd*, CompareHWND> m_HWNDmap;	// maps window handles to CWnd objects
		std::vector<TLSData*> m_ThreadData;	// vector of TLSData pointers, one for each thread
		static DWORD    st_dwTlsIndex;	// Thread Local Storage index
		static CWinApp* st_pTheApp;		// a pointer to this CWinApp object
	};


	//////////////////////////////////////////////////
	// Global functions	(within the Win32xx namespace)

	// Returns a pointer to the CWinApp object
	inline CWinApp* GetApp(){ return CWinApp::GetApp();}

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
		int len = 1 + (int)strlen(s);
		TCHAR* t = new TCHAR[len];
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

}; // namespace Win32xx


#endif // WINCORE_H

