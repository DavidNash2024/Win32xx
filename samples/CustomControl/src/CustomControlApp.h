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
    virtual ~CCustomControlApp() override = default;

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance() override;

private:
    CCustomControlApp(const CCustomControlApp&) = delete;
    CCustomControlApp& operator=(const CCustomControlApp&) = delete;

    CMyDialog m_myDialog;
};


#endif // define CUSTOMCONTROLAPP_H
