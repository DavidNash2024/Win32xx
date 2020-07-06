/* [Tab/Indent: 8/8][Line/Box: 80/74]                             (stdafx.h) *
********************************************************************************

    Contents Description: Declaration of the Win32++ classes for a sample
    program demonstrating the use of the status bar using the Win32++ 
    Windows framework, Copyright (c) 2005-2017 David Nash.

    This particular program was adapted from the StatusBar sample progam
    in the Win32++ distribution by Robert C. Tausworthe to illustrate the 
    definition of the status bar makeup by way of a data structure, in a
    similar fashion as that appearing in the Microsoft Foundation Classes 
    (MFC), and to override the Win32++ normal statusbar definition of four 
    parts to any number suitable for an application.

    The code changes to the StatusBar sample program only occur in the 
    CMainFrame and MyStatusBar classes. However, the entire program has
    been reformatted for better readability, per the Programming Notes,
    below.

    These materials are provided "as is", without warranty of any kind,
    express or implied, including but not limited to: warranties of
    merchantability, fitness for a particular purpose, and non-infringement.

    Programming Notes:
                The programming conventions used here roughly follow those 
        established by the 1997-1999 Jet Propulsion Laboratory Deep 
        Space Network Planning and Preparation Subsystem project for 
        C++ programming.

********************************************************************************

    This file is used for precompiled headers. Rarely modified header 
    files should be included.

    Based on code provided by Lynn Allan

*-----------------------------------------------------------------------------*/

#ifndef STDAFX_H
#define STDAFX_H


// Predefinitions for windows.h go here
//#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers
#include "targetver.h"          // Set the supported window features

// Specify Win32xx specific predefinitions here
//#define NO_USING_NAMESPACE        // Don't use Win32xx namespace

#if defined(_MSC_VER) && _MSC_VER == 1200   // For Visual Studio 6
  #pragma warning (disable : 4786)  // identifier was truncated
  #pragma warning (disable : 4702)  // unreachable code (bugs in Microsoft's STL)
#endif

// Rarely modified header files should be included here
#include <vector>               // Add support for std::vector
#include <map>                  // Add support for std::map
#include <string>               // Add support for std::string
#include <sstream>              // Add support for stringstream
#include <cassert>              // Add support for the assert macro
#include <stdio.h>              // Add support for C style printf, sprintf, etc.
#include <stdlib.h>             // Add support for C style character conversions atoi etc. 
#include <tchar.h>              // Add support for C style TCHARs.

#include <wxx_appcore.h>        // Add CCriticalSection, CObject, CWinThread, CWinApp
#include <wxx_archive.h>        // Add CArchive
#include <wxx_commondlg.h>      // Add CCommonDialog, CColorDialog, CFileDialog, CFindReplace, CFontDialog 
#include <wxx_controls.h>       // Add CAnimation, CComboBox, CComboBoxEx, CDateTime, CHeader, CHotKey, CIPAddress, CProgressBar, CSpinButton, CScrollBar, CSlider, CToolTip
#include <wxx_cstring.h>        // Add CString, CStringA, CStringW
#include <wxx_ddx.h>            // Add CDataExchange
#include <wxx_dialog.h>         // Add CDialog, CResizer
#include <wxx_dockframe.h>      // Add CDockFrame, CMDIDockFrame
#include <wxx_docking.h>        // Add CDocker, CDockContainer
#include <wxx_exception.h>      // Add CException, CFileException, CNotSupportedException, CResourceException, CUserException, CWinException
#include <wxx_file.h>           // Add CFile
#include <wxx_filefind.h>       // Add CFileFind
#include <wxx_folderdialog.h>   // Add CFolderDialog
#include <wxx_frame.h>          // Add CFrame
#include <wxx_gdi.h>            // Add CDC, CGDIObject, CBitmap, CBrush, CFont, CPalatte, CPen, CRgn
#include <wxx_imagelist.h>      // Add CImageList
#include <wxx_listview.h>       // Add CListView
#include <wxx_mdi.h>            // Add CMDIChild, CMDIFrame, CDockMDIFrame
#include <wxx_menu.h>           // Add CMenu
#include <wxx_menubar.h>        // Add CMenuBar
#include <wxx_metafile.h>       // Add CMetaFile, CEnhMetaFile
#include <wxx_mutex.h>          // Add CEvent, CMutex, CSemaphore
#include <wxx_preview.h>        // Add CPrintPreview
#include <wxx_printdialogs.h>   // Add CPageSetupDialog, CPrintSetupDialog
#include <wxx_propertysheet.h>  // Add CPropertyPage, CPropertySheet
#include <wxx_rebar.h>          // Add CRebar
#include <wxx_rect.h>           // Add CPoint, CRect, CSize
#include <wxx_regkey.h>         // Add CRegKey
//#include <wxx_ribbon.h>       // Add CRibbon, CRibbonFrame
#include <wxx_richedit.h>       // Add CRichEdit
#include <wxx_scrollview.h>     // Add CScrollView
#include <wxx_shared_ptr.h>     // Add Shared_Ptr
#include <wxx_socket.h>         // Add CSocket
#include <wxx_statusbar.h>      // Add CStatusBar
#include <wxx_stdcontrols.h>    // Add CButton, CEdit, CListBox
#include <wxx_tab.h>            // Add CTab, CTabbedMDI
//#include <wxx_taskdialog.h>   // Add CTaskDialog
#include <wxx_textconv.h>       // Add AtoT, AtoW, TtoA, TtoW, WtoA, WtoT etc.
#include <wxx_themes.h>         // Add MenuTheme, ReBarTheme, StatusBarTheme, ToolBarTheme
#include <wxx_time.h>           // Add CTime
#include <wxx_toolbar.h>        // Add CToolBar
#include <wxx_treeview.h>       // Add CTreeView
#include <wxx_webbrowser.h>     // Add CWebBrowser, CAXHost
#include <wxx_wincore.h>        // Add CWnd

/*----------------------------------------------------------------------------*/
#endif //STDAFX_H
