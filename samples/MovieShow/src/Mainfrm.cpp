////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "resource.h"
#include "mainfrm.h"
#include "SearchDialog.h"

#ifdef _MSC_VER
#pragma warning (disable : 4458) // disable declaration hides class member warning
#endif

#include <gdiplus.h>

#ifdef _MSC_VER
#pragma warning (default : 4458) // enable declaration hides class member warning
#endif

#if defined (_MSC_VER) && (_MSC_VER >= 1400)
#pragma warning ( push )
#pragma warning ( disable : 26812 )       // enum type is unscoped. 
#endif // (_MSC_VER) && (_MSC_VER >= 1400)

#include "MediaInfoDLL.h"

#if defined (_MSC_VER) && (_MSC_VER >= 1400)
#pragma warning ( pop )  // ( disable : 26812 )    enum type is unscoped.
#endif // (_MSC_VER) && (_MSC_VER >= 1400)

using namespace MediaInfoDLL;
using namespace Gdiplus;

// Retrieves the class identifier for the given MIME type of an encoder.
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in bytes

    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if (size == 0)
        return -1;  // Failure

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL)
        return -1;  // Failure

    GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT j = 0; j < num; ++j)
    {
#pragma warning ( push )
#pragma warning ( disable : 6385 )       // '208' bytse might be read.
        // Correct code incorrectly flaged with a C6385 warning by the VS2019 analyser.
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }
#pragma warning ( pop )  // ( disable : 6385 )    '208' bytse might be read.
    }

    free(pImageCodecInfo);
    return -1;  // Failure
}

// Constructor for the CMainFrame class
CMainFrame::CMainFrame() : m_thread(ThreadProc, this), m_pDockTree(0), m_pDockDialog(0),
                           m_isDirty(false), m_boxSetsItem(0), m_dialogWidth(0),
                           m_treeHeight(0)
{
    ZeroMemory(&m_mi, sizeof(m_mi));

    //Set m_View as the view window of the frame
    SetView(m_viewList);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(L"Win32++\\MovieShow");

    m_stopRequest = ::CreateEvent(0, TRUE, FALSE, 0);

    m_genres.push_back(L"Action, Crime, Thriller, War, Western");
    m_genres.push_back(L"Adventure, Horror, Fantasy, Science Fiction");
    m_genres.push_back(L"Animation, Family");
    m_genres.push_back(L"Comedy");
    m_genres.push_back(L"Documentary, History, Music, TV Movie");
    m_genres.push_back(L"Drama, Romance");

    int currentYear = CTime::GetCurrentTime().GetYear();
    m_decades.push_back(L"1900 - 1949");
    for (int year = 1950; year <= currentYear; year += 10)
    {
        CString str;
        str.Format(L"%d - %d", year, year + 9);
        m_decades.push_back(str);
    }
}

CMainFrame::~CMainFrame()
{
    // Destructor for CMainFrame.
}

// Converts a text string to a byte stream.
// Loads an image from the byte stream and converts it to a thumbnail.
// Saves the byte stream for the thumbnail in the specified vector.
void CMainFrame::FillImageData(const CString& source, std::vector<BYTE>& dest)
{
    DWORD size = 0;
    if (::CryptStringToBinary(source.c_str(), (DWORD)source.GetLength(), CRYPT_STRING_BASE64, NULL, &size, NULL, NULL))
    {
        // Use a vector for an array of BYTE
        std::vector<BYTE> Dest(size, 0);
        BYTE* pDest = &Dest.front();
        ::CryptStringToBinary(source.c_str(), (DWORD)source.GetLength(), CRYPT_STRING_BASE64, pDest, &size, NULL, NULL);

        // Convert the binary data to an IStream
        HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE, size);
        if(hMem != NULL)
        {
            LPVOID pImage = ::GlobalLock(hMem);
            if (pImage != NULL)
            {
                memcpy(pImage, pDest, size);
                IStream* pStream = NULL;
                if (S_OK == ::CreateStreamOnHGlobal(hMem, FALSE, &pStream))
                {

                    // Acquire GDI+ Image from the IStream
                    Image image(pStream);

                    // Create a smaller thumbnail Image.
                    Bitmap* img = (Bitmap*)image.GetThumbnailImage(280, 420, NULL, NULL);

                    IStream* stream = NULL;
                    HRESULT hr = ::CreateStreamOnHGlobal(0, TRUE, &stream);
                    if (!SUCCEEDED(hr))
                        return;

                    CLSID gifClsid;
                    GetEncoderClsid(L"image/gif", &gifClsid);
                    VERIFY(Gdiplus::Ok == img->Save(stream, &gifClsid));

                    // Get the size of the stream
                    ULARGE_INTEGER streamSize;
                    VERIFY(S_OK == IStream_Size(stream, &streamSize));
                    ULONG len = streamSize.LowPart;
                    dest.assign(len, 0);

                    // Fill buffer from stream
                    VERIFY(S_OK == IStream_Reset(stream));
                    VERIFY(S_OK == IStream_Read(stream, &dest[0], len));

                    // Cleanup
                    pStream->Release();
                    GlobalUnlock(hMem);
                }

                GlobalFree(hMem);
            }
        }
    }
}

// Fills the list view with all movies.
void CMainFrame::FillList()
{
    ClearList();

    // Lock this function for thread safety
    CThreadLock lock(m_cs);

    std::list<MovieInfo>::const_iterator iter;
    for (iter = m_moviesData.begin(); iter != m_moviesData.end(); ++iter)
    {
        GetViewList().AddItem(*iter);
    }

    GetViewList().SetLastColumnWidth();
}

// Fills the list view with movies belonging to the specified boxset.
void CMainFrame::FillListFromBoxSet(LPCTSTR boxset)
{
    ClearList();

    // Lock this function for thread safety
    CThreadLock lock(m_cs);

    std::list<MovieInfo>::const_iterator iter;
    for (iter = m_moviesData.begin(); iter != m_moviesData.end(); ++iter)
    {
        if ((*iter).boxset == boxset)
            GetViewList().AddItem(*iter);
    }

    GetViewList().SetLastColumnWidth();
}

