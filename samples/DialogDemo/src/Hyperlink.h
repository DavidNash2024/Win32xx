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
    void OnLButtonUp(LPARAM lParam);
    LRESULT OnSetCursor();

protected:
    virtual void OpenUrl();
    virtual LRESULT OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    BOOL    m_IsUrlVisited;
    BOOL    m_IsClicked;
    COLORREF m_crVisited;
    COLORREF m_crNotVisited;
    HCURSOR m_hCursor;
    CFont   m_UrlFont;
};

#endif // HYPERLINK_H

