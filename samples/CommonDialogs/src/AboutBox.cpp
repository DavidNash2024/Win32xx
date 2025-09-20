/* (26-Mar-2025)                                                (AboutBox.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the AboutBox class for the
    CommonDialogs sample application using the Win32++ Windows interface
    classes.

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"

// Called before the dialog is displayed to enter the AboutBox message.
BOOL AboutBox::OnInitDialog()
{
    SetDlgItemText(IDC_ABOUT_MSG, m_about);
    return TRUE;
}

