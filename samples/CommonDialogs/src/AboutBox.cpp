/* (01-Nov-2016) [Tab/Indent: 8/8][Line/Box: 80/74]             (AboutBox.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the AboutBox class for the
    CommonDialogs sample application using the Win32++ Windows interface
    classes.

    Programming Notes: The programming standards roughly follow those 
    established by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
    Planning and Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "StdAfx.h"
#include "StdApp.h"

/*============================================================================*/
     BOOL AboutBox:: 
OnInitDialog()                                                              /*

    Called before the dialog is displayed to enter the AboutBox message.
*-----------------------------------------------------------------------------*/
{
    SetDlgItemText(IDC_ABOUT_MSG, m_about);
    return TRUE;
}
/*----------------------------------------------------------------------------*/

