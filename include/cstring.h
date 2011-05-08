// Win32++   Pre-release Version 7.2
// Released: N/A
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2011  David Nash
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


// Acknowledgements:
// Thanks to Adam Szulc for his initial CString code.

////////////////////////////////////////////////////////
// cstring.h
//  Declaration of the cstring.h


#ifndef _WIN32XX_CSTRING_H_
#define _WIN32XX_CSTRING_H_


#include <string>
#include "wincore.h"


namespace Win32xx
{

	class CString 
	{
	public:
		CString();
		~CString();
		CString(const CString& str);
		CString(LPCTSTR pszText);
		CString(TCHAR ch, int nLength = 1);

		CString& operator = (const CString& str);
		BOOL     operator == (LPCTSTR pszText);
		BOOL     operator != (LPCTSTR pszText);
		BOOL	 operator < (LPCTSTR pszText);
		BOOL	 operator > (LPCTSTR pszText);
		BOOL	 operator <= (LPCTSTR pszText);
		BOOL	 operator >= (LPCTSTR pszText);
				 operator LPCTSTR() const;
				 operator BSTR() const;
		TCHAR&   operator [] (int nIndex);
		CString& operator += (const CString& str);
		CString	 operator + (const CString& rhs);

		// Attributes
		LPCTSTR	 c_str() const		{ return m_str.c_str(); }
		tString& GetString()		{ return m_str; }
		int      GetLength() const	{ return m_str.length(); }

		// Operations
		BSTR     AllocSysString() const;
		void	 AppendFormat(LPCTSTR pszFormat,...);
		void	 AppendFormat(UINT nFormatID, ...);
		int      Compare(LPCTSTR pszText) const;
		int      CompareNoCase(LPCTSTR pszText) const;
		int      Delete(int nIndex, int nCount = 1);
		int		 Find(TCHAR ch, int nIndex = 0 ) const;
		int      Find(LPCTSTR pszText, int nStart = 0) const;
		int		 FindOneOf(LPCTSTR pszText) const;
		void	 Format(UINT nID, ...);
		void     Format(LPCTSTR pszFormat,...);
		void     FormatV(LPCTSTR pszFormat, va_list args);
		void	 FormatMessage(LPCTSTR pszFormat,...);
		void	 FormatMessageV(LPCTSTR pszFormat, va_list args);
		TCHAR	 GetAt(int nIndex) const;
		void     Empty();
		int      Insert(int nIndex, TCHAR ch);
		int      Insert(int nIndex, const CString& str);
		BOOL     IsEmpty() const;
		CString  Left(int nCount) const;
		BOOL	 LoadString(UINT nID);
		void     MakeLower();
		void	 MakeReverse();
		void     MakeUpper();
		CString	 Mid(int nFirst) const;
		CString  Mid(int nFirst, int nCount) const;
		int      Replace(TCHAR chOld, TCHAR chNew);
		int      Replace(const LPCTSTR pszOld, LPCTSTR pszNew);
		int      Remove(LPCTSTR pszText);
		int      ReverseFind(LPCTSTR pszText, int nStart = -1) const;
		CString  Right(int nCount) const;
		void	 SetAt(int nIndex, TCHAR ch);
		BSTR	 SetSysString(BSTR* pBstr) const;
		CString	 SpanExcluding(LPCTSTR pszText) const;
		CString	 SpanIncluding(LPCTSTR pszText) const;
		CString	 Tokenize(LPCTSTR pszTokens, int& iStart) const;
		void	 Trim();
		void	 TrimLeft();
		void	 TrimLeft(TCHAR chTarget);
		void	 TrimLeft(LPCTSTR pszTargets);
		void	 TrimRight();
		void	 TrimRight(TCHAR chTarget);
		void	 TrimRight(LPCTSTR pszTargets);
		void     Truncate(int nNewLength);

#ifndef _WIN32_WCE
		int      Collate(LPCTSTR pszText) const;
		int		 CollateNoCase(LPCTSTR pszText) const;
		BOOL	 GetEnvironmentVariable(LPCTSTR pszVar);
#endif

	private:
		tString m_str;
	};

	inline CString::CString()
	{
	}

	inline CString::~CString()
	{
	}

	inline CString::CString(const CString& str)
	{
		m_str.assign(str);
	}

	inline CString::CString(LPCTSTR pszText)
	{
		m_str.assign(pszText);
	}

