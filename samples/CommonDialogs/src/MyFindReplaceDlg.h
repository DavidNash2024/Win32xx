/* (08-Feb-2016)                                          (MyFindReplaceDlg.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Declaration and implementation of the MyFindREplaceDlg
    class. This class is derived from the CFindREplaceDalog class found in the
    Win32++ Windows interface framework in order to add a title to the dialog.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef MY_FIND_REPLACE_DLG_H
#define MY_FIND_REPLACE_DLG_H

/*============================================================================*/
    class
MyFindReplaceDialog : public CFindReplaceDialog

/*----------------------------------------------------------------------------*/
{
    public:
        MyFindReplaceDialog() {}
        virtual ~MyFindReplaceDialog() {}

          // Record the title of the find/replace dialog box after an object
          // of this class is constructed, but before DoModal() is invoked.
        virtual void SetBoxTitle(LPCTSTR title) {m_boxTitle = title; }

    protected:
        virtual BOOL OnInitDialog(){ SetWindowText(m_boxTitle); return TRUE; }

    private:
        MyFindReplaceDialog(const MyFindReplaceDialog&);                // Disable copy construction
        MyFindReplaceDialog& operator = (const MyFindReplaceDialog&);   // Disable assignment operator

        CString m_boxTitle;
};
/*----------------------------------------------------------------------------*/
#endif // MY_FIND_REPLACE_DLG_H