// Fills the list view with movies belonging to all boxsets.
void CMainFrame::FillListFromAllBoxSets()
{
    ClearList();

    // Lock this function for thread safety
    CThreadLock lock(m_cs);

    std::list<MovieInfo>::const_iterator iter;
    for (iter = m_moviesData.begin(); iter != m_moviesData.end(); ++iter)
    {
        if ((*iter).boxset != L"")
            GetViewList().AddItem(*iter);
    }

    GetViewList().SetLastColumnWidth();
}

// Fills the list view with movies within the specified date range.
void CMainFrame::FillListFromDateRange(LPCTSTR dateRange)
{
    ClearList();

    // Lock this function for thread safety
    CThreadLock lock(m_cs);

    CString str = dateRange;
    CString Year1 = str.Left(4);
    CString Year2 = str.Right(4);

    std::list<MovieInfo>::const_iterator iter;
    for (iter = m_moviesData.begin(); iter != m_moviesData.end(); ++iter)
    {
        CString year = (*iter).releaseDate.Left(4);
        if (((year >= Year1) && (year <= Year2)) && (*iter).videoType == L"Movies")
            GetViewList().AddItem(*iter);
    }

    GetViewList().SetLastColumnWidth();
}

// Fills the list view with movies matching the specified mask.
void CMainFrame::FillListFromFlags(DWORD mask)
{
    ClearList();

    // Lock this function for thread safety
    CThreadLock lock(m_cs);

    std::list<MovieInfo>::const_iterator iter;
    for (iter = m_moviesData.begin(); iter != m_moviesData.end(); ++iter)
    {
        if ((*iter).flags & mask)
            GetViewList().AddItem(*iter);
    }

    GetViewList().SetLastColumnWidth();
}

// Fills the list view with movies from all genres.
void CMainFrame::FillListFromGenres(LPCTSTR genreList)
{
    ClearList();

    CString str = genreList;
    CString resToken;
    int curPos = 0;

    resToken = str.Tokenize(L",", curPos);
    while (resToken != L"")
    {
        resToken.TrimLeft();
        FillListFromGenre(resToken);
        resToken = str.Tokenize(L",", curPos);
    }

    GetViewList().SetLastColumnWidth();
}

// Fills the list view with movies matching the specified genre.
void CMainFrame::FillListFromGenre(LPCTSTR genre)
{
    // Lock this function for thread safety
    CThreadLock lock(m_cs);

    std::list<MovieInfo>::const_iterator iter;
    for (iter = m_moviesData.begin(); iter != m_moviesData.end(); ++iter)
    {
        if (((*iter).genre.Find(genre, 0) >= 0) && (*iter).videoType == L"Movies")
            GetViewList().AddItem(*iter);
    }

    GetViewList().SetLastColumnWidth();
}

// Fills the list view with movies matching the specified type.
void CMainFrame::FillListFromType(LPCTSTR videoType)
{
    ClearList();

    // Lock this function for thread safety
    CThreadLock lock(m_cs);

    std::list<MovieInfo>::const_iterator iter;
    for (iter = m_moviesData.begin(); iter != m_moviesData.end(); ++iter)
    {
        if ((*iter).videoType.Find(videoType, 0) >= 0)
            GetViewList().AddItem(*iter);
    }

    GetViewList().SetLastColumnWidth();
}

// Fills the tree view.
void CMainFrame::FillTreeItems()
{
    GetViewTree().DeleteAllItems();

    // Add some tree-view items
    HTREEITEM libraryItem = GetViewTree().AddItem(NULL, L"Video Library", 0);
    HTREEITEM moviesItem = GetViewTree().AddItem(libraryItem, L"Movies", 1);

    m_boxSetsItem = GetViewTree().AddItem(moviesItem, L"Box Sets", 2);
    std::list<CString>::iterator it;
    for (it = m_boxSets.begin(); it != m_boxSets.end(); ++it)
    {
        GetViewTree().AddItem(m_boxSetsItem, (*it), 2);
    }

    HTREEITEM genreItem = GetViewTree().AddItem(moviesItem, L"Genre", 5);
    for (it = m_genres.begin(); it != m_genres.end(); ++it)
    {
        GetViewTree().AddItem(genreItem, (*it), 5);
    }

    HTREEITEM decadesItem = GetViewTree().AddItem(moviesItem, L"Release Dates", 3);
    for (it = m_decades.begin(); it != m_decades.end(); ++it)
    {
        GetViewTree().AddItem(decadesItem, (*it), 3);
    }

    GetViewTree().AddItem(libraryItem, L"Live Performances", 6);
    GetViewTree().AddItem(libraryItem, L"Favourites", 4);

    // Expand some tree-view items
    GetViewTree().Expand(libraryItem, TVE_EXPAND);
    GetViewTree().Expand(moviesItem, TVE_EXPAND);

    GetViewTree().SelectItem(libraryItem);
}

std::vector<CString> CMainFrame::GetBoxSets()
{
    std::vector<CString> boxSets;
    HTREEITEM item = GetViewTree().GetChild(m_boxSetsItem);
    while (item != 0)
    {
        boxSets.push_back(GetViewTree().GetItemText(item));
        item = GetViewTree().GetNextSibling(item);
    };

    return boxSets;
}

// Returns the path used for the data files.
CString CMainFrame::GetDataPath() const
{
    CString filePath = GetAppDataPath();

    assert(!filePath.IsEmpty());
    filePath += L"\\";
    filePath += GetRegistryKeyName();

    return filePath;
}

// Converts a text string to a vector of words.
std::vector<CString> CMainFrame::GetWords(const CString& str) const
{
    int position = 0;
    CString token = str.Tokenize(L" ", position);
    std::vector<CString> words;
    while (!token.IsEmpty())
    {
        // Get next token.
        words.push_back(token);
        token = str.Tokenize(L" ", position);
    }

    return words;
}

