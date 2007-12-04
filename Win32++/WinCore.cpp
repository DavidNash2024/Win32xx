// Latest verion available at:
// http://sourceforge.net/projects/win32-framework


// Win32++  Version 5.61
// Released: 5th December, 2007 by:
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


////////////////////////////////////////////////////////
// WinCore.cpp
//  Definitions for the following classes:
//  CWinApp, CWnd, and CWinException

#include "WinCore.h"
#include "Default_Resource.h"


namespace Win32xx
{
	////////////////////////////////////////////////
	// Global variables within the Win32xx namespace
	//

	// Static variable for Thread Local Storage Index
	DWORD CWinApp::st_dwTlsIndex = TLS_OUT_OF_INDEXES;

	// Static variable for the pointer to the CWinApp object
	CWinApp* CWinApp::st_pTheApp = 0;

	///////////////////////////////////
	// Definitions for the CWinApp class
	//

	// To begin Win32++, inherit your application class from this one.
	// You should run only one instance of the class inherited from this.
	CWinApp::CWinApp(HINSTANCE hInstance) : m_hAccelTable(NULL), m_hWndAccel(NULL), m_hFont(NULL),
						m_hInstance(hInstance), m_hResource(hInstance), m_hRichEdit(NULL), m_hTraceEdit(NULL),
						m_IsTlsAllocatedHere(FALSE), m_pFrame(NULL)
	{
		try
		{
			// Test if this is the first instance of CWinApp
			if (GetApp() == 0)
			{
				st_dwTlsIndex = ::TlsAlloc();
				if (st_dwTlsIndex != TLS_OUT_OF_INDEXES)
				{
					st_pTheApp = this;
					m_IsTlsAllocatedHere = TRUE; //TLS allocated in this CWinApp object
				}
				else
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
		}

		catch (const CWinException &e)
		{
			// Indicate the problem
			e.MessageBox();
			if (st_pTheApp == NULL)
				throw;
		}
	}

	CWinApp::~CWinApp()
	{
		if (m_IsTlsAllocatedHere)
		{
			// Check that all CWnd windows are destroyed
			std::map<HWND, CWnd*, CompareHWND>::iterator m;
			for (m = m_HWNDmap.begin(); m != m_HWNDmap.end(); m++)
			{
				(*m).second->DestroyWindow();
			}
			m_HWNDmap.clear();

			// Do remaining tidy up
			if (st_dwTlsIndex != TLS_OUT_OF_INDEXES)
			{
				::TlsSetValue(GetTlsIndex(), NULL);
				::TlsFree(st_dwTlsIndex);
				st_dwTlsIndex = TLS_OUT_OF_INDEXES;
			}
			st_pTheApp = 0;
		}

		std::vector<TLSData*>::iterator iter;
		for(iter = m_ThreadData.begin(); iter != m_ThreadData.end(); iter++)
		{
			delete *(iter);
		}

		if (m_hRichEdit)
			::FreeLibrary(m_hRichEdit);
		if (m_hFont)
			::DeleteObject(m_hFont);
		if (m_hAccelTable)
			::DestroyAcceleratorTable(m_hAccelTable);
	}

	void CWinApp::CreateTrace()
	// Called automatically in CWinApp::Run if _DEBUG is defined.
	// You may called this function in release mode if you want to see the trace window.
	{
		if (m_Trace.GetHwnd() != 0)
		{
			DebugErrMsg(_T("Error, CreateTrace should only be called once"));
			return;
		}

		m_hRichEdit = ::LoadLibrary(_T("RICHED32.DLL"));
		if (!m_hRichEdit)
			DebugErrMsg(_T("Failed to load the RichEdit dll"));

		// Position window at the botton right of the desktop area
		RECT r = {0};
		::SystemParametersInfo(SPI_GETWORKAREA, 0, &r, 0);
		r.top = r.bottom - TRACE_HEIGHT;
		r.left = r.right - TRACE_WIDTH;
		DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION /*| WS_VISIBLE*/;

		m_Trace.CreateEx(WS_EX_TOPMOST, _T("TRACE"), _T("Trace Window"), dwStyle, r, NULL, NULL);

		::GetClientRect(m_Trace.GetHwnd(), &r);
		dwStyle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL;

		m_hTraceEdit = ::CreateWindowEx(0L, _T("RichEdit"), _T(""), dwStyle, r.left, r.top, r.right - r.left, r.bottom - r.top,
					m_Trace.GetHwnd(), NULL, GetApp()->GetInstanceHandle(), NULL);

		// Set a default font
		m_hFont = ::CreateFont(16, 0, 0, 0, FW_DONTCARE, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Courier New"));
		::SendMessage(m_hTraceEdit, WM_SETFONT, (WPARAM)m_hFont, 0);
	}

	BOOL CWinApp::InitInstance()
	{
		// InitInstance contains the initialization code for your application
		// You should override this function with the code to run when the application starts.

		// return TRUE to indicate success. FALSE will end the application
		return TRUE;
	}

	int CWinApp::MessageLoop()
	{
		// This gets any messages queued for the application, and dispatches them.

		MSG uMsg;
		int status;

		while((status = ::GetMessage(&uMsg, NULL, 0, 0))!= 0)
		{
			if (status == -1) return -1;

			if (!::TranslateAccelerator(m_hWndAccel, m_hAccelTable, &uMsg))
			{
				::TranslateMessage(&uMsg);
				::DispatchMessage(&uMsg);
			}
		}
		return LOWORD(uMsg.wParam);
	}

	int CWinApp::Run()
	{
		// Create an invisible Trace window in Debug mode
		// It's made visible on first use
#ifdef _DEBUG
		CreateTrace();
#endif

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

	void CWinApp::SetAccelerators(UINT ID_ACCEL, HWND hWndAccel)
	// ID_ACCEL is the resource ID of the accelerator table
	// hWndAccel is the window handle for translated messages
	{

		if (m_hAccelTable)
			::DestroyAcceleratorTable(m_hAccelTable);

		m_hWndAccel = hWndAccel;

		m_hAccelTable = ::LoadAccelerators(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(ID_ACCEL));
		if (!m_hAccelTable)
			DebugWarnMsg(_T("Load Accelerators failed"));
	}

	TLSData* CWinApp::SetTlsIndex()
	{
		try
		{
			// Called once for any thread that has a CWnd object
			if ((TLSData*)::TlsGetValue(GetTlsIndex()) != NULL)
				throw CWinException(_T("CWinApp::SetTlsIndex    Error, attempted to set TLS more than once"));

			TLSData* pTLSData = new TLSData;
			// Some MS compilers (including VS2003 under some circumstances) return NULL instead of throwing
			//  an exception when new fails. We make sure an exception gets thrown!
			if (pTLSData == NULL)
				throw std::bad_alloc();

			ZeroMemory(pTLSData, sizeof(TLSData));
			::TlsSetValue(GetTlsIndex(), pTLSData);

			// Store pointer in vector for deletion in destructor
			m_ThreadData.push_back(pTLSData);
			return pTLSData;
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
			// No need to rethrow this message.
		}

		catch (const std::bad_alloc &)
		{
			DebugErrMsg(_T("Failed to allocate mememory in CWinApp::SetTlsIndex"));
			throw; // Critical problem, so rethrow
		}

		return 0;
	}

	void CWinApp::Trace(LPCTSTR szString)
	// Used by the TRACE macro to output text to the trace window
	// Note: The TRACE macro is only active in debug mode (i.e when _DEBUG is defined)
	//       If you wish to see a trace window in release mode:
	//         1) Call CreateTrace to create the trace window
	//		   2) Call this function directly instead of TRACE to see trace output.
	{
		// CreateTrace must be called once before using this function
		if (m_hTraceEdit == 0)
		{
			::MessageBox(NULL, _T("Must call CreateTrace before Trace"), _T("Error"), MB_OK);
			return;
		}

		// The Trace window is initially invisible. Make it visible now.
		if (!::IsWindowVisible(m_Trace.GetHwnd()))
			::ShowWindow(m_Trace.GetHwnd(), SW_SHOWNA);

		HWND PreFocus = ::GetFocus();

		// Add CR LF to the end
		TCHAR str[80] = _T("");
		::lstrcpyn(str, szString, 77);
		::lstrcat(str, _T("\r\n"));

		::SendMessage(m_hTraceEdit, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)str);
		::SendMessage(m_hTraceEdit, EM_SCROLLCARET, (WPARAM)0, (LPARAM)0);

		SetFocus(PreFocus);
	}


	////////////////////////////////////////
	// Definitions for the CWnd class
	//
	CWnd::CWnd() : m_hWnd(NULL), m_hWndParent(NULL), m_pTLSData(NULL), m_hIconLarge(NULL), m_hIconSmall(NULL),
					m_PrevWindowProc(NULL), m_hBrushBkgnd(NULL)
	{
		// Note: m_hWnd and m_hWndParent are set in CWnd::CreateEx(...)
		::ZeroMemory(&m_cs, sizeof(CREATESTRUCT));
		m_szString[0] = _T('\0');

	}

	CWnd::~CWnd()
	{
		// Destroy the window for this object
		if (::IsWindow(m_hWnd)) DestroyWindow();

		// Destroy the GDI objects
		if (m_hIconLarge) ::DestroyIcon(m_hIconLarge);
		if (m_hIconSmall) ::DestroyIcon(m_hIconSmall);
		if (m_hBrushBkgnd) ::DeleteObject(m_hBrushBkgnd);

		// Remove the map entries
		if (GetApp())
		{
			std::map<HWND, CWnd*, CompareHWND>::iterator m;
			m = GetApp()->GetHWNDMap().begin();
			while (m != GetApp()->GetHWNDMap().end())
			{
				if (m->second == this)
					GetApp()->GetHWNDMap().erase(m++);
				else
					m++;
			}
		}
	}

	BOOL CWnd::Attach(HWND hWnd)
	// Subclasses an existing window and attaches it to a CWnd
	{
		try
		{
			if (IsWindow(hWnd))
			{
				// Allocate an iterator for our HWND map
				std::map<HWND, CWnd*, CompareHWND>::iterator m;

				GetApp()->m_MapLock.Lock();
				m = GetApp()->GetHWNDMap().find(hWnd);
				GetApp()->m_MapLock.Release();
				if (m != GetApp()->GetHWNDMap().end())
					throw CWinException(_T("Window already attached to this CWnd object"));

				m_hWnd = hWnd;
				Subclass();

				if (m_PrevWindowProc)
				{
					// Store the CWnd pointer in the HWND map
					GetApp()->m_MapLock.Lock();
					GetApp()->GetHWNDMap().insert(std::make_pair(hWnd, this));
					GetApp()->m_MapLock.Release();

					m_hWnd = hWnd;
					m_hWndParent = ::GetParent(hWnd);
					return TRUE;
				}
				else
				{
					m_hWnd = NULL;
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

	BOOL CWnd::AttachDlgItem(UINT nID, CWnd* pParent)
	// Use to convert a dialog item to a CWnd object
	{
		HWND hWnd = ::GetDlgItem(pParent->GetHwnd(), nID);

		return Attach(hWnd);
	}

	LRESULT CWnd::CallPrevWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::CallWindowProc(m_PrevWindowProc, hWnd, uMsg, wParam, lParam);
	}

	void CWnd::CenterWindow()
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

	HWND CWnd::Create(HWND hWndParent /* = NULL */)
	// Default Window Creation.
	{
		// Set the CREATESTRUCT parameters
		PreCreate(m_cs);

		// Set the Window Class Name
		TCHAR szClassName[MAX_STRING_SIZE + 1] = _T("Win32++ Window");
		if (m_cs.lpszClass)
			::lstrcpyn(szClassName, m_cs.lpszClass, MAX_STRING_SIZE);

		// Set Parent
		if (!hWndParent && m_cs.hwndParent)
			hWndParent = m_cs.hwndParent;

		// Set the window style
		DWORD dwStyle;
		if (m_cs.style)
			dwStyle = m_cs.style;
		else
			dwStyle = WS_VISIBLE | ((hWndParent)? WS_CHILD : WS_OVERLAPPEDWINDOW);

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

	HWND CWnd::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, RECT rSize, HWND hParent, HMENU hMenu, LPVOID lpParam /*= NULL*/)
	{
		int x = rSize.left;
		int y = rSize.top;
		int nWidth = rSize.right - rSize.left;
		int nHeight = rSize.bottom - rSize.top;
		return CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, x, y, nWidth, nHeight, hParent, hMenu, lpParam);
	}

	HWND CWnd::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hParent, HMENU hMenu, LPVOID lpParam /*= NULL*/)
	{
		try
		{
		// Test if Win32++ has been started
		if (GetApp() == 0)
			throw CWinException(_T("Win32++ has not been initialised properly.\n Start the Win32++ by inheriting from CWinApp."));

		// Only one window per CWnd instance
		if (::IsWindow(m_hWnd))
			throw CWinException(_T("CWnd::CreateEx ... Window already exists"));

		// Ensure a window class is registered
		TCHAR ClassName[MAX_STRING_SIZE] = _T("");
		if (lstrlen(lpszClassName) == 0)
			::lstrcpyn (ClassName, _T("Win32++ Window"), MAX_STRING_SIZE);
		else
			// Create our own local copy of szClassName.
			::lstrcpyn(ClassName, lpszClassName, MAX_STRING_SIZE);

		// Register the window class
		WNDCLASSEX wcx = {0};
		wcx.cbSize = sizeof(WNDCLASSEX);
		wcx.lpszClassName = ClassName;
		wcx.hbrBackground = m_hBrushBkgnd;
		if (!RegisterClassEx(wcx))
			throw CWinException(_T("CWnd::CreateEx  Failed to register window class"));

		// Ensure this thread has the TLS index set
		GetApp()->m_MapLock.Lock();
		m_pTLSData = (TLSData*)::TlsGetValue(GetApp()->GetTlsIndex());

		if (m_pTLSData == NULL)
			m_pTLSData = GetApp()->SetTlsIndex();
		GetApp()->m_MapLock.Release();

		// Create and store the CBT hook
		SetHook();

		// Create window
		m_hWnd = ::CreateWindowEx(dwExStyle, ClassName, lpszWindowName, dwStyle, x, y, nWidth, nHeight,
			                      hParent, hMenu, GetApp()->GetInstanceHandle(), lpParam);

		// Tidy up
		RemoveHook();

		// Now handle window creation failure
		if (!m_hWnd)
			throw CWinException(_T("CWnd::CreateEx ... Failed to Create Window"));

		if (!::GetClassInfoEx(GetApp()->GetInstanceHandle(), ClassName, &wcx))
			if (::GetClassInfoEx(NULL, ClassName, &wcx))
				throw CWinException(_T("CWnd::CreateEx  Failed to get class info"));

		m_hWndParent = hParent;

		// Automatically subclass predefined window class types
		if (wcx.lpfnWndProc != CWnd::StaticWindowProc)
		{
			Subclass();
			OnCreate(); // We missed the WM_CREATE message, so call OnCreate now
		}

		// Window creation is complete. Now call OnInitialUpdate
		OnInitialUpdate();
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
			throw;
		}

		return m_hWnd;

	} // void CWnd::CreateEx()

	LRESULT CWnd::DefWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	// Pass messages on to the appropriate default window procedure
	// CMDIChild and CMDIFrame override this function
	{
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	void CWnd::DestroyWindow()
	{
		if (m_hIconLarge != NULL)
			::DestroyIcon(m_hIconLarge);
		m_hIconLarge = NULL;

		if (m_hIconSmall != NULL)
			::DestroyIcon(m_hIconSmall);
		m_hIconSmall = NULL;

		if (m_hBrushBkgnd != NULL)
			::DeleteObject(m_hBrushBkgnd);
		m_hBrushBkgnd = NULL;

		if (::IsWindow(m_hWnd))
			::DestroyWindow(m_hWnd);

		m_hWnd = NULL;
		m_hWndParent = NULL;
		m_PrevWindowProc = NULL;
	}

	HWND CWnd::Detach()
	{
		try
		{
			//Only a subclassed window can be detached
			if (m_PrevWindowProc == 0)
				throw CWinException(_T("CWnd::Detach  Unable to detach this window"));

#if defined (_MSC_VER) && _MSC_VER <= 1200

			// use non 64 bit compliant code for Visual C++ 6 and below
			::SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)m_PrevWindowProc);

#else
  #if defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable: 4244 4312) //Temporarily disable these warnings
  #endif //defined(_MSC_VER)

			// use 64 bit compliant code otherwise
			::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)m_PrevWindowProc);

  #if defined(_MSC_VER)
    #pragma warning(pop)    // Re-enable 4244 + 4312 warnings
  #endif //defined(_MSC_VER)
#endif // defined (_MSC_VER) && _MSC_VER <= 1200

			// Remove the map entry
			std::map<HWND, CWnd*, CompareHWND>::iterator m;
			GetApp()->m_MapLock.Lock();
			m = GetApp()->GetHWNDMap().find(m_hWnd);
			if (m != GetApp()->GetHWNDMap().end())
				GetApp()->GetHWNDMap().erase(m);
			else
			{
				GetApp()->m_MapLock.Release();
				throw CWinException(_T("CWnd::Detach  Unable to find window to detach"));
			}
			GetApp()->m_MapLock.Release();

			// Clear member variables
			HWND hWnd = m_hWnd;
			m_hWnd = NULL;
			m_hWndParent = NULL;
			m_PrevWindowProc = NULL;
			return hWnd;
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
			throw;
		}

	}

