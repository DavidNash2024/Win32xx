/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]              (targetver.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

    Contents Description: This header file defines the Windows API
    macros that target the version of the Windows operating system
    this application supports.  Extracted from Win32++ sample projects,
    Copyright (c) 2005-2016 David Nash, under permissions granted therein.

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

    Special Conventions:

    Acknowledgement:
        The author would like to thank and acknowledge the advice,
        critical review, insight, and assistance provided by David Nash
        in the development of this work.

    Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Network Planning
        and Preparation Subsystem project for C++ programming.

*******************************************************************************/


// For Windows 95
//#define WINVER          0x0400
//#define _WIN32_WINDOWS  0x0400
//#define _WIN32_IE       0x0400

// For Windows 98
#define WINVER          0x0410
#define _WIN32_WINDOWS  0x0410
#define _WIN32_IE       0x0401

// For Windows NT4
//#define WINVER          0x0400
//#define _WIN32_WINNT    0x0400
//#define _WIN32_IE       0x0400

// For Windows ME
//#define WINVER          0x0500
//#define _WIN32_WINDOWS  0x0490
//#define _WIN32_IE       0x0500

// For Windows 2000
//#define WINVER          0x0500
//#define _WIN32_WINNT    0x0500
//#define _WIN32_IE       0x0500
//#define NTDDI_VERSION   0x05000000

// For Windows XP
//#define WINVER          0x0501
//#define _WIN32_WINNT    0x0501
//#define _WIN32_IE       0x0501
//#define NTDDI_VERSION   0x05010000

// For Windows Vista
//#define WINVER          0x0600
//#define _WIN32_WINNT    0x0600
//#define _WIN32_IE       0x0600
//#define NTDDI_VERSION   0x06000000

// For Windows 7
//#define WINVER          0x0601
//#define _WIN32_WINNT    0x0601
//#define _WIN32_IE       0x0700
//#define NTDDI_VERSION   0x06010000

// For Windows 8
//#define WINVER          0x0602
//#define _WIN32_WINNT    0x0602
//#define _WIN32_IE       0x0A00
//#define NTDDI_VERSION   0x06020000

//For Windows 8.1
//#define WINVER          0x0602
//#define _WIN32_WINNT    0x0602
//#define _WIN32_IE       0x0A00
//#define NTDDI_VERSION   0x06030000

//For Windows 10
//#define WINVER          0x0A00
//#define _WIN32_WINNT    0x0A00
//#define _WIN32_IE       0x0A00
//#define NTDDI_VERSION   0x0A000000