// Performs a case-insensitive search for a word in sentence.
// Returns true if a matching word is found.
bool CMainFrame::IsWordInString(const CString& sentence, const CString& word) const
{
    int pos = 0;
    CString sentenceLow = sentence;
    sentenceLow.MakeLower();

    CString wordLow = word;
    wordLow.MakeLower();

    // find each substring in sentence that matches word
    while ((pos = sentenceLow.Find(wordLow, pos)) >= 0)
    {
        // words are bound by non-isalpha characters or begin/end of word.
        int nNextChar = pos + wordLow.GetLength();
        bool isWordStart = (pos == 0) || !(iswalpha(sentenceLow[pos - 1]));
        bool isWordEnd = (nNextChar == sentenceLow.GetLength()) || !(iswalpha(sentenceLow[nNextChar]));

        // return true if the substring found is a word
        if (isWordStart && isWordEnd)
            return true;

        pos++;
    }

    return false;
}

// Loads the meta data info from the movie file.
// Stores the information in a MovieInfo struct.
void CMainFrame::LoadMovieInfoFromFile(FoundFileInfo ffi)
{
    m_mi.Clear();
    m_mi.fileName = ffi.fileName;
    m_mi.lastModifiedTime = ffi.lastModifiedTime;

    MediaInfo MI;
    MI.Option(L"Cover_Data", L"base64");    // Required for v18.03.1 and higher

    MI.Open(ffi.fileName.c_str());
    m_mi.movieName   = CString(MI.Get(Stream_General, 0, L"Movie", Info_Text, Info_Name).c_str());
    m_mi.genre       = CString(MI.Get(Stream_General, 0, L"Genre", Info_Text, Info_Name).c_str());
    m_mi.actors      = CString(MI.Get(Stream_General, 0, L"Actor", Info_Text, Info_Name).c_str());
    m_mi.description = CString(MI.Get(Stream_General, 0, L"LongDescription",  Info_Text, Info_Name).c_str());
    m_mi.releaseDate = CString(MI.Get(Stream_General, 0, L"Recorded_Date",    Info_Text, Info_Name).c_str());
    m_mi.duration    = CString(MI.Get(Stream_General, 0, L"Duration/String3", Info_Text, Info_Name).c_str());

    CString CoverImage = CString(MI.Get(Stream_General, 0, L"Cover_Data", Info_Text, Info_Name).c_str());
    FillImageData(CoverImage, m_mi.imageData);
    m_mi.genre.Remove(L" / 57");
    m_mi.videoType = L"Movies";
    m_mi.flags = 0;

    MI.Close();
}

BOOL CMainFrame::LoadRegistrySettings(LPCTSTR szKeyName)
{

    assert(NULL != szKeyName);

    if (CDockFrame::LoadRegistrySettings(szKeyName))
    {
        // Always show the toolbar and status bar (fixes old values).
        InitValues iv = GetInitValues();
        iv.showStatusBar = TRUE;
        iv.showToolBar = TRUE;
        SetInitValues(iv);

        CString strKey = L"Software\\" + GetRegistryKeyName() + L"\\Frame Settings";
        CRegKey Key;
        if (ERROR_SUCCESS == Key.Open(HKEY_CURRENT_USER, strKey, KEY_READ))
        {
            try
            {
                DWORD dwDialogWidth = 0;
                DWORD dwTreeHeight = 0;

                if (ERROR_SUCCESS != Key.QueryDWORDValue(L"Dialog Width", dwDialogWidth))
                    throw CUserException(L"RegQueryValueEx Failed");
                if (ERROR_SUCCESS != Key.QueryDWORDValue(L"Tree Height", dwTreeHeight))
                    throw CUserException(L"RegQueryValueEx Failed");

                m_dialogWidth = dwDialogWidth;
                m_treeHeight = dwTreeHeight;
                return TRUE;
            }

            catch (const CUserException&)
            {
                m_dialogWidth = 0;
                m_treeHeight = 0;
            }
        }
    }

    return FALSE;
}

// Called when the Add Folder toolbar button is pressed.
BOOL CMainFrame::OnAddFolder()
{
    if (::WaitForSingleObject(m_thread, 0) != WAIT_TIMEOUT) // ensure the thread isn't already running
    {
        CFolderDialog fd;
        fd.SetTitle(L"Choose a folder to add to the video library.");
        if (fd.DoModal(*this) == IDOK)
        {
            CSplash splash(L"Updating Library");

            CString DataPath = GetDataPath();
            CString DataFile = GetDataPath() + L"\\" + L"MovieData.bin";
            SHCreateDirectoryEx(NULL, DataPath.c_str(), NULL);

            // Remove entries from the library if the file has been removed
            {
                // Lock this code for thread safety
                CThreadLock lock(m_cs);

                std::list<MovieInfo>::iterator it = m_moviesData.begin();
                while (it != m_moviesData.end())
                {
                    if (!PathFileExists((*it).fileName))
                    {
                        TRACE((*it).fileName); TRACE("  removed from library\n");
                        it = m_moviesData.erase(it);
                    }
                    else
                        ++it;
                }
            }

            CString searchString = fd.GetFolderPath() + L"\\*.m4v";
            CFileFind fileFound;

            m_foundFiles.clear();
            if (fileFound.FindFirstFile(searchString))
            {
                do
                    if (!fileFound.IsDots())
                    {
                        FoundFileInfo ffi;
                        ffi.fileName = fileFound.GetFilePath();
                        ffi.lastModifiedTime = fileFound.GetLastWriteTime();
                        m_foundFiles.push_back(ffi);
                    }
                while (fileFound.FindNextFile());
            }

            // Create the thread and run ThreadProc
            m_thread.CreateThread(0, 0, 0);
        }
    }

    return TRUE;
}

// Called when the user clicks on Box Sets (parent or child)
// in the tree view.
BOOL CMainFrame::OnBoxSet(UINT nID)
{
    int index = nID - IDM_BOXSET_1;
    std::vector<CString> boxsets = GetBoxSets();
    int iItem = -1;
    while ((iItem = GetViewList().GetNextItem(iItem, LVIS_SELECTED)) != -1)
    {
        m_isDirty = true;
        MovieInfo* pmi = (MovieInfo*)GetViewList().GetItemData(iItem);
        pmi->boxset = (index < 0) ? L"" : boxsets[index];
        GetViewList().UpdateItemImage(iItem);
    }

    HTREEITEM item = GetViewTree().GetSelection();
    CString itemText = GetViewTree().GetItemText(item);

    HTREEITEM parentItem = GetViewTree().GetParentItem(item);
    CString parentText = GetViewTree().GetItemText(parentItem);

    if (parentText == L"Box Sets")
        FillListFromBoxSet(itemText);

    if (itemText == L"Box Sets")
        FillListFromAllBoxSets();

    CString str;
    int listItemsCount = GetViewList().GetItemCount();
    if (listItemsCount == 1)
        str.Format(L":  %d video", listItemsCount);
    else
        str.Format(L":  %d videos", listItemsCount);

    str = itemText + str;
    SetStatusText(str);
    return TRUE;
}

