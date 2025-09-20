//////////////////////////////////////////////////////
// SplitterMDIChild.h
//  Declaration of the CSimpleView and CSplitterMDIChild class

#ifndef MDICHILDVIEW_H
#define MDICHILDVIEW_H


/////////////////////////////////////////////////////////
// CSimpleView manages a simple window. It is used as the
// view window by CDockSimple.
class CSimpleView : public CWnd
{
public:
    CSimpleView();
    ~CSimpleView() override = default;
    COLORREF GetColor() {return m_color;}
    void SetColor(COLORREF color) { m_color = color; }

protected:
    virtual void OnDraw(CDC& dc) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CSimpleView(const CSimpleView&) = delete;
    CSimpleView& operator=(const CSimpleView&) = delete;

    // Message handlers
    LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);

    // Member variables
    COLORREF m_color;
};


//////////////////////////////////////////
// CDockSimple manages a docker that has a
// CSimpleView view.
class CDockSimple : public CDocker
{
public:
    CDockSimple();
    virtual ~CDockSimple() override = default;

    CSimpleView& GetSimpleView() { return m_view; }

protected:
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CDockSimple(const CDockSimple&) = delete;
    CDockSimple& operator=(const CDockSimple&) = delete;

    CSimpleView m_view;
};



//////////////////////////////////////////////////////
// CSplitterMDIChild manages the MDI child window.
// It uses CDockSimple as its view window. CDockSimple
// as other dockers docked within it.
class CSplitterMDIChild : public CMDIChild
{
public:
    CSplitterMDIChild();
    virtual ~CSplitterMDIChild() override = default;

protected:
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual void    OnInitialUpdate() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CSplitterMDIChild(const CSplitterMDIChild&) = delete;
    CSplitterMDIChild& operator=(const CSplitterMDIChild&) = delete;

    // Command handlers
    BOOL    OnColor(COLORREF rgb);

    // Message handlers
    LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);

    // Member variables
    CDockSimple m_view;
};

#endif  //MDICHILDVIEW_H
