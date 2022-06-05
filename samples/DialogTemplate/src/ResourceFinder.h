////////////////////////////////////////////////////
// ResourceFinder.h - Declaration of CResourceFinder
//

#ifndef _RESOURCE_FINDER_H_
#define _RESOURCE_FINDER_H_

struct ResourceInfo
{
    ResourceInfo() :  languageID(0), resourceID(0), typeID(0) {}
    void Empty()
    {
        typeName.Empty();
        resourceName.Empty();
        languageID = 0;
        resourceID = 0;
        typeID = 0;
    }

    CString typeName;
    CString resourceName;
    int languageID;
    size_t resourceID;
    size_t typeID;
};

//////////////////////////////////////////////////////////
// CResourceFinder finds the resources contained within an
// executable or DLL file.
class CResourceFinder
{
public:
    CResourceFinder() {}
    ~CResourceFinder() {}

    void FindResources(LPCTSTR fileName);
    const std::vector<ResourceInfo>& GetAllInfo() const { return m_allInfo; }

private:
    // Callback procedures
    static BOOL EnumTypesProc(HANDLE, LPTSTR, LONG_PTR);
    static BOOL EnumNamesProc(HANDLE, LPCTSTR, LPCTSTR, LONG_PTR);
    static BOOL EnumLangsProc(HANDLE, LPCTSTR, LPCTSTR, WORD, LONG_PTR);

    // private member variables
    ResourceInfo m_info;
    std::vector<ResourceInfo> m_allInfo;
    CString m_fileName;
};

#endif // _RESOURCE_FINDER_H_