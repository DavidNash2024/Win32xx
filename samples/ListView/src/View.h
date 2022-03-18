/////////////////////////////
// View.h
//

#ifndef VIEW_H
#define VIEW_H


//////////////////////////////////////////
// CView manages CMainFrame's view window.
class CView : public CListView
{
public:
    CView();
    virtual ~CView();

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach();
    virtual LRESULT OnNotifyReflect(WPARAM wparam, LPARAM lparam);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    // Message Handlers
    LRESULT OnCustomDraw(LPARAM lParam);

    void InsertItems();
    void SetColumns();
};


#endif // VIEW_H
