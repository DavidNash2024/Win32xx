//////////////////////////////////////////////////////////
// List.h - Declaration of CViewList and CDockList classes


#ifndef _LIST_H
#define _LIST_H


struct MovieInfo;

// Struct used by the compare function.
struct SortViewItems
{
    SortViewItems(int column, bool isSortDown) : m_column(column), m_isSortDown(isSortDown)
    {}
    int m_column;
    bool m_isSortDown;
};

// Enum for the sorting arrow
enum SHOW_ARROW
{
    SHOW_NO_ARROW,
    SHOW_UP_ARROW,
    SHOW_DOWN_ARROW
};


///////////////////////////////////////////////////////
// CListView manages a list view control. The list view
// window displays the videos in the library.
class CViewList : public CListView
{
public:
    CViewList();
    virtual ~CViewList();

    void    AddItem(const MovieInfo& mi);
    CString GetFileTime(FILETIME fileTime);
    LRESULT OnItemActivate(LPNMITEMACTIVATE pnmitem);
    LRESULT OnItemChanged(LPNMITEMACTIVATE pnmitem);
    LRESULT OnLVColumnClick(LPNMITEMACTIVATE pnmitem);
    LRESULT OnRClick();
    void    SetColumn();
    BOOL    SetHeaderSortImage(int  columnIndex, SHOW_ARROW showArrow);
    void    SetLastColumnWidth();
    void    SortColumn(int column, bool isSortDown);
    void    UpdateItemImage(int item);

protected:
    virtual void    OnAttach();
    virtual void    OnDestroy();
    virtual void    OnInitialUpdate();
    virtual LRESULT OnNotifyReflect(WPARAM, LPARAM lParam);
    virtual void    PreCreate(CREATESTRUCT& cs);

private:
    static int CALLBACK CompareFunction(LPARAM lp1, LPARAM lp2, LPARAM pSortViewItems);

    CImageList m_normal;
    CImageList m_small;
};


#endif  //_LIST_H
