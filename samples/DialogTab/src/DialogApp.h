///////////////////////////////////////
// DialogApp.h 

#ifndef DIALOGAPP_H
#define DIALOGAPP_H

#include "MyDialog.h"


// Declaration of the CDialogApp class
class CDialogApp : public CWinApp
{
public:
    CDialogApp(); 
    virtual ~CDialogApp();
    virtual BOOL InitInstance();
    CMyDialog& GetDialog() {return m_myDialog;}

private:
    CMyDialog m_myDialog;
};


// returns a reference to the CDialogApp object
inline CDialogApp& GetDlgApp() { return static_cast<CDialogApp&>(GetApp()); }


#endif // define DIALOGAPP_H

