//////////////////////////////////////////////
// StdAfx.h

// This file is used for precompiled headers
// Rarely modified header files should be included.

// Based on code provided by Lynn Allan


#ifndef STDAFX_H
#define STDAFX_H


// Specify predefinitions for windows.h here
//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define VC_EXTRALEAN			// Exclude rarely-used stuff from Windows headers
#define WINVER 0x0400			// Minimum OS required is Win95
//#define _WIN32_IE 0x0400		// Minimum version of Internet Explorer required is 4 

// Specify Win32xx specific predifinitions here
#define USE_RIBBON				// Requires Windows 7 SDK and VS2005 Express Edition or above 
//#define NO_USING_NAMESPACE		// Don't use Win32xx namespace

typedef  unsigned char  byte;

#if defined(_MSC_VER) && _MSC_VER == 1200 	// For Visual Studio 6
  #pragma warning (disable : 4786)	// identifier was truncated
  #pragma warning (disable : 4702)	// unreachable code (bugs in Microsoft's STL)
#endif

// Rarely modified header files should be included here
#include <dialog.h>
#include <docking.h>
#include <frame.h>
#include <gdi.h>
#include <listview.h>
#include <mdi.h>
#include <propertysheet.h>
#include <rebar.h>
#include <socket.h>
#include <statusbar.h>
#include <stdcontrols.h>
#include <toolbar.h>
#include <treeview.h>
#include <wincore.h>

#ifdef USE_RIBBON
  #include <ribbon.h>
#endif

#endif
