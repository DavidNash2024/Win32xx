// Win32++   Version 8.5.1
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


#ifndef _WIN32XX_FILE_H_
#define _WIN32XX_FILE_H_


#include "wxx_cstring.h"
#include "wxx_exception.h"


namespace Win32xx
{

    /////////////////////////////////////
    // The CFile class manages files. It can be used to: create; read from;
    // write to; rename; and remove a file.
    class CFile
    {
    public:

        enum OpenFlags
        {
            modeCreate =        CREATE_ALWAYS,  // Creates a new file. Truncates existing file to length 0.
            modeNoTruncate =    OPEN_ALWAYS, // Creates a new file or opens an existing one.
            shareExclusive =    0x0010, // Denies read and write access to all others.
            shareDenyWrite =    0x0020, // Denies write access to all others.
            shareDenyRead =     0x0030, // Denies read access to all others.
            shareDenyNone =     0x0040, // No sharing restrictions.
            modeRead =          0x0100, // Requests read access only.
            modeWrite =         0x0200, // Requests write access only.
            modeReadWrite =     0x0300  // Requests read and write access.
        };

        CFile();
        CFile(HANDLE hFile);
        CFile(LPCTSTR pFileName, UINT openFlags);
        virtual ~CFile();
        operator HANDLE() const;

        virtual void Close();
        virtual void Flush();
        virtual CString GetFileDirectory() const;
        virtual const CString& GetFileName() const;
        virtual CString GetFileNameExt() const;
        virtual CString GetFileNameWOExt() const;
        virtual const CString& GetFilePath() const;
        virtual CString GetFileTitle() const;
        HANDLE GetHandle() const;
        virtual ULONGLONG GetLength() const;
        virtual ULONGLONG GetPosition() const;
        virtual void Open(LPCTSTR pFileName, UINT openFlags);
        virtual UINT Read(void* pBuf, UINT count);
        static void Remove(LPCTSTR pFileName);
        static void Rename(LPCTSTR pOldName, LPCTSTR pNewName);
        virtual ULONGLONG Seek(LONGLONG seekTo, UINT method);
        virtual void SeekToBegin();
        virtual ULONGLONG SeekToEnd();
        virtual void SetLength(ULONGLONG length);
        virtual void Write(const void* pBuf, UINT count);

#ifndef _WIN32_WCE
        virtual void LockRange(ULONGLONG pos, ULONGLONG count);
        virtual void SetFilePath(LPCTSTR pFileName);
        virtual void UnlockRange(ULONGLONG pos, ULONGLONG count);
#endif

    private:
        CFile(const CFile&);                // Disable copy construction
        CFile& operator = (const CFile&);   // Disable assignment operator
        CString m_fileName;
        CString m_filePath;
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


    // Possible nOpenFlag values: CREATE_NEW, CREATE_ALWAYS, OPEN_EXISTING, OPEN_ALWAYS, TRUNCATE_EXISTING
    // Default value: OPEN_EXISTING | modeReadWrite
    // The following modes are also supported:
    //  modeCreate      Creates a new file. Truncates an existing file to length 0.
    //  modeNoTruncate  Creates a a new file, or opens an existing one.
    //  modeRead        Requests read access only.
    //  modeWrite       Requests write access only.
    //  modeReadWrite   Requests read and write access.
    //  shareExclusive  Denies read and write access to all others.
    //  shareDenyWrite  Denies write access to all others.
    //  shareDenyRead   Denies read access to all others.
    //  shareDenyNone   No sharing restrictions.
    inline CFile::CFile(LPCTSTR pFileName, UINT openFlags) : m_hFile(0)

    {
        assert(pFileName);
        Open(pFileName, openFlags);  // throws CFileException on failure
    }


    inline CFile::~CFile()
    {
        if (m_hFile != 0)
            VERIFY(::CloseHandle(m_hFile));
    }


    inline CFile::operator HANDLE() const
    {
        return m_hFile;
    }


    // Closes the file associated with this object. Closed file can no longer be read or written to.
    inline void CFile::Close()
    {

        if (m_hFile != 0)
        {
            if (!::CloseHandle(m_hFile))
            {
                m_hFile = 0;
                throw CFileException(GetFilePath(), _T("Failed to close file"));
            }
        }

        m_hFile = 0;
    }


    // Causes any remaining data in the file buffer to be written to the file.
    inline void CFile::Flush()
    {
        assert(m_hFile);
        if ( !::FlushFileBuffers(m_hFile))
            throw CFileException(GetFilePath(), _T("Failed to flush file"));
    }


    // Returns the file handle associated with this object.
    inline HANDLE CFile::GetHandle() const
    {
        return m_hFile;
    }


    // Returns the directory of the file associated with this object.
    inline CString CFile::GetFileDirectory() const
    {
        CString directory;

        int sep = m_filePath.ReverseFind(_T("\\"));
        if (sep > 0)
            directory = m_filePath.Left(sep);

        return directory;
    }


