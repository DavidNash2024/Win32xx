//////////////////////////////////
// Doc.h

#ifndef WIN32XX_DOC_H
#define WIN32XX_DOC_H


#include "targetver.h"
#include "wxx_wincore.h"
#include "wxx_archive.h"
#include "wxx_printdialogs.h"


struct PlotPoint
{
    int x;
    int y;
    bool isPenDown;
    COLORREF penColor;
};


//////////////////////////////////////////////////////////////
// CDoc holds the application's data. It inherits from CObject
// to perform data serialization to and from the archive.
class CDoc : public CObject
{
public:
    CDoc() {}
    virtual ~CDoc() {}

    std::vector<PlotPoint>& GetAllPoints() {return m_points;}   // returns a vector of PlotPoint data
    void FileOpen(LPCTSTR fileName);
    void FileSave(LPCTSTR fileName);
    void Serialize(CArchive &ar);
    void StorePoint(int x, int y, bool isPenDown, COLORREF PenColor);

private:
    std::vector<PlotPoint> m_points;    // Points of lines to draw
};


#endif  // WIN32XX_DOC_H