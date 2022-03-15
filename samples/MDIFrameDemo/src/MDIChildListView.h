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
    virtual ~CViewList();

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach();
    virtual void OnDestroy();

private:
    void InsertItems();
    void SetColumns();

    CImageList m_smallImages;
};


////////////////////////////////////////////
// CMDIChildList manages a MDI child window.
// It use CViewList as its view window.
class CMDIChildList : public CMDIChild
{
public:
    CMDIChildList();
    virtual ~CMDIChildList();

protected:
    // Virtual functions that override base class functions
    virtual int  OnCreate(CREATESTRUCT& cs);

private:
    CViewList m_listView;
    CMenu m_menu;
};

#endif  //MDICHILDLISTVIEW_H