	inline CString::CString(TCHAR ch, int nLength)
	{
		m_str.assign(nLength, ch);
	}

	inline CString& CString::operator = (const CString& str)
	{
		m_str.assign(str);
		return *this;
	}

	inline BOOL CString::operator == (LPCTSTR pszText)
	{
		assert(pszText);
		return (0 == Compare(pszText));
	}

	inline BOOL CString::operator != (LPCTSTR pszText)
	{
		assert(pszText);
        return Compare(pszText) != 0;
	}

	inline BOOL CString::operator < (LPCTSTR pszText)
	{
		assert(pszText);
		return Compare(pszText) < 0;
	}

	inline BOOL CString::operator > (LPCTSTR pszText)
	{
		assert(pszText);
		return Compare(pszText) > 0;
	}

	inline BOOL CString::operator <= (LPCTSTR pszText)
	{
		assert(pszText);
		return Compare(pszText) <= 0;
	}

	inline BOOL CString::operator >= (LPCTSTR pszText)
	{
		assert(pszText);
		return Compare(pszText) >= 0;
	}

	inline CString::operator LPCTSTR() const
	{
		return m_str.c_str();
	}

	inline TCHAR& CString::operator [] (int nIndex)
	{
		assert(nIndex >= 0);
		assert(nIndex < GetLength());
		return m_str[nIndex];
	}

	inline CString& CString::operator += (const CString& str)
	{
		m_str.append(str);
		return *this;
	}

	inline CString CString::operator + (const CString& rhs)
	{
		CString str(*this);
		str.m_str.append(rhs.m_str);
		return str;
	}

	inline BSTR CString::AllocSysString() const
	{
		return ::SysAllocStringLen(T2W(m_str.c_str()), m_str.size());
	}

	inline void CString::AppendFormat(LPCTSTR pszFormat,...)
	{
		CString str;
		str.Format(pszFormat);
		m_str.append(str);
	}

	inline void CString::AppendFormat(UINT nFormatID, ...)
	{
		CString str1;
		CString str2;
		if (str1.LoadString(nFormatID))
		{
			str2.Format(str1);
			m_str.append(str2);
		}
	}

#ifndef _WIN32_WCE
	inline int CString::Collate(LPCTSTR pszText) const
	{
		assert(pszText);
		return _tcscoll(m_str.c_str(), pszText);
	}

	inline int CString::CollateNoCase(LPCTSTR pszText) const
	{
		assert(pszText);
		return _tcsicoll(m_str.c_str(), pszText);
	}
#endif	// _WIN32_WCE

	inline int CString::Compare(LPCTSTR pszText) const
	{
		assert(pszText);
		return m_str.compare(pszText);
	}

	inline int CString::CompareNoCase(LPCTSTR pszText) const
	{
		assert(pszText);
		return _tcsicmp(m_str.data(), pszText);
	}

	inline int CString::Delete(int nIndex, int nCount /* = 1 */)
	{
		assert(nIndex >= 0);
		assert(nCount >= 0);

		m_str.erase(nIndex, nCount);
		return m_str.size();
	}

	inline void CString::Empty()
	{
		m_str.erase();
	}

	inline int CString::Find(TCHAR ch, int nIndex /* = 0 */) const
	{
		assert(nIndex >= 0);
		return m_str.find(ch);
	}

	inline int CString::Find(LPCTSTR pszText, int nIndex /* = 0 */) const
	{
		assert(pszText);
		assert(nIndex >= 0);
		return m_str.find(pszText, nIndex);
	}

	inline int CString::FindOneOf(LPCTSTR pszText) const
	{
		assert(pszText);
		return m_str.find_first_of(pszText);
	}

	inline void CString::Format(LPCTSTR pszFormat,...)
	{
		va_list args;
		va_start(args, pszFormat);
		FormatV(pszFormat, args);
		va_end(args);
	}

	inline void CString::Format(UINT nID, ...)
	{
		Empty();
		CString str;
		if (str.LoadString(nID))
			Format(str);
	}

	inline void CString::FormatV(LPCTSTR pszFormat, va_list args)
	{
		if (pszFormat)
		{
			int nResult = -1, nLength = 256;

			// A vector is used to store the TCHAR array
			std::vector<TCHAR> vBuffer;( nLength+1, _T('\0') );

			while (-1 == nResult)
			{
				vBuffer.assign( nLength+1, _T('\0') );
				nResult = _vsntprintf(&vBuffer[0], nLength, pszFormat, args);
				nLength *= 2;
			}
			m_str.assign(&vBuffer[0]);
		}
	}

