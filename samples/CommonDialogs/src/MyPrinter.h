/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]              (MyPrinter.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

    Contents Description: The declaration and implementation of the
    MyPrinter and MyPageSetup classes, which are derived from the
    CPrintDialog and CPageSetupDialog classs.
    
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

    Declaration of the MyPrinter/MyPageSetup classes

*******************************************************************************/


#ifndef MYPRINTER_H
#define MYPRINTER_H

/******************************************************************************/
    
#include "ListBoxDlgRC.h"

#define TF(b) (b ? _T("true") : _T("false"))

/*============================================================================*/
    class
MyPrinter : public CPrintDialog                                 /*

*-----------------------------------------------------------------------------*/
{
    public:
        MyPrinter(
            DWORD dwFlags = PD_ALLPAGES | PD_USEDEVMODECOPIES |
            PD_NOPAGENUMS | PD_HIDEPRINTTOFILE | PD_NOSELECTION)
            :   CPrintDialog(dwFlags)
        {
        }

        void SetBoxTitle (const CString& title) {m_sPDTitle  = title;}

    protected:
        friend class MyPageSetup;
        
        virtual BOOL OnInitDialog()
        //  Override this member method to perform special processing
        //  when the printer box is initialized. Return TRUE.
        {
              // do the base class initialization first
            CDialog::OnInitDialog();
            SetWindowTitle();
            return TRUE;
        }

        virtual void OnCancel()
        {
              // for now just announce the dialog as cancelled
            ::MessageBox(NULL, _T("Print job cancelled."),
                _T("Information"), MB_OK | MB_ICONINFORMATION |
                MB_TASKMODAL);
        }

        virtual void OnOK()
        {
            // TODO: do needed preparation to print the document
        }


        virtual void SetWindowTitle() const
        {
            SetWindowText(m_sPDTitle);
        }

    private:
        CString m_sPDTitle;     // persistent over span of object

};

/*============================================================================*/
    class
MyPageSetup : public CPageSetupDialog                               /*

*-----------------------------------------------------------------------------*/
{
    public:
        MyPageSetup(DWORD dwFlags = PSD_MARGINS)
            :   CPageSetupDialog(dwFlags)
        {
        }

        void SetBoxTitle(const CString& title) {m_sPSDTitle = title;}

    protected:
        friend class MyPrinter;
        
        virtual BOOL OnInitDialog()
        //  Override this member method to perform special processing
        //  when the printer box is initialized. Return TRUE.
        {
              // do the base class initialization first
            CDialog::OnInitDialog();
            SetWindowTitle();
            return TRUE;
        }

        virtual void OnCancel()
        {
            // for now just announce the dialog as cancelled
            ::MessageBox(NULL, _T("OnCancel"), _T("Information"),
            MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
        }

        virtual void OnOK()
        {
            // TODO: store the page parameters for actual use
        }

        virtual void SetWindowTitle() const
        {
            SetWindowText( m_sPSDTitle);
        }

    private:
        CString  m_sPSDTitle;

};
/*----------------------------------------------------------------------------*/
#endif  // MYPRINTER_H


