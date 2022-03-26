/////////////////////////////
// StdAfx.h
//

// This file is used for precompiled headers.
// Rarely modified header files should be included.
// Based on code provided by Lynn Allan.


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
#include <list>                 // Add support for std::list
#include <map>                  // Add support for std::map
#include <string>               // Add support for std::string
#include <sstream>              // Add support for stringstream
#include <cassert>              // Add support for the assert macro
#include <stdio.h>              // Add support for C style printf, sprintf, etc.
#include <stdlib.h>             // Add support for C style character conversions atoi etc.
#include <tchar.h>              // Add support for C style TCHARs.

// Add the Win32++ library
#include <wxx_appcore.h>        // Add CWinApp
#include <wxx_archive.h>        // Add CArchive
#include <wxx_controls.h>       // Add CAnimation, CComboBox, CComboBoxEx, CDateTime, CHeader, CHotKey, CIPAddress, CProgressBar, CSpinButton, CScrollBar, CSlider, CToolTip
#include <wxx_criticalsection.h> // Add CCriticalSection, CThreadLock
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
#include <wxx_hglobal.h>        // Add CHGlobal
#include <wxx_imagelist.h>      // Add CImageList
#include <wxx_listview.h>       // Add CListView
#include <wxx_mdi.h>            // Add CMDIChild, CMDIFrame, CDockMDIFrame
#include <wxx_menu.h>           // Add CMenu
#include <wxx_menubar.h>        // Add CMenuBar
#include <wxx_metafile.h>       // Add CMetaFile, CEnhMetaFile
#include <wxx_mutex.h>          // Add CEvent, CMutex, CSemaphore
#include <wxx_propertysheet.h>  // Add CPropertyPage, CPropertySheet
#include <wxx_rebar.h>          // Add CRebar
#include <wxx_rect.h>           // Add CPoint, CRect, CSize
#include <wxx_regkey.h>         // Add CRegKey
#include <wxx_richedit.h>       // Add CRichEdit
#include <wxx_scrollview.h>     // Add CScrollView
#include <wxx_shared_ptr.h>     // Add Shared_Ptr
#include <wxx_setup.h>          // Adds macros, windows header files, and CObject
#include <wxx_socket.h>         // Add CSocket
#include <wxx_statusbar.h>      // Add CStatusBar
#include <wxx_stdcontrols.h>    // Add CButton, CEdit, CListBox
#include <wxx_tab.h>            // Add CTab, CTabbedMDI
#include <wxx_textconv.h>       // Add AtoT, AtoW, TtoA, TtoW, WtoA, WtoT etc.
#include <wxx_themes.h>         // Add MenuTheme, ReBarTheme, StatusBarTheme, ToolBarTheme
#include <wxx_thread.h>         // Add CWinThread
#include <wxx_time.h>           // Add CTime
#include <wxx_toolbar.h>        // Add CToolBar
#include <wxx_treeview.h>       // Add CTreeView
#include <wxx_webbrowser.h>     // Add CWebBrowser, CAXHost
#include <wxx_wincore.h>        // Add CWnd

// WIN32_LEAN_AND_MEAN can't be used with these
#ifndef WIN32_LEAN_AND_MEAN
  #include <wxx_commondlg.h>      // Add CCommonDialog, CColorDialog, CFileDialog, CFindReplace, CFontDialog
  #include <wxx_preview.h>        // Add CPrintPreview
  #include <wxx_printdialogs.h>   // Add CPageSetupDialog, CPrintDialog

  // Win2000 or higher with Microsoft VS2008 or higher, or Clang or GNU compiler
  #if (WINVER >= 0x0500) && ((defined (_MSC_VER) && (_MSC_VER >= 1500)) || defined(__clang_major__) || defined(__GNUC__))
    #include <wxx_printdialogex.h>  // Add CPrintDialogEx
  #endif

  // Windows Vista or higher with Microsoft VS2008 or higher, or Clang compiler
  #if (WINVER >= 0x0600) && ((defined (_MSC_VER) && (_MSC_VER >= 1500)) || defined(__clang_major__))
    #include <wxx_ribbon.h>         // Add CRibbon, CRibbonFrame
  #endif
#endif

// Windows Vista or higher with Microsoft VS2008 or higher, or Clang or GNU compiler. Requires Unicode.
#if (WINVER >= 0x0600) && ((defined (_MSC_VER) && (_MSC_VER >= 1500)) || defined(__clang_major__) || defined(__GNUC__))
  #include <wxx_taskdialog.h>     // Add CTaskDialog
#endif


#endif  // define STDAFX_H
