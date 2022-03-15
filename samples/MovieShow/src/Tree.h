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
    CViewTree();
    virtual ~CViewTree();
    void Swap(HTREEITEM item1, HTREEITEM item2);

protected:
    // Virtual functions that override base class functions
    virtual void    OnDestroy();
    virtual void    OnInitialUpdate();
    virtual LRESULT OnNotifyReflect(WPARAM wparam, LPARAM lparam);
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    bool      IsBoxSetUnique(LPCTSTR text, HTREEITEM item);

    // Notification message handlers
    BOOL OnBeginLabelEdit(LPARAM lparam);
    BOOL OnEndLabelEdit(LPARAM lparam);
    BOOL OnSelChanged();

    // Member variables
    CImageList m_imlNormal;
    CString m_itemText;
};


////////////////////////////////////////////////////////
// CDockTree manages the resizing of the treeview window
// when the splitter bar is dragged.
class CDockTree : public CDocker
{
public:
    CDockTree();
    virtual ~CDockTree() {}
    CViewTree& GetViewTree() { return m_treeView; }

private:
    CViewTree m_treeView;
};


#endif  //_TREE_H
