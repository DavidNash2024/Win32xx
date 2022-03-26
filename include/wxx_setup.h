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

////////////////////////////////////////////////////////
// This file defines the set of macros and includes the
// C, C++, and windows header files required by Win32++.
// It also defines the CObject class.


#ifndef _WIN32XX_SETUP_H_
#define _WIN32XX_SETUP_H_

// Set the version macros if they aren't already set.
// These values are suitable for Windows 10 and Windows 11.
#ifndef WINVER
  #define WINVER            0x0A00
  #undef  _WIN32_WINNT
  #define _WIN32_WINNT      0x0A00
  #undef  _WIN32_IE
  #define _WIN32_IE         0x0A00
  #undef  NTDDI_VERSION
  #define NTDDI_VERSION     0x0A000000
#endif
#ifndef _WIN32_WINDOWS
  #define _WIN32_WINDOWS    WINVER
#endif
#ifndef _WIN32_IE
  #define _WIN32_IE         WINVER
#endif


// Remove pointless warning messages.
#ifdef _MSC_VER
  #if _MSC_VER < 1310    // before VS2003
    #pragma warning (disable : 4511) // copy operator could not be generated
    #pragma warning (disable : 4512) // assignment operator could not be generated
    #pragma warning (disable : 4702) // unreachable code (bugs in Microsoft's STL)
    #pragma warning (disable : 4786) // identifier was truncated
  #endif
#endif

#ifdef __BORLANDC__
  #pragma option -w-8026            // functions with exception specifications are not expanded inline
  #pragma option -w-8027            // function not expanded inline
  #pragma option -w-8030            // Temporary used for 'rhs'
  #pragma option -w-8004            // Assigned value is never used
  #define STRICT 1
#endif


#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "wxx_shared_ptr.h"

#include <WinSock2.h>   // must include before windows.h
#include <Windows.h>
#include <CommCtrl.h>
#include <Shlwapi.h>

#include <cassert>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <sstream>
#include <stdio.h>
#include <stdarg.h>
#include <tchar.h>
#include <process.h>

// Required by compilers lacking Win64 support.
#ifndef  GetWindowLongPtr
  #define GetWindowLongPtr   GetWindowLong
  #define SetWindowLongPtr   SetWindowLong
  #define GWLP_WNDPROC       GWL_WNDPROC
  #define GWLP_HINSTANCE     GWL_HINSTANCE
  #define GWLP_ID            GWL_ID
  #define GWLP_USERDATA      GWL_USERDATA
  #define DWLP_DLGPROC       DWL_DLGPROC
  #define DWLP_MSGRESULT     DWL_MSGRESULT
  #define DWLP_USER          DWL_USER
  #define DWORD_PTR          DWORD
  #define LONG_PTR           LONG
  #define ULONG_PTR          LONG
#endif
#ifndef GetClassLongPtr
  #define GetClassLongPtr    GetClassLong
  #define SetClassLongPtr    SetClassLong
  #define GCLP_HBRBACKGROUND GCL_HBRBACKGROUND
  #define GCLP_HCURSOR       GCL_HCURSOR
  #define GCLP_HICON         GCL_HICON
  #define GCLP_HICONSM       GCL_HICONSM
  #define GCLP_HMODULE       GCL_HMODULE
  #define GCLP_MENUNAME      GCL_MENUNAME
  #define GCLP_WNDPROC       GCL_WNDPROC
#endif


// Automatically include the Win32xx namespace.
// define NO_USING_NAMESPACE to skip this step.
namespace Win32xx {}
#ifndef NO_USING_NAMESPACE
using namespace Win32xx;
#endif


// Ensure UNICODE and _UNICODE definitions are consistent.
#ifdef _UNICODE
  #ifndef UNICODE
    #define UNICODE
  #endif
#endif

#ifdef UNICODE
  #ifndef _UNICODE
    #define _UNICODE
  #endif
#endif

// Define our own MIN and MAX macros.
// This avoids inconsistencies with MinGW and other compilers, and
// avoids conflicts between typical min/max macros and std::min/std::max
#define MAX(a,b)        (((a) > (b)) ? (a) : (b))
#define MIN(a,b)        (((a) < (b)) ? (a) : (b))

// Version macro
#define _WIN32XX_VER 0x0900     // Win32++ version 9.0.0

// Define the TRACE Macro.
// In debug mode, TRACE send text to the debug/output pane, or an external debugger
// In release mode, TRACE is ignored.
#ifndef TRACE
  #ifdef NDEBUG
    #define TRACE(str) (void(0))
  #else
    #define TRACE(str) Trace(str)
  #endif
#endif

// Define the VERIFY macro
// In debug mode, VERIFY asserts if the expression evaluates to zero
// In release mode, VERIFY evaluates the expression, but doesn't assert.
#ifndef VERIFY
  #ifdef NDEBUG
    #define VERIFY(f) ((void)(f))
  #else
    #define VERIFY(f) assert(f)
  #endif
#endif

namespace Win32xx
{

    class CArchive;

    ////////////////////////////////////////////////////////////////////
    // The CObject class provides support for Serialization by CArchive.
    class CObject
    {
    public:
        CObject() {}
        virtual ~CObject() {}

        virtual void Serialize(CArchive& ar);
    };


    ///////////////////////////////////////
    // Definitions for the CObject class
    //
    inline void CObject::Serialize(CArchive& /* ar */ )
    {
    //  Override Serialize in the class inherited from CObject like this.

    //  if (ar.IsStoring())
    //  {
    //      // Store a member variable in the archive
    //      ar << m_someValue;
    //  }
    //  else
    //  {
    //      // Load a member variable from the archive
    //      ar >> m_someValue;
    //  }

    }

}

#endif // _WIN32XX_SETUP_H_
