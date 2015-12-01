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


////////////////////////////////////////////////////////
// Acknowledgement:
//
// The original author of CArchive is:
//
//      Robert C. Tausworthe
//      email: robert.c.tausworthe@ieee.org
//
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
//	Contents Description:
//  The CArchive class is used to serialize and
//  deserialize data. Data is streamed to and from the
//  file specified by the user, using the >> and <<
//  operators.

// Note: This CArchive doesn't read archive written by MFC's CArchive.

//  Differences between this CArchive and MFC's CArchive
//   In this CArchive:
//    - LPCTSTR strings retrieved from the archive exactly match the string
//       saved to the archive. Strings can contain carriage return and line
//       feed characters. These characters are not stripped.
//    - CStrings retrieved from the archive exactly match the CString
//       saved in the archive. CStrings can contain any character, including
//       embedded null characters, line feeds and carriage returns.
//
//  In MFC's CArchive:
//    - LPCTSTR strings have carriage return characters stripped
//    - LPCTSTR strings are terminated by a \r\n characters.
 

#ifndef _WIN32XX_ARCHIVE_H_
#define _WIN32XX_ARCHIVE_H_

#include "wxx_appcore0.h"
#include "wxx_cstring.h"
#include "wxx_file.h"


namespace Win32xx
{

	struct ArchiveObject{UINT size; LPVOID p;};

	// Unspecified object type containing the pointer p to a 
	// memory block and its size that is either to be written into (<<)  or
	// retrieved from (>>) an archive file. This struct is used in
	// serialization and deserialization of data, particularly those that
	// are not of a type for which available << and >> operators are defined.
	// The size and pointer describe the extent and  location of the object
	// to be serialized. A typical usage would be of the form:

	//    ArchiveObject ao = {sizeof(A), &A};
	//	  ar << ao; or ar >> ao;


