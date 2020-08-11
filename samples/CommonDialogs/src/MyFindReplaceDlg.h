/* (08-Feb-2016)                                          (MyFindReplaceDlg.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, David Nash                             |
|                          All Rights Reserved.                                |
|                       email: dnash@bigpond.net.au                            |
|            url: https://sourceforge.net/projects/win32-framework             |
|                                                                              |
===============================================================================*

    Contents Description: The MyFindREplaceDlg class declaration and
    implementation. This class is derived from the CFindREplaceDalog class
    found in the Win32++ Windows interface framework.

        Caveats: The copyright displayed above extends only to the author's
    original contributions to the subject class, and to the alterations,
    additions, deletions, and other treatments of materials that may have
    been extracted from the cited sources.  Unaltered portions of those
    materials retain their original copyright status. The author hereby
    grants permission to any person obtaining a copy of this treatment
    of the subject class and any associated documentation composed by
    the author, to utilize this material, free of charge and without
    restriction or limitation, subject to the following conditions:

    These materials are provided "as is", without warranty of any kind,
    express or implied, including but not limited to: warranties of
    merchantability, fitness for a particular purpose, and non-infringement.
    In no event shall the author or copyright holders be liable for any
    claim, damages, or other liability, whether in an action of contract,
    tort or otherwise, arising from, out of, or in connection with, these
    materials, the use thereof, or any other other dealings therewith.

    Acknowledgement:
        The author would like to thank and acknowledge the advice,
        critical review, insight, and assistance provided by David Nash
        in the development of this work.

    Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
        Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

    Declaration and implementation of the MyFindReplaceDialog class

*******************************************************************************/

#ifndef MY_FIND_REPLACE_DLG_H
#define MY_FIND_REPLACE_DLG_H

#include "ListBoxDlgRC.h"

/*============================================================================*/
    class
MyFindReplaceDialog : public CFindReplaceDialog

/*----------------------------------------------------------------------------*/
{
    public:
        MyFindReplaceDialog() {}

        void SetBoxTitle(const CString& title) {m_sBoxTitle = title;}

    protected:
        virtual BOOL OnInitDialog()
          // Override this member method to perform special processing
         //  when the FindReplaceDialog  box is initialized. Return TRUE.
        {
              // do the base class initialization first
            CDialog::OnInitDialog();
            SetWindowTitle();
            return TRUE;
        }

        virtual void SetWindowTitle() const { SetWindowText(m_sBoxTitle);}

    private:
        CString m_sBoxTitle;
};
/*----------------------------------------------------------------------------*/
#endif // MY_FIND_REPLACE_DLG_H
