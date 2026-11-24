// Win32++   Version 10.4.0
// Release Date: TBA
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//           https://github.com/DavidNash2024/Win32xx
//
//
// Copyright (c) 2005-2026  David Nash
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
// Developed from code originally provided by:
//
//      Robert C. Tausworthe
//      email: robert.c.tausworthe@ieee.org
//
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
//  Contents Description:
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


#pragma once

#include "wxx_wincore.h"
#include <filesystem>
#include <fstream>


namespace Win32xx
{
    // An object used for serialization that can hold any type of data.
    // Specify a pointer to the data, and the size of the data in bytes.
    struct ArchiveObject
    {
        // member variables
        UINT m_size;
        LPVOID m_pData;

        // Constructor
        ArchiveObject(LPVOID pData, UINT size) : m_size(size), m_pData(pData) {}
    };

    // A typical usage of ArchiveObject would be of the form:
    //
    //  ArchiveObject ao(&Data, sizeof(Data));
    //  ar << ao; or ar >> ao;


    //////////////////////////////////////////////////////////////
    // CArchive serializes data to and from a file archive.
    // CArchive uses the >> and << operator overloads to serialize
    // the various data types to the archive.
    class CArchive
    {
        using StreamPtr = std::unique_ptr<std::fstream>;

    public:
        // Archive modes
        enum Mode { store = 0, load = 1 };

        // Construction and destruction
        CArchive(std::iostream& stream, Mode mode, LPCTSTR fileName = _T(""));
        CArchive(LPCTSTR fileName, Mode mode);
        virtual ~CArchive();

        // Method members
        std::iostream& GetStream() const;
        CString GetFilePath() const;
        UINT    GetObjectSchema() const;
        bool    IsLoading() const;
        bool    IsStoring() const;
        void    Read(void* buffer, UINT size);
        LPTSTR  ReadString(LPTSTR string, UINT max);
        LPSTR   ReadStringA(LPSTR string, UINT max);
        LPWSTR  ReadStringW(LPWSTR string, UINT max);
        void    SetObjectSchema(UINT schema);
        void    Write(const void* buffer, UINT size);
        void    WriteString(LPCTSTR string);
        void    WriteStringA(LPCSTR string);
        void    WriteStringW(LPCWSTR string);

        // Store operations (<< pushes data into the archive)
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
        CArchive& operator<<(const CStringA& string);
        CArchive& operator<<(const CStringW& string);
        CArchive& operator<<(const CString& string);
        CArchive& operator<<(POINT pt);
        CArchive& operator<<(RECT rc);
        CArchive& operator<<(SIZE sz);
        CArchive& operator<<(const ArchiveObject& ao);
        CArchive& operator<<(const CObject& object);
        CArchive& operator<<(wchar_t ch);

        // Load operations (>> extracts data out of the archive)
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
        CArchive& operator>>(CStringA& string);
        CArchive& operator>>(CStringW& string);
        CArchive& operator>>(CString& string);
        CArchive& operator>>(POINT& pt);
        CArchive& operator>>(RECT& rc);
        CArchive& operator>>(SIZE& sz);
        CArchive& operator>>(ArchiveObject& ao);
        CArchive& operator>>(CObject& object);
        CArchive& operator>>(wchar_t& ch);

    private:
        CArchive(const CArchive&) = delete;
        CArchive& operator=(const CArchive&) = delete;

        ULONGLONG GetRemainingBytes() const;

