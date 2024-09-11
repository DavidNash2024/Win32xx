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
void CDoc::LoadSettings(LPCWSTR keyName)
{
    CString fullKeyName = "Software\\";
    fullKeyName += keyName;
    fullKeyName += "\\Document Settings";
    CRegKey key;
    if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, fullKeyName, KEY_READ))
    {
        key.QueryBoolValue(L"CheckA", m_isCheckA);
        key.QueryBoolValue(L"CheckB", m_isCheckB);
        key.QueryBoolValue(L"CheckC", m_isCheckC);
        key.QueryDWORDValue(L"Radio", m_radio);
    }
}

// Store values in the registry.
void CDoc::SaveSettings(LPCWSTR keyName)
{
    CString fullKeyName = "Software\\";
    fullKeyName += keyName;
    fullKeyName += "\\Document Settings";

    CRegKey key;
    if (ERROR_SUCCESS == key.Create(HKEY_CURRENT_USER, fullKeyName))
    {
        if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, fullKeyName))
        {
            key.SetBoolValue(L"CheckA", m_isCheckA);
            key.SetBoolValue(L"CheckB", m_isCheckB);
            key.SetBoolValue(L"CheckC", m_isCheckC);
            key.SetDWORDValue(L"Radio", m_radio);
        }
    }
}
