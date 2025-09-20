/* (26-Mar-2025)                                        (MyFindReplaceDlg.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the MyFindReplaceDialog class
    for applications using the Win32++ Windows interface classes. This
    class derives from the CFindReplaceDialog class found in the framework.

*******************************************************************************/

#include "stdafx.h"
#include "MyFindReplaceDlg.h"

// This function is called before the dialog is displayed.
BOOL MyFindReplaceDialog::OnInitDialog()
{
    SetWindowText(m_boxTitle); return TRUE;
}

// Record the title of the find/replace dialog box after an object
// of this class is constructed, but before DoModal() is invoked.
void MyFindReplaceDialog::SetBoxTitle(LPCWSTR title)
{
    m_boxTitle = title;
}
