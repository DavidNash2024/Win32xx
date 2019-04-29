/* (02-Aug-2016) [Tab/Indent: 4/4][Line/Box: 80/74]                  (Doc.cpp) *
********************************************************************************
|                                                                              |
|                         robert.c.tausworthe@ieee.org                         |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the basic CDoc class for the
    ScrollWin demo application using the Win32++ Windows interface classes,
    Copyright (c) 2005-2017 David Nash, under permissions granted therein.

    The embodyment of a document in this class is in the form of a vector of
    UTF-16 Little Endean Unicode strings, one for each line of the document,
    whether compiled in ANSI or Unicode mode. It therefore requires a CView
    class capable of displaying this encoding.

    Caveats: These materials are available under the same provisions as found
    in the Win32++ copyright.txt notice.

    Programming Notes:
        The programming standards roughly follow those established by the
    1997-1999 Jet Propulsion Laboratory Network Planning and Preparation
    Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"

  // tab spacing
const   int tabwidth = 8;

  // latest file compilation date
CString CDoc::m_sCompiled_on = __DATE__;

  // a simple byte reversal function for UTF-16 BE
WCHAR ByteReverse(WCHAR w) { return (w >> 8) | ((w & 0xff) << 8);}

/*============================================================================*/
    CDoc::
CDoc()                                                                      /*

    Constructor.
*-----------------------------------------------------------------------------*/
    :   m_bDoc_is_dirty(FALSE), m_bDoc_is_open(FALSE), m_stDoc_width(0),
        m_stDoc_length(0)
{
    m_doclines.clear();
}

/*============================================================================*/
    CDoc::
~CDoc()                                                                     /*

    Destructor
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
    void CDoc::
AddRecord(const CStringW& entry)                                             /*

    Enter the entry record into the m_doclines list. Expand tab characters
    with spaces to conform with tabwidth spacing.
*-----------------------------------------------------------------------------*/
{
    CStringW final = entry;
    int tab = 0;
    CStringW tabbing;
    CStringW spaces = L"                            "; // tabs <= 28
    while ((tab = final.Find(L'\t')) != -1)
    {
        final.Delete(tab, 1);
        int nspaces = tabwidth - (tab % tabwidth);
        tabbing = (nspaces == 0 ?
            spaces.Left(tabwidth) : spaces.Left(nspaces));
        final.Insert(tab, tabbing);
    }
    m_doclines.push_back(final);
}

/*============================================================================*/
    BOOL CDoc::
CloseDoc()                                                                  /*

    Perform any cleanup necessary to close the document, except for
    serialization chores, which are performed separately, in the
    Serialize() member.
*-----------------------------------------------------------------------------*/
{
    m_bDoc_is_open = FALSE;
    m_stDoc_length = m_stDoc_width = 0;
    m_doclines.clear();
    return TRUE;
}

/*============================================================================*/
    Encoding    CDoc::
DetermineEncoding(UINT testlen, UINT& offset)                               /*

    Try to determine the file encoding using the first testlen bytes in the
    file image m_buffer. Return the presumed encoding and the BOM offset,
    if any.
*----------------------------------------------------------------------------*/
{
      // if a short file, just report it as UTF8
    if (testlen < 3)
        return UTF8noBOM;

    Encoding encoding = ANSI;
    char* buffer = &m_buffer[0];
      // look for a Byte Order Mark (BOM)
    unsigned char b0 = buffer[0], b1 = buffer[1], b2 = buffer[2];
    if (b0 == 0xfe && b1 == 0xff)
        encoding = UTF16BE;
    else if (b0 == 0xff && b1 == 0xfe)
        encoding = UTF16LE;
    else if (b0 == 0xef && b1 == 0xbb && b2 == 0xbf)
        encoding = UTF8wBOM;
        // check for Unicode w/o BOM
    else if (::IsTextUnicode(buffer, testlen, NULL) == 1)
        encoding = UnicodeNoBOM;
    else
    {     // check for non ANSI characters
        for (UINT i = 0; i < testlen; i++)
        {
            unsigned char b = buffer[i];
            if (b == 0 || b > 0x7f)
            {     // encoding is not ANSI, assume UTF8
                encoding = UTF8noBOM;
                break;
            }
        }
    }
    offset = (encoding == UTF16BE || encoding == UTF16LE ? 2 :
        (encoding == UTF8wBOM ? 3 : 0));
    return encoding;
}

/*============================================================================*/
    CString CDoc::
