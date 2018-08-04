//////////////////////////////////////////////////////
// MyListView.h
//  Declaration of the CMyListView class

#ifndef MYLISTVIEW_H
#define MYLISTVIEW_H


#include "ShellWrapper.h"

using namespace ShellWrapper;

class CMyListView : public CListView
{
public:
    CMyListView();
    virtual ~CMyListView();
    void DisplayFolder(CShellFolder& parentFolder, Cpidl& cpidlParent, Cpidl& cpidlRel);
    void DoDisplay();
    void ViewLargeIcons();
    void ViewList();
    void ViewSmallIcons();
    void ViewReport();

protected:
    virtual void OnAttach();
    virtual void OnDestroy();
    virtual LRESULT OnNotifyReflect(WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnNMRClick(LPNMHDR pNMHDR);
    virtual LRESULT OnNMReturn(LPNMHDR pNMHDR);
    virtual LRESULT OnLVNDispInfo(NMLVDISPINFO* pdi);
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    class ListItemData  //A nested class inside CMyListView
    {
    public:
        ListItemData(Cpidl& cpidlParent, Cpidl& cpidlRel, CShellFolder& parentFolder);
        ~ListItemData();

        CShellFolder& GetParentFolder() {return m_parentFolder;}
        Cpidl& GetFullPidl() {return m_cpidlFull;}
        Cpidl& GetRelPidl()  {return m_cpidlRel;}

    private:
        Cpidl        m_cpidlFull;     //Fully Qualified PIDL
        Cpidl        m_cpidlRel;      //Relative PIDL
        CShellFolder m_parentFolder; //Parent IShellFolder

    };  //class ListItemData (nested class)

    typedef Shared_Ptr<ListItemData> ListItemDataPtr;

    static int CALLBACK CompareProc(LPARAM param1, LPARAM param2, LPARAM paramSort);
    void DeleteItems();
    void DoBackgroundMenu(CPoint& ptScreen);
    void DoContextMenu(CPoint& ptScreen);
    void DoDefault(int item);
    void DoItemMenu(LPINT pItems, UINT cbItems, CPoint& ptScreen);
    void EnumObjects(CShellFolder& cPFolder, Cpidl& cpidlFull);
    BOOL GetFileSizeText(HANDLE hFile, LPTSTR szSize);
    BOOL GetLastWriteTime(HANDLE hFile, LPTSTR lpszString);
    void SetImageLists();

    Cpidl         m_cpidlCurFull;  //Fully Qualified pidl
    CShellFolder  m_csfCurFolder;  //Current Folder
    CContextMenu2 m_ccm2;
    std::vector <ListItemDataPtr> m_pItems;

};

#endif  // MYLISTVIEW_H
