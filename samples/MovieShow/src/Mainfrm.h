/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "Tree.h"
#include "List.h"
#include "MyDialog.h"
#include "CoverImage.h"
#include "SplashThread.h"
#include "MovieInfo.h"

// Support older compilers.
#ifndef WM_DPICHANGED
#define WM_DPICHANGED                   0x02E0
#endif

///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame which has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CDockFrame
{
public:
    struct FoundFileInfo
    {
        FoundFileInfo() { ZeroMemory(&lastModifiedTime, sizeof(lastModifiedTime)); }

        CString     fileName;
        FILETIME    lastModifiedTime;
    };

    CMainFrame();
    virtual ~CMainFrame();
    virtual HWND Create(HWND parent = 0);
    void ClearDisplay();
    void ClearList();
    void FillImageData(const CString& source, std::vector<BYTE>& dest);
    void FillList();
    void FillListFromAllBoxSets();
    void FillListFromBoxSet(LPCTSTR boxset);
    void FillListFromDateRange(LPCTSTR dateRange);
    void FillListFromFlags(DWORD dwMask);
    void FillListFromGenre(LPCTSTR genre);
    void FillListFromGenres(LPCTSTR genreList);
    void FillListFromSearch();
    void FillListFromType(LPCTSTR videoType);
    void FillTreeItems();
    void ForceToForeground();

    bool IsVideoFile(const CString& filename) const;

    std::vector<CString> GetBoxSets();
    CString GetDataPath() const;

    CViewDialog& GetViewDialog() { return m_pDockDialog->GetViewDialog(); }
    CViewList& GetViewList() { return m_viewList; }
    CViewTree& GetViewTree() { return m_pDockTree->GetViewTree(); }
    std::vector<CString> GetWords(const CString& str) const;
    bool    IsWordInString(const CString& sentence, const CString& word) const;
    void    LoadMovieInfoFromFile(const FoundFileInfo& ffi, MovieInfo& movie);
    void    LoadMovies();
    BOOL    OnAddBoxSet();
    BOOL    OnAddFolder();
    BOOL    OnBoxSet(UINT nID);
    LRESULT OnDPIChanged();
    BOOL    OnFavourite();
    void    OnFilesLoaded();
    BOOL    OnPlay();
    BOOL    OnMoveDown();
    BOOL    OnMoveUp();
    BOOL    OnRenameBoxSet();
    BOOL    OnRemoveBoxSet();
    LRESULT OnRClickListItem();
    LRESULT OnRClickTreeItem();
    BOOL    OnRemoveFile();
    BOOL    OnSearch();
    LRESULT OnSelectListItem(const MovieInfo* pmi);
    LRESULT OnSelectTreeItem();
    LRESULT OnSysCommand(UINT msg, WPARAM wparam, LPARAM lparam);
    BOOL    OnVideoType(LPCTSTR videoType);
    BOOL    OnWatchList();
    LRESULT OnWindowPosChanged(UINT msg, WPARAM wparam, LPARAM lparam);

    LRESULT PlayMovie(LPCTSTR path);
    std::list<MovieInfo>* GetMoviesData() { return &m_moviesData; }

    static UINT WINAPI ThreadProc(void* pVoid);

protected:
    virtual BOOL    LoadRegistrySettings(LPCTSTR szKeyName);
    virtual LRESULT OnBarEnd(LPDRAGPOS pDragPos);
    virtual void    OnClose();
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual void    OnInitialUpdate();
    virtual void    OnMenuUpdate(UINT nID);
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual BOOL    SaveRegistrySettings();
    virtual void    SetupMenuIcons();
    virtual void    SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CCriticalSection m_cs;
    CViewList        m_viewList;
    CWinThread       m_thread;
    CSplashThread    m_splashThread;
    std::vector<FoundFileInfo> m_filesToAdd;
    std::vector<const MovieInfo*> m_foundMovies;
    HTREEITEM        m_searchItem;

    // Use lists because pointers to members of a list are always valid.
    std::list<CString> m_boxSets;
    std::list<CString> m_decades;
    std::list<CString> m_genres;

    MoviesData   m_moviesData;
    CImageList   m_toolbarImages;
    CMenu        m_boxSetMenu;
    CMenu        m_popupMenu;

    CDockTree*   m_pDockTree;
    CDockDialog* m_pDockDialog;
    CEvent       m_stopRequest;     // An event to signal the event thread should stop
    bool         m_isDirty;         // Has m_MoviesData has been modified?
    HTREEITEM    m_boxSetsItem;
    int          m_dialogWidth;
    int          m_treeHeight;
};

#endif //MAINFRM_H
