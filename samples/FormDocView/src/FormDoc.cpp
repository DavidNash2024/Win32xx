///////////////////////////////////////
// FormDoc.cpp

// Based on code provided by Lynn Allan


#include "pch.h"
#include "FormDoc.h"
#include "resource.h"


////////////////////////////////
// CFormDoc function definitions
//

// Constructor.
CFormDoc::CFormDoc() : m_isCheckA(false), m_isCheckB(false), m_isCheckC(false), m_radio(ID_RADIO_A)
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

        DWORD dwRadio = 0;
        if (ERROR_SUCCESS == key.QueryDWORDValue(L"Radio", dwRadio))
            m_radio = static_cast<UINT>(dwRadio);
    }
}

// Saves the settings in the registry.
void CFormDoc::SaveSettings(LPCWSTR keyName)
{
    CString settingsKeyName;
    settingsKeyName << L"Software\\" << keyName << L"\\Document Settings";

    CRegKey key;
    if (ERROR_SUCCESS == key.Create(HKEY_CURRENT_USER, settingsKeyName, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE))
    {
        key.SetBoolValue(L"CheckA", GetCheckA());
        key.SetBoolValue(L"CheckB", GetCheckB());
        key.SetBoolValue(L"CheckC", GetCheckC());
        key.SetDWORDValue(L"Radio", static_cast<DWORD>(GetRadio()));
    }
}

