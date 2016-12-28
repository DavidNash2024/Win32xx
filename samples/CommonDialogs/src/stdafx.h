/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                 (StdAfx.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description: Standard Declarations File. This file is used 
	for precompiled headers. Rarely modified header files should be 
	included.  Based on code provided by Lynn Allan to Win32++ sample
	projects, Copyright (c) 2005-2016 David Nash, under permissions
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

#include <wxx_appcore.h>		// Add CCriticalSection, CObject, CWinThread, CWinApp
#include <wxx_archive.h>		// Add CArchive
#include <wxx_commondlg.h>		// Add CCommonDialog, CColorDialog, CFileDialog, CFindReplace, CFontDialog 
#include <wxx_controls.h>		// Add CAnimation, CComboBox, CComboBoxEx, CDateTime, CHeader, CHotKey, CIPAddress, CProgressBar, CSpinButton, CScrollBar, CSlider, CToolTip
#include <wxx_cstring.h>		// Add CString, CStringA, CStringW
#include <wxx_ddx.h>			// Add CDataExchange
#include <wxx_dialog.h>			// Add CDialog, CResizer
#include <wxx_dockframe.h>		// Add CDockFrame, CMDIDockFrame
#include <wxx_docking.h>		// Add CDocker, CDockContainer
#include <wxx_exception.h>		// Add CException, CFileException, CNotSupportedException, CResourceException, CUserException, CWinException
#include <wxx_file.h>			// Add CFile
#include <wxx_frame.h>			// Add CFrame
#include <wxx_gdi.h>			// Add CDC, CGDIObject, CBitmap, CBrush, CFont, CPalatte, CPen, CRgn
#include <wxx_imagelist.h>		// Add CImageList
#include <wxx_listview.h>		// Add CListView
#include <wxx_mdi.h>			// Add CMDIChild, CMDIFrame, CDockMDIFrame
#include <wxx_printdialogs.h>	// Add CPageSetupDialog, CPrintSetupDialog
#include <wxx_propertysheet.h>	// Add CPropertyPage, CPropertySheet
#include <wxx_rebar.h>			// Add CRebar
#include <wxx_regkey.h>			// Add CRegKey
//#include <wxx_ribbon.h>		// Add CRibbon, CRibbonFrame
#include <wxx_richedit.h>		// Add CRichEdit
#include <wxx_shared_ptr.h>		// Add Shared_Ptr
#include <wxx_socket.h>			// Add CSocket
#include <wxx_statusbar.h>		// Add CStatusBar
#include <wxx_stdcontrols.h>	// Add CButton, CEdit, CListBox
#include <wxx_tab.h>			// Add CTab, CTabbedMDI
//#include <wxx_taskdialog.h>	// Add CTaskDialog
#include <wxx_time.h>			// Add CTime
#include <wxx_toolbar.h>		// Add CToolBar
#include <wxx_treeview.h>		// Add CTreeView
#include <wxx_webbrowser.h>		// Add CAXWindow, CWebBrowser
#include <wxx_wincore.h>		// Add CWnd
/*-----------------------------------------------------------------------------*/
#endif // STDAFX_H