	HWND CWnd::GetAncestor(HWND hWnd) const
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

	CWnd* CWnd::GetCWndObject(HWND hWnd) const
	{
		// Returns the CWnd object associated with the window handle

		std::map<HWND, CWnd*, CompareHWND>::iterator m;

		GetApp()->m_MapLock.Lock();
		m = GetApp()->GetHWNDMap().find(hWnd);
		GetApp()->m_MapLock.Release();
		if (m != GetApp()->GetHWNDMap().end())
			// return the CWnd pointer
			return m->second;

		return NULL;	// No matching CWnd for this HWND
	}

	HBITMAP CWnd::LoadBitmap(LPCTSTR lpBitmapName)
	{
		if (GetApp() == 0)
			throw CWinException(_T("LoadBitmap ... Win32++ has not been initialised successfully."));

		HBITMAP hBitmap;

		// Try to load the bitmap from the resource handle first
		hBitmap = ::LoadBitmap(GetApp()->GetResourceHandle(), lpBitmapName);

		// The bitmap resource might be in the application's resources instead
		if (!hBitmap)
			hBitmap = ::LoadBitmap(GetApp()->GetInstanceHandle(), lpBitmapName);

		// No bitmap found, so display warning message
		if (!hBitmap)
			DebugWarnMsg(_T("Unable to load bitmap"));

		return hBitmap;
	}

