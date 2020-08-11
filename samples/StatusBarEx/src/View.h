/* [Tab/Indent: 8/8][Line/Box: 80/74]                                 (View.h) *
********************************************************************************

    Contents Description: Declaration of the CView class for a sample
    program demonstrating an expanded use of the status bar within the
    Win32++ Windows framework, Copyright (c) 2005-2017 David Nash.

    This particular program was adapted from the StatusBar sample progam
    in the Win32++ distribution by Robert C. Tausworthe to illustrate the
    definition of the status bar makeup by way of a data structure, in a
    similar fashion as that appearing in the Microsoft Foundation Classes
    (MFC), and to override the Win32++ normal statusbar definition of four
    parts to any number suitable for an application.

    The code changes to the StatusBar sample program only occur in the
    CMainFrame and MyStatusBar classes. However, the entire program has
    been reformatted for better readability, per the Programming Notes,
    below.

    These materials are provided "as is", without warranty of any kind,
    express or implied, including but not limited to: warranties of
    merchantability, fitness for a particular purpose, and non-infringement.

    Programming Notes:
                The programming conventions used here roughly follow those
        established by the 1997-1999 Jet Propulsion Laboratory Deep
        Space Network Planning and Preparation Subsystem project for
        C++ programming.

*******************************************************************************/

#ifndef VIEW_H
#define VIEW_H

/*============================================================================*/
    class
CView : public CWnd                         /*

*-----------------------------------------------------------------------------*/
{
    public:
        CView();
        virtual ~CView(){}

    protected:
        virtual void OnDraw(CDC& dc);
        virtual void OnInitialUpdate();
        virtual void PreCreate(CREATESTRUCT& cs);
        virtual void PreRegisterClass(WNDCLASS& wc);
        virtual LRESULT WndProc(UINT, WPARAM, LPARAM);
};
/*----------------------------------------------------------------------------*/
#endif // VIEW_H
