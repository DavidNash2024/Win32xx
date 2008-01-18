// Win32++  Version 5.7
// Released: 15th February, 2008 by:
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
//  CWinApp, CWnd, CWinClass, and CWinException



#ifndef WINCORE_H
#define WINCORE_H

#if defined (_MSC_VER) && _MSC_VER <= 1200
  // Correct for bug in Visual Studio 6
  #pragma warning (disable : 4786) // identifier was truncated
  #define DWORD_PTR DWORD
#endif // (_MSC_VER) && _MSC_VER <= 1200

#ifdef _MSC_VER
  #pragma warning (disable : 4511) // copy operator could not be generated
  #pragma warning (disable : 4512) // assignment operator could not be generated
  #pragma warning (disable : 4702) // unreachable code
  #define _CRT_SECURE_NO_WARNINGS  // Eliminate deprecation warnings for VS2005
#endif // _MSC_VER

// Work around for a bug in VS 2008 (fails on XP and Win2000 without this fix)
#ifndef _WIN32_WINNT
  #define _WIN32_WINNT 0x0500
#endif

#define STRICT

#include <vector>
#include <string>
#include <map>
#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <shlwapi.h>
#ifndef _WIN32_WCE
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

// Define global static TRACE macro for Debug mode only
#ifdef _DEBUG
  #define TRACE(str) (Win32xx::GetApp()->Trace(str))
#else
  #define TRACE(str) // no-op
#endif  // _DEBUG


namespace Win32xx
{
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

		// Its unlikely you would need to override these functions
		virtual BOOL Attach(HWND hWnd);
		virtual BOOL AttachDlgItem(UINT nID, CWnd* pParent);
		virtual void CenterWindow();
		virtual void DestroyWindow();
		virtual HWND Detach();
		virtual HWND GetAncestor(HWND hWnd) const;
		virtual CWnd* GetCWndObject(HWND hWnd) const;
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
		TLSData* m_pTLSData;	// a structure for the Thread Local Storage data
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
		friend class CPropertyPage; // CPropertyPage uses m_MapLock
		friend class CPropertySheet; // CPropertSheet uses m_MapLock and SetTlsIndex

	public:
		CWinApp(HINSTANCE hInstance);
		virtual ~CWinApp();

		// These are the functions you might wish to override
		virtual BOOL InitInstance();
		virtual int  MessageLoop();
		virtual int  Run();

		// Its unlikely you would need to override these functions
		virtual void CreateTrace();
		virtual void SetAccelerators(UINT ID_ACCEL, HWND hWndAccel);
		virtual void Trace(LPCTSTR szString);

		CFrame* GetFrame() {return m_pFrame;}
		DWORD GetTlsIndex() {return st_dwTlsIndex;}
		static CWinApp* GetApp() {return st_pTheApp;}
		HINSTANCE GetInstanceHandle() const {return m_hInstance;}
		HINSTANCE GetResourceHandle() const {return (m_hResource ? m_hResource : m_hInstance);}
		std::map <HWND, CWnd*, CompareHWND>& GetHWNDMap() {return m_HWNDmap;}
		void SetFrame(CFrame* pFrame){m_pFrame = pFrame;}
		void SetResourceHandle(HINSTANCE hResource) {m_hResource = hResource;}

	protected:
		HACCEL m_hAccelTable;		// handle to the accelerator table
		HWND m_hWndAccel;			// handle to the window for accelerator keys

	private:
		TLSData* SetTlsIndex();

		enum Constants
		{
			TRACE_HEIGHT = 200,
			TRACE_WIDTH  = 400
		};

