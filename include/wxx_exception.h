// Win32++   Version 8.1 beta
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



#ifndef _WIN32XX_EXCEPTION_H_
#define _WIN32XX_EXCEPTION_H_


namespace Win32xx
{
	////////////////////////////////////////
	// Declaration of the CException class
	//
	class CException : public std::exception
	{
	public:
		CException(int nMessageID);
		CException(LPCTSTR pszText = NULL, int nMessageID = 0);
		virtual ~CException() throw();

		void ReportError();
		LPCTSTR GetText() const;
		int GetMessageID() const;
		virtual const char* what() const throw() = 0; // pure virtual function

	private:
		LPCTSTR m_pszText;
		DWORD  m_nMessageID;
	};

	////////////////////////////////////////
	// Declaration of the CUserException class
	//
	class CUserException : public CException
	{
	public:
		CUserException(int nMessageID);
		CUserException(LPCTSTR pszText = NULL, int nMessageID = 0);
		virtual ~CUserException() throw();
		virtual const char* what() const throw();
	};


	////////////////////////////////////////
	// Declaration of the CWinException class
	//
	// Note: Each function guarantees not to throw an exception

	class CWinException : public CException
	{
	public:
		CWinException(int nMessageID);
		CWinException(LPCTSTR pszText= NULL, int nMessageID = 0);
		virtual ~CWinException() throw() {}
		DWORD GetError() const;
		LPCTSTR GetErrorString() const;
		virtual const char* what () const throw();

	private:
		DWORD  m_Error;
		LPCTSTR m_pszText;
		TCHAR m_szErrorString[MAX_STRING_SIZE];
	};

	//////////////////////////////////////////
	// Definitions for the CException class
	//

	inline CException::CException(int nMessageID)
			: m_pszText(NULL), m_nMessageID(nMessageID)
	{
	}

	inline CException::CException(LPCTSTR pszText /*= NULL*/, int nMessageID /*= 0*/)
			: m_pszText(pszText), m_nMessageID(nMessageID)
	{
	}

	inline CException::~CException() throw()
	{
	}

	inline LPCTSTR CException::GetText() const
	// Retrieves the string specified in the constructor.
	{
		return m_pszText;
	}

	inline int CException::GetMessageID() const
	{
		return m_nMessageID;
	}

	inline void CException::ReportError()
	{
		if (m_pszText != 0)
			MessageBox(NULL, m_pszText, _T("An CException Occurred"), MB_OK);
		else if (m_nMessageID > 0 )
			MessageBox(NULL, LoadString(m_nMessageID), _T("A CException Occurred"), MB_OK);

		else MessageBox(NULL, _T("Unspecified Error"), _T("A CException Occurred"), MB_OK);
	}


	////////////////////////////////////////
	// Definitions of the CUserException class
	//

	inline CUserException::CUserException(int nMessageID)
			: CException(nMessageID)
	{
	}
	
	inline CUserException::CUserException(LPCTSTR pszText /*= NULL*/, int nMessageID /*= 0*/)
			: CException(pszText, nMessageID)
	{
	}

	inline CUserException::~CUserException() throw()
	{
	}

	inline const char* CUserException::what() const throw()
	{
		return "CUserException thrown";
	}


	//////////////////////////////////////////
	// Definitions for the CWinException class
	//

	inline CWinException::CWinException(int nMessageID)
		: CException(nMessageID), m_Error(::GetLastError())
	{
		memset(m_szErrorString, 0, MAX_STRING_SIZE * sizeof(TCHAR));

		DWORD dwFlags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
		::FormatMessage(dwFlags, NULL, m_Error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), m_szErrorString, MAX_STRING_SIZE-1, NULL);
	}
	
	inline CWinException::CWinException(LPCTSTR pszText, int nMessageID) 
		: CException(pszText, nMessageID), m_Error(::GetLastError())
	{
		memset(m_szErrorString, 0, MAX_STRING_SIZE * sizeof(TCHAR));

		DWORD dwFlags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
		::FormatMessage(dwFlags, NULL, m_Error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), m_szErrorString, MAX_STRING_SIZE-1, NULL);
	}

	inline DWORD CWinException::GetError() const
	// Retrieves the last error code value.
	{
		return m_Error;
	}

	inline LPCTSTR CWinException::GetErrorString() const
	// Retrieves the error string from GetLastError.
	{
		return m_szErrorString;
	}

	inline const char * CWinException::what() const throw()
	// Sends the last error string to the debugger (typically displayed in the IDE's output window).
	{
		::OutputDebugString(m_szErrorString);
		return "CWinException thrown";
	} 

} // namespace Win32xx


#endif // _WIN32XX_EXCEPTION_H_
