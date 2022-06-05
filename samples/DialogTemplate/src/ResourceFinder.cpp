///////////////////////////////////////
// ResourceFinder.cpp
//


#include "stdafx.h"
#include "ResourceFinder.h"

///////////////////////////////////////
// CResourceFinder function definitions
//

// Finds the resources in the specified executable or DLL file.
void CResourceFinder::FindResources(LPCTSTR fileName)
{
    m_fileName.Empty();
    m_allInfo.clear();
    HMODULE module = LoadLibraryEx(fileName, NULL, LOAD_LIBRARY_AS_DATAFILE);
    if (module != NULL)
    {
        // Find all of the loaded file's resources.
        EnumResourceTypes(module, reinterpret_cast<ENUMRESTYPEPROC>(EnumTypesProc),
                          reinterpret_cast<LONG_PTR>(this));

        FreeLibrary(module);
        m_fileName = fileName;
    }
}

// Resource type callback procedure.
BOOL CResourceFinder::EnumTypesProc(HANDLE module, LPTSTR typeName, LONG_PTR lparam)
{
    CResourceFinder* pFinder = reinterpret_cast<CResourceFinder*>(lparam);
    CString type;

    pFinder->m_info.Empty();

    // The type may be a string or an number.
    if (IS_INTRESOURCE(typeName))
        type << reinterpret_cast<size_t>(typeName);
    else
        type << typeName;

    pFinder->m_info.typeName = type;

    // Find the names of all resources.
    EnumResourceNames(static_cast<HMODULE>(module), typeName,
                      reinterpret_cast<ENUMRESNAMEPROC>(EnumNamesProc),
                      lparam);

    return TRUE;
}

// Resource name callback procedure.
BOOL CResourceFinder::EnumNamesProc(HANDLE module, LPCTSTR typeName,
                                    LPCTSTR resourceName, LONG_PTR lparam)
{
    CResourceFinder* pFinder = reinterpret_cast<CResourceFinder*>(lparam);
    CString name;

    // The type may be a string or an number.
    if (IS_INTRESOURCE(resourceName))
    {
        pFinder->m_info.resourceID = reinterpret_cast<size_t>(resourceName);
        name << reinterpret_cast<size_t>(resourceName);
    }
    else
        name << resourceName;

    pFinder->m_info.resourceName = name;

    // Find the languages of all resources.
    EnumResourceLanguages(static_cast<HMODULE>(module), typeName, resourceName,
                          reinterpret_cast<ENUMRESLANGPROC>(EnumLangsProc), lparam);

    return TRUE;
}

// Resource language callback procedure.
BOOL CResourceFinder::EnumLangsProc(HANDLE module, LPCTSTR typeName, LPCTSTR resourceName,
    WORD languageID, LONG_PTR lparam)
{
    CResourceFinder* pFinder = reinterpret_cast<CResourceFinder*>(lparam);
    if (0 != FindResourceEx(static_cast<HMODULE>(module), typeName, resourceName, languageID))
    {
        pFinder->m_info.languageID = languageID;
        pFinder->m_allInfo.push_back(pFinder->m_info);
    }

    return TRUE;
}