// Called when the frame application is closed (before the window is destroyed).
// Stores the movie data in an archive if it has changed.
void CMainFrame::OnClose()
{
    // terminate the load files thread
    ::SetEvent(m_stopRequest);
    ::WaitForSingleObject(m_thread, INFINITE);

    ShowWindow(SW_HIDE);

    if (m_isDirty)
    {
        // Lock this code for thread safety
        CThreadLock lock(m_cs);

        // Display the splash screen.
        // The splash window is destroyed when splash goes out of scope.
        CSplash splash(L"Saving Library");

        CString DataPath = GetDataPath();
        CString DataFile = GetDataPath() + L"\\" + L"MovieData.bin";
        ::SHCreateDirectoryEx(NULL, DataPath.c_str(), NULL);

        try
        {
            CArchive ar(DataFile, CArchive::store);
            std::list<MovieInfo>::iterator it;

            std::vector<CString> boxSets = GetBoxSets();
            ar << UINT(boxSets.size());
            for (size_t i = 0; i < boxSets.size(); ++i)
            {
                ar << boxSets[i];
            }

            ar << UINT(m_moviesData.size());
            for (it = m_moviesData.begin(); it != m_moviesData.end(); ++it)
            {
                ar << (*it).fileName;
                ArchiveObject ao(&(*it).lastModifiedTime, sizeof(FILETIME));
                ar << ao;
                ar << (*it).movieName;
                ar << (*it).duration;
                ar << (*it).releaseDate;
                ar << (*it).description;
                ar << (*it).genre;
                ar << (*it).actors;
                ar << (*it).videoType;
                ar << (*it).boxset;
                ar << (*it).flags;

                UINT ImageDataSize = UINT((*it).imageData.size());
                ar << ImageDataSize;
                if (ImageDataSize > 0)
                    ar.Write(&(*it).imageData[0], ImageDataSize);
            }
            TRACE("\nSave Movies data complete \n ");
        }

        catch (const CFileException& e)
        {
            Trace(e.GetErrorString()); Trace("\n");
            ::MessageBox(NULL, L"Failed to store Movie Library", L"Error", MB_OK);
        }
    }

    CDockFrame::OnClose();
}

// Respond to toolbar or context menu input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    // OnCommand responds to menu and and toolbar input

    UNREFERENCED_PARAMETER(lparam);

    UINT nID = LOWORD(wparam);
    switch (nID)
    {
    case IDM_EDIT_COPY:     GetFocus().SendMessage(WM_COPY);    return TRUE;
    case IDM_EDIT_PASTE:    GetFocus().SendMessage(WM_PASTE);   return TRUE;

    case IDM_ADD_FOLDER:        return OnAddFolder();
    case IDM_PLAY:              return OnPlay();
    case IDM_FAVOURITE:         return OnFavourite();

    case IDM_VIDEOTYPE_LIVEPERFORMANCE: return OnVideoType(L"Live Performances");
    case IDM_VIDEOTYPE_MOVIE:           return OnVideoType(L"Movies");

    case IDM_ADD_BOXSET:        return OnAddBoxSet();
    case IDM_MOVE_UP:           return OnMoveUp();
    case IDM_MOVE_DOWN:         return OnMoveDown();
    case IDM_RENAME_BOXSET:     return OnRenameBoxSet();
    case IDM_REMOVE_BOXSET:     return OnRemoveBoxSet();

    case IDM_REMOVE_FILE:       return OnRemoveFile();
    case IDM_SEARCH:            return OnSearch();
    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDM_HELP_ABOUT:        return OnHelp();
    }

    if (nID >= IDM_BOXSET_0)
        return OnBoxSet(nID);

    return FALSE;
}

// Called in response to the add box set from the context menu on the list view.
BOOL CMainFrame::OnAddBoxSet()
{
    GetViewTree().SetFocus();
    HTREEITEM newItem = GetViewTree().AddItem(m_boxSetsItem, L"New Box Set", 2);

    GetViewTree().Expand(m_boxSetsItem, TVE_EXPAND);
    GetViewTree().EditLabel(newItem);
    m_isDirty = TRUE;

    return TRUE;
}

// Called in response to the popup menu when the user right clicks
// on a box set in the tree view. 
BOOL CMainFrame::OnMoveUp()
{
    HTREEITEM item = GetViewTree().GetSelection();
    HTREEITEM prevItem = GetViewTree().GetPrevSibling(item);

    if (item != 0 && prevItem != 0)
    {
        GetViewTree().Swap(item, prevItem);
        GetViewTree().SelectItem(prevItem);
    }
    return TRUE;
}

// Called in response to the popup menu when the user right clicks
// on a box set in the tree view. 
BOOL CMainFrame::OnMoveDown()
{
    HTREEITEM item = GetViewTree().GetSelection();
    HTREEITEM nextItem = GetViewTree().GetNextSibling(item);

    if (item != 0 && nextItem != 0)
    {
        GetViewTree().Swap(item, nextItem);
        GetViewTree().SelectItem(nextItem);
    }

    return TRUE;
}

// Called in response to the popup menu when the user right clicks
// on a box set in the tree view. 
BOOL CMainFrame::OnRenameBoxSet()
{
    HTREEITEM item = GetViewTree().GetSelection();
    GetViewTree().EditLabel(item);

    return TRUE;
}

