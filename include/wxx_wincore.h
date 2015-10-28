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


///////////////////////////////////////////////////////
// wxx_wincore.h
//  This file includes all the files necessary to start Win32++ and create
//  simple windows. Refer to the sample code at the end of this file.
//


#ifndef _WIN32XX_WINCORE_H_
#define _WIN32XX_WINCORE_H_

// Version macro
#define _WIN32XX_VER 0x0810		// Win32++ version 8.1.0


// Automatically include the Win32xx namespace
// define NO_USING_NAMESPACE to skip this step
namespace Win32xx {}
#ifndef NO_USING_NAMESPACE
  using namespace Win32xx;
#endif


// Define our own VERIFY macro
// In debug mode, VERIFY asserts if the expression evaluates to zero
// In release mode, VERIFY evaluates the expression, but doesn't assert.
#ifndef _WIN32_WCE
  #ifndef VERIFY
    #ifndef NDEBUG
      #define VERIFY(f) assert(f)
    #else
      #define VERIFY(f) ((void)(f))
    #endif
  #endif
#endif // _WIN32_WCE

// Ensure UNICODE and _UNICODE definitions are consistent
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



//////////////////////////////////////
//  Include the Win32++ header files

#include "wxx_shared_ptr.h"
#include "wxx_appcore1.h"
#include "wxx_rect.h"
#include "wxx_textconv.h"
#include "wxx_themes.h"
#include "wxx_cstring.h"
#include "wxx_wincore1.h"
#ifndef _WIN32_WCE
  #include "wxx_archive.h"
#endif
#include "wxx_gdi.h"
#ifndef _WIN32_WCE
  #include "wxx_menu.h"
#endif
#include "wxx_imagelist.h"
#include "wxx_ddx.h"
#include "wxx_appcore2.h"
#include "wxx_wincore2.h"



// Example code
/*

///////////////////////////////////
// main.cpp

// To configure your project:
// 1) Add Win32++'s include directory to the project's additional include directories for C/C++
// 2) Add Win32++'s include directory to the project's additional include directories for Resources

#include "wxx_wincore.h"


// A class that inherits from CWnd. It is used to create the window.
class CView : public CWnd
{
public:
    CView() {}
    virtual ~CView() {}
    virtual void OnDestroy() { PostQuitMessage(0); }	// Ends the program
};



int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // Start Win32++
    CWinApp theApp;

    // Create our view window
    CView MyWin;
    MyWin.Create();

    // Run the application's message loop
    return theApp.Run();
}

*/


#endif // _WIN32XX_WINCORE_H_

