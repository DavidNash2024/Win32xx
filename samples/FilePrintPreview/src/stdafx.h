/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                 (StdAfx.h) *
********************************************************************************

    Declaration of the stdafx headers precompiled for this application

*******************************************************************************/

#ifndef STDAFX_H
#define STDAFX_H


// Predefinitions for windows.h go here
//#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers
#include "targetver.h"      // Set the supported window features

// Specify Win32xx specific predefinitions here
//#define NO_USING_NAMESPACE    // Don't use Win32xx namespace

#if defined(_MSC_VER) && _MSC_VER == 1200 // For Visual Studio 6
  #pragma warning (disable : 4786)    // identifier was truncated
  #pragma warning (disable : 4702)    // unreachable code (bugs in Microsoft's STL)
#endif

// Rarely modified header files should be included here
#include <vector>       // support for std::vector
#include <map>          // support for std::map
#include <string>       // support for std::string
#include <sstream>      // support for stringstream
#include <cassert>      // support for the assert macro
#include <stdio.h>      // support for C style printf, sprintf, etc.
#include <stdlib.h>     // support for C style conversions: atoi etc.
#include <tchar.h>      // support for C style TCHARs.

#include <wxx_appcore.h>    // CCriticalSection, CObject, CWinThread,
                // CWinApp
#include <wxx_archive.h>    // CArchive
#include <wxx_commondlg.h>  // CCommonDialog, CColorDialog, CFileDialog,
                // CFindReplace, CFontDialog
#include <wxx_controls.h>   // CAnimation, CComboBox, CComboBoxEx, CDateTime,
                // CHeader, CHotKey, CIPAddress, CProgressBar,
                // CSpinButton, CScrollBar, CSlider, CToolTip
#include <wxx_cstring.h>    // CString, CStringA, CStringW
#include <wxx_ddx.h>        // CDataExchange
#include <wxx_dialog.h>     // CDialog, CResizer
#include <wxx_docking.h>    // CDocker, CDockContianer
#include <wxx_exception.h>  // CException, CFileException,
                // CNotSupportedException, CResourceException,
                // CUserException, CWinException
#include <wxx_file.h>       // CFile
#include <wxx_filefind.h>   // Add CFileFind
#include <wxx_folderdialog.h>   // Add CFolderDialog
#include <wxx_frame.h>      // CFrame, CDockFrame
#include <wxx_gdi.h>        // CDC, CGDIObject, CBitmap, CBrush, CFont,
                // CPalatte, CPen, CRgn
#include <wxx_imagelist.h>  // CImageList
#include <wxx_listview.h>   // CListView
#include <wxx_mdi.h>        // CMDIChild, CMDIFrame, CDockMDIFrame
#include <wxx_metafile.h>       // Add CMetaFile, CEnhMetaFile
#include <wxx_printdialogs.h>   // CPageSetupDialog, CPrintSetupDialog
#include <wxx_propertysheet.h>  // CPropertyPage, CPropertySheet
#include <wxx_rebar.h>      // CRebar
#include <wxx_regkey.h>     // CRegKey
//#include <wxx_ribbon.h>   // CRibbon, CRibbonFrame
#include <wxx_richedit.h>   // CRichEdit
#include <wxx_shared_ptr.h> // Shared_Ptr
#include <wxx_socket.h>     // CSocket
#include <wxx_statusbar.h>  // CStatusBar
#include <wxx_stdcontrols.h>    // CButton, CEdit, CListBox
#include <wxx_tab.h>        // CTab, CTabbedMDI
//#include <wxx_taskdialog.h>   // CTaskDialog
#include <wxx_time.h>       // CTime
#include <wxx_toolbar.h>    // CToolBar
#include <wxx_treeview.h>   // CTreeView
#include <wxx_webbrowser.h> // CAXWindow, CWebBrowser
#include <wxx_scrollview.h>     // Add CScrollView
#include <wxx_wincore.h>    // CWnd
/*----------------------------------------------------------------------------*/
#endif //STDAFX_H
