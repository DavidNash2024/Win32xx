//////////////////////////////////////////////////////
// MDIChildMax.h
//  Declaration of the CViewMax and CMDIChildMax classes

#ifndef MDICHILDMAX_H
#define MDICHILDMAX_H


////////////////////////////////////////////////////////
// CViewMax is used as the view window for CMDIChildMax.
class CViewMax : public CWnd
{
public:
    CViewMax() {}
    virtual ~CViewMax() {}

protected:
    // Virtual functions that override base class functions
    virtual void OnDraw(CDC& dc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CViewMax(const CViewMax&);                // Disable copy construction
    CViewMax& operator = (const CViewMax&);   // Disable assignment operator
};


////////////////////////////////////////////////////////////
// CMDIChildMax manages a MDI child window that is maximised
// when it is created. It uses CViewMax as its view window.
class CMDIChildMax : public CMDIChild
{
public:
    CMDIChildMax();
    virtual ~CMDIChildMax();

protected:
    // Virtual functions that override base class functions
    virtual int OnCreate(CREATESTRUCT& cs);
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    CMDIChildMax(const CMDIChildMax&);                // Disable copy construction
    CMDIChildMax& operator = (const CMDIChildMax&);   // Disable assignment operator

    CViewMax m_maxView;
    CMenu m_menu;
};

#endif  //MDICHILDMAX_H
