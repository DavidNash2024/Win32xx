// Win32++   Version 8.5.1
// Release Date: TBA
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2017  David Nash
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


#ifndef _WIN32XX_APPCORE_H_
#define _WIN32XX_APPCORE_H_


///////////////////////////////////////////////////////
// wxx_appcore.h
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


#include "wxx_appcore0.h"
#include "wxx_textconv.h"
#include "wxx_wincore0.h"
#include "wxx_exception.h"

namespace Win32xx
{

    ///////////////////////////////////////
    // Definitions for the CObject class
    //
    inline void CObject::Serialize(CArchive& /* ar */ )
    {
    //  Override Serialize in the class inherited from CObject like this.

    //  if (ar.IsStoring())
    //  {
    //      // Store a member variable in the archive
    //      ar << m_SomeValue;
    //  }
    //  else
    //  {
    //      // Load a member variable from the archive
    //      ar >> m_SomeValue;
    //  }

    }

    ///////////////////////////////////////
    // Definitions for the CWinThread class
    //

    // CWinThread constructor.
    // Override CWinThread and use this constructor for GUI threads.
    // InitInstance will be called when the thread runs.
    inline CWinThread::CWinThread() : m_pfnThreadProc(0), m_pThreadParams(0), m_hThread(0),
                                       m_nThreadID(0), m_dwThreadID(0), m_hAccel(0), m_hWndAccel(0)
    {
    }


    // CWinThread constructor.
    // Use CWinThread directly and call this constructor for worker threads.
    // Specify a pointer to the function to run when the thread starts.
    // Specifying pParam for a worker thread is optional.
    inline CWinThread::CWinThread(PFNTHREADPROC pfnThreadProc, LPVOID pParam) : m_pfnThreadProc(0),
                        m_pThreadParams(0), m_hThread(0), m_nThreadID(0), m_dwThreadID(0), m_hAccel(0), m_hWndAccel(0)
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
            if (IsRunning())
            {
                TRACE("*** Warning *** Ending CWinThread before ending its thread\n");
            }

