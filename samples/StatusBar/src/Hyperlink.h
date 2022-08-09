/////////////////////////////
// Hyperlink.h
//

#ifndef HYPERLINK_H
#define HYPERLINK_H

#ifndef IDC_HAND
#define IDC_HAND  MAKEINTRESOURCE(32649)
#endif


////////////////////////////////////////////////////////////////////////
// CHyperlink creates a window that behaves like a hyperlink.
// * A hand cursor is displayed when the Mouse hovers over the text
// * A browser is launched and the Win32++ SourceForge page is displayed
//    when the text is clicked.
// * The text color is adjusted when the text is clicked.
class CHyperlink :  public CStatic
{
public:
    CHyperlink();
    virtual ~CHyperlink();

    const CString& GetLinkName() const { return m_urlName; }

protected:
    // Virtual functions that override base class functions
    virtual void    OnAttach();
    virtual LRESULT OnMessageReflect(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CHyperlink(const CHyperlink&);                // Disable copy construction
    CHyperlink& operator = (const CHyperlink&);   // Disable assignment operator

    // Message handlers
    LRESULT OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnSetCursor(UINT msg, WPARAM wparam, LPARAM lparam);

    void OpenUrl();

    // Member variables
    bool     m_isUrlVisited;
    bool     m_isClicked;
    COLORREF m_visitedColor;
    COLORREF m_notVisitedColor;
    HCURSOR  m_hCursor;
    CFont    m_urlFont;
    CString  m_urlName;
};


#endif // HYPERLINK_H
