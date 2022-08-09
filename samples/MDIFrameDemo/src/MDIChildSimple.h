//////////////////////////////////////////////////////
// MDIChildSimple.h
//  Declaration of the CViewSimple and CMDIChildSimple class

#ifndef MDICHILDVIEW_H
#define MDICHILDVIEW_H


/////////////////////////////////////////////////////
// CViewSimple manages a simple window. It is used as
// the view window for CMDIChildSimple.
class CViewSimple : public CWnd
{
public:
    CViewSimple();
    virtual ~CViewSimple() {}

    COLORREF GetColor() {return m_color;}
    void SetColor(COLORREF color) { m_color = color; }

protected:
    // Virtual functions that override base class functions
    virtual void OnDraw(CDC& dc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CViewSimple(const CViewSimple&);                // Disable copy construction
    CViewSimple& operator = (const CViewSimple&);   // Disable assignment operator

    COLORREF m_color;
};


//////////////////////////////////////////////////////////////////
// CMDIChildSimple manages a MDI child window. It uses CViewSimple
// as its view window.
class CMDIChildSimple : public CMDIChild
{
public:
    CMDIChildSimple();
    virtual ~CMDIChildSimple();

protected:
    // Virtual functions that override base class functions
    virtual void OnClose();
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMDIChildSimple(const CMDIChildSimple&);                // Disable copy construction
    CMDIChildSimple& operator = (const CMDIChildSimple&);   // Disable assignment operator

    CViewSimple m_view;
    CMenu m_menu;
};

#endif  //MDICHILDVIEW_H
