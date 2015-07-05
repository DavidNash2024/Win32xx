// Win32++   Version 8.0
// Release Date: 5th July 2015
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


#include "wxx_wincore.h"

#ifndef _WIN32XX_FILE_H_
#define _WIN32XX_FILE_H_


namespace Win32xx
{

	class CFile
	{
	public:

		enum OpenFlags
		{
			modeCreate =        CREATE_ALWAYS,	// Creates a new file. Truncates existing file to length 0.
			modeNoTruncate =    OPEN_ALWAYS, // Creates a new file or opens an existing one.
			shareExclusive =    0x0010,	// Denies read and write access to all others.
			shareDenyWrite =    0x0020,	// Denies write access to all others.
			shareDenyRead =     0x0030,	// Denies read access to all others.
			shareDenyNone =     0x0040,	// No sharing restrictions.
			modeRead =          0x0100,	// Requests read access only.
			modeWrite =         0x0200,	// Requests write access only.
			modeReadWrite =     0x0300,	// Requests read and write access.	
		};
	
		CFile();
		CFile(HANDLE hFile);
		CFile(LPCTSTR pszFileName, UINT nOpenFlags);
		virtual ~CFile();
		operator HANDLE() const;

		virtual BOOL Close();
		virtual BOOL Flush();
		HANDLE GetHandle() const;
		virtual ULONGLONG GetLength() const;
		virtual const CString& GetFileName() const;
		virtual const CString& GetFilePath() const;
		virtual const CString& GetFileTitle() const;
		virtual ULONGLONG GetPosition() const;
		virtual BOOL Open(LPCTSTR pszFileName, UINT nOpenFlags);
		virtual UINT Read(void* pBuf, UINT nCount);
		static BOOL Remove(LPCTSTR pszFileName);
		static BOOL Rename(LPCTSTR pszOldName, LPCTSTR pszNewName);
		virtual ULONGLONG Seek(LONGLONG lOff, UINT nFrom);
		virtual void SeekToBegin();
		virtual ULONGLONG SeekToEnd();
		virtual BOOL SetLength(ULONGLONG NewLen);
		virtual void Write(const void* pBuf, UINT nCount);

#ifndef _WIN32_WCE
		virtual BOOL LockRange(ULONGLONG Pos, ULONGLONG Count);
		virtual CString OpenFileDialog(LPCTSTR pszFilePathName = NULL,
						DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, LPCTSTR pszTitle = NULL,
						LPCTSTR pszFilter = NULL, HWND hOwnerWnd = NULL);
		virtual CString SaveFileDialog(LPCTSTR pszFilePathName = NULL,
						DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, LPCTSTR pszTitle = NULL,
						LPCTSTR pszFilter = NULL, LPCTSTR pszDefExt = NULL, HWND hOwnerWnd = NULL);
		virtual void SetFilePath(LPCTSTR pszNewName);
		virtual BOOL UnlockRange(ULONGLONG Pos, ULONGLONG Count);
#endif

