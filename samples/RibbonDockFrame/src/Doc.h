/////////////////////////////
// Doc.h
//

#ifndef WIN32XX_DOC_H
#define WIN32XX_DOC_H

struct PlotPoint;


//////////////////////////////////////////////////////////////
// Declartion of the CDoc Class
// CDoc holds the application's data. It inherits from CObject
// to perform data serialization to and from the archive.
//
class CDoc : public CObject
{
public:
    CDoc() {}
    virtual ~CDoc() {}

    std::vector<PlotPoint>& GetAllPoints() {return m_points;}
    BOOL FileOpen(LPCTSTR filename);
    BOOL FileSave(LPCTSTR filename);
    void Serialize(CArchive &ar);
    void StorePoint(PlotPoint& pp);

private:
    std::vector<PlotPoint> m_points;    // Points of lines to draw
};

#endif  // WIN32XX_DOC_H