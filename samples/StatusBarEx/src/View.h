/* [Tab/Indent: 8/8][Line/Box: 80/74]                                 (View.h) *
********************************************************************************

    Contents Description: Declaration of the CView class for a sample
    program demonstrating a custom status bar within the Win32++ Windows
    framework.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef VIEW_H
#define VIEW_H

/*============================================================================*/
    class
CView : public CWnd                                                         /*

*-----------------------------------------------------------------------------*/
{
    public:
        CView() {}
        virtual ~CView(){}

    protected:
        void    OnDraw(CDC& dc);
        void    OnInitialUpdate();
        void    PreCreate(CREATESTRUCT& cs);
        void    PreRegisterClass(WNDCLASS& wc);
        LRESULT WndProc(UINT, WPARAM, LPARAM);
};
/*----------------------------------------------------------------------------*/
#endif // VIEW_H
