//////////////////////////////////////
// ShellWrapper.cpp

#include "stdafx.h"
#include "ShellWrapper.h"


namespace ShellWrapper
{
    ///////////////////////////////
    // Global functionn definitions (within this namespace)
    
    // Retrieves the file's path name from its pidl.
    BOOL GetFullFileName(LPCITEMIDLIST pidlFull, LPTSTR pszPathName)
    {
        if (!::SHGetPathFromIDList(pidlFull, pszPathName))
        {
            pszPathName[0] = _T('\0');
            return FALSE;
        }

        return TRUE;
    }

    // Retrieves the display name for the file from its pidl.
    // This is the name as it appears in Windows Explorer.
    BOOL GetDisplayName(LPCITEMIDLIST pidlFull, LPTSTR pszDisplayName)
    {
        SHFILEINFO     sfi;
        ZeroMemory(&sfi, sizeof(sfi));

        // Get the display name of the item
        if(!::SHGetFileInfo((LPCTSTR)pidlFull, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME))
        {
            pszDisplayName[0] = _T('\0');
            return FALSE;
        }

        StrCopy(pszDisplayName, sfi.szDisplayName, MAX_PATH);
        return TRUE;
    }


    ///////////////////////////////////
    //CContextMenu function definitions
    //

    // Constructor.
    CContextMenu::CContextMenu() : m_pIContextMenu(NULL)
    {
    }

    // Destructor.
    CContextMenu::~CContextMenu()
    {
        if (m_pIContextMenu)
            m_pIContextMenu->Release();
    }

    // Attaches the IContextMenu pointer to this object.
    void CContextMenu::Attach(IContextMenu* pIContextMenu)
    {
        if (m_pIContextMenu)
            m_pIContextMenu->Release();
        m_pIContextMenu = pIContextMenu;
    }

    // Carries out the command associated with the menu item.
    HRESULT CContextMenu::Invoke(CMINVOKECOMMANDINFO& Ici)
    {
        return m_pIContextMenu->InvokeCommand(&Ici);
    }

    // Attaches the IContextMenu2 pointer to ccm2.
    HRESULT CContextMenu::GetContextMenu2(CContextMenu2& ccm2)
    {
        HRESULT result = 0;
        REFIID iid = IID_IContextMenu2;

        IContextMenu2* pIContextMenu2 = NULL;
        result = m_pIContextMenu->QueryInterface(iid, (VOID**)&pIContextMenu2);
        if (S_OK == result)
            ccm2.Attach(pIContextMenu2);

        return result;
    }

    // Adds the context menu commands to the specified menu.
    HRESULT CContextMenu::QueryContextMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
    {
        return m_pIContextMenu->QueryContextMenu(hmenu, indexMenu, idCmdFirst, idCmdLast, uFlags);
    }


    ////////////////////////////////////
    //CContextMenu2 function definitions
    //

    // Constructor.
    CContextMenu2::CContextMenu2() : m_pIContextMenu2(NULL)
    {
    }

    // Destructor.
    CContextMenu2::~CContextMenu2()
    {
        if (m_pIContextMenu2)
            m_pIContextMenu2->Release();
    }

    // Attaches the IContextMenu2 pointer to this object. 
    void CContextMenu2::Attach(IContextMenu2* pIContextMenu2)
    {
        if (m_pIContextMenu2)
            m_pIContextMenu2->Release();
        m_pIContextMenu2 = pIContextMenu2;
    }

    // Handle the messages associated with menu items.
    HRESULT CContextMenu2::HandleMenuMsg(UINT msg, WPARAM wparam, LPARAM lparam)
    {
        HRESULT result = m_pIContextMenu2->HandleMenuMsg(msg, wparam, lparam);

        if((result != S_OK) && (result !=E_NOTIMPL))
        {
            TRACE("CContextMenu2::HandleMenuMsg failed\n");
        }

        return result;
    }

    // Releases the memory associated with the IContextMenu2.
    void CContextMenu2::Release()
    {
        m_pIContextMenu2->Release();
        m_pIContextMenu2 = 0;
    }


    ///////////////////////////////////
    //CShellFolder function definitions
    //

    // Constructor.
    CShellFolder::CShellFolder() : m_pIShellFolder(NULL)
    {
    }

    // Copy constructor.
    CShellFolder::CShellFolder(const CShellFolder& source) : m_pIShellFolder(NULL)
    {
        Copy(source);
    }

    // Assignment operator.
    void CShellFolder::operator=(const CShellFolder& source)
    {
        Copy(source);
    }

    // Destructor.
    CShellFolder::~CShellFolder()
    {
        Release();
    }

