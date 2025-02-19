//////////////////////////////////////////////////////////
// Tree.h - Declaration of CViewTree and CDockTree classes
//

#ifndef _TREE_H
#define _TREE_H


///////////////////////////////////////////////////////////////////
// CViewTree manages a tree view control. The tree view control
// organises videos into groups such as genres, release dates,
// boxsets, video types (movie / live performance) and favourites.
class CViewTree : public CTreeView
{
public:
    CViewTree() = default;
    virtual ~CViewTree() override;
    CViewTree(const CViewTree&) = delete;
    CViewTree& operator=(const CViewTree&) = delete;

    void SetDPIImages();
    void Swap(HTREEITEM item1, HTREEITEM item2);

protected:
    // Virtual functions that override base class functions.
    virtual void    OnInitialUpdate() override;
    virtual LRESULT OnNotifyReflect(WPARAM wparam, LPARAM lparam) override;
    virtual void    PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    bool      IsBoxSetUnique(LPCWSTR text, HTREEITEM item);

    // Message handlers.
    LRESULT OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnRButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnRButtonUp(UINT msg, WPARAM wparam, LPARAM lparam);

    // Notification message handlers.
    BOOL OnBeginLabelEdit(LPARAM lparam);
    BOOL OnEndLabelEdit(LPARAM lparam);
    BOOL OnSelChanged();

    // Member variables.
    CString m_itemText;
};


////////////////////////////////////////////////////////
// CDockTree manages the resizing of the treeview window
// when the splitter bar is dragged.
class CDockTree : public CDocker
{
public:
    CDockTree();
    virtual ~CDockTree() override = default;
    CDockTree(const CDockTree&) = delete;
    CDockTree& operator=(const CDockTree&) = delete;

    CViewTree& GetViewTree() { return m_treeView; }

protected:
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewTree m_treeView;
};


#endif  //_TREE_H