// Called in response to the popup menu when the user right clicks
// on a box set in the tree view. 
BOOL CMainFrame::OnRemoveBoxSet()
{
    HTREEITEM item = GetViewTree().GetSelection();
    CString* pString = GetViewTree().GetItemString(item);

    // Lock this function for thread safety
    CThreadLock lock(m_cs);

    std::list<MovieInfo>::iterator it;
    for (it = m_moviesData.begin(); it != m_moviesData.end(); ++it)
    {
        if ((*it).boxset == *pString)
            (*it).boxset = L"";
    }

    GetViewTree().RemoveItem(item);
    m_isDirty = TRUE;

    return TRUE;
}

// Called during window creation. Override this functions to perform tasks
// such as creating child windows.
int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
    // OnCreate controls the way the frame is created.
    // Overriding CFrame::OnCreate is optional.

    // A menu (or MenuBar) is added if the IDW_MAIN menu resource is defined.
    // Frames have all options enabled by default.
    // Use the following functions to disable options.

    // This app has a toolbar, but no menu (or menubar)

       UseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar
       UseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar
    // UseReBar(FALSE);              // Don't use a ReBar
    // UseStatusBar(FALSE);          // Don't use a StatusBar
    // UseThemes(FALSE);             // Don't use themes
    // UseToolBar(FALSE);            // Don't use a ToolBar

    // call the base class function
    return CDockFrame::OnCreate(cs);
}

// Clears the contents of the movie info dialog.
void CMainFrame::ClearList()
{
    GetViewList().DeleteAllItems();

    GetViewDialog().GetTitle().SetWindowText(0);
    GetViewDialog().GetYear().SetWindowText(0);
    GetViewDialog().GetActors().SetWindowText(0);
    GetViewDialog().GetInfo().SetWindowText(0);
    GetViewDialog().SetPicture().SetImageData().clear();
    GetViewDialog().SetPicture().Invalidate();
}

// Loads the movie data information from the archive
void CMainFrame::LoadMovies()
{
    //Information about MediaInfo
    MediaInfo MI;

    CString DataPath = GetDataPath();
    CString DataFile = GetDataPath() + L"\\" + L"MovieData.bin";
    SHCreateDirectoryEx(NULL, DataPath.c_str(), NULL);

    if (PathFileExists(DataFile))
    {
        try
        {
            CArchive ar(DataFile, CArchive::load);
            std::vector<MovieInfo>::iterator it;

            // Lock this code for thread safety
            CThreadLock lock(m_cs);

            m_moviesData.clear();
            TRACE("Loading Movies Data\n");

            UINT nBoxSets;
            ar >> nBoxSets;
            for (UINT i = 0; i < nBoxSets; ++i)
            {
                CString str;
                ar >> str;
                m_boxSets.push_back(str);
            }

            UINT size;
            ar >> size;
            for (UINT i = 0; i < size; ++i)
            {
                MovieInfo mi;

                ar >> mi.fileName;
                ArchiveObject ao(&mi.lastModifiedTime, sizeof(FILETIME));
                ar >> ao;
                ar >> mi.movieName;
                ar >> mi.duration;
                ar >> mi.releaseDate;
                ar >> mi.description;
                ar >> mi.genre;
                ar >> mi.actors;
                ar >> mi.videoType;
                ar >> mi.boxset;
                ar >> mi.flags;

                UINT ImageDataSize = 0;
                ar >> ImageDataSize;
                if (ImageDataSize > 0)
                {
                    mi.imageData.resize(ImageDataSize);
                    ar.Read(&mi.imageData[0], ImageDataSize);
                }

                m_moviesData.push_back(mi);
            }
        }
        catch (const CFileException& e)
        {
            Trace(e.GetErrorString()); Trace("\n");
            ::MessageBox(NULL, L"Failed to load Movie Library", L"Error", MB_OK);
            m_moviesData.clear();
            m_boxSets.clear();
        }
    }

    TRACE("Load Movies data complete \n ");
}

// Called in response to favourites on the toolbar or the
// list view popup menu. Sets the movie's favourite flag.
BOOL CMainFrame::OnFavourite()
{
    int iItem = -1;
    while ((iItem = GetViewList().GetNextItem(iItem, LVIS_SELECTED)) != -1)
    {
        m_isDirty = true;
        MovieInfo* pmi = (MovieInfo*)GetViewList().GetItemData(iItem);
        BOOL IsFavourite = pmi->flags & 0x0001;
        IsFavourite = !IsFavourite;
        pmi->flags = ((pmi->flags & 0xFFFE) | IsFavourite) ? 0x0001 : 0x000;
        GetToolBar().CheckButton(IDM_FAVOURITE, IsFavourite);

        // Update the list view
        HTREEITEM item = GetViewTree().GetSelection();
        CString itemText = GetViewTree().GetItemText(item);

        if (itemText == L"Favourites")
            FillListFromFlags(0x0001);

        GetViewList().UpdateItemImage(iItem);
    }

    return TRUE;
}

// Called after the frame is created, but before it is displayed.
void CMainFrame::OnInitialUpdate()
{
    // Add the dialog.
    int width = m_dialogWidth? m_dialogWidth : (GetViewRect().Width() / 3);
    DWORD dockStyle = DS_NO_UNDOCK | DS_NO_CAPTION;
    m_pDockDialog = static_cast<CDockDialog*>(AddDockedChild(new CDockDialog, dockStyle | DS_DOCKED_LEFT, width));

    // Add the tree view.
    int height = m_treeHeight? m_treeHeight : GetViewRect().Height() / 3;
    m_pDockTree = static_cast<CDockTree*>(m_pDockDialog->AddDockedChild(new CDockTree, dockStyle | DS_DOCKED_TOP, height));

    // Display the splash screen.
    // The splash window is destroyed when splash goes out of scope.
    CSplash splash(L"Loading Library");

    // Fill the tree view and list view.
    LoadMovies();
    FillTreeItems();

    ShowWindow(SW_SHOW);
}

// Called in response to toolbar input or a context menu on
// the list view. Plays the chosen movie.  
BOOL CMainFrame::OnPlay()
{
    int item = GetViewList().GetNextItem(-1, LVNI_SELECTED);
    MovieInfo* pmi = (MovieInfo*)GetViewList().GetItemData(item);
    if (pmi)
        PlayMovie(pmi->fileName);

    return TRUE;
}

