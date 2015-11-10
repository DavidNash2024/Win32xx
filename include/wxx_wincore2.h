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
// wxx_wincore2.h
// This file contains the definition of the CWnd class.


#ifndef _WIN32XX_WINCORE2_H_
#define _WIN32XX_WINCORE2_H_


#include "wxx_wincore1.h"


namespace Win32xx
{

	////////////////////////////////////////
	// Definitions for the CWnd class
	//
	inline CWnd::CWnd() : m_hWnd(NULL), m_PrevWindowProc(NULL)
	{
		// Note: m_hWnd is set in CWnd::CreateEx(...)
	}

	inline CWnd::CWnd(HWND hWnd) : m_PrevWindowProc(NULL)
	{
		// A private constructor, used internally.

		SetHwnd(hWnd);
	}

	inline CWnd::~CWnd()
	{
		// Destroys the window for this object and cleans up resources.
		Destroy();
	}

	inline void CWnd::AddToMap()
	// Store the window handle and CWnd pointer in the HWND map
	{
		assert( &GetApp() );
		GetApp().m_csMapLock.Lock();

		assert(!GetApp().GetCWndFromMap(*this));

		GetApp().m_mapHWND.insert(std::make_pair(GetHwnd(), this));
		GetApp().m_csMapLock.Release();
	}

