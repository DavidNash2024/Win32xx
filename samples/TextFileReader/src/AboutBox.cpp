/* (10-08-2016) [Tab/Indent: 4/4][Line/Box: 80/74]              (AboutBox.cpp) *
********************************************************************************
|                                                                              |
|                Authors: Robert Tausworthe, David Nash, 2020                  |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the AboutBox dialog for this
    sample program using the Win32++ Windows interface classes.

    Programming Notes: The programming standards roughly follow those
    established by the 1997-1999 Jet Propulsion Laboratory Network Planning
    and Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"

/*============================================================================*/
    AboutBox::
AboutBox()                                                                  /*

    AboutBox constructor: uses the IDW_ABOUT dialog in resource.rc
*-----------------------------------------------------------------------------*/
    :   CDialog(IDW_ABOUT)
{
}

/*============================================================================*/
     BOOL AboutBox::
OnInitDialog()                                                              /*
/*
    Called when the AboutBox dialog is created.
*-----------------------------------------------------------------------------*/
{
     SetDlgItemText(IDM_CREDITS, m_aboutBoxInfo);
     return TRUE;
}
/*----------------------------------------------------------------------------*/
