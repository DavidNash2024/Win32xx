//////////////////////////////////////////////////
// FormApp.h

#ifndef SDI_APP_H
#define SDI_APP_H


#include "Mainfrm.h"


///////////////////////////////////////////////////////////////
// CFormApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CFormApp : public CWinApp
{
public:
    CFormApp();
    virtual ~CFormApp() override;

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance() override;

private:
    CFormApp(const CFormApp&) = delete;
    CFormApp& operator=(const CFormApp&) = delete;

    CMainFrame m_frame;
};


#endif // define SDI_APP_H
