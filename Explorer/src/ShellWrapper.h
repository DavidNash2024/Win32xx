//////////////////////////////////////////////////////
// ShellWrapper.h
//  Declaration of the CContextMenu, CContextMenu2,
//    CEnumIDList, Cpidl, and CShellFolder classes

#ifndef SHELLWRAPPER_H
#define SHELLWRAPPER_H

#include <shlobj.h>


//For Visual C++ 6 and without a modern SDK
#ifndef DWORD_PTR
#define DWORD_PTR DWORD
#endif


namespace ShellWrapper
{

	//Global functions
	BOOL GetFullFileName(LPCITEMIDLIST pidlFull, LPTSTR pszPathName);
	BOOL GetDisplayName(LPCITEMIDLIST pidlFull, LPTSTR pszDisplayName);

	//Wrapper class for a LPITEMIDLIST commonly called a pidl.
	class Cpidl
	{
	friend class CEnumIDList;
	public:
		Cpidl();
		Cpidl(const Cpidl& pidlSource);
		virtual ~Cpidl();

		virtual HRESULT SHGetSpecialFolderLocation(HWND hwnd, int csidl);
		virtual DWORD_PTR SHGetFileInfo(DWORD dwFileAttributes, SHFILEINFO& sfi, UINT uFlags);
		virtual void Copy(const Cpidl & cpidlSource);
		virtual void Copy(LPCITEMIDLIST pidlSource);
		virtual void Concatenate(const Cpidl& cpidlParent, const Cpidl& cpidlRel);
		virtual void Concatenate(LPCITEMIDLIST pidlParent, LPCITEMIDLIST pidlRel);
		virtual LPITEMIDLIST GetParent();
		virtual LPITEMIDLIST GetPidl() const {return m_pidl;}
		virtual	LPITEMIDLIST GetRelative();
		virtual BOOL IsEqual(const Cpidl &cpidl);
		void operator= (const Cpidl& cpidlSource);
		BOOL operator== (const Cpidl& cpidl);
		
		//a global function declared as friend
		friend const Cpidl operator+ (const Cpidl& cpidlFull, const Cpidl& cpidlRel);

	private:
		void Attach(LPCITEMIDLIST pidl);
		UINT GetSize(LPCITEMIDLIST pidl);
		LPITEMIDLIST GetNextItem(LPCITEMIDLIST pidl) ;
		void Delete();

		LPITEMIDLIST m_pidl;
		LPITEMIDLIST m_pidlParent;
		LPMALLOC m_pMalloc;
	};


	//Wrapper class for an IContextMenu2 pointer
	class CContextMenu2
	{
	friend class CContextMenu;
	public:
		CContextMenu2();
		virtual ~CContextMenu2();

		void Attach(IContextMenu2* pIContextMenu2);
		virtual HRESULT HandleMenuMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual IContextMenu2* GetIContextMenu2() {return m_pIContextMenu2;}
		void Release();

	private:
		IContextMenu2* m_pIContextMenu2;

	};

	//Wrapper class for an IContextMenu pointer
	class CContextMenu
	{
	friend class CShellFolder;
	public:
		CContextMenu();
		virtual ~CContextMenu();

		virtual HRESULT InvokeCommand(CMINVOKECOMMANDINFO& Ici);
		virtual HRESULT QueryInterface(REFIID iid, CContextMenu2& cm2);
		virtual HRESULT QueryContextMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);

	private:
		void Attach(IContextMenu* pContextMenu);
		IContextMenu* m_pIContextMenu;
	};

	//Wrapper class for a LPENUMIDLIST
	class CEnumIDList
	{
	friend class CShellFolder;
	public:
		CEnumIDList();
		virtual ~CEnumIDList();
		virtual HRESULT Next(ULONG Elements, Cpidl& cpidl, ULONG& ulFetched);

	private:
		void Attach(LPENUMIDLIST EnumList);
		LPENUMIDLIST m_pEnumIDList;
	};


	//Wrapper class for a LPSHELLFOLDER
	class CShellFolder
	{
	public:
		CShellFolder();
		CShellFolder(const CShellFolder& csfSource);
		void operator=(const CShellFolder& csfSource);
		virtual ~CShellFolder();
		virtual void Delete();
		virtual HRESULT BindToObject(const Cpidl& pidl, LPBC pbc, REFIID riid, CShellFolder& NewFolder);
		virtual HRESULT CompareIDs(LPARAM lParam, const Cpidl& cpidl1, const Cpidl& cpidl2);
		virtual void Copy(const CShellFolder& Source);
		virtual void Copy(LPSHELLFOLDER Source);
		virtual HRESULT CreateViewObject(HWND hwnd, REFIID riid, CContextMenu& ccm);
		virtual HRESULT EnumObjects(HWND hwndOwner, int grfFlags, CEnumIDList& cenumIDList);
		virtual HRESULT GetAttributesOf(UINT cidl, const Cpidl& cpidl, ULONG& rgfInOut);
		virtual HRESULT SHGetDesktopFolder();
		virtual HRESULT GetUIObjectOf(HWND hwndOwner, UINT nItems, Cpidl* cpidlArray, REFIID riid, UINT rgfReserved, CContextMenu& cm);
		virtual LPSHELLFOLDER GetIShellFolder() {return m_IShellFolder;}

	private:
		HRESULT AddRef() {return m_IShellFolder->AddRef();}
		void Attach(LPSHELLFOLDER ShellFolder);

		LPSHELLFOLDER m_IShellFolder;
	}; //class CShellFolder

} //namespace ShellWrapper

#endif  // define SHELLWRAPPER_H

