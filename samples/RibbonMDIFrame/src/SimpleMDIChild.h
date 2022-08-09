/////////////////////////////
// SimpleMDIChild.h
//

#ifndef MDICHILDVIEW_H
#define MDICHILDVIEW_H


// User defined window message sent when the Simple MDI Child is created
// WPARAM is a pointer to the view window.
#define UWM_SIMPLECREATED       (WM_APP + 0x0001)


///////////////////////////////////////////////////////////
// CSimpleView manages the view window for CSimpleMDIChild.
class CSimpleView : public CWnd
{
public:
    CSimpleView();
    COLORREF GetColor() { return m_color; }
    void SetColor(COLORREF color) { m_color = color; }

protected:
    // Virtual functions that override base class functions
    virtual void OnDraw(CDC& dc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CSimpleView(const CSimpleView&);                // Disable copy construction
    CSimpleView& operator = (const CSimpleView&);   // Disable assignment operator

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
    virtual ~CSimpleMDIChild();

protected:
    // Virtual functions that override base class functions
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int OnCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CSimpleMDIChild(const CSimpleMDIChild&);                // Disable copy construction
    CSimpleMDIChild& operator = (const CSimpleMDIChild&);   // Disable assignment operator

    CSimpleView m_view;
    CMenu m_menu;
};


#endif  //MDICHILDVIEW_H
