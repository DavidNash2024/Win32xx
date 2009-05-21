// Acknowledgements:
// Lynn Allan for her assistance with pre-compiled headers for Win32++


#ifndef STDAFX_H
#define STDAFX_H

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

// Set minimum values for Windows macros. Adjust these as required.
#if _MSC_VER > 1200
#define WINVER 0x0400
#define _WIN32_WINNT 0x0400
#endif

#define VC_EXTRALEAN      // Exclude rarely-used stuff from Windows headers
#define _WINSOCKAPI_      // Prevent winsock.h #include's.

// Remove pointless warning messages for MS compilers prior to VS 2008 
#if defined (_MSC_VER) && _MSC_VER <= 1500
  #pragma warning (disable : 4511) // copy operator could not be generated
  #pragma warning (disable : 4512) // assignment operator could not be generated
  #pragma warning (disable : 4702) // unreachable code (bugs in Microsoft's STL)
  #pragma warning (disable : 4786) // identifier was truncated
  #pragma warning (disable : 4996) // function or variable may be unsafe (deprecated)
#endif // _MSC_VER

#include <vector>
#include <map>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <assert.h>
#include <windows.h>
#include <commctrl.h>
#include <tchar.h>

#include "dialog.h"
#include "docking.h"
#include "frame.h"
#include "gdi.h"
#include "listview.h"
#include "mdi.h"
#include "propertysheet.h"
#include "rebar.h"
#include "socket.h"
#include "splitter.h"
#include "statusbar.h"
#include "toolbar.h"
#include "treeview.h"
#include "wincore.h"

#endif