	inline BOOL CWnd::Attach(HWND hWnd)
	// Subclass an existing window and attach it to a CWnd
	{
		assert( &GetApp() );
		assert( ::IsWindow(hWnd) );
		assert( !IsWindow() );

		// Ensure this thread has the TLS index set
		// Note: Perform the attach from the same thread as the window's message loop
		GetApp().SetTlsData();

		Cleanup();			// Cleanup any previous attachment
		Subclass(hWnd);		// Set the window's callback to CWnd::StaticWindowProc

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
		if (GetParent().GetHwnd() != NULL) rcParent = GetParent().GetWindowRect();
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
			HMONITOR hActiveMonitor = pfnMonitorFromWindow(*this, MONITOR_DEFAULTTONEAREST);
			MONITORINFO mi;
			ZeroMemory(&mi, sizeof(MONITORINFO));
			mi.cbSize = sizeof(MONITORINFO);

			if(pfnGetMonitorInfo(hActiveMonitor, &mi))
			{
				rcDesktop = mi.rcWork;
				if (GetParent().GetHwnd() == NULL) rcParent = mi.rcWork;
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
		if ( &GetApp() )
			RemoveFromMap();

		SetHwnd(NULL);
		m_PrevWindowProc = NULL;
	}

	inline HWND CWnd::Create(HWND hWndParent /* = NULL */)
	// Creates the window. This is the default method of window creation.
	{
		// Test if Win32++ has been started
		assert( &GetApp() );

		WNDCLASS wc;
		ZeroMemory(&wc, sizeof(WNDCLASS));

		CREATESTRUCT cs;
		ZeroMemory(&cs, sizeof(CREATESTRUCT));

		// Set the WNDCLASS parameters
		PreRegisterClass(wc);
		if (wc.lpszClassName)
		{
			RegisterClass(wc);
			cs.lpszClass = wc.lpszClassName;
		}
		else
			cs.lpszClass = _T("Win32++ Window");

		// Set a reasonable default window style
		DWORD dwOverlappedStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
		cs.style = WS_VISIBLE | ((hWndParent)? WS_CHILD : dwOverlappedStyle);

		// Set a reasonable default window position
		if (NULL == hWndParent)
		{
			cs.x  = CW_USEDEFAULT;
			cs.cx = CW_USEDEFAULT;
			cs.y  = CW_USEDEFAULT;
			cs.cy = CW_USEDEFAULT;
		}

		// Allow the CREATESTRUCT parameters to be modified
		PreCreate(cs);

		DWORD dwStyle = cs.style & ~WS_VISIBLE;
		HWND hWnd;

		// Create the window
#ifndef _WIN32_WCE
		hWnd = CreateEx(cs.dwExStyle, cs.lpszClass, cs.lpszName, dwStyle,
			    cs.x, cs.y, cs.cx, cs.cy, hWndParent,
				cs.hMenu, cs.lpCreateParams);

		if (cs.style & WS_VISIBLE)
		{
			if		(cs.style & WS_MAXIMIZE) ShowWindow(SW_MAXIMIZE);
			else if (cs.style & WS_MINIMIZE) ShowWindow(SW_MINIMIZE);
			else	ShowWindow();
		}

#else
		hWnd = CreateEx(cs.dwExStyle, cs.lpszClass, cs.lpszName, cs.style,
			    cs.x, cs.y, cs.cx, cs.cy, hWndParent,
				0, cs.lpCreateParams);
#endif

		return hWnd;
	}

	inline HWND CWnd::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rc, HWND hWndParent, UINT nID, LPVOID lpParam /*= NULL*/)
	// Creates the window by specifying all the window creation parameters
	{
		int x = rc.left;
		int y = rc.top;
		int cx = rc.right - rc.left;
		int cy = rc.bottom - rc.top;
		HMENU hMenu = hWndParent? (HMENU)(INT_PTR)nID : ::LoadMenu(GetApp().GetResourceHandle(), MAKEINTRESOURCE(nID));

		return CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, x, y, cx, cy, hWndParent, hMenu, lpParam);
	}

	inline HWND CWnd::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU nIDorHMenu, LPVOID lpParam /*= NULL*/)
	// Creates the window by specifying all the window creation parameters
	{
		assert( &GetApp() );		// Test if Win32++ has been started
		assert( !IsWindow() );	// Only one window per CWnd instance allowed

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
		{
			TRACE("*** RegisterClass failed ***\n");
			assert( 0 );
		}

		// Ensure this thread has the TLS index set
		TLSData* pTLSData = GetApp().SetTlsData();

		// Store the CWnd pointer in thread local storage
		pTLSData->pWnd = this;

		// Create window
		HWND hWnd = ::CreateWindowEx(dwExStyle, ClassName, lpszWindowName, dwStyle, x, y, nWidth, nHeight,
								hWndParent, nIDorHMenu, GetApp().GetInstanceHandle(), lpParam);


		// Automatically subclass predefined window class types
		::GetClassInfo(GetApp().GetInstanceHandle(), lpszClassName, &wc);
		if (wc.lpfnWndProc != GetApp().m_Callback)
		{
			Subclass(hWnd);

			// Override this to perform tasks after the window is attached.
			OnAttach();
		}

		// Now handle window creation failure
		if (hWnd == 0)
		{
			TRACE("*** Failed to create window ***\n");
			TRACE(SystemErrorMessage(::GetLastError()));
			assert(hWnd);
		}

		// Clear the CWnd pointer from TLS
		pTLSData->pWnd = NULL;

		// Window creation is complete. Now call OnInitialUpdate
		PostMessage(UWM_WINDOWCREATED);

		return hWnd;
	}

	inline void CWnd::Destroy()
	// Destroys the window and returns the CWnd back to its default state, ready for reuse.
	{
		if (GetCWndPtr(*this) == this)
		{
			if (IsWindow())
				::DestroyWindow(*this);
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

		HWND hWnd = GetHwnd();
		Cleanup();

		return hWnd;
	}

#ifndef _WIN32_WCE
	inline void CWnd::DoDataExchange(CDataExchange& DX)
	//	This function performs data exchange and validation functions on
	//	dialog data using DDX and DDV functions.  Never call this function
	//	directly. It is called by the UpdateData() member function. Call
	//	UpdateData() to write data into a set of dialog box controls or to
	//	retrieve data from that set of dialog boxes.  Override this method
	//	in the dialogs that utilize the DDX/DDV functions.
	{
		  // TODO: in each override of this method, enter DDX and DDV functions
		  // here for each of the contols attached to the parent dialog window
		  // that m_DX is assigned to.

		UNREFERENCED_PARAMETER(DX);
	}
#endif

	inline LRESULT CWnd::FinalWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	// Pass messages on to the appropriate default window procedure
	// CMDIChild and CMDIFrame override this function
	{
		if (m_PrevWindowProc)
			return ::CallWindowProc(m_PrevWindowProc, *this, uMsg, wParam, lParam);
		else
			return ::DefWindowProc(*this, uMsg, wParam, lParam);
	}

	inline CWnd* CWnd::GetCWndPtr(HWND hWnd)
	// Retrieves the pointer to the CWnd associated with the specified HWND.
	{
		assert( &GetApp() );
		return hWnd? GetApp().GetCWndFromMap(hWnd) : 0;
	}

	inline CWnd CWnd::GetAncestor(UINT gaFlags /*= GA_ROOTOWNER*/) const
	// The GetAncestor function retrieves the ancestor (root parent)
	// of the window. Supports Win95.
	{
		assert(IsWindow());
		HWND hWnd = GetHwnd();

		// Load the User32 DLL
		typedef HWND WINAPI GETANCESTOR(HWND, UINT);
		GETANCESTOR* pfnGetAncestor = NULL;
		HMODULE hModule = ::LoadLibrary(_T("USER32.DLL"));

		if (hModule != 0)
		{
			// Declare a pointer to the GetAncestor function
#ifndef _WIN32_WCE
			pfnGetAncestor = reinterpret_cast<GETANCESTOR*>(::GetProcAddress(hModule, "GetAncestor"));
#else
			pfnGetAncestor = reinterpret_cast<GETANCESTOR*>(::GetProcAddress(hModule, L"GetAncestor"));
#endif

			if (pfnGetAncestor)
				hWnd = (*pfnGetAncestor)(*this, gaFlags);

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
		::GetClassName(*this, str.GetBuffer(MAX_STRING_SIZE), MAX_STRING_SIZE);
		str.ReleaseBuffer();
		return str;
	}

	inline CString CWnd::GetDlgItemText(int nIDDlgItem) const
	// Retrieves the title or text associated with a control in a dialog box.
	{
		assert(IsWindow());

		int nLength = ::GetWindowTextLength(::GetDlgItem(*this, nIDDlgItem));
		CString str;
		::GetDlgItemText(*this, nIDDlgItem, str.GetBuffer(nLength), nLength+1);
		str.ReleaseBuffer();
		return str;
	}

	inline CString CWnd::GetWindowText() const
	// Retrieves the text of the window's title bar.
	{
		assert(IsWindow());

		int nLength = ::GetWindowTextLength(*this);
		CString str;
		::GetWindowText(*this, str.GetBuffer(nLength), nLength+1);
		str.ReleaseBuffer();
		return str;
	}

	inline void CWnd::OnClose()
	// Called in response to WM_CLOSE. Override to suppress destroying the window
	// WM_CLOSE is sent by SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0) or by clicking X
	//  in the top right corner. Usually, only top level windows receive WM_CLOSE.
	{
		::DestroyWindow(*this);
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

	inline void CWnd::OnInitialUpdate()
	{
		// This function is called automatically once the window is created
		// Override it in your derived class to automatically perform tasks
		// after window creation.
	}

	inline LRESULT CWnd::MessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
			hWnd = GetDlgItem((int)wParam);
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

		CWnd* Wnd = GetApp().GetCWndFromMap(hWnd);

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

	inline LRESULT CWnd::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// Window controls and other subclassed windows are expected to do their own
		// drawing, so we don't call OnDraw for those.

		// Note: CustomDraw or OwnerDraw are normally used to modify the drawing of
		//       controls, but overriding OnPaint is also an option.
		
		if (!m_PrevWindowProc)
		{
			if (::GetUpdateRect(*this, NULL, FALSE))
			{
				CPaintDC dc(*this);
				OnDraw(dc);
			}
			else
			// RedrawWindow can require repainting without an update rect
			{
				CClientDC dc(*this);
				OnDraw(dc);
			}

			// No more drawing required
			return 0L;
		}

		// Allow window controls to do their default drawing
		return FinalWindowProc(uMsg, wParam, lParam);
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
		assert( &GetApp() );
		assert( ('\0' != wc.lpszClassName[0] && ( lstrlen(wc.lpszClassName) <=  MAX_STRING_SIZE) ) );

		// Check to see if this classname is already registered
		WNDCLASS wcTest;
		ZeroMemory(&wcTest, sizeof(WNDCLASS));
		BOOL Done = FALSE;

		if (::GetClassInfo(GetApp().GetInstanceHandle(), wc.lpszClassName, &wcTest))
		{
			wc = wcTest;
			Done = TRUE;
		}

		if (!Done)
		{
			// Set defaults
			wc.hInstance	= GetApp().GetInstanceHandle();
			wc.lpfnWndProc	= CWnd::StaticWindowProc;

			// Register the WNDCLASS structure
			VERIFY ( ::RegisterClass(&wc) != 0 );

			Done = TRUE;
		}

		return Done;
	}

	inline BOOL CWnd::RemoveFromMap()
	{
		BOOL Success = FALSE;

		if ( &GetApp() )
		{
			// Allocate an iterator for our HWND map
			std::map<HWND, CWnd*, CompareHWND>::iterator m;

			CWinApp& App = GetApp();

			// Erase the CWnd pointer entry from the map
			App.m_csMapLock.Lock();
			for (m = App.m_mapHWND.begin(); m != App.m_mapHWND.end(); ++m)
			{
				if (this == m->second)
				{
					App.m_mapHWND.erase(m);
					Success = TRUE;
					break;
				}
			}

			App.m_csMapLock.Release();

		}

		return Success;
	}

	inline HICON CWnd::SetIconLarge(int nIcon)
	// Sets the large icon associated with the window
	{
		assert( &GetApp() );
		assert(IsWindow());

#ifndef _WIN32_WCE
		HICON hIconLarge = (HICON) (::LoadImage (GetApp().GetResourceHandle(), MAKEINTRESOURCE (nIcon), IMAGE_ICON,
		::GetSystemMetrics (SM_CXICON), ::GetSystemMetrics (SM_CYICON), LR_SHARED));
#else
		HICON hIconLarge = (HICON) (::LoadImage (GetApp().GetResourceHandle(), MAKEINTRESOURCE (nIcon), IMAGE_ICON,
		::GetSystemMetrics (SM_CXICON), ::GetSystemMetrics (SM_CYICON), 0));
#endif

		if (hIconLarge != 0)
			SendMessage (WM_SETICON, WPARAM (ICON_BIG), LPARAM (hIconLarge));
		else
			TRACE("**WARNING** SetIconLarge Failed\n");

		return hIconLarge;
	}

	inline HICON CWnd::SetIconSmall(int nIcon)
	// Sets the small icon associated with the window
	{
		assert( &GetApp() );
		assert(IsWindow());

#ifndef _WIN32_WCE
		HICON hIconSmall = (HICON) (::LoadImage (GetApp().GetResourceHandle(), MAKEINTRESOURCE (nIcon), IMAGE_ICON,
		::GetSystemMetrics (SM_CXSMICON), ::GetSystemMetrics (SM_CYSMICON), LR_SHARED));
#else
		HICON hIconSmall = (HICON) (::LoadImage (GetApp().GetResourceHandle(), MAKEINTRESOURCE (nIcon), IMAGE_ICON,
		::GetSystemMetrics (SM_CXSMICON), ::GetSystemMetrics (SM_CYSMICON), 0));
#endif

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
		assert( &GetApp() );

		CWnd* w = GetApp().GetCWndFromMap(hWnd);
		if (w == 0)
		{
			// The CWnd pointer wasn't found in the map, so add it now

			// Retrieve the pointer to the TLS Data
			TLSData* pTLSData = GetApp().GetTlsData();
			assert(pTLSData);

			// Retrieve pointer to CWnd object from Thread Local Storage TLS
			w = pTLSData->pWnd;
			assert(w);				// pTLSData->pCWnd is assigned in CreateEx
			pTLSData->pWnd = NULL;

			// Store the CWnd pointer in the HWND map
			w->SetHwnd(hWnd);
			w->AddToMap();
		}

		return w->WndProc(uMsg, wParam, lParam);

	} // LRESULT CALLBACK StaticWindowProc(...)

	inline void CWnd::Subclass(HWND hWnd)
	// A private function used by CreateEx, Attach and AttachDlgItem
	{
		assert(::IsWindow(hWnd));

		m_PrevWindowProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)CWnd::StaticWindowProc);
		SetHwnd(hWnd);
		
		AddToMap();			// Store the CWnd pointer in the HWND map
	}
