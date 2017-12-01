/////////////////////////////
// Hyperlink.h

#ifndef HYPERLINK_H
#define HYPERLINK_H

#include "wxx_wincore.h"

#ifndef IDC_HAND
#define IDC_HAND  MAKEINTRESOURCE(32649)
#endif

class CHyperlink :  public CWnd
{
public:
    CHyperlink();
    virtual ~CHyperlink();
    virtual BOOL AttachDlgItem(UINT nID, CWnd& Parent);
    void OnLButtonDown();
    void OnLButtonUp(LPARAM lParam);

protected:
    virtual void OpenUrl();
    virtual LRESULT OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    BOOL    m_bUrlVisited;
    BOOL    m_bClicked;
    COLORREF m_crVisited;
    COLORREF m_crNotVisited;
    HCURSOR m_hCursor;
    CFont   m_UrlFont;
};

#endif // HYPERLINK_H

