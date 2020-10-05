///////////////////////////////////////
// DlgSubclassApp.h


#ifndef DLGSUBCLASSAPP_H
#define DLGSUBCLASSAPP_H

#include "MyDialog.h"


//////////////////////////////////////////////////////////////
// CDlgSubclassApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates the
// main dialog when it runs.
class CDlgSubclassApp : public CWinApp
{
public:
    CDlgSubclassApp();
    virtual ~CDlgSubclassApp();
    virtual BOOL InitInstance();
    CMyDialog& GetDialog() {return m_myDialog;}

private:
    CMyDialog m_myDialog;
};


// returns a pointer to the CDlgSubclassApp object
inline CDlgSubclassApp* GetSubApp() { return static_cast<CDlgSubclassApp*>(GetApp()); }


#endif // define DLGSUBCLASSAPP_H

