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
    CClientDialog& GetDialog() { return m_clientDialog; }

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance();

private:
    CDialogApp(const CDialogApp&);                // Disable copy construction
    CDialogApp& operator = (const CDialogApp&);   // Disable assignment operator

    CClientDialog m_clientDialog;
};


// returns a pointer to the CDialogApp object
inline CDialogApp* GetDlgApp() { return static_cast<CDialogApp*>(GetApp()); }


#endif // define DIALOGAPP_H
