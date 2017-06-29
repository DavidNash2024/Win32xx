// Win32++   Version 8.4.1
// Release Date: TBA
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2017  David Nash
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
// The original author of CFileFind is:
//
//      Robert C. Tausworthe
//      email: robert.c.tausworthe@ieee.org
//
////////////////////////////////////////////////////////


////////////////////////////////////////////////////////
// The CFileFind provides a means of finding one or more
// files that match a search string. Information can be
// extracted from each file found. For example:
//
//
//	CFileFind ff;
//	if (ff.FindFirstFile(_T("C:\\SomeFolder\\*.*")))
//	{
//		do
//		{
//	 	   	// Do something with each file found
//			std::cout << ff.GetFilePath() << std::endl;			
//		}
//		while (ff.FindNextFile());
//	}
//


#ifndef _WIN32XX_FILEFIND_H_
#define _WIN32XX_FILEFIND_H_

#include "wxx_appcore0.h"
#include "wxx_cstring.h"


namespace Win32xx
{

	/////////////////////////////////////
	// Declaration of the CFindFile class
	//
	class CFileFind
	{
		public:
			CFileFind();
			virtual ~CFileFind();

			virtual BOOL		FindFirstFile(LPCTSTR pstrName = NULL);
			virtual BOOL		FindNextFile();
			virtual FILETIME	GetCreationTime() const;
			virtual	DWORD		GetFileAttributes() const;
			virtual CString		GetFileName() const;
			virtual CString		GetFilePath() const;
			virtual CString		GetFileTitle() const;
			virtual CString		GetFileURL() const;
			virtual FILETIME	GetLastAccessTime() const;
			virtual FILETIME	GetLastWriteTime() const;
			virtual	ULONGLONG	GetLength() const;
			virtual CString		GetRoot() const;
			virtual BOOL		IsArchived() const;
			virtual BOOL		IsCompressed() const;
			virtual BOOL 		IsDirectory() const;
			virtual BOOL		IsDots() const;
			virtual BOOL		IsHidden() const;
			virtual BOOL		IsNormal() const;
			virtual BOOL		IsReadOnly() const;
			virtual BOOL		IsSystem() const;
			virtual BOOL		IsTemporary() const;

		private:
			void 	Close();

