/////////////////////////////
// Hyperlink.h
//

#ifndef HYPERLINK_H
#define HYPERLINK_H


#ifndef IDC_HAND
#define IDC_HAND  MAKEINTRESOURCE(32649)
#endif


//////////////////////////////////////////////////
// CHyperlink manages the hyperlink. The hyperlink
// is a modified static control.
class CHyperlink :  public CStatic
{
public:
    CHyperlink();
    virtual ~CHyperlink();

protected:
    // Virtual functions that override base class functions
    virtual void    OnAttach();
    virtual LRESULT OnMessageReflect(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CHyperlink(const CHyperlink&);                // Disable copy construction
    CHyperlink& operator = (const CHyperlink&);   // Disable assignment operator

    // Message handlers
    void OnLButtonDown();
    void OnLButtonUp(LPARAM lparam);
    void OpenUrl();
    LRESULT OnSetCursor();

    // Member variables
    bool    m_isUrlVisited;
    bool    m_isClicked;
    COLORREF m_crVisited;
    COLORREF m_crNotVisited;
    HCURSOR m_hCursor;
    CFont   m_urlFont;
};

#endif // HYPERLINK_H

