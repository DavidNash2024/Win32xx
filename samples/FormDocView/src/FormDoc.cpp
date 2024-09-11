///////////////////////////////////////
// FormDoc.cpp

// Based on code provided by Lynn Allan


#include "stdafx.h"
#include "FormDoc.h"
#include "resource.h"


////////////////////////////////
// CFormDoc function definitions
//

// Constructor.
CFormDoc::CFormDoc() : m_isCheckA(false), m_isCheckB(false), m_isCheckC(false), m_radio(ID_RADIO_A)
{
}

// Destructor.
CFormDoc::~CFormDoc()
{
}

// Loads the settings from the registry.
void CFormDoc::LoadSettings(LPCWSTR keyName)
{
    CString settingsKeyName;
    settingsKeyName << L"Software\\" << keyName << L"\\Document Settings";
    CRegKey key;
    if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, settingsKeyName, KEY_READ))
    {
        key.QueryBoolValue(L"CheckA", m_isCheckA);
        key.QueryBoolValue(L"CheckB", m_isCheckB);
        key.QueryBoolValue(L"CheckC", m_isCheckC);
        key.QueryDWORDValue(L"Radio", m_radio);
    }
}

// Saves the settings in the registry.
void CFormDoc::SaveSettings(LPCWSTR keyName)
{
    CString settingsKeyName;
    settingsKeyName << L"Software\\" << keyName << L"\\Document Settings";

    CRegKey key;
    if (ERROR_SUCCESS == key.Create(HKEY_CURRENT_USER, settingsKeyName))
    {
        if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, settingsKeyName))
        {
            key.SetBoolValue(L"CheckA", m_isCheckA);
            key.SetBoolValue(L"CheckB", m_isCheckB);
            key.SetBoolValue(L"CheckC", m_isCheckC);
            key.SetDWORDValue(L"Radio", m_radio);
        }
    }
}

