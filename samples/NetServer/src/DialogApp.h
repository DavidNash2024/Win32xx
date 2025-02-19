/////////////////////////////
// DialogApp.h
//

#ifndef DIALOGAPP_H
#define DIALOGAPP_H

#include "SvrDialog.h"


/////////////////////////////////////////////////////////////////
// CDialogApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main dialog
// when it runs.
class CDialogApp : public CWinApp
{
public:
    CDialogApp();
    virtual ~CDialogApp() override = default;
    CSvrDialog& GetDialog() {return m_svrDialog;}

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance() override;

private:
    CDialogApp(const CDialogApp&) = delete;
    CDialogApp& operator=(const CDialogApp&) = delete;

    CSvrDialog m_svrDialog;
};


// returns a pointer to the CDialogApp object
inline CDialogApp* GetDlgApp() { return static_cast<CDialogApp*>(GetApp()); }


#endif // define DIALOGAPP_H

