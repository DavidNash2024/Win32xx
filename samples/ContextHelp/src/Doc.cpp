///////////////////////////////////////
// Doc.cpp

// Based on code provided by Lynn Allan


#include "stdafx.h"
#include "Doc.h"


// Definitions for the CDoc class
CDoc::CDoc() : m_CheckA(FALSE), m_CheckB(FALSE), m_CheckC(FALSE), m_Radio(0)
{
}

CDoc::~CDoc()
{
}

DWORD CDoc::GetRegDwordFromOpenKey(HKEY hKey, LPCTSTR pName)
{
  DWORD   dwType;
  DWORD   dwCount = sizeof(DWORD);
  DWORD   dwValue = 0;
  if (ERROR_SUCCESS == RegQueryValueEx(hKey, pName, NULL, &dwType, (LPBYTE)&dwValue, &dwCount))
      return dwValue;
  else
      return 0;
}

void CDoc::LoadDocRegistry(LPCTSTR szKeyName)
{
    HKEY hKey;
    CString strKey = _T("Software\\");
    strKey += szKeyName;
    strKey += _T("\\Document Settings");
    if (ERROR_SUCCESS ==RegOpenKeyEx(HKEY_CURRENT_USER, strKey, 0,
        KEY_READ, &hKey))
    {
        m_CheckA = GetRegDwordFromOpenKey(hKey, _T("CheckA")) & 1;
        m_CheckB = GetRegDwordFromOpenKey(hKey, _T("CheckB")) & 1;
        m_CheckC = GetRegDwordFromOpenKey(hKey, _T("CheckC")) & 1;
        m_Radio = GetRegDwordFromOpenKey(hKey, _T("Radio"));

        RegCloseKey(hKey);
    }
}

void CDoc::SaveDocRegistry(LPCTSTR szKeyName)
{
    HKEY hKey;
    CString strKey = _T("Software\\");
    strKey += szKeyName;
    strKey += _T("\\Document Settings");

    RegCreateKeyEx(HKEY_CURRENT_USER, strKey, 0, NULL,
    REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);

    RegSetValueEx(hKey, _T("CheckA"), 0, REG_DWORD, (LPBYTE)&m_CheckA, sizeof(BOOL));
    RegSetValueEx(hKey, _T("CheckB"), 0, REG_DWORD, (LPBYTE)&m_CheckB, sizeof(BOOL));
    RegSetValueEx(hKey, _T("CheckC"), 0, REG_DWORD, (LPBYTE)&m_CheckC, sizeof(BOOL));
    RegSetValueEx(hKey, _T("Radio"), 0, REG_DWORD, (LPBYTE)&m_Radio, sizeof(BOOL));

    RegCloseKey(hKey);
}

