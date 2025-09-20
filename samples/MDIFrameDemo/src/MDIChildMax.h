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
    CViewMax() = default;
    virtual ~CViewMax() override = default;

protected:
    virtual void OnDraw(CDC& dc) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewMax(const CViewMax&) = delete;
    CViewMax& operator=(const CViewMax&) = delete;
};


////////////////////////////////////////////////////////////
// CMDIChildMax manages a MDI child window that is maximised
// when it is created. It uses CViewMax as its view window.
class CMDIChildMax : public CMDIChild
{
public:
    CMDIChildMax();
    virtual ~CMDIChildMax() override = default;

protected:
    virtual int OnCreate(CREATESTRUCT& cs) override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMDIChildMax(const CMDIChildMax&) = delete;
    CMDIChildMax& operator=(const CMDIChildMax&) = delete;

    CViewMax m_maxView;
    CMenu m_menu;
};

#endif  //MDICHILDMAX_H
