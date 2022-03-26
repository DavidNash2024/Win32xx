// Win32++   Version 9.0
// Release Date: TBA
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2022  David Nash
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



#ifndef _WIN32XX_THREAD_H_
#define _WIN32XX_THREAD_H_

namespace Win32xx
{

    // typedef for _beginthreadex's callback function.
    typedef UINT(WINAPI* PFNTHREADPROC)(LPVOID);

    ////////////////////////////////////////////////////////////////////
    // CWinThread manages a thread. It supports GUI threads and
    // worker threads. For a GUI thread, it runs the message loop.
    // Use the default constructor to create a GUI thread.
    // Use the constructor that specifies the thread' callback procedure
    //  to create a worker thread.
    class CWinThread : public CMessagePump
    {
    public:
        CWinThread();
        CWinThread(PFNTHREADPROC pfnThreadProc, LPVOID pParam);
        virtual ~CWinThread();

        // Overridables
        BOOL InitInstance();

        // Operations
        HANDLE  CreateThread(unsigned initflag = 0, unsigned stack_size = 0, LPSECURITY_ATTRIBUTES pSecurityAttributes = NULL);
        HWND    GetMainWnd() const;
        HANDLE  GetThread() const;
        int     GetThreadID() const;
        int     GetThreadPriority() const;
        BOOL    IsRunning() const { return (WaitForSingleObject(m_thread, 0) == WAIT_TIMEOUT); }
        BOOL    PostThreadMessage(UINT message, WPARAM wparam, LPARAM lparam) const;
        DWORD   ResumeThread() const;
        void    SetMainWnd(HWND wnd);
        BOOL    SetThreadPriority(int priority) const;
        DWORD   SuspendThread() const;
        operator HANDLE () const { return GetThread(); }

    private:
        CWinThread(const CWinThread&);              // Disable copy construction
        CWinThread& operator = (const CWinThread&); // Disable assignment operator

        static  UINT WINAPI StaticThreadProc(LPVOID pCThread);

