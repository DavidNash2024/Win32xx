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

    void InsertItems();
    LRESULT OnCustomDraw(LPARAM lParam);
    void SetColumns();

protected:
    virtual void OnAttach();
    virtual LRESULT OnNotifyReflect(WPARAM wparam, LPARAM lparam);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
};


#endif // VIEW_H
