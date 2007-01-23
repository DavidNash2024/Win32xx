// Win32++  Version 5.0.3 Beta
// Modified: 24th January, 2007 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
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
#include "Frame.h"
#include "Default_Resource.h"


namespace Win32xx
{
	/////////////////////////////////
	// Global functions and variables
	//
	void DebugErrMsg(LPCTSTR ErrorMsg)
	{
	#ifdef _DEBUG
		::MessageBox (0, ErrorMsg, TEXT("Exception"), MB_ICONEXCLAMATION | MB_OK);
	#else
		UNREFERENCED_PARAMETER(ErrorMsg);
	#endif  //_DEBUG
	}

	void DebugWarnMsg(LPCTSTR WarnMsg)
	{
	#ifdef _DEBUG
		::MessageBox (0, WarnMsg, TEXT("Warning"), MB_ICONINFORMATION | MB_OK);
	#else
		UNREFERENCED_PARAMETER(WarnMsg);
	#endif  //_DEBUG
	}

	// Function which returns a pointer to the CWinApp object
	CWinApp* GetApp(){ return CWinApp::GetApp(); }

	// Static variable for Thread Local Storage Index
	DWORD CWinApp::st_dwTlsIndex = TLS_OUT_OF_INDEXES;

	// Static variable for the pointer to the CWinApp object
	CWinApp* CWinApp::st_pTheApp = 0;

	///////////////////////////////////
	// Definitions for the CWinApp class
	//

