/////////////////////////////
// View.h
//

#ifndef VIEW_H
#define VIEW_H

#include "Edit.h"

// Struct containing the list item data.
struct ListItemData
{
    ListItemData(CString subItem0, CString subItem1, CString subItem2)
    {
        m_subItemText.push_back(subItem0);
        m_subItemText.push_back(subItem1);
        m_subItemText.push_back(subItem2);
    }

    std::vector<CString> m_subItemText;
};

// Struct used by the compare function.
struct SortViewItems
{
    SortViewItems(int column, bool isSortDown) : m_column(column), m_isSortDown(isSortDown)
    {}
    int m_column;
    bool m_isSortDown;
};

// IDs for the sorting arrow.
const int SHOW_NO_ARROW = 1;
const int SHOW_UP_ARROW = 2;
const int SHOW_DOWN_ARROW = 3;

typedef Shared_Ptr<ListItemData> ListItemDataPtr;


//////////////////////////////////////////
// CView manages CMainFrame's view window.
class CView : public CListView
{
public:
    CView();
    virtual ~CView();
    void AddItem(CString subItem0, CString subItem1, CString subItem2);

protected:
    // Virtual functions that override base class functions.
    virtual void    OnAttach();
    virtual LRESULT OnNotifyReflect(WPARAM wparam, LPARAM lparam);
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    // Message Handlers.
    LRESULT OnClick(LPNMLISTVIEW pListView);
    LRESULT OnCustomDraw(LPNMLVCUSTOMDRAW pLVCustomDraw);
    LRESULT OnItemChanged(LPNMLISTVIEW pListView);
    LRESULT OnLVColumnClick(LPNMITEMACTIVATE pnmitem);
    LRESULT OnUpdateText();

    // Private functions.
    static int CALLBACK CompareFunction(LPARAM param1, LPARAM param2, LPARAM pSortViewItems);
    void AddAllItems();
    void SetColumns();
    void SetHeaderSortImage(int  columnIndex, int showArrow);
    void SortColumn(int column, bool isSortDown);

    // Private members.
    std::vector<ListItemDataPtr> m_allListItemData;  // vector of smart pointers
    CMyEdit m_edit;
    int m_row;
    int m_column;
};


#endif // VIEW_H
