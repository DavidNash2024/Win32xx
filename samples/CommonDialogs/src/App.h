/////////////////////////////////////////
// App.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef SDI_APP_H
#define SDI_APP_H

#include "Mainfrm.h"

/////////////////////////////////////////////////////////
// This class manages the application. It initializes the
// application, creates the main frame, and handles
// application-level messages.
class CApp : public CWinApp
{
public:
    CApp() = default;
    virtual ~CApp() override = default;

protected:
    virtual BOOL    InitInstance() override;
    virtual BOOL    OnIdle(LONG) override;

private:
    CApp(const CApp&) = delete;
    CApp& operator=(const CApp&) = delete;

    CMainFrame  m_frame;              // The main frame object.
};


#endif // define SDI_APP_H
