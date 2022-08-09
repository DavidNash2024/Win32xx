/* (08-Feb-2016)                                                 (MyFileDlg.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Declaration and implementation of the MyFileDlg class,
    which is derived from the CFileDalog class found in the Win32++ Windows
    interface framework in order to add a title to the dialog.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef MY_FILE_DLG_H
#define MY_FILE_DLG_H

/*============================================================================*/
    class
MyFileDialog : public CFileDialog

/*----------------------------------------------------------------------------*/
{
    public:
        MyFileDialog
        (
            BOOL bOpenFileDialog = TRUE,
            LPCTSTR sDefExt      = NULL,
            LPCTSTR sFileName    = NULL,
            DWORD dwFlags        = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
            LPCTSTR  sFilter     = NULL
        )   : CFileDialog(bOpenFileDialog, sDefExt, sFileName,
                dwFlags, sFilter) {}

          // Record the title of the read/saveas dialog box after an object
          // of this class is constructed, but before DoModal() is invoked.
        virtual void SetBoxTitle(LPCTSTR title)
                        { m_title = title; SetTitle(title);}

    private:
        MyFileDialog(const MyFileDialog&);                // Disable copy construction
        MyFileDialog& operator = (const MyFileDialog&);   // Disable assignment operator

        CString m_title;       // persistent over object span
};
/*----------------------------------------------------------------------------*/
#endif // MY_FILE_DLG_H
