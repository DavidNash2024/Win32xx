/* (26-Mar-2025)                                                       (App.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the CApp class for the CommonDialogs
    sample program using the Win32++ Windows interface classes. A global access
    function is also defined to the CApp object.

*******************************************************************************/

#ifndef SDI_APP_H
#define SDI_APP_H


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

    CMainFrame  m_frame;              // the main frame object
};


#endif // define SDI_APP_H