    // Converts SHELLFOLDER pointer to a CShellFolder object.
    // The memory allocated for the SHELLFOLDER pointer is released
    // in the destructor.    
    void CShellFolder::Attach(LPSHELLFOLDER pIShellFolder)
    {
        Release();
        m_pIShellFolder = pIShellFolder;
    }

    // Compare function, used for sorting.
    HRESULT CShellFolder::CompareIDs(LPARAM lparam, const Cpidl& cpidl1, const Cpidl& cpidl2)
    {
        LPITEMIDLIST pidl1 = cpidl1.GetPidl();
        LPITEMIDLIST pidl2 = cpidl2.GetPidl();
        return m_pIShellFolder->CompareIDs(lparam, pidl1, pidl2);
    }

    //Assigns the value to the CShellFolder object.
    void CShellFolder::Copy(LPSHELLFOLDER source)
    {
        Release(); //Release the current m_IShellFolder
        m_pIShellFolder = source;
        if (source)
            AddRef();
    }

    //Assigns the value to the CShellFolder object.
    void CShellFolder::Copy(const CShellFolder& rhs)
    {
        if (&rhs != this)
        {
            Release();
            m_pIShellFolder = rhs.m_pIShellFolder;
            if (rhs.m_pIShellFolder)
                AddRef();
        }
    }

    // Assigns the Desktop folder to CShellFolder.
    HRESULT CShellFolder::DesktopFolder()
    {
        HRESULT result = ::SHGetDesktopFolder(&m_pIShellFolder);

        if (result != NOERROR)
        {
            TRACE("CShellFolder::SHGetDesktopFolder failed\n");
        }
        return result;
    }

    // Retrieves the attributes of the item identified by its pidl.
    HRESULT CShellFolder::GetAttributes(UINT cidl, const Cpidl& cpidl, ULONG& rgfInOut)
    {
        LPCITEMIDLIST pidl = cpidl.GetPidl();
        HRESULT result = m_pIShellFolder->GetAttributesOf(cidl, &pidl, &rgfInOut);

        if (result != S_OK)
        {
            TRACE("CShellFolder::GetAttributesOf failed\n");
        }
        return result;
    }

    // Retrieves the context menu, when no item is selected.
    HRESULT CShellFolder::GetContextMenu(HWND owner, CContextMenu& ccm)
    {
        REFIID riid = IID_IContextMenu;
        IContextMenu* pcm;
        HRESULT result = m_pIShellFolder->CreateViewObject(owner, riid, (LPVOID*)&pcm);
        if (S_OK == result)
            ccm.Attach(pcm);
        else
        {
            TRACE("CShellFolder::CreateViewObject failed\n");
        }
        return result;
    }

    // Retrieves the context menu, for one or more selected items.
    // pidlArray is either an array of LPCITEMIDLIST or a pointer to a single LPCITEMIDLIST.
    HRESULT CShellFolder::GetContextMenu(HWND owner, UINT items, LPCITEMIDLIST* pidlArray, CContextMenu& cm)
    {
        IContextMenu* ppv;
        REFIID riid = IID_IContextMenu;
        HRESULT result = m_pIShellFolder->GetUIObjectOf(owner, items, pidlArray, riid, 0, (VOID**)&ppv);

        if (S_OK == result)
            cm.Attach(ppv);
        else
        {
            TRACE("CShellFolder::GetUIObjectOf failed\n");
        }

        return result;
    }

    // Retrieves the CEnumIDList. It can be used to enumerate the folder.
    HRESULT CShellFolder::GetEnumIDList(HWND owner, int grfFlags, CEnumIDList& cenumIDList)
    {
        LPENUMIDLIST pEnum;
        HRESULT result = 0;
        if (m_pIShellFolder)
        {
            result = m_pIShellFolder->EnumObjects(owner, grfFlags, &pEnum);
            if (S_OK == result)
                cenumIDList.Attach(pEnum);
            else
                TRACE("CShellFolder::EnumObjects failed\n");
        }
        else
            TRACE("CShellFolder::EnumObjects failed\n");

        return result;
    }

    // Retrieves the subfolder for the specified pidl.
    HRESULT CShellFolder::GetSubFolder(const Cpidl& cpidl, CShellFolder& subFolder)
    {
        REFIID riid = IID_IShellFolder;
        LPSHELLFOLDER ifolder;
        LPITEMIDLIST pidl = cpidl.GetPidl();

        HRESULT result = m_pIShellFolder->BindToObject(pidl, NULL, riid, (VOID**)&ifolder);
        if (S_OK == result)
            subFolder.Attach(ifolder);
        else
        {
            TRACE("CShellFolder::BindToObject failed\n");
        }

        return result;
    }

    // Releases the memory allocated for m_IShellFolder (if any).
    void CShellFolder::Release()
    {
        if (m_pIShellFolder)
            m_pIShellFolder->Release();
        m_pIShellFolder = NULL;
    }


