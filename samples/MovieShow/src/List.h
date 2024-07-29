//////////////////////////////////////////////////////////
// List.h - Declaration of CViewList and CDockList classes


#ifndef _LIST_H
#define _LIST_H


struct MovieInfo;

// Struct used by the compare function.
struct SortViewItems
{
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
    virtual ~CViewList() override;
    CViewList(const CViewList&) = delete;               // Disable copy construction
    CViewList& operator=(const CViewList&) = delete;    // Disable assignment operator

    void    AddItem(const MovieInfo& mi);
    void    SetDPIImages();
    void    SetLastColumnWidth();
    void    SortColumn(int column, bool isSortDown);
    void    UpdateItemImage(int item);

protected:
    // Virtual functions that override base class functions
    virtual void    OnAttach() override;
    virtual void    OnInitialUpdate() override;
    virtual LRESULT OnNotify(WPARAM, LPARAM lparam) override;
    virtual LRESULT OnNotifyReflect(WPARAM, LPARAM lparam) override;
    virtual void    PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    static int CALLBACK CompareFunction(LPARAM lp1, LPARAM lp2, LPARAM pSortViewItems);

    // Message handlers
    LRESULT OnCustomDraw(LPNMCUSTOMDRAW pCustomDraw);
    LRESULT OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnItemActivate(LPNMLISTVIEW pListView);
    LRESULT OnItemChanged(LPNMLISTVIEW pListView);
    LRESULT OnLVColumnClick(LPNMLISTVIEW pListView);
    LRESULT OnRClick();
    LRESULT OnWindowPosChanged(UINT msg, WPARAM wparam, LPARAM lparam);

    CHeader m_header;
    CString GetFileTime(FILETIME fileTime);
    void    SetColumn();
    BOOL    SetHeaderSortImage(int  columnIndex, int showArrow);

    // Member variables
    int m_oldDPI;
};


#endif  //_LIST_H
