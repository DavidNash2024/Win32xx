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
		CString(LPCTSTR str);
		CString(TCHAR ch, int nLength = 1);

		CString&    operator = (const CString& str);
		BOOL        operator == (LPCTSTR str);
		BOOL        operator != (LPCTSTR str);
		BOOL		operator < (LPCTSTR str);
		BOOL		operator > (LPCTSTR str);
		BOOL		operator <= (LPCTSTR str);
		BOOL		operator >= (LPCTSTR str);
					operator LPCTSTR() const;
					operator BSTR() const;
		TCHAR&      operator [] (int nChar);
		CString&    operator += (const CString& str);
		CString		operator + (const CString& rhs);

		int         GetLength() const;
		void        Empty();
		BOOL        IsEmpty() const;

		int         Compare(LPCTSTR str) const;
		int         CompareNoCase(LPCTSTR str) const;
		int         Find(LPCTSTR str, int nStart = 0) const;
		int			FindOneOf(LPCTSTR str) const;
		int         ReverseFind(LPCTSTR str, int nStart = -1) const;

		int         Replace(TCHAR chOld, TCHAR chNew);
		int         Replace(const LPCTSTR strOld, LPCTSTR strNew);
		int         Remove(LPCTSTR str);

		int         Insert(int nIndex, TCHAR ch);
		int         Insert(int nIndex, const CString& str);
		int         Delete(int nIndex, int nCount = 1);
		void        Truncate(int nNewLength);

		CString     Mid(int nFirst, int nCount) const;
		CString     Left(int nCount) const;
		CString     Right(int nCount) const;

		void        MakeLower();
		void		MakeReverse();
		void        MakeUpper();

		CString&    Trim();
		CString&    TrimLeft();
		CString&    TrimRight();

		void        Format(const TCHAR *format,...);

		BSTR        AllocSysString() const;

	private:
		void        FormatArgList(const TCHAR *format, va_list args);
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

	inline CString::CString(LPCTSTR str)
	{
		m_str.assign(str);
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

	inline BOOL CString::operator == (LPCTSTR str)
	{
		return Compare(str) == 0;
	}

	inline BOOL CString::operator != (LPCTSTR str)
	{
        return Compare(str) != 0;
	}

	inline BOOL CString::operator < (LPCTSTR str)
	{
		return Compare(str) < 0;
	}
	inline BOOL CString::operator > (LPCTSTR str)
	{
		return Compare(str) > 0;
	}
	inline BOOL CString::operator <= (LPCTSTR str)
	{
		return Compare(str) <= 0;
	}
	inline BOOL CString::operator >= (LPCTSTR str)
	{
		return Compare(str) >= 0;
	}

	inline CString::operator LPCTSTR() const
	{
	//	return m_str.data();
		return m_str.c_str();
	}

	inline TCHAR& CString::operator [] (int nChar)
	{
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

	inline int CString::GetLength() const
	{
		return m_str.size();
	}

	inline void CString::Empty()
	{
		m_str.clear();
	}

	inline BOOL CString::IsEmpty() const
	{
		return m_str.empty();
	}

	inline int CString::Compare(LPCTSTR str) const
	{
		return m_str.compare(str);
	}

	inline int CString::CompareNoCase(LPCTSTR str) const
	{
		return _tcsicmp(m_str.data(), str);
	}

	inline int CString::Find(LPCTSTR str, int nPos /* = 0 */) const
	{
		return m_str.find(str, nPos);
	}

	inline int CString::FindOneOf(LPCTSTR str) const
	{
		return m_str.find_first_of(str);
	}

	inline int CString::ReverseFind(LPCTSTR str, int nPos /* = -1 */) const
	{
		return m_str.rfind(str, nPos);
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

	inline int CString::Replace(LPCTSTR strOld, LPCTSTR strNew)
	{
		int nCount = 0;
		size_t pos = 0;
		while ((pos = m_str.find(strOld, pos)) != std::string::npos)
		{
			m_str.replace(pos, lstrlen(strOld), strNew);
			pos += lstrlen(strNew);
			++nCount;
		}
		return nCount;
	}

	inline int CString::Remove(LPCTSTR str)
	{
		int nCount = 0;
		size_t pos = 0;
		while ((pos = m_str.find(str, pos)) != std::string::npos)
		{
			m_str.erase(pos, lstrlen(str));
			++nCount;
		}
		return nCount;
	}

	inline int CString::Insert(int nIndex, TCHAR ch)
	{
		m_str.insert(nIndex, &ch, 1);
		return m_str.size();
	}

	inline int CString::Insert(int nIndex, const CString& str)
	{
		m_str.insert(nIndex, str);
		return m_str.size();
	}

	inline int CString::Delete(int nIndex, int nCount /* = 1 */)
	{
		m_str.erase(nIndex, nCount);
		return m_str.size();
	}

	inline void CString::Truncate(int nNewLength)
	{
		m_str.erase(nNewLength);
	}

	inline CString CString::Mid(int nFirst, int nCount) const
	{
		CString str;
		str.m_str.assign((LPCTSTR)*this, nFirst, nFirst + nCount);
		return str;
	}

	inline CString CString::Left(int nCount) const
	{
		CString str;
		str.m_str.assign((LPCTSTR)*this, 0, nCount);
		return str;
	}

	inline CString CString::Right(int nCount) const
	{
		CString str;
		str.m_str.assign((LPCTSTR)*this, m_str.size() - nCount, nCount);
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

	inline CString& CString::Trim()
	{
		return TrimLeft().TrimRight();
	}

	inline CString& CString::TrimLeft()
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
		return *this;
	}

	inline CString& CString::TrimRight()
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
		return *this;
	}

	inline void CString::Format(const TCHAR *format,...)
	{
		va_list args;
		va_start(args, format);
		FormatArgList(format, args);
		va_end(args);
	}

	inline void CString::FormatArgList(const TCHAR *format, va_list args)
	{
		if (format)
		{
			int nResult = -1, nLength = 256;
			
			// A vector is used to store the TCHAR array
			std::vector<TCHAR> vBuffer;( nLength+1, _T('\0') );
			
			while (nResult == -1)
			{
				vBuffer.assign( nLength+1, _T('\0') );
				nResult = _vsntprintf(&vBuffer.front(), nLength, format, args);
				nLength *= 2;
			}
			m_str.assign(&vBuffer.front());
		}
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

}	// namespace Win32xx

#endif//__CSTRING_H__