	// To begin Win32++, inherit your application class from this one.
	// You should run only one instance of the class inherited from this.
	CWinApp::CWinApp(HINSTANCE hInstance) : m_hFont(NULL), m_hInstance(hInstance), m_hRichEdit(NULL),
											m_IsTlsAllocatedHere(FALSE), m_pFrame(NULL), m_pTrace(NULL)
	{
		try
		{
			// Test if this is the first instance of CWinApp
			if (GetApp() == 0)
			{
				st_dwTlsIndex = ::TlsAlloc();
				if (st_dwTlsIndex == TLS_OUT_OF_INDEXES)
					throw CWinException(TEXT("CWinApp::CWinApp  Failed to allocate TLS Index"));

				st_pTheApp = this;
				m_IsTlsAllocatedHere = TRUE; //TLS allocated in this CWinApp object
			}
			else
			{
				// We get here if Win32++ is used incorrectly, as more than one instance
				// of a CWinApp derived class is started.
 				throw CWinException(TEXT("Error!  An instance of CWinApp (or a class derived from CWinApp) is already running"));
			}

			m_hAccelTable = ::LoadAccelerators(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDW_MAIN));
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CWinApp::CWinApp"));
		}
	}

	CWinApp::~CWinApp()
	{
		delete m_pTrace;
		if (m_hRichEdit)
			::FreeLibrary(m_hRichEdit);
		if (m_hFont)
			::DeleteObject(m_hFont);

		if (m_IsTlsAllocatedHere)
		{
			// Allocate an iterator for our HWND map
			std::map<HWND, CWnd*, CompareHWND>::iterator m;

			//Check that all CWnd objects are destroyed
			if (GetApp()->m_HWNDmap.begin() != GetApp()->m_HWNDmap.end())
				DebugErrMsg(TEXT("Warning: Shutting down CWinApp before all CWnd objects destroyed"));

			// Do remaining tidy up
			if (st_dwTlsIndex != TLS_OUT_OF_INDEXES)
			{
				::TlsSetValue(GetTlsIndex(), NULL);
				::TlsFree(st_dwTlsIndex);
				st_dwTlsIndex = TLS_OUT_OF_INDEXES;
				st_pTheApp = 0;
			}
		}

		std::vector<TLSData*>::iterator iter;
		for(iter = m_ThreadData.begin(); iter != m_ThreadData.end(); iter++)
		{
			delete *(iter);
		}
	}

	int CWinApp::MessageLoop()
	{
		// This gets any messages queued for the application, and dispatches them.

		MSG uMsg;
		int status;

		while((status = ::GetMessage(&uMsg, NULL, 0, 0))!= 0)
		{
			if (status == -1) return -1;

			if (GetFrame())
			{
				if (!::TranslateAccelerator(GetFrame()->GetHwnd(), m_hAccelTable, &uMsg))
				{
					::TranslateMessage(&uMsg);
					::DispatchMessage(&uMsg);
				}
			}
			else
			{
				::TranslateMessage(&uMsg);
				::DispatchMessage(&uMsg);
			}

		}
		return LOWORD(uMsg.wParam);
	}

	TLSData* CWinApp::SetTlsIndex()
	{
		try
		{
			// Called once for any thread that has a CWnd object
			if ((TLSData*)::TlsGetValue(GetTlsIndex()) != NULL)
				throw CWinException(TEXT("CWinApp::SetTlsIndex    Error, attempted to set TLS more than once"));

			TLSData* pTLSData = new TLSData;
			ZeroMemory(pTLSData, sizeof(TLSData));
			::TlsSetValue(GetTlsIndex(), pTLSData);

			// Store pointer in vector for deletion in destructor
			m_ThreadData.push_back(pTLSData);
			return pTLSData;
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CWnd construction"));
		}
		return 0;
	}

	void CWinApp::Trace(LPCTSTR szstring)
	{
		// Used by the TRACE macro to output text to the trace window
		// Note: The TRACE macro is only active in debug mode (i.e when _DEBUG is defined)

		m_TraceLock.Lock();

		static HWND TraceEdit;

		//Create the Trace CWnd object and window
		if (m_pTrace == NULL)
		{
			// Get Current keyboard focus
			HWND hPrevFocus = ::GetFocus();
			if (m_pFrame)
				if (hPrevFocus == m_pFrame->GetMenubar().GetHwnd())
					hPrevFocus = m_pFrame->GetHwnd();

			m_hRichEdit = ::LoadLibrary(TEXT("RICHED32.DLL"));
			if (!m_hRichEdit)
				DebugErrMsg(TEXT("Failed to load the RichEdit dll"));

			m_pTrace = new CWnd;

			// Position window at the botton right of the desktop area
			RECT r = {0};
			::SystemParametersInfo(SPI_GETWORKAREA, 0, &r, 0);
			r.top = r.bottom - TRACE_HEIGHT;
			r.left = r.right - TRACE_WIDTH;
			DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_VISIBLE;

			m_pTrace->CreateEx(WS_EX_TOPMOST, TEXT("TRACE"), TEXT("Trace Window"), dwStyle, r, NULL, NULL);

			::GetClientRect(m_pTrace->GetHwnd(), &r);
			dwStyle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL;

			TraceEdit = ::CreateWindowEx(0L, TEXT(/*"Edit"*/"RichEdit"), TEXT(""), dwStyle, r.left, r.top, r.right - r.left, r.bottom - r.top,
						m_pTrace->GetHwnd(), NULL, GetApp()->GetInstanceHandle(), NULL);

			// Set a default font
			m_hFont = ::CreateFont(16, 0, 0, 0, FW_DONTCARE, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_MODERN, TEXT("Courier New"));
			::SendMessage(TraceEdit, WM_SETFONT, (WPARAM)m_hFont, 0);

			// Return previous keyboard focus
			::SetFocus(hPrevFocus);
		}

		// Add CR LF to the end
		int nLength = ::GetWindowTextLength(TraceEdit);
		::SendMessage(TraceEdit, EM_SETSEL, (WPARAM)nLength, (LPARAM)nLength);
		::SendMessage(TraceEdit, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)TEXT("\r\n\0"));

		// Append text to the end of the (rich)edit control
		nLength = ::GetWindowTextLength(TraceEdit);
		::SendMessage(TraceEdit, EM_SETSEL, (WPARAM)nLength, (LPARAM)nLength);
		::SendMessage(TraceEdit, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)szstring);

		// Scroll to bottom
		::SendMessage(TraceEdit, EM_SCROLLCARET, (WPARAM)0, (LPARAM)0);

		m_TraceLock.Release();
	}


	////////////////////////////////////////
	// Definitions for the CWnd class
	//
	CWnd::CWnd() : m_hWnd(NULL), m_hWndParent(NULL), m_hIconLarge(NULL), m_hIconSmall(NULL), m_PrevWindowProc(NULL)
	{
		// Note: m_hWnd and m_hWndParent are set in CWnd::CreateEx(...)
		try
		{
			::ZeroMemory(&m_cs, sizeof(CREATESTRUCT));
			m_szString[0] = TEXT('\0');

			// Test if Win32++ has been started properly
			if (GetApp() == 0)
				throw CWinException(TEXT("Win32++ has not been successfully initialised."));

			GetApp()->m_MapLock.Lock();
			m_pTLSData = (TLSData*)::TlsGetValue(GetApp()->GetTlsIndex());

			// Ensure each thread has the TLS index set
			if (m_pTLSData == NULL)
				m_pTLSData = GetApp()->SetTlsIndex();
			GetApp()->m_MapLock.Release();
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CWnd construction"));
		}
	}

	CWnd::~CWnd()
	{
		// Destroy the window for this object
		if (::IsWindow(m_hWnd)) DestroyWindow();

		// Destroy the icons
		if (m_hIconLarge) ::DestroyIcon(m_hIconLarge);
		if (m_hIconSmall) ::DestroyIcon(m_hIconSmall);

		// Remove the map entries
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

	BOOL CWnd::Attach(HWND hWnd)
	// Subclasses an existing window and attaches it to a CWnd
	{
		try
		{
			if (IsWindow(hWnd))
			{
				if (m_hWnd)
					throw CWinException(TEXT("Window already attached to this CWnd object"));

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
					throw CWinException(TEXT("CWnd::Attach .. Subclass failed"));
				}
			}
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CWnd::Attach"));
		}

		return FALSE;
	}

	BOOL CWnd::AttachDlgItem(UINT nID, CWnd* pParent)
	// Use to convert a dialog item to a CWnd object
	{
		HWND hWnd = ::GetDlgItem(pParent->GetHwnd(), nID);

		return Attach(hWnd);
	}

	LRESULT CWnd::CallPrevWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::CallWindowProc(m_PrevWindowProc, hwnd, uMsg, wParam, lParam);
	}

	HWND CWnd::Create(HWND hWndParent /* = NULL */)
	// Default Window Creation.
	{
		try
		{
			// Set the CREATESTRUCT parameters
			PreCreate(m_cs);

			// Set the Window Class Name
			TCHAR szClassName[MAX_STRING_SIZE + 1] = TEXT("Win32++ Window");
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
			int x  = (m_cs.cx && m_cs.cy)? m_cs.x  : CW_USEDEFAULT;
			int cx = (m_cs.cx && m_cs.cy)? m_cs.cx : CW_USEDEFAULT;
			int y  = (m_cs.cx && m_cs.cy)? m_cs.y  : CW_USEDEFAULT;
			int cy = (m_cs.cx && m_cs.cy)? m_cs.cy : CW_USEDEFAULT;

			// Create the window
			if (!CreateEx(m_cs.dwExStyle, szClassName, m_cs.lpszName, dwStyle, x, y,
				cx, cy, hWndParent, m_cs.hMenu, m_cs.lpCreateParams))
				throw CWinException(TEXT("CWnd::Create ... CreateEx failed"));
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CWnd::Create"));
		}
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
			// Only one window per CWnd instance
			if (::IsWindow(m_hWnd))
				throw CWinException(TEXT("CWnd::CreateEx ... Window already exists"));

			// Ensure a window class is registered
			TCHAR ClassName[MAX_STRING_SIZE];
			if (lstrlen(lpszClassName) == 0)
				::lstrcpyn (ClassName, TEXT("Win32++ Window"), MAX_STRING_SIZE);
			else
				// Create our own local copy of szClassName.
				::lstrcpyn(ClassName, lpszClassName, MAX_STRING_SIZE);

			// Register the window class
			WNDCLASSEX wcx = {0};
			wcx.cbSize = sizeof(WNDCLASSEX);
			wcx.lpszClassName = ClassName;
			RegisterClassEx(wcx);

			// Create and store the CBT hook
			SetHook();

			// Create window
			m_hWndParent = hParent;
			m_hWnd = ::CreateWindowEx(dwExStyle, ClassName, lpszWindowName, dwStyle, x, y, nWidth, nHeight, hParent, hMenu, GetApp()->GetInstanceHandle(), lpParam);

			// Tidy up
			RemoveHook();

			// Now handle window creation failure
			if (!m_hWnd)
				throw CWinException(TEXT("CWnd::CreateEx ... Failed to Create Window"));

			// Window creation is complete. Now call OnInitialUpdate
			OnInitialUpdate();
		}

		catch (const CWinException &e)
		{
			m_hWndParent = NULL;
			e.MessageBox();
		}

		catch (...)
		{
			m_hWndParent = NULL;
			DebugErrMsg(TEXT("Exception in CWnd::CreateEx"));
		}

		return m_hWnd;

	} // void CWnd::CreateEx()

	void CWnd::DestroyWindow()
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
	}

	HWND CWnd::Detach()
	{
		HWND hWnd = m_hWnd;
		m_hWnd = NULL;
		m_hWndParent = NULL;
		m_PrevWindowProc = NULL;

		// Remove the map entry
		std::map<HWND, CWnd*, CompareHWND>::iterator m;

		m = GetApp()->GetHWNDMap().begin();
		while (m != GetApp()->GetHWNDMap().end())
		{
			if (m->second == this)
				GetApp()->GetHWNDMap().erase(m++);
			else
				m++;
		}

		return hWnd;
	}

	HWND CWnd::GetAncestor(HWND hwnd)
	{
		// Returns the root parent.  Supports Win95
		HWND hwndParent = ::GetParent(hwnd);
		while (::IsChild(hwndParent, hwnd))
		{
			hwnd = hwndParent;
			hwndParent = ::GetParent(hwnd);
		}

		return hwnd;
	}

	CWnd* CWnd::GetCWndObject(HWND hWnd)
	{
		// Returns the CWnd object associated with the window handle

		std::map<HWND, CWnd*, CompareHWND>::iterator m;

		GetApp()->m_MapLock.Lock();
		m = GetApp()->GetHWNDMap().find(hWnd);
		GetApp()->m_MapLock.Release();
		if (m != GetApp()->GetHWNDMap().end())
			// return the CWnd pointer
			return m->second;

		return NULL;
	}

	LPCTSTR CWnd::LoadString(int nID)
	{
		// Returns the string associated with a Resource ID
		try
		{
			if (GetApp() == 0)
				throw CWinException(TEXT("Win32++ has not been initialised successfully."));

			::lstrcpy(m_szString, TEXT(""));
			::LoadString (GetApp()->GetInstanceHandle(), nID, m_szString, MAX_STRING_SIZE -1);
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CWnd::LoadString"));
		}

		return (LPCTSTR) m_szString;
	}

	BOOL CWnd::OnCommand(UINT /*nID*/)
	{
		// Override this to handle WM_COMMAND messages, for example

		//	switch (nID)
		//	{
		//	case IDM_FILE_NEW:
		//		OnFileNew();
		//		break;
		//	}

		// return CFrame::OnCommand(nID);

		return 0;
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

	LRESULT CWnd::OnNotify(WPARAM wParam, LPARAM lParam)
	{
		// By default, Win32++ calls OnNotifyReflect to pass the notification
		//  from WM_NOTIFY back to the child that generated it.

		// Override this function to handle notifications in the parent CWnd class.

		HWND hwnd = ((LPNMHDR)lParam)->hwndFrom;
		CWnd* Wnd = GetCWndObject(hwnd);

		if (Wnd != NULL)
			return Wnd->OnNotifyReflect(wParam, lParam);

		return 0L;
	}

	LRESULT CWnd::OnNotifyReflect(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		// Override this function to process the WM_NOTIFY notifications reflected back
		// originating CWnd object

		return 0L;
	}

	void CWnd::OnPaint(HDC)
	{
		// Override this function in your derived class to perform
		// drawing tasks.
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
				throw CWinException(TEXT("Wnd::RegisterClassEx   Invalid class name"));

			// Check to see if this classname is already registered
			WNDCLASSEX wcxTest = {0};
			wcxTest.cbSize = sizeof(WNDCLASSEX);
			if (::GetClassInfoEx(GetApp()->GetInstanceHandle(), wcx.lpszClassName, &wcxTest))
				return TRUE;

			// Set reasonable defaults
			wcx.cbSize		= sizeof(WNDCLASSEX);
			wcx.hInstance	= GetApp()->GetInstanceHandle();
			wcx.lpfnWndProc	= CWnd::StaticWindowProc;

			if (wcx.hbrBackground == 0)	wcx.hbrBackground	= (HBRUSH)::GetStockObject(WHITE_BRUSH);
			if (wcx.hCursor == 0)		wcx.hCursor			= ::LoadCursor(NULL, IDC_ARROW);
			if (wcx.hIcon == 0) 		wcx.hIcon			= ::LoadIcon(NULL, IDI_APPLICATION);
			if (wcx.hIconSm == 0)		wcx.hIconSm			= ::LoadIcon(NULL, IDI_APPLICATION);

			// Register the WNDCLASSEX structure
			if (!::RegisterClassEx(&wcx))
				throw CWinException(TEXT("Failed to register Window Class"));

			return TRUE;
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CWnd::RegisterClassEx"));
		}

		return FALSE;
	}

	void CWnd::RemoveHook()
	{
		if (m_pTLSData->hCBTHook)
		{
			::UnhookWindowsHookEx(m_pTLSData->hCBTHook);
			m_pTLSData->hCBTHook = NULL;
		}
	}

	void CWnd::SetHook()
	{
		// Create the CBT Hook
		HHOOK hHook = ::SetWindowsHookEx(WH_CBT, (HOOKPROC)CWnd::StaticCBTProc, 0, ::GetCurrentThreadId());
		if (!hHook)
			throw CWinException(TEXT("CWnd::SetHook ... SetWindowsHookEx Failed"));

		// Store the hook and 'this' pointer in Thread Local Storage
		m_pTLSData->hCBTHook = hHook;
		m_pTLSData->pCWnd = this;
	}

	HICON CWnd::SetIconLarge(int nIcon)
	{
		m_hIconLarge = (HICON) (::LoadImage (GetApp()->GetInstanceHandle(), MAKEINTRESOURCE (nIcon), IMAGE_ICON,
		::GetSystemMetrics (SM_CXICON), ::GetSystemMetrics (SM_CYICON), 0));

		::SendMessage (m_hWnd, WM_SETICON, WPARAM (ICON_BIG), LPARAM (m_hIconLarge));
		return m_hIconLarge;
	}

	HICON CWnd::SetIconSmall(int nIcon)
	{
		m_hIconSmall = (HICON) (::LoadImage (GetApp()->GetInstanceHandle(), MAKEINTRESOURCE (nIcon), IMAGE_ICON,
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
				throw CWinException(TEXT("CWnd::SetParent ... Failed to set parent"));
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CWnd::SetParent"));
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
				throw (CWinException(TEXT("CWnd::StaticCBTProc ... Unable to get TLS")));

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
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CWnd::StaticCBTProc"));
		}

		return 0L;
	}

	LRESULT CALLBACK CWnd::StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		try
		{
			{
			// Allocate an iterator for our HWND map
			std::map<HWND, CWnd*, CompareHWND>::iterator m;

			GetApp()->m_MapLock.Lock();
			m = GetApp()->GetHWNDMap().find(hWnd);
			GetApp()->m_MapLock.Release();
			if (m != GetApp()->GetHWNDMap().end())
				return m->second->WndProc(hWnd, uMsg, wParam, lParam);

			}
			throw CWinException(TEXT("CWnd::StaticWindowProc .. Failed to route message"));
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CWnd::StaticWindowProc"));
		}

		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);;

	} // LRESULT CALLBACK StaticWindowProc(...)

	void CWnd::Subclass()
	{
		// Subclass the window to pass messages to WndProc
	#if defined (_MSC_VER) && _MSC_VER <= 1200
		// use non 64 bit compliant code for Visual C++ 6 and below
		m_PrevWindowProc = (WNDPROC)::SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)CWnd::StaticWindowProc);
	#else
		// use 64 bit compliant code otherwise
		m_PrevWindowProc = (WNDPROC)::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)CWnd::StaticWindowProc);
	#endif
	}

	/*WNDPROC*/ void CWnd::Superclass(LPCTSTR OldClass, LPCTSTR NewClass)
	// Superclassing occurs before window creation. It allows
	// common controls to pass messages via CWnd::WndProc.
	// Superclassing also allows additional messages to be processed
	{
		// Step 1:  Extract the old class's window procedure
		WNDCLASSEX wcx = {0};
		wcx.cbSize = sizeof(WNDCLASSEX);
		::GetClassInfoEx(NULL, OldClass, &wcx);
		m_PrevWindowProc = wcx.lpfnWndProc;

		// Step 2: Register the new window class
		wcx.hInstance = GetApp()->GetInstanceHandle();
		wcx.lpszClassName = NewClass;
		wcx.lpfnWndProc = CWnd::StaticWindowProc;
		RegisterClassEx(wcx);
	}

	LRESULT CWnd::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	// The window procedure for handling messages
	// When you override this in your derived class to handle other messages,
	//  you will probably want to call this base class as well.
	{
    	switch (uMsg)
		{
		case WM_COMMAND:
			OnCommand(LOWORD(wParam));
			break;  // Some commands require default processing
		case WM_CREATE:
			OnCreate();
			break;
	// An example of how to end the application when the window closes
	//  If needed, put this in the class you inherit from CWnd
	//	case WM_DESTROY:
	//		::PostQuitMessage(0);
	//		return 0L;
		case WM_NOTIFY:
			if (m_PrevWindowProc) break;
			return OnNotify(wParam, lParam);
		case WM_PAINT:
			{
				// Just do default painting if subclassed
				if (!m_PrevWindowProc)
				{
					::PAINTSTRUCT ps;
					HDC hDC = ::BeginPaint(hwnd, &ps);
					OnPaint(hDC);
					::EndPaint(hwnd, &ps);
					return 0L;
				}
			}
			break;
		}

		// Now hand all messages to the default procedure
		if (m_PrevWindowProc)
			return ::CallWindowProc(m_PrevWindowProc, hwnd, uMsg, wParam, lParam);
		else
			return ::DefWindowProc(hwnd, uMsg, wParam, lParam);

	} // LRESULT CWnd::WindowProc(...)

	////////////////////////////////////////
	// Definitions for the CWinException class

	void CWinException::MessageBox() const
	{
#ifdef _DEBUG

		TCHAR buf1 [MAX_STRING_SIZE/2 -10];
		TCHAR buf2 [MAX_STRING_SIZE/2 -10];
		TCHAR buf3 [MAX_STRING_SIZE];

		::lstrcpyn(buf1, GetMessage(), MAX_STRING_SIZE/2 -10);

		// Display Last Error information if it's useful
		if (m_err != 0)
		{
			LPVOID lpMsgBuf;
			::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL, m_err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf, 0, NULL );

			::lstrcpyn(buf2, (LPTSTR)lpMsgBuf, MAX_STRING_SIZE/2 -10);

			::wsprintf(buf3, TEXT("%s\n\n     %s\n\n"), buf1, buf2);
			::LocalFree(lpMsgBuf);
		}
		else
			::wsprintf(buf3, TEXT("%s"), buf1);

		DebugErrMsg(buf3);

#endif  // _DEBUG
	}

} // namespace Win32xx

