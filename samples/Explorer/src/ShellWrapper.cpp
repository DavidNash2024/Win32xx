//////////////////////////////////////
// ShellWrapper.cpp

#include "stdafx.h"
#include "ShellWrapper.h"


/////////////////////////////////////////////////////////////
// Global function in the ShellWrapper namespace
//

namespace ShellWrapper
{
    ///////////////////////////////////
    //global functionnct definitions (within this namespace)
    BOOL GetFullFileName(LPCITEMIDLIST pidlFull, LPTSTR pszPathName)
    {
        if (!::SHGetPathFromIDList(pidlFull, pszPathName))
        {
            pszPathName[0] = _T('\0');
            return FALSE;
        }

        return TRUE;
    }

    BOOL GetDisplayName(LPCITEMIDLIST pidlFull, LPTSTR pszDisplayName)
    {
        SHFILEINFO     sfi;

        // Get the display name of the item
        if(!::SHGetFileInfo((LPCTSTR)pidlFull, 0, &sfi, sizeof(sfi), SHGFI_PIDL | SHGFI_DISPLAYNAME))
        {
            pszDisplayName[0] = _T('\0');
            return FALSE;
        }

        StrCopy(pszDisplayName, sfi.szDisplayName, MAX_PATH -1);
        return TRUE;
    }


    ///////////////////////////////////
    //CContextMenu function definitions
    CContextMenu::CContextMenu() : m_pIContextMenu(NULL)
    {
    }

    CContextMenu::~CContextMenu()
    {
        if (m_pIContextMenu)
            m_pIContextMenu->Release();
    }

    void CContextMenu::Attach(IContextMenu* pIContextMenu)
    {
        if (m_pIContextMenu)
            m_pIContextMenu->Release();
        m_pIContextMenu = pIContextMenu;
    }

    HRESULT CContextMenu::InvokeCommand(CMINVOKECOMMANDINFO& Ici)
    {
        HRESULT result = m_pIContextMenu->InvokeCommand(&Ici);

        if (result != NOERROR)
        {
            TRACE("CContextMenu::InvokeCommand failed\n");
        }
        return result;
    }

    HRESULT CContextMenu::QueryInterface(REFIID iid, CContextMenu2& ccm2)
    {
        HRESULT result = 0;
        if (IID_IContextMenu2 == iid)
        {
            IContextMenu2* pIContextMenu2 = NULL;
            result = m_pIContextMenu->QueryInterface(iid, (VOID**)&pIContextMenu2);
            if (S_OK == result)
                ccm2.Attach(pIContextMenu2);
            else
            {
                TRACE("CContextMenu::QueryInterface failed\n");
            }
        }
        else
            TRACE("Not Implemented!\n");

        return result;
    }

    HRESULT CContextMenu::QueryContextMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
    {
        HRESULT result = m_pIContextMenu->QueryContextMenu(hmenu, indexMenu, idCmdFirst, idCmdLast, uFlags) ;
        if(result & 0x80000000)
        {
            TRACE("CContextMenu::QueryContextMenu failed\n");
        }
        return result;
    }


    ////////////////////////////////////
    //CContextMenu2 function definitions
    CContextMenu2::CContextMenu2() : m_pIContextMenu2(NULL)
    {
    }

    CContextMenu2::~CContextMenu2()
    {
        if (m_pIContextMenu2)
            m_pIContextMenu2->Release();
    }

    void CContextMenu2::Attach(IContextMenu2* pIContextMenu2)
    {
        if (m_pIContextMenu2)
            m_pIContextMenu2->Release();
        m_pIContextMenu2 = pIContextMenu2;
    }

    HRESULT CContextMenu2::HandleMenuMsg(UINT msg, WPARAM wparam, LPARAM lparam)
    {
        HRESULT result = m_pIContextMenu2->HandleMenuMsg(msg, wparam, lparam);

        if((result != S_OK) && (result !=E_NOTIMPL))
        {
            TRACE("CContextMenu2::HandleMenuMsg failed\n");
        }

        return result;
    }

    void CContextMenu2::Release()
    {
        m_pIContextMenu2->Release();
        m_pIContextMenu2 = 0;
    }


    ///////////////////////////////////
    //CShellFolder function definitions
    CShellFolder::CShellFolder() : m_IShellFolder(NULL)
    {
    }

    CShellFolder::CShellFolder(const CShellFolder& csfSource) : m_IShellFolder(NULL)
    {
        Copy(csfSource);
    }

    void CShellFolder::operator=(const CShellFolder& csfSource)
    {
        Copy(csfSource);
    }

    CShellFolder::~CShellFolder()
    {
        Delete();
    }

    void CShellFolder::Attach(LPSHELLFOLDER IShellFolder)
    //Converts SHELLFOLDER pointer to a CShellFolder object.
    //The memory allocated for the SHELLFOLDER pointer is released
    //  in the destructor
    {
        Delete();
        m_IShellFolder = IShellFolder;
    }

