///////////////////////////////////////
// FormDoc.cpp

// Based on code provided by Lynn Allan


#include "stdafx.h"
#include "FormDoc.h"


////////////////////////////////
// CFormDoc function definitions
//

// Constructor.
CFormDoc::CFormDoc() : m_isCheckA(0), m_isCheckB(0), m_isCheckC(0), m_radio(0)
{
}

// Destructor.
CFormDoc::~CFormDoc()
{
}

// Loads the settings from the registry.
void CFormDoc::LoadSettings(LPCTSTR keyName)
{
    CString fullKeyName = _T("Software\\");
    fullKeyName += keyName;
    fullKeyName += _T("\\Document Settings");
    CRegKey key;
    if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, fullKeyName, KEY_READ))
    {
        key.QueryDWORDValue(_T("CheckA"), m_isCheckA);
        key.QueryDWORDValue(_T("CheckB"), m_isCheckB);
        key.QueryDWORDValue(_T("CheckC"), m_isCheckC);
        key.QueryDWORDValue(_T("Radio"), m_radio);
    }
}

// Saves the settings in the registry.
void CFormDoc::SaveSettings(LPCTSTR keyName)
{
    CString fullKeyName = _T("Software\\");
    fullKeyName += keyName;
    fullKeyName += _T("\\Document Settings");

    CRegKey key;
    if (ERROR_SUCCESS == key.Create(HKEY_CURRENT_USER, fullKeyName))
    {
        if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, fullKeyName))
        {
            key.SetDWORDValue(_T("CheckA"), m_isCheckA);
            key.SetDWORDValue(_T("CheckB"), m_isCheckB);
            key.SetDWORDValue(_T("CheckC"), m_isCheckC);
            key.SetDWORDValue(_T("Radio"), m_radio);
        }
    }
}

