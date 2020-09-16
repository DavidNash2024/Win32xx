/////////////////////////////
// CustomControlApp.h
//

#ifndef CUSTOMCONTROLAPP_H
#define CUSTOMCONTROLAPP_H

#include "MyDialog.h"


/////////////////////////////////////////////////////////////////
// CCustomControlApp manages the application. It initializes the
// Win32++ framework when it is constructed, and creates the main
// dialog when it runs.
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