    HRESULT CShellFolder::BindToObject(const Cpidl& cpidl, LPBC pbc, REFIID riid, CShellFolder& NewFolder)
    {
        LPSHELLFOLDER FolderTemp;
        LPITEMIDLIST pidl = cpidl.GetPidl();

        HRESULT result = m_IShellFolder->BindToObject(pidl, pbc, riid, (VOID**)&FolderTemp);
        if(S_OK == result)
            NewFolder.Attach(FolderTemp);
        else
        {
            TRACE("CShellFolder::BindToObject failed\n");
        }

        return result;
    }

    HRESULT CShellFolder::CompareIDs(LPARAM lparam, const Cpidl& cpidl1, const Cpidl& cpidl2)
    {
        LPITEMIDLIST pidl1 = cpidl1.GetPidl();
        LPITEMIDLIST pidl2 = cpidl2.GetPidl();
        return m_IShellFolder->CompareIDs(lparam, pidl1, pidl2);
    }

    void CShellFolder::Copy(LPSHELLFOLDER Source)
    //Assigns the value to the CShellFolder object.
    {
        Delete(); //Release the current m_IShellFolder
        m_IShellFolder = Source;
        if (Source) 
            AddRef();
    }

    void CShellFolder::Copy(const CShellFolder& Source)
    //Assigns the value to the CShellFolder object.
    {
        Delete(); //Release the current m_IShellFolder
        m_IShellFolder = Source.m_IShellFolder;
        if (Source.m_IShellFolder)
            AddRef();
    }

    HRESULT CShellFolder::CreateViewObject(HWND hwnd, REFIID riid, CContextMenu& ccm)
    {
        IContextMenu* pcm;
        HRESULT result = m_IShellFolder->CreateViewObject(hwnd, riid, (LPVOID*)&pcm);
        if (S_OK == result)
            ccm.Attach(pcm);
        else
        {
            TRACE("CShellFolder::CreateViewObject failed\n");
        }
        return result;
    }

    HRESULT CShellFolder::EnumObjects(HWND hwndOwner, int grfFlags, CEnumIDList& cenumIDList)
    {
        LPENUMIDLIST pEnum;
        HRESULT result = 0;
        if (m_IShellFolder)
        {
            result = m_IShellFolder->EnumObjects(hwndOwner, grfFlags, &pEnum);
            if (S_OK == result)
                cenumIDList.Attach(pEnum);
            else
                TRACE("CShellFolder::EnumObjects failed\n");
        }
        else
            TRACE("CShellFolder::EnumObjects failed\n");

        return result;
    }

    HRESULT CShellFolder::GetAttributesOf(UINT cidl, const Cpidl& cpidl, ULONG& rgfInOut)
    {
        LPCITEMIDLIST pidl = cpidl.GetPidl();
        HRESULT result = m_IShellFolder->GetAttributesOf(cidl, &pidl, &rgfInOut);

        if (result != S_OK)
        {
            TRACE("CShellFolder::GetAttributesOf failed\n");
        }
        return result;
    }

    HRESULT CShellFolder::SHGetDesktopFolder()
    {
        HRESULT result = ::SHGetDesktopFolder(&m_IShellFolder);

        if (result != NOERROR)
        {
            TRACE("CShellFolder::SHGetDesktopFolder failed\n");
        }
        return result;
    }

    HRESULT CShellFolder::GetUIObjectOf(HWND hwndOwner, UINT nItems, Cpidl* cpidlArray, REFIID riid, UINT rgfReserved, CContextMenu& cm)
    //cpidlArray is either an array of Cpidl or a pointer to a single Cpidl
    {
        LPCITEMIDLIST* pPidlArray;
        pPidlArray = (LPCITEMIDLIST*)::CoTaskMemAlloc(sizeof(LPITEMIDLIST) * nItems);
        if(!pPidlArray)
            throw CWinException(_T("Failed to allocate memory for pidl array"));

        for(UINT i = 0; i < nItems; i++)
            pPidlArray[i] = cpidlArray[i].GetPidl();

        IContextMenu* ppv;
        HRESULT result = m_IShellFolder->GetUIObjectOf(hwndOwner, nItems, pPidlArray, riid, &rgfReserved, (VOID**)&ppv);

        if (S_OK == result)
            cm.Attach(ppv);
        else
        {
            TRACE("CShellFolder::GetUIObjectOf failed\n");
        }

        CoTaskMemFree(pPidlArray);
        return result;
    }

