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

    void OnLButtonDown();
    void OnLButtonUp(LPARAM lparam);
    void OpenUrl();
    LRESULT OnSetCursor();

protected:
    virtual void OnAttach();

    virtual LRESULT OnMessageReflect(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    bool    m_isUrlVisited;
    bool    m_isClicked;
    COLORREF m_crVisited;
    COLORREF m_crNotVisited;
    HCURSOR m_hCursor;
    CFont   m_urlFont;
};

#endif // HYPERLINK_H

