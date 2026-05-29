/////////////////////////////////////////
// App.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef SDI_APP_H
#define SDI_APP_H

#include "Mainfrm.h"

////////////////////////////////////////////////////////
// The CApp class manages the application instance.
class CApp : public CWinApp
{
public:
    CApp() = default;
    virtual ~CApp() override = default;

protected:
    virtual BOOL InitInstance() override;

private:
    CApp(const CApp&) = delete;
    CApp& operator=(const CApp&) = delete;

    CMainFrame m_frame;
};

#endif // define SDI_APP_H