// Called when a menu is about to be displayed.
void CMainFrame::OnMenuUpdate(UINT nID)
{
    switch (nID)
    {

    case IDM_MOVE_DOWN:
    {
        HTREEITEM item = GetViewTree().GetSelection();
        bool isAtBottom = (GetViewTree().GetNextSibling(item) == 0);
        m_boxSetMenu.EnableMenuItem(nID, isAtBottom? MF_GRAYED : MF_ENABLED);
        break;
    }

    case IDM_MOVE_UP:
    {
        HTREEITEM item = GetViewTree().GetSelection();
        bool isAtTop = (GetViewTree().GetPrevSibling(item) == 0);
        m_boxSetMenu.EnableMenuItem(nID, isAtTop ? MF_GRAYED : MF_ENABLED);
        break;
    }

    }
}

// Called in response to a right mouse click on the list view. 
void CMainFrame::OnRClickListItem()
{
    // Load the popup menu
    CMenu topMenu(IDM_VIDEO_MENU);
    m_popupMenu = topMenu.GetSubMenu(0);

    CMenu boxsetMenu = m_popupMenu.GetSubMenu(3);
    std::vector<CString> boxSets = GetBoxSets();

    for (size_t i = 0; i < boxSets.size(); i++)
    {
        boxsetMenu.AppendMenu(MF_STRING, IDM_BOXSET_1 + i, boxSets[i]);
    }

    CPoint screenPoint = GetCursorPos();

    // Start the popup menu
    m_popupMenu.TrackPopupMenuEx(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, screenPoint.x, screenPoint.y, *this, NULL/*&tpm*/);
}

// Called in response to a right mouse click on the tree view.
void CMainFrame::OnRClickTreeItem()
{
    CPoint screenPoint = GetCursorPos();
    CPoint clientPoint = screenPoint;
    GetViewTree().ScreenToClient(clientPoint);
    TVHITTESTINFO tv = { 0 };
    tv.pt = clientPoint;
    HTREEITEM item = GetViewTree().HitTest(tv);
    if (item != 0)
        GetViewTree().SelectItem(item);

    if (GetViewTree().GetItemText(item) == L"Box Sets")
    {
        GetViewTree().Expand(item, TVE_EXPAND);

        // The menu for a boxset parent
        CMenu topMenu(IDM_BOXSET_MENU);
        m_boxSetMenu = topMenu.GetSubMenu(0);

        // Start the popup menu
        m_boxSetMenu.TrackPopupMenuEx(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, screenPoint.x, screenPoint.y, *this, NULL/*&tpm*/);
    }

    HTREEITEM parent = GetViewTree().GetParentItem(item);
    if (GetViewTree().GetItemText(parent) == L"Box Sets")
    {
        // The menu for an boxset child
        CMenu topMenu(IDM_BOXSETCHILD_MENU);
        m_boxSetMenu = topMenu.GetSubMenu(0);

        // Start the popup menu
        m_boxSetMenu.TrackPopupMenuEx(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, screenPoint.x, screenPoint.y, *this, NULL/*&tpm*/);
    }
}

// Called in response to a click on the toolbar search button.
// Opens a dialog to perform a word search on the select items.
BOOL CMainFrame::OnSearch()
{
    CSearchDialog dlg(IDD_SEARCHDLG);

    if (dlg.DoModal(*this) == IDOK)
    {
        ClearList();

        // Lock this code for thread safety
        CThreadLock lock(m_cs);

        std::vector<const MovieInfo*> foundMovies;
        std::list<MovieInfo>::const_iterator iter;

        // Find movies matching each title word.
        if (!dlg.GetTitleString().IsEmpty())
        {
            std::vector<CString> words = GetWords(dlg.GetTitleString());

            for (UINT i = 0; i < words.size(); ++i)
            {
                for (iter = m_moviesData.begin(); iter != m_moviesData.end(); ++iter)
                {
                    if (IsWordInString((*iter).movieName, words[i]))
                        foundMovies.push_back(&(*iter));
                }
            }
        }

        // Find movies matching each actor word.
        if (!dlg.GetActorsString().IsEmpty())
        {
            std::vector<CString> words = GetWords(dlg.GetActorsString());
            for (UINT i = 0; i < words.size(); ++i)
            {
                for (iter = m_moviesData.begin(); iter != m_moviesData.end(); ++iter)
                {
                    if (IsWordInString((*iter).actors, words[i]))
                        foundMovies.push_back(&(*iter));
                }
            }
        }

        // Find movies matching each description word.
        if (!dlg.GetInfoString().IsEmpty())
        {
            std::vector<CString> words = GetWords(dlg.GetInfoString());
            for (UINT i = 0; i < words.size(); ++i)
            {
                for (iter = m_moviesData.begin(); iter != m_moviesData.end(); ++iter)
                {
                    if (IsWordInString((*iter).description, words[i]))
                        foundMovies.push_back(&(*iter));
                }
            }
        }

        // Fill the listview with the found movies.
        for (UINT i = 0; i < foundMovies.size(); ++i)
        {
            MoviesData::iterator it;
            for (it = m_moviesData.begin(); it != m_moviesData.end(); ++it)
            {
                if (&(*it) == foundMovies[i])
                {
                    GetViewList().AddItem(*it);
                    break;
                }
            }
        }

        // Select the first ListView item.
        GetViewList().SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);

        CString str;
        int listItemsCount = GetViewList().GetItemCount();
        if (listItemsCount == 1)
            str.Format(L":  %d video found", listItemsCount);
        else
            str.Format(L":  %d videos found", listItemsCount);

        str = L"Search" + str;
        SetStatusText(str);
    }

    return TRUE;
}

