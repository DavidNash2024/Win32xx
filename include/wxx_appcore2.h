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


#ifndef _WIN32XX_APPCORE2_H_
#define _WIN32XX_APPCORE2_H_


///////////////////////////////////////////////////////
// wxx_appcore2.h
// This file contains the definitions of the following set of classes.
//
// 1) CObject: A base class for CWnd and any other class that uses serialization.
//             It provides a virtual Serialize function for use by CArchive.
//
// 2) CWinApp: This class is used start Win32++ and run the message loop. You
//            should inherit from this class to start Win32++ in your own
//            application.
//
// 3) CWinThread: This class is the parent class for CWinApp. It is also the
//            class used to create additional GUI and worker threads.


#include "wxx_appcore1.h"


namespace Win32xx
{

	///////////////////////////////////////
	// Definitions for the CObject class
	//
	inline void CObject::Serialize(CArchive& /* ar */ )
	{
	//	Override Serialize in the class inherited from CObject like this.
	
	//	if (ar.IsStoring())
	//	{
	//		// Store a member variable in the archive
	//		ar << m_SomeValue;
	//	}
	//	else
	//	{
	//		// Load a member variable from the archive
	//		ar >> m_SomeValue;
	//	}

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
		m_hThread = (HANDLE)::_beginthreadex(pSecurityAttributes, stack_size, m_pfnThreadProc, m_pThreadParams, initflag, &m_nThreadID);
#endif

		if (m_hThread == 0)
			throw CWinException(_T("Failed to create thread"));
	}

	inline CWnd* CWinThread::GetMainWnd() const
	// Retrieves a pointer to the main window for this thread.
	// Note: CFrame set's itself as the main window of its thread
	{
		TLSData* pTLSData = GetApp().GetTlsData();
		return pTLSData->pMainWnd;
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

			if (!PreTranslateMessage(Msg))
			{
				::TranslateMessage(&Msg);
				::DispatchMessage(&Msg);
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
					CWnd* pWnd = GetApp().GetCWndFromMap(hWnd);
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

	inline BOOL CWinThread::PostThreadMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) const
	// Posts a message to the thread. The message will reach the MessageLoop, but 
	//  will not call a CWnd's WndProc.
	{
		assert(m_hThread);
		return ::PostThreadMessage(GetThreadID(), uMsg, wParam, lParam);
	}

	inline DWORD CWinThread::ResumeThread() const
	// Resumes a thread that has been suspended, or created with the CREATE_SUSPENDED flag.
	{
		assert(m_hThread);
		return ::ResumeThread(m_hThread);
	}

	inline void CWinThread::SetAccelerators(HACCEL hAccel, CWnd* pWndAccel)
	// hAccel is the handle of the accelerator table
	// pWndAccel is the window pointer for translated messages
	{
		m_pWndAccel = pWndAccel;
		m_hAccel = hAccel;
	}

	inline void CWinThread::SetMainWnd(CWnd* pWnd)
	// Sets the main window for this thread.
	// Note: CFrame set's itself as the main window of its thread
	{
		TLSData* pTLSData = GetApp().SetTlsData();
		pTLSData->pMainWnd = pWnd;
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
		assert(pThread);

		if (pThread->InitInstance())
		{
			GetApp().SetTlsData();
			return pThread->MessageLoop();
		}

		return 0;
	}


	////////////////////////////////////
	// Definitions for the CWinApp class
	//

	// To begin Win32++, inherit your application class from this one.
	// You must run only one instance of the class inherited from this.
	inline CWinApp::CWinApp() : m_Callback(NULL)
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

	inline CWinApp::~CWinApp()
	{
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

#ifndef _WIN32_WCE
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
		return static_cast<TLSData*>(TlsGetValue(m_dwTlsData));
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
		return ::LoadCursor(GetResourceHandle(), lpszResourceName);
	}

	inline HCURSOR CWinApp::LoadCursor(int nIDCursor) const
	// Loads the cursor resource from the resource script (resource.rc)
	{
		return ::LoadCursor(GetResourceHandle(), MAKEINTRESOURCE (nIDCursor));
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
		return ::LoadIcon(GetResourceHandle(), lpszResourceName);
	}

	inline HICON CWinApp::LoadIcon(int nIDIcon) const
	// Loads the icon resource whose size conforms to the SM_CXICON and SM_CYICON system metric values
	{
		return ::LoadIcon(GetResourceHandle(), MAKEINTRESOURCE (nIDIcon));
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
		return ::LoadImage(GetResourceHandle(), lpszResourceName, uType, cx, cy, fuLoad);
	}

	inline HANDLE CWinApp::LoadImage(int nIDImage, UINT uType, int cx, int cy, UINT fuLoad) const
	// Loads an icon, cursor, animated cursor, or bitmap
	// uType is the image type. It can be IMAGE_BITMAP, IMAGE_CURSOR or IMAGE_ICON
	// cx and cy are the desired width and height in pixels
	// fuLoad can be LR_DEFAULTCOLOR, LR_CREATEDIBSECTION, LR_DEFAULTSIZE, LR_LOADFROMFILE,
	//   LR_LOADMAP3DCOLORS, R_LOADTRANSPARENT, LR_MONOCHROME, LR_SHARED, LR_VGACOLOR.
	// The image should be destroyed unless it is loaded with LR_SHARED
	{
		return ::LoadImage(GetResourceHandle(), MAKEINTRESOURCE (nIDImage), uType, cx, cy, fuLoad);
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
		TLSData* pTLSData = GetTlsData();
		
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
		

} // namespace Win32xx

#endif // _WIN32XX_APPCORE2_H_