	private:
		CFile(const CFile&);				// Disable copy construction
		CFile& operator = (const CFile&);	// Disable assignment operator
		CString m_FileName;
		CString m_FilePath;
		CString m_FileTitle;
		HANDLE m_hFile;
	};

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace Win32xx
{
	inline CFile::CFile() : m_hFile(0)
	{
	}

	inline CFile::CFile(HANDLE hFile) : m_hFile(hFile)
	{
	}

	inline CFile::CFile(LPCTSTR pszFileName, UINT nOpenFlags) : m_hFile(0)
	// Possible nOpenFlag values: CREATE_NEW, CREATE_ALWAYS, OPEN_EXISTING, OPEN_ALWAYS, TRUNCATE_EXISTING
	// Default value: OPEN_EXISTING | modeReadWrite
	// 
	// The following modes are also supported:
	//	modeCreate		Creates a new file. Truncates an existing file to length 0.
	//	modeNoTruncate	Creates a a new file, or opens an existing one.
	//	modeRead		Requests read access only.
	//	modeWrite		Requests write access only.
	//	modeReadWrite	Requests read and write access.
	//	shareExclusive	Denies read and write access to all others.
	//	shareDenyWrite	Denies write access to all others.
	//	shareDenyRead	Denies read access to all others.
	//	shareDenyNone	No sharing restrictions.
	{
		assert(pszFileName);
		if (!Open(pszFileName, nOpenFlags))
			throw CWinException(_T("Failed to open file"));
	}

	inline CFile::~CFile()
	{
		Close();
	}

	inline CFile::operator HANDLE() const
	{
		return m_hFile;
	}

	inline BOOL CFile::Close()
	// Closes the file associated with this object. Closed file can no longer be read or written to.
	{
		BOOL bResult = TRUE;
		if (m_hFile != 0)
			bResult = CloseHandle(m_hFile);

		m_hFile = 0;
		return bResult;
	}

	inline BOOL CFile::Flush()
	// Causes any remaining data in the file buffer to be written to the file.
	{
		assert(m_hFile);
		return FlushFileBuffers(m_hFile);
	}

	inline HANDLE CFile::GetHandle() const
	{
		return m_hFile;
	}

	inline ULONGLONG CFile::GetLength( ) const
	// Returns the length of the file in bytes.
	{
		assert(m_hFile);

		LONG HighPosCur = 0;
		LONG HighPosEnd = 0;

		DWORD LowPosCur = SetFilePointer(m_hFile, 0, &HighPosCur, FILE_CURRENT);
		DWORD LowPosEnd = SetFilePointer(m_hFile, 0, &HighPosEnd, FILE_END);
		SetFilePointer(m_hFile, LowPosCur, &HighPosCur, FILE_BEGIN);

		ULONGLONG Result = ((ULONGLONG)HighPosEnd << 32) + LowPosEnd;
		return Result;
	}

	inline const CString& CFile::GetFileName() const
	// Returns the filename of the file associated with this object.
	{
		return (const CString&)m_FileName;
	}

	inline const CString& CFile::GetFilePath() const
	// Returns the full filename including the directory of the file associated with this object.
	{
		return (const CString&)m_FilePath;
	}

	inline const CString& CFile::GetFileTitle() const
	// Returns the filename of the file associated with this object, excluding the path and the file extension
	{
		return (const CString&)m_FileTitle;
	}

	inline ULONGLONG CFile::GetPosition() const
	// Returns the current value of the file pointer, which can be used in subsequent calls to Seek.
	{
		assert(m_hFile);
		LONG High = 0;
		DWORD LowPos = SetFilePointer(m_hFile, 0, &High, FILE_CURRENT);

		ULONGLONG Result = ((ULONGLONG)High << 32) + LowPos;
		return Result;
	}

#ifndef _WIN32_WCE
	inline BOOL CFile::LockRange(ULONGLONG Pos, ULONGLONG Count)
	// Locks a range of bytes in and open file.
	{
		assert(m_hFile);

		DWORD dwPosHigh = (DWORD)(Pos >> 32);
		DWORD dwPosLow = (DWORD)(Pos & 0xFFFFFFFF);
		DWORD dwCountHigh = (DWORD)(Count >> 32);
		DWORD dwCountLow = (DWORD)(Count & 0xFFFFFFFF);

		return ::LockFile(m_hFile, dwPosLow, dwPosHigh, dwCountLow, dwCountHigh);
	}
#endif

	inline BOOL CFile::Open(LPCTSTR pszFileName, UINT nOpenFlags)
	// Prepares a file to be written to or read from.
	// Possible nOpenFlag values: CREATE_NEW, CREATE_ALWAYS, OPEN_EXISTING, OPEN_ALWAYS, TRUNCATE_EXISTING
	// Default value: OPEN_EXISTING | modeReadWrite
	// 
	// The following modes are also supported:
	//	modeCreate		Creates a new file. Truncates an existing file to length 0.
	//	modeNoTruncate	Creates a a new file, or opens an existing one.
	//	modeRead		Requests read access only.
	//	modeWrite		Requests write access only.
	//	modeReadWrite	Requests read and write access.
	//	shareExclusive	Denies read and write access to all others.
	//	shareDenyWrite	Denies write access to all others.
	//	shareDenyRead	Denies read access to all others.
	//	shareDenyNone	No sharing restrictions.
	{
		if (m_hFile != 0) Close();

		DWORD dwAccess = 0;
		switch (nOpenFlags & 0xF)
		{
		case modeRead:			dwAccess = GENERIC_READ;	break;
		case modeWrite:			dwAccess = GENERIC_WRITE;	break;
		case modeReadWrite:		dwAccess = GENERIC_READ | GENERIC_WRITE; break;
		default:				dwAccess = GENERIC_READ | GENERIC_WRITE; break;
		}

		DWORD dwShare = 0;
		switch (nOpenFlags & 0xF0)
		{
		case shareExclusive:	dwShare = 0; break;
		case shareDenyWrite:	dwShare = FILE_SHARE_READ;	break;
		case shareDenyRead:		dwShare = FILE_SHARE_WRITE; break;
		case shareDenyNone:		dwShare = FILE_SHARE_READ | FILE_SHARE_WRITE; break;
		default:                dwShare = 0; break;
		}

		DWORD dwCreate = nOpenFlags & 0xF;
		if (dwCreate & OPEN_ALWAYS) nOpenFlags = OPEN_ALWAYS;
		if (dwCreate == 0) dwCreate = OPEN_EXISTING;

		m_hFile = ::CreateFile(pszFileName, dwAccess, dwShare, NULL, dwCreate, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == m_hFile)
		{
			m_hFile = 0;
		}

#ifndef _WIN32_WCE
		if (m_hFile != 0)
		{
			SetFilePath(pszFileName);
		}
#endif

		return (m_hFile != 0);
	}

#ifndef _WIN32_WCE
	inline CString CFile::OpenFileDialog(LPCTSTR pszFilePathName, DWORD dwFlags, LPCTSTR pszTitle, LPCTSTR pszFilter, HWND hOwnerWnd)
	// Displays the file open dialog.
	// Returns a CString containing either the selected file name or an empty CString.
	{
		CString str;
		if (pszFilePathName)
			str = pszFilePathName;

		int MaxPath = 260; // Should be a const int but VS6 can't handle it.

		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);

#if defined OPENFILENAME_SIZE_VERSION_400
		if (GetWinVersion() < 2500)
			ofn.lStructSize = OPENFILENAME_SIZE_VERSION_400;
#endif

		ofn.hwndOwner = hOwnerWnd;
		ofn.hInstance = GetApp().GetInstanceHandle();
		ofn.lpstrFilter = pszFilter;
		ofn.lpstrTitle = pszTitle? pszTitle : _T("Open File");
		ofn.Flags = dwFlags;
		ofn.nMaxFile = MaxPath;

		ofn.lpstrFile = (LPTSTR)str.GetBuffer(MaxPath);
		::GetOpenFileName(&ofn);
		str.ReleaseBuffer();

		return str;
	}
#endif

