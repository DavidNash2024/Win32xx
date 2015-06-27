/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]              (targetver.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

	Contents Description: This header file defines the Windows API
	macros that target the version of the Windows operating system
	this application supports.  Extracted from Win32++ sample projects,
	Copyright (c) 2005-2015 David Nash, under permissions granted therein.

        Caveats: The copyright displayed above extends only to the author's
	original contributions to the subject class, and  to the alterations,
	additions, deletions, and  other treatments of materials that may have
	been extracted from the cited sources.  Unaltered portions of those
	materials retain their original copyright status. The author hereby
	grants permission to any person obtaining a copy of this treatment
	of the subject class and  any associated documentation composed by
	the author, to utilize this material, free of charge and  without
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

	Special Conventions:

	Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	Definition of the Windows version for which this application
	is compiled.

	Note: this file is not required for TDM-GCC, which makes its own
	declarations of these parameters.  It may be necessary, if this file
	remains to be named in stdafx.h to change the values below.  This has
	been done for the Windows XP values under which this demo was
	developed.

*******************************************************************************/


// For Windows 95
//#define WINVER          0x0400
//#define _WIN32_WINDOWS  0x0400
//#define _WIN32_IE       0x0300

// For Windows 98
//#define WINVER          0x0410
//#define _WIN32_WINDOWS  0x0410
//#define _WIN32_IE       0x0401

// For Windows NT4
//#define WINVER          0x0400
//#define _WIN32_WINNT    0x0400
//#define _WIN32_IE       0x0200
//#define NTDDI_VERSION   0x05000000

// For Windows ME
//#define WINVER          0x0500
//#define _WIN32_WINNT    0x0500
//#define _WIN32_IE       0x0500

// For Windows 2000
//#define WINVER          0x0500
//#define _WIN32_WINNT    0x0500
//#define _WIN32_IE       0x0500
//#define NTDDI_VERSION   0x05000000

// For Windows XP
#ifndef WINVER
#define WINVER          0x0502
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT    0x0502
#endif
#ifndef _WIN32_IE
#define _WIN32_IE       0x0602
#endif
#ifndef NTDDI_VERSION
#define NTDDI_VERSION   0x05020000
#endif

// For Windows Vista
//#define WINVER          0x0600
//#define _WIN32_WINNT    0x0600
//#define _WIN32_IE       0x0600
//#define NTDDI_VERSION   0x06000000

// For Windows 7
//#define WINVER          0x0601
//#define _WIN32_WINNT    0x0601
//#define _WIN32_IE       0x0601
//#define NTDDI_VERSION   0x06010000


// Users of Visual Studio 10 can do this instead
// #include "SDKDDKver.h"

