/////////////////////////////
// View.h
//

#ifndef VIEW_H
#define VIEW_H

#include "Edit.h"


//////////////////////////////////////////
// CView manages CMainFrame's view window.
class CView : public CListView
{
public:
    CView();
    virtual ~CView();

protected:
    // Virtual functions that override base class functions.
    virtual void    OnAttach();
    virtual LRESULT OnNotifyReflect(WPARAM wparam, LPARAM lparam);
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    // Message Handlers.
    LRESULT OnClick(LPARAM lparam);
    LRESULT OnCustomDraw(LPARAM lparam);
    LRESULT OnItemChanged(LPARAM lparam);
    LRESULT OnUpdateText();

    // Private functions.
    void InsertItems();
    void SetColumns();

    // Private members.
    CMyEdit m_edit;
    int m_editRow;
    int m_editColumn;
};


#endif // VIEW_H
