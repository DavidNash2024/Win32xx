/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]               (resource.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description:  Resource Definitions used by the BasicForm SDI
	sample application resource.rc file.  Adapted to the Win32++ Windows
	interface classes, Copyright (c) 2005-2016 David Nash, under
	permissions granted therein.

        Caveats: The copyright displayed above extends only to the author's
	original contributions to the subject class, and to the alterations,
	additions, deletions, and other treatments of materials that may have
	been extracted from the cited sources.  Unaltered portions of those
	materials retain their original copyright status. The author hereby
	grants permission to any person obtaining a copy of this treatment
	of the subject class and any associated documentation composed by
	the author, to utilize this material, free of charge and without
	restriction or limitation, subject to the following conditions:

		The above copyright notice, as well as that of David Nash
		and Win32++, together with the respective permission
		conditions shall be included in all copies or substantial
		portions of this material so copied, modified, merged,
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

	        Entity ID 	Object
		IDP_ 		Message-box prompt
		IDD_ 		Dialog-box ID
		ID_             Standard MFC toolbar and menu command
		IDT_		Toolbar command
		IDM_		Menu command
		IDR_ 		Frame-related resource
		IDC_ 		Control resource
		IDW_            Win32++ default resources

	Notes about Resource IDs: In general, resource IDs can have values
	from 1 to 65535. Programs with resource IDs higher than 65535 may
	not be supported by some Windows versions.

	* Windows uses the icon with the lowest resource ID as the
	application icon. The Win32++ default_resource.h file defines this
	identifier as IDW_MAIN. The other default system identifiers it
	defines are all numbered below 100 and are all prefixed IDW_. These
	include resource IDs for MENU, ICON, ToolBar Bitmap, Accelerator,
	and Window Caption.

	* The Windows Platform SDK defines the WM_APP constant as a value
	to be used by applications to define private messages that do not
	conflict with system messages. The Win32++ suite is viewed as an
	application by the WINAPI and preempts the first 24 (in version 7.6.1)
	of these, so this file defines its resource IDs beginning at a
	reasonable distance above this, to allow for their future expansion.
	This value is defined as THIS_APP, below.

	* If multiple static controls appear in a dialog, these should be
	given unique identifiers if they are active. Otherwise, an ID of -1
	is used to designate these static controls do not actively participate
	in the application.

	Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	Definition of resource ID values

*******************************************************************************/

#ifndef RESOURCE_H_DEFINED
#define RESOURCE_H_DEFINED


  // Include the resource IDs defined by Win32++, numbered 51 - 99.
  // These are prefixed by IDW_.
#include "default_resource.h"

#ifndef IDC_STATIC
#define IDC_STATIC (-1)
#endif

  // resource ID of the main frame dialog (see resource.rc)
#define IDD_FORM_DIALOG                100
#define IDC_ABOUT_BOX_CREDITS          101

  // resource IDs of menu or toolbar icons, bitmaps, or other images
#define IDI_COLOR_CHOICE               200
#define IDI_FONT_CHOICE                201

  // resource IDs for Menu and ToolBar commands
#define IDM_FILE_NEW                   1001
#define IDM_FILE_OPEN                  1002
#define IDM_FILE_SAVE                  1003
#define IDM_FILE_SAVEAS                1004
#define IDM_FILE_CLOSE                 1005
#define IDM_FILE_PAGESETUP             1006
#define IDM_FILE_PRINT                 1007
#define IDM_FILE_PREVIEW	           1008
#define IDM_FILE_EXIT                  1009
#define IDM_EDIT_UNDO                  1010
#define IDM_EDIT_REDO                  1011
#define IDM_EDIT_CUT                   1012
#define IDM_EDIT_COPY                  1013
#define IDM_EDIT_PASTE                 1014
#define IDM_EDIT_FIND                  1015
#define IDM_EDIT_REPLACE               1016
#define IDM_EDIT_DELETE                1017
#define IDM_HELP_CONTENT               1018
#define IDM_HELP_CONTEXT               1019
#define IDM_FONT_CHOICE                1020
#define IDM_COLOR_CHOICE               1021

  // resource IDs for client area controls (IDOK has its own, so not given here)
#define IDM_EDITBOX			           1050

#define IDD_CLISTVIEWBOX_DIALOG        2000   // the CListBoxDialog box
#define IDC_LIST_BOX  		           2001   // the list box within the above

  // message identifiers for common dialog communications
#define IDM_HELP_COLORDLG              2010
#define IDM_HELP_FILEDLG_OPEN          2011
#define IDM_HELP_FILEDLG_NEW           2012
#define IDM_HELP_FILEDLG_SAVEAS        2013
#define IDM_HELP_FINDDLG               2014
#define IDM_HELP_REPLACEDLG            2015
#define IDM_HELP_FONTDLG               2016
#define IDM_HELP_PRINTDLG              2017
#define IDM_HELP_PAGESETDLG            2018
#define IDM_HELP_LIST_BOX              2019

#define IDC_HELP_COMDLG                2100


/*-----------------------------------------------------------------------------*/
#endif // RESOURCE_H_DEFINED
