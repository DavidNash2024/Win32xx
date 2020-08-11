/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]               (resource.h) *
********************************************************************************
|                                                                              |
|             Copyright (c) 2020, Robert C. Tausworthe, David Nash             |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

    Contents Description:  Resource definitions used by a minimal SDI
    application's resource.rc file.  Adapted to the Win32++ Windows
    interface classes, Copyright (c) 2005-2020 David Nash, under
    permissions granted therein.

    The above copyright notice, as well as that of David Nash and Win32++,
    together with the respective permissionconditions shall be included in all
    copies or substantial portions of this material so copied, modified, merged,
    published, distributed, or otherwise held by others.

    These materials are provided "as is", without warranty of any kind,
    express or implied, including but not limited to: warranties of
    merchantability, fitness for a particular purpose, and non-infringement.
    In no event shall the authors or copyright holders be liable for any
    claim, damages, or other liability, whether in an action of contract,
    tort or otherwise, arising from, out of, or in connection with, these
    materials, the use thereof, or any other other dealings therewith.

    Special Conventions:  The specification of IDs for resource objects
    generally adhere to prefixes as shown in the following table. Note
    that different kinds of resource objects have different prefixes to
    distinguish the types of entities they belong to:

        Entity ID   Object
        IDP_        Message-box prompt
        IDD_        Dialog-box ID
        ID_             Standard toolbar and  menu command
        IDT_        Toolbar command
        IDM_        Menu command
        IDR_        Frame-related resource
        IDC_        Control resource
        IDW_            Win32++ default resources

    Notes about Resource IDs: In general, resource IDs can have values
    from 1 to 65535. Programs with resource IDs higher than 65535 may
    not be supported by some Windows versions.

    * Windows uses the icon with the lowest resource ID as the
    application icon. The Win32++ default_resource.h file defines this
    identifier as IDW_MAIN. The other default system identifiers it
    defines are all numbered below 100 and  are all prefixed IDW_. These
    include resource IDs for MENU, ICON, ToolBar Bitmap, Accelerator,
     and  Window Caption.

    * The Windows Platform SDK defines the WM_APP constant as a value
    to be used by applications to define private messages that do not
    conflict with system messages. The Win32++ suite is viewed as an
    application by the WINAPI and  preempts the first 24 (in version 7.6.1)
    of these, so this file defines its resource IDs beginning at a
    reasonable distance above this, to allow for their future expansion.
    This value is defined as 150 , below.

    * If multiple static controls appear in a dialog, these should be
    given unique identifiers if they are active. Otherwise, an ID of -1
    is used to designate these static controls do not actively participate
    in the application.

    Programming Notes:
    The programming standards roughly follow those established
    by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
    Planning and Preparation Subsystem project for C++ programming.

    Acknowledgement:
    The author would like to thank and acknowledge the advice, critical
    review, insight, and assistance provided by David Nash in the development
    of this work.

********************************************************************************

    Definitions of this application's resource identifiers

*******************************************************************************/

// Support XP themes on older compilers
#ifndef RT_MANIFEST
  #define RT_MANIFEST 24
#endif

  // Include the Resource IDs defined by Win32++, numbered 51 - 99.
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

/*-----------------------------------------------------------------------------*/