GetFilter()   const                                                              /*

    Return
*----------------------------------------------------------------------------*/
{
    return LoadString(IDS_FILE_FILTER);
}

/*============================================================================*/
    UINT CDoc::
GetLength()                                                                 /*

    Return the document length, in records.
*----------------------------------------------------------------------------*/
{
    if (!m_bDoc_is_open)
        m_stDoc_length = 0;

    return m_stDoc_length = static_cast<UINT>(m_doclines.size());
}

/*============================================================================*/
    CSize CDoc::
GetMaxExtent(const CDC& dc)                                              /*

    Return the maximum extent of all text lines in the document computed
    for the current font in the given dc and the line it occurred on, as
    a CPoint.
*----------------------------------------------------------------------------*/
{
    int h_extent = 0;
    int v_extent = 0;
    for (UINT i = 0; i < GetLength(); i++)
    {
        CStringW s = m_doclines[i];
        CSize ex;
        ::GetTextExtentPoint32W(dc, s.c_str(), s.GetLength(), &ex);
        h_extent = MAX(ex.cx, h_extent);
        v_extent +=  ex.cy;
    }
    return CSize(h_extent, v_extent);
}

/*============================================================================*/
    CStringW CDoc::
GetRecord(UINT rcd, UINT left /* = 0 */, UINT length /* = MAX_STRING_SIZE */) /*

    Return a CString containing the document rcd record, starting at
    the left position and continuing for length characters.
*-----------------------------------------------------------------------------*/
{
    if (!m_bDoc_is_open || m_stDoc_length == 0)
        return L"";

    CStringW rtn = m_doclines[rcd];
    UINT rtnlen =  rtn.GetLength();
    long maxlen = (long)rtnlen - (long)left;
    if (maxlen <= 0 || left > rtnlen)
        return L"";

    if ((long)length > maxlen)
            return rtn.Mid(left);

    return rtn.Mid(left, length);
}

/*============================================================================*/
    UINT CDoc::
GetWidth()                                                                  /*

    Return the document width, in characters
*-----------------------------------------------------------------------------*/
{
    if (!m_bDoc_is_open)
        m_stDoc_width = 0;

    return m_stDoc_width;
}

/*============================================================================*/
    BOOL CDoc::
IsOpen() const                                                                   /*

    Return TRUE if the document has been loaded successfully, BOOL
    otherwise.
*----------------------------------------------------------------------------*/
{
    return m_bDoc_is_open;
}

/*============================================================================*/
    BOOL CDoc::
OpenDoc(LPCTSTR filename)                                                   /*

    Open the document from the given filename and populate the record array.
    State parameters that were serialized in the prior execution will have
    already been loaded.
*-----------------------------------------------------------------------------*/
{
    BOOL ok = FALSE; // declare default return value
    try
    {
        if (!filename)
            throw CUserException(_T("No file name was given."));

        CString file = filename;
        if (file.IsEmpty())
           throw CUserException(_T("No file name was given."));

          // if there is currently a document open, close it if different
        if (IsOpen())
        {
            CString msg;
            if (file.CompareNoCase(m_open_doc_path) == 0)
            {
                msg.Format(_T("Document file\n'%s'\nis already open."),
                    m_open_doc_path.c_str());
                ::MessageBox(NULL, msg, _T("Information"), MB_OK |
                    MB_ICONINFORMATION | MB_TASKMODAL);
                  // not deemed a failure, as the file is open, as specified
                return TRUE;
            }
            else
                CloseDoc();
        }
          // ok, now open the file, set the open flag, and record the path
        m_fDoc_file.Open(file, OPEN_EXISTING | CFile::modeRead);
        m_bDoc_is_open = TRUE;
        m_open_doc_path = m_fDoc_file.GetFilePath();
          // try to determine whether the file is Unicode
          // use a test length of characters from the file
        UINT testlen = 100;
        UINT doclen  = static_cast<UINT>(m_fDoc_file.GetLength());
        if (doclen < testlen)
            testlen = doclen;
          // resize the file buffer to fit content
        m_buffer.resize(doclen);
          // read the entire file contents into the buffer
        UINT n = m_fDoc_file.Read((void*)&m_buffer[0], doclen);
        if (n != doclen)
            throw CFileException(_T("Reading the file failed."));

        UINT offset = 0;
        Encoding encoding = DetermineEncoding(testlen, offset);
        m_doclines.clear();
        switch (encoding)
        {
            case ANSI:
            case UTF8wBOM:
            case UTF8noBOM:
                ReadABytes(encoding, doclen, offset);
                break;

            case UTF16LE:
            case UTF16BE:
                ReadWBytes(encoding, doclen, offset);
                break;

            default:
                assert(TRUE);
        }
        ok = TRUE;
    }
    catch (CException &e)
    {
        CString msg;
        msg.Format(_T("File could not be opened and read:\n\n%s"),
            e.GetText());
        ::MessageBox(NULL, msg, _T("Error"), MB_OK | MB_ICONEXCLAMATION |
            MB_TASKMODAL);
        ok = m_bDoc_is_open = FALSE;
        m_open_doc_path.Empty();
    }
    m_fDoc_file.Close();
    m_stDoc_length = static_cast<UINT>(m_doclines.size());
    return ok;
}

