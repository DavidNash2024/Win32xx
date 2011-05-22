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


#ifndef _WIN32XX_FILE_H_
#define _WIN32XX_FILE_H_


#include "wincore.h"

namespace Win32xx
{

	class CFile
	{
	public:
		CFile();
		CFile(HANDLE hFile);
		CFile(LPCTSTR pszFileName, UINT nOpenFlags);
		operator HANDLE() const;

		BOOL Close();
		BOOL Flush();
		ULONGLONG GetLength( ) const;
		const CString& GetFileName() const;
		const CString& GetFilePath() const;
		const CString& GetFileTitle() const;
		ULONGLONG GetPosition() const;
		BOOL LockRange(ULONGLONG Pos, ULONGLONG Count);
		BOOL Open(LPCTSTR pszFileName, UINT nOpenFlags);
		BOOL OpenFileDialog(LPCTSTR pszFilePathName = NULL,
						DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, LPCTSTR pszFilter = NULL,
						CWnd* pParentWnd = NULL);
		UINT Read(void* pBuf, UINT nCount);
		static BOOL Remove(LPCTSTR pszFileName);
		static BOOL Rename(LPCTSTR pszOldName, LPCTSTR pszNewName);
		BOOL SaveFileDialog(LPCTSTR pszFilePathName = NULL,
						DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, LPCTSTR pszFilter = NULL,
						CWnd* pParentWnd = NULL);
		ULONGLONG Seek(LONGLONG lOff, UINT nFrom);
		void SeekToBegin();
		ULONGLONG SeekToEnd();
		void SetFilePath(LPCTSTR pszNewName);
		BOOL SetLength(ULONGLONG NewLen);
		BOOL UnlockRange(ULONGLONG Pos, ULONGLONG Count);
		BOOL Write(const void* pBuf, UINT nCount);

	private:
		CString m_FileName;
		CString m_FilePath;
		CString m_FileTitle;
		HANDLE m_hFile;
	};

}



namespace Win32xx
{
	inline CFile::CFile() : m_hFile(0)
	{
	}

	inline CFile::CFile(HANDLE hFile) : m_hFile(hFile)
	{
	}

	inline CFile::CFile(LPCTSTR pszFileName, UINT nOpenFlags) : m_hFile(0)
	{
		assert(pszFileName);
		Open(pszFileName, nOpenFlags);
		assert(m_hFile);
	}

	inline CFile::operator HANDLE() const
	{
		return m_hFile;
	}

	inline BOOL CFile::Close()
	{
		BOOL bResult = TRUE;
		if (m_hFile)
			bResult = CloseHandle(m_hFile);

		return bResult;
	}

	inline BOOL CFile::Flush()
	{
		assert(m_hFile);
		return FlushFileBuffers(m_hFile);
	}

	inline ULONGLONG CFile::GetLength( ) const
	{
		assert (m_hFile);
		CFile File = CFile(m_hFile);
		ULONGLONG Result = File.SeekToEnd();

		return Result;
	}

	inline const CString& CFile::GetFileName() const
	{
		return (const CString&)m_FileName;
	}

	inline const CString& CFile::GetFilePath() const
	{
		return (const CString&)m_FilePath;
	}

	inline const CString& CFile::GetFileTitle() const
	{
		return (const CString&)m_FileTitle;
	}

	ULONGLONG CFile::GetPosition() const
	{
		assert(m_hFile);
		LONG High = 0;
		DWORD LowPos = SetFilePointer(m_hFile, 0, &High, FILE_CURRENT);

		ULONGLONG Result = ((ULONGLONG)High << 32) + LowPos;
		return Result;
	}

	inline BOOL CFile::LockRange(ULONGLONG Pos, ULONGLONG Count)
	{
		assert(m_hFile);

		DWORD dwPosHigh = (DWORD)(Pos >> 32);
		DWORD dwPosLow = (DWORD)(Pos & 0xFFFFFFFF);
		DWORD dwCountHigh = (DWORD)(Count >> 32);
		DWORD dwCountLow = (DWORD)(Count & 0xFFFFFFFF);

		return ::LockFile(m_hFile, dwPosLow, dwPosHigh, dwCountLow, dwCountHigh);
	}

	inline BOOL CFile::Open(LPCTSTR pszFileName, UINT nOpenFlags)
	{
		if (m_hFile) Close();

		m_hFile = ::CreateFile(pszFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, nOpenFlags, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == m_hFile)
		{
			TRACE(_T("Failed\n"));
			m_hFile = 0;
		}

		if (m_hFile)
		{
			SetFilePath(pszFileName);
		}

		return (m_hFile != 0);
	}

