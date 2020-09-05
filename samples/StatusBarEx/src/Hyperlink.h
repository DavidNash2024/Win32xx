/* [Tab/Indent: 8/8][Line/Box: 80/74]                            (Hyperlink.h) *
********************************************************************************

    Contents Description: Declaration of the CHyperlink class for a sample
    program demonstrating the use of the status bar within the Win32++
    Windows framework.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef HYPERLINK_H
#define HYPERLINK_H

#ifndef IDC_HAND
#define IDC_HAND  MAKEINTRESOURCE(32649)
#endif

  // the hyperlink text visited/unvisited and background gradient colors
const COLORREF
        hyTextColor[2]  = {RGB(128,   0, 128), RGB(  0,   0, 255)},
        hyBkgrColor     =  RGB(125, 230, 255);

/*============================================================================*/
    class
CHyperlink :    public CStatic                                              /*

    Create a window that behaves like a hyperlink. Display a hand cursor when
    the mouse hovers over the text. Launch a browser and the Win32++ SourceForge
    page when the text is clicked. Adjust the text color.
*-----------------------------------------------------------------------------*/
{
    public:
        CHyperlink();
        virtual ~CHyperlink() {}

        virtual void    OnAttach();
        virtual LRESULT OnLButtonDown(UINT, WPARAM, LPARAM);
        virtual LRESULT OnLButtonUp(UINT, WPARAM, LPARAM);
        virtual LRESULT OnSetCursor(UINT, WPARAM, LPARAM);

    protected:
        virtual void    OpenUrl();
        virtual LRESULT OnMessageReflect(UINT, WPARAM, LPARAM);
        virtual LRESULT WndProc(UINT, WPARAM, LPARAM);

    private:
        BOOL     m_isUrlVisited;
        BOOL     m_isClicked;
        COLORREF m_crVisited;
        COLORREF m_crNotVisited;
        HCURSOR  m_cursor;
        CFont    m_urlFont;
};
/*----------------------------------------------------------------------------*/
#endif // HYPERLINK_H

