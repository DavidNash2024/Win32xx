//////////////////////////////////////////////////////////
// MDIChildTreeView.h
//  Declaration of the CViewTree and CMDIChildTreeView classes

#ifndef MDICHILDTREEVIEW_H
#define MDICHILDTREEVIEW_H


//////////////////////////////////////////////////////
// CViewTree manages a tre view control. It is used as
// the view window for CMDIChildTree.
class CViewTree : public CTreeView
{
public:
    CViewTree();
    virtual ~CViewTree();

protected:
    // Virtual functions that override base class functions
    virtual void OnDestroy();
    virtual void OnAttach();

private:
    CImageList m_normalImages;
};


//////////////////////////////////////////////////////////////
// CMDIChildTree manages a MDI child window. It uses CViewTree
// as its view window.
class CMDIChildTree : public CMDIChild
{
public:
    CMDIChildTree();
    virtual ~CMDIChildTree();

protected:
    // Virtual functions that override base class functions
    virtual int  OnCreate(CREATESTRUCT& cs);

private:
    CViewTree m_TreeView;
    CMenu m_Menu;
};

#endif  //MDICHILDTREEVIEW_H
