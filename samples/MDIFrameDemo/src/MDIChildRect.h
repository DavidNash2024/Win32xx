//////////////////////////////////////////////////////
// MDIChildRect.h
//  Declaration of the CViewRect and CMDIChildRect classes

#ifndef MDICHILDRECT_H
#define MDICHILDRECT_H


///////////////////////////////////////////////////////////
// CViewRect manages a window that displays rectangles with
// a random size and color. It is used as the view window
// for CMDIChildRect.
class CViewRect : public CWnd
{
public:
    CViewRect() : m_cxClientMax(0), m_cyClientMax(0) {}

protected:
    // Virtual functions that override base class functions
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnDestroy();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CViewRect(const CViewRect&);                // Disable copy construction
    CViewRect& operator = (const CViewRect&);   // Disable assignment operator

    // Message Handlers
    virtual LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnTimer(UINT msg, WPARAM wparam, LPARAM lparam);

    // Member variables
    int m_cxClientMax;
    int m_cyClientMax;
};


//////////////////////////////////////////////////////////////
// CMDIChildRect manages a MDI child window. It uses CViewRect
// as its view window.
class CMDIChildRect : public CMDIChild
{
public:
    CMDIChildRect();
    virtual ~CMDIChildRect();

protected:
    // Virtual functions that override base class functions
    virtual int OnCreate(CREATESTRUCT& cs);

private:
    CMDIChildRect(const CMDIChildRect&);                // Disable copy construction
    CMDIChildRect& operator = (const CMDIChildRect&);   // Disable assignment operator

    CViewRect m_rectView;
    CMenu m_menu;
};


#endif  //MDICHILDRECT_H
