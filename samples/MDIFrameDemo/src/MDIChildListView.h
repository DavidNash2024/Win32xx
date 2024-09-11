//////////////////////////////////////////////////////////
// MDIChildListView.h
//  Declaration of the CViewList and CMDIChildListView classes

#ifndef MDICHILDLISTVIEW_H
#define MDICHILDLISTVIEW_H


///////////////////////////////////////////
// CViewList manages a list view control.
// It is the view window for CMDIChildList.
class CViewList : public CListView
{
public:
    CViewList();
    virtual ~CViewList() override;

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewList(const CViewList&) = delete;
    CViewList& operator=(const CViewList&) = delete;

    void InsertItems();
    void SetColumns();
};


////////////////////////////////////////////
// CMDIChildList manages a MDI child window.
// It use CViewList as its view window.
class CMDIChildList : public CMDIChild
{
public:
    CMDIChildList();
    virtual ~CMDIChildList() override;

protected:
    // Virtual functions that override base class functions
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMDIChildList(const CMDIChildList&) = delete;
    CMDIChildList& operator=(const CMDIChildList&) = delete;

    CViewList m_listView;
    CMenu m_menu;
};

#endif  //MDICHILDLISTVIEW_H