	LPCTSTR CWnd::LoadString(UINT nID)
	{
		// Returns the string associated with a Resource ID

		if (GetApp() == 0)
			throw CWinException(_T("LoadString ... Win32++ has not been initialised successfully."));

		::lstrcpy(m_szString, _T(""));
		if (!::LoadString (GetApp()->GetResourceHandle(), nID, m_szString, MAX_STRING_SIZE))
		{
			// The string resource might be in the application's resources instead
			if (::LoadString (GetApp()->GetInstanceHandle(), nID, m_szString, MAX_STRING_SIZE))
				return (LPCTSTR) m_szString;

			TCHAR msg[80] = _T("");
			::wsprintf(msg, _T("LoadString - No string resource for %d"), nID);
			DebugWarnMsg(msg);
		}

		return (LPCTSTR) m_szString;
	}

	BOOL CWnd::OnCommand(WPARAM /*wParam*/, LPARAM /*lParam*/)
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

	void CWnd::OnCreate()
	{
		// This function is called when a WM_CREATE message is recieved
		// Override it in your derived class to automatically perform tasks
		//  during window creation.
	}

	void CWnd::OnInitialUpdate()
	{
		// This function is called automatically once the window is created
		// Override it in your derived class to automatically perform tasks
		// after window creation.
	}