    // Returns the filename of the file associated with this object, not including the directory.
    inline const CString& CFile::GetFileName() const
    {
        return m_fileName;
    }


    // Returns the extension part of the filename of the file associated with this object.
    inline CString CFile::GetFileNameExt() const
    {
        CString extension;

        int dot = m_fileName.ReverseFind(_T("."));
        if (dot > 0)
            extension = m_fileName.Mid(dot+1, lstrlen(m_fileName));

        return extension;
    }


    // Returns the filename of the file associated with this object, not including the directory, without its extension.
    inline CString CFile::GetFileNameWOExt() const
    {
        CString fileNameWOExt = m_fileName;

        int dot = m_fileName.ReverseFind(_T("."));
        if (dot > 0)
            fileNameWOExt = m_fileName.Left(dot);

        return fileNameWOExt;
    }


    // Returns the full filename including the directory of the file associated with this object.
    inline const CString& CFile::GetFilePath() const
    {
        return m_filePath;
    }


    // Returns the string that the system would use to display the file name to
    // the user. The string might or might not contain the filename's extension
    // depending on user settings.
    inline CString CFile::GetFileTitle() const
    {
        CString fileTitle;
        int buffSize = m_filePath.GetLength();
        if (buffSize > 0)
        {
            ::GetFileTitle(m_filePath, fileTitle.GetBuffer(buffSize), static_cast<WORD>(buffSize));
            fileTitle.ReleaseBuffer();
        }

        return fileTitle;
    }


    // Returns the length of the file in bytes.
    inline ULONGLONG CFile::GetLength() const
    {
        assert(m_hFile);

        LONG highPosCur = 0;
        LONG highPosEnd = 0;

        DWORD lowPosCur = SetFilePointer(m_hFile, 0, &highPosCur, FILE_CURRENT);
        DWORD lowPosEnd = SetFilePointer(m_hFile, 0, &highPosEnd, FILE_END);
        SetFilePointer(m_hFile, lowPosCur, &highPosCur, FILE_BEGIN);

        ULONGLONG result = (static_cast<ULONGLONG>(highPosEnd) << 32) + lowPosEnd;
        return result;
    }


    // Returns the current value of the file pointer, which can be used in subsequent calls to Seek.
    inline ULONGLONG CFile::GetPosition() const
    {
        assert(m_hFile);
        LONG high = 0;
        DWORD lowPos = SetFilePointer(m_hFile, 0, &high, FILE_CURRENT);

        ULONGLONG result = (static_cast<ULONGLONG>(high) << 32) + lowPos;
        return result;
    }

#ifndef _WIN32_WCE

    // Locks a range of bytes in and open file.
    inline void CFile::LockRange(ULONGLONG pos, ULONGLONG count)
    {
        assert(m_hFile);

        DWORD posHigh = static_cast<DWORD>(pos >> 32);
        DWORD posLow = static_cast<DWORD>(pos & 0xFFFFFFFF);
        DWORD countHigh = static_cast<DWORD>(count >> 32);
        DWORD countLow = static_cast<DWORD>(count & 0xFFFFFFFF);

        if (!::LockFile(m_hFile, posLow, posHigh, countLow, countHigh))
            throw CFileException(GetFilePath(), _T("Failed to lock the file"));
    }

#endif

    // Prepares a file to be written to or read from.
    // Possible nOpenFlag values: CREATE_NEW, CREATE_ALWAYS, OPEN_EXISTING, OPEN_ALWAYS, TRUNCATE_EXISTING
    // Default value: OPEN_EXISTING | modeReadWrite
    // The following modes are also supported:
    //  modeCreate      Creates a new file. Truncates an existing file to length 0.
    //  modeNoTruncate  Creates a a new file, or opens an existing one.
    //  modeRead        Requests read access only.
    //  modeWrite       Requests write access only.
    //  modeReadWrite   Requests read and write access.
    //  shareExclusive  Denies read and write access to all others.
    //  shareDenyWrite  Denies write access to all others.
    //  shareDenyRead   Denies read access to all others.
    //  shareDenyNone   No sharing restrictions.
    inline void CFile::Open(LPCTSTR pFileName, UINT openFlags)
    {
        if (m_hFile != 0) Close();

        DWORD access = 0;
        switch (openFlags & 0xF00)
        {
        case modeRead:
            access = GENERIC_READ;    break;
        case modeWrite:
            access = GENERIC_WRITE;   break;
        case modeReadWrite:
            access = GENERIC_READ | GENERIC_WRITE; break;
        default:
            access = GENERIC_READ | GENERIC_WRITE; break;
        }

        DWORD share = 0;
        switch (openFlags & 0xF0)
        {
        case shareExclusive:    share = 0; break;
        case shareDenyWrite:    share = FILE_SHARE_READ;  break;
        case shareDenyRead:     share = FILE_SHARE_WRITE; break;
        case shareDenyNone:     share = FILE_SHARE_READ | FILE_SHARE_WRITE; break;
        default:                share = 0; break;
        }

        DWORD create = openFlags & 0xF;
        if (create & OPEN_ALWAYS) openFlags = OPEN_ALWAYS;
        if (create == 0) create = OPEN_EXISTING;

        m_hFile = ::CreateFile(pFileName, access, share, NULL, create, FILE_ATTRIBUTE_NORMAL, NULL);

        if (INVALID_HANDLE_VALUE == m_hFile)
        {
            m_hFile = 0;
            throw CFileException(pFileName, _T("Failed to open file"));
        }

#ifndef _WIN32_WCE
        if (m_hFile != 0)
        {
            SetFilePath(pFileName);
        }
#endif

    }


