/////////////////////////////
// DialogApp.h
//

#ifndef DIALOGAPP_H
#define DIALOGAPP_H

#include "ClientDialog.h"

//////////////////////////////////////
// Declaration of the CDialogApp class
//
class CDialogApp : public CWinApp
{
public:
    CDialogApp();
    virtual ~CDialogApp();
    virtual BOOL InitInstance();

private:
    CClientDialog m_clientDialog;
};


#endif // define DIALOGAPP_H