/*============================================================================*/
    void CDoc::
ReadABytes(Encoding encoding, UINT docsize, UINT offset)                    /*

    Read the document file in single byte mode and deposit lines in the
    document line string vector. Throw an exception if a failure occurs.
*-----------------------------------------------------------------------------*/
{
      // add a terminating 0 to the buffer
    m_buffer.resize(docsize + 1, '\0');
      // determine the text conversion code page to use
    UINT CPage = ((encoding == UTF8wBOM || encoding == UTF8noBOM) ?
        CP_UTF8 : CP_ACP);
      // convert the file buffer to wide chars using this code page
    CAtoW wb(&m_buffer[0] + offset, CPage);
      // get access to the converted array and its length
    LPCWSTR pWStr = wb;
    UINT length   = static_cast<UINT>(wcslen(pWStr));
      // declare the entry string that will be added to the document and scan
      // the converted array for end-of-line characters
    CStringW entry;
    for (UINT i = 0; i < length; i++)
    {
        wchar_t w = pWStr[i];
        RecordEntry(w, entry) ;
    }
      // if there is a partial line, add it to the document
    if (entry.GetLength() > 0)
        AddRecord(entry);
}

/*============================================================================*/
    void CDoc::
ReadWBytes(Encoding encoding, UINT doclen, UINT offset)                     /*

    Read the file buffer in wide character mode and deposit lines in the
    document line string vector. Throw an exception if a failure occurs.
*-----------------------------------------------------------------------------*/
{
    UINT length = doclen - offset;
    UINT size   = length / 2;
      // copy the file character buffer into a wide-character array
    std::vector<wchar_t> wideArray(size + 1, L'\0');
    memcpy(&wideArray[0], &m_buffer[0] + offset, length);
      // declare the entry string that will be added to the document and scan
      // the wide array for end-of-line characters
    CStringW entry;
    for (UINT i = 0; i < size; i++)
    {
        wchar_t w = wideArray[i];
          // if big-endian, reverse w's bytes
        if (encoding == UTF16BE)
            w = ByteReverse(w);

        RecordEntry(w, entry) ;
    }
      // if there remains a partial entry, add it to the document
    if (entry.GetLength() > 0)
        AddRecord(entry);
}

/*============================================================================*/
    void CDoc::
RecordEntry(wchar_t w, CStringW& entry)                                     /*

    If w is not an end-of-line or end_of_string character, append it to the
    entry string. If it is an end-=of-line or end-of_string character, then
    commit the entry into the document record array.
*-----------------------------------------------------------------------------*/
{
      // skip return characters, if any
    if (w == L'\r')
        return;

      // break on end-of-line or end-of-string
    if (w == L'\n' || w == L'\0')
    {
          // enter the record
        AddRecord(entry);
          // record the longest length
        m_stDoc_width = MAX(m_stDoc_width,
            static_cast<UINT>(entry.GetLength()));
        entry.Empty();
    }
    else
        entry += w;
}
/*============================================================================*/
    void CDoc::
Serialize(CArchive& ar)                                                     /*

        Called to serialize the document to or deserialize it from the
    archive ar, depending on the sense of IsStoring().  Leaves the
    archive open for for further operations.
*-----------------------------------------------------------------------------*/
{
      // perform loading or storing
    if (ar.IsStoring())
    {
        // each item serialized is written to the archive
        // file as a char stream of the proper length,
        // preceded by that length. In some cases, other forms os
        // data are saved, from which the primary items are then
        // reconstructed.

    }
    else    // recovering
    {
        // each item deserialized from the archive is
        // retrieved by first reading its length and then
        // loading in that number of bytes into the data
        // item saved in the archive, as above. Some items require
        // additional converstion procedures.
    }
}