#ifndef _WIN32_WCE
	inline BOOL CWnd::UpdateData(CDataExchange& DX, BOOL bReadFromControl)
	//	Dialog Data Exchange support. Call this function to read values from
	//	(bReadFromControl is TRUE) or deposit values into (bReadFromControl
	//	is FALSE) a set of controls appearing in DDX/DDV statements in the
	//	DoDataExchange() member method.
	//
	//	Return TRUE if the operation is successful, or FALSE otherwise. When
	//	called with bReadFromControl a TRUE value, success means the data has
	//	been validated.
	//
	//	When a dialog box is uses data exchange, its OnInitDialog should call 
	//  UpdateData(FALSE, FALSE) before the dialog box is visible. 
	//  Also, the implementation of its OnOK should call UpdatData(TRUE) to
	//  retrieve the data, and then close the dialog box.
	//	If the Cancel button is clicked in the dialog box, the dialog box should
	//	be closed without the data being retrieved.
	{
		// must not update data before the window is created
		assert(IsWindow());

		// A critical section which is released when ccs goes out of scope
		CCriticalSection ccs;
		ccs.Lock();

		DX.Init(*this, bReadFromControl);

		BOOL ok = FALSE;  // Remains FALSE if DoDataExchange throws a CUserException
		try
		{
			DoDataExchange(DX);
			if (DX.GetLastControl() != NULL && DX.GetLastEditControl() != NULL)
			{	
				// select all characters in the edit control
				::SetFocus(DX.GetLastEditControl());
				::SendMessage(DX.GetLastEditControl(), EM_SETSEL, 0, -1);
			}
			ok = TRUE; // DoDataExchage completed succesfully
		}
		catch(const CUserException& e)
		{
			// Validation has failed. Call the Fail to display the error.
			DX.Fail( e.GetText() );
		}

		return ok;
	}