	LRESULT CWnd::OnMessage(HWND hWndParent, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

		CWnd* Wnd = GetCWndObject(hWnd);

		if (Wnd != NULL)
			return Wnd->OnMessageReflect(uMsg, wParam, lParam);

		return 0L;
	}

	LRESULT CWnd::OnMessageReflect(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
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

	LRESULT CWnd::OnNotify(WPARAM /*wParam*/, LPARAM /*lParam*/)
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

	LRESULT CWnd::OnNotifyReflect(WPARAM /*wParam*/, LPARAM /*lParam*/)
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

	void CWnd::OnPaint(HDC)
	{
		// Override this function in your derived class to perform drawing tasks.
	}

	void CWnd::PreCreate(CREATESTRUCT& cs)
	// Called by CWnd::Create to set some window parameters
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

	BOOL CWnd::RegisterClassEx(WNDCLASSEX& wcx)
	{
		try
		{
			if ((lstrlen(wcx.lpszClassName) == 0) || (lstrlen(wcx.lpszClassName) >  MAX_STRING_SIZE))
				throw CWinException(_T("CWnd::RegisterClassEx   Invalid class name"));

			// Check to see if this classname is already registered
			WNDCLASSEX wcxTest = {0};
			wcxTest.cbSize = sizeof(WNDCLASSEX);

			if (::GetClassInfoEx(GetApp()->GetInstanceHandle(), wcx.lpszClassName, &wcxTest))
				return TRUE;

			// Set reasonable defaults
			wcx.cbSize		= sizeof(WNDCLASSEX);
			wcx.hInstance	= GetApp()->GetInstanceHandle();
			wcx.lpfnWndProc	= CWnd::StaticWindowProc;

			if (wcx.hbrBackground == 0)	wcx.hbrBackground	= m_hBrushBkgnd? m_hBrushBkgnd : (HBRUSH)::GetStockObject(WHITE_BRUSH);
			if (wcx.hCursor == 0)		wcx.hCursor			= ::LoadCursor(NULL, IDC_ARROW);
			if (wcx.hIcon == 0) 		wcx.hIcon			= ::LoadIcon(NULL, IDI_APPLICATION);
			if (wcx.hIconSm == 0)		wcx.hIconSm			= ::LoadIcon(NULL, IDI_APPLICATION);

			// Register the WNDCLASSEX structure
			if (!::RegisterClassEx(&wcx))
				throw CWinException(_T("Failed to register Window Class"));

			return TRUE;
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
			throw;
		}

	}

	void CWnd::RemoveHook()
	{
		if (m_pTLSData->hCBTHook)
		{
			::UnhookWindowsHookEx(m_pTLSData->hCBTHook);
			m_pTLSData->hCBTHook = NULL;
		}
	}

	void CWnd::SetBkgndColor(COLORREF color)
	{
		// Note:  This sets the background color for all windows with this class name,
		//         not just this window.

		if (m_hBrushBkgnd) ::DeleteObject(m_hBrushBkgnd);
		m_hBrushBkgnd = CreateSolidBrush(color);

		if (m_hWnd)
		{
#if defined (_MSC_VER) && _MSC_VER > 1200
  #pragma warning(push)
  #pragma warning(disable: 4244 ) //Temporarily disable C4244 warning
			// use 64 bit compliant code
			::SetClassLongPtr(m_hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)m_hBrushBkgnd);
  #pragma warning(pop)    // Re-enable 4244 warnings
#else
			// use non 64 bit compliant code
			::SetClassLong(m_hWnd, GCL_HBRBACKGROUND, (LONG)m_hBrushBkgnd);
#endif
		}
	}

