/* [Tab/Indent: 8/8][Line/Box: 80/74]	                          (Resource.h) *
********************************************************************************

	Contents Description: Declaration of the resource identifiers for a 
	sample program demonstrating an expanded use of the status bar within
	the Win32++ Windows framework, Copyright (c) 2005-2017 David Nash.

	This particular program was adapted from the StatusBar sample progam
	in the Win32++ distribution by Robert C. Tausworthe to illustrate the 
	definition of the status bar makeup by way of a data structure, in a
	similar fashion as that appearing in the Microsoft Foundation Classes 
	(MFC), and to override the Win32++ normal statusbar definition of four 
	parts to any number suitable for an application.

	The code changes to the StatusBar sample program only occur in the 
	CMainFrame and MyStatusBar classes. However, the entire program has
	been reformatted for better readability, per the Programming Notes,
	below.

	These materials are provided "as is", without warranty of any kind,
	express or implied, including but not limited to: warranties of
	merchantability, fitness for a particular purpose, and non-infringement.

	Programming Notes:
                The programming conventions used here roughly follow those 
		established by the 1997-1999 Jet Propulsion Laboratory Deep 
		Space Network Planning and Preparation Subsystem project for 
		C++ programming.

*******************************************************************************/
//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by Resource.rc
//

// include the Resource IDs defined by Win32++
#include "default_resource.h"

#define IDM_FILE_NEW                    121
#define IDM_FILE_OPEN                   122
#define IDM_FILE_SAVE                   123
#define IDM_FILE_SAVEAS                 124
#define IDM_FILE_PRINT                  125
#define IDM_FILE_CLOSE                  126
#define IDM_FILE_EXIT                   127
#define IDM_EDIT_UNDO                   130
#define IDM_EDIT_REDO                   131
#define IDM_EDIT_CUT                    132
#define IDM_EDIT_COPY                   133
#define IDM_EDIT_PASTE                  134
#define IDM_EDIT_DELETE                 135
#define IDM_HELP_ABOUT                  140

#define IDM_STATUSBAR                   150
#define IDM_STATUS_HLINK		151
#define IDM_STATUS_MESSAGE		152
#define IDM_PROGRESS			153
#define IDM_SEPARATOR			154
#define IDM_BLANK			155

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NO_MFC                     1
#define _APS_NEXT_RESOURCE_VALUE        129
#define _APS_NEXT_COMMAND_VALUE         32771
#define _APS_NEXT_CONTROL_VALUE         1000
#define _APS_NEXT_SYMED_VALUE           130
#endif
#endif