    //////////////////////////////////
    //CEnumIDList function definitions
    CEnumIDList::CEnumIDList() : m_pEnumIDList(NULL)
    {
    }

    CEnumIDList::~CEnumIDList()
    {
        if (m_pEnumIDList)
            m_pEnumIDList->Release();
    }

    //Converts a LPENUMIDLIST to a CEnumIDList object.
    //  The destructor will release memory allocated for the LPENUMIDLIST
    void CEnumIDList::Attach(LPENUMIDLIST EnumList)
    {
        m_pEnumIDList = EnumList;
    }

    // Retrieves the specified number of item identifiers in the enumeration sequence.
    HRESULT CEnumIDList::Next(ULONG Elements, Cpidl& cpidl, ULONG& ulFetched)
    {
        LPITEMIDLIST pidl;
        if (m_pEnumIDList)
        {
            HRESULT result = m_pEnumIDList->Next(Elements, &pidl, &ulFetched);
            if (NOERROR == result)
                cpidl.Attach(pidl);

            if ((NOERROR != result) && (S_FALSE != result))
            {
                TRACE("CEnumIDList::Next failed\n");
            }
            return result;
        }
        else
            return S_FALSE;
    }

    /////////////////////////////
    // Cpidl function definitions
    //

    // Constructor.
    Cpidl::Cpidl() : m_pidl(NULL), m_pidlParent(NULL)
    {
    }

    // Copy Constructor.
    Cpidl::Cpidl(const Cpidl& cpidlSource) : m_pidl(NULL), m_pidlParent(NULL)
    {
        Copy(cpidlSource);
    }

    // Assignment operator.
    void Cpidl::operator= (const Cpidl& cpidlSource)
    {
        if (&cpidlSource != this)
            Copy(cpidlSource);
    }

    // Equivalence operator.
    BOOL Cpidl::operator== (const Cpidl& cpidl)
    {
        return IsEqual(cpidl);
    }

    // Appends cpidlRel to cpidlFull.
    // The fully qualified cpidl must come first.
    const Cpidl operator+ (const Cpidl& cpidlFull, const Cpidl& cpidlRel)
    {
        Cpidl Temp;
        Temp.Concatenate(cpidlFull, cpidlRel);
        return Temp;
    }

    // Destructor.
    Cpidl::~Cpidl()
    {
        Delete();
        if (m_pidlParent)
            ::CoTaskMemFree(m_pidlParent);
    }

    // Attaches a ITEMIDLIST pointer to a Cpidl object.
    // The memory allocated for the original pidl is released
    // in the destructor.
    void Cpidl::Attach(LPCITEMIDLIST pidl)
    {
        Delete();
        m_pidl = (LPITEMIDLIST)pidl;
    }

    void Cpidl::Delete()
    {
        if (m_pidl)
            ::CoTaskMemFree(m_pidl);

        m_pidl = NULL;
    }

    // Creates a new Cpidl object by concatenating (chaining together)
    // a parent and relative Cpidl.
    void Cpidl::Concatenate(const Cpidl& cpidlParent, const Cpidl& cpidlRel)
    {
        Concatenate(cpidlParent.GetPidl(), cpidlRel.GetPidl());
    }

    // Create a new fully qualified Cpidl object from a Parent pidl
    // and a relative pidl.
    void Cpidl::Concatenate(LPCITEMIDLIST pidlParent, LPCITEMIDLIST pidlRel)
    {
        Delete();
        if (!pidlRel) 
            return; // pidlRel should not be NULL

        UINT cb1 = 0;

        // Calculate the size of pidlParent without the two bytes of NULL terminator.
        // This size can be zero if pidlParent points to the desktop.
        if (pidlParent)
            cb1 = GetSize(pidlParent) - (2 * sizeof(BYTE));

        UINT cb2 = GetSize(pidlRel);

        // Create a new ITEMIDLIST that is the size of both pidlParent and pidlRel,
        // then copy pidlParent and pidlRel to the new list.
        m_pidl = (LPITEMIDLIST)::CoTaskMemAlloc(cb1 + cb2);
        if (!m_pidl)
            throw CWinException(_T("Failed to allocate memory for pidl"));

        ::ZeroMemory(m_pidl, cb1 + cb2);
        if (pidlParent)
            ::CopyMemory(m_pidl, pidlParent, cb1);

        ::CopyMemory(((LPBYTE)m_pidl) + cb1, pidlRel, cb2);

    }

    // The Cpidl object stores a copy of the pidlSource's m_pidl.
    void Cpidl::Copy(const Cpidl& cpidlSource)
    {
        if (&cpidlSource != this)
            Copy(cpidlSource.GetPidl());
    }