	void CWnd::SetHook()
	{
		// Create the CBT Hook
		HHOOK hHook = ::SetWindowsHookEx(WH_CBT, (HOOKPROC)CWnd::StaticCBTProc, 0, ::GetCurrentThreadId());
		if (!hHook)
			throw CWinException(_T("CWnd::SetHook ... SetWindowsHookEx Failed"));

		// Store the hook and 'this' pointer in Thread Local Storage
		m_pTLSData->hCBTHook = hHook;
		m_pTLSData->pCWnd = this;
	}

	HICON CWnd::SetIconLarge(int nIcon)
	{
		m_hIconLarge = (HICON) (::LoadImage (GetApp()->GetResourceHandle(), MAKEINTRESOURCE (nIcon), IMAGE_ICON,
		::GetSystemMetrics (SM_CXICON), ::GetSystemMetrics (SM_CYICON), 0));

		::SendMessage (m_hWnd, WM_SETICON, WPARAM (ICON_BIG), LPARAM (m_hIconLarge));
		return m_hIconLarge;
	}

	HICON CWnd::SetIconSmall(int nIcon)
	{
		m_hIconSmall = (HICON) (::LoadImage (GetApp()->GetResourceHandle(), MAKEINTRESOURCE (nIcon), IMAGE_ICON,
		::GetSystemMetrics (SM_CXSMICON), ::GetSystemMetrics (SM_CYSMICON), 0));

		::SendMessage (m_hWnd, WM_SETICON, WPARAM (ICON_SMALL), LPARAM (m_hIconSmall));
		return m_hIconSmall;
	}