#endif // _WIN32_WCE

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
				CWnd* pWnd = GetApp().GetCWndFromMap((HWND)lParam);
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
				CWnd* pWndFrom = GetApp().GetCWndFromMap(hwndFrom);

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
						CWnd* pWndFromParent = GetApp().GetCWndFromMap(::GetParent(hwndFrom));
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
				// OnPaint calls OnDraw when appropriate
				OnPaint(uMsg, wParam, lParam);
			}

			return 0L;

		case WM_ERASEBKGND:
			{
				CDC dc((HDC)wParam);
				BOOL bResult;

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
				lr = MessageReflect(uMsg, wParam, lParam);
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
		return ::BeginPaint(*this, &ps);
	}

	inline BOOL CWnd::BringWindowToTop() const
	// The BringWindowToTop function brings the specified window to the top
	// of the Z order. If the window is a top-level window, it is activated.
	{
        assert(IsWindow());
		return ::BringWindowToTop(*this);
	}

	inline LRESULT CWnd::CallWindowProc(WNDPROC lpPrevWndFunc, UINT Msg, WPARAM wParam, LPARAM lParam) const
	{
        assert(IsWindow());
		return ::CallWindowProc(lpPrevWndFunc, *this, Msg, wParam, lParam);
	}

	inline BOOL CWnd::CheckDlgButton(int nIDButton, UINT uCheck) const
	// The CheckDlgButton function changes the check state of a button control.
	{
        assert(IsWindow());
		return ::CheckDlgButton(*this, nIDButton, uCheck);
	}

	inline BOOL CWnd::CheckRadioButton(int nIDFirstButton, int nIDLastButton, int nIDCheckButton) const
	// The CheckRadioButton function adds a check mark to (checks) a specified radio button in a group
	// and removes a check mark from (clears) all other radio buttons in the group.
	{
		assert(IsWindow());
		return ::CheckRadioButton(*this, nIDFirstButton, nIDLastButton, nIDCheckButton);
	}

	inline CWnd CWnd::ChildWindowFromPoint(POINT pt) const
	// Determines which, if any, of the child windows belonging to a parent window contains
	// the specified point. The search is restricted to immediate child windows.
	// Grandchildren, and deeper descendant windows are not searched.
	{
		assert(IsWindow());
		return CWnd(::ChildWindowFromPoint(*this, pt));
	}

	inline BOOL CWnd::ClientToScreen(POINT& pt) const
	// The ClientToScreen function converts the client-area coordinates of a specified point to screen coordinates.
	{
		assert(IsWindow());
		return ::ClientToScreen(*this, &pt);
	}

	inline BOOL CWnd::ClientToScreen(RECT& rc) const
	// The ClientToScreen function converts the client-area coordinates of a specified RECT to screen coordinates.
	{
		assert(IsWindow());
		return static_cast<BOOL>(::MapWindowPoints(*this, NULL, (LPPOINT)&rc, 2));
	}

	inline HDWP CWnd::DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) const
	// The DeferWindowPos function updates the specified multiple-window – position structure for the window.
	// The hWndInsertAfter can one of:  HWND_BOTTOM, HWND_NOTOPMOST, HWND_TOP, or HWND_TOPMOST
	{
        assert(IsWindow());
		return ::DeferWindowPos(hWinPosInfo, *this, hWndInsertAfter, x, y, cx, cy, uFlags);
	}

	inline HDWP CWnd::DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, const RECT& rc, UINT uFlags) const
	// The DeferWindowPos function updates the specified multiple-window – position structure for the window.
	// The hWndInsertAfter can one of:  HWND_BOTTOM, HWND_NOTOPMOST, HWND_TOP, or HWND_TOPMOST
	{
		assert(IsWindow());
		return ::DeferWindowPos(hWinPosInfo, *this, hWndInsertAfter, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, uFlags);
	}

	inline LRESULT CWnd::DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam) const
	// This function provides default processing for any window messages that an application does not process.
	{
		assert(IsWindow());
		return ::DefWindowProc(*this, uMsg, wParam, lParam);
	}

	inline BOOL CWnd::DrawMenuBar() const
	// The DrawMenuBar function redraws the menu bar of the specified window. If the menu bar changes after
	// the system has created the window, this function must be called to draw the changed menu bar.
	{
		assert(IsWindow());
		return ::DrawMenuBar(*this);
	}

	inline BOOL CWnd::EnableWindow(BOOL bEnable /*= TRUE*/) const
	// The EnableWindow function enables or disables mouse and
	// keyboard input to the window.
	{
		assert(IsWindow());
		return ::EnableWindow(*this, bEnable);
	}

	inline BOOL CWnd::EndPaint(PAINTSTRUCT& ps) const
	// The EndPaint function marks the end of painting in the specified window. This function is required for
	// each call to the BeginPaint function, but only after painting is complete.
	{
		assert(IsWindow());
		return ::EndPaint(*this, &ps);
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
		return ::GetClassLongPtr(*this, nIndex);
	}

	inline CRect CWnd::GetClientRect() const
	// The GetClientRect function retrieves the coordinates of a window's client area.
	// The client coordinates specify the upper-left and lower-right corners of the
	// client area. Because client coordinates are relative to the upper-left corner
	// of a window's client area, the coordinates of the upper-left corner are (0,0).
	{
		assert(IsWindow());
		CRect rc;
		::GetClientRect(*this, &rc);
		return rc;
	}

	inline CDC CWnd::GetDC() const
	// The GetDC function retrieves the display device context (DC)
	// for the client area of the window. Use like this:
	// CDC dc = GetDC;
	{
		assert(IsWindow());
		return CClientDC(*this);
	}

	inline CDC CWnd::GetDCEx(HRGN hrgnClip, DWORD flags) const
	// The GetDCEx function retrieves a display device context (DC) for the
	// client area or entire area of a window. Use like this:
	// CDC dc = GetDCEx(hrgnClip, flags);
	{
		assert(IsWindow());
		return CClientDCEx(*this, hrgnClip, flags);
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
		return ::GetDlgCtrlID(*this);
	}

	inline CWnd CWnd::GetDlgItem(int nIDDlgItem) const
	// The GetDlgItem function retrieves a handle to a control in the dialog box.
	{
		assert(IsWindow());
		return CWnd( ::GetDlgItem(*this, nIDDlgItem) );
	}

	inline UINT CWnd::GetDlgItemInt(int nIDDlgItem, BOOL* lpTranslated, BOOL bSigned) const
	// The GetDlgItemInt function translates the text of a specified control in a dialog box into an integer value.
	{
		assert(IsWindow());
		return ::GetDlgItemInt(*this, nIDDlgItem, lpTranslated, bSigned);
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
		HFONT hFont = reinterpret_cast<HFONT>(SendMessage(WM_GETFONT, 0, 0));
		return CFont(hFont);
	}

	inline HICON CWnd::GetIcon(BOOL bBigIcon) const
	// Retrieves a handle to the large or small icon associated with a window.
	{
		assert(IsWindow());
		return reinterpret_cast<HICON>(SendMessage(WM_GETICON, (WPARAM)bBigIcon, 0));
	}

	inline CWnd CWnd::GetNextDlgGroupItem(HWND hCtl, BOOL bPrevious) const
	// The GetNextDlgGroupItem function retrieves the first control in a group of controls that
	// precedes (or follows) the specified control in a dialog box.
	{
		assert(IsWindow());
		return CWnd(::GetNextDlgGroupItem(*this, hCtl, bPrevious));
	}

	inline CWnd CWnd::GetNextDlgTabItem(HWND hCtl, BOOL bPrevious) const
	// The GetNextDlgTabItem function retrieves the first control that has the WS_TABSTOP style
	// that precedes (or follows) the specified control.
	{
		assert(IsWindow());
		return CWnd(::GetNextDlgTabItem(*this, hCtl, bPrevious));
	}

	inline CWnd CWnd::GetParent() const
	// The GetParent function retrieves the specified window's parent or owner.
	{
		assert(IsWindow());
		return CWnd(::GetParent(*this));
	}

	inline LONG_PTR CWnd::GetWindowLongPtr(int nIndex) const
	// The GetWindowLongPtr function retrieves information about the window.
	{
		assert(IsWindow());
		return ::GetWindowLongPtr(*this, nIndex);
	}

	inline BOOL CWnd::GetScrollInfo(int fnBar, SCROLLINFO& si) const
	// The GetScrollInfo function retrieves the parameters of a scroll bar, including
	// the minimum and maximum scrolling positions, the page size, and the position
	// of the scroll box (thumb).
	{
		assert(IsWindow());
		return ::GetScrollInfo(*this, fnBar, &si);
	}

	inline CRect CWnd::GetUpdateRect(BOOL bErase) const
	// The GetUpdateRect function retrieves the coordinates of the smallest rectangle that completely
	// encloses the update region of the specified window.
	{
		assert(IsWindow());
		CRect rc;
		::GetUpdateRect(*this, &rc, bErase);
		return rc;
	}

	inline int CWnd::GetUpdateRgn(HRGN hRgn, BOOL bErase) const
	// The GetUpdateRgn function retrieves the update region of a window by copying it into the specified region.
	{
		assert(IsWindow());
		return ::GetUpdateRgn(*this, hRgn, bErase);
	}

	inline CWnd CWnd::GetWindow(UINT uCmd) const
	// The GetWindow function retrieves a window that has the specified
	// relationship (Z-Order or owner) to the specified window.
	// Possible uCmd values: GW_CHILD, GW_ENABLEDPOPUP, GW_HWNDFIRST, GW_HWNDLAST,
	// GW_HWNDNEXT, GW_HWNDPREV, GW_OWNER
	{
		assert(IsWindow());
		return CWnd( ::GetWindow(*this, uCmd) );
	}

	inline CDC CWnd::GetWindowDC() const
	// The GetWindowDC function retrieves the device context (DC) for the entire window,
	// including title bar, menus, and scroll bars. Use like this:
	// CDC dc = GetWindowDC();
	{
		assert(IsWindow());
		return CWindowDC(*this);
	}

	inline CRect CWnd::GetWindowRect() const
	// retrieves the dimensions of the bounding rectangle of the window.
	// The dimensions are given in screen coordinates that are relative to the
	// upper-left corner of the screen.
	{
		assert(IsWindow());
		CRect rc;
		::GetWindowRect(*this, &rc);
		return rc;
	}

	inline int CWnd::GetWindowTextLength() const
	// The GetWindowTextLength function retrieves the length, in characters, of the specified window's
	// title bar text (if the window has a title bar).
	{
		assert(IsWindow());
		return ::GetWindowTextLength(*this);
	}

	inline void CWnd::Invalidate(BOOL bErase /*= TRUE*/) const
	// The Invalidate function adds the entire client area to the window's update region.
	// The update region represents the portion of the window's client area that must be redrawn.
	{
		assert(IsWindow());
		::InvalidateRect(*this, NULL, bErase);
	}

	inline BOOL CWnd::InvalidateRect(LPCRECT lpRect, BOOL bErase /*= TRUE*/) const
	// The InvalidateRect function adds a rectangle to the window's update region.
	// The update region represents the portion of the window's client area that must be redrawn.
	{
		assert(IsWindow());
		return ::InvalidateRect(*this, lpRect, bErase);
	}

	inline BOOL CWnd::InvalidateRgn(HRGN hRgn, BOOL bErase /*= TRUE*/) const
	// The InvalidateRgn function invalidates the client area within the specified region
	// by adding it to the current update region of a window. The invalidated region,
	// along with all other areas in the update region, is marked for painting when the
	// next WM_PAINT message occurs.
	{
		assert(IsWindow());
		return ::InvalidateRgn(*this, hRgn, bErase);
	}

	inline BOOL CWnd::IsChild(HWND hwndChild) const
	// The IsChild function tests whether a window is a child window or descendant window
	// of a parent window's CWnd.
	{
		assert(IsWindow());
		return ::IsChild(*this, hwndChild);
	}

	inline BOOL CWnd::IsDialogMessage(LPMSG lpMsg) const
	// The IsDialogMessage function determines whether a message is intended for the specified dialog box and,
	// if it is, processes the message.
	{
		assert(IsWindow());
		return ::IsDialogMessage(*this, lpMsg);
	}

	inline UINT CWnd::IsDlgButtonChecked(int nIDButton) const
	// The IsDlgButtonChecked function determines whether a button control has a check mark next to it
	// or whether a three-state button control is grayed, checked, or neither.
	{
		assert(IsWindow());
		return ::IsDlgButtonChecked(*this, nIDButton);
	}

	inline BOOL CWnd::IsWindowEnabled() const
	// The IsWindowEnabled function determines whether the window is enabled
	// for mouse and keyboard input.
	{
		assert(IsWindow());
		return ::IsWindowEnabled(*this);
	}

	inline BOOL CWnd::IsWindowVisible() const
	// The IsWindowVisible function retrieves the visibility state of the window.
	{
		// Microsoft's IsWindowVisible is buggy, so we do it like this
		assert(IsWindow());
		return (GetWindowLongPtr(GWL_STYLE) & WS_VISIBLE) != 0;
	}

	inline BOOL CWnd::IsWindow() const
	// The IsWindow function determines whether the window exists.
	{
		return ::IsWindow(*this);
	}

	inline void  CWnd::MapWindowPoints(HWND hWndTo, POINT& pt) const
	// The MapWindowPoints function converts (maps) a set of points from a coordinate space relative to one
	// window to a coordinate space relative to another window.
	{
		assert(IsWindow());
		::MapWindowPoints(*this, hWndTo, &pt, 1);
	}

	inline void CWnd::MapWindowPoints(HWND hWndTo, RECT& rc) const
	// The MapWindowPoints function converts (maps) a set of points from a coordinate space relative to one
	// window to a coordinate space relative to another window.
	{
		assert(IsWindow());
		::MapWindowPoints(*this, hWndTo, (LPPOINT)&rc, 2);
	}

	inline void CWnd::MapWindowPoints(HWND hWndTo, LPPOINT ptArray, UINT nCount) const
	// The MapWindowPoints function converts (maps) a set of points from a coordinate space relative to one
	// window to a coordinate space relative to another window.
	{
		assert(IsWindow());
		::MapWindowPoints(*this, hWndTo, (LPPOINT)ptArray, nCount);
	}

	inline int CWnd::MessageBox(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) const
	// The MessageBox function creates, displays, and operates a message box.
	// Possible combinations of uType values include: MB_OK, MB_HELP, MB_OKCANCEL, MB_RETRYCANCEL,
	// MB_YESNO, MB_YESNOCANCEL, MB_ICONEXCLAMATION, MB_ICONWARNING, MB_ICONERROR (+ many others).
	{
		assert(IsWindow());
		return ::MessageBox(*this, lpText, lpCaption, uType);
	}

	inline BOOL CWnd::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint /* = TRUE*/) const
	// The MoveWindow function changes the position and dimensions of the window.
	{
		assert(IsWindow());
		return ::MoveWindow(*this, x, y, nWidth, nHeight, bRepaint = TRUE);
	}

	inline BOOL CWnd::MoveWindow(const RECT& rc, BOOL bRepaint /* = TRUE*/) const
	// The MoveWindow function changes the position and dimensions of the window.
	{
		assert(IsWindow());
		return ::MoveWindow(*this, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, bRepaint);
	}

	inline BOOL CWnd::PostMessage(UINT uMsg, WPARAM wParam /*= 0L*/, LPARAM lParam /*= 0L*/) const
	// The PostMessage function places (posts) a message in the message queue
	// associated with the thread that created the window and returns without
	// waiting for the thread to process the message.
	{
		assert(IsWindow());
		return ::PostMessage(*this, uMsg, wParam, lParam);
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
		return ::RedrawWindow(*this, lpRectUpdate, hRgn, flags);
	}

	inline int CWnd::ReleaseDC(HDC hDC) const
	// The ReleaseDC function releases a device context (DC), freeing it for use
	// by other applications.
	{
		assert(IsWindow());
		return ::ReleaseDC(*this, hDC);
	}

	inline BOOL CWnd::ScreenToClient(POINT& Point) const
	// The ScreenToClient function converts the screen coordinates of a specified point on the screen to client-area coordinates.
	{
		assert(IsWindow());
		return ::ScreenToClient(*this, &Point);
	}

	inline BOOL CWnd::ScreenToClient(RECT& rc) const
	// The ScreenToClient function converts the screen coordinates of a specified RECT on the screen to client-area coordinates.
	{
		assert(IsWindow());
		return static_cast<BOOL>(::MapWindowPoints(NULL, *this, (LPPOINT)&rc, 2));
	}

	inline LRESULT CWnd::SendDlgItemMessage(int nIDDlgItem, UINT Msg, WPARAM wParam, LPARAM lParam) const
	// The SendDlgItemMessage function sends a message to the specified control in a dialog box.
	{
		assert(IsWindow());
		return ::SendDlgItemMessage(*this, nIDDlgItem, Msg, wParam, lParam);
	}

	inline LRESULT CWnd::SendMessage(UINT uMsg, WPARAM wParam /*= 0L*/, LPARAM lParam /*= 0L*/) const
	// The SendMessage function sends the specified message to a window or windows.
	// It calls the window procedure for the window and does not return until the
	// window procedure has processed the message.
	{
		assert(IsWindow());
		return ::SendMessage(*this, uMsg, wParam, lParam);
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
		return ::SendNotifyMessage(*this, Msg, wParam, lParam);
	}

	inline CWnd CWnd::SetActiveWindow() const
	// The SetActiveWindow function activates the window, but
	// not if the application is in the background.
	{
		assert(IsWindow());
		return CWnd( ::SetActiveWindow(*this) );
	}

	inline CWnd CWnd::SetCapture() const
	// The SetCapture function sets the mouse capture to the window.
	// SetCapture captures mouse input either when the mouse is over the capturing
	// window, or when the mouse button was pressed while the mouse was over the
	// capturing window and the button is still down.
	{
		assert(IsWindow());
		return CWnd( ::SetCapture(*this) );
	}

	inline ULONG_PTR CWnd::SetClassLongPtr(int nIndex, LONG_PTR dwNewLong) const
	// The SetClassLongPtr function replaces the specified value at the specified offset in the
	// extra class memory or the WNDCLASSEX structure for the class to which the window belongs.
	{
		assert(IsWindow());
		return ::SetClassLongPtr(*this, nIndex, dwNewLong);
	}

	inline CWnd CWnd::SetFocus() const
	// The SetFocus function sets the keyboard focus to the window.
	{
		assert(IsWindow());
		return CWnd( ::SetFocus(*this) );
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
		return reinterpret_cast<HICON>(SendMessage(WM_SETICON, (WPARAM)bBigIcon, (LPARAM)hIcon));
	}

	inline BOOL CWnd::SetForegroundWindow() const
	// The SetForegroundWindow function puts the thread that created the window into the
	// foreground and activates the window.
	{
		assert(IsWindow());
		return ::SetForegroundWindow(*this);
	}

	inline CWnd CWnd::SetParent(HWND hWndParent) const
	// The SetParent function changes the parent window of the child window.
	{
		assert(IsWindow());
		return CWnd(::SetParent(*this, hWndParent));
	}

	inline BOOL CWnd::SetRedraw(BOOL bRedraw /*= TRUE*/) const
	// This function allows changes in that window to be redrawn or prevents changes
	// in that window from being redrawn.
	{
		assert(IsWindow());
		return static_cast<BOOL>(::SendMessage(*this, WM_SETREDRAW, (WPARAM)bRedraw, 0L));
	}

	inline LONG_PTR CWnd::SetWindowLongPtr(int nIndex, LONG_PTR dwNewLong) const
	// The SetWindowLongPtr function changes an attribute of the window.
	{
		assert(IsWindow());
		return ::SetWindowLongPtr(*this, nIndex, dwNewLong);
	}

	inline BOOL CWnd::SetWindowPos(HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) const
	// The SetWindowPos function changes the size, position, and Z order of a child, pop-up,
	// or top-level window.
	// The pInsertAfter can one of:  &wndTop, &wndTopMost, &wndBottom, or &wndNoTopMost
	{
		assert(IsWindow());
	//	HWND hWndInsertAfter = pInsertAfter? pInsertAfter->GetHwnd() : (HWND)0;
		return ::SetWindowPos(*this, hWndInsertAfter, x, y, cx, cy, uFlags);
	}

	inline BOOL CWnd::SetWindowPos(HWND hWndInsertAfter, const RECT& rc, UINT uFlags) const
	// The SetWindowPos function changes the size, position, and Z order of a child, pop-up,
	// or top-level window.
	// The SetWindowPos function changes the size, position, and Z order of a child, pop-up,
	// or top-level window.
	// The pInsertAfter can one of:  &wndTop, &wndTopMost, &wndBottom, or &wndNoTopMost
	{
		assert(IsWindow());
		return ::SetWindowPos(*this, hWndInsertAfter, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, uFlags);
	}

	inline int CWnd::SetWindowRgn(CRgn& Rgn, BOOL bRedraw /*= TRUE*/) const
	// The SetWindowRgn function sets the window region of the window.
	// The window region determines the area within the window where the system permits drawing.
	// The window now owns the region so it is detached from Rgn.
	{
		assert(IsWindow());
		HRGN hRgn = (HRGN)Rgn.GetHandle();
		int iResult = ::SetWindowRgn(*this, hRgn, bRedraw);
		if (iResult && hRgn)
			Rgn.Detach();	// The system owns the region now

		return iResult;
	}

	inline LONG_PTR CWnd::SetDlgCtrlID(int idCtrl) const
	// Assigns an id to the window. Note that only child windows can have an ID assigned
	{
		assert(IsWindow());
		return SetWindowLongPtr(GWLP_ID, idCtrl);
	}

	inline BOOL CWnd::SetDlgItemInt(int nIDDlgItem, UINT uValue, BOOL bSigned) const
	// The SetDlgItemInt function sets the text of a control in a dialog box to the string representation of a specified integer value.
	{
		assert(IsWindow());
		return ::SetDlgItemInt(*this, nIDDlgItem, uValue, bSigned);
	}

	inline BOOL CWnd::SetDlgItemText(int nIDDlgItem, LPCTSTR lpString) const
	// The SetDlgItemText function sets the title or text of a control in a dialog box.
	{
		assert(IsWindow());
		return ::SetDlgItemText(*this, nIDDlgItem, lpString);
	}

	inline UINT_PTR CWnd::SetTimer(UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc) const
	// Creates a timer with the specified time-out value.
	{
		assert(IsWindow());
		return ::SetTimer(*this, nIDEvent, uElapse, lpTimerFunc);
	}

	inline BOOL CWnd::SetWindowText(LPCTSTR lpString) const
	// The SetWindowText function changes the text of the window's title bar (if it has one).
	{
		assert(IsWindow());
		return ::SetWindowText(*this, lpString);
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

			hr = (*pfn)(*this, pszSubAppName, pszSubIdList);

			::FreeLibrary(hMod);
		}

