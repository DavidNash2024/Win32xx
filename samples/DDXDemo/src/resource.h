/* (03-Jun-2015) [Tab/Indent: 8/8][Line/Box: 80/74]              (resource.h) *
********************************************************************************
|                                                                              |
|                      Author: Robert C. Tausworthe, 2020                      |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the DDXDemo sample program resource
    identifers used by its resource.rc and other members. This resource file
    is a modified version of that found in the FormDocView sample
    distributed with the Win32++ Windows interface classes. The modified sample
    program was based on code provided by Lynn Allan. This program extends the
    given sample by application of Dialog Data Exchange and Validation (DDX/DDV).

    Acknowledgement: The author would like to thank and acknowledge the advice,
    critical review, insight, and assistance provided by David Nash in the
    development of this work.

*******************************************************************************/

#ifndef RESOURCE_H
#define RESOURCE_H

// Support XP themes on older compilers
#ifndef RT_MANIFEST
  #define RT_MANIFEST 24
#endif

  // Include the Resource IDs defined by Win32++
#include "default_resource.h"

  // Resource ID for the dialog
#define IDD_MAIN_DIALOG                 100

  // Resource IDs for standard menu items
#define IDM_FILE_NEW                    200
#define IDM_FILE_OPEN                   201
#define IDM_FILE_SAVE                   202
#define IDM_FILE_SAVEAS                 203
#define IDM_FILE_PRINT                  204
#define IDM_FILE_CLOSE                  205
#define IDM_FILE_EXIT                   206
#define IDM_EDIT_UNDO                   207
#define IDM_EDIT_REDO                   208
#define IDM_EDIT_CUT                    209
#define IDM_EDIT_COPY                   210
#define IDM_EDIT_PASTE                  211
#define IDM_EDIT_DELETE                 212

  // Resource IDs for the app's menu-items (also on dialog)
#define IDC_RADIO_A                     300
#define IDC_RADIO_B                     301
#define IDC_RADIO_C                     302
#define IDC_CHECK_A                     303
#define IDC_CHECK_B                     304
#define IDC_CHECK_C                     305

  // Resource IDs for dialog edit boxes
#define IDC_EDIT_BYTE                   400
#define IDC_EDIT_SHORT                  401
#define IDC_EDIT_INT                    402
#define IDC_EDIT_UINT                   403
#define IDC_EDIT_LONG                   404
#define IDC_EDIT_DWORD                  405
#define IDC_EDIT_FLOAT                  406
#define IDC_EDIT_DOUBLE                 407
#define IDC_EDIT_LPTSTR                 408
#define IDC_EDIT_CSTRING                409
#define IDC_EDIT_RICHEDIT               410
#define IDC_EDIT_STATUS                 411

  // Resource IDs for other controls
#define IDC_LISTBOX                     500
#define IDC_SLIDER                      501
#define IDC_PROGRESSBAR                 502
#define IDC_SCROLLBAR                   503
#define IDC_COMBOBOX                    504
#define IDC_DATE_TIME                   505
#define IDC_CALENDAR                    506
#define IDC_STATUS_GROUP                507

  // Resource IDs for buttons (except STATUS = IDOK button)
#define IDC_PUSH_ME_BUTTON              600
#define IDB_ROSE_BITMAP                 601
#define IDC_ROSE_BITMAP                 602

  // Resource ID for small menu icons
#define IDB_TOOLBAR16                   603

/*----------------------------------------------------------------------------*/
#endif // RESOURCE_H

