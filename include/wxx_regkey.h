// Win32++   Version 8.0 Alpha
// Release Date: TBA
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2015  David Nash
//
// Permission is hereby granted, free of charge, to
// any person obtaining a copy of this software and
// associated documentation files (the "Software"),
// to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom
// the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice
// shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////


////////////////////////////////////////////////////////
// wxx_regkey.h
//  Declaration of the CRegKey class

#ifndef _WIN32XX_REGKEY_H_
#define _WIN32XX_REGKEY_H_

namespace Win32xx
{

	class CRegKey
	{
	public:
		CRegKey();
		CRegKey(HKEY hKey);
		~CRegKey();
		operator HKEY() const { return m_hKey; }
		CRegKey& operator =(CRegKey& key);

		void Attach(HKEY hKey);
		LONG Close();
		LONG Create(HKEY hKeyParent, LPCTSTR lpszKeyName, LPTSTR lpszClass = REG_NONE, DWORD dwOptions = REG_OPTION_NON_VOLATILE,
			           REGSAM samDesired = KEY_READ | KEY_WRITE, LPSECURITY_ATTRIBUTES lpSecAttr = NULL, LPDWORD lpdwDisposition = NULL);
		LONG DeleteSubKey(LPCTSTR lpszSubKey);
		LONG DeleteValue(LPCTSTR lpszSubKey);
		HKEY Detach();
		LONG EnumKey(DWORD iIndex, LPTSTR pszName, LPDWORD pnNameLength, FILETIME* pftLastWriteTime = NULL);
		LONG Flush();
		HKEY GetKey() { return m_hKey; }
		LONG NotifyChangeKeyValue(BOOL bWatchSubtree, DWORD dwNotifyFilter, HANDLE hEvent, BOOL bAsync = TRUE);
		LONG Open(HKEY hKeyParent, LPCTSTR lpszKeyName, REGSAM samDesired = KEY_READ | KEY_WRITE);
		LONG QueryBinaryValue(LPCTSTR pszValueName, void* pValue, ULONG* pnBytes);
		LONG QueryDWORDValue(LPCTSTR pszValueName, DWORD& dwValue);
		LONG QueryGUIDValue(LPCTSTR pszValueName, GUID& guidValue);
		LONG QueryMultiStringValue(LPCTSTR pszValueName, LPTSTR pszValue, ULONG* pnChars);
		LONG QueryStringValue(LPCTSTR pszValueName, LPTSTR pszValue, ULONG* pnChars);
		LONG QueryValue(LPCTSTR pszValueName, DWORD* pdwType, void* pData, ULONG* pnBytes);
		LONG RecurseDeleteKey(LPCTSTR lpszKey);
		LONG SetBinaryValue(LPCTSTR pszValueName, const void* pValue, ULONG nBytes);
		LONG SetDWORDValue(LPCTSTR pszValueName, DWORD dwValue);
		LONG SetGUIDValue(LPCTSTR pszValueName, REFGUID guidValue);
		LONG SetKeySecurity(SECURITY_INFORMATION si, PSECURITY_DESCRIPTOR psd);
		LONG SetMultiStringValue(LPCTSTR pszValueName, LPCTSTR pszValue);
		LONG SetStringValue(LPCTSTR pszValueName, LPCTSTR pszValue, DWORD dwType = REG_SZ);
		LONG SetValue(LPCTSTR pszValueName, DWORD dwType, const void* pValue, ULONG nBytes);

#ifdef REG_QWORD
		LONG QueryQWORDValue(LPCTSTR pszValueName, ULONGLONG& qwValue);
		LONG SetQWORDValue(LPCTSTR pszValueName, ULONGLONG qwValue);
#endif

#if (WINVER >= 0x0600)
		LONG SetKeyValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL);
#endif

