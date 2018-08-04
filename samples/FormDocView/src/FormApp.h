//////////////////////////////////////////////////
// FormApp.h

#ifndef SDI_APP_H
#define SDI_APP_H


#include "Mainfrm.h"


// Declaration of the CFrameApp class
class CFormApp : public CWinApp
{
public:
    CFormApp();
    virtual ~CFormApp();
    virtual BOOL InitInstance();
    CMainFrame& GetMainFrame() { return m_frame; }

private:
    CMainFrame m_frame;

};


// Handy global functions
inline CFormApp& GetSdiApp() { return static_cast<CFormApp&>(GetApp()); }


#endif // define SDI_APP_H

