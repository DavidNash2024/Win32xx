//////////////////////////////////////////////////////////
// MDIChildTreeView.h
//  Declaration of the CViewTree and CMDIChildTreeView classes

#ifndef MDICHILDTREEVIEW_H
#define MDICHILDTREEVIEW_H

/////////////////////////////////////
// Declaration of the CViewTree class
//
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

/////////////////////////////////////////////
// Declaration of the CMDIChildTree class
//
class CMDIChildTree : public CMDIChild
{
public:
    CMDIChildTree();
    virtual ~CMDIChildTree();

protected:
    virtual int  OnCreate(CREATESTRUCT& cs);

private:
    CViewTree m_TreeView;
    CMenu m_Menu;
};

#endif  //MDICHILDTREEVIEW_H
