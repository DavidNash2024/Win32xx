/* (11-Nov-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                    (App.h) *
********************************************************************************

    Declaration of the CApp class for the FilePrintPreview program.

********************************************************************************

    Acknowledgment. This demo program was adapted from the PrintPreview 
    sample program appearing in the Win32++ framework sample folder, created 
    by  David Nash and published under the permissions granted in that work.
    The adaptation here reimplements the PrintView window as a separate 
    popup window that appears on screen along with the regular program 
    window. This work has been developed under the co-authorship of Robert
    Tausworthe and David Nash, and released under the copyright provisions 
    of the Win32++ Interface Classes software, copyright (c) David Nash,
    2005-2017. The former author acknowledges and thanks the latter for his 
    patient direction and inspiration in the development of the classes of 
    these classes.

*******************************************************************************/

#ifndef THE_APP_H
#define THE_APP_H

#include "MainFrm.h"

/*============================================================================*/
    class
CApp : public CWinApp                                                   /*

*-----------------------------------------------------------------------------*/
{
    public:
        CApp() {}
        virtual ~CApp()  {}

        virtual BOOL    InitInstance();
            CMainFrame& TheFrame() { return m_Frame; }

    private:
        CMainFrame m_Frame;
};


/*============================================================================*/
    inline CApp&
TheApp()                                /*

    Return a reference to the CApp object.
*-----------------------------------------------------------------------------*/
{
    return static_cast<CApp&>(GetApp()); 
}

/*============================================================================*/
    inline CRichView& 
GetRichView()                               /*

    Return a reference to the rich view object being previewed.
*-----------------------------------------------------------------------------*/
{
    return TheApp().TheFrame().GetRichView();
}
/*----------------------------------------------------------------------------*/
#endif // THE_APP_H
