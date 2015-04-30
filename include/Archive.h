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
// Acknowledgement:
// 
// The original author of CArchive is:
//
//      Robert C. Tausworthe
//      2812 Rancho Costero
//      Carlsbad, CA 92009
//
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
//	Contents Description:
//  The CArchive class is used to serialize and 
//  deserialize data. Data is streamed to and from the 
//  file specified by the user, using the >> and << 
//  operators. 



#ifndef _WIN32XX_ARCHIVE_H_
#define _WIN32XX_ARCHIVE_H_

#include "wincore.h"
#include "file.h"

namespace Win32xx
{

	struct ArchiveObject{UINT size; LPVOID p; LPCTSTR msg;};

	// Unspecified object type containing the size of, a pointer p to, and  a
	// message about a memory block that is either to be written into (<<)  or 
	// retrieved from (>>) an archive file. This struct is used in
	// serialization and  deserialization of data, particularly those that
	// are not of a type for which available << and >> operators are defined.
	// The size and  pointer describe the extent and  location of the object
	// to be serialized, and  the message msg, which may be NULL, is inserted
	// into the exception message that is thrown when a failure to serialize
	// the object is encountered.  A typical usage would be of the form

	//    ArchiveObject ao = {sizeof(A), &A, "typeof(A)"};
	//	  ar << ao; or ar >> ao;
	
	
	//=============================================================================
	class CArchive
	// Serialization CArchive class
	{
	public:
		// file modes
		enum mode{nil = -1, read, write};

		// construction and  destruction
		CArchive();
		~CArchive();

		// method members
		void    Close();
		UINT 	GetObjectSchema();
		bool 	IsLoading() const;
		bool 	IsOpen() const;
		bool 	IsStoring() const;
		bool	Open(CString filename, CArchive::mode);
		void 	Read(void* lpBuf, UINT size, LPCTSTR msg = TEXT(""));
		void    SetError(UINT nError);
		void 	SetObjectSchema(UINT nSchema);
		void    SyncPoint();
		void 	Write(const void* lpBuf, UINT size, LPCTSTR msg = TEXT(""));

		// insertion operations
		CArchive& operator<<(BYTE by);
		CArchive& operator<<(WORD w);
		CArchive& operator<<(LONG l);
		CArchive& operator<<(DWORD dw);
		CArchive& operator<<(float f);
		CArchive& operator<<(double d);
		CArchive& operator<<(int i);
		CArchive& operator<<(short w);
		CArchive& operator<<(char ch);
		CArchive& operator<<(unsigned u);
		CArchive& operator<<(LPCTSTR s);
		CArchive& operator<<(const CString& s);
		CArchive& operator<<(const CPoint& s);
		CArchive& operator<<(const ArchiveObject& ob);

		// extraction operations
		CArchive& operator>>(BYTE& by);
		CArchive& operator>>(WORD& w);
		CArchive& operator>>(DWORD& dw);
		CArchive& operator>>(LONG& l);
		CArchive& operator>>(float& f);
		CArchive& operator>>(double& d);
		CArchive& operator>>(int& i);
		CArchive& operator>>(short& w);
		CArchive& operator>>(char& ch);
		CArchive& operator>>(unsigned& u);
		CArchive& operator>>(LPTSTR s);
		CArchive& operator>>(CString& s);
		CArchive& operator>>(CPoint& s);
		CArchive& operator>>(ArchiveObject& ob);

		// public data members
		CString	m_sFileName;
		mode	m_mMode;
		int     m_nError;

	protected:

	private:
		// private data members
		CFile    m_file;        // archive file FILE, initially closed
		bool     m_is_storing;  // archive direction switch
		UINT     m_schema;      // archive version schema
		CString	 m_sync_mark;	// archive sync test marker
	};

