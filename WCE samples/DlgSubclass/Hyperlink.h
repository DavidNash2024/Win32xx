/////////////////////////////
// Hyperlink.h


#ifndef HYPERLINK_H
#define HYPERLINK_H

#include "wxx_wincore.h"

#ifndef IDC_HAND
#define IDC_HAND  MAKEINTRESOURCE(32649)
#endif


//////////////////////////////////////////////////
// CHyperlink manages the hyperlink. The hyperlink
// is a modified static control.
class CHyperlink :  public CWnd
{
public:
    CHyperlink();
    virtual ~CHyperlink();
    virtual BOOL AttachDlgItem(UINT id, CWnd& parent);
    void OnLButtonDown();
    void OnLButtonUp(LPARAM lParam);

protected:
    virtual void OpenUrl();
    virtual LRESULT OnMessageReflect(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    BOOL    m_isUrlVisited;
    BOOL    m_isClicked;
    COLORREF m_visitedColor;
    COLORREF m_notVisitedColor;
    HCURSOR m_hCursor;
    CFont   m_urlFont;
};


#endif // HYPERLINK_H

