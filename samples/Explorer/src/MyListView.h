/////////////////////////////
// MyListView.h
//

#ifndef MYLISTVIEW_H
#define MYLISTVIEW_H

#include "ShellWrapper.h"

using namespace ShellWrapper;

// Struct used by the compare function.
struct SortViewItems
{
    SortViewItems(int column, bool isSortDown) : m_column(column), m_isSortDown(isSortDown)
    {}
    int m_column;
    bool m_isSortDown;
};

// Enum for the sorting arrow.
enum SHOW_ARROW
{
    SHOW_NO_ARROW,
    SHOW_UP_ARROW,
    SHOW_DOWN_ARROW
};

//////////////////////////////////////////////////
// CMyListView manages a list view control.
// The list view control is used to display files.
class CMyListView : public CListView
{
public:
    CMyListView();
    virtual ~CMyListView();
    void DisplayFolder(CShellFolder& parentFolder, Cpidl& cpidlParent, Cpidl& cpidlRel);
    void DoDisplay();
    static ULONGLONG FileTimeToULL(FILETIME ft);
    BOOL SetHeaderSortImage(int  columnIndex, SHOW_ARROW showArrow);
    void SortColumn(int column, bool isSortDown);
    void ViewLargeIcons();
    void ViewList();
    void ViewSmallIcons();
    void ViewReport();

protected:
    virtual void OnAttach();
    virtual void OnDestroy();
    virtual LRESULT OnLVColumnClick(LPNMITEMACTIVATE pnmitem);
    virtual LRESULT OnLVNDispInfo(NMLVDISPINFO* pdi);
    virtual LRESULT OnNMRClick(LPNMHDR pNMHDR);
    virtual LRESULT OnNMReturn(LPNMHDR pNMHDR);
    virtual LRESULT OnNotifyReflect(WPARAM wparam, LPARAM lparam);
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    // ListItemData is a nested class inside CMyListView.
    // Each list view item has a pointer to ListItemData.
    class ListItemData
    {
    public:
        ListItemData(Cpidl& cpidlParent, Cpidl& cpidlRel, CShellFolder& parentFolder);
        ~ListItemData();

        CShellFolder& GetParentFolder() {return m_parentFolder;}
        Cpidl& GetFullCpidl() {return m_cpidlFull;}
        Cpidl& GetRelCpidl()  {return m_cpidlRel;}

        ULONGLONG m_fileSize;
        FILETIME  m_fileTime;
        CString   m_fileType;
        bool      m_isFolder;

    private:
        Cpidl        m_cpidlFull;     // Fully Qualified PIDL
        Cpidl        m_cpidlRel;      // Relative PIDL
        CShellFolder m_parentFolder;  // Parent IShellFolder

    };  // class ListItemData (nested class)

    // Note: Modern C++ compilers can use this typedef instead.
    // typedef std::shared_ptr<ListItemData> ListItemDataPtr;
    typedef Shared_Ptr<ListItemData> ListItemDataPtr;

    static int CALLBACK CompareFunction(LPARAM param1, LPARAM param2, LPARAM pSortViewItems);
    void DoBackgroundMenu(CPoint& ptScreen);
    void DoContextMenu(CPoint& ptScreen);
    void DoDefault(int item);
    void DoItemMenu(LPINT pItems, UINT items, CPoint& ptScreen);
    void EnumObjects(CShellFolder& cPFolder, Cpidl& cpidlFull);
    BOOL GetFileSizeText(ULONGLONG fileSize, LPTSTR string);
    BOOL GetLastWriteTime(FILETIME modified, LPTSTR string);
    void SetImageLists();

    Cpidl         m_cpidlCurFull;    // Fully Qualified pidl
    CShellFolder  m_csfCurFolder;    // Current Folder
    CContextMenu2 m_ccm2;
    std::vector <ListItemDataPtr> m_pItems; // vector of smart pointers.
};

#endif  // MYLISTVIEW_H