    //Deletes the memory allocated for m_IShellFolder (if any)
    void CShellFolder::Delete()
    {
        if (m_IShellFolder)
            m_IShellFolder->Release();
        m_IShellFolder = NULL;
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

    //Cpidl function definitions
    Cpidl::Cpidl() : m_pidl(NULL), m_pidlParent(NULL)
    {
    }

    Cpidl::Cpidl(const Cpidl& cpidlSource) : m_pidl(NULL), m_pidlParent(NULL)
    {
        Copy(cpidlSource);
    }

    void Cpidl::operator= (const Cpidl& cpidlSource)
    {
        Copy(cpidlSource);
    }

    BOOL Cpidl::operator== (const Cpidl& cpidl)
    {
        return IsEqual(cpidl);
    }

    const Cpidl operator+ (const Cpidl& cpidlFull, const Cpidl& cpidlRel)
    {
        //Appends cpidlRel to cpidlFull.
        //The order of the cpidls DOES matter.
        //The fully qualified cpidl must come first.
        Cpidl Temp;
        Temp.Concatenate(cpidlFull, cpidlRel);
        return Temp;
    }

    Cpidl::~Cpidl()
    {
        Delete();
        if (m_pidlParent)
            ::CoTaskMemFree(m_pidlParent);
    }

    void Cpidl::Attach(LPCITEMIDLIST pidl)
    //Converts a ITEMIDLIST pointer to a Cpidl object
    //The memory allocated for the original pidl will be released
    //  in the destructor
    {
        Delete();  //Release the memory for m_pidl
        m_pidl = (LPITEMIDLIST)pidl;
    }

    void Cpidl::Delete()
    {
        if (m_pidl)
            ::CoTaskMemFree(m_pidl);

        m_pidl = NULL;
    }

    HRESULT Cpidl::SHGetSpecialFolderLocation(HWND hwnd, int csidl)
    {
        Delete(); //Release the memory for m_pidl
        HRESULT result = ::SHGetSpecialFolderLocation(hwnd, csidl, &m_pidl);
        if (result != S_OK)
        {
            TRACE("Cpidl::SHGetSpecialFolderLocation failed\n");
        }
        return result;
    }

    DWORD_PTR Cpidl::SHGetFileInfo(DWORD dwFileAttributes, SHFILEINFO& sfi, UINT uFlags)
    {
        LPITEMIDLIST pidl = m_pidl;
        return (DWORD_PTR)::SHGetFileInfo((LPCTSTR)pidl, dwFileAttributes, &sfi, sizeof(SHFILEINFO), uFlags);
    }

    void Cpidl::Copy(const Cpidl& cpidlSource)
    //The Cpidl object stores a copy of the pidlSource's m_pidl.
    {
        Copy(cpidlSource.GetPidl());
    }

    void Cpidl::Copy(LPCITEMIDLIST pidlSource)
    //The Cpidl object stores a copy of the pidlSource. The memory allocated to
    //  pidlSource is NOT released.
    {
        UINT cbSource;
        Delete(); //Release the memory for m_pidl

        if(NULL == pidlSource) return;

        //Allocate memory for m_pidl
        cbSource = GetSize(pidlSource);
        m_pidl = (LPITEMIDLIST)::CoTaskMemAlloc(cbSource);
        if (!m_pidl)
            throw CWinException(_T("Failed to allocate memory for pidl"));

        ::CopyMemory(m_pidl, pidlSource, cbSource);
    }

    LPITEMIDLIST Cpidl::GetParent()
    {
    // Delete m_pidlParent
        UINT nSize = GetSize(m_pidl);
        if (m_pidlParent)
            ::CoTaskMemFree(m_pidlParent);
        m_pidlParent = NULL;

    // Make sure it's a valid PIDL.
        if (NULL == m_pidl)
            return(NULL);

    // Copy m_pidl to m_pidlParent
        m_pidlParent = (LPITEMIDLIST)::CoTaskMemAlloc(nSize);
        if (!m_pidlParent)
            throw CWinException(_T("Failed to allocate pidlParent memory"));

        ::CopyMemory(m_pidlParent, m_pidl, nSize);

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

    LPITEMIDLIST Cpidl::GetRelative()
    //Stores a copy of the relative pidl obtained from a fully qualified pidl source.
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

    void Cpidl::Concatenate(const Cpidl& cpidlParent, const Cpidl& cpidlRel)
    //Creates a new Cpidl object by concatenating (chain together)
    //  a parent and relative Cpidl.
    {
        Concatenate(cpidlParent.GetPidl(), cpidlRel.GetPidl());
    }

    void Cpidl::Concatenate(LPCITEMIDLIST pidlParent, LPCITEMIDLIST pidlRel)
    //Create a new fully qualified Cpidl object from a Parent pidl
    //  and a relative pidl.
    {
        UINT cb1 = 0;
        UINT cb2;

        Delete(); //Release the current m_pidl

        if (!pidlRel) return; //pidlRel should not be NULL

        // Calculate the size of pidlParent without the two bytes of NULL terminator.
        // This size can be zero if pidlParent points to the desktop.
        if(pidlParent)
            cb1 = GetSize(pidlParent) - (2 * sizeof(BYTE));

        cb2 = GetSize(pidlRel);

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

    LPITEMIDLIST Cpidl::GetNextItem(LPCITEMIDLIST pidl)
    {
        // Check for valid pidl.
        if(NULL == pidl)
            return NULL;

        // Get the size of the specified item identifier.
        int cb = pidl->mkid.cb;

        // If the size is zero, it is the end of the list.
        if (0 == cb)
            return NULL;

        // Add cb to pidl (casting to increment by bytes).
        return (LPITEMIDLIST) (((LPBYTE) pidl) + cb);
    }

    BOOL Cpidl::IsEqual(const Cpidl &cpidl)
    {
        CShellFolder sf;
        sf.SHGetDesktopFolder();
        return (0 == sf.CompareIDs(0, *this, cpidl) );
    }
}
