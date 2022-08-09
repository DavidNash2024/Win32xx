///////////////////////////////////////
// DialogDemoApp.h
//

#ifndef DLGSUBCLASSAPP_H
#define DLGSUBCLASSAPP_H

#include "MyDialog.h"


/////////////////////////////////////////////////////////////
// CDialogDemoApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates the
// main dialog when it runs.
class CDialogDemoApp : public CWinApp
{
public:
    CDialogDemoApp();
    virtual ~CDialogDemoApp();

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance();

private:
    CDialogDemoApp(const CDialogDemoApp&);                // Disable copy construction
    CDialogDemoApp& operator = (const CDialogDemoApp&);   // Disable assignment operator

    CMyDialog m_myDialog;
};


#endif // define DLGSUBCLASSAPP_H
