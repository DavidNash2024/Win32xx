// Win32++   Version 8.1 beta
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
// wxx_winutils.h
//  Declaration of the following classes:
//  CPoint, CRect, and CSize

#ifndef _WIN32XX_WINUTILS_H_
#define _WIN32XX_WINUTILS_H_

#include "wxx_rect.h"
#include "wxx_textconv.h"
#include "wxx_themes.h"


// Define our own MIN and MAX macros
// this avoids inconsistencies with Dev-C++ and other compilers, and
// avoids conflicts between typical min/max macros and std::min/std::max
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))


// Define our own VERIFY macro
// In debug mode, VERIFY asserts if the expression evaluates to zero
// In release mode, VERIFY evaluates the expression, but doesn't assert.
#ifndef VERIFY
  #ifndef NDEBUG
    #define VERIFY(f) assert(f)
  #else
    #define VERIFY(f) ((void)(f))
  #endif
#endif

// Ensure UNICODE and _UNICODE definitions are the same
#ifdef _UNICODE
  #ifndef UNICODE
    #define UNICODE
  #endif
#endif

#ifdef UNICODE
  #ifndef _UNICODE
    #define _UNICODE
  #endif
#endif


namespace Win32xx
{

	////////////////////////////////////////
	// Global Functions
	//

	inline CWinApp& GetApp()
	// Returns a reference to the CWinApp derived class
	{
		return *CWinApp::SetnGetThis();
	}

	inline void TRACE(LPCSTR str)
	// TRACE sends a string to the debug/output pane, or an external debugger
	{
  #ifndef NDEBUG
		OutputDebugString(A2T(str));
  #else
		UNREFERENCED_PARAMETER(str); // no-op
  #endif
	}

	inline void TRACE(LPCWSTR str)
	// TRACE sends a string to the debug/output pane, or an external debugger
	{
  #ifndef NDEBUG
		OutputDebugString(W2T(str));
  #else
		UNREFERENCED_PARAMETER(str); // no-op
  #endif
	}


  #ifndef _WIN32_WCE		// for Win32/64 operating systems, not WinCE

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
		HMODULE hComCtl = ::LoadLibrary(_T("COMCTL32.DLL"));
		if (hComCtl == 0)
			return 0;

		int ComCtlVer = 400;

		if (::GetProcAddress(hComCtl, "InitCommonControlsEx"))
		{
			// InitCommonControlsEx is unique to 4.7 and later
			ComCtlVer = 470;

			if (::GetProcAddress(hComCtl, "DllGetVersion"))
			{
				typedef HRESULT CALLBACK DLLGETVERSION(DLLVERSIONINFO*);
				DLLGETVERSION* pfnDLLGetVersion = NULL;

				pfnDLLGetVersion = reinterpret_cast<DLLGETVERSION*>(::GetProcAddress(hComCtl, "DllGetVersion"));
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
			else if (::GetProcAddress(hComCtl, "InitializeFlatSB"))
				ComCtlVer = 471;	// InitializeFlatSB is unique to version 4.71
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
		// For Win95 and NT, cbSize needs to be 44
		if (1400 == (GetWinVersion()) || (2400 == GetWinVersion()))
			return CCSIZEOF_STRUCT(MENUITEMINFO, cch);

		return sizeof(MENUITEMINFO);
	}

	inline UINT GetSizeofNonClientMetrics()
	{
		// This function correctly determines the sizeof NONCLIENTMETRICS

  #if (WINVER >= 0x0600)
		// Is OS version less than Vista, adjust size to correct value
		if (GetWinVersion() < 2600)
			return CCSIZEOF_STRUCT(NONCLIENTMETRICS, lfMessageFont);
  #endif

		return sizeof(NONCLIENTMETRICS);
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
		BOOL IsAeroThemed = FALSE;

		// Test if Windows version is XP or greater
		if (GetWinVersion() >= 2501)
		{
			HMODULE hMod = ::LoadLibrary(_T("uxtheme.dll"));

			if(hMod != 0)
			{
				// Declare pointers to IsCompositionActive function
				FARPROC pIsCompositionActive = ::GetProcAddress(hMod, "IsCompositionActive");

				if(pIsCompositionActive)
				{
					if(pIsCompositionActive())
					{
						IsAeroThemed = TRUE;
					}
				}
				::FreeLibrary(hMod);
			}
		}

		return IsAeroThemed;
	}

	inline BOOL IsXPThemed()
	{
		BOOL IsXPThemed = FALSE;

		// Test if Windows version is XP or greater
		if (GetWinVersion() >= 2501)
		{
			HMODULE hMod = ::LoadLibrary(_T("uxtheme.dll"));
			if(hMod != 0)
			{
				// Declare pointers to functions
				FARPROC pIsAppThemed   = ::GetProcAddress(hMod, "IsAppThemed");
				FARPROC pIsThemeActive = ::GetProcAddress(hMod, "IsThemeActive");

				if(pIsAppThemed && pIsThemeActive)
				{
					if(pIsAppThemed() && pIsThemeActive())
					{
						// Test if ComCtl32 dll used is version 6 or later
						IsXPThemed = (GetComCtlVersion() >= 600);
					}
				}
				::FreeLibrary(hMod);
			}
		}

		return IsXPThemed;
	}

  #endif // #ifndef _WIN32_WCE

	inline void LoadCommonControls()
	// Loads the common controls using InitCommonControlsEx or InitCommonControls.
	// Returns TRUE of InitCommonControlsEx is used.
	{
		// Load the Common Controls DLL
		HMODULE hComCtl = ::LoadLibrary(_T("COMCTL32.DLL"));
		if (hComCtl == 0)
			hComCtl = ::LoadLibrary(_T("COMMCTRL.DLL"));

		if (hComCtl)
		{
			// Declare a typedef for the InItCommonControlsEx function
			typedef BOOL WINAPI INIT_EX(INITCOMMONCONTROLSEX*);

  #ifdef _WIN32_WCE
			INIT_EX* pfnInitEx = (INIT_EX*)::GetProcAddress(hComCtl, _T("InitCommonControlsEx"));
  #else
			INIT_EX* pfnInitEx = (INIT_EX*)::GetProcAddress(hComCtl, "InitCommonControlsEx");
  #endif

			if (pfnInitEx)
			{
				// Load the full set of common controls
				INITCOMMONCONTROLSEX InitStruct;
				InitStruct.dwSize = sizeof(INITCOMMONCONTROLSEX);
				InitStruct.dwICC = ICC_WIN95_CLASSES|ICC_BAR_CLASSES|ICC_COOL_CLASSES|ICC_DATE_CLASSES;


  #ifndef _WIN32_WCE
				if (GetComCtlVersion() > 470)
				{
					InitStruct.dwICC |= ICC_INTERNET_CLASSES|ICC_NATIVEFNTCTL_CLASS|ICC_PAGESCROLLER_CLASS|ICC_USEREX_CLASSES;
				}
  #endif
  
				// Call InitCommonControlsEx
				if ( !((*pfnInitEx)(&InitStruct)) )
				{
					InitCommonControls();
				}

			}
			else
			{
				// InitCommonControlsEx not supported. Use older InitCommonControls
				InitCommonControls();
			}

			::FreeLibrary(hComCtl);
		}
	}

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

}


#endif	// _WIN32XX_WINUTILS_H_
