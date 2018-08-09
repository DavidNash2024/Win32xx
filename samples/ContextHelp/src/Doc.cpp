///////////////////////////////////////
// Doc.cpp

// Based on code provided by Lynn Allan


#include "stdafx.h"
#include "Doc.h"


// Definitions for the CDoc class
CDoc::CDoc() : m_checkA(FALSE), m_checkB(FALSE), m_checkC(FALSE), m_radio(0)
{
}

CDoc::~CDoc()
{
}

DWORD CDoc::GetRegDwordFromOpenKey(HKEY hKey, LPCTSTR pName)
{
  DWORD   type;
  DWORD   count = sizeof(DWORD);
  DWORD   value = 0;
  if (ERROR_SUCCESS == RegQueryValueEx(hKey, pName, NULL, &type, (LPBYTE)&value, &count))
      return value;
  else
      return 0;
}

void CDoc::LoadDocRegistry(LPCTSTR keyName)
{
    HKEY hKey;
    CString fullKeyName = _T("Software\\");
	fullKeyName += keyName;
	fullKeyName += _T("\\Document Settings");
    if (ERROR_SUCCESS ==RegOpenKeyEx(HKEY_CURRENT_USER, fullKeyName, 0,
        KEY_READ, &hKey))
    {
        m_checkA = GetRegDwordFromOpenKey(hKey, _T("CheckA")) & 1;
        m_checkB = GetRegDwordFromOpenKey(hKey, _T("CheckB")) & 1;
        m_checkC = GetRegDwordFromOpenKey(hKey, _T("CheckC")) & 1;
        m_radio = GetRegDwordFromOpenKey(hKey, _T("Radio"));

        RegCloseKey(hKey);
    }
}

void CDoc::SaveDocRegistry(LPCTSTR keyName)
{
    HKEY hKey;
    CString fullKeyName = _T("Software\\");
	fullKeyName += keyName;
	fullKeyName += _T("\\Document Settings");

    RegCreateKeyEx(HKEY_CURRENT_USER, fullKeyName, 0, NULL,
    REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);

    RegSetValueEx(hKey, _T("CheckA"), 0, REG_DWORD, (LPBYTE)&m_checkA, sizeof(BOOL));
    RegSetValueEx(hKey, _T("CheckB"), 0, REG_DWORD, (LPBYTE)&m_checkB, sizeof(BOOL));
    RegSetValueEx(hKey, _T("CheckC"), 0, REG_DWORD, (LPBYTE)&m_checkC, sizeof(BOOL));
    RegSetValueEx(hKey, _T("Radio"), 0, REG_DWORD, (LPBYTE)&m_radio, sizeof(BOOL));

    RegCloseKey(hKey);
}

