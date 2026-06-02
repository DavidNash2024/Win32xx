/////////////////////////////////////////
// MyFileDlg.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef MY_FILE_DLG_H
#define MY_FILE_DLG_H

/////////////////////////////////////////////////////////
// MyFileDlg inherits from the Win32++ CFileDialog class.
// It adds a title to the dialog.
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

    CString m_title;
};

#endif // MY_FILE_DLG_H
