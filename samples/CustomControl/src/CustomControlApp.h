/////////////////////////////
// CustomControlApp.h
//

#ifndef CUSTOMCONTROLAPP_H
#define CUSTOMCONTROLAPP_H

#include "MyDialog.h"

//////////////////////////////////////
// Declaration of the CDialogApp class
//
class CCustomControlApp : public CWinApp
{
public:
    CCustomControlApp();
    virtual ~CCustomControlApp();
    virtual BOOL InitInstance();

private:
    CMyDialog m_myDialog;
};


#endif // define CUSTOMCONTROLAPP_H
