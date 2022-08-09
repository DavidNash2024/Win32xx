/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                    (App.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the CApp class for the CommonDialogs
    sample program using the Win32++ Windows interface classes. A global access
    function is also defined to the CApp object.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef SDI_APP_H
#define SDI_APP_H

/*============================================================================*/
    class
CApp : public CWinApp                                                       /*

    This application's app class, a pattern for developing new apps.
*-----------------------------------------------------------------------------*/
{
    public:
        CApp() {}
        virtual ~CApp(){}

        CString MakeAppDataPath(const CString&) const;

    protected:
        virtual BOOL    InitInstance();
        virtual BOOL    OnIdle(LONG);

    private:
        CApp(const CApp&);                // Disable copy construction
        CApp& operator = (const CApp&);   // Disable assignment operator

        CMainFrame  m_frame;              // the main frame object
};

/*============================================================================*/
    inline
CApp* TheApp()                                                              /*

    Global access to this application object.
*----------------------------------------------------------------------------*/
{
    return (CApp*)GetApp();
}
/*-----------------------------------------------------------------------------*/
#endif // define SDI_APP_H

