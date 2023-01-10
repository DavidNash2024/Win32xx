/////////////////////////////
// Doc.cpp
//

#include "stdafx.h"
#include "Doc.h"

/////////////////////////////
// CDoc function definitions.
//
void CDoc::FileOpen(LPCTSTR filename)
// Loads the plotpoint data from the archive.
// Throws an exception if unable to read the file.
{
    GetAllPoints().clear();
    CArchive ar(filename, CArchive::load);
    ar >> *this;
}

void CDoc::FileSave(LPCTSTR filename)
// Stores the plotpoint data in the archive.
// Throws an exception if unable to save the file.
{
    CArchive ar(filename, CArchive::store);
    ar << *this;
}

void CDoc::Serialize(CArchive &ar)
// Uses CArchive to stream data to or from a file
{

    if (ar.IsStoring())
    {
        // Store the number of points.
        UINT points = UINT(GetAllPoints().size());
        ar << points;

        // Store the PlotPoint data.
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

        // Load the number of points.
        ar >> points;

        // Load the PlotPoint data.
        for (UINT u = 0; u < points; ++u)
        {
            ArchiveObject ao( &pp, sizeof(pp) );
            ar >> ao;
            GetAllPoints().push_back(pp);
        }
    }

}

void CDoc::StorePoint(int x, int y, bool isPenDown, COLORREF penColor)
{
    PlotPoint pp;
    pp.x = x;
    pp.y = y;
    pp.isPenDown = isPenDown;
    pp.color = penColor;

    // Add the point to the vector.
    m_points.push_back(pp);
}