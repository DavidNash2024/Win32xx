// Win32++   Version 10.2.0
// Release Date: 20th September 2025
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//           https://github.com/DavidNash2024/Win32xx
//
//
// Copyright (c) 2005-2025  David Nash
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



#ifndef _WIN32XX_MUTEX_H_
#define _WIN32XX_MUTEX_H_


///////////////////////////////////////////////////////
// wxx_mutex.h
// This file contains the declarations of the following set of classes.
//
// 1) CEvent: Creates a named or unnamed event. Use the SetEvent function to
//            set the state of an event object to signalled. Use the ResetEvent
//            function to reset the state of an event object to non-signalled.
//            Threads can check the status of the event with one of the wait
//            functions. When the state of an auto-reset event object is
//            signalled, it remains signalled until a single waiting thread is
//            released. The system then automatically resets the state to
//            non-signalled. If no threads are waiting, the event object's
//            state remains signalled.
//
// 2) CMutex: Creates a named or unnamed mutex. Threads use one of the wait
//            functions to request ownership of the mutex. The state of a mutex
//            object is signalled when it is not owned by any thread. Threads
//            can check the status of the event with one of the wait functions.
//
// 3) CSemaphore: Creates a named or unnamed semaphore. The state of a
//            semaphore object is signalled when its count is greater than
//            zero, and non-signalled when its count is equal to zero. The
//            initialCount parameter specifies the initial count.
//            Each time a waiting thread is released because of the semaphore's
//            signalled state, the count of the semaphore is decreased by one.
//            Threads can check the status of the event with one of the wait
//            functions.
//


namespace Win32xx
{
    ///////////////////////////////////////////////////////////////////
    // CEvent manages an event object. Event objects can be set to
    // a signalled or non-signalled state to facilitate synchronization
    // between threads.
    class CEvent
    {
    public:
        CEvent(BOOL isInitiallySignaled = FALSE, BOOL isManualReset = FALSE,
            LPCTSTR name = nullptr, LPSECURITY_ATTRIBUTES attributes = nullptr);

        HANDLE GetHandle() const { return m_event; }
        operator HANDLE() const  { return m_event; }

        void ResetEvent();
        void SetEvent();

    private:
        CEvent(const CEvent&) = delete;
        CEvent& operator=(const CEvent&) = delete;

        HANDLE m_event;
    };

    /////////////////////////////////////////////////////////
    // CMutex manages a mutex object. A mutex object is a
    // synchronization object whose state is set to signalled
    // when it is not owned by any thread, and non-signalled
    // when it is owned. Only one thread at a time can own
    // a mutex object.
    class CMutex
    {
    public:
        CMutex(BOOL isInitiallySignaled = FALSE, LPCTSTR name = nullptr,
            LPSECURITY_ATTRIBUTES pAttributes = nullptr);

        HANDLE GetHandle() const { return m_mutex; }
        operator HANDLE() const  { return m_mutex; }

    private:
        CMutex(const CMutex&) = delete;
        CMutex& operator=(const CMutex&) = delete;

        HANDLE m_mutex;
    };

    ///////////////////////////////////////////////////////////////
    // CSemaphore manages a semaphore object. A semaphore object
    // is a synchronization object that maintains a count between
    // zero and a specified maximum value. The count is decremented
    // each time a thread completes a wait for the semaphore object
    // and incremented each time a thread releases the semaphore.
    class CSemaphore
    {
    public:
        CSemaphore(LONG initialCount, LONG maxCount, LPCTSTR name,
            LPSECURITY_ATTRIBUTES attributes = nullptr);

        HANDLE GetHandle() const { return m_semaphore; }
        operator HANDLE() const  { return m_semaphore; }
        BOOL ReleaseSemaphore(LONG releaseCount, LONG* pPreviousCount = nullptr);

    private:
        CSemaphore(const CSemaphore&) = delete;
        CSemaphore& operator=(const CSemaphore&) = delete;

        HANDLE m_semaphore;
    };


    //////////////////////////////////////
    // CEvent member function definitions.
    //

