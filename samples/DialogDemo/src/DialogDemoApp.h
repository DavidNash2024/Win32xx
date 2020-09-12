///////////////////////////////////////
// DialogDemoApp.h
//

#ifndef DLGSUBCLASSAPP_H
#define DLGSUBCLASSAPP_H

#include "MyDialog.h"

//////////////////////////////////////////
// Declaration of the CDialogDemoApp class
//
class CDialogDemoApp : public CWinApp
{
public:
    CDialogDemoApp();
    virtual ~CDialogDemoApp();
    virtual BOOL InitInstance();

private:
    CMyDialog m_myDialog;
};


#endif // define DLGSUBCLASSAPP_H
