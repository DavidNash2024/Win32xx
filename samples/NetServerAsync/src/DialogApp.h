///////////////////////////////////////
// DialogApp.h 

#ifndef DIALOGAPP_H
#define DIALOGAPP_H


#include "SvrDialog.h"


// Declaration of the CDialogApp class
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

