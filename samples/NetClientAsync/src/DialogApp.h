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

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance();

private:
    CDialogApp(const CDialogApp&);                // Disable copy construction
    CDialogApp& operator = (const CDialogApp&);   // Disable assignment operator

    CClientDialog m_clientDialog;
};


#endif // define DIALOGAPP_H
