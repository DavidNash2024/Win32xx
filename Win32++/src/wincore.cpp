#include "wincore.h"


namespace Win32xx
{

	//////////////////////////////////////////////////
	// Global functions	(within the Win32xx namespace)

	// Returns a pointer to the CWinApp derrived class
	CWinApp* GetApp()
	{
		return CWinApp::SetnGetThis();
	}

	// Displays an error message in a message box. Debug mode only.
	void DebugWarnMsg(LPCTSTR WarnMsg)
	{
	#ifdef _DEBUG
		TRACE(_T("*** Warning:  "));
		TRACE(WarnMsg);
		TRACE(_T("\n"));
		::MessageBox (0, WarnMsg, _T("Warning"), MB_ICONINFORMATION | MB_OK);
	#else
		UNREFERENCED_PARAMETER(WarnMsg); // no-op
	#endif  //_DEBUG
	}

	// Displays a warning message in a messagebox. Debug mode only
	void DebugErrMsg(LPCTSTR ErrorMsg)
	{
	#ifdef _DEBUG
		TRACE(_T("*** Error:  "));
		TRACE(ErrorMsg);
		TRACE(_T("\n"));
		::MessageBox (0, ErrorMsg, _T("Error"), MB_ICONEXCLAMATION | MB_OK);
	#else
		UNREFERENCED_PARAMETER(ErrorMsg); // no-op
	#endif  //_DEBUG
	}

	// TRACE sends a string to the debug/output pane, or an external debugger
	void TRACE(LPCTSTR str)
	{
	#ifdef _DEBUG
		OutputDebugString(str);
	#else
		UNREFERENCED_PARAMETER(str); // no-op
	#endif
	}

  #ifndef _WIN32_WCE		// for Win32/64 operating systems

	int GetWinVersion()
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

