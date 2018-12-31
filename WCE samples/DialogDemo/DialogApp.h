//////////////////////////////////////////////////
// DialogApp.h
//  Declaration of the CDialogApp class

#ifndef DIALOGAPP_H
#define DIALOGAPP_H


#include "wxx_dialog.h"
#include "MyDialog.h"


class CDialogApp : public CWinApp
{
public:
    CDialogApp();
    virtual ~CDialogApp();
    virtual BOOL InitInstance();
    CMyDialog& GetDlg() { return myDialog; }

private:
    CMyDialog myDialog;
};


// returns a pointer to the CDialogApp object
inline CDialogApp* GetDlgApp() { return static_cast<CDialogApp*>(GetApp()); }


#endif // define DIALOGAPP_H