	inline void CString::FormatMessage(LPCTSTR pszFormat,...)
	{
		va_list args;
		va_start(args, pszFormat);
		FormatMessageV(pszFormat, args);
		va_end(args);
	}

	inline void CString::FormatMessageV(LPCTSTR pszFormat, va_list args)
	{
		LPTSTR pszTemp;
		if (pszFormat)
		{
			DWORD dwResult = ::FormatMessage(FORMAT_MESSAGE_FROM_STRING|FORMAT_MESSAGE_ALLOCATE_BUFFER, pszFormat, 0, 0, pszTemp, 0, &args);

			if (0 == dwResult || 0 == pszTemp )
				throw std::bad_alloc();

			m_str = pszTemp;
			LocalFree(pszTemp);
		}
	}

	inline TCHAR CString::GetAt(int nIndex) const
	{
		assert(nIndex >= 0);
		assert(nIndex < GetLength());
		return m_str[nIndex];
	}

#ifndef _WIN32_WCE
	inline BOOL CString::GetEnvironmentVariable(LPCTSTR pszVar)
	{
		assert(pszVar);
		Empty();

		int nLength = ::GetEnvironmentVariable(pszVar, NULL, 0);
		if (nLength > 0)
		{
			std::vector<TCHAR> vBuffer( nLength+1, _T('\0') );
			::GetEnvironmentVariable(pszVar, &vBuffer[0], nLength);
			m_str = &vBuffer[0];
		}

		return (BOOL)nLength;
	}
#endif // _WIN32_WCE

	inline int CString::Insert(int nIndex, TCHAR ch)
	{
		assert(nIndex >= 0);
		assert(ch);

		m_str.insert(nIndex, &ch, 1);
		return m_str.size();
	}

	inline int CString::Insert(int nIndex, const CString& str)
	{
		assert(nIndex >= 0);

		m_str.insert(nIndex, str);
		return m_str.size();
	}

	inline BOOL CString::IsEmpty() const
	{
		return m_str.empty();
	}

	inline CString CString::Left(int nCount) const
	{
		assert(nCount >= 0);

		CString str;
		str.m_str.assign(c_str(), 0, nCount);
		return str;
	}

	inline BOOL CString::LoadString(UINT nID)
	{
		assert (GetApp());

		int nSize = 64;
		TCHAR* pTCharArray = 0;
		std::vector<TCHAR> vString;
		int nTChars = nSize;

		Empty();

		// Increase the size of our array in a loop until we load the entire string
		// The ANSI and _UNICODE versions of LoadString behave differently. This technique works for both.
		while ( nSize-1 <= nTChars )
		{
			nSize = nSize * 4;
			vString.assign(nSize+1, _T('\0'));
			pTCharArray = &vString[0];
			nTChars = ::LoadString (GetApp()->GetResourceHandle(), nID, pTCharArray, nSize);
		}

		if (nTChars > 0)
			m_str.assign(pTCharArray);

		return (nTChars != 0);
	}

	inline void CString::MakeLower()
	{
		std::transform(m_str.begin(), m_str.end(), m_str.begin(), &::tolower);
	}

	inline void CString::MakeReverse()
	{
		std::reverse(m_str.begin(), m_str.end());
	}

	inline void CString::MakeUpper()
	{
		std::transform(m_str.begin(), m_str.end(), m_str.begin(), &::toupper);
	}

	inline CString CString::Mid(int nFirst) const
	{
		return Mid(nFirst, GetLength());
	}

	inline CString CString::Mid(int nFirst, int nCount) const
	{
		assert(nFirst >= 0);
		assert(nCount >= 0);

		CString str;
		str.m_str.assign(c_str(), nFirst, nFirst + nCount);
		return str;
	}

	inline int CString::ReverseFind(LPCTSTR pszText, int nIndex /* = -1 */) const
	{
		assert(pszText);
		return m_str.rfind(pszText, nIndex);
	}

	inline void CString::SetAt(int nIndex, TCHAR ch)
	{
		assert(nIndex >= 0);
		assert(nIndex < GetLength());
		m_str[nIndex] = ch;
	}

	inline CString CString::SpanExcluding(LPCTSTR pszText) const
	{
		assert (pszText);

		CString str;
		size_t pos = 0;

		while ((pos = m_str.find_first_not_of(pszText, pos)) != std::string::npos)
		{
			str.m_str.append(1, m_str[pos++]);
		}

		return str;
	}

