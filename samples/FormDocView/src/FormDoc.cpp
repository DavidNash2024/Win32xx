///////////////////////////////////////
// FormDoc.cpp

// Based on code provided by Lynn Allan


#include "stdafx.h"
#include "FormDoc.h"


// Definitions for the CFormDoc class
CFormDoc::CFormDoc() : m_isCheckA(FALSE), m_isCheckB(FALSE), m_isCheckC(FALSE), m_radio(0)
{
}

CFormDoc::~CFormDoc()
{
}

DWORD CFormDoc::GetRegDwordFromOpenKey(HKEY hKey, LPCTSTR keyName)
{
  DWORD   dwType;
  DWORD   dwCount = sizeof(DWORD);
  DWORD   dwValue = 0;
  if (ERROR_SUCCESS == RegQueryValueEx(hKey, keyName, NULL, &dwType, reinterpret_cast<LPBYTE>(&dwValue), &dwCount))
      return dwValue;
  else
      return 0;
}

void CFormDoc::LoadDocRegistry(LPCTSTR keyName)
{
    HKEY hKey;
    CString strKey = _T("Software\\");
    strKey += keyName;
    strKey += _T("\\Document Settings");
    if (ERROR_SUCCESS ==RegOpenKeyEx(HKEY_CURRENT_USER, strKey, 0,
        KEY_READ, &hKey))
    {
        m_isCheckA = GetRegDwordFromOpenKey(hKey, _T("CheckA")) & 1;
        m_isCheckB = GetRegDwordFromOpenKey(hKey, _T("CheckB")) & 1;
        m_isCheckC = GetRegDwordFromOpenKey(hKey, _T("CheckC")) & 1;
        m_radio = GetRegDwordFromOpenKey(hKey, _T("Radio"));

        RegCloseKey(hKey);
    }
}

void CFormDoc::SaveDocRegistry(LPCTSTR keyName)
{
    HKEY hKey;
    CString fullKeyName = _T("Software\\");
	fullKeyName += keyName;
	fullKeyName += _T("\\Document Settings");

    RegCreateKeyEx(HKEY_CURRENT_USER, fullKeyName, 0, NULL,
    REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);

    RegSetValueEx(hKey, _T("CheckA"), 0, REG_DWORD, (LPBYTE)&m_isCheckA, sizeof(BOOL));
    RegSetValueEx(hKey, _T("CheckB"), 0, REG_DWORD, (LPBYTE)&m_isCheckB, sizeof(BOOL));
    RegSetValueEx(hKey, _T("CheckC"), 0, REG_DWORD, (LPBYTE)&m_isCheckC, sizeof(BOOL));
    RegSetValueEx(hKey, _T("Radio"), 0, REG_DWORD, (LPBYTE)&m_radio, sizeof(BOOL));

    RegCloseKey(hKey);
}

