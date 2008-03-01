// Win32++  Version 6.0
// Released: 4th March, 2008 by:
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

	// Store the pointer used for our CWnd::StaticWindowProc function
	WNDPROC st_pfnWndProc = 0;

	///////////////////////////////////
	// Definitions for the CWinApp class
	//

	// To begin Win32++, inherit your application class from this one.
	// You should run only one instance of the class inherited from this.
	CWinApp::CWinApp() : m_hAccelTable(NULL), m_hWndAccel(NULL), m_IsTlsAllocatedHere(FALSE), m_pFrame(NULL)
	{
		try
		{
			// Test if this is the first instance of CWinApp
			if (0 == GetApp() )
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

			m_hInstance = (HINSTANCE) ::GetModuleHandle(0);
			m_hResource = m_hInstance;
		}

		catch (const CWinException &e)
		{
			// Indicate the problem
			e.MessageBox();
			if (NULL == st_pTheApp)
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

		if (m_hAccelTable)
			::DestroyAcceleratorTable(m_hAccelTable);
	}

	void CWinApp::AddToMap(HWND hWnd, CWnd* w)
	{
		// Store the Window pointer into the HWND map
		m_MapLock.Lock();
		m_HWNDmap.insert(std::make_pair(hWnd, w));
		m_MapLock.Release();
	}

	CWnd* CWinApp::GetCWndFromMap(HWND hWnd)
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

	BOOL CWinApp::RemoveFromMap(CWnd* w)
	{
		// Allocate an iterator for our HWND map
		std::map<HWND, CWnd*, CompareHWND>::iterator m;

		// Erase the CWnd pointer entry from the map
		m_MapLock.Lock();
		for (m = m_HWNDmap.begin(); m != m_HWNDmap.end(); m++)
		{
			if (w == m->second)
			{
				m_HWNDmap.erase(m);
				m_MapLock.Release();
				return TRUE;
			}
		}

		m_MapLock.Release();
		return FALSE;
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
			if (-1 == status) return -1;

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

		m_hAccelTable = ::LoadAccelerators(m_hResource, MAKEINTRESOURCE(ID_ACCEL));
		if (!m_hAccelTable)
			TRACE(_T("Load Accelerators failed\n"));
	}

	void CWinApp::SetFrame(CFrame* pFrame)
	{
		// Store the pointer to the (first) frame
		if (0 == m_pFrame)
			m_pFrame = pFrame;
	}

	TLSData* CWinApp::SetTlsIndex()
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
	CWnd::CWnd() : m_hWnd(NULL), m_hWndParent(NULL), m_hIconLarge(NULL),
					m_hIconSmall(NULL), m_PrevWindowProc(NULL)
	{
		// Note: m_hWnd and m_hWndParent are set in CWnd::CreateEx(...)
		::ZeroMemory(&m_cs, sizeof(CREATESTRUCT));
		::ZeroMemory(&m_wc, sizeof(WNDCLASS));
	}

	CWnd::~CWnd()
	{
		// Destroy the window for this object
		if (::IsWindow(m_hWnd)) DestroyWindow();

		// Destroy the GDI objects
		if (m_hIconLarge) ::DestroyIcon(m_hIconLarge);
		if (m_hIconSmall) ::DestroyIcon(m_hIconSmall);

		// Remove the map entries
		if (GetApp())
		{
			GetApp()->RemoveFromMap(this);
		}
	}

	BOOL CWnd::Attach(HWND hWnd)
	// Subclasses an existing window and attaches it to a CWnd
	{
		try
		{
			if (IsWindow(hWnd))
			{
				if (0 != GetApp()->GetCWndFromMap(hWnd))
					throw CWinException(_T("Window already attached to this CWnd object"));

				m_hWnd = hWnd;
				Subclass();

				if (m_PrevWindowProc)
				{
					// Store the CWnd pointer in the HWND map
					GetApp()->AddToMap(hWnd, this);

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
			if (wc.lpfnWndProc != st_pfnWndProc)
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

		if (::IsWindow(m_hWnd))
			::DestroyWindow(m_hWnd);

		m_hWnd = NULL;
		m_hWndParent = NULL;
		m_PrevWindowProc = NULL;
	}

	HWND CWnd::Detach()
	{
		//Only a subclassed window can be detached
		if (0 == m_PrevWindowProc)
			throw CWinException(_T("CWnd::Detach  Unable to detach this window"));

#if defined GWLP_WNDPROC

  #if defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable: 4244 4312) //Temporarily disable these warnings
  #endif //defined(_MSC_VER)

		// use 64 bit compliant code otherwise
		::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)m_PrevWindowProc);

  #if defined(_MSC_VER)
    #pragma warning(pop)    // Re-enable 4244 + 4312 warnings
  #endif //defined(_MSC_VER)

#else
		// use non 64 bit compliant code
		::SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)m_PrevWindowProc);

#endif // defined GWLP_WNDPROC

		// Remove the map entry
		if (!GetApp()->RemoveFromMap(this))
			throw CWinException(_T("CWnd::Detach  Unable to find window to detach"));

		// Clear member variables
		HWND hWnd = m_hWnd;
		m_hWnd = NULL;
		m_hWndParent = NULL;
		m_PrevWindowProc = NULL;
		return hWnd;
	}

	CWnd* CWnd::FromHandle(HWND hWnd) const
	{
		// Returns the CWnd object associated with the window handle
		return GetApp()->GetCWndFromMap(hWnd);
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

	tString CWnd::GetDlgItemString(int nIDDlgItem)
	{
		// Calculate the buffer size to hold the null terminated text
		int nLength = 1 + ::GetWindowTextLength(GetDlgItem(m_hWnd, nIDDlgItem));

		tString String;
		if (nLength > 0)
		{
			TCHAR* szString = new TCHAR[nLength];
			if (NULL == szString)
				throw std::bad_alloc();

			::GetDlgItemText(m_hWnd, nIDDlgItem, szString, nLength);
			String = szString;

			delete []szString;
		}

		return String;
	}

	tString CWnd::GetWindowString()
	{
		// Gets the window title for an ordinary window, or the text in an edit control

		// Calculate the buffer size to hold the null terminated text
		int nLength = 1 + ::GetWindowTextLength(m_hWnd);

		tString String;
		if (nLength > 0)
		{
			TCHAR* szString = new TCHAR[nLength];
			if (NULL == szString)
				throw std::bad_alloc();

			::GetWindowText(m_hWnd, szString, nLength);
			String = szString;

			delete []szString;
		}
		return String;
	}

	HBITMAP CWnd::LoadBitmap(LPCTSTR lpBitmapName)
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

	LPCTSTR CWnd::LoadString(UINT nID)
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

		CWnd* Wnd = FromHandle(hWnd);

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

	void CWnd::PreRegisterClass(WNDCLASS& wc)
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

	BOOL CWnd::RegisterClass(WNDCLASS& wc)
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

			// Store callback address (its not always simply the function pointer to CWnd::StaticWindowProc)
			CCriticalSection RegLock;
			RegLock.Lock();
			if (0 == st_pfnWndProc)
			{
				GetClassInfo(GetApp()->GetInstanceHandle(), wc.lpszClassName, &wcTest);
				st_pfnWndProc = wcTest.lpfnWndProc;
			}
			RegLock.Release();
			return TRUE;
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		return FALSE;
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
		if (::IsWindow(hParent))
		{
			m_hWndParent = hParent;
			if (::IsWindow(m_hWnd))
				::SetParent(m_hWnd, hParent);
		}
		else
			throw CWinException(_T("CWnd::SetParent ... Failed to set parent"));

	}

	LRESULT CALLBACK CWnd::StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
				GetApp()->AddToMap(hWnd, w);

				// Store the HWND in the CWnd object early
				w->m_hWnd = hWnd;

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

	void CWnd::Subclass()
	// A private function used by CreateEx, Attach and AttachDlgItem
	{
		if (m_PrevWindowProc)
			throw CWinException(_T("Subclass failed.  Already Subclassed"));

		// Subclass the window to pass messages to WndProc

#if defined  GWLP_WNDPROC // defined if Win64 supported

  #if defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable: 4244 4312) //Temporarily disable these warnings
  #endif //defined(_MSC_VER)

		// use 64 bit compliant code
		WNDPROC WndProc = (WNDPROC)::GetWindowLongPtr(m_hWnd, GWLP_WNDPROC);
		if (WndProc == st_pfnWndProc)
			throw CWinException(_T("Subclass failed.  Already sending messages to StaticWindowProc"));
		m_PrevWindowProc = (WNDPROC)::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)CWnd::StaticWindowProc);

  #if defined(_MSC_VER)
    #pragma warning(pop)    // Re-enable 4244 + 4312 warnings
  #endif //defined(_MSC_VER)

#else
		// use non 64 bit compliant code
		WNDPROC WndProc = (WNDPROC)::GetWindowLong(m_hWnd, GWL_WNDPROC);
		if (WndProc == st_pfnWndProc)
			throw CWinException(_T("Subclass failed.  Already sending messages to StaticWindowProc"));
		m_PrevWindowProc = (WNDPROC)::SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)CWnd::StaticWindowProc);

#endif // defined GWLP_WNDPROC
	}

	LRESULT CWnd::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
				CWnd* WndFrom = FromHandle(((LPNMHDR)lParam)->hwndFrom);
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


} // namespace Win32xx