	private:
		HKEY m_hKey;
	};

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace Win32xx
{
	inline CRegKey::CRegKey() : m_hKey(0)
	{
	}

	inline CRegKey::CRegKey(HKEY hKey) : m_hKey(0)
	{
		Attach(hKey);
	}

	inline CRegKey::~CRegKey()
	{
		Close();
	}

	inline CRegKey& CRegKey::operator =(CRegKey& key)
	{ 
		Close(); 
		Attach(key.m_hKey); 
		return *this;
	}

	inline void CRegKey::Attach(HKEY hKey)
	{
		assert(m_hKey == NULL);
		m_hKey = hKey;
	}

	inline LONG CRegKey::Close()
	{
		LONG lRes = ERROR_SUCCESS;

		if (m_hKey != NULL)
		{
			lRes = ::RegCloseKey(m_hKey);
			m_hKey = NULL;
		}

		return lRes;
	}

	inline LONG CRegKey::Create(HKEY hKeyParent, LPCTSTR lpszKeyName, LPTSTR lpszClass, DWORD dwOptions,
		           REGSAM samDesired, LPSECURITY_ATTRIBUTES lpSecAttr, LPDWORD lpdwDisposition)
	{
		HKEY hKey = 0;
		LONG lRes =  ::RegCreateKeyEx(hKeyParent, lpszKeyName, 0, lpszClass, dwOptions, samDesired, lpSecAttr, &hKey, lpdwDisposition);

		// RegCreateKeyEx opens existing keys, so close it now.
		::RegCloseKey(hKey);

		return lRes;
	}

	inline LONG CRegKey::DeleteSubKey(LPCTSTR lpszSubKey)
	{
		assert(m_hKey);
		return ::RegDeleteKey(m_hKey, lpszSubKey);
	}

	inline LONG CRegKey::DeleteValue(LPCTSTR lpszSubKey )
	{
		assert(m_hKey);
		return ::RegDeleteValue(m_hKey, lpszSubKey);
	}

	inline HKEY CRegKey::Detach()
	{
		assert(m_hKey);
		HKEY hKey = m_hKey;
		m_hKey = 0;
		return hKey;
	}

	inline LONG CRegKey::EnumKey(DWORD iIndex, LPTSTR pszName, LPDWORD pnNameLength, FILETIME* pftLastWriteTime)
	{
		assert(m_hKey);
		return ::RegEnumKeyEx(m_hKey, iIndex, pszName, pnNameLength, 0, 0, 0, pftLastWriteTime);
	}

	inline LONG CRegKey::Flush()
	{
		assert(m_hKey);
		return ::RegFlushKey(m_hKey);
	}

	inline LONG CRegKey::NotifyChangeKeyValue(BOOL bWatchSubtree, DWORD dwNotifyFilter, HANDLE hEvent, BOOL bAsync)
	{
		assert(m_hKey);
		return ::RegNotifyChangeKeyValue(m_hKey, bWatchSubtree, dwNotifyFilter, hEvent, bAsync);
	}

	inline LONG CRegKey::Open(HKEY hKeyParent, LPCTSTR lpszKeyName, REGSAM samDesired)
	{
		assert(hKeyParent);
		Close();
		return ::RegOpenKeyEx(hKeyParent, lpszKeyName, 0, samDesired, &m_hKey);
	}

	inline LONG CRegKey::QueryBinaryValue(LPCTSTR pszValueName, void* pValue, ULONG* pnBytes)
	{
		assert(m_hKey);
		DWORD dwType = REG_BINARY;
		return ::RegQueryValueEx(m_hKey, pszValueName, 0, &dwType, (LPBYTE)pValue, pnBytes);
	}

	inline LONG CRegKey::QueryDWORDValue(LPCTSTR pszValueName, DWORD& dwValue)
	{
		assert(m_hKey);
		DWORD dwType = REG_DWORD;
		DWORD nBytes = sizeof(DWORD);
		return ::RegQueryValueEx(m_hKey, pszValueName, 0, &dwType, (LPBYTE)&dwValue, &nBytes);
	}

	inline LONG CRegKey::QueryGUIDValue(LPCTSTR pszValueName, GUID& guidValue)
	{
		assert(m_hKey);

		TCHAR szGUID[64];
		guidValue = GUID_NULL;
		ULONG nCount = 64;
		LONG lRes = QueryStringValue(pszValueName, szGUID, &nCount);

		if (lRes == ERROR_SUCCESS)
		{
			HRESULT hr = ::CLSIDFromString(T2OLE(szGUID), &guidValue);

			if (FAILED(hr))
				lRes = ERROR_INVALID_DATA;
		}

		return lRes;
	}

	inline LONG CRegKey::QueryMultiStringValue(LPCTSTR pszValueName, LPTSTR pszValue, ULONG* pnChars)
	{
		assert(m_hKey);
		DWORD dwType = REG_MULTI_SZ;
		return ::RegQueryValueEx(m_hKey, pszValueName, 0, &dwType, (LPBYTE)pszValue, pnChars);
	}



	inline LONG CRegKey::QueryStringValue(LPCTSTR pszValueName, LPTSTR pszValue, ULONG* pnChars)
	{
		assert(m_hKey);
		DWORD dwType = REG_SZ;
		return ::RegQueryValueEx(m_hKey, pszValueName, 0, &dwType, (LPBYTE)pszValue, pnChars);
	}

	inline LONG CRegKey::QueryValue(LPCTSTR pszValueName, DWORD* pdwType, void* pData, ULONG* pnBytes)
	{
		assert(m_hKey);
		return ::RegQueryValueEx(m_hKey, pszValueName, 0, pdwType, (LPBYTE)pData, pnBytes);
	}

	inline LONG CRegKey::RecurseDeleteKey(LPCTSTR lpszKey)
	{
		assert(m_hKey);
		assert(lpszKey);

		CRegKey Key;
		LONG lRes = Key.Open(m_hKey, lpszKey, KEY_READ | KEY_WRITE);
		if (lRes != ERROR_SUCCESS)
			return lRes;

		FILETIME time;
		DWORD dwSize = MAX_PATH;
		TCHAR szSubKey[MAX_PATH];
		while (ERROR_SUCCESS == ::RegEnumKeyEx(Key, 0, szSubKey, &dwSize, NULL, NULL, NULL, &time))
		{
			lRes = Key.RecurseDeleteKey(szSubKey);
			if (lRes != ERROR_SUCCESS)
				return lRes;
			dwSize = MAX_PATH;
		}

		Key.Close();
		return DeleteSubKey(lpszKey);
	}

	inline LONG CRegKey::SetBinaryValue(LPCTSTR pszValueName, const void* pValue, ULONG nBytes)
	{
		assert(m_hKey);
		return ::RegSetValueEx(m_hKey, pszValueName, 0, REG_BINARY, (LPBYTE)pValue, nBytes);
	}

	inline LONG CRegKey::SetDWORDValue(LPCTSTR pszValueName, DWORD dwValue)
	{
		assert(m_hKey);
		return ::RegSetValueEx(m_hKey, pszValueName, 0, REG_DWORD, (LPBYTE)&dwValue, sizeof(DWORD));
	}

	inline LONG CRegKey::SetGUIDValue(LPCTSTR pszValueName, REFGUID guidValue)
	{
		assert(m_hKey);
		OLECHAR szGUID[64];
		::StringFromGUID2(guidValue, szGUID, 64);

		return SetStringValue(pszValueName, OLE2T(szGUID));
	}

#if (WINVER >= 0x0600)
	inline LONG CRegKey::SetKeyValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName)
	{
		assert(m_hKey);
		return ::RegSetKeyValue(m_hKey, lpszKeyName, lpszValueName, REG_SZ, lpszValue, lstrlen(lpszValue)*sizeof(TCHAR) );
	}
