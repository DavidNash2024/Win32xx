//////////////////////////////////////////////////////////
// MDIChildListView.h
//  Declaration of the CViewList and CMDIChildListView classes

#ifndef MDICHILDLISTVIEW_H
#define MDICHILDLISTVIEW_H


class CViewList : public CListView
{
public:
    CViewList();
    virtual ~CViewList();
    virtual int  AddItem(LPCTSTR text, int image);
    virtual void OnAttach();
    virtual void OnDestroy();
    virtual void InsertItems();
    virtual void SetColumns();

private:
    CImageList m_smallImages;

};


class CMDIChildListView : public CMDIChild
{
public:
    CMDIChildListView();
    virtual ~CMDIChildListView();

protected:
    virtual int  OnCreate(CREATESTRUCT& cs);

private:
    CViewList m_listView;
    CMenu m_menu;

};

#endif  //MDICHILDLISTVIEW_H

