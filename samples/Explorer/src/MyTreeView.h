//////////////////////////////////////////////////////
// MyTreeView.h
//  Declaration of the CMyTreeView class

#ifndef MYTREEVIEW_H
#define MYTREEVIEW_H


#include "ShellWrapper.h"

using namespace ShellWrapper;


class CMyTreeView : public CTreeView
{
public:
    CMyTreeView();
    virtual ~CMyTreeView();
    BOOL GetRootItems();
    BOOL SelectFromListView(Cpidl& cpidlFull);

 protected:
    virtual void    OnAttach();
    virtual void    OnDestroy();
    virtual LRESULT OnNotifyReflect(WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnNMRClick(LPNMHDR pNMHDR);
    virtual LRESULT OnTVNGetDispInfo(LPNMTVDISPINFO pNMTVDI);
    virtual LRESULT OnTVNExpanding(LPNMTREEVIEW pNMTV);
    virtual LRESULT OnTVNSelChanged(LPNMTREEVIEW pNMTV);
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    class TreeItemData //A nested class inside CMyTreeView
    {
    public:
        TreeItemData(Cpidl& cpidlParent, Cpidl& cpidlRel, CShellFolder& parentFolder);
        TreeItemData(Cpidl& cpidl);
        ~TreeItemData();

        CShellFolder& GetParentFolder() {return m_parentFolder;}
        Cpidl& GetFullCpidl() {return m_cpidlFull;}
        Cpidl& GetRelCpidl()  {return m_cpidlRel;}

    private:
        Cpidl        m_cpidlFull;       //Fully Qualified PIDL
        Cpidl        m_cpidlRel;        //Relative PIDL
        CShellFolder m_parentFolder;   //Parent Folder

    };  //class TreeItemData

    typedef Shared_Ptr<TreeItemData> TreeItemDataPtr;

    static int CALLBACK CompareProc(LPARAM param1, LPARAM param2, LPARAM paramSort);
    void DeleteItems();
    void DoItemMenu(HTREEITEM item, CPoint& point);
    void DoContextMenu(CPoint& point);
    void EnumObjects(HTREEITEM parentItem, CShellFolder& parentFolder, Cpidl& cpidlParent);
    BOOL GetChildItems(HTREEITEM parentItem);

    CContextMenu2 m_ccm2;
    std::vector <TreeItemDataPtr> m_pItems;

};

#endif  // MYTREEVIEW_H
