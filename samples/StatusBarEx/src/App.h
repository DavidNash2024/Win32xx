/* [Tab/Indent: 8/8][Line/Box: 80/74]                                  (App.h) *
********************************************************************************

    Contents Description: Declaration of the CApp class for a sample program
    demonstrating the use of a custom status bar within the Win32++ Windows
    framework.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef STATUSBARAPP_H
#define STATUSBARAPP_H

#include "Mainfrm.h"

/*============================================================================*/
    class
CApp : public CWinApp                                                       /*

    Declaration of the CApp class
*-----------------------------------------------------------------------------*/
{
    public:
        CApp() {}
        virtual ~CApp() {}

        BOOL            InitInstance();
        CMainFrame&     GetMainFrame() { return m_frame; }

    private:
        CMainFrame m_frame;
};
/*-----------------------------------------------------------------------------*/
#endif // define STATUSBARAPP_H