#endif

		return hr;
	}

	inline BOOL CWnd::ShowWindow(int nCmdShow /*= SW_SHOWNORMAL*/) const
	// The ShowWindow function sets the window's show state.
	{
		assert(IsWindow());
		return ::ShowWindow(*this, nCmdShow);
	}

	inline BOOL CWnd::UpdateWindow() const
	// The UpdateWindow function updates the client area of the window by sending a
	// WM_PAINT message to the window if the window's update region is not empty.
	// If the update region is empty, no message is sent.
	{
		assert(IsWindow());
		return ::UpdateWindow(*this);
	}

	inline BOOL CWnd::ValidateRect(LPCRECT prc) const
	// The ValidateRect function validates the client area within a rectangle by
	// removing the rectangle from the update region of the window.
	{
		assert(IsWindow());
		return ::ValidateRect(*this, prc);
	}

	inline BOOL CWnd::ValidateRgn(HRGN hRgn) const
	// The ValidateRgn function validates the client area within a region by
	// removing the region from the current update region of the window.
	{
		assert(IsWindow());
		return ::ValidateRgn(*this, hRgn);
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
		return ::CloseWindow(*this);
	}

	inline int CWnd::DlgDirList(LPTSTR lpPathSpec, int nIDListBox, int nIDStaticPath, UINT uFileType) const
	// The DlgDirList function replaces the contents of a list box with the names of the subdirectories and files
	// in a specified directory. You can filter the list of names by specifying a set of file attributes.
	{
		assert(IsWindow());
		return ::DlgDirList(*this, lpPathSpec, nIDListBox, nIDStaticPath, uFileType);
	}

	inline int CWnd::DlgDirListComboBox(LPTSTR lpPathSpec, int nIDComboBox, int nIDStaticPath, UINT uFiletype) const
	// The DlgDirListComboBox function replaces the contents of a combo box with the names of the subdirectories
	// and files in a specified directory. You can filter the list of names by specifying a set of file attributes.
	{
		assert(IsWindow());
		return ::DlgDirListComboBox(*this, lpPathSpec, nIDComboBox, nIDStaticPath, uFiletype);
	}

	inline BOOL CWnd::DlgDirSelectEx(LPTSTR lpString, int nCount, int nIDListBox) const
	// The DlgDirSelectEx function retrieves the current selection from a single-selection list box. It assumes that the list box
	// has been filled by the DlgDirList function and that the selection is a drive letter, filename, or directory name.
	{
		assert(IsWindow());
		return ::DlgDirSelectEx(*this, lpString, nCount, nIDListBox);
	}

	inline BOOL CWnd::DlgDirSelectComboBoxEx(LPTSTR lpString, int nCount, int nIDComboBox) const
	// The DlgDirSelectComboBoxEx function retrieves the current selection from a combo box filled by using the
	// DlgDirListComboBox function. The selection is interpreted as a drive letter, a file, or a directory name.
	{
		assert(IsWindow());
		return ::DlgDirSelectComboBoxEx(*this, lpString, nCount, nIDComboBox);
	}

    #ifndef WIN32_LEAN_AND_MEAN
    inline void CWnd::DragAcceptFiles(BOOL fAccept) const
	// Registers whether a window accepts dropped files.
	{
		assert(IsWindow());
		::DragAcceptFiles(*this, fAccept);
	}
    #endif

	inline BOOL CWnd::DrawAnimatedRects(int idAni, RECT& rcFrom, RECT& rcTo) const
	// The DrawAnimatedRects function draws a wire-frame rectangle and animates it to indicate the opening of
	// an icon or the minimizing or maximizing of a window.
	{
		assert(IsWindow());
		return ::DrawAnimatedRects(*this, idAni, &rcFrom, &rcTo);
	}

	inline BOOL CWnd::DrawCaption(HDC hDC, RECT& rc, UINT uFlags) const
	// The DrawCaption function draws a window caption.
	{
		assert(IsWindow());
		return ::DrawCaption(*this, hDC, &rc, uFlags);
	}

	inline BOOL CWnd::EnableScrollBar(UINT uSBflags, UINT uArrows) const
	// The EnableScrollBar function enables or disables one or both scroll bar arrows.
	{
		assert(IsWindow());
		return ::EnableScrollBar(*this, uSBflags, uArrows);
	}

	inline CWnd CWnd::GetLastActivePopup() const
	// The GetLastActivePopup function determines which pop-up window owned by the specified window was most recently active.
	{
		assert(IsWindow());
		return CWnd( ::GetLastActivePopup(*this) );
	}

	inline CMenu CWnd::GetMenu() const
	// The GetMenu function retrieves a handle to the menu assigned to the window.
	{
		assert(IsWindow());
		return CMenu(::GetMenu(*this));
	}

	inline int CWnd::GetScrollPos(int nBar) const
	// The GetScrollPos function retrieves the current position of the scroll box
	// (thumb) in the specified scroll bar.
	{
		assert(IsWindow());
		return ::GetScrollPos(*this, nBar);
	}

	inline BOOL CWnd::GetScrollRange(int nBar, int& MinPos, int& MaxPos) const
	// The GetScrollRange function retrieves the current minimum and maximum scroll box
	// (thumb) positions for the specified scroll bar.
	{
		assert(IsWindow());
		return ::GetScrollRange(*this, nBar, &MinPos, &MaxPos );
	}

	inline CMenu CWnd::GetSystemMenu(BOOL bRevert) const
	// The GetSystemMenu function allows the application to access the window menu (also known as the system menu
	// or the control menu) for copying and modifying.
	{
		assert(IsWindow());
		return CMenu( ::GetSystemMenu(*this, bRevert) );
	}

	inline CWnd CWnd::GetTopWindow() const
	// The GetTopWindow function examines the Z order of the child windows associated with the parent window and
	// retrieves a handle to the child window at the top of the Z order.
	{
		assert(IsWindow());
		return CWnd( ::GetTopWindow(*this) );
	}

	inline BOOL CWnd::GetWindowPlacement(WINDOWPLACEMENT& wndpl) const
	// The GetWindowPlacement function retrieves the show state and the restored,
	// minimized, and maximized positions of the window.
	{
		assert(IsWindow());
		return ::GetWindowPlacement(*this, &wndpl);
	}

	inline BOOL CWnd::HiliteMenuItem(HMENU hMenu, UINT uItemHilite, UINT uHilite) const
	// The HiliteMenuItem function highlights or removes the highlighting from an item in a menu bar.
	{
		assert(IsWindow());
		return ::HiliteMenuItem(*this, hMenu, uItemHilite, uHilite);
	}

	inline BOOL CWnd::IsIconic() const
	// The IsIconic function determines whether the window is minimized (iconic).
	{
		assert(IsWindow());
		return ::IsIconic(*this);
	}

	inline BOOL CWnd::IsZoomed() const
	// The IsZoomed function determines whether the window is maximized.
	{
		assert(IsWindow());
		return ::IsZoomed(*this);
	}

	inline BOOL CWnd::KillTimer(UINT_PTR uIDEvent) const
	// Destroys the specified timer.
	{
		assert(IsWindow());
		return ::KillTimer(*this, uIDEvent);
	}

	inline BOOL CWnd::LockWindowUpdate() const
	// Disables drawing in the window. Only one window can be locked at a time.
	// Use UnLockWindowUpdate to re-enable drawing in the window
	{
		assert(IsWindow());
		return ::LockWindowUpdate(*this);
	}

	inline BOOL CWnd::OpenIcon() const
	// The OpenIcon function restores a minimized (iconic) window to its previous size and position.
	{
		assert(IsWindow());
		return ::OpenIcon(*this);
	}

	inline void CWnd::Print(HDC hDC, DWORD dwFlags) const
	// Requests that the window draw itself in the specified device context, most commonly in a printer device context.
	{
		assert(IsWindow());
		SendMessage(*this, WM_PRINT, (WPARAM)hDC, (LPARAM)dwFlags);
	}

	inline BOOL CWnd::ScrollWindow(int XAmount, int YAmount, LPCRECT lprcScroll, LPCRECT lprcClip) const
	// The ScrollWindow function scrolls the contents of the specified window's client area.
	{
		assert(IsWindow());
		return ::ScrollWindow(*this, XAmount, YAmount, lprcScroll, lprcClip);
	}

	inline int CWnd::ScrollWindowEx(int dx, int dy, LPCRECT lprcScroll, LPCRECT lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate, UINT flags) const
	// The ScrollWindow function scrolls the contents of the window's client area.
	{
		assert(IsWindow());
		return ::ScrollWindowEx(*this, dx, dy, lprcScroll, lprcClip, hrgnUpdate, lprcUpdate, flags);
	}

	inline BOOL CWnd::SetMenu(HMENU hMenu) const
	// The SetMenu function assigns a menu to the specified window.
	// A hMenu of NULL removes the menu.
	{
		assert(IsWindow());
		return ::SetMenu(*this, hMenu);
	}

	inline int CWnd::SetScrollInfo(int fnBar, const SCROLLINFO& si, BOOL fRedraw) const
	// The SetScrollInfo function sets the parameters of a scroll bar, including
	// the minimum and maximum scrolling positions, the page size, and the
	// position of the scroll box (thumb).
	{
		assert(IsWindow());
		return ::SetScrollInfo(*this, fnBar, &si, fRedraw);
	}

	inline int CWnd::SetScrollPos(int nBar, int nPos, BOOL bRedraw) const
	// The SetScrollPos function sets the position of the scroll box (thumb) in
	// the specified scroll bar.
	{
		assert(IsWindow());
		return ::SetScrollPos(*this, nBar, nPos, bRedraw);
	}

	inline BOOL CWnd::SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw) const
	// The SetScrollRange function sets the minimum and maximum scroll box positions for the scroll bar.
	{
		assert(IsWindow());
		return ::SetScrollRange(*this, nBar, nMinPos, nMaxPos, bRedraw);
	}

	inline BOOL CWnd::SetWindowPlacement(const WINDOWPLACEMENT& wndpl) const
	// The SetWindowPlacement function sets the show state and the restored, minimized,
	// and maximized positions of the window.
	{
		assert(IsWindow());
		return ::SetWindowPlacement(*this, &wndpl);
	}

	inline BOOL CWnd::ShowOwnedPopups(BOOL fShow) const
	// The ShowOwnedPopups function shows or hides all pop-up windows owned by the specified window.
	{
		assert(IsWindow());
		return ::ShowOwnedPopups(*this, fShow);
	}

	inline BOOL CWnd::ShowScrollBar(int nBar, BOOL bShow) const
	// The ShowScrollBar function shows or hides the specified scroll bar.
	{
		assert(IsWindow());
		return ::ShowScrollBar(*this, nBar, bShow);
	}

	inline BOOL CWnd::ShowWindowAsync(int nCmdShow) const
	// The ShowWindowAsync function sets the show state of a window created by a different thread.
	{
		assert(IsWindow());
		return ::ShowWindowAsync(*this, nCmdShow);
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



	////////////////////////////////////////
	// Global Functions
	//

  #ifndef _WIN32_WCE		// for Win32/64 operating systems, not WinCE

	inline UINT GetSizeofNonClientMetrics()
	// This function correctly determines the sizeof NONCLIENTMETRICS
	{

  #if (WINVER >= 0x0600)
		// Is OS version less than Vista, adjust size to correct value
		if (GetWinVersion() < 2600)
			return CCSIZEOF_STRUCT(NONCLIENTMETRICS, lfMessageFont);
  #endif

		return sizeof(NONCLIENTMETRICS);
	}

	inline BOOL IsLeftButtonDown()
	// Reports the state of the left mouse button
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
}


#endif // _WIN32XX_WINCORE2_H_

