/////////////////////////////
// DialogApp.h
//

#ifndef DIALOGAPP_H
#define DIALOGAPP_H

#include "ClientDialog.h"


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
    CClientDialog& GetDialog() {return m_clientDialog;}

private:
    CClientDialog m_clientDialog;
};


// returns a pointer to the CDialogApp object
inline CDialogApp* GetDlgApp() { return static_cast<CDialogApp*>(GetApp()); }


#endif // define DIALOGAPP_H
