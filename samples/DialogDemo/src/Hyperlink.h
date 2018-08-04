/////////////////////////////
// Hyperlink.h

#ifndef HYPERLINK_H
#define HYPERLINK_H


#ifndef IDC_HAND
#define IDC_HAND  MAKEINTRESOURCE(32649)
#endif

class CHyperlink :  public CStatic
{
public:
    CHyperlink();
    virtual ~CHyperlink();
    virtual void OnAttach();
    void OnLButtonDown();
    void OnLButtonUp(LPARAM lparam);
    LRESULT OnSetCursor();

protected:
    virtual void OpenUrl();
    virtual LRESULT OnMessageReflect(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    BOOL    m_isUrlVisited;
    BOOL    m_isClicked;
    COLORREF m_crVisited;
    COLORREF m_crNotVisited;
    HCURSOR m_hCursor;
    CFont   m_urlFont;
};

#endif // HYPERLINK_H