    // Reads from the file, storing the contents in the specified buffer.
    inline UINT CFile::Read(void* pBuf, UINT count)
    {
        assert(m_hFile);

        if (count == 0) return 0;

        assert(pBuf);
        DWORD read = 0;

        if (!::ReadFile(m_hFile, pBuf, count, &read, NULL))
            throw CFileException(GetFilePath(), _T("Failed to read from file"));

        return read;
    }


    // Renames the specified file.
    inline void CFile::Rename(LPCTSTR pOldName, LPCTSTR pNewName)
    {
        if (!::MoveFile(pOldName, pNewName))
            throw CFileException(pOldName, _T("Failed to rename file"));
    }


    // Deletes the specified file.
    inline void CFile::Remove(LPCTSTR pFileName)
    {
        if (!::DeleteFile(pFileName))
            throw CFileException(pFileName, _T("Failed to delete file"));
    }


    // Positions the current file pointer.
    // Permitted values for method are: FILE_BEGIN, FILE_CURRENT, or FILE_END.
    inline ULONGLONG CFile::Seek(LONGLONG seekTo, UINT method)
    {
        assert(m_hFile);
        assert(method == FILE_BEGIN || method == FILE_CURRENT || method == FILE_END);

        LONG high = LONG(seekTo >> 32);
        LONG low = (LONG)(seekTo & 0xFFFFFFFF);

        DWORD lowPos = SetFilePointer(m_hFile, low, &high, method);

        ULONGLONG result = (static_cast<ULONGLONG>(high) << 32) + lowPos;
        return result;
    }


    // Sets the current file pointer to the beginning of the file.
    inline void CFile::SeekToBegin()
    {
        assert(m_hFile);
        Seek(0, FILE_BEGIN);
    }


    // Sets the current file pointer to the end of the file.
    inline ULONGLONG CFile::SeekToEnd()
    {
        assert(m_hFile);
        return Seek(0, FILE_END);
    }

#ifndef _WIN32_WCE

    // Assigns the specified full file path to this object.
    // Call this function if the file path is not supplied when the CFile is constructed.
    inline void CFile::SetFilePath(LPCTSTR pFileName)
    {
        LPTSTR pShortFileName = NULL;

        int buffSize = ::GetFullPathName(pFileName, 0, 0, 0);
        if (buffSize > 0)
        {
            ::GetFullPathName(pFileName, buffSize, m_filePath.GetBuffer(buffSize), &pShortFileName);

            if (pShortFileName != NULL)
                m_fileName = pShortFileName;
            else
                m_fileName = _T("");

            m_filePath.ReleaseBuffer();
        }
    }

#endif


    // Changes the length of the file to the specified value.
    inline void CFile::SetLength(ULONGLONG length)
    {
        assert(m_hFile);

        Seek(length, FILE_BEGIN);
        if (!::SetEndOfFile(m_hFile))
            throw CFileException(GetFilePath(), _T("Failed to change the file length"));
    }

#ifndef _WIN32_WCE

    // Unlocks a range of bytes in an open file.
    inline void CFile::UnlockRange(ULONGLONG pos, ULONGLONG count)
    {
        assert(m_hFile);

        DWORD posHigh = static_cast<DWORD>(pos >> 32);
        DWORD posLow = static_cast<DWORD>(pos & 0xFFFFFFFF);
        DWORD countHigh = static_cast<DWORD>(count >> 32);
        DWORD countLow = static_cast<DWORD>(count & 0xFFFFFFFF);

        if (!::UnlockFile(m_hFile, posLow, posHigh, countLow, countHigh))
            throw CFileException(GetFilePath(), _T("Failed to unlock the file"));
    }

#endif


    // Writes the specified buffer to the file.
    inline void CFile::Write(const void* pBuf, UINT count)
    {
        assert(m_hFile);

        if (count == 0) return;

        assert(pBuf);
        DWORD written = 0;
        if (!::WriteFile(m_hFile, pBuf, count, &written, NULL))
            throw CFileException(GetFilePath(), _T("Failed to write to file"));

        if (written != count)
            throw CFileException(GetFilePath(), _T("Failed to write to file"));
    }


}   // namespace Win32xx

#endif
