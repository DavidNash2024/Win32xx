

#ifndef WIN32XX_DOC_H
#define WIN32XX_DOC_H


// CDoc holds the application's data. It inherits from CObject
// to perform data serialization to and from the archive.

class CDoc : public CObject
{
public:
    CDoc() {}
    ~CDoc() {}

    void FileLoad(LPCTSTR filename);
    void FileStore(LPCTSTR filename);
    void Serialize(CArchive& ar);

private:

};

#endif  // WIN32XX_DOC_H