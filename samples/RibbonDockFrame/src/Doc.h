/////////////////////////////
// Doc.h
//

#ifndef WIN32XX_DOC_H
#define WIN32XX_DOC_H

struct PlotPoint;


//////////////////////////////////////////////////////////////
// Declaration of the CDoc Class
// CDoc holds the application's data. It inherits from CObject
// to perform data serialization to and from the archive.
//
class CDoc : public CObject
{
public:
    CDoc() {}
    virtual ~CDoc() override {}

    std::vector<PlotPoint>& GetAllPoints() { return m_points; }
    BOOL FileOpen(LPCWSTR filename);
    BOOL FileSave(LPCWSTR filename);
    void StorePoint(PlotPoint& pp);

protected:
    // Virtual functions that override base class functions
    void Serialize(CArchive &ar) override;

private:
    std::vector<PlotPoint> m_points;    // Points of lines to draw
};

#endif  // WIN32XX_DOC_H