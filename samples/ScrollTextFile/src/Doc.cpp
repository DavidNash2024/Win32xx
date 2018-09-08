/* (02-Aug-2016) [Tab/Indent: 4/4][Line/Box: 80/74]                  (Doc.cpp) *
********************************************************************************
|                                                                              |
|                         robert.c.tausworthe@ieee.org                         |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the basic CDoc class for the 
    ScrollWin demo application using the Win32++ Windows interface classes,
    Copyright (c) 2005-2017 David Nash, under permissions granted therein.

 	Caveats: These materials are available under the same provisions as found 
	in the Win32++ copyright.txt notice.

    Programming Notes:
        The programming standards roughly follow those established by the 
    1997-1999 Jet Propulsion Laboratory Network Planning and Preparation 
    Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"

static  LPCTSTR file_dlg_filter = _T("Text Files (*.txt)\0*.txt\0")
                  _T("All Files (*.*)\0*.*\0\0");
  // tab spacing
const   int tabwidth    = 8;

  // latest file compilation date
CString CDoc::m_sCompiled_on = __DATE__;

  // a simple byte reversal function for UTF-16 BE
WCHAR ByteReverse(WCHAR w) { return (w >> 8) | ((w & 0xff) << 8);}

/*============================================================================*/
    CDoc::
CDoc()                                                                      /*

    Constructor.
*-----------------------------------------------------------------------------*/
{
    m_doclines.clear();
    m_stDoc_length = 0;
    m_stDoc_width = 0;
    m_lpszFile_dlg_filter = file_dlg_filter;
    m_bDoc_is_open = FALSE;
    m_open_doc_path.Empty();
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
    CStringW tabbing,
        spaces = L"                            "; // tabs <= 28
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
DetermineEncoding(const char *buffer, UINT testlen, UINT& offset)           /*

    Try to determine the file encoding using the testlen number of bytes read 
    in from the document file into the given buffer. Return the presumed
    encoding and the BOM offset, if any.
*----------------------------------------------------------------------------*/
{
      // if a short file, just report it as UTF8
    if (testlen < 3)
        return UTF8noBOM;

       // read the test length of the file contents
    m_fDoc_file.Read((void*)buffer, testlen);
    Encoding encoding = unknown;
      // look for Byte Order Mark (BOM)
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
            if (buffer[i] == 0 || buffer[i] > 0x7f)
            {     // encoding is not ANSI, assume UTF8
                encoding = UTF8noBOM;
                break;
            }
        }
        if (encoding == unknown)
            encoding = ANSI;
    }
    offset = (encoding == UTF16BE || encoding == UTF16LE ? 2 :
        (encoding == UTF8wBOM ? 3 : 0));
    return encoding;
}

/*============================================================================*/
    LPCTSTR CDoc::
GetFilter()                                                                 /*

    Return
*----------------------------------------------------------------------------*/
{
    return m_lpszFile_dlg_filter;
}

/*============================================================================*/
    UINT CDoc::
GetLength()                                                                 /*

    Return the document length, in records.
*----------------------------------------------------------------------------*/
{
    if (!m_bDoc_is_open)
        m_stDoc_length = 0;

    return m_stDoc_length = m_doclines.size();
}

/*============================================================================*/
    CSize CDoc::
GetMaxExtent(const CDC& dc)                                                 /*

    Return the maximum extent of all text lines in the document computed
    for the current font in the given dc and the line it occurred on, as
    a CPoint.
*----------------------------------------------------------------------------*/
{
    int h_extent = 0,
        v_extent = 0;
    for (UINT i = 0; i < GetLength(); i++)
    {
        CString s = m_doclines[i];
        CSize ex = dc.GetTextExtentPoint32(s.c_str(), s.GetLength());
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
IsOpen()                                                                    /*

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
        UINT testlen = 100,
             doclen  = static_cast<UINT>(m_fDoc_file.GetLength());
        if (doclen < testlen)
            testlen = doclen;
        std::vector<char> buffer(testlen);
        UINT offset = 0;
        Encoding encoding = DetermineEncoding(&buffer.front(), testlen, offset);
          // prepare to read in the file contents      
        m_fDoc_file.Seek(offset, FILE_BEGIN);
        m_doclines.clear();
        switch (encoding)
        {
            case ANSI:
            case UTF8wBOM:
            case UTF8noBOM:
                ReadABytes(encoding, doclen - offset);
                break;

            case UTF16LE:
            case UTF16BE:
                ReadWBytes(encoding, doclen - offset);
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
    m_stDoc_length = m_doclines.size();
    return ok;
}

/*============================================================================*/
    void CDoc::
ReadABytes(Encoding encoding, UINT doclen)                                  /*

    Read the document file in single byte mode and deposit lines in the
    document line string vector. Throw an exception if a failure occurs.
*-----------------------------------------------------------------------------*/
{
    CStringW entry;
    CStringA line;
    for (UINT fileloc = 0; fileloc < doclen; fileloc++)
    {
        CStringA a;
        UINT n = m_fDoc_file.Read(a.GetBuffer(1), 1);
        a.ReleaseBuffer();
        if (n != 1)
            throw CFileException(_T("File read error."));

        if (a == "\r")
            continue;

            // check for end of line or end of string           
        if (a == "\n" || a.IsEmpty())
        {
                // enter the record, translate if UTF-8 encoded
            if (encoding == UTF8wBOM || encoding == UTF8noBOM)
                entry = CAtoW(line, CP_UTF8);
            else
                entry = AtoW(line);
            AddRecord(entry);
            line.Empty();
            m_stDoc_width = MAX(m_stDoc_width, 
                static_cast<UINT>(entry.GetLength()));
        }
        else
            line += a;
    }
      // if there is a partial line, add it to the document
    if (line.GetLength() > 0)
    {
        entry = AtoW(line);
        AddRecord(entry);
    }
}

/*============================================================================*/
    void CDoc::
ReadWBytes(Encoding encoding, UINT doclen)                                 /*

    Read the file in wide character mode and deposit lines in the document
    line string vector. Throw an exception if a failure occurs.
*-----------------------------------------------------------------------------*/
{
    CStringW entry;
    for (UINT fileloc = 0; fileloc < doclen; fileloc += 2)
    {
        CStringW w;
        UINT n = m_fDoc_file.Read(w.GetBuffer(2), 2);
        w.ReleaseBuffer();
        if (n != 2)
            throw CFileException(_T("File read error."));
 
        if (encoding == UTF16BE)
            w[0] = ByteReverse(w[0]);
        if (w == L"\r")
            continue;

            // check for end of line or end of string           
        if (w == L"\n" || w.IsEmpty())
        {
            AddRecord(entry);
            m_stDoc_width = MAX(m_stDoc_width, 
                static_cast<UINT>(entry.GetLength()));
            entry.Empty();
        }
        else 
            entry += w;
    }
      // if there remains a partial entry, add it to the document
    if (entry.GetLength() > 0)
        AddRecord(entry);
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