	// Global functions
	CString		LastErrorMessage(void);
	CString		SystemErrorMessage(int errnum);


} // namespace Win32xx

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace Win32xx
{

	inline CArchive::CArchive()
	// Construct an empty CArchive object
	{
		m_sFileName.Empty();
		m_mMode       = nil;
		m_schema      = 0;
		m_is_storing  = false;
		m_nError      = NO_ERROR;
		m_sync_mark	  = TEXT("!@#$%^&*()_+");
	}

	//============================================================================
	inline CArchive::~CArchive()
	// Destroy CArchive object.
	{
	}

	//============================================================================
	inline bool CArchive::Open(CString filename, CArchive::mode mode)
	// Open the archive filename in the given CArchive::mode and  create a
	// buffer hving the given nBuffSize.  The mode can be read or write.
	// Return true if the archive opens without error, or false, otherwise.
	// Does throw an exception if the schema cannot be written or read back.
	{
		::SetLastError(0);
		
		// if the archive is open, close it
		Close();
		
		// Record the archive mode, even if the filename cannot be opened,
		// for use in later determining error status of a subsequent
		// de/serialization operation.
		if ((m_mMode = mode) == read)
		{
			if (!m_file.Open(filename.c_str(), OPEN_EXISTING))
			{
				CString msg = TEXT("Archived information file missing:\n") + filename +
					TEXT("\nInformation not recovered.\n");

				::MessageBox(NULL, msg, TEXT("Information"), MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
				return false;
			}
			m_is_storing = false;
			m_sFileName = filename;
			
			// recover schema of serialized configuration
			*this >> m_schema;
		}
		else if (mode == write)
		{
			// if filename not given, use name used for reading
			CString fn = filename.GetLength() == 0 ? m_sFileName : filename;
			
			// open the file, simply, in binary mode
			if (!(m_file.Open(fn.c_str(), CREATE_ALWAYS)))
			{
				CString msg = TEXT("Could not write CArchive file:\n")
					+ fn + TEXT("Current environment not saved.");
				
				MessageBox(NULL, msg, TEXT("Information"), MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
				m_nError = ERROR_FILE_NOT_FOUND; // write open error
				return false;
			}
			m_is_storing = true;
			m_sFileName = filename;
			
			// record schema of current configuration if at beginning
			*this << m_schema;
		}
		else // mode is nil
		{
			m_nError = ERROR_INVALID_DATA; // invalid archive error
			::MessageBox(NULL, TEXT("Invalid archive mode."), TEXT("Error"), \
				MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
		}
		
		m_nError = NO_ERROR;
		return mode != nil;
	}

	//============================================================================
	inline void CArchive::Close()
	// Close the CArchive object: close the file and  delete the buffer.
	{
		 // if the archive is open
		if (m_file.GetHandle())
		{
			// flush if in write mode
			if (m_mMode == write)
				m_file.Flush();
			m_file.Close();
		}

		m_mMode = nil;
		m_nError = NO_ERROR;
		m_sFileName.Empty();
	}

	//============================================================================
	inline void CArchive::Read(void* lpBuf, UINT size, LPCTSTR msg /* = "" */)
	// Read size bytes from the open archive file into the given lpBuf.
	// Return on success and  the number of bytes actually read is size.
	// Throw an exception if not successful containing the message msg.
	{
		::SetLastError(0);

		// read, simply and  in binary mode, the size into the lpBuf
		if (size == 0  ||
			(m_file.Read(lpBuf, size) != size)  ||
			(m_nError = ::GetLastError()) != NO_ERROR)
		{
			CString e;
			e.Format(TEXT("CArchive read error: %s\n%s"), msg,
				SystemErrorMessage(m_nError).c_str());
			throw CWinException(e.c_str());
		}
	}

	//============================================================================
	inline void CArchive::Write(const void* lpBuf, UINT size, LPCTSTR msg /* = "" */)
	// Write size characters of from the lpBuf into the open archive file  and
	// return successfully if the number of characters actually written is
	// size. Throw an exception if unsuccessful containing the message msg.
	{
		::SetLastError(0);
		
		// write size characters in lpBuf to the  file	
		if ((size == 0)  ||
			(m_nError != NO_ERROR)  ||
			(!m_file.Write(lpBuf, size))  ||
			((m_nError = ::GetLastError()) != NO_ERROR))
		{
			CString e;
			e.Format(TEXT("CArchive write error: %s\n%s"), msg,
				SystemErrorMessage(m_nError).c_str());
			throw CWinException(e.c_str());
		}
	}

	//============================================================================
	inline UINT CArchive::GetObjectSchema()
	// Return the archived data schema. This acts as a version number on
	// the format of the archived data for special handling when there
	// are several versions of the serialized data to be accommodated
	// by the application.
	{
		return m_schema;
	}

	//============================================================================
	inline void CArchive::SetError(UINT nError)
	// Set the archive error indicator to nError.
	{
		m_nError = nError;
	}

	//============================================================================
	inline void CArchive::SetObjectSchema(UINT nSchema)
	// Record the archived data schema number.  This acts as a version number
	// on the format of the archived data for special handling when there
	// are several versions of the serialized data to be accommodated
	// by the application.
	{
		m_schema = nSchema;
	}

	//============================================================================
	inline void CArchive::SyncPoint()
	// If storing, write the sync mark string into the archive; if loading,
	// read back and  validate that the sync mark is present. If unable to
	// write or validate the sync mark, throw an exception.  Otherwise,
	// leave the archive open for further serialization.
		
	// The use of SyncPoint() is recommended to assure that corruptions  or
	// errors in serialization or deserialization of an archive are more
	// likely to be handled more gracefully than if such synchronization
	// were not present.
	{
		CString e;
		if (m_nError != NO_ERROR)
		{
			e.Format(TEXT("CArchive sync point error:\n%s"),
				SystemErrorMessage(m_nError).c_str());
			throw CWinException(e.c_str());
		}

		// error if writing to an unopened archive
		if (!IsOpen())
		{
			m_nError = ERROR_READ_FAULT;
			e.Format(TEXT("CArchive not opened for sync point.\n%s"),
				SystemErrorMessage(m_nError).c_str());
			throw CWinException(e.c_str());
		}
		  
		// perform loading or storing
		if (IsStoring())
		{
			// Storing to archive
			*this << m_sync_mark;
		}
		else
		{
			// Loading from archive
			CString sync_check;
			*this >> sync_check;

			if (sync_check != m_sync_mark)
			{
				m_nError = ERROR_READ_FAULT;
				e.Format(TEXT("CArchive sync point read error.\n%s"),
					SystemErrorMessage(m_nError).c_str());
				throw CWinException(e.c_str());
			} 
		}
	}

	//============================================================================
	inline bool CArchive::IsLoading() const
	// Return the current sense of serialization, true if the archive is
	// being loaded.
	{
		 return !m_is_storing;
	}

	//============================================================================
	inline bool CArchive::IsOpen() const
	// Return the status of the archive object file, true if it is open.
	{
		return (m_file.GetHandle() != NULL);
	}

	//============================================================================
	inline bool CArchive::IsStoring() const
	// Return the current sense of serialization, true if the archive is
	// being stored.
	{
		return m_is_storing;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(BYTE b)
	// Write the BYTE b into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(b), &b, TEXT("byte")};
		*this << ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(WORD w)
	// Write the WORD w into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(w), &w, TEXT("word")};
		*this << ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(LONG l)
	// Write the LONG l into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(l), &l, TEXT("long")};
		*this << ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(DWORD dw)
	// Write the DWORD dw into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(dw), &dw, TEXT("DWORD")};
		*this << ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(float f)
	// Write the float f into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(f), &f, TEXT("float")};
		*this << ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(double d)
	// Write the double d into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(d), &d, TEXT("double")};
		*this << ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(int i)
	// Write the int i into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(i), &i, TEXT("int")};
		*this << ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(short s)
	// Write the short s into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(s), &s, TEXT("short")};
		*this << ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(char c)
	// Write the char c into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(c), &c, TEXT("char")};
		*this << ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(unsigned u)
	// Write the unsigned u into the archive file. Throw an exception if an
	// error occurs.
	{
		ArchiveObject ob = {sizeof(u), &u, TEXT("unsigned")};
		*this << ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(LPCTSTR string)
	// Write the LPCTSTR string into the archive file. Throw an exception
	// if an error occurs.
	{
		UINT size = (lstrlen(string) + 1) * sizeof(TCHAR);
		 
		// Write() throws exception upon error
		Write((char *)&size, sizeof(size_t), TEXT("C string"));
		Write(string, size, TEXT("C string"));
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(const CString& string)
	// Write the CString string into the archive file. Throw an exception
	// if an error occurs.
	{
		UINT size = (lstrlen(string.c_str()) + 1) * sizeof(TCHAR);
		  
		// Write() throws exception upon error
		Write((char *)&size, sizeof(size_t), TEXT("CString"));
		Write(string.c_str(), size, TEXT("CString"));
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(const CPoint& pt)
	// Write the CString string into the archive file. Throw an exception
	// if an error occurs.
	{
		UINT size = sizeof(pt);
		  
		// Write() throws exception upon error
		Write((char *)&size, sizeof(size_t), TEXT("CPoint"));
		Write((char *)&pt, size, TEXT("CPoint"));
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator<<(const ArchiveObject& ob)
	// Write arbitrary CArchive object into this CArchive. Only ob.size  and
	// pointer ob.p to location are given. Throw an excepton if unable
	// to do so successfully.
	{
		Write((char *)&ob.size, sizeof(size_t), ob.msg);
		  
		// Write() throws exception upon error
		Write(ob.p, ob.size, ob.msg);
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(BYTE& b)
	// Read a BYTE from the archive and  store it in b.  Throw an exception if
	// unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(b), &b, TEXT("byte")};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(WORD& w)
	// Read a WORD from the archive and  store it in w.  Throw an exception if
	// unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(w), &w, TEXT("WORD")};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(LONG& l)
	// Read a LONG from the archive and  store it in l.  Throw an exception if
	// unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(l), &l, TEXT("long")};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(DWORD& dw)
	// Read a DWORD	 from the archive and  store it in dw.  Throw an
	// exception if unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(dw), &dw, TEXT("DWORD")};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(float& f)
	// Read a float from the archive and  store it in f.  Throw an exception if
	// unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(f), &f, TEXT("float")};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(double& d)
	// Read a double from the archive and  store it in d.  Throw an exception if
	// unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(d), &d, TEXT("double")};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(int& i)
	// Read an int from the archive and  store it in i.  Throw an exception if
	// unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(i), &i, TEXT("int")};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(short& i)
	// Read a short from the archive and  store it in i.  Throw an exception if
	// unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(i), &i, TEXT("short")};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(char& c)
	// Read a char from the archive and  store it in c.  Throw an exception if
	// unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(c), &c, TEXT("char")};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(unsigned& u)
	// Read an unsigned int from the archive and  store it in u.  Throw an
	// exception if unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(u), &u, TEXT("unsigned")};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(LPTSTR string)
	// Read a TCHAR string from the archive and  store it in string.  Throw
	// an exception if unable to do so correctly. Note: exceptions are thrown
	// only on inability to read the recorded number of chars from the archive
	// stream.
	{
		UINT size;
		Read((char *)&size, sizeof(size_t), TEXT("C string"));
		Read(string, size, TEXT("C string"));
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(CString& string)
	// Read a CString from the archive and  store it in string.  Throw an
	// exception if unable to do so correctly. Note: exceptions are thrown
	// only on inability to read the recorded number of chars from the archive
	// stream.
	{
		UINT size;
		Read((char *)&size, sizeof(size_t));
		  // A TCHAR buffer is needed to receive the stored chars in order to
		  // be assigned to a CString in wide character mode. The sizing below
		  // may be more than needed, but its life is short.
		TCHAR *buffer = new TCHAR[size];
		Read(buffer, size);
		string = buffer;
		delete [] buffer;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(CPoint& pt)
	// Read a CPoint from the archive and  store it in string.  Throw an
	// exception if unable to do so correctly.
	{
		ArchiveObject ob = {sizeof(pt), &pt, TEXT("CPoint")};
		*this >> ob;
		return *this;
	}

	//============================================================================
	inline CArchive& CArchive::operator>>(ArchiveObject& ob)
	// Read a char string of size ob.size from the archive and  store it in
	// the location pointed to by ob.p.  Throw an exception if unable to
	// do so correctly.
	{
		UINT size;
		Read((char *)&size, sizeof(size_t), ob.msg);
		if (size != ob.size)
		{
			m_nError = ERROR_INVALID_DATA;
			CString e;
			e.Format(TEXT("CArchive corruption reading: %s"), ob.msg);
			throw CWinException(e.c_str());
		}
		
		Read(ob.p, ob.size, ob.msg);
		return *this;
	}


	// Global functions
	//============================================================================
	inline CString	LastErrorMessage()
	// Return the string currently in the last error message buffer.
	{
		DWORD err = ::GetLastError();
		
		// for some reason, this always returns err = 126 when
		// no error has been committed.  This module hides that
		// behavior, and  will then be in error if, in fact,
		// an 126 error actually occurs.
		if (err == 126)
			::SetLastError(err = 0);
		
		return SystemErrorMessage(err);
	}

	//===========================================================================
	inline CString	SystemErrorMessage(int errnum)
	// Return the string error message corresponding to the numeric errnum
	// returned by a DOS app.
	{
		LPTSTR s;
		if(::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM, NULL, errnum, 0, (LPTSTR)&s, 0,
			NULL) == 0)
		{
			// failed, unknown error code
			s = (LPTSTR)"Unknown error.";
		}
		else
		{
			// success, use TCHAR version of strchr to find any '\r'
			LPTSTR p = _tcschr(s, TEXT('\r'));
			if(p != NULL)
			{
				// lose it and  end the string
				*p = TEXT('\0');
			}
		}

		CString error;
		error.Format(TEXT("(%d) %s"), errnum, s);
		::LocalFree(s);
		return error;
	}

} // namespace Win32xx

#endif // _WIN32XX_ARCHIVE_H_
