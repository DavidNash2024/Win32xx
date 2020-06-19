/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "tree.h"
#include "list.h"
#include "MyDialog.h"
#include "CoverImage.h"
#include "Splash.h"

// User defined message issued when the video library has been updated.
#define UWM_FILESLOADED      (WM_APP + 0x0001)

struct MovieInfo
{
    MovieInfo()
    {
        ZeroMemory(&lastModifiedTime, sizeof(lastModifiedTime));
        flags = 0;
    }

    std::vector<BYTE> imageData;
    FILETIME lastModifiedTime;
    CString fileName;
    CString movieName;
    CString duration;
    CString releaseDate;
    CString coverImage;
    CString description;
    CString genre;
    CString actors;
    CString videoType;  // Movie; Live Performance
    CString boxset;
    DWORD   flags;    // for favourites etc.
};

typedef std::list<MovieInfo> MoviesData;

// Declaration of the CMainFrame class
// The frame has a toolbar and statusbar.
// A resizable treeview, listview and dialog occupy the view area. 
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
    void FillListFromType(LPCTSTR videoType);
    void FillTreeItems();

    bool IsVideoFile(const CString& filename) const;

    std::vector<CString> GetBoxSets();
    CString GetDataPath() const;

    CViewDialog& GetViewDialog() { return m_pDockDialog->GetViewDialog(); }
    CViewList& GetViewList()  { return m_viewList; }
    CViewTree& GetViewTree() { return m_pDockTree->GetViewTree(); }  
    std::vector<CString> GetWords(const CString& str) const;
    bool IsWordInString(const CString& sentence, const CString& word) const;
    void LoadMovieInfoFromFile(const FoundFileInfo& ffi, MovieInfo& movie);
    void LoadMovies();
    BOOL OnAddBoxSet();
    BOOL OnAddFolder();
    BOOL OnBoxSet(UINT nID);
    BOOL OnFavourite();
    void OnFilesLoaded();
    BOOL OnPlay();
    BOOL OnMoveDown();
    BOOL OnMoveUp();
    BOOL OnRenameBoxSet();
    BOOL OnRemoveBoxSet();
    void OnRClickListItem();
    void OnRClickTreeItem();
    BOOL OnRemoveFile();
    BOOL OnSearch();
    void OnSelectListItem(const MovieInfo* pmi);
    void OnSelectTreeItem();
    BOOL OnVideoType(LPCTSTR videoType);

    void PlayMovie(LPCTSTR path);
    std::list<MovieInfo>& SetMoviesData() { return m_moviesData; }

    static UINT WINAPI ThreadProc(void* pVoid);

protected:
    virtual BOOL    LoadRegistrySettings(LPCTSTR szKeyName);
    virtual void    OnClose();
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual void    OnInitialUpdate();
    virtual void    OnMenuUpdate(UINT nID);
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual BOOL    SaveRegistrySettings();
    virtual void    SetupMenuIcons();
    virtual void    SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CCriticalSection m_cs;
    CViewList        m_viewList;
    CWinThread       m_thread;
    CSplash          m_splash;
    std::vector<FoundFileInfo> m_foundFiles;
    
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
    HANDLE       m_stopRequest;     // An event to signal the event thread should stop
    bool         m_isDirty;         // Has m_MoviesData has been modified?
    HTREEITEM    m_boxSetsItem;
    int          m_dialogWidth;
    int          m_treeHeight;
};

#endif //MAINFRM_H

