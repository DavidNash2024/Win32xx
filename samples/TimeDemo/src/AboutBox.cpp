/* (10-08-2014) [Tab/Indent: 8/8][Line/Box: 80/74]              (AboutBox.cpp) *
********************************************************************************
|                                                                              |
|             Copyright (c) 2020, Robert C. Tausworthe, David Nash             |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the AboutBox dialog for this
    application using the Win32++ framework, Copyright (c) 2005-2020 David Nash, 
    under permissions granted therein.

    The above copyright notice, as well as that of David Nash and Win32++, 
    together with the respective permissionconditions shall be included in all 
    copies or substantial portions of this material so copied, modified, merged,
    published, distributed, or otherwise held by others.

    These materials are provided "as is", without warranty of any kind,
    express or implied, including but not limited to: warranties of
    merchantability, fitness for a particular purpose, and non-infringement.
    In no event shall the authors or copyright holders be liable for any
    claim, damages, or other liability, whether in an action of contract,
    tort or otherwise, arising from, out of, or in connection with, these
    materials, the use thereof, or any other other dealings therewith.

    Programming Notes: The programming standards roughly follow those 
    established by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
    Planning and Preparation Subsystem project for C++ programming.
        
 
*******************************************************************************/

#include "stdafx.h"
#include "AboutBox.h"
#include "App.h"
#include "Resource.h"

/*============================================================================*/
    AboutBox::
AboutBox()                                                              /*

    AboutBox constructor: uses the IDW_ABOUT dialog in resource.rc
*-----------------------------------------------------------------------------*/
    :   CDialog(IDW_ABOUT)
{
}

/*============================================================================*/
     BOOL AboutBox::         
OnInitDialog()                                                          /*

     When the AboutBox is instantiated, enter the box contents.
*-----------------------------------------------------------------------------*/
{
     SetDlgItemText(IDC_CREDITS, m_status);
     return TRUE;
}
/*----------------------------------------------------------------------------*/

