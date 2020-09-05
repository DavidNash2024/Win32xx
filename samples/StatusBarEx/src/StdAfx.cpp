/* [Tab/Indent: 8/8][Line/Box: 80/74]                             (StdAfx.cpp) *
********************************************************************************

    Contents Description: Implementation of the Win32++ classes for a sample
    program demonstrating the extended use of the status bar using the
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

*******************************************************************************/
//////////////////////////////////////////////
// StdAfx.cpp

// This file is used for precompiled hearders. Precompiled headers allow
//  programs to be recompiled more quickly

// To use precompiled headers, do the following:
// 1) Add the set of rarely changed headers to stdafx.h
// 2) Include stdafx.h in each cpp file. It must be included first.
// 3) Add stdafx.cpp to your project.
// 4) Turn on precompiled hearders in the project


#include "stdafx.h"