        PFNTHREADPROC m_pfnThreadProc;  // Callback function for worker threads
        LPVOID m_pThreadParams;         // Thread parameter for worker threads
        HANDLE m_thread;                // Handle of this thread
        UINT m_threadID;                // ID of this thread
    };

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace Win32xx
{

    ///////////////////////////////////////
    // Definitions for the CWinThread class
    //

    // CWinThread constructor.
    // Override CWinThread and use this constructor for GUI threads.
    // InitInstance will be called when the thread runs.
    inline CWinThread::CWinThread() : m_pfnThreadProc(0), m_pThreadParams(0), m_thread(0),
                                       m_threadID(0)
    {
    }

    // CWinThread constructor.
    // Use CWinThread directly and call this constructor for worker threads.
    // Specify a pointer to the function to run when the thread starts.
    // Specifying pParam for a worker thread is optional.
    inline CWinThread::CWinThread(PFNTHREADPROC pfnThreadProc, LPVOID pParam) : m_pfnThreadProc(0),
                        m_pThreadParams(0), m_thread(0), m_threadID(0)
    {
        m_pfnThreadProc = pfnThreadProc;
        m_pThreadParams = pParam;
    }

    inline CWinThread::~CWinThread()
    {
        if (m_thread)
        {
            // A thread's state is set to signalled when the thread terminates.
            // If your thread is still running at this point, you have a bug.
            if (IsRunning())
            {
                TRACE("*** Warning *** Ending CWinThread before ending its thread\n");
            }

            // Close the thread's handle
            ::CloseHandle(m_thread);
        }
    }

    // Creates a new thread
    // Valid argument values:
    // initflag                 Either CREATE_SUSPENDED or 0
    // stack_size               Either the stack size or 0
    // pSecurityAttributes      Either a pointer to SECURITY_ATTRIBUTES or 0
    // Refer to CreateThread in the Windows API documentation for more information.
    inline HANDLE CWinThread::CreateThread(unsigned initflag /* = 0 */, unsigned stack_size/* = 0 */,
                                           LPSECURITY_ATTRIBUTES pSecurityAttributes /*= NULL*/)
    {
        if (NULL == m_pfnThreadProc) m_pfnThreadProc = CWinThread::StaticThreadProc;
        if (NULL == m_pThreadParams) m_pThreadParams = this;

        // Reusing the CWinThread
        if (m_thread)
        {
            assert(!IsRunning());
            CloseHandle(m_thread);
        }

        m_thread = reinterpret_cast<HANDLE>(::_beginthreadex(pSecurityAttributes, stack_size, m_pfnThreadProc,
                     m_pThreadParams, initflag, &m_threadID));

        if (m_thread == 0)
            throw CWinException(GetApp()->MsgAppThread());

        return m_thread;
    }

    // Retrieves a handle to the main window for this thread.
    // Use SetMainWnd to set this threads main window.
    inline HWND CWinThread::GetMainWnd() const
    {
        TLSData* pTLSData = GetApp()->GetTlsData();

        // Will assert if the thread doesn't have TLSData assigned.
        // TLSData is assigned when the first window in the thread is created.
        assert (pTLSData);

        return pTLSData ? pTLSData->mainWnd : 0;
    }

    // Retrieves the handle of this thread.
    inline HANDLE CWinThread::GetThread() const
    {
        return m_thread;
    }

    // Retrieves the thread's ID.
    inline int CWinThread::GetThreadID() const
    {
        assert(m_thread);

        return m_threadID;
    }

    // Retrieves this thread's priority
    // Refer to GetThreadPriority in the Windows API documentation for more information.
    inline int CWinThread::GetThreadPriority() const
    {
        assert(m_thread);
        return ::GetThreadPriority(m_thread);
    }

    // Override this function to perform tasks when the thread starts.
    // return TRUE to run a message loop, otherwise return FALSE.
    // A thread with a window must run a message loop.
    inline BOOL CWinThread::InitInstance()
    {
        return FALSE;
    }


/*    // This function manages the way window message are dispatched
    // to a window procedure.
    inline int CWinThread::MessageLoop()
    {
        MSG msg;
        ZeroMemory(&msg, sizeof(msg));
        int status = 1;
        LONG lCount = 0;

        while (status != 0)
        {
            // While idle, perform idle processing until OnIdle returns FALSE
            while (!::PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE) && OnIdle(lCount) != FALSE)
                ++lCount;

            lCount = 0;

            // Now wait until we get a message
            if ((status = ::GetMessage(&msg, NULL, 0, 0)) == -1)
                return -1;

            if (!PreTranslateMessage(msg))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }

        }

        return LOWORD(msg.wParam);
    } */

/*    // This function is called by the MessageLoop. It is called when the message queue
    // is empty. Return TRUE to continue idle processing or FALSE to end idle processing
    // until another message is queued. The count is incremented each time OnIdle is
    // called, and reset to 0 each time a new messages is processed.
    inline BOOL CWinThread::OnIdle(LONG )
    {
        return FALSE;
    }

    // This function is called by the MessageLoop. It processes the
    // keyboard accelerator keys and calls CWnd::PreTranslateMessage for
    // keyboard and mouse events.
    inline BOOL CWinThread::PreTranslateMessage(MSG& msg)
    {
        BOOL isProcessed = FALSE;

        // only pre-translate mouse and keyboard input events
        if ((msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST) ||
            (msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST))
        {
            // Search the chain of parents for pretranslated messages.
            for (HWND wnd = msg.hwnd; wnd != 0; wnd = ::GetParent(wnd))
            {
                CWnd* pWnd = GetApp()->GetCWndFromMap(wnd);
                if (pWnd)
                {
                    isProcessed = pWnd->PreTranslateMessage(msg);
                    if (isProcessed)
                        break;
                }
            }
        }

        return isProcessed;
    } */

    // Posts a message to the thread. The message will reach the MessageLoop, but
    // will not call a CWnd's WndProc.
    // Refer to PostThreadMessage in the Windows API documentation for more information.
    inline BOOL CWinThread::PostThreadMessage(UINT msg, WPARAM wparam, LPARAM lparam) const
    {
        assert(m_thread);
        return ::PostThreadMessage(GetThreadID(), msg, wparam, lparam);
    }

    // Resumes a thread that has been suspended, or created with the CREATE_SUSPENDED flag.
    // Refer to ResumeThread in the Windows API documentation for more information.
    inline DWORD CWinThread::ResumeThread() const
    {
        assert(m_thread);
        return ::ResumeThread(m_thread);
    }

    // Sets the main window for this thread.
    inline void CWinThread::SetMainWnd(HWND wnd)
    {
        TLSData* pTLSData = GetApp()->SetTlsData();
        pTLSData->mainWnd = wnd;
    }

    // Sets the priority of this thread. The nPriority parameter can
    // be -7, -6, -5, -4, -3, 3, 4, 5, or 6 or other values permitted
    // by the SetThreadPriority Windows API function.
    // Refer to SetThreadPriority in the Windows API documentation for more information.
    inline BOOL CWinThread::SetThreadPriority(int priority) const
    {
        assert(m_thread);
        return ::SetThreadPriority(m_thread, priority);
    }

    // Suspends this thread. Use ResumeThread to resume the thread.
    // Refer to SuspendThread in the Windows API documentation for more information.
    inline DWORD CWinThread::SuspendThread() const
    {
        assert(m_thread);
        return ::SuspendThread(m_thread);
    }

    // When the GUI thread starts, it runs this function.
    inline UINT WINAPI CWinThread::StaticThreadProc(LPVOID pCThread)
    {
        // Get the pointer for this CWinThread object
        CWinThread* pThread = static_cast<CWinThread*>(pCThread);
        assert(pThread != 0);

        if ((pThread != 0) && (pThread->InitInstance()))
        {
            GetApp()->SetTlsData();
            return pThread->MessageLoop();
        }

        return 0;
    }

}

#endif // _WIN32XX_THREAD_H_