// Called in response to a popup menu on the list view.
// Removes the chosen file from the movie libaray.
BOOL CMainFrame::OnRemoveFile()
{
    // Lock this function for thread safety
    CThreadLock lock(m_cs);

    std::vector<CString> filenames;
    int itemIndex = -1;
    while ((itemIndex = GetViewList().GetNextItem(itemIndex, LVIS_SELECTED)) != -1)
    {
        MovieInfo* pmi = (MovieInfo*)GetViewList().GetItemData(itemIndex);
        filenames.push_back(pmi->fileName);
    }

    for (UINT i = 0; i < filenames.size(); ++i)
    {
        MoviesData::iterator it;
        for (it = m_moviesData.begin(); it != m_moviesData.end(); ++it)
        {
            if ((*it).fileName == filenames[i])
            {
                TRACE(m_mi.fileName); TRACE(" removed from library\n");
                m_moviesData.erase(it);
                m_isDirty = true;
                break;
            }
        }
    }

    OnSelectTreeItem();

    return TRUE;
}

// Updates the dialog with information about the movie.
void CMainFrame::OnSelectListItem(const MovieInfo* pmi)
{
    CViewDialog& dialog = (CViewDialog&)m_pDockDialog->GetView();
    assert(pmi);

    dialog.GetTitle().SetWindowText(pmi->movieName);
    dialog.GetYear().SetWindowText(pmi->releaseDate);

    // Replace " / " with cariage return, line feed to put each actor on a new line
    CString actors = pmi->actors;
    actors.Replace(L" / ", L"\r\n");
    dialog.GetActors().SetWindowText(actors);
    dialog.GetInfo().SetWindowText(pmi->description);
    dialog.SetPicture().SetImageData() = pmi->imageData;
    dialog.SetPicture().Invalidate();

    BOOL isFavourite = pmi->flags & 0x0001;

    if (GetViewList().GetNextItem(-1, LVNI_SELECTED) < 0)
        isFavourite = FALSE;

    GetToolBar().CheckButton(IDM_FAVOURITE, isFavourite);
}

// Called when a selection is made on the tree view.
void CMainFrame::OnSelectTreeItem()
{
    HTREEITEM item = GetViewTree().GetSelection();
    CString itemText = GetViewTree().GetItemText(item);

    HTREEITEM parentItem = GetViewTree().GetParentItem(item);
    CString parentText = GetViewTree().GetItemText(parentItem);

    if (itemText == L"Video Library")
        FillList();
    else if (itemText == L"Movies")
        FillListFromType(itemText);
    else if (itemText == L"Box Sets")
        FillListFromAllBoxSets();
    else if (itemText == L"Release Dates")
        FillListFromType(L"Movies");
    else if (itemText == L"Genre")
        FillListFromType(L"Movies");
    else if (itemText == L"Live Performances")
        FillListFromType(itemText);
    else if (parentText == L"Box Sets")
        FillListFromBoxSet(itemText);
    else if (parentText == L"Release Dates")
        FillListFromDateRange(itemText);
    else if (parentText == L"Genre")
        FillListFromGenres(itemText);
    else if (itemText == L"Favourites")
        FillListFromFlags(0x0001);
    else
        ClearList();

    GetViewList().SortColumn(0, true);

    CString str;
    int listItemsCount = GetViewList().GetItemCount();
    if (listItemsCount == 1)
        str.Format(L":  %d video", listItemsCount);
    else
        str.Format(L":  %d videos", listItemsCount);

    str = itemText + str;
    SetStatusText(str);
}

// Called in response to the popup menu on the list view.
// Sets the video type to a "Movie" or "Live Performance"
BOOL CMainFrame::OnVideoType(LPCTSTR videoType)
{
    int iItem = -1;
    while ((iItem = GetViewList().GetNextItem(iItem, LVIS_SELECTED)) != -1)
    {
        m_isDirty = true;
        MovieInfo* pmi = (MovieInfo*)GetViewList().GetItemData(iItem);
        pmi->videoType = videoType;
        GetViewList().UpdateItemImage(iItem);
    }

    return TRUE;
}

// Plays the selected movie.
void CMainFrame::PlayMovie(LPCTSTR path)
{
    if (PathFileExists(path))
        ::ShellExecute(*this, L"open", path, NULL, NULL, SW_SHOW);
    else
    {
        CString str = CString(L"Unable to play\n") + path;
        MessageBox(str, L"Unable to play movie", MB_OK);
    }
}

// Sets the CREATESTRUCT struct prior to window creation.
void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
    // This function is called before the frame is created.
    // It provides an opportunity to modify the various CREATESTRUCT
    // parameters used in the frame window's creation.

    // The WS_EX_LAYOUTRTL style requires Windows 2000 or above in targetver.h
    // cs.dwExStyle = WS_EX_LAYOUTRTL;      // Set Right-To-Left Window Layout

    // Call base clase to set defaults
    CDockFrame::PreCreate(cs);

    cs.style &= ~WS_VISIBLE;    // Remove the WS_VISIBLE style. The frame will be initially hidden.
}


// Saves some settings in the registry.
BOOL CMainFrame::SaveRegistrySettings()
{
    if (!GetRegistryKeyName().IsEmpty())
    {
        try
        {
            CString KeyName = L"Software\\" + GetRegistryKeyName() + L"\\Frame Settings";
            CRegKey Key;

            if (ERROR_SUCCESS != Key.Create(HKEY_CURRENT_USER, KeyName))
                throw CUserException(L"RegCreateKeyEx failed");
            if (ERROR_SUCCESS != Key.Open(HKEY_CURRENT_USER, KeyName))
                throw CUserException(L"RegCreateKeyEx failed");

            DWORD cxDialog = DWORD(m_pDockDialog->GetDockSize());
            DWORD cyTree = DWORD(m_pDockTree->GetDockSize());

            if (ERROR_SUCCESS != Key.SetDWORDValue(L"Dialog Width", cxDialog))
                throw CUserException(L"RegSetValueEx failed");
            if (ERROR_SUCCESS != Key.SetDWORDValue(L"Tree Height", cyTree))
                throw CUserException(L"RegSetValueEx failed");

            return CDockFrame::SaveRegistrySettings();
        }

        catch (const CUserException& e)
        {
            Trace("*** Failed to save registry settings. ***\n");
            Trace(e.GetText()); Trace("\n");

            CString KeyName = L"Software\\" + GetRegistryKeyName();
            CRegKey Key;

            if (ERROR_SUCCESS == Key.Open(HKEY_CURRENT_USER, KeyName))
            {
                // Roll back the registry changes by deleting this subkey
                Key.DeleteSubKey(L"Frame Settings");
            }

            return FALSE;
        }
    }

    return FALSE;
}

