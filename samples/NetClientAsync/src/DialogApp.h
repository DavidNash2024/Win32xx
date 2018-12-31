///////////////////////////////////////
// DialogApp.h 

#ifndef DIALOGAPP_H
#define DIALOGAPP_H

#include "ClientDialog.h"


// Declaration of the CDialogApp class
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