			WIN32_FIND_DATA m_FindData;
			HANDLE 		m_hFileFind;
			CString		m_strRoot;
	};

}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{
	
	//////////////////////////////////////
	// Definitions for the CFindFile class
	//	

	//============================================================================
	inline CFileFind::CFileFind()
	//	Construct a CFileFind object for file searches.
	{
		m_hFileFind = INVALID_HANDLE_VALUE;
	}


	//============================================================================
	inline CFileFind::~CFileFind()
	{
		Close();
	}


	//============================================================================
	inline void CFileFind::Close()
	//	Closes the FileFind handle and returns the CFileFind object to default.
	{
		ZeroMemory(&m_FindData, sizeof(m_FindData));

		if (m_hFileFind != INVALID_HANDLE_VALUE)
			::FindClose(m_hFileFind);

		m_strRoot.Empty();
		m_hFileFind = INVALID_HANDLE_VALUE;
	}


	//============================================================================
	inline BOOL CFileFind::FindFirstFile(LPCTSTR pstrName /* = NULL */)
	//	Searches a directory for a file or subdirectory with a name that matches
	//	the pstrName. pstrName can contain a valid directory or path, and a
	//	file name that can contain the '?' and '*' wildcard characters.
	//	A name of "*.*" is used if no name is specified.
	//	Returns TRUE if a matching file was found, FALSE otherwise.
	{
		// Reset the this to default
		Close();

		// Default the name to all files in the current directory
		if (pstrName == NULL)
			pstrName = _T("*.*");
		
		// Search for the first file matching the name
		m_hFileFind = ::FindFirstFile(pstrName, &m_FindData);

		if (m_hFileFind == INVALID_HANDLE_VALUE)
		{
			Close();
			return FALSE;
		}
		
		// extract the directory part of the name (if any)
		CString str = pstrName;
		int delimiter = str.ReverseFind(_T("\\"));
		if (delimiter >= 0)
		{
			m_strRoot = str.Left(delimiter);
			m_strRoot += _T('\\');
		}

		return TRUE;
	}


	//============================================================================
	inline BOOL CFileFind::FindNextFile()
	//	Finds the next file or directory that matches the string specified in FindFirstFile.
	//	Return TRUE if the next file was found, FALSE on failure.
	{
		assert(m_hFileFind != INVALID_HANDLE_VALUE);

		BOOL IsFound = ::FindNextFile(m_hFileFind, &m_FindData);
		
		if (!IsFound)
			Close();

		return IsFound;
	}


	//============================================================================
	inline FILETIME CFileFind::GetCreationTime() const
	// Return the found file's creation time
	{
		assert(m_hFileFind != INVALID_HANDLE_VALUE);
		return m_FindData.ftCreationTime;
	}


	//============================================================================
	inline DWORD CFileFind::GetFileAttributes() const
	//	Returns the found file's attributes. Possible attributes are:
	//	FILE_ATTRIBUTE_ARCHIVE; FILE_ATTRIBUTE_COMPRESSED; FILE_ATTRIBUTE_DIRECTORY;
	//	FILE_ATTRIBUTE_ENCRYPTED; FILE_ATTRIBUTE_HIDDEN; FILE_ATTRIBUTE_NORMAL;
	//	FILE_ATTRIBUTE_OFFLINE; FILE_ATTRIBUTE_READONLY; FILE_ATTRIBUTE_REPARSE_POINT;
	//	FILE_ATTRIBUTE_SPARSE_FILE; FILE_ATTRIBUTE_SYSTEM; FILE_ATTRIBUTE_TEMPORARY.
	{
		assert(m_hFileFind != INVALID_HANDLE_VALUE);	
		return m_FindData.dwFileAttributes;
	}


	//============================================================================
	inline CString CFileFind::GetFileName() const
	//	Return the found file's name, including the file's extension.
	{
		assert(m_hFileFind != INVALID_HANDLE_VALUE);
		return m_FindData.cFileName;
	}


	//============================================================================
	inline CString CFileFind::GetFilePath() const
	//  Returns the full path of the found file, including the directory, 
	//  file title, and extension. 
	{
		assert(m_hFileFind != INVALID_HANDLE_VALUE);	

		CString SearchName = m_strRoot + m_FindData.cFileName;
		CString FilePath;

		int nBuffSize = ::GetFullPathName(SearchName, 0, 0, 0);
		if (nBuffSize > 0)
		{
			::GetFullPathName(SearchName, nBuffSize, FilePath.GetBuffer(nBuffSize), 0);
			FilePath.ReleaseBuffer();
		}

		return FilePath;
	}


	//============================================================================
	inline CString CFileFind::GetFileTitle() const	
	//	Return the file name, without the extension.
	{
		assert(m_hFileFind != INVALID_HANDLE_VALUE);

		CString name = m_FindData.cFileName;
		if (!IsDots())
		{
			int dot = name.ReverseFind(_T("."));
			if (dot >= 0)
				name = name.Left(dot);
		}
		return name;
	}


	//============================================================================
	inline CString CFileFind::GetFileURL() const
	//	Return the URL form of the path name, viz., file://path
	{
		assert(m_hFileFind != INVALID_HANDLE_VALUE);

		CString strResult(_T("file://"));
		strResult += GetFilePath();
		return strResult;
	}


	//============================================================================
	inline FILETIME CFileFind::GetLastAccessTime() const
	//	Return the last access time of the found file
	{
		assert(m_hFileFind != INVALID_HANDLE_VALUE);
		return m_FindData.ftLastAccessTime;
	}


	//============================================================================
	inline FILETIME CFileFind::GetLastWriteTime() const
	//	Return the last write time of the found file
	{
		assert(m_hFileFind != INVALID_HANDLE_VALUE);
		return m_FindData.ftLastWriteTime;
	}


	//============================================================================
	inline ULONGLONG CFileFind::GetLength() const
	//	Return the length of the found file, in bytes.
	{
		assert(m_hFileFind != INVALID_HANDLE_VALUE);
		ULONGLONG length = m_FindData.nFileSizeHigh;
		length <<= 32;											
		return length | m_FindData.nFileSizeLow;
	}


	//============================================================================
	inline CString CFileFind::GetRoot() const
	//	Return the directory part of the name search string
	{
		assert(m_hFileFind != INVALID_HANDLE_VALUE);
		return m_strRoot;
	}


	//============================================================================
	inline BOOL CFileFind::IsArchived() const
	//	Return TRUE if the archive attribute is set
	{
		assert(m_hFileFind != INVALID_HANDLE_VALUE);
		return (m_FindData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0;
	}


	//============================================================================
	inline BOOL CFileFind::IsCompressed() const
	//	Return TRUE if the found file is compressed.
	{
		assert(m_hFileFind != INVALID_HANDLE_VALUE);
		return (m_FindData.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED) != 0;
	}


	//============================================================================
	inline BOOL CFileFind::IsDirectory() const
	//	Return TRUE if the found file is a directory.
	{
		assert(m_hFileFind != INVALID_HANDLE_VALUE);
		return (m_FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
	}


	//============================================================================
	inline BOOL CFileFind::IsDots() const
	//	Return TRUE if the found file is the current directory or parent directory.
	{
		assert(m_hFileFind != INVALID_HANDLE_VALUE);
		LPCTSTR fn = m_FindData.cFileName;
		return (IsDirectory() && (fn[0] == _T('.')) && ((fn[1] == _T('\0') ||
			(fn[1] == _T('.') && fn[2] == _T('\0')))));
	}


	//============================================================================
	inline BOOL CFileFind::IsHidden() const
	//	Return TRUE if the found file is hidden.
	{
		assert(m_hFileFind != INVALID_HANDLE_VALUE);
		return (m_FindData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0;
	}


	//============================================================================
	inline BOOL CFileFind::IsNormal() const
	//	Return TRUE if the found file has the FILE_ATTRIBUTE_NORMAL attribute.
	{
		assert(m_hFileFind != INVALID_HANDLE_VALUE);
		return (m_FindData.dwFileAttributes & FILE_ATTRIBUTE_NORMAL) != 0;
	}


	//============================================================================
	inline BOOL CFileFind::IsReadOnly() const
	//	Return TRUE if the found file is read only.
	{
		assert(m_hFileFind != INVALID_HANDLE_VALUE);
		return (m_FindData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0;
	}


	//============================================================================
	inline BOOL CFileFind::IsSystem() const
	//	Return TRUE if the found file is a system file.
	{
		assert(m_hFileFind != INVALID_HANDLE_VALUE);
		return (m_FindData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0;
	}


	//============================================================================
	inline BOOL CFileFind::IsTemporary() const
	//	Return TRUE if the found file is a temporary file.
	{
		assert(m_hFileFind != INVALID_HANDLE_VALUE);
		return (m_FindData.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY) != 0;
	}


}


#endif // _WIN32XX_FILEFIND_H_
