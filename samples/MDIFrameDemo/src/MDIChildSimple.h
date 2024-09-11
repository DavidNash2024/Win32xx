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
    virtual ~CViewSimple() override {}

    COLORREF GetColor() {return m_color;}
    void SetColor(COLORREF color) { m_color = color; }

protected:
    // Virtual functions that override base class functions
    virtual void OnDraw(CDC& dc) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewSimple(const CViewSimple&) = delete;
    CViewSimple& operator=(const CViewSimple&) = delete;

    COLORREF m_color;
};


//////////////////////////////////////////////////////////////////
// CMDIChildSimple manages a MDI child window. It uses CViewSimple
// as its view window.
class CMDIChildSimple : public CMDIChild
{
public:
    CMDIChildSimple();
    virtual ~CMDIChildSimple() override;

protected:
    // Virtual functions that override base class functions
    virtual void OnClose() override;
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMDIChildSimple(const CMDIChildSimple&) = delete;
    CMDIChildSimple& operator=(const CMDIChildSimple&) = delete;

    CViewSimple m_view;
    CMenu m_menu;
};

#endif  //MDICHILDVIEW_H
