/* (12-Jun-2015) [Tab/Indent: 8/8][Line/Box: 80/74]                    (App.h) *
********************************************************************************
|                                                                              |
|                      Author: Robert C. Tausworthe, 2020                      |
|                                                                              |
===============================================================================*

    Contents Description: The DDXDemo CApp class declaration. This class
    governs the overall actions of the DDX/DDV Demonstration program.
    This class is a modified version of that found in the FormDocView sample
    distributed with the Win32++ Windows interface classes. The modified sample
    program was based on code provided by Lynn Allan. This demo extends the
    given sample by application of Dialog Data Exchange and Validation (DDX/DDV).

    Programming Notes: The programming standards roughly follow those
    established by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
    Planning and Preparation Subsystem project for C++ programming.

    Acknowledgement: The author would like to thank and acknowledge the advice,
    critical review, insight, and assistance provided by David Nash in the
    development of this work.

*******************************************************************************/

#ifndef SDI_APP_H
#define SDI_APP_H

#include "Mainfrm.h"

/*============================================================================*/
    class
CApp : public CWinApp                                                   /*

    Declaration of the CApp class of the App-Frame-Doc-View architecture.
*-----------------------------------------------------------------------------*/
{
    public:
        CApp() {}
        virtual ~CApp() {}

        BOOL        InitInstance();
        CMainFrame* TheFrame() { return &m_frame; }

    private:
        CMainFrame m_frame;
};

/*============================================================================*/
    inline CApp*
TheApp()                                                                /*

    Global connection to the CApp object.
*-----------------------------------------------------------------------------*/
{
    return (CApp*)(GetApp());
}

/*----------------------------------------------------------------------------*/
#endif // define SDI_APP_H