// Adds icons for popup menus.
void CMainFrame::SetupMenuIcons()
{
    AddMenuIcon(IDM_ADD_FOLDER, GetApp()->LoadIcon(IDI_ADDFOLDER));
    AddMenuIcon(IDM_PLAY, GetApp()->LoadIcon(IDI_PLAY));
    AddMenuIcon(IDM_FAVOURITE, GetApp()->LoadIcon(IDI_FAVOURITES));
    AddMenuIcon(IDM_REMOVE_FILE, GetApp()->LoadIcon(IDI_REMOVE));
    AddMenuIcon(IDM_HELP_ABOUT, GetApp()->LoadIcon(IDI_HELPABOUT));

    AddMenuIcon(IDM_VIDEOTYPE_MOVIE, GetApp()->LoadIcon(IDI_MOVIES));
    AddMenuIcon(IDM_VIDEOTYPE_LIVEPERFORMANCE, GetApp()->LoadIcon(IDI_VIOLIN));

    AddMenuIcon(IDM_ADD_BOXSET, GetApp()->LoadIcon(IDI_ADDBOXSET));
    AddMenuIcon(IDM_MOVE_UP, GetApp()->LoadIcon(IDI_MOVEUP));
    AddMenuIcon(IDM_MOVE_DOWN, GetApp()->LoadIcon(IDI_MOVEDOWN));
    AddMenuIcon(IDM_REMOVE_BOXSET, GetApp()->LoadIcon(IDI_REMOVE));
    AddMenuIcon(IDM_RENAME_BOXSET, GetApp()->LoadIcon(IDI_EDITBOXSET));

    // Accomodate up to 64 boxsets
    for (int i = IDM_BOXSET_1; i < IDM_BOXSET_1 + 64; i++)
        AddMenuIcon(i, GetApp()->LoadIcon(IDI_BOXSET));
}

// Configure the toolbar.
void CMainFrame::SetupToolBar()
{
    // Create the normal ImageList for the toolbar
    m_toolbarImages.Create(32, 32, ILC_COLOR32, 0, 0);
    m_toolbarImages.Add(GetApp()->LoadIcon(IDI_ADDFOLDER));
    m_toolbarImages.Add(GetApp()->LoadIcon(IDI_PLAY));
    m_toolbarImages.Add(GetApp()->LoadIcon(IDI_FAVOURITES));
    m_toolbarImages.Add(GetApp()->LoadIcon(IDI_SEARCH));
    m_toolbarImages.Add(GetApp()->LoadIcon(IDI_HELPABOUT));
    GetToolBar().SetImageList(m_toolbarImages);

    // Add toolbar buttons and set their Resource IDs
    AddToolBarButton(IDM_ADD_FOLDER);
    AddToolBarButton(0);                        // Separator
    AddToolBarButton(IDM_PLAY);
    AddToolBarButton(IDM_FAVOURITE);
    AddToolBarButton(IDM_SEARCH);
    AddToolBarButton(0);                        // Separator
    AddToolBarButton(IDM_HELP_ABOUT);
}

// Called after the video library is updated.
void CMainFrame::OnFilesLoaded()
{
    m_foundFiles.clear();
    OnSelectTreeItem();
}

// This function runs in a separate thread because
// loading the meta data from files can be time consuming.
// This thread modifies m_moviesData, so all access to
// m_moviesData should be protected by a CThreadLock.
UINT WINAPI CMainFrame::ThreadProc(void* pVoid)
{
    CMainFrame* pFrame = reinterpret_cast<CMainFrame*>(pVoid);

    MediaInfo MI;
    if (MI.IsReady())   // Is MediaInfo.dll loaded?
    {
        // Lock this code for thread safety
        CThreadLock lock(pFrame->m_cs);

        CSplash splash(L"Updating Library");
        splash.CreateBar();
        splash.GetBar().SetRange(0, (short)pFrame->m_foundFiles.size());

        unsigned short barPos = 0;
        for (size_t i = 0; i < pFrame->m_foundFiles.size(); i++)
        {
            // The stop request is set if the app is trying to close.
            if (::WaitForSingleObject(pFrame->m_stopRequest, 0) != WAIT_TIMEOUT)
            {
                // Abort the thread
                return 0;
            }

            barPos++;
            
            // Update the splash screen's progress bar
            splash.GetBar().SetPos(barPos);

            CString fullName = pFrame->m_foundFiles[i].fileName;
            bool isFileInLibrary = false;
            std::list<MovieInfo>::iterator it = pFrame->m_moviesData.begin();
            while (it != pFrame->m_moviesData.end())
            {
                if ((*it).fileName == fullName)
                {
                    CFileFind ff;
                    ff.FindFirstFile(fullName);
                    CTime t1(ff.GetLastWriteTime());
                    CTime t2((*it).lastModifiedTime);
                    if (t1 != t2)
                    {
                        // remove the modified file from the library
                        TRACE(pFrame->m_mi.fileName); TRACE(" removed modified file from library\n");
                        pFrame->m_moviesData.erase(it);
                    }
                    else
                        isFileInLibrary = true;

                    // No need to check further
                    break;
                }

                ++it;
            }

            // Only add files not already in the library
            if (!isFileInLibrary)
            {
                pFrame->m_isDirty = true;
                pFrame->LoadMovieInfoFromFile(pFrame->m_foundFiles[i]);
                pFrame->m_moviesData.push_back(pFrame->m_mi);

                TRACE(pFrame->m_mi.fileName); TRACE(" added to library\n");
            }
        }
    }
    else
    {
        // Report the error in a message  box.
        ::MessageBox(NULL, MI.Inform().c_str(), L"Error", MB_OK);
    }

    // The UWM_FILESLOADED updates the list view with the new data.
    // Post the message becase the frame is in a different thread.
    pFrame->PostMessage(UWM_FILESLOADED);

    return 0;
}

// Process the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_EXITSIZEMOVE:
        GetViewList().SetLastColumnWidth();
        break;

    case UWM_FILESLOADED:
        OnFilesLoaded();
        break;
    }

    // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}

