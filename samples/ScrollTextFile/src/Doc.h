/* (02-Aug-2016) [Tab/Indent: 4/4][Line/Box: 80/74]                    (Doc.h) *
********************************************************************************
|                                                                              |
|                         robert.c.tausworthe@ieee.org                         |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the basic CDoc class for the ScrollWin 
    demo application using the Win32++ Windows interface classes, Copyright (c)
    2005-2017 David Nash, under permissions granted therein.

    Caveats: These materials are available under the same provisions as found 
    in the Win32++ copyright.txt notice.

    Programming Notes:
        The programming standards roughly follow those established by the 
    1997-1999 Jet Propulsion Laboratory Network Planning and Preparation 
    Subsystem project for C++ programming.

*******************************************************************************/

#ifndef SDI_DOC_H
#define SDI_DOC_H

  // the following encodings are supported
enum Encoding
{
    unknown,
    ANSI,
    UTF8noBOM,
    UTF8wBOM,
    UTF16LE,
    UTF16BE,
    UnicodeNoBOM
};

/*============================================================================*/
    class 
CDoc                                                                        /*

*-----------------------------------------------------------------------------*/
{
    public:

        CDoc();
        virtual ~CDoc();
        virtual void    AddRecord(const CStringW& entry);
        virtual CString GetFilter() const;
        virtual UINT    GetLength();
        virtual CSize   GetMaxExtent(const CDC& dc);
        virtual CStringW GetRecord(UINT, UINT left = 0, 
                            UINT length = MAX_STRING_SIZE);
        virtual UINT    GetWidth();
        virtual BOOL    IsOpen() const;
        virtual BOOL    CloseDoc();
        virtual BOOL    OpenDoc(LPCTSTR);
        virtual void    Serialize(CArchive& ar);

          // public data members
        static CString  m_sCompiled_on;  // compilation date, mmm dd yyyy

    private:
        Encoding    DetermineEncoding(UINT testlen, UINT& offset);
        void        ReadABytes(Encoding encoding, UINT doclen, UINT offset);
        void        ReadWBytes(Encoding encoding, UINT doclen, UINT offset);
        void        RecordEntry(wchar_t w, CStringW& entry);

          // private data members
        CFile   m_fDoc_file;     // holds the document name, path, etc.
        BOOL    m_bDoc_is_dirty; // document has been altered since open
        BOOL    m_bDoc_is_open;  // the document status
        UINT    m_stDoc_width;   // width, in characters
        UINT    m_stDoc_length;  // length, in records
        CString m_open_doc_path; // empty if closed
        std::vector<CStringW>   m_doclines; // the lines in the document
        std::vector<char>       m_buffer;   // the file contents buffer
};
/*-----------------------------------------------------------------------------*/
#endif //SDI_DOC_H