        // Private data members
        std::iostream* m_pStream;   // pointer to the archive stream
        StreamPtr      m_stream;    // unique_ptr to locally created std::fstream
        CString        m_filePath;  // stored file path for CFileException
        UINT           m_schema;    // archive version schema
        bool           m_isStoring; // archive direction switch
    };

} // namespace Win32xx

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace Win32xx
{

    //////////////////////////////////////
    // Definitions for the CArchive class.
    //

    // Constructs a CArchive object from an unowned std::iostream reference.
    // The specified stream must already be open in std::ios::binary mode.
    inline CArchive::CArchive(std::iostream& stream, Mode mode, LPCTSTR fileName)
        : m_pStream(&stream), m_filePath(fileName ? fileName : _T("")),
          m_schema(UINT(-1)), m_isStoring(mode == store)
    {
        if (!m_pStream->good())
            throw CFileException(m_filePath, GetApp()->MsgArReadFail());
    }

    // Constructs an owned CArchive object backed by std::fstream.
    // A file with the specified name is created for storing (if required), and
    // also opened. A failure to open the file will throw CFileException.
    inline CArchive::CArchive(LPCTSTR fileName, Mode mode)
        : m_pStream(nullptr), m_filePath(fileName ? fileName : _T("")),
          m_schema(UINT(-1)), m_isStoring(mode == store)
    {
        std::ios::openmode openMode = std::ios::binary;
        openMode |= (mode == load) ? std::ios::in : (std::ios::out | std::ios::trunc);

        m_stream = std::make_unique<std::fstream>(std::filesystem::path(fileName), openMode);
        if (!m_stream->is_open())
        {
            const DWORD err = ::GetLastError();
            throw CFileException(m_filePath, err ? err : ERROR_OPEN_FAILED);
        }

        m_pStream = m_stream.get();
    }

    // Destructor.
    inline CArchive::~CArchive()
    {
        if (m_stream && m_stream->is_open())
        {
            if (IsStoring())
                m_stream->flush();

            m_stream->close();
        }
    }

    // Returns the stream associated with the archive.
    inline std::iostream& CArchive::GetStream() const
    {
        assert(m_pStream);
        return *m_pStream;
    }

    // Returns the file path associated with the archive.
    inline CString CArchive::GetFilePath() const
    {
        return m_filePath;
    }

    // Returns the archived data schema.
    inline UINT CArchive::GetObjectSchema() const
    {
        return m_schema;
    }

    // Returns true if the archive is being loaded.
    inline bool CArchive::IsLoading() const
    {
        return !m_isStoring;
    }

    // Returns true if the archive is being stored.
    inline bool CArchive::IsStoring() const
    {
        return m_isStoring;
    }

    // Helper to determine unread bytes remaining in the input stream.
    inline ULONGLONG CArchive::GetRemainingBytes() const
    {
        assert(m_pStream);
        const std::streampos currentPos = m_pStream->tellg();
        if (currentPos == std::streampos(-1))
            return 0;

        m_pStream->seekg(0, std::ios::end);
        const std::streampos endPos = m_pStream->tellg();
        m_pStream->seekg(currentPos);

        if (endPos == std::streampos(-1) || endPos < currentPos)
            return 0;

        return static_cast<ULONGLONG>(endPos - currentPos);
    }

    // Reads size bytes from the open archive stream into the given buffer.
    // Throws CFileException if not successful.
    inline void CArchive::Read(void* buffer, UINT size)
    {
        assert(m_pStream);
        if (size == 0)
            return;

        if (m_pStream)
        {
            m_pStream->read(static_cast<char*>(buffer), static_cast<std::streamsize>(size));
            if (!m_pStream->good() || m_pStream->gcount() != static_cast<std::streamsize>(size))
                throw CFileException(m_filePath, GetApp()->MsgArReadFail());
        }
    }

    // Reads at most max-1 TCHAR characters from the archive and stores it
    // in a string.
    inline LPTSTR CArchive::ReadString(LPTSTR string, UINT max)
    {
        assert(max > 0);

        CString str;
        *this >> str;
        StrCopy(string, str.c_str(), max);
        return string;
    }

    // Reads at most max-1 CHAR characters from the archive and stores it
    // in a string.
    inline LPSTR CArchive::ReadStringA(LPSTR string, UINT max)
    {
        assert(max > 0);

        CStringA str;
        *this >> str;
        StrCopyA(string, str.c_str(), max);
        return string;
    }

    // Reads at most max-1 WCHAR characters from the archive and stores it
    // in a string.
    inline LPWSTR CArchive::ReadStringW(LPWSTR string, UINT max)
    {
        assert(max > 0);

        CStringW str;
        *this >> str;
        StrCopyW(string, str.c_str(), max);
        return string;
    }

    // Records the archived data schema number.
    inline void CArchive::SetObjectSchema(UINT schema)
    {
        m_schema = schema;
    }

    // Writes size bytes from the buffer into the open archive stream.
    // Throws CFileException if unsuccessful.
    inline void CArchive::Write(const void* buffer, UINT size)
    {
        assert(m_pStream);
        if (size == 0)
            return;

        if (m_pStream)
        {
            m_pStream->write(static_cast<const char*>(buffer), static_cast<std::streamsize>(size));
            if (!m_pStream->good())
            {
                const DWORD err = ::GetLastError();
                throw CFileException(m_filePath, err ? err : ERROR_WRITE_FAULT);
            }
        }
    }

    // Writes the LPCTSTR string into the archive stream.
    inline void CArchive::WriteString(LPCTSTR string)
    {
        int chars = static_cast<int>(_tcslen(string));
        bool isUnicode = (sizeof(TCHAR) == sizeof(WCHAR));

        // Store the Unicode state and number of characters in the archive.
        *this << isUnicode;
        *this << chars;

        Write(string, chars * sizeof(TCHAR));
    }

    // Writes the LPCSTR string into the archive stream.
    inline void CArchive::WriteStringA(LPCSTR string)
    {
        int chars = static_cast<int>(std::strlen(string));
        bool isUnicode = false;

        // Store the Unicode state and number of characters in the archive.
        *this << isUnicode;
        *this << chars;

        Write(string, chars * sizeof(CHAR));
    }

    // Writes the LPCWSTR string into the archive stream.
    inline void CArchive::WriteStringW(LPCWSTR string)
    {
        int chars = static_cast<int>(std::wcslen(string));
        bool isUnicode = true;

        *this << isUnicode;
        *this << chars;

        Write(string, chars * sizeof(WCHAR));
    }

    // Store operators (<<)

    inline CArchive& CArchive::operator<<(BYTE b)
    {
        ArchiveObject ob(&b, sizeof(b));
        *this << ob;
        return *this;
    }

    inline CArchive& CArchive::operator<<(WORD w)
    {
        ArchiveObject ob(&w, sizeof(w));
        *this << ob;
        return *this;
    }

    inline CArchive& CArchive::operator<<(LONG l)
    {
        ArchiveObject ob(&l, sizeof(l));
        *this << ob;
        return *this;
    }

    inline CArchive& CArchive::operator<<(LONGLONG ll)
    {
        ArchiveObject ob(&ll, sizeof(ll));
        *this << ob;
        return *this;
    }

    inline CArchive& CArchive::operator<<(ULONGLONG ull)
    {
        ArchiveObject ob(&ull, sizeof(ull));
        *this << ob;
        return *this;
    }

    inline CArchive& CArchive::operator<<(DWORD dw)
    {
        ArchiveObject ob(&dw, sizeof(dw));
        *this << ob;
        return *this;
    }

    inline CArchive& CArchive::operator<<(float f)
    {
        ArchiveObject ob(&f, sizeof(f));
        *this << ob;
        return *this;
    }

    inline CArchive& CArchive::operator<<(double d)
    {
        ArchiveObject ob(&d, sizeof(d));
        *this << ob;
        return *this;
    }

    inline CArchive& CArchive::operator<<(int i)
    {
        ArchiveObject ob(&i, sizeof(i));
        *this << ob;
        return *this;
    }

    inline CArchive& CArchive::operator<<(short s)
    {
        ArchiveObject ob(&s, sizeof(s));
        *this << ob;
        return *this;
    }

    inline CArchive& CArchive::operator<<(char c)
    {
        ArchiveObject ob(&c, sizeof(c));
        *this << ob;
        return *this;
    }

    inline CArchive& CArchive::operator<<(wchar_t ch)
    {
        ArchiveObject ob(&ch, sizeof(ch));
        *this << ob;
        return *this;
    }

    inline CArchive& CArchive::operator<<(unsigned u)
    {
        ArchiveObject ob(&u, sizeof(u));
        *this << ob;
        return *this;
    }

    inline CArchive& CArchive::operator<<(bool b)
    {
        ArchiveObject ob(&b, sizeof(b));
        *this << ob;
        return *this;
    }

    inline CArchive& CArchive::operator<<(const CStringA& string)
    {
        int chars = string.GetLength();
        bool isUnicode = false;

        // Store the Unicode state and number of characters in the archive.
        *this << isUnicode;
        *this << chars;

        Write(string.c_str(), chars * sizeof(CHAR));
        return *this;
    }

    inline CArchive& CArchive::operator<<(const CStringW& string)
    {
        int chars = string.GetLength();
        bool isUnicode = true;

        // Store the Unicode state and number of characters in the archive.
        *this << isUnicode;
        *this << chars;

        Write(string.c_str(), chars * sizeof(WCHAR));
        return *this;
    }

    inline CArchive& CArchive::operator<<(const CString& string)
    {
        int chars = string.GetLength();
        bool isUnicode = (sizeof(TCHAR) == sizeof(WCHAR));

        // Store the Unicode state and number of characters in the archive.
        *this << isUnicode;
        *this << chars;

        Write(string.c_str(), chars * sizeof(TCHAR));
        return *this;
    }

    inline CArchive& CArchive::operator<<(POINT pt)
    {
        ArchiveObject ob(&pt, sizeof(pt));
        *this << ob;
        return *this;
    }

    inline CArchive& CArchive::operator<<(RECT rc)
    {
        ArchiveObject ob(&rc, sizeof(rc));
        *this << ob;
        return *this;
    }

    inline CArchive& CArchive::operator<<(SIZE sz)
    {
        ArchiveObject ob(&sz, sizeof(sz));
        *this << ob;
        return *this;
    }

    inline CArchive& CArchive::operator<<(const ArchiveObject& ao)
    {
        Write(&ao.m_size, sizeof(ao.m_size));
        Write(ao.m_pData, ao.m_size);
        return *this;
    }

    inline CArchive& CArchive::operator<<(const CObject& object)
    {
        const_cast<CObject&>(object).Serialize(*this);
        return *this;
    }

    // Load operators (>>)
    inline CArchive& CArchive::operator>>(BYTE& b)
    {
        ArchiveObject ob(&b, sizeof(b));
        *this >> ob;
        return *this;
    }

    inline CArchive& CArchive::operator>>(WORD& w)
    {
        ArchiveObject ob(&w, sizeof(w));
        *this >> ob;
        return *this;
    }

    inline CArchive& CArchive::operator>>(LONG& l)
    {
        ArchiveObject ob(&l, sizeof(l));
        *this >> ob;
        return *this;
    }

    inline CArchive& CArchive::operator>>(LONGLONG& ll)
    {
        ArchiveObject ob(&ll, sizeof(ll));
        *this >> ob;
        return *this;
    }

    inline CArchive& CArchive::operator>>(ULONGLONG& ull)
    {
        ArchiveObject ob(&ull, sizeof(ull));
        *this >> ob;
        return *this;
    }

    inline CArchive& CArchive::operator>>(DWORD& dw)
    {
        ArchiveObject ob(&dw, sizeof(dw));
        *this >> ob;
        return *this;
    }

    inline CArchive& CArchive::operator>>(float& f)
    {
        ArchiveObject ob(&f, sizeof(f));
        *this >> ob;
        return *this;
    }

    inline CArchive& CArchive::operator>>(double& d)
    {
        ArchiveObject ob(&d, sizeof(d));
        *this >> ob;
        return *this;
    }

    inline CArchive& CArchive::operator>>(int& i)
    {
        ArchiveObject ob(&i, sizeof(i));
        *this >> ob;
        return *this;
    }

    inline CArchive& CArchive::operator>>(short& i)
    {
        ArchiveObject ob(&i, sizeof(i));
        *this >> ob;
        return *this;
    }

    inline CArchive& CArchive::operator>>(char& c)
    {
        ArchiveObject ob(&c, sizeof(c));
        *this >> ob;
        return *this;
    }

    inline CArchive& CArchive::operator>>(wchar_t& ch)
    {
        ArchiveObject ob(&ch, sizeof(ch));
        *this >> ob;
        return *this;
    }

    inline CArchive& CArchive::operator>>(unsigned& u)
    {
        ArchiveObject ob(&u, sizeof(u));
        *this >> ob;
        return *this;
    }

    inline CArchive& CArchive::operator>>(bool& b)
    {
        ArchiveObject ob(&b, sizeof(b));
        *this >> ob;
        return *this;
    }

    inline CArchive& CArchive::operator>>(CStringA& string)
    {
        bool isUnicode = false;
        int chars = 0;
        constexpr int kMaxArchiveStringChars = 16 * 1024 * 1024;

        // Retrieve the Unicode state and number of characters from the archive.
        *this >> isUnicode;
        *this >> chars;

        if (chars < 0 ||
            static_cast<size_t>(chars) > kMaxArchiveStringChars ||
            static_cast<size_t>(chars) > GetRemainingBytes())
        {
            throw CFileException(m_filePath, GetApp()->MsgArReadFail());
        }
        if (isUnicode)
            throw CFileException(m_filePath, GetApp()->MsgArNotCStringA());

        Read(string.GetBuffer(chars), static_cast<size_t>(chars));
        string.ReleaseBuffer(chars);
        return *this;
    }
    inline CArchive& CArchive::operator>>(CStringW& string)
    {
        bool isUnicode = false;
        int chars = 0;
        constexpr size_t kMaxArchiveStringChars = 16u * 1024u * 1024u;

        // Retrieve the Unicode state and number of characters from the archive.
        *this >> isUnicode;
        *this >> chars;

        if (!isUnicode ||
            chars < 0 ||
            static_cast<size_t>(chars) > kMaxArchiveStringChars ||
            static_cast<size_t>(chars) > GetRemainingBytes() / sizeof(WCHAR))
        {
            throw CFileException(m_filePath, GetApp()->MsgArReadFail());
        }
        WCHAR* buffer = string.GetBuffer(chars);
        try
        {
            Read(buffer, static_cast<size_t>(chars * sizeof(WCHAR)));
            string.ReleaseBuffer(chars);
        }
        catch (...)
        {
            string.ReleaseBuffer(0);
            throw;
        }

        return *this;
    }

    inline CArchive& CArchive::operator>>(CString& string)
    {
        bool isUnicode = false;
        int chars = 0;
        constexpr size_t kMaxArchiveStringChars = 16u * 1024u * 1024u;
        *this >> isUnicode;
        *this >> chars;
        const ULONGLONG byteCount = isUnicode ?
            static_cast<size_t>(chars) * sizeof(WCHAR) : static_cast<size_t>(chars);
        
        if (chars < 0 ||
            static_cast<size_t>(chars) > kMaxArchiveStringChars ||
            byteCount > GetRemainingBytes())
        {
            throw CFileException(m_filePath, GetApp()->MsgArReadFail());
        }

        if (isUnicode)
        {
            std::vector<WCHAR> vWChar(size_t(chars) + 1, L'\0');
            WCHAR* buf = vWChar.data();

            Read(buf, static_cast<size_t>(chars) * 2);

#ifdef UNICODE
            memcpy(string.GetBuffer(chars), buf, size_t(chars) * 2);
#else
            WideCharToMultiByte(CP_ACP, 0, buf, chars, string.GetBuffer(chars), chars, nullptr, nullptr);
#endif

            string.ReleaseBuffer(chars);
        }
        else
        {
            std::vector<char> vChar(size_t(chars) + 1, '\0');
            char* buf = vChar.data();

            Read(buf, static_cast<size_t>(chars));
#ifdef UNICODE
            MultiByteToWideChar(CP_ACP, 0, buf, chars, string.GetBuffer(chars), chars);
#else
            memcpy(string.GetBuffer(chars), buf, chars);
#endif

            string.ReleaseBuffer(chars);
        }

        return *this;
    }

    inline CArchive& CArchive::operator>>(POINT& pt)
    {
        ArchiveObject ob(&pt, sizeof(pt));
        *this >> ob;
        return *this;
    }

    inline CArchive& CArchive::operator>>(RECT& rc)
    {
        ArchiveObject ob(&rc, sizeof(rc));
        *this >> ob;
        return *this;
    }

    inline CArchive& CArchive::operator>>(SIZE& sz)
    {
        ArchiveObject ob(&sz, sizeof(sz));
        *this >> ob;
        return *this;
    }

    inline CArchive& CArchive::operator>>(ArchiveObject& ao)
    {
        assert(m_pStream);
        UINT size;
        Read(&size, sizeof(size));
        if (size != ao.m_size)
        {
            throw CFileException(m_filePath, GetApp()->MsgArReadFail());
        }

        Read(ao.m_pData, ao.m_size);
        return *this;
    }

    inline CArchive& CArchive::operator>>(CObject& object)
    {
        object.Serialize(*this);
        return *this;
    }

} // namespace Win32xx

