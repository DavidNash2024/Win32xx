/* [Tab/Indent: 8/8][Line/Box: 80/74]                            (Hyperlink.h) *
********************************************************************************

    Contents Description: Declaration of the CHyperlink class for a sample
    program demonstrating the use of the status bar using the Win32++
    Windows framework, Copyright (c) 2005-2017 David Nash.

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

#ifndef HYPERLINK_H
#define HYPERLINK_H


#ifndef IDC_HAND
#define IDC_HAND  MAKEINTRESOURCE(32649)
#endif

  // the hyperlink text visited/unvisited and background gradient colors
const COLORREF  HYTextColor[2]  = {RGB(128,   0, 128), RGB(  0,   0, 255)},
        HYBkgrColor =  RGB(125, 230, 255);

/*============================================================================*/
    class
CHyperlink :    public CStatic                      /*

    Creates a window that behaves like a hyperlink. A hand cursor is
    displayed when the Mouse hovers over the text. A browser is launched
    and the Win32++ SourceForge page is displayed when the text is clicked.
    The text color is adjusted.
*-----------------------------------------------------------------------------*/
{
    public:
        CHyperlink();
        virtual ~CHyperlink();
        virtual void OnAttach();
        LRESULT OnLButtonDown(UINT, WPARAM, LPARAM);
        LRESULT OnLButtonUp(UINT, WPARAM, LPARAM);
        LRESULT OnSetCursor(UINT, WPARAM, LPARAM);

    protected:
        virtual void OpenUrl();
        virtual LRESULT OnMessageReflect(UINT, WPARAM, LPARAM);
        virtual LRESULT WndProc(UINT, WPARAM, LPARAM);

    private:
        BOOL     m_IsUrlVisited;
        BOOL     m_IsClicked;
        COLORREF m_crVisited;
        COLORREF m_crNotVisited;
        HCURSOR  m_hCursor;
        CFont    m_UrlFont;
};
/*----------------------------------------------------------------------------*/
#endif // HYPERLINK_H

