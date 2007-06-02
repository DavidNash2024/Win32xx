//////////////////////////////////////
// ShellWrapper.cpp

#include "../Win32++/Wincore.h"
#include "ShellWrapper.h"


namespace ShellWrapper
{

	BOOL GetFullFileName(LPCITEMIDLIST pidlFull, LPTSTR pszPathName)
	{
		if (!::SHGetPathFromIDList(pidlFull, pszPathName))
		{
			pszPathName[0] = TEXT('\0');
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
			pszDisplayName[0] = TEXT('\0');
			return FALSE;
		}

		::lstrcpyn(pszDisplayName, sfi.szDisplayName, MAX_PATH -1);
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
		HRESULT hr = m_pIContextMenu->InvokeCommand(&Ici);

		if (hr != NOERROR)
		{
			TRACE(TEXT("CContextMenu::InvokeCommand failed"));
		}
		return hr;
	}

	HRESULT CContextMenu::QueryInterface(REFIID iid, CContextMenu2& ccm2)
	{
		HRESULT hr = 0;
		if (iid == IID_IContextMenu2)
		{
			IContextMenu2* pIContextMenu2 = NULL;
			hr = m_pIContextMenu->QueryInterface(iid, (VOID**)&pIContextMenu2);
			if (hr == S_OK)
				ccm2.Attach(pIContextMenu2);
			else
			{
				TRACE(TEXT("CContextMenu::QueryInterface failed"));
			}
		}
		else
			TRACE(TEXT("Not Implemented!"));

		return hr;
	}

	HRESULT CContextMenu::QueryContextMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
	{
		HRESULT hr = m_pIContextMenu->QueryContextMenu(hmenu, indexMenu, idCmdFirst, idCmdLast, uFlags) ;
		if(hr & 0x80000000)
		{
			TRACE(TEXT("CContextMenu::QueryContextMenu failed"));
		}
		return hr;
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

	HRESULT CContextMenu2::HandleMenuMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		HRESULT hr = m_pIContextMenu2->HandleMenuMsg(uMsg, wParam, lParam);

		if((hr != S_OK) && (hr !=E_NOTIMPL))
		{
			TRACE(TEXT("CContextMenu2::HandleMenuMsg failed"));
		}

		return hr;
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

		HRESULT hr = m_IShellFolder->BindToObject(pidl, pbc, riid, (VOID**)&FolderTemp);
		if(hr == S_OK)
			NewFolder.Attach(FolderTemp);
		else
		{
			TRACE(TEXT("CShellFolder::BindToObject failed"));
		}

		return hr;
	}

	HRESULT CShellFolder::CompareIDs(LPARAM lParam, const Cpidl& cpidl1, const Cpidl& cpidl2)
	{
		LPITEMIDLIST pidl1 = cpidl1.GetPidl();
		LPITEMIDLIST pidl2 = cpidl2.GetPidl();
		return m_IShellFolder->CompareIDs(lParam, pidl1, pidl2);
	}

	void CShellFolder::Copy(LPSHELLFOLDER Source)
	//Assigns the value to the CShellFolder object.
	{
		Delete(); //Release the current m_IShellFolder
		m_IShellFolder = Source;
		if (Source) AddRef();
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
		HRESULT hr = m_IShellFolder->CreateViewObject(hwnd, riid, (LPVOID*)&pcm);
		if (hr == S_OK)
			ccm.Attach(pcm);
		else
		{
			TRACE(TEXT("CShellFolder::CreateViewObject failed"));
		}
		return hr;
	}


	HRESULT CShellFolder::EnumObjects(HWND hwndOwner, int grfFlags, CEnumIDList& cenumIDList)
	{
		LPENUMIDLIST pEnum;
		HRESULT hr = m_IShellFolder->EnumObjects(hwndOwner, grfFlags, &pEnum);
		if (hr == S_OK)
			cenumIDList.Attach(pEnum);
		else
		{
			TRACE(TEXT("CShellFolder::EnumObjects failed"));
		}
		return hr;
	}

	HRESULT CShellFolder::GetAttributesOf(UINT cidl, const Cpidl& cpidl, ULONG& rgfInOut)
	{
		LPCITEMIDLIST pidl = cpidl.GetPidl();
		HRESULT hr = m_IShellFolder->GetAttributesOf(cidl, &pidl, &rgfInOut);

		if (hr != S_OK)
		{
			TRACE(TEXT("CShellFolder::GetAttributesOf failed"));
		}
		return hr;
	}

	HRESULT CShellFolder::SHGetDesktopFolder()
	{
		HRESULT hr = ::SHGetDesktopFolder(&m_IShellFolder);

		if (hr != NOERROR)
		{
			TRACE(TEXT("CShellFolder::SHGetDesktopFolder failed"));
		}
		return hr;
	}

