//////////////////////////////////////////////////
// FormApp.h

#ifndef SDI_APP_H
#define SDI_APP_H


#include "Mainfrm.h"


/////////////////////////////////////
// Declaration of the CFrameApp class
//
class CFormApp : public CWinApp
{
public:
    CFormApp();
    virtual ~CFormApp();
    virtual BOOL InitInstance();

private:
    CMainFrame m_frame;

};


#endif // define SDI_APP_H