	//=============================================================================
	class CArchive
	// Serialization CArchive class
	{
	public:
		// file modes
		enum mode {store = 0, load = 1};

		// construction and  destruction
		CArchive(CFile& File, mode Mode);
		CArchive(LPCTSTR FileName, mode nMode);
		~CArchive();

		// method members
		UINT 	GetObjectSchema();
		bool 	IsLoading() const;
		bool 	IsStoring() const;
		void 	Read(void* lpBuf, UINT size);
		LPTSTR	ReadString(LPTSTR szString, UINT nMax);
		void 	SetObjectSchema(UINT nSchema);
		void 	Write(const void* lpBuf, UINT size);
		void	WriteString(LPCTSTR string);

		// insertion operations
		CArchive& operator<<(BYTE by);
		CArchive& operator<<(WORD w);
		CArchive& operator<<(LONG l);
		CArchive& operator<<(LONGLONG ll);
		CArchive& operator<<(ULONGLONG ull);
		CArchive& operator<<(DWORD dw);
		CArchive& operator<<(float f);
		CArchive& operator<<(double d);
		CArchive& operator<<(int i);
		CArchive& operator<<(short w);
		CArchive& operator<<(char ch);
		CArchive& operator<<(unsigned u);
		CArchive& operator<<(bool b);
		CArchive& operator<<(const CString& s);
		CArchive& operator<<(const POINT& pt);
		CArchive& operator<<(const RECT& rc);
		CArchive& operator<<(const SIZE& sz);
		CArchive& operator<<(const ArchiveObject& ao);
		CArchive& operator<<(const CObject& Ob);
#if !defined (_MSC_VER) ||  ( _MSC_VER > 1310 )
		// wchar_t is not an a built-in type on older MS compilers
		CArchive& operator<<(wchar_t ch);
#endif

		// extraction operations
		CArchive& operator>>(BYTE& by);
		CArchive& operator>>(WORD& w);
		CArchive& operator>>(DWORD& dw);
		CArchive& operator>>(LONG& l);
		CArchive& operator>>(LONGLONG& ll);
		CArchive& operator>>(ULONGLONG& ull);
		CArchive& operator>>(float& f);
		CArchive& operator>>(double& d);
		CArchive& operator>>(int& i);
		CArchive& operator>>(short& w);
		CArchive& operator>>(char& ch);
		CArchive& operator>>(unsigned& u);
		CArchive& operator>>(bool& b);
		CArchive& operator>>(CString& s);
		CArchive& operator>>(POINT& pt);
		CArchive& operator>>(RECT& rc);
		CArchive& operator>>(SIZE& sz);
		CArchive& operator>>(ArchiveObject& ao);
		CArchive& operator>>(CObject& Ob);
#if !defined (_MSC_VER) ||  ( _MSC_VER > 1310 )
		// wchar_t is not an a built-in type on older MS compilers
		CArchive& operator>>(wchar_t& ch);
#endif


	private:
		CArchive(const CArchive&);				// Disable copy construction
		CArchive& operator = (const CArchive&); // Disable assignment operator

		// private data members
		CFile*	m_pFile;			// archive file FILE
		bool	m_IsStoring;		// archive direction switch
		UINT	m_Schema;			// archive version schema
		bool	m_IsFileManaged;	// delete the CFile pointer in destructor;

	};


} // namespace Win32xx

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace Win32xx
{
	inline CArchive::CArchive(CFile& File, mode Mode) : m_Schema((UINT)-1), m_IsFileManaged(false)
	// Construct a CArchive object
	// The specified file must already be open for loading or storing.
	{
		m_pFile = &File;

		if (Mode == load)
		{
			m_IsStoring = false;
		}
		else
		{
			m_IsStoring = true;
		}
	}

	inline CArchive::CArchive(LPCTSTR FileName, mode Mode) : m_pFile(0), m_Schema((UINT)-1)
	// Construct a CArchive object
	// A file with the specified name is created for storing (if required), and
	// also opened. A failure to open the file will throw an exception.
	{
		m_IsFileManaged = true;

		try
		{
			if (Mode == load)
			{	
				// Open the archive for loading
				m_pFile = new CFile(FileName, CFile::modeRead);	
				m_IsStoring = false;
			}
			else
			{
				// Open the archive for storing. Creates file if required
				m_pFile = new CFile(FileName, CFile::modeCreate);
				m_IsStoring = true;
			}
		}

		catch(...)
		{
			delete m_pFile;
			throw; // Rethrow the exception
		}
	}

	//============================================================================
	inline CArchive::~CArchive()
	// Destroy CArchive object.
	{
		if (m_pFile)
		{
			// if the file is open
			if (m_pFile->GetHandle())
			{
				// flush if in write mode
				if (IsStoring())
					m_pFile->Flush();

				m_pFile->Close();
			}

			if (m_IsFileManaged)
			{
				delete m_pFile;
			}
		}
	}

	//============================================================================
	inline void CArchive::Read(void* lpBuf, UINT size)
	// Read size bytes from the open archive file into the given lpBuf.
	// Throw an exception if not successful.
	{
		// read, simply and  in binary mode, the size into the lpBuf
		assert(m_pFile);
		m_pFile->Read(lpBuf, size);
	}

	//============================================================================
	inline void CArchive::Write(const void* lpBuf, UINT size)
	// Write size characters of from the lpBuf into the open archive file.
	// Throw an exception if unsuccessful.
	{
		// write size characters in lpBuf to the  file
		assert(m_pFile);
		m_pFile->Write(lpBuf, size);
	}

	//============================================================================
	inline UINT CArchive::GetObjectSchema()
	// Return the archived data schema. This acts as a version number on
	// the format of the archived data for special handling when there
	// are several versions of the serialized data to be accommodated
	// by the application.
	{
		return m_Schema;
	}

	//============================================================================
	inline void CArchive::SetObjectSchema(UINT nSchema)
	// Record the archived data schema number.  This acts as a version number
	// on the format of the archived data for special handling when there
	// are several versions of the serialized data to be accommodated
	// by the application.
	{
		m_Schema = nSchema;
	}

	//============================================================================
	inline bool CArchive::IsLoading() const
	// Return the current sense of serialization, true if the archive is
	// being loaded.
	{
		 return !m_IsStoring;
	}

	//============================================================================
	inline bool CArchive::IsStoring() const
	// Return the current sense of serialization, true if the archive is
	// being stored.
	{
		return m_IsStoring;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(BYTE b)
	// Write the BYTE b into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(b), &b};
		*this << ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(WORD w)
	// Write the WORD w into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(w), &w};
		*this << ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(LONG l)
	// Write the LONG l into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(l), &l};
		*this << ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(LONGLONG ll)
	// Write the LONG l into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(ll), &ll};
		*this << ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(ULONGLONG ull)
	// Write the LONG l into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(ull), &ull};
		*this << ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(DWORD dw)
	// Write the DWORD dw into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(dw), &dw};
		*this << ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(float f)
	// Write the float f into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(f), &f};
		*this << ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(double d)
	// Write the double d into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(d), &d};
		*this << ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(int i)
	// Write the int i into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(i), &i};
		*this << ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(short s)
	// Write the short s into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(s), &s};
		*this << ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(char c)
	// Write the char c into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(c), &c};
		*this << ob;
		return *this;
	}

// wchar_t is not an a built-in type on older MS compilers
#if !defined (_MSC_VER) ||  ( _MSC_VER > 1310 )
	
	//============================================================================
	inline CArchive& CArchive::operator<<(wchar_t ch)
	// Write the wchar_t ch into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(ch), &ch};
		*this << ob;
		return *this;
	}
