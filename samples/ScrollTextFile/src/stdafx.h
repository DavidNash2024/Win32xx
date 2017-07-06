/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                 (StdAfx.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2017, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description: Standard Declarations File. This file is used 
	for precompiled headers. Rarely modified header files should be 
	included.  Based on code provided by Lynn Allan to Win32++ sample
	projects, Copyright (c) 2005-2017 David Nash, under permissions
	granted therein.
	
	Adapted for use in this application.

        Caveats: The copyright displayed above extends only to the author's
	original contributions to the subject class, and  to the alterations,
	additions, deletions, and  other treatments of materials that may have
	been extracted from the cited sources.  Unaltered portions of those
	materials retain their original copyright status. The author hereby
	grants permission to any person obtaining a copy of this treatment
	of the subject class and  any associated documentation composed by
	the author, to utilize this material, free of charge and  without
	restriction or limitation, subject to the following conditions:

		The above copyright notice, as well as that of David Nash
		and Win32++, together with the respective permission
		conditions shall be included in all copies or substantial
		portions of this material so copied, modified, merged,
		published, distributed, or otherwise held by others.

	These materials are provided "as is", without warranty of any kind,
	express or implied, including but not limited to: warranties of
	merchantability, fitness for a particular purpose, and non-infringement.
	In no event shall the authors or copyright holders be liable for any
	claim, damages, or other liability, whether in an action of contract,
	tort or otherwise, arising from, out of, or in connection with, these
	materials, the use thereof, or any other other dealings therewith.

	To use precompiled headers, do the following:
	1) Add the set of rarely changed headers to stdafx.h
	2) Include stdafx.h in each cpp file. It must be included first.
	3) Compile stdafx.h into the precompiled hearder stdafx.h.gch by
	   including stdafx.mak into the makefile in the project options:

		all-before: stdafx.h.gch
                stdafx.h.gch: stdafx.h
			$(CPP) -c stdafx.h -o stdafx.h.gch $(CXXFLAGS)

	4) Add stdafx.cpp to your project source files.

	Special Conventions:

	Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	Declaration of the stdafx headers precompiled for this application

*******************************************************************************/

#ifndef STDAFX_H
#define STDAFX_H

/*============================================================================*/

typedef  unsigned char  byte;

// This file is used for precompiled headers
// Rarely modified header files should be included.

// Based on code provided by Lynn Allan

// Predefinitions for windows.h go here
//#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#include "targetver.h"	// Set the supported window features

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
#include <wxx_commondlg.h>
#include <wxx_controls.h>
#include <wxx_cstring.h>
#include <wxx_dialog.h>
#include <wxx_ddx.h>
#include <wxx_docking.h>
#include <wxx_file.h>
#include <wxx_filefind.h>
#include <wxx_frame.h>
#include <wxx_gdi.h>
#include <wxx_imagelist.h>
#include <wxx_listview.h>
#include <wxx_mdi.h>
#include <wxx_metafile.h>
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
/*-----------------------------------------------------------------------------*/
#endif // STDAFX_H
