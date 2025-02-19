/////////////////////////////
// Doc.h
//

#ifndef WIN32XX_DOC_H
#define WIN32XX_DOC_H


///////////////////////////////////////
// Declaration of the PlotPoint struct.
//
struct PlotPoint
{
    int x;
    int y;
    bool isPenDown;
    COLORREF color;
};


//////////////////////////////////////////////////////////////
// CDoc holds the application's data. It inherits from CObject
// to perform data serialization to and from the archive.
class CDoc : public CObject
{
public:
    CDoc() = default;
    virtual ~CDoc() override = default;
    std::vector<PlotPoint>& GetAllPoints() { return m_points; }   // returns a vector of PlotPoint data
    void FileOpen(LPCWSTR filename);
    void FileSave(LPCWSTR filename);
    void StorePoint(int x, int y, bool isPenDown, COLORREF penColor);

protected:
    // Virtual functions that override base class functions.
    void Serialize(CArchive &ar) override;

private:
    std::vector<PlotPoint> m_points;    // Points of lines to draw.
};

#endif  // WIN32XX_DOC_H
