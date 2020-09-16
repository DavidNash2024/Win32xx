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
    virtual ~CDialogApp();
    virtual BOOL InitInstance();
    CSvrDialog& GetDialog() {return m_svrDialog;}

private:
    CSvrDialog m_svrDialog;
};


// returns a pointer to the CDialogApp object
inline CDialogApp* GetDlgApp() { return static_cast<CDialogApp*>(GetApp()); }


#endif // define DIALOGAPP_H

