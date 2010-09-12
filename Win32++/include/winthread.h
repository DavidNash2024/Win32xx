// Win32++  Version 7.0 (Prerelease version)
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2010  David Nash
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


// The CWinThread class simplifies the use of threads with Win32++.
// To use threads in your Win32++ application, inherit a class from
// CWinThread, and override InitInstance. When your class is instanciated,
// a new thread is started, and the InitInstance function is called to
// run in the new thread.

// If your thread is used to run one or more windows, InitInstance should 
// return TRUE, causing the MessageLoop function to be called. If your
// thread doesn't require a MessageLoop, it should return FALSE. Threads
// which don't run a message loop as sometimes referred to as "worker" threads.

// Note: It is your job to end the thread before CWinThread ends!
//       To end a thread with a message loop, post a WM_QUIT message to the thread.
//       To end a thread without a message loop, set an event, and end the thread 
//       when the event is received.

// Hint: It is never a good idea to use things like TerminateThread or ExitThread to 
//       end your thread. These represent poor programming techniques, and are likely 
//       to leak memory and resources.

// More Hints for thread programming:
// 1) Avoid using SendMessage between threads, as this will cause one thread to wait for
//    the other to respond. Use PostMessage between threads to avoid this problem.
// 2) Access to variables and resources shared between threads need to be made thread safe.
//    Having one thread modify a resouce or variable while another thread is accessing it is
//    a recipe for disaster.
// 3) Thread Local Storage (TLS) can be used to replace global variables to make them thread
//    safe. With TLS, each thread gets its own copy of the variable.
// 4) Critical Sections can be used to make shared resources thread safe.
// 5) Window messages (including user defined messages) can be posted between GUI threads to
//    communicate information between them.
// 6) Events (created by CreateEvent) can be used to comunicate information between threads 
//    (both GUI and worker threads).
// 7) Avoid using sleep to synchronise threads. Generally speaking, the various wait 
//    functions (e.g. WaitForSingleObject) will be better for this.

// About Threads:
// Each program that executes has a "process" allocated to it. A process has one or more
// threads. Threads run independantly of each other. It is the job of the operating system
// to manage the running of the threads, and do the task switching between threads as required.
// Systems with multiple CPUs will be able to run as many threads simultaneously as there are
// CPUs. 

// Threads behave like a program within a program. When the main thread starts, the application 
// runs the WinMain function and ends when WinMain ends. When another thread starts, it too
// will run the function provided to it, and end when that function ends.


#ifndef _WINTHREAD_H_
#define _WINTHREAD_H_


namespace Win32xx
{

	//////////////////////////////////////
	// Declaration of the CWinThread class
	//
	class CWinThread
	{
	public:
		CWinThread();
		CWinThread(LPSECURITY_ATTRIBUTES pSecurityAttributes, unsigned stack_size, unsigned initflag);
		virtual ~CWinThread();
		
		// Overridables
		virtual BOOL InitInstance();
		virtual int MessageLoop();

		// Operations
		HANDLE	GetThread()	const;
		int		GetThreadID() const;
		int		GetThreadPriority() const;
		DWORD	ResumeThread() const;
		BOOL	SetThreadPriority(int nPriority) const;
		DWORD	SuspendThread() const;

	private:
		CWinThread(const CWinThread&);				// Disable copy construction
		CWinThread& operator = (const CWinThread&);	// Disable assignment operator
		void CreateThread(LPSECURITY_ATTRIBUTES pSecurityAttributes, unsigned stack_size, unsigned initflag);
		static	UINT WINAPI StaticThreadCallback(LPVOID pCThread);

		HANDLE m_hThread;			// Handle of this thread
		UINT m_nThreadID;			// ID of this thread
	};
	
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace Win32xx
{

	///////////////////////////////////////
	// Definitions for the CWinThread class
	//
	inline CWinThread::CWinThread() : m_hThread(0), m_nThreadID(0)
	{
		CreateThread(0, 0, CREATE_SUSPENDED);
	}

	inline CWinThread::CWinThread(LPSECURITY_ATTRIBUTES pSecurityAttributes, unsigned stack_size, unsigned initflag)
		: m_hThread(0), m_nThreadID(0)
										
	{
		// Valid argument values:
		// pSecurityAttributes		Either a pointer to SECURITY_ATTRIBUTES or 0
		// stack_size				Either the stack size or 0
		// initflag					Either CREATE_SUSPENDED or 0
		
		CreateThread(pSecurityAttributes, stack_size, initflag);
	}

	inline CWinThread::~CWinThread()
	{	
		// A thread's state is set to signalled when the thread terminates.
		// If your thread is still running at this point, you have a bug.
		if (0 != WaitForSingleObject(m_hThread, 0))
			TRACE(_T("*** Error *** Ending CWinThread before ending its thread\n"));

		// Close the thread's handle
		::CloseHandle(m_hThread);
	}

	inline void CWinThread::CreateThread(LPSECURITY_ATTRIBUTES pSecurityAttributes, unsigned stack_size, unsigned initflag)
	{
		// NOTE:  By default, the thread is created in the default state.
		//		  _beginthreadex will be undefined if a single-threaded run-time library is used. Use a Multithreaded run-time.
		m_hThread = (HANDLE)_beginthreadex(pSecurityAttributes, stack_size, CWinThread::StaticThreadCallback, (LPVOID) this, initflag, &m_nThreadID);

		if (0 == m_hThread)
			throw CWinException(_T("Failed to create thread"));
	}

	inline HANDLE CWinThread::GetThread() const
	{
		assert(m_hThread);
		return m_hThread;
	}

	inline int CWinThread::GetThreadID() const 
	{
		assert(m_hThread);
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
		// Override this function if your thread needs a different message loop
		return GetApp()->MessageLoop();
	}

	inline DWORD CWinThread::ResumeThread() const
	{
		assert(m_hThread);
		return ::ResumeThread(m_hThread);
	}

	inline DWORD CWinThread::SuspendThread() const
	{
		assert(m_hThread);
		return ::SuspendThread(m_hThread); 
	}
	
	inline BOOL CWinThread::SetThreadPriority(int nPriority) const
	{
		assert(m_hThread);
		return ::SetThreadPriority(m_hThread, nPriority);
	}

	inline UINT WINAPI CWinThread::StaticThreadCallback(LPVOID pCThread)
	// When the thread starts, it runs this function.
	{
		// Get the pointer for this CMyThread object
		CWinThread* pThread = (CWinThread*)pCThread;

		if (pThread->InitInstance())
			return pThread->MessageLoop();

		return 0;
	}
	
}

#endif // #define _WINTHREAD_H_

