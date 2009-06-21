#ifndef STDAFX_H
#define STDAFX_H


// Based on code provided by Lynn Allan


#define WIN32_LEAN_AND_MEAN    // Exclude rarely-used stuff from Windows headers

// This avoid warning from vc7.1 about targeting for IE 5
// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#if _MSC_VER > 1200
#define WINVER 0x0400
#endif

#define VC_EXTRALEAN    // Exclude rarely-used stuff from Windows headers

typedef  unsigned char  byte;

#ifdef _MSC_VER
  #pragma warning (disable : 4786)	// identifier was truncated
  #pragma warning (disable : 4702)	// unreachable code (bugs in Microsoft's STL)
#endif

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

#include <dialog.h>
#include <docking.h>
#include <frame.h>
#include <gdi.h>
#include <listview.h>
#include <mdi.h>
#include <propertysheet.h>
#include <rebar.h>
#include <socket.h>
#include <splitter.h>
#include <statusbar.h>
#include <toolbar.h>
#include <treeview.h>
#include <wincore.h>

#ifdef _MSC_VER
  #pragma warning (default : 4786)	// identifier was truncated
  #pragma warning (default : 4702)	// unreachable code (bugs in Microsoft's STL)
#endif

#endif