#endif

	inline LONG CRegKey::SetKeySecurity(SECURITY_INFORMATION si, PSECURITY_DESCRIPTOR psd)
	{
		assert(m_hKey);
		return ::RegSetKeySecurity(m_hKey, si, psd);
	}

	inline LONG CRegKey::SetMultiStringValue(LPCTSTR pszValueName, LPCTSTR pszValue)
	{
		assert(m_hKey);
		assert(pszValue);

		// pszValue contains one or more null terminated strings.
		// Calculate the length of all strings including the terminating characters.
		ULONG nBytes = 0;
		LPCTSTR pszTemp = pszValue;
		ULONG nLength = lstrlen(pszTemp) +1; // +1 to include the terminating character

		while (nLength != 1)
		{
			pszTemp += nLength;
			nBytes += nLength*sizeof(TCHAR);
			nLength = lstrlen(pszTemp) +1;
		}

		return ::RegSetValueEx(m_hKey, pszValueName, 0, REG_MULTI_SZ, (LPBYTE)pszValue, nBytes);
	}

	inline LONG CRegKey::SetStringValue(LPCTSTR pszValueName, LPCTSTR pszValue, DWORD dwType)
	{
		assert(m_hKey);
		return ::RegSetValueEx(m_hKey, pszValueName, 0, dwType, (LPBYTE)pszValue, lstrlen(pszValue)*sizeof(TCHAR));
	}

	inline LONG CRegKey::SetValue(LPCTSTR pszValueName, DWORD dwType, const void* pValue, ULONG nBytes)
	{
		assert(m_hKey);
		return ::RegSetValueEx(m_hKey, pszValueName, 0, dwType, (BYTE*)pValue, nBytes);
	}

#ifdef REG_QWORD

	inline LONG CRegKey::QueryQWORDValue(LPCTSTR pszValueName, ULONGLONG& qwValue)
	{
		assert(m_hKey);
		DWORD dwType = REG_QWORD;
		DWORD nBytes = sizeof(ULONGLONG);
		return ::RegQueryValueEx(m_hKey, pszValueName, 0, &dwType, (LPBYTE)&qwValue, &nBytes);
	}

	inline LONG CRegKey::SetQWORDValue(LPCTSTR pszValueName, ULONGLONG qwValue)
	{
		assert(m_hKey);
		return ::RegSetValueEx(m_hKey, pszValueName, 0, REG_QWORD, (LPBYTE)&qwValue, sizeof(ULONGLONG) );
	}

#endif

}


#endif // defined _WIN32XX_REGKEY_H_
