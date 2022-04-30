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
void CFormDoc::LoadSettings(LPCTSTR keyName)
{
    CString settingsKeyName;
    settingsKeyName << _T("Software\\") << keyName << _T("\\Document Settings");
    CRegKey key;
    if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, settingsKeyName, KEY_READ))
    {
        key.QueryBoolValue(_T("CheckA"), m_isCheckA);
        key.QueryBoolValue(_T("CheckB"), m_isCheckB);
        key.QueryBoolValue(_T("CheckC"), m_isCheckC);
        key.QueryDWORDValue(_T("Radio"), m_radio);
    }
}

// Saves the settings in the registry.
void CFormDoc::SaveSettings(LPCTSTR keyName)
{
    CString settingsKeyName;
    settingsKeyName << _T("Software\\") << keyName << _T("\\Document Settings");

    CRegKey key;
    if (ERROR_SUCCESS == key.Create(HKEY_CURRENT_USER, settingsKeyName))
    {
        if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, settingsKeyName))
        {
            key.SetBoolValue(_T("CheckA"), m_isCheckA);
            key.SetBoolValue(_T("CheckB"), m_isCheckB);
            key.SetBoolValue(_T("CheckC"), m_isCheckC);
            key.SetDWORDValue(_T("Radio"), m_radio);
        }
    }
}

