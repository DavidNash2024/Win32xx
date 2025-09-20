/////////////////////////////
// SimpleMDIChild.h
//

#ifndef MDICHILDVIEW_H
#define MDICHILDVIEW_H


// User defined window message sent when the Simple MDI Child is created.
// WPARAM is a pointer to the view window.
#define UWM_SIMPLECREATED       (WM_APP + 0x0001)


///////////////////////////////////////////////////////////
// CSimpleView manages the view window for CSimpleMDIChild.
class CSimpleView : public CWnd
{
public:
    CSimpleView();
    virtual ~CSimpleView() override = default;
    COLORREF GetColor() { return m_color; }
    void SetColor(COLORREF color) { m_color = color; }

protected:
    virtual void OnDraw(CDC& dc) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CSimpleView(const CSimpleView&) = delete;
    CSimpleView& operator=(const CSimpleView&) = delete;

    // Message handlers
    LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);

    // Member variables
    COLORREF m_color;
};


////////////////////////////////////////////
// CSimpleMDIChild manages a MDI child window.
// It uses CSimpleView as its view window.
class CSimpleMDIChild : public CMDIChild
{
public:
    CSimpleMDIChild();
    virtual ~CSimpleMDIChild() override = default;

protected:
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int OnCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CSimpleMDIChild(const CSimpleMDIChild&) = delete;
    CSimpleMDIChild& operator=(const CSimpleMDIChild&) = delete;

    CSimpleView m_view;
    CMenu m_menu;
};


#endif  //MDICHILDVIEW_H
