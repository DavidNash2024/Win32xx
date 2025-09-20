/* (26-Mar-2025)                                                 (MyFileDlg.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Declaration and implementation of the MyFileDlg class,
    which is derived from the CFileDalog class found in the Win32++ Windows
    interface framework in order to add a title to the dialog.

*******************************************************************************/

#ifndef MY_FILE_DLG_H
#define MY_FILE_DLG_H

class MyFileDialog : public CFileDialog
{
public:
    MyFileDialog (
        BOOL bOpenFileDialog = TRUE,
        LPCWSTR sDefExt = nullptr,
        LPCWSTR sFileName = nullptr,
        DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        LPCWSTR  sFilter     = nullptr
    ) : CFileDialog(bOpenFileDialog, sDefExt, sFileName,  dwFlags, sFilter) {}

    virtual ~MyFileDialog() override = default;
    void SetBoxTitle(LPCWSTR title) { m_title = title; SetTitle(title);}

private:
    MyFileDialog(const MyFileDialog&) = delete;
    MyFileDialog& operator=(const MyFileDialog&) = delete;

    CString m_title;       // persistent over object span
};

#endif // MY_FILE_DLG_H
