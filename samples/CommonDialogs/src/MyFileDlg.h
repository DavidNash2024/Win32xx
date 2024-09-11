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
            LPCWSTR sDefExt      = nullptr,
            LPCWSTR sFileName    = nullptr,
            DWORD dwFlags        = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
            LPCWSTR  sFilter     = nullptr
        )   : CFileDialog(bOpenFileDialog, sDefExt, sFileName,
                dwFlags, sFilter) {}

          // Record the title of the read/saveas dialog box after an object
          // of this class is constructed, but before DoModal() is invoked.
        void SetBoxTitle(LPCWSTR title)
                        { m_title = title; SetTitle(title);}

    private:
        MyFileDialog(const MyFileDialog&) = delete;
        MyFileDialog& operator=(const MyFileDialog&) = delete;

        CString m_title;       // persistent over object span
};
/*----------------------------------------------------------------------------*/
#endif // MY_FILE_DLG_H
