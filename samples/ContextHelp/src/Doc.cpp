/////////////////////////////
// Doc.cpp
//

#include "stdafx.h"
#include "Doc.h"
#include "resource.h"

////////////////////////////
// CDoc function definitions
//

// Constructor.
CDoc::CDoc() : m_isCheckA(false), m_isCheckB(false), m_isCheckC(false), m_radio(ID_RADIO_A)
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
        key.QueryBoolValue(_T("CheckA"), m_isCheckA);
        key.QueryBoolValue(_T("CheckB"), m_isCheckB);
        key.QueryBoolValue(_T("CheckC"), m_isCheckC);
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
            key.SetBoolValue(_T("CheckA"), m_isCheckA);
            key.SetBoolValue(_T("CheckB"), m_isCheckB);
            key.SetBoolValue(_T("CheckC"), m_isCheckC);
            key.SetDWORDValue(_T("Radio"), m_radio);
        }
    }
}