            // Close the thread's handle
            VERIFY(::CloseHandle(m_hThread));
        }
    }


    // Creates a new thread
    // Valid argument values:
    // initflag                 Either CREATE_SUSPENDED or 0
    // stack_size               Either the stack size or 0
    // pSecurityAttributes      Either a pointer to SECURITY_ATTRIBUTES or 0
    inline HANDLE CWinThread::CreateThread(unsigned initflag /* = 0 */, unsigned stack_size/* = 0 */, LPSECURITY_ATTRIBUTES pSecurityAttributes /*= NULL*/)
    {
        if (NULL == m_pfnThreadProc) m_pfnThreadProc = CWinThread::StaticThreadProc;
        if (NULL == m_pThreadParams) m_pThreadParams = this;

		// Reusing the CWinThread
		if (m_hThread)
		{
			assert(!IsRunning());
			VERIFY(CloseHandle(m_hThread));
		}

#ifdef _WIN32_WCE
        m_hThread = reinterpret_cast<HANDLE>(::CreateThread(pSecurityAttributes, stack_size, (LPTHREAD_START_ROUTINE)m_pfnThreadProc, m_pThreadParams, initflag, &m_dwThreadID));
#else
        m_hThread = reinterpret_cast<HANDLE>(::_beginthreadex(pSecurityAttributes, stack_size, m_pfnThreadProc, m_pThreadParams, initflag, &m_nThreadID));
#endif

        if (m_hThread == 0)
            throw CWinException(_T("Failed to create thread"));

        return m_hThread;
    }


    // Retrieves a handle to the main window for this thread.
    // Note: CFrame set's itself as the main window of its thread
    inline HWND CWinThread::GetMainWnd() const
    {
        TLSData* pTLSData = GetApp().GetTlsData();

        // Will assert if the thread doesn't have TLSData assigned.
        // TLSData is assigned when the first window in the thread is created.
        assert (pTLSData);

        return pTLSData->hMainWnd;
    }


    // Retrieves the handle of this thread.
    inline HANDLE CWinThread::GetThread() const
    {
        return m_hThread;
    }


    // Retrieves the thread's ID.
    inline int CWinThread::GetThreadID() const
    {
        assert(m_hThread);

#ifdef _WIN32_WCE
        return m_dwThreadID;
#endif

        return m_nThreadID;
    }


    // Retrieves this thread's priority
    inline int CWinThread::GetThreadPriority() const
    {
        assert(m_hThread);
        return ::GetThreadPriority(m_hThread);
    }


    // Override this function to perform tasks when the thread starts.
    // return TRUE to run a message loop, otherwise return FALSE.
    // A thread with a window must run a message loop.
    inline BOOL CWinThread::InitInstance()
    {
        return FALSE;
    }


    // This function manages the way window message are dispatched
    // to a window procedure.
    inline int CWinThread::MessageLoop()
    {
        MSG Msg;
        ZeroMemory(&Msg, sizeof(Msg));
        int status = 1;
        LONG lCount = 0;

        while (status != 0)
        {
            // While idle, perform idle processing until OnIdle returns FALSE
            while (!::PeekMessage(&Msg, 0, 0, 0, PM_NOREMOVE) && OnIdle(lCount) != FALSE  )
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


    // This functions is called by the MessageLoop. It is called when the message queue
    // is empty. Return TRUE to continue idle processing or FALSE to end idle processing
    // until another message is queued. lCount is incremented each time OnIdle is called,
    // and reset to 0 each time a new messages is processed.
    inline BOOL CWinThread::OnIdle(LONG lCount)
    {
        UNREFERENCED_PARAMETER(lCount);

        return FALSE;
    }


    // This functions is called by the MessageLoop. It processes the
    // keyboard accelerator keys and calls CWnd::PreTranslateMessage for
    // keyboard and mouse events.
    inline BOOL CWinThread::PreTranslateMessage(MSG& Msg)
    {
        BOOL IsProcessed = FALSE;

        // only pre-translate mouse and keyboard input events
        if ((Msg.message >= WM_KEYFIRST && Msg.message <= WM_KEYLAST) ||
            (Msg.message >= WM_MOUSEFIRST && Msg.message <= WM_MOUSELAST))
        {
            // Process keyboard accelerators
            if ( ::TranslateAccelerator(GetAcceleratorsWindow(), GetAcceleratorTable(), &Msg))
                IsProcessed = TRUE;
            else
            {
                // Search the chain of parents for pretranslated messages.
                for (HWND hWnd = Msg.hwnd; hWnd != NULL; hWnd = ::GetParent(hWnd))
                {
                    CWnd* pWnd = GetApp().GetCWndFromMap(hWnd);
                    if (pWnd)
                    {
                        IsProcessed = pWnd->PreTranslateMessage(Msg);
                        if(IsProcessed)
                            break;
                    }
                }
            }
        }

        return IsProcessed;
    }


    // Posts a message to the thread. The message will reach the MessageLoop, but
    // will not call a CWnd's WndProc.
    inline BOOL CWinThread::PostThreadMessage(UINT msg, WPARAM wparam, LPARAM lparam) const
    {
        assert(m_hThread);
        return ::PostThreadMessage(GetThreadID(), msg, wparam, lparam);
    }


    // Resumes a thread that has been suspended, or created with the CREATE_SUSPENDED flag.
    inline DWORD CWinThread::ResumeThread() const
    {
        assert(m_hThread);
        return ::ResumeThread(m_hThread);
    }


    // hAccel is the handle of the accelerator table
    // hWndAccel is the window handle for translated messages.
    inline void CWinThread::SetAccelerators(HACCEL hAccel, HWND hWndAccel)
    {
        m_hWndAccel = hWndAccel;
        m_hAccel = hAccel;
    }


    // Sets the main window for this thread.
    // Note: CFrame set's itself as the main window of its thread
    inline void CWinThread::SetMainWnd(HWND hWnd)
    {
        TLSData* pTLSData = GetApp().SetTlsData();
        pTLSData->hMainWnd = hWnd;
    }


    // Sets the priority of this thread. The nPriority parameter can
    // be -7, -6, -5, -4, -3, 3, 4, 5, or 6 or other values permitted
    // by the SetThreadPriority Windows API function.
    inline BOOL CWinThread::SetThreadPriority(int nPriority) const
    {
        assert(m_hThread);
        return ::SetThreadPriority(m_hThread, nPriority);
    }


    // Suspends this thread. Use ResumeThread to resume the thread.
    inline DWORD CWinThread::SuspendThread() const
    {
        assert(m_hThread);
        return ::SuspendThread(m_hThread);
    }


    // When the thread starts, it runs this function.
    inline UINT WINAPI CWinThread::StaticThreadProc(LPVOID pCThread)
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
    // You must run only one instance of the class inherited from CWinApp.

    inline CWinApp::CWinApp() : m_Callback(NULL), m_hDevMode(0), m_hDevNames(0)
    {
        if ( 0 != SetnGetThis() )
        {
            // Test if this is the only instance of CWinApp
            throw CNotSupportedException(_T("Only one instance of CWinApp is permitted"));
        }

        m_dwTlsData = ::TlsAlloc();
        if (m_dwTlsData == TLS_OUT_OF_INDEXES)
        {
            // We only get here in the unlikely event that all TLS indexes are already allocated by this app
            // At least 64 TLS indexes per process are allowed. Win32++ requires only one TLS index.
            throw CNotSupportedException(_T("CWinApp::CWinApp  Failed to allocate Thread Local Storage"));
        }

        SetnGetThis(this);

        // Set the instance handle
#ifdef _WIN32_WCE
        m_hInstance = (HINSTANCE)GetModuleHandle(0);
#else
        MEMORY_BASIC_INFORMATION mbi;
        ZeroMemory(&mbi, sizeof(mbi));
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
        // Deallocate the global memory
        GlobalFreeAll(m_hDevMode);
        GlobalFreeAll(m_hDevNames);

        // Forcibly destroy any remaining windows now. Windows created from
        //  static CWnds or dangling pointers are destroyed here.
        std::map<HWND, CWnd*, CompareHWND>::const_iterator m;
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

        SetnGetThis(0, true);
    }


    // Adds a HDC and CDC_Data* pair to the map.
    inline void CWinApp::AddCDCData(HDC hDC, CDC_Data* pData)
    {
        CThreadLock mapLock(m_csGDILock);
        m_mapCDCData.insert(std::make_pair(hDC, pData));
    }


    // Adds a HGDIOBJ and CGDI_Data* pair to the map.
    inline void CWinApp::AddCGDIData(HGDIOBJ hGDI, CGDI_Data* pData)
    {
        CThreadLock mapLock(m_csGDILock);
        m_mapCGDIData.insert(std::make_pair(hGDI, pData));
    }


    // Adds a HIMAGELIST and Ciml_Data* pair to the map.
    inline void CWinApp::AddCImlData(HIMAGELIST hIml, CIml_Data* pData)
    {
        CThreadLock mapLock(m_csMapLock);
        m_mapCImlData.insert(std::make_pair(hIml, pData));
    }

#ifndef _WIN32_WCE

    // Adds a HMENU and CMenu_Data* to the map.
    inline void CWinApp::AddCMenuData(HMENU hMenu, CMenu_Data* pData)
    {
        CThreadLock mapLock(m_csMapLock);
        m_mapCMenuData.insert(std::make_pair(hMenu, pData));
    }

#endif

    // Free the specified global memory. It also provides a TRACE warning
    // if the global memory is currently locked.
    inline void CWinApp::GlobalFreeAll(HGLOBAL hGlobal)
    {
        if (hGlobal == 0)
            return;

#ifndef _WIN32_WCE
        // check validity of the handle
        assert(::GlobalFlags(hGlobal) != GMEM_INVALID_HANDLE);
        // decrement the lock count associated with the handle
        UINT nCount = ::GlobalFlags(hGlobal) & GMEM_LOCKCOUNT;
        while (nCount--)
        {
            TRACE("***WARNING Global memory still locked ***\n");
            ::GlobalUnlock(hGlobal);
        }
#endif

        // finally, really free the handle
        ::GlobalFree(hGlobal);
    }


    // Retrieves a pointer to CDC_Data from the map
    inline CDC_Data* CWinApp::GetCDCData(HDC hDC)
    {
        std::map<HDC, CDC_Data*, CompareHDC>::const_iterator m;

        // Find the CDC data mapped to this HDC
        CDC_Data* pCDCData = 0;
        CThreadLock mapLock(m_csGDILock);
        m = m_mapCDCData.find(hDC);

        if (m != m_mapCDCData.end())
            pCDCData = m->second;

        return pCDCData;
    }


    // Retrieves a pointer to CGDI_Data from the map
    inline CGDI_Data* CWinApp::GetCGDIData(HGDIOBJ hObject)
    {
        std::map<HGDIOBJ, CGDI_Data*, CompareGDI>::const_iterator m;

        // Find the CGDIObject data mapped to this HGDIOBJ
        CGDI_Data* pCGDIData = 0;
        CThreadLock mapLock(m_csGDILock);
        m = m_mapCGDIData.find(hObject);

        if (m != m_mapCGDIData.end())
            pCGDIData = m->second;

        return pCGDIData;
    }


    // Retrieves a pointer to CIml_Data from the map
    inline CIml_Data* CWinApp::GetCImlData(HIMAGELIST himl)
    {
        std::map<HIMAGELIST, CIml_Data*, CompareHIMAGELIST>::const_iterator m;

        // Find the CImageList data mapped to this HIMAGELIST
        CIml_Data* pCImlData = 0;
        CThreadLock mapLock(m_csMapLock);
        m = m_mapCImlData.find(himl);

        if (m != m_mapCImlData.end())
            pCImlData = m->second;

        return pCImlData;
    }

#ifndef _WIN32_WCE

    // Retrieves a pointer to CMenu_Data from the map
    inline CMenu_Data* CWinApp::GetCMenuData(HMENU hMenu)
    {
        std::map<HMENU, CMenu_Data*, CompareHMENU>::const_iterator m;

        // Find the CMenu data mapped to this HMENU
        CMenu_Data* pCMenuData = 0;
        CThreadLock mapLock(m_csMapLock);
        m = m_mapCMenuData.find(hMenu);

        if (m != m_mapCMenuData.end())
            pCMenuData = m->second;

        return pCMenuData;
    }

#endif

    // Retrieves the CWnd pointer associated with the specified hWnd.
    inline CWnd* CWinApp::GetCWndFromMap(HWND hWnd)
    {
        // Allocate an iterator for our HWND map
        std::map<HWND, CWnd*, CompareHWND>::const_iterator m;

        // Find the CWnd pointer mapped to this HWND
        CWnd* pWnd = 0;
        CThreadLock mapLock(m_csMapLock);
        m = m_mapHWND.find(hWnd);

        if (m != m_mapHWND.end())
            pWnd = m->second;

        return pWnd;
    }


    // Retrieves the pointer to the Thread Local Storage data for the current thread.
    inline TLSData* CWinApp::GetTlsData() const
    {
        return static_cast<TLSData*>(TlsGetValue(m_dwTlsData));
    }


    // InitInstance contains the initialization code for your application
    // You should override this function with the code to run when the application starts.
    // return TRUE to indicate success. FALSE will end the application,
    inline BOOL CWinApp::InitInstance()
    {
        return TRUE;
    }


    // Loads the cursor resource from the resource script (resource.rc)
    inline HCURSOR CWinApp::LoadCursor(LPCTSTR lpszResourceName) const
    {
        return ::LoadCursor(GetResourceHandle(), lpszResourceName);
    }


    // Loads the cursor resource from the resource script (resource.rc)
    inline HCURSOR CWinApp::LoadCursor(int nIDCursor) const
    {
        return ::LoadCursor(GetResourceHandle(), MAKEINTRESOURCE (nIDCursor));
    }


    // Returns the handle of a standard cursor. Standard cursors include:
    // IDC_APPSTARTING, IDC_ARROW, IDC_CROSS, IDC_HAND, IDC_HELP, IDC_IBEAM, IDC_NO, IDC_SIZEALL,
    // IDC_SIZENESW, IDC_SIZENS, IDC_SIZENWSE, IDC_SIZEWE, IDC_UPARROW, IDC_WAIT
    inline HCURSOR CWinApp::LoadStandardCursor(LPCTSTR lpszCursorName) const
    {
        return ::LoadCursor(0, lpszCursorName);
    }


    // Loads the icon resource whose size conforms to the SM_CXICON and SM_CYICON system metric values
    // For other icon sizes, use the LoadImage windows API function.
    inline HICON CWinApp::LoadIcon(LPCTSTR lpszResourceName) const
    {
        return ::LoadIcon(GetResourceHandle(), lpszResourceName);
    }


    // Loads the icon resource whose size conforms to the SM_CXICON and SM_CYICON system metric values
    inline HICON CWinApp::LoadIcon(int nIDIcon) const
    {
        return ::LoadIcon(GetResourceHandle(), MAKEINTRESOURCE (nIDIcon));
    }


    // Returns the handle of a standard Icon. Standard Icons include:
    // IDI_APPLICATION, IDI_ASTERISK, IDI_ERROR, IDI_EXCLAMATION,
    // IDI_HAND, IDI_INFORMATION, IDI_QUESTION, IDI_WARNING
    inline HICON CWinApp::LoadStandardIcon(LPCTSTR lpszIconName) const
    {
        return ::LoadIcon(0, lpszIconName);
    }


    // Loads an icon, cursor, animated cursor, or bitmap image.
    // uType is the image type. It can be IMAGE_BITMAP, IMAGE_CURSOR or IMAGE_ICON.
    // cx and cy are the desired width and height in pixels.
    // fuLoad can be LR_DEFAULTCOLOR, LR_CREATEDIBSECTION, LR_DEFAULTSIZE, LR_LOADFROMFILE,
    // LR_LOADMAP3DCOLORS, R_LOADTRANSPARENT, LR_MONOCHROME, LR_SHARED, LR_VGACOLOR.
    // Ideally the image should be destroyed unless it is loaded with LR_SHARED.
    inline HANDLE CWinApp::LoadImage(LPCTSTR lpszResourceName, UINT uType, int cx, int cy, UINT fuLoad) const
    {
        return ::LoadImage(GetResourceHandle(), lpszResourceName, uType, cx, cy, fuLoad);
    }


    // Loads an icon, cursor, animated cursor, or bitmap.
    // uType is the image type. It can be IMAGE_BITMAP, IMAGE_CURSOR or IMAGE_ICON.
    // cx and cy are the desired width and height in pixels.
    // fuLoad can be LR_DEFAULTCOLOR, LR_CREATEDIBSECTION, LR_DEFAULTSIZE, LR_LOADFROMFILE,
    // LR_LOADMAP3DCOLORS, R_LOADTRANSPARENT, LR_MONOCHROME, LR_SHARED, LR_VGACOLOR.
    // Ideally the image should be destroyed unless it is loaded with LR_SHARED.
    inline HANDLE CWinApp::LoadImage(int nIDImage, UINT uType, int cx, int cy, UINT fuLoad) const
    {
        return ::LoadImage(GetResourceHandle(), MAKEINTRESOURCE (nIDImage), uType, cx, cy, fuLoad);
    }


    // Runs the application and starts the message loop.
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


    // Registers a temporary window class so we can get the callback
    // address of CWnd::StaticWindowProc.
    // This technique works for all Window versions, including WinCE.
    inline void CWinApp::SetCallback()
    {
        WNDCLASS wcDefault;
        ZeroMemory(&wcDefault, sizeof(wcDefault));

        LPCTSTR szClassName     = _T("Win32++ Temporary Window Class");
        wcDefault.hInstance     = GetInstanceHandle();
        wcDefault.lpfnWndProc   = CWnd::StaticWindowProc;
        wcDefault.lpszClassName = szClassName;

        VERIFY(::RegisterClass(&wcDefault) != 0);

        // Retrieve the class information
        ZeroMemory(&wcDefault, sizeof(wcDefault));
        ::GetClassInfo(GetInstanceHandle(), szClassName, &wcDefault);

        // Save the callback address of CWnd::StaticWindowProc
        assert(wcDefault.lpfnWndProc);  // Assert fails when running UNICODE build on ANSI OS.
        m_Callback = wcDefault.lpfnWndProc;
        VERIFY(::UnregisterClass(szClassName, GetInstanceHandle()) != 0);
    }


    // Sets the current cursor and returns the previous one.
    // Note:The cursor will be set to the window's class cursor (if one is set) each time the
    // mouse is moved over the window. You can specify different cursors for different
    // conditions while processing WM_SETCURSOR.
    inline HCURSOR CWinApp::SetCursor(HCURSOR hCursor) const
    {
        return ::SetCursor(hCursor);
    }


    // This function stores the 'this' pointer in a static variable.
    // Once stored, it can be used later to return the 'this' pointer.
    // CWinApp's constructor calls this function and sets the static variable.
    // CWinApp's destructor resets pWinApp to 0.
    inline CWinApp* CWinApp::SetnGetThis(CWinApp* pThis /*= 0*/, bool reset /*= false*/)
    {
        static CWinApp* pWinApp = 0;

        if (pWinApp == 0)
            pWinApp = pThis;
        else
            assert(pThis == 0);

        if (reset)
            pWinApp = 0;

        return pWinApp;
    }


    // This function can be used to load a resource dll.
    // A resource dll can be used to define resources in different languages.
    // To use this function, place code like this in InitInstance
    //   HINSTANCE hResource = LoadLibrary(_T("MyResourceDLL.dll"));
    //   SetResourceHandle(hResource);
    inline void CWinApp::SetResourceHandle(HINSTANCE hResource)
    {
        m_hResource = hResource;
    }


    // Creates the Thread Local Storage data for the current thread if none already exists.
    inline TLSData* CWinApp::SetTlsData()
    {
        TLSData* pTLSData = GetTlsData();
        if (NULL == pTLSData)
        {
            pTLSData = new TLSData;

            CThreadLock TLSLock(m_csAppLock);
            m_vTLSData.push_back(pTLSData); // store as a Shared_Ptr

            ::TlsSetValue(m_dwTlsData, pTLSData);
        }

        return pTLSData;
    }


} // namespace Win32xx

#endif // _WIN32XX_APPCORE_H_

