/* (11-Nov-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                  (App.CPP) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the CApp class for the
    FilePrintPreview demo program with the Windows operating system using the
    Win32++ Windows interface classes.

    Programming Notes: The programming style roughly follows that established
    for the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "App.h"


/*============================================================================*/
    BOOL CApp::
InitInstance()                                                          /*

    Called by the framework to create the frame.
*-----------------------------------------------------------------------------*/
{
    m_frame.Create();   // throws a CWinException on failure
    return TRUE;
}

