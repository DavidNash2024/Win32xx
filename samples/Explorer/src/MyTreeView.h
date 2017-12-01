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
    BOOL SelectFromListView(Cpidl& CpidlFull);

 protected:
    virtual void    OnAttach();
    virtual void    OnDestroy();
    virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnNMRClick(LPNMHDR pNMHDR);
    virtual LRESULT OnTVNGetDispInfo(LPNMTVDISPINFO pNMTVDI);
    virtual LRESULT OnTVNExpanding(LPNMTREEVIEW pNMTV);
    virtual LRESULT OnTVNSelChanged(LPNMTREEVIEW pNMTV);
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    class TreeItemData //A nested class inside CMyTreeView
    {
    public:
        TreeItemData(Cpidl& cpidlParent, Cpidl& cpidlRel, CShellFolder& cParentFolder);
        TreeItemData(Cpidl& cpidl);
        ~TreeItemData();

        CShellFolder& GetParentFolder() {return m_cParentFolder;}
        Cpidl& GetFullCpidl() {return m_cpidlFull;}
        Cpidl& GetRelCpidl()  {return m_cpidlRel;}

    private:
        Cpidl        m_cpidlFull;       //Fully Qualified PIDL
        Cpidl        m_cpidlRel;        //Relative PIDL
        CShellFolder m_cParentFolder;   //Parent Folder

    };  //class TreeItemData

    typedef Shared_Ptr<TreeItemData> TreeItemDataPtr;

    static int CALLBACK CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
    void DeleteItems();
    void DoItemMenu(HTREEITEM hItem, CPoint& ptScreen);
    void DoContextMenu(CPoint& ptScreen);
    void EnumObjects(HTREEITEM hParentItem, CShellFolder& cParentFolder, Cpidl& cpidlParent);
    BOOL GetChildItems(HTREEITEM hParentItem);

    CContextMenu2 m_ccm2;
    std::vector <TreeItemDataPtr> m_pItems;

};

#endif  // MYTREEVIEW_H
