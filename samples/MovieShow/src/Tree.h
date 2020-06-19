//////////////////////////////////////////////////////////
// Tree.h - Declaration of CViewTree and CDockTree classes


#ifndef _TREE_H
#define _TREE_H


// Declaration of the CViewTree class
// Organises videos into groups such as genres, release dates,
//  boxsets, video types (movie / live performance) and favourites.
class CViewTree : public CTreeView
{
public:
    CViewTree();
    virtual ~CViewTree();
    
    HTREEITEM AddItem(HTREEITEM parent, LPCTSTR text, int imageIndex);
    CString*  GetItemString(HTREEITEM item);
    bool      IsBoxSetUnique(LPCTSTR text, HTREEITEM item);
    BOOL      OnBeginLabelEdit(LPARAM lparam);
    BOOL      OnEndLabelEdit(LPARAM lparam);
    BOOL      OnSelChanged();
    void      RemoveItem(HTREEITEM item);
    void      Swap(HTREEITEM item1, HTREEITEM item2);
    
protected:
    virtual void    OnDestroy();
    virtual void    OnInitialUpdate();
    virtual LRESULT OnNotifyReflect(WPARAM wparam, LPARAM lparam);
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CImageList m_imlNormal;
    std::list<CString> m_itemsText;
    std::map<HTREEITEM, CString*> m_itemMap; 
};


////////////////////////////////////////////////////////////
// Declaration of the CDockTree class
// Manages the resizing of the treeview window when the 
//  splitter bar is dragged.
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
