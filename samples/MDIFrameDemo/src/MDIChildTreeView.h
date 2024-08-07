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
    virtual void OnAttach();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CViewTree(const CViewTree&);               // Disable copy construction
    CViewTree& operator=(const CViewTree&);    // Disable assignment operator
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
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMDIChildTree(const CMDIChildTree&);                // Disable copy construction
    CMDIChildTree& operator=(const CMDIChildTree&);   // Disable assignment operator

    CViewTree m_treeView;
    CMenu m_menu;
};

#endif  //MDICHILDTREEVIEW_H
