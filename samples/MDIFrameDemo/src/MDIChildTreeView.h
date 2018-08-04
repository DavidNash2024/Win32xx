//////////////////////////////////////////////////////////
// MDIChildTreeView.h
//  Declaration of the CViewTree and CMDIChildTreeView classes

#ifndef MDICHILDTREEVIEW_H
#define MDICHILDTREEVIEW_H


class CViewTree : public CTreeView
{
public:
    CViewTree();
    virtual ~CViewTree();
    virtual void OnDestroy();
    virtual void OnAttach();
    virtual HTREEITEM AddItem(HTREEITEM hParent, LPCTSTR text, int image);

private:
    CImageList m_normalImages;
};


class CMDIChildTreeView : public CMDIChild
{
public:
    CMDIChildTreeView();
    virtual ~CMDIChildTreeView();

protected:
    virtual int  OnCreate(CREATESTRUCT& cs);

private:
    CViewTree m_TreeView;
    CMenu m_Menu;
};

#endif  //MDICHILDTREEVIEW_H