	void CWnd::SetParent(HWND hParent)
	{
		try
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

		catch (const CWinException &e)
		{
			e.MessageBox();
			// Not a critical problem, so no need to rethrow
		}
	}

	LRESULT CALLBACK CWnd::StaticCBTProc(int msg, WPARAM wParam, LPARAM lParam)
	// With a CBTHook in place, the application receives additional messages.
	// The HCBT_CREATEWND message is the first message to be generated when
	// a window is about to be created.
	{
		try
		{
			// Retrieve pointer to CWnd object from Thread Local Storage TLS
			TLSData* pTLSData = (TLSData*)TlsGetValue(GetApp()->GetTlsIndex());
			if (pTLSData == NULL)
				throw CWinException(_T("CWnd::StaticCBTProc ... Unable to get TLS"));

			CWnd* w = pTLSData->pCWnd;

			// Only proceed for HCBT_CREATEWND messages
			if (msg == HCBT_CREATEWND)
			{
				// Now remove the CBT hook
				::UnhookWindowsHookEx(pTLSData->hCBTHook);
				pTLSData->hCBTHook = NULL;

				// Store the Window pointer into the HWND map
				GetApp()->m_MapLock.Lock();
				GetApp()->GetHWNDMap().insert(std::make_pair((HWND) wParam, w));
				GetApp()->m_MapLock.Release();

				// Store the HWND in the CWnd object early
				w->m_hWnd = (HWND) wParam;

				return 0L;
			}
			return ::CallNextHookEx(pTLSData->hCBTHook, msg, wParam, lParam);
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
			return 0L;
		}

	}