	inline CString CString::SpanIncluding(LPCTSTR pszText) const
	{
		assert (pszText);

		CString str;
		size_t pos = 0;

		while ((pos = m_str.find_first_of(pszText, pos)) != std::string::npos)
		{
			str.m_str.append(1, m_str[pos++]);
		}

		return str;
	}

	inline int CString::Remove(LPCTSTR pszText)
	{
		assert(pszText);

		int nCount = 0;
		size_t pos = 0;
		while ((pos = m_str.find(pszText, pos)) != std::string::npos)
		{
			m_str.erase(pos, lstrlen(pszText));
			++nCount;
		}
		return nCount;
	}

	inline int CString::Replace(TCHAR chOld, TCHAR chNew)
	{
		int nCount = 0;
		tString::iterator it = m_str.begin();
		while (it != m_str.end())
		{
			if (*it == chOld)
			{
				*it = chNew;
				++nCount;
			}
			++it;
		}
		return nCount;
	}

	inline int CString::Replace(LPCTSTR pszOld, LPCTSTR pszNew)
	{
		assert(pszOld);
		assert(pszNew);

		int nCount = 0;
		size_t pos = 0;
		while ((pos = m_str.find(pszOld, pos)) != std::string::npos)
		{
			m_str.replace(pos, lstrlen(pszOld), pszNew);
			pos += lstrlen(pszNew);
			++nCount;
		}
		return nCount;
	}

	inline CString CString::Right(int nCount) const
	{
		assert(nCount >= 0);

		CString str;
		str.m_str.assign(c_str(), m_str.size() - nCount, nCount);
		return str;
	}

	inline BSTR CString::SetSysString(BSTR* pBstr) const
	{
		assert(pBstr);

		if ( !::SysReAllocStringLen(pBstr, T2W(m_str.c_str()), m_str.length()) )
			throw std::bad_alloc();

		return *pBstr;
	}

	inline CString CString::Tokenize(LPCTSTR pszTokens, int& iStart) const
	{
		assert(pszTokens);
		assert(iStart >= 0);

		CString str;
		size_t pos1 = m_str.find_first_not_of(pszTokens, iStart);
		size_t pos2 = m_str.find_first_of(pszTokens, pos1);

		iStart = pos2 + 1;
		if (pos2 == m_str.npos)
			iStart = -1;

		if (pos1 != m_str.npos)
			str.m_str = m_str.substr(pos1, pos2-pos1);

		return str;
	}

	inline void CString::Trim()
	{
		TrimLeft();
		TrimRight();
	}

	inline void CString::TrimLeft()
	{
		// This method is supported by the Borland 5.5 compiler
		tString::iterator iter;
		for (iter = m_str.begin(); iter < m_str.end(); ++iter)
		{
			if (!isspace(*iter))
				break;
		}

		m_str.erase(m_str.begin(), iter);
	}

	inline void CString::TrimLeft(TCHAR chTarget)
	{
		m_str.erase(0, m_str.find_first_not_of(chTarget));
	}

	inline void CString::TrimLeft(LPCTSTR pszTargets)
	{
		assert(pszTargets);
		m_str.erase(0, m_str.find_first_not_of(pszTargets));
	}

	inline void CString::TrimRight()
	{
		// This method is supported by the Borland 5.5 compiler
		tString::reverse_iterator riter;
		for (riter = m_str.rbegin(); riter < m_str.rend(); ++riter)
		{
			if (!isspace(*riter))
				break;
		}

		m_str.erase(riter.base(), m_str.end());
	}

	inline void CString::TrimRight(TCHAR chTarget)
	{
		size_t pos = m_str.find_last_not_of(chTarget);
		if (pos != std::string::npos)
			m_str.erase(++pos);
	}

	inline void CString::TrimRight(LPCTSTR pszTargets)
	{
		assert(pszTargets);

		size_t pos = m_str.find_last_not_of(pszTargets);
		if (pos != std::string::npos)
			m_str.erase(++pos);
	}

	inline void CString::Truncate(int nNewLength)
	{
		assert(nNewLength >= 0);
		m_str.erase(nNewLength);
	}



	///////////////////////////////////
	// Global Functions

	inline CString LoadString(UINT nID)
	{
		CString str;
		str.LoadString(nID);
		return str;
	}


}	// namespace Win32xx

#endif//_WIN32XX_CSTRING_H_
