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


// This software is based on code kindly provided by Adam Szulc.


#ifndef __CSTRING_H__
#define __CSTRING_H__


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

		CString&    operator = (const CString& str);
		BOOL        operator == (LPCTSTR pszText);
		BOOL        operator != (LPCTSTR pszText);
		BOOL		operator < (LPCTSTR pszText);
		BOOL		operator > (LPCTSTR pszText);
		BOOL		operator <= (LPCTSTR pszText);
		BOOL		operator >= (LPCTSTR pszText);
					operator LPCTSTR() const;
					operator BSTR() const;
		TCHAR&      operator [] (int nChar);
		CString&    operator += (const CString& str);
		CString		operator + (const CString& rhs);

		std::basic_string<TCHAR>& GetStdString() { return m_str; }	
		int         GetLength() const;

		BSTR        AllocSysString() const;
		int         Collate(LPCTSTR pszText) const;
		int			CollateNoCase(LPCTSTR pszText) const;
		int         Compare(LPCTSTR pszText) const;
		int         CompareNoCase(LPCTSTR pszText) const;
		int         Delete(int nIndex, int nCount = 1);
		int         Find(LPCTSTR pszText, int nStart = 0) const;
		int			FindOneOf(LPCTSTR pszText) const;
		void        Format(LPCTSTR pszFormat,...);
		void        FormatV(LPCTSTR pszFormat, va_list args);
		void        Empty();
		int         Insert(int nIndex, TCHAR ch);
		int         Insert(int nIndex, const CString& str);
		BOOL        IsEmpty() const;
		CString     Left(int nCount) const;	
		void        MakeLower();
		void		MakeReverse();
		void        MakeUpper();
		CString     Mid(int nFirst, int nCount) const;
		int         Replace(TCHAR chOld, TCHAR chNew);
		int         Replace(const LPCTSTR pszOld, LPCTSTR pszNew);
		int         Remove(LPCTSTR pszText);
		int         ReverseFind(LPCTSTR pszText, int nStart = -1) const;
		CString     Right(int nCount) const;
		CString		SpanExcluding(LPCTSTR pszText) const;
		CString		SpanIncluding(LPCTSTR pszText) const;
		CString		Tokenize(LPCTSTR pszTokens, int& iStart) const;
		void		Trim();
		void		TrimLeft();
		void		TrimRight();
		void        Truncate(int nNewLength);

	private:
		std::basic_string<TCHAR> m_str;
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
		return Compare(pszText) == 0;
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
	//	return m_str.data();
		return m_str.c_str();
	}

	inline TCHAR& CString::operator [] (int nChar)
	{
		assert(nChar >= 0);
		return m_str.at(nChar);
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
	#ifdef _UNICODE
		return SysAllocStringLen(m_str.data(), m_str.size());
	#else
		int nLen = MultiByteToWideChar(CP_ACP, 0, m_str.data(), m_str.size(), NULL, NULL );
		BSTR bstr = ::SysAllocStringLen(NULL, nLen);
		if (bstr != NULL)
		 ::MultiByteToWideChar(CP_ACP, 0, m_str.data(), m_str.size(), bstr, nLen);
		return bstr;
	#endif//_UNICODE
	}

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
		m_str.clear();
	}

	inline int CString::Find(LPCTSTR pszText, int nPos /* = 0 */) const
	{
		assert(pszText);
		assert(nPos >= 0);
		return m_str.find(pszText, nPos);
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

	inline void CString::FormatV(LPCTSTR pszFormat, va_list args)
	{
		if (pszFormat)
		{
			int nResult = -1, nLength = 256;
			
			// A vector is used to store the TCHAR array
			std::vector<TCHAR> vBuffer;( nLength+1, _T('\0') );
			
			while (nResult == -1)
			{
				vBuffer.assign( nLength+1, _T('\0') );
				nResult = _vsntprintf(&vBuffer.front(), nLength, pszFormat, args);
				nLength *= 2;
			}
			m_str.assign(&vBuffer.front());
		}
	}

	inline int CString::GetLength() const
	{
		return m_str.size();
	}

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
		str.m_str.assign((LPCTSTR)*this, 0, nCount);
		return str;
	}

	inline void CString::MakeLower()
	{
	//	std::transform(m_str.begin(), m_str.end(), m_str.begin(), std::tolower);
		std::transform(m_str.begin(), m_str.end(), m_str.begin(), &::tolower);
	}

	inline void CString::MakeReverse()
	{
		CString str(*this);
		m_str.assign(str.m_str.rbegin(), str.m_str.rend());
	}

	inline void CString::MakeUpper()
	{
	//	std::transform(m_str.begin(), m_str.end(), m_str.begin(), std::toupper);
		std::transform(m_str.begin(), m_str.end(), m_str.begin(), &::toupper);
	}

	inline CString CString::Mid(int nFirst, int nCount) const
	{
		assert(nFirst >= 0);
		assert(nCount >= 0);

		CString str;
		str.m_str.assign((LPCTSTR)*this, nFirst, nFirst + nCount);
		return str;
	}

	inline int CString::ReverseFind(LPCTSTR pszText, int nPos /* = -1 */) const
	{
		assert(pszText);
		return m_str.rfind(pszText, nPos);
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
		std::basic_string<TCHAR>::iterator it = m_str.begin();
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
		str.m_str.assign((LPCTSTR)*this, m_str.size() - nCount, nCount);
		return str;
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
	//   m_str.erase(m_str.begin(), std::find_if(m_str.begin(), m_str.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));

		// This method is supported by the Borland 5.5 compiler

		std::basic_string<TCHAR>::iterator iter;
		for (iter = m_str.begin(); iter < m_str.end(); ++iter)
		{
			if (!::isspace(*iter))
				break;
		}

		m_str.erase(m_str.begin(), iter);
	}

	inline void CString::TrimRight()
	{
	//   m_str.erase(std::find_if(m_str.rbegin(),m_str. rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), m_str.end());

		// This method is supported by the Borland 5.5 compiler

		std::basic_string<TCHAR>::reverse_iterator riter;
		for (riter = m_str.rbegin(); riter < m_str.rend(); ++riter)
		{
			if (!::isspace(*riter))
				break;
		}

		m_str.erase(riter.base(), m_str.end());
	}

	inline void CString::Truncate(int nNewLength)
	{
		assert(nNewLength >= 0);

		m_str.erase(nNewLength);
	}

}	// namespace Win32xx

#endif//__CSTRING_H__
