/////////////////////////////
// View.h
//

#ifndef VIEW_H
#define VIEW_H

#include "Doc.h"


//////////////////////////////////////////
// CView manages CMainFrame's view window.
class CView : public CWnd
{
public:
    CView();
    virtual ~CView(){}
    CDoc& GetDoc();
    void PrintPage(CDC& dc, UINT page = 1);
    void QuickPrint(LPCTSTR docName);
    void SetDarkMode(bool isDarkMode) { m_isDarkMode = isDarkMode; }

protected:
    // Virtual functions that override base class functions
    virtual void    OnDraw(CDC& dc);
    virtual void    OnInitialUpdate();
    virtual void    PreRegisterClass(WNDCLASS& wc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView(const CView&);                // Disable copy construction
    CView& operator = (const CView&);   // Disable assignment operator

    CDoc m_doc;
    bool m_isDarkMode;
};


#endif // VIEW_H