	inline UINT CFile::Read(void* pBuf, UINT nCount)
	// Reads from the file, storing the contents in the specified buffer.
	{
		assert(m_hFile);

		if (nCount == 0) return 0;

		assert(pBuf);
		DWORD dwRead = 0;

		if (!::ReadFile(m_hFile, pBuf, nCount, &dwRead, NULL))
			throw CWinException(_T("Failed to read from file"));

		return dwRead;
	}

	inline BOOL CFile::Rename(LPCTSTR pszOldName, LPCTSTR pszNewName)
	// Renames the specified file.
	{
		return ::MoveFile(pszOldName, pszNewName);
	}

	inline BOOL CFile::Remove(LPCTSTR pszFileName)
	// Deletes the specified file.
	{
		return ::DeleteFile(pszFileName);
	}

#ifndef _WIN32_WCE
	inline CString CFile::SaveFileDialog(LPCTSTR pszFilePathName, DWORD dwFlags, LPCTSTR pszTitle, LPCTSTR pszFilter, LPCTSTR pszDefExt, HWND hOwnerWnd)
	// Displays the SaveFileDialog.
	// Returns a CString containing either the selected file name or an empty CString
	{
		CString str;
		if (pszFilePathName)
			str = pszFilePathName;

		int MaxPath = 260; // Should be a const int but VS6 can't handle it.

		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);

#if defined OPENFILENAME_SIZE_VERSION_400
		if (GetWinVersion() < 2500)
			ofn.lStructSize = OPENFILENAME_SIZE_VERSION_400;
#endif