	LRESULT CALLBACK CWnd::StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		std::map<HWND, CWnd*, CompareHWND>::iterator m;
		try
		{
			// Allocate an iterator for our HWND map
			std::map<HWND, CWnd*, CompareHWND>::iterator m;

			GetApp()->m_MapLock.Lock();
			m = GetApp()->GetHWNDMap().find(hWnd);
			GetApp()->m_MapLock.Release();
			if (m != GetApp()->GetHWNDMap().end())
			{
				return m->second->WndProc(hWnd, uMsg, wParam, lParam);
			}

			// Every message should get routed, we should never get here
			throw CWinException(_T("CWnd::StaticWindowProc .. Failed to route message"));
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

	void CWnd::Subclass()
	// A private function used by CreateEx, Attach and AttachDlgItem
	{
		try
		{
			if (m_PrevWindowProc)
				throw CWinException(_T("Subclass failed.  Already Subclassed"));

			// Subclass the window to pass messages to WndProc

#if defined (_MSC_VER) && _MSC_VER <= 1200

			// use non 64 bit compliant code for Visual C++ 6 and below
			WNDPROC WndProc = (WNDPROC)::GetWindowLong(m_hWnd, GWL_WNDPROC);
			if (WndProc == CWnd::StaticWindowProc)
				throw CWinException(_T("Subclass failed.  Already sending messages to StaticWindowProc"));
			m_PrevWindowProc = (WNDPROC)::SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)CWnd::StaticWindowProc);
#else

  #if defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable: 4244 4312) //Temporarily disable these warnings
  #endif //defined(_MSC_VER)

			// use 64 bit compliant code otherwise
			WNDPROC WndProc = (WNDPROC)::GetWindowLongPtr(m_hWnd, GWLP_WNDPROC);
			if (WndProc == CWnd::StaticWindowProc)
				throw CWinException(_T("Subclass failed.  Already sending messages to StaticWindowProc"));
			m_PrevWindowProc = (WNDPROC)::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)CWnd::StaticWindowProc);

  #if defined(_MSC_VER)
    #pragma warning(pop)    // Re-enable 4244 + 4312 warnings
  #endif //defined(_MSC_VER)
