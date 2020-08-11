/////////////////////////////
// Doc.cpp
//

#include "stdafx.h"
#include "Doc.h"

////////////////////////////
// CDoc function definitions
//

// Constructor.
CDoc::CDoc() : m_checkA(0), m_checkB(0), m_checkC(0), m_radio(0)
{
}

// Destructor.
CDoc::~CDoc()
{
}

// Load values from the registry.
void CDoc::LoadSettings(LPCTSTR keyName)
{
    CString fullKeyName = _T("Software\\");
    fullKeyName += keyName;
    fullKeyName += _T("\\Document Settings");
    CRegKey key;
    if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, fullKeyName, KEY_READ))
    {
        key.QueryDWORDValue(_T("CheckA"), m_checkA);
        key.QueryDWORDValue(_T("CheckB"), m_checkB);
        key.QueryDWORDValue(_T("CheckC"), m_checkC);
        key.QueryDWORDValue(_T("Radio"), m_radio);
    }
}

// Store values in the registry.
void CDoc::SaveSettings(LPCTSTR keyName)
{
    CString fullKeyName = _T("Software\\");
    fullKeyName += keyName;
    fullKeyName += _T("\\Document Settings");

    CRegKey key;
    if (ERROR_SUCCESS == key.Create(HKEY_CURRENT_USER, fullKeyName))
    {
        if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, fullKeyName))
        {
            key.SetDWORDValue(_T("CheckA"), m_checkA);
            key.SetDWORDValue(_T("CheckB"), m_checkB);
            key.SetDWORDValue(_T("CheckC"), m_checkC);
            key.SetDWORDValue(_T("Radio"), m_radio);
        }
    }
}
