/* [Tab/Indent: 8/8][Line/Box: 80/74]                            (TargetVer.h) *
********************************************************************************

    Contents Description: Declaration of the Windows parameters for a sample
    program demonstrating an expanded use of the status bar within the
    Win32++ Windows framework, Copyright (c) 2005-2017 David Nash.

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

*******************************************************************************

    targetver.h is used to define the Windows API macros that target the
    version of the Windows operating system you wish to support.

*-----------------------------------------------------------------------------*/

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