#endif // defined (_MSC_VER) && _MSC_VER <= 1200

		}

		catch (const CWinException &e)
		{
			e.MessageBox();
			throw;
		}

	}

	LRESULT CWnd::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// Override this function in your class derrived from CWnd if you wish to handle messages
		// A typical function might look like this:

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

	LRESULT CWnd::WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
				CWnd* Wnd = GetCWndObject((HWND)lParam);
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
				CWnd* WndFrom = GetCWndObject(((LPNMHDR)lParam)->hwndFrom);
				if (WndFrom != NULL)
				{
					lr = WndFrom->OnNotifyReflect(wParam, lParam);
					if (lr) return lr;
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

	////////////////////////////////////////
	// Definitions for the CWinException class

	void CWinException::MessageBox() const
	{
#ifdef _DEBUG

		TCHAR buf1 [MAX_STRING_SIZE/2 -10] = _T("");
		TCHAR buf2 [MAX_STRING_SIZE/2 -10] = _T("");
		TCHAR buf3 [MAX_STRING_SIZE]       = _T("");

		::lstrcpyn(buf1, GetMessage(), MAX_STRING_SIZE/2 -10);

		// Display Last Error information if it's useful
		if (m_err != 0)
		{
			LPVOID lpMsgBuf;
			::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL, m_err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf, 0, NULL );

			::lstrcpyn(buf2, (LPTSTR)lpMsgBuf, MAX_STRING_SIZE/2 -10);

			::wsprintf(buf3, _T("%s\n\n     %s\n\n"), buf1, buf2);
			::LocalFree(lpMsgBuf);
		}
		else
			::wsprintf(buf3, _T("%s"), buf1);

		DebugErrMsg(buf3);

#endif  // _DEBUG
	}

	// Displays a warning message in a message box. Debug mode only.


} // namespace Win32xx


