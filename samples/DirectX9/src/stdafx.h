//////////////////////////////////////////////
// StdAfx.h

// This file is used for precompiled headers
// Rarely modified header files should be included.

// Based on code provided by Lynn Allan


#ifndef STDAFX_H
#define STDAFX_H


// Predefinitions for windows.h go here
//#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#include "targetver.h"			// Set the supported window features

// Specify Win32xx specific predefinitions here
//#define NO_USING_NAMESPACE		// Don't use Win32xx namespace

#if defined(_MSC_VER) && _MSC_VER == 1200 	// For Visual Studio 6
  #pragma warning (disable : 4786)	// identifier was truncated
  #pragma warning (disable : 4702)	// unreachable code (bugs in Microsoft's STL)
#endif

// Rarely modified header files should be included here
#include <vector>
#include <map>
#include <string>
#include <sstream>		// Add support for stringstream
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>

#include <wxx_archive.h>
#include <wxx_controls.h>
#include <wxx_cstring.h>
#include <wxx_dialog.h>
#include <wxx_docking.h>
#include <wxx_file.h>
#include <wxx_frame.h>
#include <wxx_gdi.h>
#include <wxx_imagelist.h>
#include <wxx_listview.h>
#include <wxx_mdi.h>
#include <wxx_propertysheet.h>
#include <wxx_rebar.h>
#include <wxx_regkey.h>
//#include <wxx_ribbon.h>
#include <wxx_richedit.h>
#include <wxx_shared_ptr.h>
#include <wxx_socket.h>
#include <wxx_statusbar.h>
#include <wxx_stdcontrols.h>
#include <wxx_tab.h>
//#include <wxx_taskdialog.h>
#include <wxx_time.h>
#include <wxx_toolbar.h>
#include <wxx_treeview.h>
#include <wxx_webbrowser.h>
#include <wxx_wincore.h>


#endif
