/////////////////////////////
// MyTreeView.h
//

#ifndef MYTREEVIEW_H
#define MYTREEVIEW_H

#include "ShellWrapper.h"

using namespace ShellWrapper;


//////////////////////////////////////////////////.
// CMyTreeView manages a tree view control.
// The tree view control is used to display folders.
class CMyTreeView : public CTreeView
{
public:
    CMyTreeView();
    virtual ~CMyTreeView() override;
    BOOL GetRootItems();
    BOOL SelectFromListView(Cpidl& cpidlFull);

protected:
    // Virtual functions that override base class functions
    virtual void    OnAttach() override;
    virtual void    OnDestroy() override;
    virtual LRESULT OnNMRClick(LPNMHDR pNMHDR);
    virtual LRESULT OnNotifyReflect(WPARAM wparam, LPARAM lparam) override;
    virtual LRESULT OnTVNGetDispInfo(LPNMTVDISPINFO pNMTVDI);
    virtual LRESULT OnTVNExpanding(LPNMTREEVIEW pNMTV);
    virtual LRESULT OnTVNSelChanged(LPNMTREEVIEW pNMTV);
    virtual void    PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMyTreeView(const CMyTreeView&) = delete;
    CMyTreeView& operator=(const CMyTreeView&) = delete;

    // TreeItemData is a nested class inside CMyTreeView.
    // Each tree view item has a pointer to TreeItemData.
    class TreeItemData
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
        CShellFolder m_parentFolder;    //Parent Folder

    };  //class TreeItemData

    // Note: Modern C++ compilers can use this typedef instead.
    // typedef std::shared_ptr<TreeItemData> TreeItemDataPtr;
    typedef std::unique_ptr<TreeItemData> TreeItemDataPtr;

    static int CALLBACK CompareFunction(LPARAM param1, LPARAM param2, LPARAM paramSort);

    void DoContextMenu(CPoint& point);
    void DoItemMenu(HTREEITEM item, CPoint& point);
    void EnumObjects(HTREEITEM parentItem, CShellFolder& parentFolder, Cpidl& cpidlParent);
    BOOL GetChildItems(HTREEITEM parentItem);

    // Member variables
    CContextMenu2 m_ccm2;
    std::vector <TreeItemDataPtr> m_pItems; // vector of smart pointers.
};

#endif  // MYTREEVIEW_H
