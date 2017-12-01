/* (01-Nov-2016) [Tab/Indent: 8/8][Line/Box: 80/74]             (AboutBox.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the AboutBox class for the
    CommonDialogs SDI sample application using the Win32++ Windows interface
    classes, Copyright (c) 2005-2016 David Nash, under permissions granted
    therein.

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
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
        Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

    Implementation of the AboutBox class

*******************************************************************************/

#include "StdAfx.h"
#include "StdApp.h"

/*******************************************************************************

    AboutBox dialog implementation

/=============================================================================*/
    void AboutBox::
OnHelpAbout()                                                           /*

    Display the application's About dialog box, which shows a credits string
    supplied by the application. This typically contains an application
    name, copyright information, and date of most recent compilation.
*-----------------------------------------------------------------------------*/
{
      // Ensure that only one About dialog displays even for multiple
      // activation commands
    CString s = theAppGlobal.GetAboutBoxInfo();
    if (!IsWindow())
        Create();  // make the AboutBox modeless

    SetDlgItemText(IDC_ABOUT_MSG, s); // tbd m_sCredits);
    SetFocus();     // set the focus on the box
    return;
}

