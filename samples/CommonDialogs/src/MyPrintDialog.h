/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]          (MyPrintDialog.h) *
********************************************************************************
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: The declaration and implementation of the
    MyPrintDialog and MyPageSetup classes, which are derived from the
    CPrintDialog and CPageSetupDialog class.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef MYPRINTER_H
#define MYPRINTER_H

/*============================================================================*/
    class
MyPrintDialog : public CPrintDialog                                         /*

*-----------------------------------------------------------------------------*/
{
    public:
        MyPrintDialog(DWORD dwFlags = PD_ALLPAGES | PD_USEDEVMODECOPIES |
            PD_NOPAGENUMS | PD_HIDEPRINTTOFILE | PD_NOSELECTION)
            :   CPrintDialog(dwFlags){}
        virtual ~MyPrintDialog(){}

        virtual void    SetBoxTitle (LPCTSTR title) {m_sPDTitle  = title;}

    private:
        virtual BOOL    OnInitDialog() { SetWindowTitle(); return TRUE; }
        virtual void    OnCancel()
                  { ::MessageBox(NULL, _T("Print job cancelled."),
                      _T("Information"), MB_OK | MB_ICONINFORMATION |
                      MB_TASKMODAL); }

          // override thid method to provide the needed preparation to
          // print the document upon user approval
        virtual void    OnOK() {}

        virtual void    SetWindowTitle() const { SetWindowText(m_sPDTitle);}

        CString m_sPDTitle;     // persistent over span of object
};

/*============================================================================*/
    class
MyPageSetup : public CPageSetupDialog                                   /*

*-----------------------------------------------------------------------------*/
{
    public:
        MyPageSetup(DWORD dwFlags = PSD_MARGINS)
            :   CPageSetupDialog(dwFlags) { }
        virtual ~MyPageSetup() {}

           // Record the title of the page setup dialog box after an object
          // of this class is constructed, but before DoModal() is invoked.
       virtual void SetBoxTitle(LPCTSTR title) {m_PSDTitle = title;}

    protected:
          //  Override this member method to perform special processing
          //  when the printer box is initialized. Return TRUE.
        virtual BOOL OnInitDialog() {SetWindowTitle(); return TRUE;}

          // Override this member to perform any special processing to reset
          // the printer to its incoming state. For now just announce that
          //the dialog was cancelled
        virtual void OnCancel()
                        { ::MessageBox(NULL, _T("Page setup cancelled. "),
                            _T("Information"), MB_OK | MB_ICONINFORMATION |
                            MB_TASKMODAL); }

          // override this member to store the page parameters for  use
          // in printing
        virtual void OnOK() {}
        virtual void SetWindowTitle() const { SetWindowText( m_PSDTitle);}

    private:
        CString  m_PSDTitle;
};
/*----------------------------------------------------------------------------*/
#endif  // MYPRINTER_H


