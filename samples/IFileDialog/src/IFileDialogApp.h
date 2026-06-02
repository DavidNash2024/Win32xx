/////////////////////////////
// IFileDialogApp.h
//

#ifndef _IFILE_DIALOG_APP_
#define _IFILE_DIALOG_APP_

#include "MyTaskDialog.h"


//////////////////////////////////////////////////////////////////////
// CIFileDialogApp manages the application. It initializes the Win32++
// framework when it is constructed, and create the main task
// dialog window when it runs.
class CIFileDialogApp : public CWinApp
{
public:
    CIFileDialogApp() = default;
    virtual ~CIFileDialogApp() override = default;

protected:
    virtual BOOL InitInstance() override;

private:
    CIFileDialogApp(const CIFileDialogApp&) = delete;
    CIFileDialogApp& operator=(const CIFileDialogApp&) = delete;

    CMyTaskDialog myTaskDialog;
};


#endif // define _IFILE_DIALOG_APP_
