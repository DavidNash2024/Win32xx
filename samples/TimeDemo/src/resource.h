/////////////////////////////////////////
// resource.h
// Authors: Robert Tausworthe, David Nash
//

// The specification of IDs for resource objects generally adhere to prefixes
// as shown in the following table.
//
// Entity id   Object
// IDP_        Message-box prompt
// IDD_        Dialog-box id
// ID_         Standard MFC toolbar and menu command
// IDT_        Toolbar command
// IDM_        Menu command
// IDR_        Frame-related resource
// IDC_        Control resource
// IDW_        Win32++ default resources


#ifndef RESOURCE_H_DEFINED
#define RESOURCE_H_DEFINED


// Include the resource IDs defined by Win32++, numbered 51 - 99.
// These are prefixed by IDW_.
#include "default_resource.h"

  // the base of this app's IDs:
#define THIS_APP                        150

  // resource IDs for Menu and  ToolBar commands
#define IDM_FILE_NEW                    151
#define IDM_FILE_OPEN                   152
#define IDM_FILE_SAVE                   153
#define IDM_FILE_SAVEAS                 154
#define IDM_FILE_CLOSE                  155
#define IDM_FILE_EXIT                   158

#define IDM_HELP_ABOUT                  167
#define IDM_COLOR_CHOICE                170
#define IDM_FONT_CHOICE                 171

#define IDB_MENUICONS                   175

  // resource IDs for controls
#define IDC_CREDITS                     180

#endif // RESOURCE_H_DEFINED