	inline BOOL CFile::OpenFileDialog(LPCTSTR pszFilePathName, DWORD dwFlags, LPCTSTR pszFilter, CWnd* pParentWnd)
	{
		BOOL bResult = FALSE;
		OPENFILENAME ofn = {0};

		ofn.lStructSize = sizeof(OPENFILENAME);

#if defined OPENFILENAME_SIZE_VERSION_400
		if (GetWinVersion() < 2500)
			ofn.lStructSize = OPENFILENAME_SIZE_VERSION_400;
#endif

		ofn.hwndOwner = pParentWnd? pParentWnd->GetHwnd() : NULL;
		ofn.hInstance = GetApp()->GetInstanceHandle();
		ofn.lpstrFilter = pszFilter;
		ofn.lpstrFile = (LPTSTR)pszFilePathName;
		ofn.nMaxFile = lstrlen(pszFilePathName);
		ofn.lpstrTitle = _T("Open File");
		ofn.Flags = dwFlags;

		if(::GetOpenFileName(&ofn))
			bResult = Open(ofn.lpstrFile, ofn.Flags);

		return bResult;
	}

	inline UINT CFile::Read(void* pBuf, UINT nCount)
	{
		assert(m_hFile);
		DWORD dwRead = 0;

		if (!::ReadFile(m_hFile, pBuf, nCount, &dwRead, NULL))
			dwRead = 0;

		return dwRead;
	}

	inline BOOL CFile::Rename(LPCTSTR pszOldName, LPCTSTR pszNewName)
	{
		return ::MoveFile(pszOldName, pszNewName);
	}

	inline BOOL CFile::Remove(LPCTSTR pszFileName)
	{
		return::DeleteFile(pszFileName);
	}

	inline BOOL CFile::SaveFileDialog(LPCTSTR pszFilePathName, DWORD dwFlags, LPCTSTR pszFilter, CWnd* pParentWnd)
	{
		BOOL bResult = FALSE;
		OPENFILENAME ofn = {0};
		ofn.lStructSize = sizeof(OPENFILENAME);

#if defined OPENFILENAME_SIZE_VERSION_400
		if (GetWinVersion() < 2500)
			ofn.lStructSize = OPENFILENAME_SIZE_VERSION_400;
#endif

		ofn.hwndOwner = pParentWnd? pParentWnd->GetHwnd() : NULL;
		ofn.hInstance = GetApp()->GetInstanceHandle();
		ofn.lpstrFilter = pszFilter;
		ofn.lpstrFile = (LPTSTR)pszFilePathName;
		ofn.nMaxFile = lstrlen(pszFilePathName);
		ofn.lpstrTitle = _T("Open File");
		ofn.Flags = dwFlags;

		if(::GetSaveFileName(&ofn))
			bResult = Open(ofn.lpstrFile, ofn.Flags);

		return bResult;
	}

	inline ULONGLONG CFile::Seek(LONGLONG lOff, UINT nFrom)
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
	{
		assert(m_hFile);
		Seek(0, FILE_BEGIN);
	}

	inline ULONGLONG CFile::SeekToEnd()
	{
		assert(m_hFile);
		Seek(0, FILE_END);
	}

	inline void CFile::SetFilePath(LPCTSTR pszFileName)
	{
		TCHAR* pFileName = NULL;
		int nBuffSize = ::GetFullPathName(pszFileName, NULL, 0, 0);
		if (nBuffSize > 0)
		{
			TCHAR* pBuff = m_FilePath.GetBuffer(nBuffSize);
			::GetFullPathName(pszFileName, nBuffSize, pBuff, &pFileName);
			m_FilePath.ReleaseBuffer();
			m_FileName = pFileName;
			int nPos = m_FileName.ReverseFind(_T("."));
			m_FileTitle = m_FileName.Left(nPos);
		}
	}

	inline BOOL CFile::SetLength(ULONGLONG NewLen)
	{
		assert(m_hFile);

		Seek(NewLen, FILE_BEGIN);
		return ::SetEndOfFile(m_hFile);
	}

	inline BOOL CFile::UnlockRange(ULONGLONG Pos, ULONGLONG Count)
	{
		assert(m_hFile);

		DWORD dwPosHigh = (DWORD)(Pos >> 32);
		DWORD dwPosLow = (DWORD)(Pos & 0xFFFFFFFF);
		DWORD dwCountHigh = (DWORD)(Count >> 32);
		DWORD dwCountLow = (DWORD)(Count & 0xFFFFFFFF);

		return ::UnlockFile(m_hFile, dwPosLow, dwPosHigh, dwCountLow, dwCountHigh);
	}

	inline BOOL CFile::Write(const void* pBuf, UINT nCount)
	{
		assert(m_hFile);
		DWORD dwWritten = 0;
		BOOL bResult = ::WriteFile(m_hFile, pBuf, nCount, &dwWritten, NULL);
		if (dwWritten != nCount)
			bResult = FALSE;

		return bResult;
	}


}	// namespace Win32xx

#endif
