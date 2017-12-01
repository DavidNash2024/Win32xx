/* (02-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                  (Doc.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2017, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                         robert.c.tausworthe@ieee.org                         |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the basic CDoc class for the
    ScrollWin demo application using the Win32++ Windows interface classes,
    Copyright c) 2005-2017 David Nash, under permissions granted therein.

        Caveats: The copyright displayed above extends only to the author's
    original contributions to the subject class, and to the alterations,
    additions, deletions, and other treatments of materials that may have
    been extracted from the cited sources.  Unaltered portions of those
    materials retain their original copyright status. The author hereby
    grants permission to any person obtaining a copy of this treatment
    of the subject class and any associated documentation composed by
    the author, to utilize this material, free of charge and without
    restriction or limitation, subject to the following conditions:

        The above copyright notice, as well as that of David Nash
        and Win32++, together with the respective permission
        conditions shall be included in all copies or substantial
        portions of this material so copied, modified, merged,
        published, distributed, or otherwise held by others.

    These materials are provided "as is", without warranty of any kind,
    express or implied, including but not limited to: warranties of
    merchantability, fitness for a particular purpose, and non-infringement.
    In no event shall the authors or copyright holders be liable for any
    claim, damages, or other liability, whether in an action of contract,
    tort or otherwise, arising from, out of, or in connection with, these
    materials, the use thereof, or any other other dealings therewith.

    Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Network Planning
        and Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"

static  LPCTSTR file_dlg_filter = _T("Text Files (*.txt)\0*.txt\0")
                  _T("All Files (*.*)\0*.*\0\0");
  // tab spacing
const   int tabwidth    = 8;

  // latest file compilation date
CString CDoc::m_sCompiled_on = __DATE__;

/*============================================================================*/
    CDoc::
CDoc()                                  /*

    Constructor.
*-----------------------------------------------------------------------------*/
    :   m_fDoc_file(INVALID_HANDLE_VALUE)
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
~CDoc()                                 /*

    Destructor
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
    void CDoc::
AddRecord(const CString& entry)                     /*

    Enter the entry record into the m_doclines list. Expand tab characters
    with spaces to conform with tabwidth spacing.
*-----------------------------------------------------------------------------*/
{
    CString final = entry;
    int tab = 0;
    CString tabbing,
        spaces = _T("                            "); // tabs <= 28
    while ((tab = final.Find(_T('\t'))) != -1)
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
CloseDoc()                              /*

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
    LPCTSTR CDoc::
GetFilter()                             /*

    Return
*----------------------------------------------------------------------------*/
{
    return m_lpszFile_dlg_filter;
}

/*============================================================================*/
    UINT CDoc::
GetLength()                             /*

    Return the document length, in records.
*----------------------------------------------------------------------------*/
{
    if (!m_bDoc_is_open)
        m_stDoc_length = 0;

    return m_stDoc_length = m_doclines.size();
}

/*============================================================================*/
    CSize CDoc::
GetMaxExtent(const CDC& dc)                     /*

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
    CString CDoc::
GetRecord(UINT rcd, UINT left /* = 0 */, UINT length /* = MAX_STRING_SIZE */)
                                    /*
    Return a CString containing the document rcd record, starting at
    the left position and continuing for length characters.
*----------------------------------------------------------------------------*/
{
    if (!m_bDoc_is_open || m_stDoc_length == 0)
        return _T("");

    CString rtn = m_doclines[rcd];
    UINT rtnlen =  rtn.GetLength();
    long maxlen = (long)rtnlen - (long)left;
    if (maxlen <= 0 || left > rtnlen)
        return _T("");
        
    if ((long)length > maxlen)
            return rtn.Mid(left);
            
    return rtn.Mid(left, length);
}

/*============================================================================*/
    UINT CDoc::
GetWidth()                              /*

    Return the document width, in characters
*----------------------------------------------------------------------------*/
{
    if (!m_bDoc_is_open)
        m_stDoc_width = 0;

    return m_stDoc_width;
}

/*============================================================================*/
    BOOL CDoc::
IsOpen()                                /*

    Return TRUE if the document has been loaded successfully, BOOL
    otherwise.
*----------------------------------------------------------------------------*/
{
    return m_bDoc_is_open;
}

/*============================================================================*/
    BOOL CDoc::
OpenDoc(const CString& file)                        /*

    Open the document from the given file. Previous state parameters that
    were serialized in the prior execution will have already been loaded.
*-----------------------------------------------------------------------------*/
{
    if (file.IsEmpty())
        return FALSE;
    
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
    BOOL ok = FALSE;
    try
    {
        m_fDoc_file.Open(file, OPEN_EXISTING | CFile::modeRead);
        m_bDoc_is_open = TRUE;
        m_open_doc_path = m_fDoc_file.GetFilePath();
        m_stDoc_width = 0;
        CStringA a;
        CStringW w;
        CString entry, t;
        UINT length = static_cast<UINT>(m_fDoc_file.GetLength());
        if (length > 2)
        {
            m_fDoc_file.Read(a.GetBuffer(2), 2);
            a.ReleaseBuffer(2);
        }
        else
            a[0] = a[1] = 1;
        BOOL UnicodeFile = (a[0] == 0 || a[1] == 0);
        m_fDoc_file.SeekToBegin();
        UINT charsize = (UnicodeFile ? sizeof(WCHAR) : sizeof(CHAR)),
             nchars  = length / charsize;
        m_doclines.clear();
        for (UINT fileloc = 0; fileloc < nchars; fileloc++)
        {
            if(UnicodeFile)
            {
                UINT n = m_fDoc_file.Read(w.GetBuffer(charsize),
                    charsize);
                w.ReleaseBuffer(charsize);
                if (n != charsize)
                    throw CUserException(_T("Read error."));

                t = WtoT(w);
            }
            else // ANSI file
            {
                UINT n = m_fDoc_file.Read(a.GetBuffer(charsize),
                    charsize);
                a.ReleaseBuffer(charsize);
                if (n != charsize)
                    throw CUserException(_T("Read error."));

                t = AtoT(a);

            }
            if (t[0] == _T('\n') || t[0] == _T('\0'))
            {
                AddRecord(entry);
                m_stDoc_width = MAX(m_stDoc_width, 
                    static_cast<UINT>(entry.GetLength()));
                entry.Empty();
            }
            else if (t[0] == _T('\r'))
                continue;

            else 
                entry += t;
        }
          // if there is an as-yet unentered record, add it now
        if (entry.GetLength() > 0)
            AddRecord(entry);
        ok = TRUE;
    }
    catch (...)
    {
        ok = m_bDoc_is_open = FALSE;
        m_open_doc_path.Empty();
    }
    m_fDoc_file.Close();
    m_stDoc_length = m_doclines.size();
    return ok;
}

/*============================================================================*/
        void CDoc::
Serialize(CArchive& ar)                                                 /*

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

