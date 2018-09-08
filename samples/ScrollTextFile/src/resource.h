/* (02-Aug-2016) [Tab/Indent: 4/4][Line/Box: 80/74]               (resource.h) *
********************************************************************************
|                                                                              |
|                         robert.c.tausworthe@ieee.org                         |
|                                                                              |
===============================================================================*

    Contents Description:  Resources Definitions used by the ScrollWin demo 
    program and its resource.rc file.  Adapted to the Win32++ Windows interface 
    classes, Copyright c) 2005-2017 David Nash, under permissions granted 
    therein.
    
    Programming Notes:
        The programming standards roughly follow those established by the 
    1997-1999 Jet Propulsion Laboratory Network Planning and Preparation 
    Subsystem project for C++ programming.

*******************************************************************************/


  // include the Resource IDs defined by Win32++, numbered 51 - 99 and
  // prefixed with IDW_
#include "default_resource.h"

  //Resource IDs for standard menu items and ToolBar items
#define IDM_FILE_OPEN                   1000
#define IDM_FILE_CLOSE                  1001
#define IDM_FILE_EXIT                   1002
#define IDM_FONT_CHOICE                 1003
#define IDM_COLOR_CHOICE                1004
#define IDM_CREDITS                     1005

  // bitmap identifiers
#define IDB_TOOLBAR                     1100

  // toolbar identifiers (not used)
#define IDB_TOOLBAR_NORM                1200
#define IDB_TOOLBAR_HOT                 1201
#define IDB_TOOLBAR_DIS                 1202
#define IDB_TOOLBAR_SML                 1203
  // app parameters
#define IDS_APP_VERSION                 1300
#define IDS_ARCHIVE_FILE_EXT            1301
#define IDS_DATAPATH_SUBDIR             1302
#define IDS_DOC_DEFAULT_EXT             1303
#define IDS_FILE_FILTER                 1304
#define IDS_MAX_MRU_ENTRIES             1306

/*-----------------------------------------------------------------------------*/

