//////////////////////////////////////////////
// StdAfx.h

// This file is used for precompiled headers
// Rarely modified header files should be included.

// Based on code provided by Lynn Allan


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
#include <assert.h>             // Add support for the assert macro
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
#include <wxx_docking.h>        // Add CDocker, CDockContianer
#include <wxx_exception.h>      // Add CException, CFileException, CNotSupportedException, CResourceException, CUserException, CWinException
#include <wxx_file.h>           // Add CFile
#include <wxx_frame.h>          // Add CFrame, CDockFrame
#include <wxx_gdi.h>            // Add CDC, CGDIObject, CBitmap, CBrush, CFont, CPalatte, CPen, CRgn
#include <wxx_imagelist.h>      // Add CImageList
#include <wxx_listview.h>       // Add CListView
#include <wxx_mdi.h>            // Add CMDIChild, CMDIFrame, CDockMDIFrame
#include <wxx_printdialogs.h>   // Add CPageSetupDialog, CPrintSetupDialog
#include <wxx_propertysheet.h>  // Add CPropertyPage, CPropertySheet
#include <wxx_rebar.h>          // Add CRebar
#include <wxx_regkey.h>         // Add CRegKey
//#include <wxx_ribbon.h>       // Add CRibbon, CRibbonFrame
#include <wxx_richedit.h>       // Add CRichEdit
#include <wxx_shared_ptr.h>     // Add Shared_Ptr
#include <wxx_socket.h>         // Add CSocket
#include <wxx_statusbar.h>      // Add CStatusBar
#include <wxx_stdcontrols.h>    // Add CButton, CEdit, CListBox
#include <wxx_tab.h>            // Add CTab, CTabbedMDI
//#include <wxx_taskdialog.h>   // Add CTaskDialog
#include <wxx_time.h>           // Add CTime
#include <wxx_toolbar.h>        // Add CToolBar
#include <wxx_treeview.h>       // Add CTreeView
#include <wxx_webbrowser.h>     // Add CAXWindow, CWebBrowser
#include <wxx_wincore.h>        // Add CWnd


#endif