	HRESULT CShellFolder::GetUIObjectOf(HWND hwndOwner, UINT nItems, Cpidl* cpidlArray, REFIID riid, UINT rgfReserved, CContextMenu& cm)
	//cpidlArray is either an array of Cpidl or a pointer to a single Cpidl
	{
		LPCITEMIDLIST* pPidlArray;
		pPidlArray = (LPCITEMIDLIST*)::CoTaskMemAlloc(sizeof(LPITEMIDLIST) * nItems);
		if(!pPidlArray) return 0;

		for(UINT i = 0; i < nItems; i++)
			pPidlArray[i] = cpidlArray[i].GetPidl();

		IContextMenu* ppv;
		HRESULT hr = m_IShellFolder->GetUIObjectOf(hwndOwner, nItems, pPidlArray, riid, &rgfReserved, (VOID**)&ppv);

		if (hr == S_OK)
			cm.Attach(ppv);
		else
		{
			TRACE(TEXT("CShellFolder::GetUIObjectOf failed"));
		}

		CoTaskMemFree(pPidlArray);
		return hr;
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
			HRESULT hr = m_pEnumIDList->Next(Elements, &pidl, &ulFetched);
			if (hr == NOERROR)
				cpidl.Attach(pidl);

			if ((hr != NOERROR) && (hr != S_FALSE))
			{
				TRACE(TEXT("CEnumIDList::Next failed"));
			}
			return hr;
		}
		else
			return S_FALSE;
	}

	//Cpidl function definitions
	Cpidl::Cpidl() : m_pidl(NULL), m_pidlParent(NULL)
	{
		::SHGetMalloc(&m_pMalloc);
	}

	Cpidl::Cpidl(const Cpidl& cpidlSource) : m_pidl(NULL), m_pidlParent(NULL)
	{
		::SHGetMalloc(&m_pMalloc);
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
			m_pMalloc->Free(m_pidlParent);

		m_pMalloc->Release();
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
			m_pMalloc->Free(m_pidl);
		m_pidl = NULL;
	}

	HRESULT Cpidl::SHGetSpecialFolderLocation(HWND hwnd, int csidl)
	{
		Delete(); //Release the memory for m_pidl
		HRESULT hr = ::SHGetSpecialFolderLocation(hwnd, csidl, &m_pidl);
		if (hr != S_OK)
		{
			TRACE(TEXT("Cpidl::SHGetSpecialFolderLocation failed"));
		}
		return hr;
	}

	DWORD_PTR Cpidl::SHGetFileInfo(DWORD dwFileAttributes, SHFILEINFO& sfi, UINT uFlags)
	{
		LPITEMIDLIST pidl = m_pidl;
		return ::SHGetFileInfo((LPCTSTR)pidl, dwFileAttributes, &sfi, sizeof(SHFILEINFO), uFlags);
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

		if(pidlSource == NULL) return;

		//Allocate memory for m_pidl
		cbSource = GetSize(pidlSource);
		m_pidl = (LPITEMIDLIST)m_pMalloc->Alloc(cbSource);
		if (!m_pidl) return;

		::CopyMemory(m_pidl, pidlSource, cbSource);
	}

	LPITEMIDLIST Cpidl::GetParent()
	{
	// Delete m_pidlParent
		UINT nSize = GetSize(m_pidl);
		if (m_pidlParent)
			m_pMalloc->Free(m_pidlParent);
		m_pidlParent = NULL;

	// Make sure it's a valid PIDL.
		if (m_pidl == NULL)
			return(NULL);

	// Copy m_pidl to m_pidlParent
		m_pidlParent = (LPITEMIDLIST)m_pMalloc->Alloc(nSize);
		if (!m_pidlParent)
			return NULL;

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
		if(m_pidl == NULL) return NULL;

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
		m_pidl = (LPITEMIDLIST)m_pMalloc->Alloc(cb1 + cb2);
		if (m_pidl)
		{
			::ZeroMemory(m_pidl, cb1 + cb2);
			if (pidlParent)
				::CopyMemory(m_pidl, pidlParent, cb1);

			::CopyMemory(((LPBYTE)m_pidl) + cb1, pidlRel, cb2);
		}
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
		if(pidl == NULL)
			return NULL;

		// Get the size of the specified item identifier.
		int cb = pidl->mkid.cb;

		// If the size is zero, it is the end of the list.
		if (cb == 0)
			return NULL;

		// Add cb to pidl (casting to increment by bytes).
		return (LPITEMIDLIST) (((LPBYTE) pidl) + cb);
	}

	BOOL Cpidl::IsEqual(const Cpidl &cpidl)
	{
		CShellFolder sf;
		sf.SHGetDesktopFolder();
		return (sf.CompareIDs(0, *this, cpidl) == 0);
	}
}