    // The Cpidl object stores a copy of the pidlSource. The memory allocated to
    // pidlSource is NOT released.
    void Cpidl::Copy(LPCITEMIDLIST pidlSource)
    {
        UINT cbSource;
        Delete();

        if(NULL == pidlSource) return;

        //Allocate memory for m_pidl
        cbSource = GetSize(pidlSource);
        m_pidl = (LPITEMIDLIST)::CoTaskMemAlloc(cbSource);
        if (!m_pidl)
            throw CWinException(_T("Failed to allocate memory for pidl"));

        ::CopyMemory(m_pidl, pidlSource, cbSource);
    }

    // Retrieves information about an object in the file system, such as a file,
    // a folder, a directory, or a drive root.
    DWORD_PTR Cpidl::GetFileInfo(DWORD dwFileAttributes, SHFILEINFO& sfi, UINT uFlags)
    {
        LPITEMIDLIST pidl = m_pidl;
        return (DWORD_PTR)::SHGetFileInfo((LPCTSTR)pidl, dwFileAttributes, &sfi, sizeof(SHFILEINFO), uFlags);
    }

    // Returns the next item in the item identifier list.
    LPITEMIDLIST Cpidl::GetNextItem(LPCITEMIDLIST pidl)
    {
        // Check for valid pidl.
        if (NULL == pidl)
            return NULL;

        // Get the size of the specified item identifier.
        int cb = pidl->mkid.cb;

        // If the size is zero, it is the end of the list.
        if (0 == cb)
            return NULL;

        // Add cb to pidl (casting to increment by bytes).
        return (LPITEMIDLIST)(((LPBYTE)pidl) + cb);
    }

    // Returns the parent item in the item identifier list.
    LPITEMIDLIST Cpidl::GetParent()
    {
        // Make sure it's a valid PIDL.
        if (NULL == m_pidl)
            return(NULL);

        // Delete m_pidlParent
        if (m_pidlParent)
            ::CoTaskMemFree(m_pidlParent);
        m_pidlParent = NULL;

        // Copy m_pidl to m_pidlParent
        UINT size = GetSize(m_pidl);
        m_pidlParent = (LPITEMIDLIST)::CoTaskMemAlloc(size);
        if (!m_pidlParent)
            throw CWinException(_T("Failed to allocate pidlParent memory"));

        ::CopyMemory(m_pidlParent, m_pidl, size);

        // Identify the last item's position
        if (m_pidlParent->mkid.cb)
        {
            LPITEMIDLIST pidlNext = m_pidlParent;
            LPITEMIDLIST pidlTemp = m_pidlParent;

            while (pidlNext->mkid.cb)
            {
                pidlTemp = pidlNext;
                pidlNext = GetNextItem(pidlTemp);
            }
            // Remove the last item, set it's size to 0.
            pidlTemp->mkid.cb = 0;
        }

        return m_pidlParent;
    }

    // Stores a copy of the relative pidl obtained from a fully qualified pidl source.
    LPITEMIDLIST Cpidl::GetRelative()
    {
        if(NULL == m_pidl) return NULL;

        LPITEMIDLIST pidlNext = m_pidl;
        LPITEMIDLIST pidlRel  = m_pidl;

        while( pidlNext->mkid.cb )
        {
            pidlNext = GetNextItem(pidlNext);
            if (pidlNext->mkid.cb)
                pidlRel = (LPITEMIDLIST)pidlNext;
        }

        UINT nPos = GetSize(m_pidl) - GetSize(pidlRel);
        return (LPITEMIDLIST)((LPBYTE)m_pidl + nPos);
    }

    // Retrieves the total size of the item identifier list.
    UINT Cpidl::GetSize(LPCITEMIDLIST pidl)
    {
        UINT cbTotal = 0;
        if (!pidl) return 0;

        while(pidl)
        {
            cbTotal += pidl->mkid.cb;
            pidl = GetNextItem(pidl);
        }

        // Requires a 16 bit zero value for the NULL terminator
        cbTotal += 2 * sizeof(BYTE);

        return cbTotal;
    }

    // Assigns a special folder to this object.
    HRESULT Cpidl::GetSpecialFolderLocation(HWND hwnd, int csidl)
    {
        Delete();
        HRESULT result = ::SHGetSpecialFolderLocation(hwnd, csidl, &m_pidl);
        if (result != S_OK)
        {
            TRACE("Cpidl::SHGetSpecialFolderLocation failed\n");
        }
        return result;
    }

    // Returns true if the two Cpidl's identify the same item.
    BOOL Cpidl::IsEqual(const Cpidl &cpidl)
    {
        CShellFolder sf;
        sf.DesktopFolder();
        return (0 == sf.CompareIDs(0, *this, cpidl) );
    }
}
