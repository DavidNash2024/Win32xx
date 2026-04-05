/////////////////////////////////////////////
// AboutDialog.h

#ifndef _ABOUTDIALOG_H_
#define _ABOUTDIALOG_H_

#include "resource.h"

////////////////////////////////////////////////////////
// CAboutDialog manages the dialog which displays
// information about this program.
class CAboutDialog : public CDialog
{
public:
    CAboutDialog() : CDialog(IDW_ABOUT) {}
    virtual BOOL OnInitDialog() override
    {
        // Set the Win32++ version string.
        UINT ver = _WIN32XX_VER;
        CString Win32xxVersion;
        Win32xxVersion << "Win32++ Version " << ver / 0x100 << "." <<
            (ver % 0x100) / 0x10 << "." << (ver % 0x10);

        SetDlgItemText(IDC_STATIC1, Win32xxVersion);
        SetDlgItemText(IDC_STATIC2, L"by David Nash");

        return TRUE;
    }
};

#endif // _ABOUTDIALOG_H_
