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
        ~Cpidl();

        HRESULT SHGetSpecialFolderLocation(HWND hwnd, int csidl);
        DWORD_PTR SHGetFileInfo(DWORD dwFileAttributes, SHFILEINFO& sfi, UINT uFlags);
        void Copy(const Cpidl& cpidlSource);
        void Copy(LPCITEMIDLIST pidlSource);
        void Concatenate(const Cpidl& cpidlParent, const Cpidl& cpidlRel);
        void Concatenate(LPCITEMIDLIST pidlParent, LPCITEMIDLIST pidlRel);
        LPITEMIDLIST GetParent();
        LPITEMIDLIST GetPidl() const { return m_pidl; }
        LPITEMIDLIST GetRelative();
        BOOL IsEqual(const Cpidl& cpidl);
        void operator= (const Cpidl& cpidlSource);
        BOOL operator== (const Cpidl& cpidl);
        operator LPITEMIDLIST () { return m_pidl; }
        
        //a global function declared as friend
        friend const Cpidl operator+ (const Cpidl& cpidlFull, const Cpidl& cpidlRel);

    private:
        void Attach(LPCITEMIDLIST pidl);
        UINT GetSize(LPCITEMIDLIST pidl);
        LPITEMIDLIST GetNextItem(LPCITEMIDLIST pidl) ;
        void Delete();

        LPITEMIDLIST m_pidl;
        LPITEMIDLIST m_pidlParent;
    };


    //Wrapper class for an IContextMenu2 pointer
    class CContextMenu2
    {
    friend class CContextMenu;
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

    //Wrapper class for an IContextMenu pointer
    class CContextMenu
    {
    friend class CShellFolder;
    public:
        CContextMenu();
        ~CContextMenu();

        HRESULT InvokeCommand(CMINVOKECOMMANDINFO& Ici);
        HRESULT QueryInterface(REFIID iid, CContextMenu2& cm2);
        HRESULT QueryContextMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);

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
        ~CEnumIDList();
        HRESULT Next(ULONG Elements, Cpidl& cpidl, ULONG& ulFetched);

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
        ~CShellFolder();
        void Delete();
        HRESULT BindToObject(const Cpidl& pidl, LPBC pbc, REFIID riid, CShellFolder& NewFolder);
        HRESULT CompareIDs(LPARAM lparam, const Cpidl& cpidl1, const Cpidl& cpidl2);
        void Copy(const CShellFolder& Source);
        void Copy(LPSHELLFOLDER Source);
        HRESULT CreateViewObject(HWND hwnd, REFIID riid, CContextMenu& ccm);
        HRESULT EnumObjects(HWND hwndOwner, int grfFlags, CEnumIDList& cenumIDList);
        HRESULT GetAttributesOf(UINT cidl, const Cpidl& cpidl, ULONG& rgfInOut);
        HRESULT SHGetDesktopFolder();
        HRESULT GetUIObjectOf(HWND hwndOwner, UINT nItems, LPCITEMIDLIST* pidlArray, REFIID riid, CContextMenu& cm);
        LPSHELLFOLDER GetIShellFolder() {return m_IShellFolder;}

    private:
        HRESULT AddRef() {return m_IShellFolder->AddRef();}
        void Attach(LPSHELLFOLDER ShellFolder);

        LPSHELLFOLDER m_IShellFolder;
    }; //class CShellFolder

} //namespace ShellWrapper

#endif  // define SHELLWRAPPER_H

