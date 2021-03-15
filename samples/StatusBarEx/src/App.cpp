/* [Tab/Indent: 8/8][Line/Box: 80/74]                                (App.cpp) *
********************************************************************************

    Contents Description: Implementation of the CApp class for a sample program
    demonstrating the use of a custom status bar within the Win32++ Windows
    framework.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "App.h"


/*******************************************************************************

    Implementation of the CApp class

*=============================================================================*/
    BOOL CApp::
InitInstance()                                                              /*

    Create the Frame Window.
*-----------------------------------------------------------------------------*/
{
    try
    {
        m_frame.Create(); // throws a CWinException on failue
        return TRUE;
    }
    catch (...)
    {     // We get here if the Frame creation fails
        ::MessageBox(NULL, _T("Failed to create Frame window"),
            _T("ERROR"), MB_ICONERROR);
    }
    return FALSE; // returning FALSE ends the application
}
/*----------------------------------------------------------------------------*/
