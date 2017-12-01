/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                 (StdApp.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

    Contents Description: Standard application declarations header file.
    This file is used to #include all the application's header files. Fo
    use in programs compiled using the Win32++ interface classes, Copyright
    (c) 2005-2016 David Nash, under permissions granted therein.

    Adapted for use in this application.

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

    To use precompiled headers, do the following:
    1) Add the set of application headers to stdapp.h
    2) Include StdApp.h in each cpp file. It must be included first.
    3) Compile StdApp.h into the precompiled hearder StdApp.h.gch by
       including StdApp.mak into the makefile in the project options:

        all-before: StdApp.h.gch
                StdApp.h.gch: StdApp.h
            $(CPP) -c StdApp.h -o StdApp.h.gch $(CXXFLAGS)

    4) Add StdApp.cpp to your project source files.

    Special Conventions:

    Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
        Planning and Preparation Subsystem project for C++ programming.
    Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
        Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

    Declaration of the StdApp headers for this application

*******************************************************************************/


#ifndef STD_APP_H
#define STD_APP_H

/*============================================================================*/

#include "Resource.h"
#include "ListBoxDlgRC.h"
#include "AppGlobalRC.h"
#include "AppGlobal.h"
#include "ColorDefs.h"
#include "CustomButton.h"
#include "ColorChoice.h"
#include "MyFileDlg.h"
#include "MyFontDlg.h"
#include "MyFindReplaceDlg.h"
#include "MyPrinter.h"
#include "RichEditView.h"
#include "Doc.h"
#include "PrintInfo.h"
#include "View.h"
#include "AboutBox.h"
#include "MainFrm.h"
#include "App.h"

/*----------------------------------------------------------------------------*/
#endif // STD_APP_H
