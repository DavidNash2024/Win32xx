/* (10-08-2014) [Tab/Indent: 8/8][Line/Box: 80/74]              (AboutBox.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the AboutBox dialog for
    a skeleton SDI application using the Win32++ Windows interface classes,
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
        
    Acknowledgement:
    The author would like to thank and acknowledge the advice, critical
    review, insight, and assistance provided by David Nash in the development
    of this work.       

*******************************************************************************/

#include "stdafx.h"
#include "AboutBox.h"
#include "App.h"
#include "Resource.h"


/*============================================================================*/
    AboutBox::
AboutBox()                                      /*

    AboutBox constructor: uses the IDW_ABOUT dialog in resource.rc
*-----------------------------------------------------------------------------*/
    :   CDialog(IDW_ABOUT)
{
}

/*============================================================================*/
     AboutBox::
~AboutBox()                                     /*

    AboutBox destructor.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
     INT_PTR AboutBox::
DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)                 /*

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
        case WM_INITDIALOG: // this message is received immediately before
        {           //  the dialog box is displayed
          // send the credits box the latest compilation information
        SetDlgItemText(IDC_CREDITS, CApp::m_sAboutStatement.c_str());
        return TRUE;
        }

        case WM_COMMAND:    // this message is received when one of the
                    // dialog's commands is activated.
        switch (LOWORD(wParam)) // Specifies the identifier of the menu
        {           // item, control, or accelerator.
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

