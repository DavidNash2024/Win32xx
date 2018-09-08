/* (10-08-2016) [Tab/Indent: 4/4][Line/Box: 80/74]              (AboutBox.cpp) *
********************************************************************************
|                                                                              |
|                         robert.c.tausworthe@ieee.org                         |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of a basic AboutBox dialog for a 
    typical application using the Win32++ Windows interface classes, Copyright 
    (c) 2005-2017 David Nash, under permissions granted therein.

 	Caveats: These materials are available under the same provisions as found 
	in the Win32++ copyright.txt notice.

    Programming Notes:
        The programming standards roughly follow those established by the 
    1997-1999 Jet Propulsion Laboratory Network Planning and Preparation 
    Subsystem project for C++ programming.

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
     AboutBox::
~AboutBox()                                                                 /*

    AboutBox destructor.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
     BOOL AboutBox::
DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)                         /*

    This is the callback procedure for the "AboutBox" dialog.  It activates
    either by selecting it in the main menu or by pressing the appropriate
    accelerator key.  The uMsg is the message to interpret, LOWORD(wParam)
    is the identifier of the menu item, control, or accelerator key, and
    lParam is unused (here). The function returns TRUE if the dialog window
    was sent a WM_COMMAND or WM_INITDIALOG message, or FALSE if not.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(lParam);
    switch (uMsg)
    {
        case WM_INITDIALOG:
        {     // send the credits box the latest compilation information
            SetDlgItemText(IDM_CREDITS, m_sAboutBoxInfo.c_str());
            return TRUE;
        }

        case WM_COMMAND:
            switch (LOWORD(wParam)) 
            {
                case IDOK:
                case IDCANCEL:
                {
                    EndDialog((INT_PTR) LOWORD(wParam));
                    return TRUE;
                }
            }
    }
      // for uMsg's not handled here
    return FALSE;
}

