//////////////////////////////////////////////////////////
// MDIChildListView.h
//  Declaration of the CViewList and CMDIChildListView classes

#ifndef MDICHILDLISTVIEW_H
#define MDICHILDLISTVIEW_H

////////////////////////////////////
// Definition of the CViewList class
//
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

////////////////////////////////////////
// Definition of the CMDIChildList class
//
class CMDIChildList : public CMDIChild
{
public:
    CMDIChildList();
    virtual ~CMDIChildList();

protected:
    virtual int  OnCreate(CREATESTRUCT& cs);

private:
    CViewList m_listView;
    CMenu m_menu;

};

#endif  //MDICHILDLISTVIEW_H