		return nVersion;
	}

	int GetComCtlVersion()
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

	UINT GetSizeofNonClientMetrics()
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
	BOOL IsLeftButtonDown()
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

	BOOL IsXPThemed()
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
	LPTSTR lstrcpyn(LPTSTR lpstrDest, LPCTSTR lpstrSrc, int nLength)
	{
		if(NULL == lpstrDest || NULL == lpstrSrc || nLength <= 0)
			return NULL;
		int nLen = MIN((int)lstrlen(lpstrSrc), nLength - 1);
		LPTSTR lpstrRet = (LPTSTR)memcpy(lpstrDest, lpstrSrc, nLen * sizeof(TCHAR));
		lpstrDest[nLen] = _T('\0');
		return lpstrRet;
	}
  #endif // !lstrcpyn

	tString CharToTString(const char* s)
	{
		// Handy for converting char to TCHAR
		tString tstr;
  #ifdef UNICODE
		size_t len = strlen(s);
		if (0 == len) return tstr;

		TCHAR* t = new TCHAR[len +1];
		if (NULL == t) throw std::bad_alloc();

		mbstowcs(t, s, len);
		tstr = t;
		delete []t;
  #else
		tstr = s;
  #endif
		return tstr;
	}

	std::string TCharToString(LPCTSTR t)
	{
		// Handy for converting TCHAR to char
		// If the conversion fails, an empty string is returned.
		std::string str;
  #ifdef UNICODE
		// calculate the size of the char string required
		// Note: If wcstombs encounters a wide character it cannot convert
		//       to a multibyte character, it returns �1.
		size_t len = wcstombs(0, t, 0);
		if (len <= 0) return str;

		char* c = new char[len + 1];
		if (NULL == c) throw std::bad_alloc();
		c[0] = '\0';

		wcstombs(c, t, len);
		c[len] = '\0';
		str = c;
		delete []c;
  #else
		str = t;
  #endif
		return str;
	}

	CPoint GetCursorPos()
	{
		CPoint pt;
		::GetCursorPos(&pt);
		return pt;
	}



	///////////////////////////////////
	// Definitions for the CWinException class
	//
	void CWinException::MessageBox() const
	{
		TCHAR buf1 [MAX_STRING_SIZE/2 -10] = _T("");
		TCHAR buf2 [MAX_STRING_SIZE/2 -10] = _T("");
		TCHAR buf3 [MAX_STRING_SIZE]       = _T("");

		lstrcpyn(buf1, m_msg, MAX_STRING_SIZE/2 -10);

		// Display Last Error information if it's useful
		if (m_err != 0)
		{
			DWORD dwFlags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
			::FormatMessage(dwFlags, NULL, m_err,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf2, MAX_STRING_SIZE/2 -10, NULL);

			::wsprintf(buf3, _T("%s\n\n     %s\n\n"), buf1, buf2);
		}
		else
			::wsprintf(buf3, _T("%s"), buf1);

		TRACE(_T("*** ERROR: An Exception occured ***\n"));
		TRACE(buf3);
		TRACE(_T("\n\n"));

		DebugErrMsg(buf3);
	}


	///////////////////////////////////
	// Definitions for the CWinApp class
	//

	// To begin Win32++, inherit your application class from this one.
	// You should run only one instance of the class inherited from this.
	CWinApp::CWinApp() : m_Callback(NULL)
	{
		try
		{
			// Test if this is the first instance of CWinApp
			if (0 == SetnGetThis() )
			{
				m_dwTlsIndex = ::TlsAlloc();
				if (m_dwTlsIndex == TLS_OUT_OF_INDEXES)
				{
					// We only get here in the unlikely event that all TLS indexes are already allocated by this app
					// At least 64 TLS indexes per process are allowed. Win32++ requires only one TLS index.
					throw CWinException(_T("CWinApp::CWinApp  Failed to allocate TLS Index"));
				}
				SetnGetThis(this);
			}
			else
			{
				// We get here if Win32++ is used incorrectly, i.e. more than one instance
				// of a CWinApp derived class is started.
 				throw CWinException(_T("Error!  An instance of CWinApp (or a class derived from CWinApp) is already running"));
			}

		    // Get store the instance handle
			m_hInstance = GetModuleHandle(0);

			m_hResource = m_hInstance;
			DefaultClass();
		}

		catch (const CWinException &e)
		{
			// Indicate the problem
			e.MessageBox();
			throw;
		}
	}

	CWinApp::~CWinApp()
	{
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

		std::vector<TLSData*>::iterator iter;
		for(iter = m_vTLSData.begin(); iter != m_vTLSData.end(); ++iter)
		{
			delete *(iter);
		}

		SetnGetThis((CWinApp*)-1);
	}

	void CWinApp::DefaultClass()
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

		if (!::GetClassInfo(GetInstanceHandle(), szClassName, &wcDefault))
			if (0 == ::RegisterClass(&wcDefault))
				throw CWinException(_T("CWinApp::DefaultClass ... Failed to set Default class"));

		// Retrieve the class information
		ZeroMemory(&wcDefault, sizeof(wcDefault));
		if (!::GetClassInfo(GetInstanceHandle(), szClassName, &wcDefault))
			throw CWinException(_T("CWinApp::DefaultClass ... Failed to get Default class info"));

		// Save the callback address of CWnd::StaticWindowProc
		m_Callback = wcDefault.lpfnWndProc;
	}

	CWnd* CWinApp::GetCWndFromMap(HWND hWnd)
	{
		// Allocate an iterator for our HWND map
		std::map<HWND, CWnd*, CompareHWND>::iterator m;

		// Find the CWnd pointer mapped to this HWND
		m_csMapLock.Lock();
		m = m_mapHWND.find(hWnd);
		m_csMapLock.Release();
		if (m != m_mapHWND.end())
			return m->second;
		else
			return 0;
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

	int CWinApp::Run()
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
			throw;	// Rethrow bad alloc exception
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
			throw;	// Rethrow exception
		}

		catch (...)
		{
			::MessageBox(NULL, _T("Unknown Exception"), _T("Error"), MB_OK);
			throw;	// Rethrow unknown exception
		}
	}

	CWinApp* CWinApp::SetnGetThis(CWinApp* pThis /*= 0*/)
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
				m_csTlsData.Lock();
				m_vTLSData.push_back(pTLSData);
				m_csTlsData.Release();
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
	CWnd::CWnd() : m_hWnd(NULL), m_hIconLarge(NULL),
					m_hIconSmall(NULL), m_PrevWindowProc(NULL)
	{
		// Note: m_hWnd is set in CWnd::CreateEx(...)
		::ZeroMemory(&m_cs, sizeof(CREATESTRUCT));
		::ZeroMemory(&m_wc, sizeof(WNDCLASS));
	}

	CWnd::~CWnd()
	{
		// Destroy the window for this object
		Destroy();
	}

	void CWnd::AddToMap()
	{
		// Store the window handle and CWnd pointer in the HWND map
		GetApp()->m_csMapLock.Lock();
		if (m_hWnd == 0)
			throw CWinException(_T("CWnd::AddToMap  can't add a NULL HWND"));
		if (GetApp()->GetCWndFromMap(m_hWnd))
			throw CWinException(_T("CWnd::AddToMap  HWND already in map"));

		GetApp()->m_mapHWND.insert(std::make_pair(m_hWnd, this));
		GetApp()->m_csMapLock.Release();
	}

	BOOL CWnd::Attach(HWND hWnd)
	// Subclasses an existing window and attaches it to a CWnd
	{
		try
		{
			if (!::IsWindow(hWnd))
				throw CWinException(_T("Attach failed, not a valid hwnd"));

			if (0 != GetApp()->GetCWndFromMap(hWnd))
				throw CWinException(_T("Window already attached to this CWnd object"));

			Subclass(hWnd);

			// Store the CWnd pointer in the HWND map
			AddToMap();
			OnInitialUpdate();
			return TRUE;
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
			throw;
		}
	}

	BOOL CWnd::AttachDlgItem(UINT nID, CWnd* pParent)
	// Use to convert a dialog item to a CWnd object
	{
		HWND hWnd = ::GetDlgItem(pParent->GetHwnd(), nID);

		return Attach(hWnd);
	}

	void CWnd::CenterWindow() const
	{
		// Centers this window over it's parent

		CRect rc = GetWindowRect();
		CRect rcParent;
		CRect rcDesktop;

		// Get screen dimensions excluding task bar
		::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcDesktop, 0);

		// Get the parent window dimensions (parent could be the desktop)
		if (GetParent() != NULL) ::GetWindowRect(GetParent(), &rcParent);
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

	HWND CWnd::Create(HWND hWndParent /* = NULL */)
	// Default Window Creation.
	{
		try
		{
			// Test if Win32++ has been started
			if (0 == GetApp())
				throw CWinException(_T("Win32++ has not been initialised properly.\n Start the Win32++ by inheriting from CWinApp."));

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
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		return m_hWnd;
	}

	HWND CWnd::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rc, HWND hParent, HMENU hMenu, LPVOID lpParam /*= NULL*/)
	{
		int x = rc.left;
		int y = rc.top;
		int cx = rc.right - rc.left;
		int cy = rc.bottom - rc.top;
		return CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, x, y, cx, cy, hParent, hMenu, lpParam);
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
			wc.style 		 = CS_DBLCLKS;	// generate left button double click messages
			
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
			e.MessageBox();
		}

		return m_hWnd;

	} // HWND CWnd::CreateEx()

	LRESULT CWnd::FinalWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	// Pass messages on to the appropriate default window procedure
	// CMDIChild and CMDIFrame override this function
	{
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	void CWnd::Destroy()
	{
		if (IsWindow()) ::DestroyWindow(m_hWnd);

		// Return the CWnd to its default state
		if (m_hIconLarge) ::DestroyIcon(m_hIconLarge);
		if (m_hIconSmall) ::DestroyIcon(m_hIconSmall);

		RemoveFromMap();
		m_hIconLarge = NULL;
		m_hIconSmall = NULL;
		m_hWnd = NULL;
		m_PrevWindowProc = NULL;
	}

	HWND CWnd::Detach()
	// Reverse an Attach
	{
		//Only a subclassed window can be detached
		if (0 == m_PrevWindowProc)
			throw CWinException(_T("CWnd::Detach  Unable to detach this window"));

		::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)m_PrevWindowProc);

		// Clear member variables
		HWND hWnd = m_hWnd;
		Destroy();

		return hWnd;
	}

	CWnd* CWnd::FromHandle(HWND hWnd)
	{
		// Returns the CWnd object associated with the window handle
		return GetApp()->GetCWndFromMap(hWnd);
	}

	HWND CWnd::GetAncestor() const
	// The GetAncestor function retrieves the handle to the ancestor (root parent)
	// of the window.
	{
		// Returns the root parent.  Supports Win95
		HWND hWnd = m_hWnd;
		HWND hWndParent = ::GetParent(hWnd);
		while (::IsChild(hWndParent, hWnd))
		{
			hWnd = hWndParent;
			hWndParent = ::GetParent(hWnd);
		}

		return hWnd;
	}

	tString CWnd::GetClassString() const
	// Retrieves the name of the class to which the specified window belongs
	{
		TCHAR szString[MAX_STRING_SIZE +1];
		tString tstr;
		::GetClassName(m_hWnd, szString, MAX_STRING_SIZE);
		tstr = szString;

		return tstr;
	}

	tString CWnd::GetDlgItemString(int nIDDlgItem) const
	// The GetDlgItemString function retrieves the title or text associated
	// with a control in a dialog box.
	{
		int nLength = ::GetWindowTextLength(GetDlgItem(nIDDlgItem));

		tString tstr;
		if (nLength > 0)
		{
			TCHAR szString[MAX_STRING_SIZE +1];
			::GetDlgItemText(m_hWnd, nIDDlgItem, szString, MAX_STRING_SIZE);
			tstr = szString;
		}

		return tstr;
	}

	tString CWnd::GetWindowString() const
	// Gets the window title for an ordinary window, or the text in an edit control
	{
		tString tstr;
		int nLength = ::GetWindowTextLength(m_hWnd);
		if (nLength > 0)
		{
			TCHAR* pszString = new TCHAR[nLength+1];
			if (NULL == pszString) throw std::bad_alloc();
			if (0 != ::GetWindowText(m_hWnd, pszString, nLength+1))
				tstr = pszString;

			delete [] pszString;
		}
		return tstr;
	}

	HBITMAP CWnd::LoadBitmap(LPCTSTR lpBitmapName) const
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

		m_tsLoadString = _T("");
		TCHAR szString[MAX_STRING_SIZE +1] = _T("");
		if (!::LoadString (GetApp()->GetResourceHandle(), nID, szString, MAX_STRING_SIZE))
		{
			// The string resource might be in the application's resources instead
			if (::LoadString (GetApp()->GetInstanceHandle(), nID, szString, MAX_STRING_SIZE))
			{
				m_tsLoadString = szString;
				return (LPCTSTR) m_tsLoadString.c_str();
			}

			TCHAR msg[80] = _T("");
			::wsprintf(msg, _T("LoadString - No string resource for %d\n"), nID);
			TRACE(msg);
		}

		m_tsLoadString = szString;

		// Never return a pointer to a local variable, it is out of scope when the function returns.
		// We return a pointer to a member variable so it remains in scope.
		return m_tsLoadString.c_str();
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

	LRESULT CWnd::MessageReflect(HWND hWndParent, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		//      Return the value recommended by the Windows API documentation.
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
		//      Return the value recommended by the Windows API documentation.
		// }

		// return 0L for unhandled notifications
		return 0L;
	}

	void CWnd::OnPaint(HDC)
	// Called when part of the client area of the window needs to be painted
	{
		// Override this function in your derived class to perform drawing tasks.
	}

	void CWnd::OnMenuUpdate(UINT /*nID*/)
	// Called when menu items are about to be displayed
	{
		// Override this function to modify the behaviour of menu items,
		// such as adding or removing checkmarks
	}

	void CWnd::PreCreate(CREATESTRUCT& cs)
	// Called by CWnd::Create to set some window parameters
	{
		// Test if Win32++ has been started
		if (0 == GetApp())
			throw CWinException(_T("Win32++ has not been initialised properly.\n Start the Win32++ by inheriting from CWinApp."));

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

	void CWnd::PreRegisterClass(WNDCLASS& wc)
	// Called by CWnd::Create to set some window parameters
	//  Useful for setting the background brush and cursor
	{
		// Test if Win32++ has been started
		if (0 == GetApp())
			throw CWinException(_T("Win32++ has not been initialised properly.\n Start the Win32++ by inheriting from CWinApp."));

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

	BOOL CWnd::PreTranslateMessage(MSG* /*pMsg*/)
	{
		// Override this function if your class requires input messages to be
		// translated before normal processing. Function which translate messages
		// include TranslateAccelerator, TranslateMDISysAccel and IsDialogMessage.
		// Return TRUE if the message is translated.

		return FALSE;
	}

	BOOL CWnd::RegisterClass(WNDCLASS& wc)
	// A private function used by the PreRegisterClass function to register a
	//  window class prior to window creation
	{
		try
		{
			// Test if Win32++ has been started
			if (0 == GetApp())
				throw CWinException(_T("Win32++ has not been initialised properly.\n Start the Win32++ by inheriting from CWinApp."));

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

	BOOL CWnd::RemoveFromMap()
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
					pApp->m_csMapLock.Release();
					return TRUE;
				}
			}

			pApp->m_csMapLock.Release();
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



	LRESULT CALLBACK CWnd::StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	// All CWnd windows direct their messages here. This function redirects the message
	// to the CWnd's WndProc function.
	{
		try
		{
			CWnd* w = GetApp()->GetCWndFromMap(hWnd);
			if (0 != w)
			{
				// CWnd pointer found, so call the CWnd's WndProc
				return w->WndProc(uMsg, wParam, lParam);
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

				return w->WndProc(uMsg, wParam, lParam);
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

	void CWnd::Subclass(HWND hWnd)
	// A private function used by CreateEx, Attach and AttachDlgItem
	{
		if (m_PrevWindowProc)
			throw CWinException(_T("Subclass failed.  Already Subclassed"));

		// Subclass the window to pass messages to WndProc
		WNDPROC WndProc = (WNDPROC)::GetWindowLongPtr(hWnd, GWLP_WNDPROC);
		if (WndProc == GetApp()->m_Callback)
			throw CWinException(_T("Subclass failed.  Already sending messages to StaticWindowProc"));
		m_PrevWindowProc = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)CWnd::StaticWindowProc);
		if (NULL == m_PrevWindowProc)
			throw CWinException(_T("Subclass failed."));

		m_hWnd = hWnd;
	}

	LRESULT CWnd::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

	LRESULT CWnd::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	// All WndProc functions should pass unhandled window messages to this function
	{
    	switch (uMsg)
		{
		case WM_COMMAND:
			{
				// Refelect this message if it's from a control
				CWnd* pWnd = FromHandle((HWND)lParam);
				if (pWnd != NULL)
					if (pWnd->OnMessageReflect(uMsg, wParam, lParam))
						return TRUE;

				// Handle user commands
				if (OnCommand(wParam, lParam))
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
				// Do Notification reflection if it came from a CWnd object
				HWND hwndFrom = ((LPNMHDR)lParam)->hwndFrom;
				CWnd* pWndFrom = FromHandle(hwndFrom);
				LRESULT lr = 0L;

				if (!(IsRebar()))	// Skip notification reflection for rebars to avoid double handling
				{
					if (pWndFrom != NULL)
					{
						lr = pWndFrom->OnNotifyReflect(wParam, lParam);
						if (lr) return lr;
					}
					else
					{
						// Some controls (eg ListView) have child windows.
						// Reflect those notifications too.
						CWnd* pWndFromParent = FromHandle(::GetParent(hwndFrom));
						if (pWndFromParent != NULL)
						{
							lr = pWndFromParent->OnNotifyReflect(wParam, lParam);
							if (lr) return lr;
						}
					}
				}

				// Handle user notifications
				lr = OnNotify(wParam, lParam);
				if (lr) return lr;
			}
			break;

		case WM_PAINT:
			{
				if (m_PrevWindowProc) break; // Allow normal painting for subclassed windows

				if (::GetUpdateRect(m_hWnd, NULL, FALSE))
				{
					::PAINTSTRUCT ps;
					HDC hDC = ::BeginPaint(m_hWnd, &ps);

					OnPaint(hDC);
					::EndPaint(m_hWnd, &ps);
				}
				else
				// RedrawWindow can require repainting without an update rect
				{
					HDC hDC = ::GetDC(m_hWnd);

					OnPaint(hDC);
					::ReleaseDC(m_hWnd, hDC);
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

	BOOL CWnd::BringWindowToTop() const
	// The BringWindowToTop function brings the specified window to the top
	// of the Z order. If the window is a top-level window, it is activated.
	{
		return ::BringWindowToTop(m_hWnd);
	}

	LRESULT CWnd::CallWindowProc(WNDPROC lpPrevWndFunc, UINT Msg, WPARAM wParam, LPARAM lParam) const
	{
		return ::CallWindowProc(lpPrevWndFunc, m_hWnd, Msg, wParam, lParam);
	}

	BOOL CWnd::CheckDlgButton(int nIDButton, UINT uCheck) const
	// The CheckDlgButton function changes the check state of a button control.
	{
		return ::CheckDlgButton(m_hWnd, nIDButton, uCheck);
	}

	HDWP CWnd::DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) const
	// The DeferWindowPos function updates the specified multiple-window � position structure for the window.
	{
		return ::DeferWindowPos(hWinPosInfo, m_hWnd, hWndInsertAfter, x, y, cx, cy, uFlags);
	}

	HDWP CWnd::DeferWindowPos(HDWP hWinPosInfo, HWND hWndInsertAfter, const RECT& rc, UINT uFlags) const
	// The DeferWindowPos function updates the specified multiple-window � position structure for the window.
	{
		return ::DeferWindowPos(hWinPosInfo, m_hWnd, hWndInsertAfter, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, uFlags);
	}

	LRESULT CWnd::DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam) const
	// This function provides default processing for any window messages that an application does not process.
	{
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}

	BOOL CWnd::DrawMenuBar() const
	{
		return ::DrawMenuBar(m_hWnd);
	}

	BOOL CWnd::EnableWindow(BOOL bEnable /*= TRUE*/) const
	// The EnableWindow function enables or disables mouse and
	// keyboard input to the window.
	{
		return ::EnableWindow(m_hWnd, bEnable);
	}

	ULONG_PTR CWnd::GetClassLongPtr(int nIndex) const
	// The GetClassLongPtr function retrieves the specified value from the
	// WNDCLASSEX structure associated with the window.
	{
		return ::GetClassLongPtr(m_hWnd, nIndex);
	}

	CRect CWnd::GetClientRect() const
	// The GetClientRect function retrieves the coordinates of a window's client area.
	// The client coordinates specify the upper-left and lower-right corners of the
	// client area. Because client coordinates are relative to the upper-left corner
	// of a window's client area, the coordinates of the upper-left corner are (0,0).
	{
		CRect rc;
		::GetClientRect(m_hWnd, &rc);
		return rc;
	}

	HDC CWnd::GetDC() const
	// The GetDC function retrieves a handle to a display device context (DC) for the
	// client area of the window.
	{
		return ::GetDC(m_hWnd);
	}

	HDC CWnd::GetDCEx(HRGN hrgnClip, DWORD flags) const
	// The GetDCEx function retrieves a handle to a display device context (DC) for the
	// client area or entire area of a window
	{
		return ::GetDCEx(m_hWnd, hrgnClip, flags);
	}

	HWND CWnd::GetDlgItem(int nIDDlgItem) const
	// The GetDlgItem function retrieves a handle to a control in the dialog box.
	{
		return ::GetDlgItem(m_hWnd, nIDDlgItem);
	}

	int CWnd::GetDlgItemInt(int nIDDlgItem, BOOL* lpTranslated, BOOL bSigned) const
	{
		return ::GetDlgItemInt(m_hWnd, nIDDlgItem, lpTranslated, bSigned);
	}

	HWND CWnd::GetParent() const
	{
		return ::GetParent(m_hWnd);
	}

	LONG_PTR CWnd::GetWindowLongPtr(int nIndex) const
	// The GetWindowLongPtr function retrieves information about the window.
	{
		return ::GetWindowLongPtr(m_hWnd, nIndex);
	}

	BOOL CWnd::GetScrollInfo(int fnBar, SCROLLINFO& si) const
	// The GetScrollInfo function retrieves the parameters of a scroll bar, including
	// the minimum and maximum scrolling positions, the page size, and the position
	// of the scroll box (thumb).
	{
		return ::GetScrollInfo(m_hWnd, fnBar, &si);
	}

	HWND CWnd::GetWindow(UINT uCmd) const
	// The GetWindow function retrieves a handle to a window that has the specified
	// relationship (Z-Order or owner) to the specified window.
	// Possible uCmd values: GW_CHILD, GW_ENABLEDPOPUP, GW_HWNDFIRST, GW_HWNDLAST,
	// GW_HWNDNEXT, GW_HWNDPREV, GW_OWNER
	{
		return ::GetWindow(m_hWnd, uCmd);
	}

	HDC CWnd::GetWindowDC() const
	// The GetWindowDC function retrieves the device context (DC) for the entire
	// window, including title bar, menus, and scroll bars.
	{
		return ::GetWindowDC(m_hWnd);
	}

	CRect CWnd::GetWindowRect() const
	// retrieves the dimensions of the bounding rectangle of the window.
	// The dimensions are given in screen coordinates that are relative to the
	// upper-left corner of the screen.
	{
		CRect rc;
		::GetWindowRect(m_hWnd, &rc);
		return rc;
	}

	void CWnd::Invalidate(BOOL bErase /*= TRUE*/) const
	// The Invalidate function adds the entire client area the window's update region.
	// The update region represents the portion of the window's client area that must be redrawn.
	{
		::InvalidateRect(m_hWnd, NULL, bErase);
	}

	BOOL CWnd::InvalidateRect(LPCRECT lpRect, BOOL bErase /*= TRUE*/) const
	// The InvalidateRect function adds a rectangle to the window's update region.
	// The update region represents the portion of the window's client area that must be redrawn.
	{
		return ::InvalidateRect(m_hWnd, lpRect, bErase);
	}

	BOOL CWnd::InvalidateRgn(CONST HRGN hRgn, BOOL bErase /*= TRUE*/) const
	// The InvalidateRgn function invalidates the client area within the specified region
	// by adding it to the current update region of a window. The invalidated region,
	// along with all other areas in the update region, is marked for painting when the
	// next WM_PAINT message occurs.
	{
		return ::InvalidateRgn(m_hWnd, hRgn, bErase);
	}

	BOOL CWnd::IsChild(HWND hWndParent) const
	// The IsChild function tests whether a window is a child window or descendant window
	// of a parent window's CWnd.
	{
		return ::IsChild(hWndParent, m_hWnd);
	}

	BOOL CWnd::IsWindowEnabled() const
	// The IsWindowEnabled function determines whether the window is enabled
	// for mouse and keyboard input.
	{
		return ::IsWindowEnabled(m_hWnd);
	}

	BOOL CWnd::IsWindowVisible() const
	// The IsWindowVisible function retrieves the visibility state of the window.
	{
		return ::IsWindowVisible(m_hWnd);
	}

	BOOL CWnd::IsWindow() const
	// The IsWindow function determines whether the window exists.
	{
		return ::IsWindow(m_hWnd);
	}

	int CWnd::MessageBox(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType) const
	// The MessageBox function creates, displays, and operates a message box.
	// Possible combinations of uType values include: MB_OK, MB_HELP, MB_OKCANCEL, MB_RETRYCANCEL,
	// MB_YESNO, MB_YESNOCANCEL, MB_ICONEXCLAMATION, MB_ICONWARNING, MB_ICONERROR (+ many others).
	{
		return ::MessageBox(m_hWnd, lpText, lpCaption, uType);
	}

	void CWnd::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint /* = TRUE*/) const
	// The MoveWindow function changes the position and dimensions of the window.
	{
		::MoveWindow(m_hWnd, x, y, nWidth, nHeight, bRepaint = TRUE);
	}

	void CWnd::MoveWindow(const RECT& rc, BOOL bRepaint /* = TRUE*/) const
	// The MoveWindow function changes the position and dimensions of the window.
	{
		::MoveWindow(m_hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, bRepaint);
	}

	BOOL CWnd::PostMessage(UINT uMsg, WPARAM wParam /*= 0L*/, LPARAM lParam /*= 0L*/) const
	// The PostMessage function places (posts) a message in the message queue
	// associated with the thread that created the window and returns without
	// waiting for the thread to process the message.
	{
		return ::PostMessage(m_hWnd, uMsg, wParam, lParam);
	}

	BOOL CWnd::PostMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
	// Required by by some macros
	{
		return ::PostMessage(hWnd, uMsg, wParam, lParam);
	}

	BOOL CWnd::RedrawWindow(LPCRECT lpRectUpdate, HRGN hRgn, UINT flags) const
	// The RedrawWindow function updates the specified rectangle or region in a window's client area.
	{
		return ::RedrawWindow(m_hWnd, lpRectUpdate, hRgn, flags);
	}

	int CWnd::ReleaseDC(HDC hDC) const
	// The ReleaseDC function releases a device context (DC), freeing it for use
	// by other applications.
	{
		return ::ReleaseDC(m_hWnd, hDC);
	}

	LRESULT CWnd::SendDlgItemMessage(int nIDDlgItem, UINT Msg, WPARAM wParam, LPARAM lParam) const
	// The SendDlgItemMessage function sends a message to the specified control in a dialog box.
	{
		return ::SendDlgItemMessage(m_hWnd, nIDDlgItem, Msg, wParam, lParam);
	}

	LRESULT CWnd::SendMessage(UINT uMsg, WPARAM wParam /*= 0L*/, LPARAM lParam /*= 0L*/) const
	// The SendMessage function sends the specified message to a window or windows.
	// It calls the window procedure for the window and does not return until the
	// window procedure has processed the message.
	{
		return ::SendMessage(m_hWnd, uMsg, wParam, lParam);
	}

	LRESULT CWnd::SendMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
	// Required by by some macros
	{
		return ::SendMessage(hWnd, uMsg, wParam, lParam);
	}

	HWND CWnd::SetActiveWindow() const
	// The SetActiveWindow function activates the window, but
	// not if the application is in the background.
	{
		return ::SetActiveWindow(m_hWnd);
	}

	HWND CWnd::SetCapture() const
	// The SetCapture function sets the mouse capture to the window.
	// SetCapture captures mouse input either when the mouse is over the capturing
	// window, or when the mouse button was pressed while the mouse was over the
	// capturing window and the button is still down.
	{
		return ::SetCapture(m_hWnd);
	}

	ULONG_PTR CWnd::SetClassLongPtr(int nIndex, LONG_PTR dwNewLong) const
	// The SetClassLongPtr function replaces the specified value at the specified offset in the
	// extra class memory or the WNDCLASSEX structure for the class to which the window belongs.
	{
		return ::SetClassLongPtr(m_hWnd, nIndex, dwNewLong);
	}

	HWND CWnd::SetFocus() const
	// The SetFocus function sets the keyboard focus to the window.
	{
		return ::SetFocus(m_hWnd);
	}

	BOOL CWnd::SetForegroundWindow() const
	// The SetForegroundWindow function puts the thread that created the window into the
	// foreground and activates the window.
	{
		return ::SetForegroundWindow(m_hWnd);
	}

	HWND CWnd::SetParent(HWND hParent) const
	// The SetParent function changes the parent window of the child window.
	{
		return ::SetParent(m_hWnd, hParent);
	}

	BOOL CWnd::SetRedraw(BOOL bRedraw /*= TRUE*/) const
	// This function allows changes in that window to be redrawn or prevents changes
	// in that window from being redrawn.
	{
		return (BOOL)::SendMessage(m_hWnd, WM_SETREDRAW, (WPARAM)bRedraw, 0L);
	}

	LONG_PTR CWnd::SetWindowLongPtr(int nIndex, LONG_PTR dwNewLong) const
	// The SetWindowLongPtr function changes an attribute of the window.
	{
		return ::SetWindowLongPtr(m_hWnd, nIndex, dwNewLong);
	}

	BOOL CWnd::SetWindowPos(HWND hWndInsertAfter, int x, int y, int cx, int cy, UINT uFlags) const
	// The SetWindowPos function changes the size, position, and Z order of a child, pop-up,
	// or top-level window.
	{
		return ::SetWindowPos(m_hWnd, hWndInsertAfter, x, y, cx, cy, uFlags);
	}

	BOOL CWnd::SetWindowPos(HWND hWndInsertAfter, const RECT& rc, UINT uFlags) const
	// The SetWindowPos function changes the size, position, and Z order of a child, pop-up,
	// or top-level window.
	{
		return ::SetWindowPos(m_hWnd, hWndInsertAfter, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, uFlags);
	}

	int CWnd::SetWindowRgn(HRGN hRgn, BOOL bRedraw /*= TRUE*/) const
	// The SetWindowRgn function sets the window region of the window.
	// The window region determines the area within the window where the system permits drawing.
	{
		return ::SetWindowRgn(m_hWnd, hRgn, bRedraw);
	}

	BOOL CWnd::SetDlgItemInt(int nIDDlgItem, UINT uValue, BOOL bSigned) const
	{
		return ::SetDlgItemInt(m_hWnd, nIDDlgItem, uValue, bSigned);
	}

	BOOL CWnd::SetDlgItemText(int nIDDlgItem, LPCTSTR lpString) const
	// The SetDlgItemText function sets the title or text of a control in a dialog box.
	{
		return ::SetDlgItemText(m_hWnd, nIDDlgItem, lpString);
	}

	BOOL CWnd::SetWindowText(LPCTSTR lpString) const
	// The SetWindowText function changes the text of the window's title bar (if it has one).
	{
		return ::SetWindowText(m_hWnd, lpString);
	}

	BOOL CWnd::ShowWindow(int nCmdShow /*= SW_SHOWNORMAL*/) const
	// The ShowWindow function sets the window's show state.
	{
		return ::ShowWindow(m_hWnd, nCmdShow);
	}

	BOOL CWnd::UpdateWindow() const
	// The UpdateWindow function updates the client area of the window by sending a
	// WM_PAINT message to the window if the window's update region is not empty.
	// If the update region is empty, no message is sent.
	{
		return ::UpdateWindow(m_hWnd);
	}

	BOOL CWnd::ValidateRect(LPCRECT prc) const
	// The ValidateRect function validates the client area within a rectangle by
	// removing the rectangle from the update region of the window.
	{
		return ::ValidateRect(m_hWnd, prc);
	}

	BOOL CWnd::ValidateRgn(HRGN hRgn) const
	// The ValidateRgn function validates the client area within a region by
	// removing the region from the current update region of the window.
	{
		return ::ValidateRgn(m_hWnd, hRgn);
	}

//
// These functions aren't supported on WinCE
//
#ifndef _WIN32_WCE
	BOOL CWnd::CloseWindow() const
	// The CloseWindow function minimizes (but does not destroy) the window.
	// To destroy a window, an application can use the Destroy function.
	{
		return ::CloseWindow(m_hWnd);
	}

	BOOL CWnd::EnableScrollBar(UINT uSBflags, UINT uArrows) const
	{
		return ::EnableScrollBar(m_hWnd, uSBflags, uArrows);
	}

	HMENU CWnd::GetMenu() const
	// The GetMenu function retrieves a handle to the menu assigned to the window.
	{
		return ::GetMenu(m_hWnd);
	}

	int CWnd::GetScrollPos(int nBar) const
	// The GetScrollPos function retrieves the current position of the scroll box
	// (thumb) in the specified scroll bar.
	{
		return ::GetScrollPos(m_hWnd, nBar);
	}

	BOOL CWnd::GetScrollRange(int nBar, int& MinPos, int& MaxPos) const
	// The GetScrollRange function retrieves the current minimum and maximum scroll box
	// (thumb) positions for the specified scroll bar.
	{
		return ::GetScrollRange(m_hWnd, nBar, &MinPos, &MaxPos );
	}

	BOOL CWnd::GetWindowPlacement(WINDOWPLACEMENT& wndpl) const
	// The GetWindowPlacement function retrieves the show state and the restored,
	// minimized, and maximized positions of the window.
	{
		return ::GetWindowPlacement(m_hWnd, &wndpl);
	}

	BOOL CWnd::IsIconic() const
	// The IsIconic function determines whether the window is minimized (iconic).
	{
		return ::IsIconic(m_hWnd);
	}

	BOOL CWnd::IsZoomed() const
	// The IsZoomed function determines whether the window is maximized.
	{
		return ::IsZoomed(m_hWnd);
	}

	BOOL CWnd::LockWindowUpdate(HWND hWndLock) const
	// Disables or enables drawing in the specified window. Only one window can be locked at a time.
	// Use a hWndLock of NULL to re-enable drawing in the window
	{
		return ::LockWindowUpdate(hWndLock);
	}

	BOOL CWnd::ScrollWindow(int XAmount, int YAmount, LPCRECT prcScroll, LPCRECT prcClip) const
	{
		return ::ScrollWindow(m_hWnd, XAmount, YAmount, prcScroll, prcClip);
	}

	int CWnd::ScrollWindowEx(int dx, int dy, LPCRECT prcScroll, LPCRECT prcClip, HRGN hrgnUpdate, LPRECT prcUpdate, UINT flags) const
	// The ScrollWindow function scrolls the contents of the window's client area.
	{
		return ::ScrollWindowEx(m_hWnd, dx, dy, prcScroll, prcClip, hrgnUpdate, prcUpdate, flags);
	}

	BOOL CWnd::SetMenu(HMENU hMenu) const
	// The SetMenu function assigns a new menu to the specified window.
	{
		return ::SetMenu(m_hWnd, hMenu);
	}

	int CWnd::SetScrollInfo(int fnBar, const SCROLLINFO& si, BOOL fRedraw) const
	// The SetScrollInfo function sets the parameters of a scroll bar, including
	// the minimum and maximum scrolling positions, the page size, and the
	// position of the scroll box (thumb).
	{
		return ::SetScrollInfo(m_hWnd, fnBar, &si, fRedraw);
	}

	int CWnd::SetScrollPos(int nBar, int nPos, BOOL bRedraw) const
	// The SetScrollPos function sets the position of the scroll box (thumb) in
	// the specified scroll bar
	{
		return ::SetScrollPos(m_hWnd, nBar, nPos, bRedraw);
	}

	BOOL CWnd::SetScrollRange(int nBar, int nMinPos, int nMaxPos, BOOL bRedraw) const
	// The SetScrollRange function sets the minimum and maximum scroll box positions for the scroll bar.
	{
		return ::SetScrollRange(m_hWnd, nBar, nMinPos, nMaxPos, bRedraw);
	}

	BOOL CWnd::SetWindowPlacement(const WINDOWPLACEMENT& wndpl) const
	// The SetWindowPlacement function sets the show state and the restored, minimized,
	// and maximized positions of the window.
	{
		return ::SetWindowPlacement(m_hWnd, &wndpl);
	}

	BOOL CWnd::ShowScrollBar(int nBar, BOOL bShow) const
	{
		return ::ShowScrollBar(m_hWnd, nBar, bShow);
	}
#endif

}; // namespace Win32xx

