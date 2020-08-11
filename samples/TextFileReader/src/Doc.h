/* (02-Aug-2016) [Tab/Indent: 4/4][Line/Box: 80/74]                    (Doc.h) *
********************************************************************************
|                                                                              |
|                Authors: Robert Tausworthe, David Nash, 2020                  |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the CDoc class for this sample program
    using the Win32++ Windows interface classes.

    Programming Notes: The programming standards roughly follow those
    established by the 1997-1999 Jet Propulsion Laboratory Network Planning
    and Preparation Subsystem project for C++ programming.

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

    This class is the interface between the file to be displayed and the CView
    class, which displays it. Parameters of this class deemed to be persistent
    may be serialized.
*-----------------------------------------------------------------------------*/
{
    public:

        CDoc();
        virtual ~CDoc(){}

        void        AddRecord(const CStringW& entry);
        UINT        GetLength();
        CStringW    GetRecord(UINT, UINT left = 0, UINT length = MAX_STRING_SIZE);
        UINT        GetWidth();
        BOOL        IsOpen() const;
        BOOL        CloseDoc();
        BOOL        OpenDoc(LPCTSTR);

    private:
        WCHAR       ByteReverse(WCHAR w) // byte reversal function for UTF-16 BE
                        { return (w >> 8) | ((w & 0xff) << 8); }

        Encoding    DetermineEncoding(UINT testlen, UINT& offset);
        void        ReadABytes(Encoding encoding, UINT doclen, UINT offset);
        void        ReadWBytes(Encoding encoding, UINT doclen, UINT offset);
        void        RecordEntry(wchar_t w, CStringW& entry);

        CString     m_compiledOn;           // compilation date, mmm dd yyyy
        CFile       m_file;                 // the document file object
        BOOL        m_isOpen;               // the document status
        UINT        m_width;                // width, in characters
        CString     m_openPath;             // empty when closed
        std::vector<CStringW>   m_doclines; // the document content
        std::vector<char>       m_buffer;   // the file contents buffer
};
/*-----------------------------------------------------------------------------*/
#endif //SDI_DOC_H
