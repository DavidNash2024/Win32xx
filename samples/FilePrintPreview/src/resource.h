/* (12-Nov-2016) [Tab/Indent: 8/8][Line/Box: 80/74]               (resource.h) *
********************************************************************************

    Definitions of this application's resource identifiers.

********************************************************************************

    Acknowledgment. This demo program was adapted from the PrintPreview 
    sample program appearing in the Win32++ framework sample folder, created 
    by  David Nash and published under the permissions granted in that work.
    The adaptation here implements the PrintView window as a separate popup
    window that appears on screen along with the regular program window.
    This work has been developed under the co-authorship of Robert C. 
    Tausworthe and David Nash, and released under the copyright provisions 
    of the Win32++ Interface Classes software, copyright (c) David Nash,
    2005-2018. The former author acknowledges and thanks the latter for his 
    patient direction and inspiration in the development of the classes of 
    these classes.

*******************************************************************************/


  // Include the Resource IDs defined by Win32++, numbered 51 - 99.
  // These are prefixed by IDW_.
#include "default_resource.h"

  // Program and Print Preview dialog parameters
#define HIDE_HELP						  1
#define PVW_DLG_WIDTH					100
#define PVW_DLG_HEIGHT					100
#define PVW_TB_HEIGHT				     20

  // Resource IDs for menu items and ToolBar items
#define IDM_FILE_NEW					121
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
#define IDM_WRAP_NONE					141
#define IDM_WRAP_WINDOW					142
#define IDM_WRAP_PRINTER				143
#define IDM_HELP_ABOUT                  150

  // resource IDs for the print preview dialog window
#define IDD_PRINTPREVIEW                160
#define IDC_PREVIEW_PRINT               161
#define IDC_PREVIEW_SETUP               162
#define IDC_PREVIEW_PAGE				163
#define IDC_PREVIEW_OFPAGES				164
#define IDC_PREVIEW_FIRST				165
#define IDC_PREVIEW_PREV                166
#define IDC_PREVIEW_NEXT                167
#define IDC_PREVIEW_LAST				168
#define IDC_PREVIEW_CLOSE               169
#define IDC_PREVIEW_PANE                170
#define IDC_PREVIEW_HELP                171
#define IDC_PREVIEW_ZOOMCOMBO			172

  // bitmaps for the print preview dialog buttons
#define IDB_PREVIEW_FIRST				180
#define IDB_PREVIEW_PREV                181
#define IDB_PREVIEW_NEXT                182
#define IDB_PREVIEW_LAST				183

 // resource IDs of the demo program icons
#define IDI_PRINTPREVIEW                190
#define IDI_PRINTSETUP                  191
#define IDI_QUICKPRINT                  192
#define IDI_PRINT                       193
#define IDI_FONT_OPTION					194
#define IDI_CHECKBOX_OFF				195

  // resource IDs of the print preview setup dialog
#define IDD_PREVIEW_SETUP				200
#define IDC_SCREEN_WIDTH                201
#define IDC_SCREEN_HEIGHT               202
#define IDC_PREVIEW_WIDTH               203
#define IDC_PREVIEW_HEIGHT              204
#define IDC_PAGE_SETUP                  205