    // Creates a named or unnamed event.
    // Parameters:
    //  isInitiallySignaled - TRUE the initial state of the created event is signalled, FALSE otherwise
    //  isManualReset  - TRUE requires the use of the ResetEvent function to set the event state to non-signalled.
    //                 - FALSE the event is automatically reset to non-signalled after a single waiting thread has been released.
    //  name           - pointer to a null terminated string specifying the event's name. Can be nullptr.
    //                 - If name matches an existing event, the existing handle is retrieved.
    //  attributes     - Pointer to a SECURITY_ATTRIBUTES structure that determines whether the returned
    //                   handle can be inherited by child processes. If attributes is nullptr, the
    //                   handle cannot be inherited.
    inline CEvent::CEvent(BOOL isInitiallySignaled, BOOL isManualReset, LPCTSTR name,
                    LPSECURITY_ATTRIBUTES attributes)
    : m_event(nullptr)
    {
        m_event = ::CreateEvent(attributes, isManualReset, isInitiallySignaled, name);
        if (m_event == nullptr)
            throw CResourceException(GetApp()->MsgMtxEvent());
    }

    // Sets the specified event object to the non-signalled state.
    inline void CEvent::ResetEvent()
    {
        VERIFY(::ResetEvent(m_event));
    }

    // Sets the specified event object to the signalled state.
    inline void CEvent::SetEvent()
    {
        VERIFY(::SetEvent(m_event));
    }


    //////////////////////////////////////
    // CMutex member function definitions.
    //

    // Creates a named or unnamed mutex.
    // Parameters:
    //  isInitiallySignaled - TRUE the initial state of the created mutex is signalled, FALSE otherwise
    //  name           - pointer to a null terminated string specifying the mutex's name. Can be nullptr.
    //                 - If name matches an existing mutex, the existing handle is retrieved.
    //  attributes     - Pointer to a SECURITY_ATTRIBUTES structure that determines whether the returned
    //                   handle can be inherited by child processes. If attributes is nullptr, the
    //                   handle cannot be inherited.
    inline CMutex::CMutex(BOOL isInitiallySignaled, LPCTSTR name,
                            LPSECURITY_ATTRIBUTES attributes)
    : m_mutex(nullptr)
    {
        m_mutex = ::CreateMutex(attributes, isInitiallySignaled, name);
        if (m_mutex == nullptr)
            throw CResourceException(GetApp()->MsgMtxMutex());
    }


    //////////////////////////////////////
    // CMutex member function definitions.
    //

    // Creates a named or unnamed semaphore.
    // Parameters:
    //  initialCount   - Initial count for the semaphore object. This value must be greater than or equal
    //                   to zero and less than or equal to lMaximumCount.
    //  maxCount       - Maximum count for the semaphore object. This value must be greater than zero.
    //  attributes     - Pointer to a SECURITY_ATTRIBUTES structure that determines whether the returned
    //                   handle can be inherited by child processes. If attributes is nullptr, the
    //                   handle cannot be inherited.
    inline CSemaphore::CSemaphore(LONG initialCount, LONG maxCount, LPCTSTR name,
                            LPSECURITY_ATTRIBUTES attributes)
    : m_semaphore(nullptr)
    {
        assert(maxCount > 0);
        assert(initialCount <= maxCount);

        m_semaphore = ::CreateSemaphore(attributes, initialCount, maxCount, name);
        if (m_semaphore == nullptr)
            throw CResourceException(GetApp()->MsgMtxSemaphore());
    }

    // Increases the count of the specified semaphore object by a specified amount.
    // Parameters:
    //  releaseCount   - Amount by which the semaphore object's current count is to be increased.
    //                   must be greater than zero.
    //  pPreviousCount - pointer to a variable to receive the previous count.
    inline BOOL CSemaphore::ReleaseSemaphore(LONG releaseCount, LONG* pPreviousCount)
    {
        BOOL result = ::ReleaseSemaphore(m_semaphore, releaseCount, pPreviousCount);
        return result;
    }

}


#endif // _WIN32XX_MUTEX_H_

