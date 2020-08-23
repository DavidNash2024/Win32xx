/* (11-Nov-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                    (App.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the CApp class for the FilePrintPreview 
    program using the Win32++ Windows interface classes. 

    Programming Notes: The programming standards roughly follow those 
    established by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
    Planning and Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef THE_APP_H
#define THE_APP_H

#include "MainFrm.h"

/*============================================================================*/
    class
CApp : public CWinApp                                                       /*

*-----------------------------------------------------------------------------*/
{
    public:
        CApp() {}
        virtual ~CApp()  {}

        virtual BOOL    InitInstance();
            CMainFrame& TheFrame() { return m_frame; }

    private:
        CMainFrame m_frame;
};

/*============================================================================*/
    inline CApp*
TheApp()                                                                    /*

    Return a reference to the CApp object.
*-----------------------------------------------------------------------------*/
{
    return static_cast<CApp*>(GetApp()); 
}

/*============================================================================*/
    inline CMainFrame& 
GetFrame()                                                      /*

    Return a reference to the CMainFrame object.
*-----------------------------------------------------------------------------*/
{
    return TheApp()->TheFrame();
}

/*============================================================================*/
    inline CRichView& 
GetRichView()                                                       /*

    Return a reference to the rich view object being previewed.
*-----------------------------------------------------------------------------*/
{
    return GetFrame().GetRichView();
}
/*----------------------------------------------------------------------------*/
#endif // THE_APP_H 
