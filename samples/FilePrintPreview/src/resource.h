/* (12-Nov-2016) [Tab/Indent: 8/8][Line/Box: 80/74]               (resource.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Definitions of this application's resource identifiers.

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
        IDW_        Win32++ default resources.

*******************************************************************************/

  // Support XP themes on older compilers
#ifndef RT_MANIFEST
  #define RT_MANIFEST 24
#endif

  // Include the Resource IDs defined by Win32++, numbered 51 - 99.
  // These are prefixed by IDW_.
#include "default_resource.h"

  // Program and Print Preview dialog parameters
#define HIDE_HELP                         1
#define PVW_DLG_WIDTH                   100
#define PVW_DLG_HEIGHT                  100
#define PVW_TB_HEIGHT                    20

  // Resource IDs for menu items and ToolBar items
#define IDM_FILE_NEW                    121
#define IDM_FILE_OPEN                   122
#define IDM_FILE_SAVE                   123
#define IDM_FILE_SAVEAS                 124
#define IDM_FILE_PRINTSETUP             125
#define IDM_FILE_PREVIEW                126
#define IDM_FILE_PRINT                  127
#define IDM_FILE_QUICKPRINT             128
#define IDM_FILE_CLOSE                  129
#define IDM_FILE_EXIT                   130
#define IDM_EDIT_UNDO                   131
#define IDM_EDIT_REDO                   132
#define IDM_EDIT_CUT                    133
#define IDM_EDIT_COPY                   134
#define IDM_EDIT_PASTE                  135
#define IDM_EDIT_DELETE                 136
#define IDM_OPTIONS_FONT                140
#define IDM_WRAP_NONE                   141
#define IDM_WRAP_WINDOW                 142
#define IDM_WRAP_PRINTER                143
#define IDM_HELP_ABOUT                  150

  // resource IDs for the print preview dialog window
#define IDD_PRINTPREVIEW                160
#define IDC_PREVIEW_PRINT               161
#define IDC_PREVIEW_PAGE                163
#define IDC_PREVIEW_OFPAGES             164
#define IDC_PREVIEW_FIRST               165
#define IDC_PREVIEW_PREV                166
#define IDC_PREVIEW_NEXT                167
#define IDC_PREVIEW_LAST                168
#define IDC_PREVIEW_CLOSE               169
#define IDC_PREVIEW_PANE                170
#define IDC_PREVIEW_HELP                171
#define IDC_PREVIEW_ZOOMCOMBO           172

  // bitmaps for the print preview dialog buttons
#define IDB_PREVIEW_FIRST               180
#define IDB_PREVIEW_PREV                181
#define IDB_PREVIEW_NEXT                182
#define IDB_PREVIEW_LAST                183

 // resource IDs of the program icons
#define IDI_PRINTPREVIEW                190
#define IDI_QUICKPRINT                  192
#define IDI_PRINT                       193
#define IDI_FONT_OPTION                 194
#define IDI_CHECKBOX_OFF                195

/*----------------------------------------------------------------------------*/