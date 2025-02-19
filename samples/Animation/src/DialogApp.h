/////////////////////////////
// DialogApp.h
//

#ifndef DIALOGAPP_H
#define DIALOGAPP_H

#include "MyDialog.h"

/////////////////////////////////////////////////////////////////
// CDialogApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main dialog
// when it runs.
class CDialogApp : public CWinApp
{
public:
    CDialogApp();
    virtual ~CDialogApp() override = default;

protected:
    virtual BOOL InitInstance() override;

private:
    CDialogApp(const CDialogApp&) = delete;
    CDialogApp& operator=(const CDialogApp&) = delete;

    CMyDialog m_myDialog;
};


#endif // define DIALOGAPP_H
