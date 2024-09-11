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
    virtual ~CViewTree() override;

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewTree(const CViewTree&) = delete;
    CViewTree& operator=(const CViewTree&) = delete;
};


//////////////////////////////////////////////////////////////
// CMDIChildTree manages a MDI child window. It uses CViewTree
// as its view window.
class CMDIChildTree : public CMDIChild
{
public:
    CMDIChildTree();
    virtual ~CMDIChildTree() override;

protected:
    // Virtual functions that override base class functions
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMDIChildTree(const CMDIChildTree&) = delete;
    CMDIChildTree& operator=(const CMDIChildTree&) = delete;

    CViewTree m_treeView;
    CMenu m_menu;
};

#endif  //MDICHILDTREEVIEW_H