#endif

	//============================================================================
	inline CArchive& CArchive::operator<<(unsigned u)
	// Write the unsigned u into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(u), &u};
		*this << ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(bool b)
	// Write the bool b into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(b), &b};
		*this << ob;
		return *this;
	}

	//============================================================================
	inline void CArchive::WriteString(LPCTSTR string)
	// Write the LPCTSTR string into the archive file. 
	// The string must be null terminated. 
	// Throw an exception if an error occurs.
	{
		UINT nChars = lstrlen(string);
		bool IsUnicode = sizeof(TCHAR) -1;
		
		// Store the Unicode state and number of characters in the archive 
		*this << IsUnicode;
		*this << nChars;
		
		Write(string, nChars*sizeof(TCHAR));
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(const CString& string)
	// Write the CString string into the archive file.
	// The CString can contain any characters including embedded nulls.
	// Throw an exception if an error occurs.
	{
		UINT nChars = string.GetLength();
		bool IsUnicode = sizeof(TCHAR) -1;
		
		// Store the Unicode state and number of characters in the archive 
		*this << IsUnicode;
		*this << nChars;

		Write(string.c_str(), nChars*sizeof(TCHAR));
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(const POINT& pt)
	// Write the POINT pt into the archive file. Throw an exception
	// if an error occurs.
	{
		UINT size = sizeof(pt);

		// Write() throws exception upon error
		Write(&size, sizeof(size));
		Write(&pt, size);
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(const RECT& rc)
	// Write the RECT rc into the archive file. Throw an exception
	// if an error occurs.
	{
		UINT size = sizeof(rc);

		// Write() throws exception upon error
		Write(&size, sizeof(size));
		Write(&rc, size);
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(const SIZE& sz)
	// Write the SIZE sz into the archive file. Throw an exception
	// if an error occurs.
	{
		UINT size = sizeof(sz);

		// Write() throws exception upon error
		Write(&size, sizeof(size));
		Write(&sz, size);
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(const ArchiveObject& ao)
	// Write arbitrary CArchive object into this CArchive. Only ob.size  and
	// pointer ob.p to location are given. Throw an excepton if unable
	// to do so successfully.
	{
		Write(&ao.size, sizeof(ao.size));

		// Write() throws exception upon error
		Write(ao.p, ao.size);
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(BYTE& b)
	// Read a BYTE from the archive and  store it in b.  Throw an exception if
	// unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(b), &b};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(WORD& w)
	// Read a WORD from the archive and  store it in w.  Throw an exception if
	// unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(w), &w};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(LONG& l)
	// Read a LONG from the archive and store it in l.  Throw an exception if
	// unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(l), &l};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(LONGLONG& ll)
	// Read a LONGLONG from the archive and store it in ll.  Throw an exception if
	// unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(ll), &ll};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(ULONGLONG& ull)
	// Read a ULONGLONG from the archive and store it in ull.  Throw an exception if
	// unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(ull), &ull};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(DWORD& dw)
	// Read a DWORD	 from the archive and  store it in dw.  Throw an
	// exception if unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(dw), &dw};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(float& f)
	// Read a float from the archive and  store it in f.  Throw an exception if
	// unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(f), &f};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(double& d)
	// Read a double from the archive and  store it in d.  Throw an exception if
	// unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(d), &d};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(int& i)
	// Read an int from the archive and  store it in i.  Throw an exception if
	// unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(i), &i};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(short& i)
	// Read a short from the archive and  store it in i.  Throw an exception if
	// unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(i), &i};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(char& c)
	// Read a char from the archive and  store it in c.  Throw an exception if
	// unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(c), &c};
		*this >> ob;
		return *this;
	}

// wchar_t is not an a built-in type on older MS compilers
#if !defined (_MSC_VER) ||  ( _MSC_VER > 1310 )

	//============================================================================
	inline CArchive& CArchive::operator>>(wchar_t& ch)
	// Read a wchar_t from the archive and  store it in ch.  Throw an exception if
	// unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(ch), &ch};
		*this >> ob;
		return *this;
	}

#endif

	//============================================================================
	inline CArchive& CArchive::operator>>(unsigned& u)
	// Read an unsigned int from the archive and  store it in u.  Throw an
	// exception if unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(u), &u};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(bool& b)
	// Read an bool from the archive and  store it in b.  Throw an
	// exception if unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(b), &b};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline LPTSTR CArchive::ReadString(LPTSTR szString, UINT nMax)
	// The size (in characters) of szString array must be nMax or greater.
	// Reads at most nMax-1 TCHAR characters from the archive and store it
	// in szString. Strings read from the archive are converted from ANSI
	// or Unicode to TCHAR if required, and are NULL terminated.
	// Throw an exception if unable to do so correctly. 
	{
		assert (nMax > 0);

		CString str;
		*this >> str;
		lstrcpyn(szString, str.c_str(), nMax);
		return szString;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(CString& string)
	// Read a CString from the archive and  store it in string.  Throw an
	// exception if unable to do so correctly. Note: exceptions are thrown
	// only on inability to read the recorded number of chars from the archive
	// stream.
	{
		bool IsUnicode;
		UINT nChars;
		
		// Retrieve the Unicode state and number of characters from the archive
		*this >> IsUnicode;
		*this >> nChars;
	
		if (IsUnicode)
		{	
			WCHAR* buf = new WCHAR[nChars];
			Read(buf, nChars*2);
			
#ifdef _UNICODE
			memcpy(string.GetBuffer(nChars), buf, nChars*2);
#else
			// Convert the archive string from Wide to Ansi
			WideCharToMultiByte(CP_ACP, 0, buf, nChars, string.GetBuffer(nChars), nChars, NULL,NULL);
#endif

			string.ReleaseBuffer(nChars);
			delete[] buf;
		}
		else
		{
			char* buf = new char[nChars];
			Read(buf, nChars);

#ifdef _UNICODE
			// Convert the archive string from Ansi to Wide
			MultiByteToWideChar(CP_ACP, 0, buf, nChars, string.GetBuffer(nChars), nChars);
#else
			memcpy(string.GetBuffer(nChars), buf, nChars);
#endif

			string.ReleaseBuffer(nChars);
			delete[] buf;
		}

		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(POINT& pt)
	// Read a POINT from the archive and  store it in pt.  Throw an
	// exception if unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(pt), &pt};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(RECT& rc)
	// Read a RECT from the archive and  store it in rc.  Throw an
	// exception if unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(rc), &rc};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(SIZE& sz)
	// Read a SIZE from the archive and  store it in sz.  Throw an
	// exception if unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(sz), &sz};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(ArchiveObject& ao)
	// Read a char string of size ob.size from the archive and  store it in
	// the location pointed to by ob.p.  Throw an exception if unable to
	// do so correctly.
	{
		assert(m_pFile);
		UINT size;
		Read(&size, sizeof(size));
		if (size != ao.size)
		{
		//	throw CWinException(_T("Unable to read object from archive"));
			throw CFileException(m_pFile->GetFilePath(), _T("Unable to read object from archive"));
		}

		Read(ao.p, ao.size);
		return *this;
	}

	inline CArchive& CArchive::operator<<(const CObject& Ob)
	{
		((CObject&)Ob).Serialize(*this);
		return *this;
	}

	inline CArchive& CArchive::operator>>(CObject& Ob)
	{
		Ob.Serialize(*this);
		return *this;
	}


} // namespace Win32xx

#endif // _WIN32XX_ARCHIVE_H_
