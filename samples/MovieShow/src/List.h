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

// IDs for the sorting arrow
const int SHOW_NO_ARROW = 1;
const int SHOW_UP_ARROW = 2;
const int SHOW_DOWN_ARROW = 3;


///////////////////////////////////////////////////////
// CListView manages a list view control. The list view
// window displays the videos in the library.
class CViewList : public CListView
{
public:
    CViewList();
    virtual ~CViewList();

    void    AddItem(const MovieInfo& mi);
    void    SetLastColumnWidth();
    void    SortColumn(int column, bool isSortDown);
    void    UpdateItemImage(int item);

protected:
    // Virtual functions that override base class functions
    virtual void    OnAttach();
    virtual void    OnDestroy();
    virtual void    OnInitialUpdate();
    virtual LRESULT OnNotify(WPARAM, LPARAM lparam);
    virtual LRESULT OnNotifyReflect(WPARAM, LPARAM lparam);
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CViewList(const CViewList&);                // Disable copy construction
    CViewList& operator = (const CViewList&);   // Disable assignment operator

    static int CALLBACK CompareFunction(LPARAM lp1, LPARAM lp2, LPARAM pSortViewItems);

    // Message handlers
    LRESULT OnCustomDraw(LPNMCUSTOMDRAW pCustomDraw);
    LRESULT OnItemActivate(LPNMLISTVIEW pListView);
    LRESULT OnItemChanged(LPNMLISTVIEW pListView);
    LRESULT OnLVColumnClick(LPNMLISTVIEW pListView);
    LRESULT OnRClick();

    CString GetFileTime(FILETIME fileTime);
    void    SetColumn();
    BOOL    SetHeaderSortImage(int  columnIndex, int showArrow);

    // Member variables
    CImageList m_normal;
    CImageList m_small;
};


#endif  //_LIST_H
