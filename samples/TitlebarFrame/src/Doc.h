/////////////////////////////
// Doc.h
//

#ifndef WIN32XX_DOC_H
#define WIN32XX_DOC_H


//////////////////////////////////////////////////////////////
// CDoc holds the application's data. It inherits from CObject
// to perform data serialization to and from the archive.
class CDoc : public CObject
{
public:
    CDoc() = default;
    virtual ~CDoc() override = default;
    void FileLoad(LPCWSTR filename);
    void FileStore(LPCWSTR filename);

protected:
    // Virtual functions that override base class functions
    virtual void Serialize(CArchive& ar) override;
};

#endif  // WIN32XX_DOC_H