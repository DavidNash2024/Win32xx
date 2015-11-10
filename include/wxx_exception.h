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


//
// Sample code, demonstrating how to use CUserException with a string resource:
//
//  try
//  {
//		...	
//  	if (Failed)
//			throw CUserException(nID));
//		...
//	}
//	catch(const CException &e) // catch all exceptions inherited from CException
// 	{
//		// display the exception in a message box
//		::MessageBox(NULL, LoadString(e.GetMessageID()), A2T(e.what()), MB_ICONERROR);
//	}

//
// Sample code, demonstrating how to use CUserException with a text string:
//
//  try
//  {
//		...	
//  	if (Failed)
//			throw CUserException(_T("Some Text"));
//		...
//	}
//	catch(const CException &e) // catch all exceptions inherited from CException
// 	{
//		// display the exception in a message box
//		::MessageBox(NULL, e.GetText(), A2T(e.what()), MB_ICONERROR);
//	}

  

namespace Win32xx
{
	////////////////////////////////////////
	// Declaration of the CException class
	//
	//  This class has a pure virtual function and is an abstract class.
	//  We can't throw a CException directly, but we can throw any exception
	//  inherited from CException. We can catch all exceptions inherited from
	//  CException with a single catch statement.
	//
	class CException : public std::exception
	{
	public:
		CException(int nMessageID);
		CException(LPCTSTR pszText = NULL, int nMessageID = 0);
		virtual ~CException() throw();

		void ReportError() const throw();
		LPCTSTR GetText() const throw();
		int GetMessageID() const throw();
		virtual const char* what() const throw() = 0; // pure virtual function

	private:
		TCHAR m_szText[MAX_STRING_SIZE];
		DWORD  m_nMessageID;
	};

	//////////////////////////////////////////
	// Declaration of the CUserException class
	//
	//  This exception it thrown by CDataExchange when verifications fail.
	//  It is also the exception that is typically thrown by users.
	//	Users have the option of specifying text when the exception is thrown,
	//  and the option of specifying a message ID which could load text from
	//	a resource.
	//
	// Note: Each function guarantees not to throw an exception
	//
	class CUserException : public CException
	{
	public:
		CUserException(int nMessageID);
		CUserException(LPCTSTR pszText = NULL, int nMessageID = 0);
		virtual ~CUserException() throw();
		virtual const char* what() const throw();
	};
	
	
	//////////////////////////////////////////////////
	// Declaration of the CNotSupportedException class
	//
	//	This exception is used by the Win32++ framework to indicate
	//  critical errors. 
	//
	// Note: Each function guarantees not to throw an exception
	//
	class CNotSupportedException : public CException
	{
	public:
		CNotSupportedException(int nMessageID);
		CNotSupportedException(LPCTSTR pszText = NULL, int nMessageID = 0);
		virtual ~CNotSupportedException() throw();
		virtual const char* what() const throw();
	};


	////////////////////////////////////////
	// Declaration of the CWinException class
	//
	//  This exception can be used to display the information retrieved from
	//  GetLastError in text form when a Windows API function fails.
	//
	// Note: Each function guarantees not to throw an exception
	//
	class CWinException : public CException
	{
	public:
		CWinException(int nMessageID);
		CWinException(LPCTSTR pszText= NULL, int nMessageID = 0);
		virtual ~CWinException() throw() {}
		DWORD GetError() const throw();
		LPCTSTR GetErrorString() const throw();
		virtual const char* what () const throw();

	private:
		DWORD  m_Error;
		TCHAR m_szErrorString[MAX_STRING_SIZE];
	};
	

	//////////////////////////////////////////
	// Definitions for the CException class
	//
	inline CException::CException(int nMessageID)
			: m_nMessageID(nMessageID)
	{
		memset(m_szText, 0, MAX_STRING_SIZE * sizeof(TCHAR));
	}

	inline CException::CException(LPCTSTR pszText /*= NULL*/, int nMessageID /*= 0*/)
			: m_nMessageID(nMessageID)
	{
		memset(m_szText, 0, MAX_STRING_SIZE * sizeof(TCHAR));
		if (pszText)
			lstrcpyn(m_szText, pszText, MAX_STRING_SIZE-1);
	}

	inline CException::~CException() throw()
	{
	}

	inline LPCTSTR CException::GetText() const throw()
	// Retrieves the string specified in the constructor.
	{
		return m_szText;
	}

	inline int CException::GetMessageID() const throw()
	{
		return m_nMessageID;
	}

	inline void CException::ReportError() const throw()
	{
		if (m_szText[0] != 0)
			MessageBox(NULL, m_szText, _T("Exception Occurred"), MB_OK);
		else if (m_nMessageID > 0 )
			MessageBox(NULL, LoadString(m_nMessageID), _T("Exception Occurred"), MB_OK);

		else MessageBox(NULL, _T("Unspecified Error"), _T("Exception Occurred"), MB_OK);
	}


	////////////////////////////////////////
	// Definitions of the CUserException class
	//
	inline CUserException::CUserException(int nMessageID)
			: CException(nMessageID)
	{
		::OutputDebugString(_T("*** CUserException thrown ***\n"));
	}
	
	inline CUserException::CUserException(LPCTSTR pszText /*= NULL*/, int nMessageID /*= 0*/)
			: CException(pszText, nMessageID)
	{
		::OutputDebugString(_T("*** CUserException thrown ***\n"));
	}

	inline CUserException::~CUserException() throw()
	{
	}

	inline const char* CUserException::what() const throw()
	{
		return "Win32xx::CUserException";
	}
	
	
	//////////////////////////////////////////////////
	// Definitions of the CNotSupportedException class
	//	
	inline CNotSupportedException::CNotSupportedException(int nMessageID)
		: CException(nMessageID)
	{
		::OutputDebugString(_T("*** CNotSupportedException thrown ***\n"));
	}
	
	inline CNotSupportedException::CNotSupportedException(LPCTSTR pszText /*= NULL*/, int nMessageID /*= 0*/)
		: CException(pszText, nMessageID)
	{
		::OutputDebugString(_T("*** CNotSupportedException thrown ***\n"));
	}
	
	inline CNotSupportedException::~CNotSupportedException() throw()
	{
	}
	
	inline const char* CNotSupportedException::what() const throw()
	{
		return "Win32xx::CNotSupportedException";		
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
		
		::OutputDebugString(_T("*** CWinException thrown ***\n"));
		::OutputDebugString(m_szErrorString);
	}
	
	inline CWinException::CWinException(LPCTSTR pszText, int nMessageID) 
		: CException(pszText, nMessageID), m_Error(::GetLastError())
	{
		memset(m_szErrorString, 0, MAX_STRING_SIZE * sizeof(TCHAR));

		DWORD dwFlags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
		::FormatMessage(dwFlags, NULL, m_Error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), m_szErrorString, MAX_STRING_SIZE-1, NULL);
		
		::OutputDebugString(_T("*** CWinException thrown ***\n"));
		::OutputDebugString(m_szErrorString);
	}

	inline DWORD CWinException::GetError() const throw()
	// Retrieves the last error code value.
	{
		return m_Error;
	}

	inline LPCTSTR CWinException::GetErrorString() const throw()
	// Retrieves the error string from GetLastError.
	{
		return m_szErrorString;
	}

	inline const char * CWinException::what() const throw()
	// Sends the last error string to the debugger (typically displayed in the IDE's output window).
	{
		return "Win32xx::CWinException";
	} 

} // namespace Win32xx


#endif // _WIN32XX_EXCEPTION_H_
