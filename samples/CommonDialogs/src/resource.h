/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]               (resource.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description:  Resource Definitions used by the CommonDialogs
    sample application resource.rc file.  Adapted to the Win32++ Windows
    interface classes.
    
    Special Conventions:  The specification of IDs for resource objects
    generally adhere to prefixes as shown in the following table. Note
    that different kinds of resource objects have different prefixes to
    distinguish the types of entities they belong to:
    
        Entity id   Object
        IDP_        Message-box prompt
        IDD_        Dialog-box id
        ID_         Standard MFC toolbar and menu command
        IDT_        Toolbar command
        IDM_        Menu command
        IDR_        Frame-related resource
        IDC_        Control resource
        IDW_        Win32++ default resources
    
*******************************************************************************/

#ifndef RESOURCE_H_DEFINED
#define RESOURCE_H_DEFINED

/*============================================================================*/
  
// Support XP themes on older compilers
#ifndef RT_MANIFEST
  #define RT_MANIFEST 24
#endif

  // Include the resource IDs defined by Win32++, numbered 51 - 99.
  // These are prefixed by IDW_.
#include "default_resource.h"

  // resource id of the main frame dialog (see resource.rc)
#define IDD_MAIN_DIALOG                 100         

  // resource IDs for Menu and ToolBar commands
#define IDM_FILE_NEW                    101         
#define IDM_FILE_OPEN                   102         
#define IDM_FILE_SAVE                   103         
#define IDM_FILE_SAVEAS                 104         
#define IDM_FILE_CLOSE                  105         
#define IDM_FILE_PAGESETUP              106         
#define IDM_FILE_PRINT                  107         
#define IDM_FILE_EXIT                   109         
#define IDM_EDIT_UNDO                   110         
#define IDM_EDIT_REDO                   111         
#define IDM_EDIT_CUT                    112         
#define IDM_EDIT_COPY                   113         
#define IDM_EDIT_PASTE                  114         
#define IDM_EDIT_FIND                   115         
#define IDM_EDIT_REPLACE                116         
#define IDM_EDIT_DELETE                 117         
#define IDM_FONT_CHOICE                 120         
#define IDM_COLOR_CHOICE                121
#define IDC_ABOUT_MSG                   130     
#define IDS_APP_VERSION                 140
#define IDS_ARCHIVE_FILE_EXT            141
#define IDS_DATAPATH_SUBDIR             142
#define IDS_DOC_DEFAULT_EXT             143
#define IDS_FILE_FILTER                 144
#define IDS_MAX_MRU_ENTRIES             145
  // resource IDs for client area controls (IDOK has its own, so not given here)
#define IDC_RICHEDITBOX                 150
#define IDM_RICHEDWRAP                  151
  // bitmap
#define IDB_MENUICONS                   160
  // CListBoxDlg resources
#define IDD_CLISTVIEWBOX                200
#define IDC_LIST_BOX                    201  

/*-----------------------------------------------------------------------------*/
#endif // RESOURCE_H_DEFINED
