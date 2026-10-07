// Win32++   Version 10.4.0
// Release Date: TBA
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//           https://github.com/DavidNash2024/Win32xx
//
//
// Copyright (c) 2005-2026  David Nash
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

#pragma once

#include <mutex>

namespace Win32xx
{
    // CCriticalSection emulates a Windows Critical Section. A recursive_mutex
    // allows the same thread to be locked multiple times.
    using CCriticalSection = std::recursive_mutex;

    // CThreadLock is a RAII wrapper that provides automatic, scope-based locking
    // and unlocking.
    using CThreadLock = std::lock_guard<std::recursive_mutex>;
}

// This code demonstrates how to use CCriticalSection and CThreadLock.
//  
// class MyClass
// {
// public:
//     MyClass() = default;
//     ~MyClass() = default;
//     void SomeFunction()
//     {
//        // Lock this code, preventing other threads from running it until the lock
//        // is released. The lock is automatically released when Lock goes out of scope.  
//        CThreadLock Lock(m_cs);  // m_cs is a CCriticalSection member variable.
//
//        // Do something that requires thread safety.
//        DoSomething(); 
//     }
// private:
//     CCriticalSection m_cs;
// };
