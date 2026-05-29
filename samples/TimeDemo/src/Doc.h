/////////////////////////////////////////
// Doc.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef SDI_DOC_H
#define SDI_DOC_H

///////////////////////////////////////////////////////////////////////////////
// The document class. It is responsible for maintaining the document content,
// and for handling the file operations, such as opening and saving the
// document. It is derived from CObject, which provides the basic serialization
// functionality for the document.
class CDoc : public CObject
{
public:

    CDoc();
    virtual ~CDoc() override = default;

    int     GetDocLength() const;
    CString GetDocOpenFileName(const CString&) const;
    const CString&
            GetDocPath() const      { return m_docPath; }
    CString GetDocRecord(int, int left = 0, int length = -1) const;
    CString GetDocSaveFileName(const CString&) const;
    int     GetDocWidth() const;
    CTime   GetTimeFromStr(LPCWSTR szTime, int nDST = -1 ) const;
    bool    OpenDoc(const CString &);
    void    NewDocument();
    void    PushContent(const CString&);
    bool    SaveDoc();
    bool    SaveDocAs();
    CString Truth(bool b)           {return b ? L"true" : L"false";}

    // Public data members.
    static const    CString m_compiledOn;  // date, mmm dd yyyy

protected:
    virtual void    Serialize(CArchive& ar) override;

private:
    CString     m_docPath;
    int         m_docWidth;             // Length, in characters.
    LPCWSTR     m_fileDlgFilter;        // File dialog filter.
    std::vector<CString> m_docContent;  // Array of document lines.
};

#endif // SDI_DOC_H