		CCriticalSection m_MapLock;	// thread synchronisation for m_HWNDmap
		HFONT m_hFont;				// handle to the font in the Trace window
		HINSTANCE m_hInstance;		// handle to the applications instance
		HINSTANCE m_hResource;		// handle to the applications resources
		HMODULE m_hRichEdit;		// handle to the module for the RichEdit dll
		HWND m_hTraceEdit;			// handle to the Trace edit window
		BOOL m_IsTlsAllocatedHere;	// a flag set for the Thread Local Storage
		CFrame* m_pFrame;			// pointer to the CFrame object
		CWnd m_Trace;				// CWnd object for the Trace window
		std::map<HWND, CWnd*, CompareHWND> m_HWNDmap;	// maps window handles to CWnd objects
		std::vector<TLSData*> m_ThreadData;	// vector of TLSData pointers, one for each thread
		static DWORD    st_dwTlsIndex;	// Thread Local Storage index
		static CWinApp* st_pTheApp;		// a pointer to this CWinApp object
	};


	////////////////////////////////////////
	// Declaration of the CWinException class
	//
	class CWinException
	{
	public:
		CWinException (LPCTSTR msg) : m_err (::GetLastError()), m_msg(msg) {}
		void MessageBox() const;

	private:
		LPCTSTR GetMessage() const { return m_msg; }

		DWORD  m_err;
		LPCTSTR m_msg;
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
		::MessageBox (0, ErrorMsg, _T("Exception"), MB_ICONEXCLAMATION | MB_OK);
	#else
		UNREFERENCED_PARAMETER(ErrorMsg); // no-op
	#endif  //_DEBUG
	}


  ///////////////////////////////////////////
  // Global functions to extend the WinCE API
  //

  #ifdef _WIN32_WCE		// for WinCE operating systems

  inline UINT wce_GetMenuState(HMENU hMenu, UINT uId, UINT uFlags)
  {
      MENUITEMINFO mii;

      memset(&mii, 0, sizeof(MENUITEMINFO));
      mii.cbSize = sizeof(MENUITEMINFO);
      mii.fMask = MIIM_STATE;

      if (uFlags & MF_BYPOSITION) {
          GetMenuItemInfo(hMenu, uId, TRUE, &mii);
      }
      else
      {
          GetMenuItemInfo(hMenu, uId, FALSE, &mii);
      }

      return mii.fState;
  }

  inline int wce_GetMenuItemCount(HMENU hMenu)
  {
      const int MAX_NUM_ITEMS = 256;
      int  iPos, iCount;

      MENUITEMINFO mii;
      memset(&mii, 0, sizeof(MENUITEMINFO));
      mii.cbSize = sizeof(MENUITEMINFO);

      iCount = 0;
      for (iPos = 0; iPos < MAX_NUM_ITEMS; iPos++)
      {
          if (FALSE == GetMenuItemInfo(hMenu, (UINT)iPos, TRUE, &mii))
          {
              break;
          }
          iCount++;
      }

      return iCount;
  }

  inline UINT wce_GetMenuItemID(HMENU hMenu, int nPos)
  {
      MENUITEMINFO mii;
      memset(&mii, 0, sizeof(mii));
      mii.cbSize = sizeof(mii);
      mii.fMask = MIIM_ID;
      GetMenuItemInfo(hMenu, nPos, TRUE, &mii);

      return mii.wID;
  }

  inline BOOL wce_ModifyMenu(HMENU hMenu, UINT uPosition, UINT uFlags, UINT uIDNewItem, LPCTSTR lpNewItem)
  {
      // Handle MF_BYCOMMAND case
      if (MF_BYPOSITION != (uFlags & MF_BYPOSITION))
      {
          int nMax = wce_GetMenuItemCount(hMenu);
          int nCount = 0;
          while (uPosition != wce_GetMenuItemID(hMenu, nCount) && (nCount < nMax))
          {
              nCount++;
          }
          uPosition = nCount;
          uFlags |= MF_BYPOSITION;
      }

      if (FALSE == DeleteMenu(hMenu, uPosition, uFlags))
      {
          return FALSE;
      }

      return InsertMenu(hMenu, uPosition, uFlags, uIDNewItem, lpNewItem);
	}

#else // for Win32 operating systems

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

  #endif // _WIN32_WCE

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

  #ifndef TLS_OUT_OF_INDEXES
	#define TLS_OUT_OF_INDEXES ((DWORD)0xFFFFFFFF)
  #endif

  #ifndef WM_PARENTNOTIFY
    #define WM_PARENTNOTIFY 0x0210
  #endif

	}; // namespace Win32xx


#endif // WINCORE_H

