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
    ///////////////////
    // Global functions
    //
    BOOL GetDisplayName(LPCITEMIDLIST pidlFull, LPWSTR pszDisplayName);
    BOOL GetFullFileName(LPCITEMIDLIST pidlFull, LPWSTR pszPathName);


    ////////////////////////////////////////////////////////////////
    // Cpidl is a wrapper for a LPITEMIDLIST, commonly called a pidl
    class Cpidl
    {
        friend const Cpidl operator+ (const Cpidl& cpidlFull, const Cpidl& cpidlRel);
    public:
        Cpidl();
        Cpidl(const Cpidl& pidlSource);
        ~Cpidl();

        void Attach(LPCITEMIDLIST pidl);
        void Concatenate(const Cpidl& cpidlParent, const Cpidl& cpidlRel);
        void Concatenate(LPCITEMIDLIST pidlParent, LPCITEMIDLIST pidlRel);
        void Copy(const Cpidl& cpidlSource);
        void Copy(LPCITEMIDLIST pidlSource);
        DWORD_PTR GetFileInfo(DWORD dwFileAttributes, SHFILEINFO& sfi, UINT uFlags);
        LPITEMIDLIST GetParent();
        LPITEMIDLIST GetPidl() const { return m_pidl; }
        LPITEMIDLIST GetRelative();
        HRESULT GetSpecialFolderLocation(HWND hwnd, int csidl);
        bool IsEqual(const Cpidl& cpidl) const;
        void operator= (const Cpidl& cpidlSource);
        operator LPITEMIDLIST () { return m_pidl; }

    private:
        UINT GetSize(LPCITEMIDLIST pidl);
        LPITEMIDLIST GetNextItem(LPCITEMIDLIST pidl) ;
        void Delete();

        LPITEMIDLIST m_pidl;
        LPITEMIDLIST m_pidlParent;
    };

    // Equality operator overload.
    bool operator==(const Cpidl& lhs, const Cpidl& rhs);


    ///////////////////////////////////////////////////////////
    // CContextMenu2 is a wrapper for an IContextMenu2 pointer.
    class CContextMenu2
    {
    public:
        CContextMenu2();
        ~CContextMenu2();

        void Attach(IContextMenu2* pIContextMenu2);
        HRESULT HandleMenuMsg(UINT msg, WPARAM wparam, LPARAM lparam);
        IContextMenu2* GetIContextMenu2() {return m_pIContextMenu2;}
        void Release();

    private:
        IContextMenu2* m_pIContextMenu2;
    };


    /////////////////////////////////////////////////////////
    // CContextMenu is a wrapper for an IContextMenu pointer.
    class CContextMenu
    {
    public:
        CContextMenu();
        ~CContextMenu();

        void    Attach(IContextMenu* pContextMenu);
        HRESULT Invoke(CMINVOKECOMMANDINFO& Ici);
        HRESULT GetContextMenu2(CContextMenu2& cm2);
        HRESULT QueryContextMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);

    private:
        IContextMenu* m_pIContextMenu;
    };


    ///////////////////////////////////////////////
    // CEnumIDList is a wrapper for a LPENUMIDLIST.
    //
    class CEnumIDList
    {
    public:
        CEnumIDList();
        ~CEnumIDList();
        void Attach(LPENUMIDLIST EnumList);
        HRESULT Next(ULONG Elements, Cpidl& cpidl, ULONG& ulFetched);

    private:
        LPENUMIDLIST m_pEnumIDList;
    };


    /////////////////////////////////////////////////
    // CShellFolder is a wrapper for a LPSHELLFOLDER.
    class CShellFolder
    {
    public:
        CShellFolder();
        CShellFolder(const CShellFolder& csfSource);
        void operator=(const CShellFolder& csfSource);
        ~CShellFolder();

        HRESULT AddRef() { return m_pIShellFolder->AddRef(); }
        HRESULT CompareIDs(LPARAM lparam, const Cpidl& cpidl1, const Cpidl& cpidl2);
        HRESULT DesktopFolder();
        HRESULT GetAttributes(UINT cidl, const Cpidl& cpidl, ULONG& rgfInOut);
        HRESULT GetContextMenu(HWND owner, CContextMenu& ccm);
        HRESULT GetContextMenu(HWND owner, UINT items, LPCITEMIDLIST* pidlArray, CContextMenu& cm);
        HRESULT GetEnumIDList(HWND owner, int grfFlags, CEnumIDList& cenumIDList);
        LPSHELLFOLDER GetIShellFolder() {return m_pIShellFolder;}
        HRESULT GetSubFolder(const Cpidl& pidl, CShellFolder& subFolder);
        void Release();

    private:
        void Attach(LPSHELLFOLDER ShellFolder);
        void Copy(const CShellFolder& Source);
        void Copy(LPSHELLFOLDER Source);

        LPSHELLFOLDER m_pIShellFolder;
    };

} //namespace ShellWrapper

#endif  // define SHELLWRAPPER_H