		ofn.hwndOwner = hOwnerWnd;
		ofn.hInstance = GetApp().GetInstanceHandle();
		ofn.lpstrFilter = pszFilter;
		ofn.lpstrFileTitle = (LPTSTR)pszFilePathName;
		ofn.lpstrDefExt = pszDefExt;
		ofn.lpstrTitle = pszTitle? pszTitle : _T("Save File");
		ofn.Flags = dwFlags;
		ofn.nMaxFile = MaxPath;
		ofn.lpstrFile = (LPTSTR)str.GetBuffer(MaxPath);
		::GetSaveFileName(&ofn);
		str.ReleaseBuffer();

		return str;
	}
#endif

	inline ULONGLONG CFile::Seek(LONGLONG lOff, UINT nFrom)
	// Positions the current file pointer.
	// Permitted values for nFrom are: FILE_BEGIN, FILE_CURRENT, or FILE_END.
	{
		assert(m_hFile);
		assert(nFrom == FILE_BEGIN || nFrom == FILE_CURRENT || nFrom == FILE_END);

		LONG High = LONG(lOff >> 32);
		LONG Low = (LONG)(lOff & 0xFFFFFFFF);

		DWORD LowPos = SetFilePointer(m_hFile, Low, &High, nFrom);

		ULONGLONG Result = ((ULONGLONG)High << 32) + LowPos;
		return Result;
	}

	inline void CFile::SeekToBegin()
	// Sets the current file pointer to the beginning of the file.
	{
		assert(m_hFile);
		Seek(0, FILE_BEGIN);
	}

	inline ULONGLONG CFile::SeekToEnd()
	// Sets the current file pointer to the end of the file.
	{
		assert(m_hFile);
		return Seek(0, FILE_END);
	}

#ifndef _WIN32_WCE
	inline void CFile::SetFilePath(LPCTSTR pszFileName)
	// Specifies the full file name, including its path
	{
		TCHAR* pFileName = NULL;
		int nBuffSize = ::GetFullPathName(pszFileName, 0, 0, 0);
		if (nBuffSize > 0)
		{
			::GetFullPathName(pszFileName, nBuffSize, m_FilePath.GetBuffer(nBuffSize), &pFileName);
			m_FilePath.ReleaseBuffer();
			m_FileName = pFileName;
			int nPos = m_FileName.ReverseFind(_T("."));
			if (nPos >= 0)
				m_FileTitle = m_FileName.Left(nPos);
		}
	}
#endif

	inline BOOL CFile::SetLength(ULONGLONG NewLen)
	// Changes the length of the file to the specified value.
	{
		assert(m_hFile);

		Seek(NewLen, FILE_BEGIN);
		return ::SetEndOfFile(m_hFile);
	}

#ifndef _WIN32_WCE
	inline BOOL CFile::UnlockRange(ULONGLONG Pos, ULONGLONG Count)
	// Unlocks a range of bytes in an open file.
	{
		assert(m_hFile);

		DWORD dwPosHigh = (DWORD)(Pos >> 32);
		DWORD dwPosLow = (DWORD)(Pos & 0xFFFFFFFF);
		DWORD dwCountHigh = (DWORD)(Count >> 32);
		DWORD dwCountLow = (DWORD)(Count & 0xFFFFFFFF);

		return ::UnlockFile(m_hFile, dwPosLow, dwPosHigh, dwCountLow, dwCountHigh);
	}
#endif

	inline void CFile::Write(const void* pBuf, UINT nCount)
	// Writes the specified buffer to the file.
	{
		assert(m_hFile);

		if (nCount == 0) return;
		
		assert(pBuf);
		DWORD dwWritten = 0;
		if (!::WriteFile(m_hFile, pBuf, nCount, &dwWritten, NULL))
			throw CWinException(_T("Failed to write to file"));
		
		if (dwWritten != nCount)
			throw CWinException(_T("Failed to write to file"));
	}


}	// namespace Win32xx

#endif
