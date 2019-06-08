

#include "Doc.h"
#include "ScribbleApp.h"


// Loads the plotpoint data from the archive.
// Throws an exception if unable to read the file.
void CDoc::FileOpen(LPCTSTR filename)
{
    GetAllPoints().clear();
    CArchive ar(filename, CArchive::load);
    ar >> *this;
}


// Stores the plotpoint data in the archive.
// Throws an exception if unable to save the file.
void CDoc::FileSave(LPCTSTR filename)
{
    CArchive ar(filename, CArchive::store);
    ar << *this;
}


// Uses CArchive to stream data to or from a file.
void CDoc::Serialize(CArchive &ar)
{

    if (ar.IsStoring())
    {
        // Store the number of points
        UINT points = UINT(GetAllPoints().size());
        ar << points;
        
        // Store the PlotPoint data
        std::vector<PlotPoint>::iterator iter;
        for (iter = GetAllPoints().begin(); iter < GetAllPoints().end(); ++iter)
        {
            ArchiveObject ao( &(*iter), sizeof(PlotPoint) );
            ar << ao;
        }
    }
    else
    {
        UINT points;
        PlotPoint pp;
        GetAllPoints().clear();

        // Load the number of points
        ar >> points;

        // Load the PlotPoint data
        for (UINT u = 0; u < points; ++u)
        {
            ArchiveObject ao( &pp, sizeof(pp) );
            ar >> ao;
            GetAllPoints().push_back(pp);
        }
    }

}


// Store the specified point information.
void CDoc::StorePoint(int x, int y, bool isPenDown, COLORREF penColor)
{
    PlotPoint pp;
    pp.x = x;
    pp.y = y;
    pp.isPenDown = isPenDown;
    pp.penColor = penColor;

    m_points.push_back(pp); //Add the point to the vector
}